#!/usr/bin/env python
#
# arpa2lifecycle -- Life Cycle Management through lifecycleState attributes.
#
# The life cycle is usually initialised by inserting a simple record in
# LDAP, and letting life cycles take control.  The initial life cycle
# state may look like this:
#
# lifecycleState: cert . pubkey@ request@ acme?signed selfsig?signed certified@ dane?dnsdel deprecated@ expired@
#
# This leaves many time-bound actions to be steppd through, each time moving
# the dot to the right until it is at the end of the life cycle.  Each stap has
# an action related to, which is tried with exponential fallback until it
# succeeds. Presumably, LDAP will then be updated too.
#
#TODO#CERT_SPECIFIC:
# pubkey == Having obtained a public key.
# request == Having created a certificate request.
# certified == Having a certified certificate.
# deprecated == Certificate is outruled by other certificates.
# expired == Certificate is no longer available for active use.
#
# Guards like acme?signed selfsig?signed and dane?dnsdel are meaningless
# to this script, and such evaluation steps are not taken, because they
# are the action domain of the Pulley Backend for Life Cycle Management.
#
# From: Rick van Rein <rick@openfortress.nl>


import sys
import time
import re


# This script is called with the following arguments:
#  - DN of the LDAP object
#  - lifecycleState that got triggered
#TODO# Pass as a message, or shell command; not as commandline arguments


event_re = re.compile ('^([a-zA-Z0-9_]+)[?]([a-zA-Z0-9_]+)$')
timer_re = re.compile ('^([a-zA-Z0-9_]+)[@]([0-9]*)$')


# We require exactly two arguments
#
assert (len (sys.argv) == 3)

objdn = sys.argv [1]
prestate = sys.argv [2].split ()
dotidx = prestate.index ('.')

workdone = prestate [:dotidx]
worktodo = prestate [dotidx+1:]

# Skip any lifecycle?event formats
# (These would have been accepted already)
#
while worktodo != [] and event_re.match (worktodo [0]):
	workdone.append (worktodo.pop (0))

# We should have some work to do
#TODO# Or not, if we end in lcname?evname and just need the dot after it
#
while worktodo != []:
	next_event = worktodo [0]

	# Test if the upcoming entry might be a lcname?evname
	# then this is to be fed back into the Life Cycle Manager
	# except when we have not done anything yet.
	#
	if event_re.match (next_event) is not None:
		# Presumably the form lcname?evname
		if workdone == []:
			workdone.append (worktodo.pop (0))
			continue
		else:
			break

	# We should be asked about a time-based event
	#
	timer_timing = timer_re.match (next_event)
	if timer_timing is not None:
		(event,timeout) = timer_timing.groups ()
		if timeout != '':
			post_timeout = timeout
		else:
			post_timeout = '%d' % int (time.time ())

	#TODO#CERT_SPECIFIC_LOGIC:

	if event == 'pubkey':
		pass #TODO# certtool --generate-privkey --ecdsa --outder --outfile "pkcs11:..."

	elif event == 'request':
		pass #TODO# certtool --generate-request --load-privkey "pkcs11:..." --load-pubkey "pkcs11:..." --outder --outfile request.der

	elif event == 'certified':
		# No need for explicit action (in this life cycle)
		# Some other life cycle should have setup pkiUser
		# Set a timeout for the expired@ event?
		pass

	elif event == 'deprecated':
		# No need for explicit action (in this life cycle)
		pass

	elif event == 'expired':
		# No need for explicit action (in this life cycle)
		# This timeout should trigger at the NotAfter time
		pass

	else:
		raise NotImplementedError ('Unknown event in %s' % next_event)

	#TODO#END_CERT_SPECIFIC_LOGIC.

	# We did not break out of the loop, so move the dot forward
	#
	worktodo.pop (0)
	workdone.append ( '%s@%s' % (event,post_timeout) )


# Produce the new attribute value -- with the dot possibly moved on
#
print 'workdone =', workdone
poststate = ' '.join (workdone + ['.'] + worktodo)
print 'DEBUG: pre: ', prestate
print 'DEBUG: post:', poststate
if poststate == prestate:
	raise Exception ('Nothing happened')
else:
	raise NotImplementedError ('Need to update LDAP')



class LifecycleState (object):
	"""Parse and manage a lifecycleState attribute.
	"""

	def __init__ (self, lcstate):
		"""Setup an object with the lifecycleState attribute value.
		   Allow further processing and, at the end, possibly
		   derive extra elements to put into an LDAP modification
		   statement.
		"""
		self.lcstate = lcstate.split ()
		self.rollback ()


	def rollback (self):
		"""Rollback any changes made here.  This can be used to
		   avoid modifying LDAP when something went wrong.
		"""
		TODO:SETUP:OBJECT:FROM:self.lcstate
		dotidx = prestate.index ('.')


	def ldapmods (self):
		"""Return changes to the lcstate since initialisation or
		   since the last rollback in the form of an LDAP modify.
		"""
		TODO:RETURN:SOMETHING:USEFUL


class LifecycleObject (object):
	"""Wrap around an LDAP object with a given DN.  Collect changes
	   into one grand update.
	   TODO: Support additional changes to other objects, as this
	   may be desired by the life cycle too.  A subclass may setup
	   such things.
	"""

	def __init__ (self, dn):
		"""Setup an object for the given distinguishedName.
		"""
		self.dn = dn
		self.rollback ()


	def rollback (self):
		"""Rollback any changes made here.  This can be used to
		   avoid modifying LDAP when something went wrong.
		"""
		TODO:SETUP:OBJECT:FROM:self.dn


	def addmods (self, mods):
		"""Add modifications to the LDAP object from an external
		   source.  Usually, this is the change to the
		   lifecycleState, which is not manually done.
		"""
		TODO:ADD:TO:MODS


	def commit (self):
		"""Commit the transaction.  LDAP is not supportive of
		   two-phase commit, so this function may raise an
		   exception.
		"""
		TODO:COMMIT:CHANGES


class LifecycleHandler (object):
	"""The Lifecycle Handler class represents a processing node for
	   pairs of distinguishedName (DN) and lifecycleState (LCS) that
	   triggers do_xxx actions defined in a subclass.  These actions
	   may modify the LDAP object as stored at the DN.
	"""


	"""The name of this Lifecycle Handler must be overridden
	   in a subclass, and will be used to compare against the
	   input provided.  This serves as a correctness criterium.
	"""
	lcname = None


	def __init__ (self, stream):
		"""Create a new Lifecycle handler, which serves
		   multiple instances of a named life cycle.  Updates
		   are collected from an input stream that sends pairs
		   of lines; one line with the DN for the life cycle
		   instance and a second line with the current content
		   of the lifecycleState for our life cycle.
		"""
		self.stream = stream


	def _linepairs (self):
		"""Internal function.  Generator for line pairs from
		   the input stream.
		"""
		while True:
			ln1 = self.stream.readline ()
			if ln1 == '':
				return
			if ln1 [-1:] == '\n':
				ln1 = ln1 [:-1]
			else:
				syslog.syslog (syslog.LOG_WARN, 'Input DN line not properly terminated:' % ln1)
			ln2 = self.stream.readline ()
			if ln2 == '':
				syslog.syslog (syslog.LOG_ERR, 'Line pair incomplete; dropping DN %s' % ln1)
			if ln2 [-1:] == '\n':
				ln2 = ln2 [:-1]
			else:
				syslog.syslog (syslog.LOG_WARN, 'Input LCS line not properly terminated:' % ln2)
			yield (ln1,ln2)


	def handle_forever (self):
		"""Start looping on input lines, and handling them in
		   a pairwise manner.  Each time when an action named
		   xxx may fire, a subclass call self.do_xxx(req) is
		   made.  The request contains the lifecycleState in
		   a parsed and ready-for-updates manner, and it can
		   also dig up other attributes in LDAP.
		"""
		for (dn,lcs) in self._linepairs ():
			self.process_request (dn, lcs)


	def process_request (self, dn, lcs):
		"""Process a request, which is of pair of DN and LCS.
		   This may invoke self.do_xxx(lcstate,ldapobj) methods
		   in the subclass.
		   TODO: How about doing more than one action at once?
		"""
		try:
			lcstate = LifecycleState (lcs)
			ldapobj = LifecycleObject (dn)
			lcname = lcstate.lifecycle ()
			action = lcstate.nextaction ()
		except Exception as e:
			exc = str (e)
			syslog.syslog (syslog.LOG_ERR, 'Request error: %s' % (exc,))
			return
		if not lcname == self.lcname:
			syslog.syslog (syslog.LOG_ERR, 'Lifecycle name %s does not match handler name %s' % (lcname,self.lcname or '(unset)')
			return
		mth = getattr (self, 'do_' + action, None)
		if mth is None:
			syslog.syslog (syslog.LOG_ERR, 'Ignoring unknown action %s in life cycle %s' % (action,lcname))
			return
		try:
			mth (self, lcstate, ldapobj)
		except Exception as e:
			exc = str (e)
			syslog.syslog (syslog.LOG_ERR, 'Action %s on object %s failed: %s' % (action,dn,exc))
			return
		try:
			ldapobj.addmods (lcstate.ldapmods ())
			ldapobj.commit ()
		except Exception as e:
			exc = str (e)
			syslog.syslog (syslog.LOG_ERR, 'Saving to LDAP object %s failed: ' % (dn,exc))
			return


