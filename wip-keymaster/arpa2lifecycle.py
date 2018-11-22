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

