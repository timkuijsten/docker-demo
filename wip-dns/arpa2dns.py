#!/usr/bin/env python
#
# arpa2dns -- Manage data published in DNS
#
# From: Rick van Rein <rick@openfortress.nl>


import os
import sys
import uuid
import string
import re

import arpa2cmd
import cmd
from cmdparser import cmdparser

import tightknot

import json


#
# This uses the Python library to control Knot DNS.
# Its document includes a decent example:
#
#     Example:
#         import json
#         from libknot.control import *
#     
#         #load_lib("/usr/lib/libknot.so")
#     
#         ctl = KnotCtl()
#         ctl.connect("/var/run/knot/knot.sock")
#     
#         try:
#             ctl.send_block(cmd="conf-begin")
#             resp = ctl.receive_block()
#     
#             ctl.send_block(cmd="conf-set", section="zone", item="domain", data="test")
#             resp = ctl.receive_block()
#     
#             ctl.send_block(cmd="conf-commit")
#             resp = ctl.receive_block()
#     
#             ctl.send_block(cmd="conf-read", section="zone", item="domain")
#             resp = ctl.receive_block()
#             print(json.dumps(resp, indent=4))
#         finally:
#             ctl.send(KnotCtlType.END)
#             ctl.close()
#



#TODO# BE LAZY, BOOT KNOTD FROM HERE
os.system ("/etc/init.d/knot start")
import time
time.sleep (2)



#TODO# BE LAZY, START THINGS FROM HERE

knot = tightknot.TightKnot ()

knot.have_conf ()
# knot.knot (cmd='conf-set', section='zone', item='domain', data='orvelte.nep')
knot.add_zone ('orvelte.nep')
knot.try_commit ()

knot.have_zones ('orvelte.nep')
knot.add_rr ('orvelte.nep', '@',
		'3600', 'SOA',
		'ns1.orvelte.nep. admin.orvelte.nep. 0 10800 3600 1814400 3600')
# knot.knotc_shell ('zone-set orvelte.nep @ 3600 SOA ns1.orvelte.nep. admin.orvelte.nep. 0 10800 3600 1814400 3600', expect_ok=True)
knot.add_rr ('orvelte.nep', '_443._tcp.www',
		'3600', 'TLSA',
		('%d %d %d %s' % (0,1,2,'6660')))
# knot.knotc_shell ('zone-set orvelte.nep _443._tcp.www.orvelte.nep 3600 TLSA 0 1 2 666', expect_ok=True)
knot.try_commit ()

knot.have_conf ()
# knot.knot (cmd='conf-unset', section='zone', item='domain', data='orvelte.nep')
knot.del_zone ('orvelte.nep')
knot.force_abort ()

knot.close ()


#
# Produce lists of new values (or indicate anything is accepted)
#
#TODO# Map to arpa2dns values
#
def token_factory (token):
	# For now, allow anything
	return None



class Cmd (arpa2cmd.Cmd):

	version = (0,0)
	prompt = 'arpa2dns> '
	intro = 'Edit zone data.  Be reported on expected times of publication, based on prior record timings.'


	def __init__ (self):
		arpa2cmd.Cmd.__init__ (self)
		self.knot = tightknot.TightKnot ()
		self.tlsa_config = (0,0,0)

	@cmdparser.CmdMethodDecorator(token_factory=token_factory)
	def do_zone (self, args, fields):
		"""
		zone ( add <zone> <ns1> <rp> | del <zone> )

		Add a zone with its first public master and responsible
		person email address; remove a zone.
		"""
		subcmd = args [1]
		zone = args [2]
		print 'Got: zone', subcmd, zone
		self.knot.have_conf ()
		zone_cmd = 'zone-set' if subcmd == 'add' else 'zone-unset'
		self.knot.have_conf ()
		if subcmd == 'add':
			self.knot.add_zone (zone)
		else:
			self.knot.del_zone (zone)
		if not self.knot.try_commit ():
			sys.stderr.write ('Failed to %s zone %s\n' % (subcmd,zone))
			return False
		if subcmd == 'del':
			return False
		self.knot.have_zones (zone)
		ns1 = args [3]
		rp = args [4].replace ('@', '.')
		self.knot.rr (zone, '@', '3600', 'SOA',
			'%s %s 0 10800 3600 1814400 3600' % (ns1,rp))
		if not self.knot.try_commit ():
			sys.stderr.write ('Failed to setup zone %s\n' % (zone,))
		return False

	@cmdparser.CmdMethodDecorator(token_factory=token_factory)
	def do_tlsa (self, args, fields):
		"""
		tlsa ( ( add | del ) <zone> <_proto> <port> <fqdn> <to-be-matched> |
		       config (ca-root|pkix-end|trust-root|any-end) (cert|pubkey) (full|sha256|sha512) )

		Adds or removes TLSA records for a certificatie supplied in the
		<to-be-matched> parameter.  Any computations on the certificate
		are assumed to be dealt with by the client, as this is most likely
		the certificate generating party, with knowledge about how to do
		this.

		The particulars of the published TLSA records are independently set
		with the config command form.  The parameters are in symbolic forms
		to make the result more readable.  This time, knowledge is best on
		the side of the DNS shell.

		The setup is not fool-proof; one must provide the suitable kind of
		data in <to-be-matched> for it to work.
		"""
		subcmd = args [1]
		if subcmd == 'config':
			certusage = { 'ca-root':0, 'pkix-end':1, 'trust-root':2, 'any-end':3 } [args [2]]
			selector  = { 'cert':0, 'pubkey':1                                   } [args [3]]
			matchtype = { 'full':0, 'sha256':1, 'sha512':2                       } [args [4]]
			self.tlsa_config = (certusage, selector, matchtype)
		if subcmd in ['add', 'del']:
			zone = args [2]
			prefix = '_' + args [4] + '.' + args [3] + '.'
			owner = prefix + args [5]
			tomatch = args [6]
			(tag0,tag1,tag2) = self.tlsa_config
			self.knot.have_zones (zone)
			work_rr = self.knot.add_rr if subcmd == 'add' else self.knot.del_rr
			work_rr (zone, owner, '3600', 'TLSA',
				'%d %d %d %s' % (tag0,tag1,tag2,tomatch))
			if not self.knot.try_commit ():
				sys.stderr.write ('Failed to %s TLSA record for %s under %s\n' % (subcmd, owner, zone))
			return False

	@cmdparser.CmdMethodDecorator(token_factory=token_factory)
	def do_acme (self, args, fields):
		"""
		acme ( add | del ) <zone> <fqdn> <txtfield>

		Publish a response to an ACME challenge in DNS.  These entries
		are sometimes required while obtaining certificates to use under
		a domain name.  When protected by DNSSEC, the mechanism can
		indeed be quite strong, though one wonders why the publication
		of a TLSA record is not used.

		This command is a purpose-specific wrapper around general DNS
		editing actions, intended to simplify interfacing to ACME and
		to provide a separate access point in terms of privileges.  If
		your intention is to use ACME, use this command instead of
		direct intervention with resource records.
		"""
		subcmd = args [1]
		zone = args [2]
		fqdn = args [3]
		txtfield = args [4].replace ('"', '\\"')
		if fqdn [-2:] != '.@':
			sys.stderr.write ('The <fqdn> parameter should end in ".@" for this use')
			return False
		if fqdn [-3:] in ['..@', '.@']:
			sys.stderr.write ('The <fqdn> parameter is not properly formatted')
			return False
		name = '_acme-challenge.' + fqdn [:-2]
		self.knot.have_zone (zone)
		zone_cmd = 'zone-set' if subcmd == 'add' else 'zone-unet'
		self.knot.knot (cmd=zone_cmd, section=zone, item=name, data='3600 TXT "' + txtfield + '"')
		if not self.knot.try_commit ():
			sys.stderr.write ('Publication failed')
		return False

	@cmdparser.CmdMethodDecorator(token_factory=token_factory)
	def do_record (self, args, fields):
		"""
		record ( add | del ) <zone> <fqdn> [in] [<ttl>] <rtype> <rdata> [...]
		"""
		subcmd = args [1]
		zone = args [2]
		fqdn = args [3]
		rtype = fields ['<rtype>']
		rdata = fields ['<rdata>']
		ttl = fields ['<ttl>'] or '3600'
		rec_cmd = 'zone-set' if subcmd == 'add' else 'zone-unset'
		self.knot.have_zone (zone)
		self.knot.knot (cmd=rec_cmd, section=zone, item=fqdn, data=('%s %s %s' % (ttl,rtype,rdata)))
		if not self.knot.try_commit ():
			sys.stderr.write ('Failed to %s %s in zone %s' % (subcmd,rtype,zone))
		return False






try:
	shell = Cmd ()
	shell.cmdloop ()

except Exception as e:
	print 'EXCEPTION:', e

