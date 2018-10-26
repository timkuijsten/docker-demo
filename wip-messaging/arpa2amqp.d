#!/usr/bin/env python
#
# arpa2amqp.d -- Receive requests to ARPA2 shells over AMQP.
#
# This is an AMQP 1.0 server that receives shell command
# requests, finds the shells to run them in, and sends
# back the result.
#
# The shells do not provide exit codes (other than the
# shutdown of the shell) but instead provide output on
# stdout and stderr.  This is sent back to the requester,
# if a reply_to address is presented along with the
# shell request.  Future versions of the command might
# receive stdin from the message, though that will
# mostly be empty.
#
# The idea is to have the same handler classes run under
# arpa2shell in an interactive mode and in here using a
# batch-processing mode.  The interactive shell can be
# used by administrators to correct for problems.
#
# From: Rick van Rein <rick@openfortress.nl>


import sys

import arpa2cmd


# Note: cStringIO is faster, but ASCII-only
from StringIO import StringIO


from proton import Message
from proton.handlers import MessagingHandler
from proton.reactor import Container


class ARPA2ShellDaemon (MessagingHandler):

	"""The ARPA2ShellDaemon awaits incoming ARPA2
	   shell requests, and sends them into backend
	   processes that run those shells.  Results are
	   sent back if a reply_to address is provided
	   in the shell request batch message.
	   
	   TODO: AUTHENTICATION AND AUTHORISATION
	         ...or can we leave that to Qpid Broker?
	"""


	# Create a new instance of the ARPA2ShellDaemon.
	#
	def __init__ (self, broker, address):
		super (ARPA2ShellDaemon,self).__init__ ()
		self.shell = { }
		self.broker  = broker
		self.address = address


	# Load the shell as a plugin module to this daemon.
	# Shells must hold an arpa2cmd.Cmd instance named Cmd.
	# The processes are retained for future re-use.
	#
	#TODO# We may want to be able to reset shells.
	#
	def get_shell (self, modname):
		if modname [:5] != 'arpa2':
			raise Exception ('Not a command shell: ' + modname)
		if self.shell.has_key (modname):
			return self.shell [modname]
		try:
			mod = __import__ (modname)
		except ImportError:
			raise Exception ('Shell not avaiable: ' +  modname)
		if 'Cmd' not in dir (mod):
			raise Exception ('Not a command shell: ' + modname)
		cmd = mod.Cmd ()
		if not isinstance (cmd, arpa2cmd.Cmd):
			raise Exception ('Not an ARPA2 shell: ' + modname)
		self.shell [modname] = cmd
		return cmd


	# Prefix lines with the given prompt and return the text.
	#
	def _prefix_lines (self, prompt, text):
		text = prompt + text.replace ('\n', '\n' + prompt)
		return text


	# Run a command in a given shell and collect the results.
	# The command may span multiple lines, adding to stdin.
	# Return the string to be inserted in the reply message.
	#
	def run_command (self, shellname, command):
		#DEBUG# print 'running shell', shellname, 'for', command
		savedio = (sys.stdin, sys.stdout, sys.stderr)
		if '\n' in command:
			(command,input) = command.split ('\n', 1)
		else:
			input = ''
		sys.stdin  = StringIO (input)
		sys.stdout = StringIO ()
		sys.stderr = StringIO ()
		try:
			shell = self.get_shell (shellname)
			shell.onecmd (command)
		except Exception as e:
			sys.stderr.write (str (e) + '\n')
		finally:
			cmdout = sys.stdout.getvalue ()
			cmderr = sys.stdout.getvalue ()
			(sys.stdin, sys.stdout, sys.stderr) = savedio
		reply  = shellname + '>> ' + command + '\n'
		reply += self._prefix_lines ('>> ', cmderr)
		reply += self._prefix_lines ('> ',  cmdout)
		#DEBUG# print 'returning reply', reply
		return reply


	# Split a message into shell commands that are run
	# in their respective shells.  Return the collective
	# reply to the batch job in the message.
	#
	def run_message (self, message):
		reply = ''
		while message != '':
			#DEBUG# print 'splitting message', message
			endcmd = message.find ('\narpa2')
			if endcmd != -1:
				endcmd += 1
			assert (endcmd != 0)
			#DEBUG# print 'Splitting prompt off of', message, '[:' + str (endcmd) + ']'
			try:
				(shell,cmd) = message [:endcmd].split ('> ', 1)
				reply += self.run_command (shell, cmd)
			except Exception as e:
				reply += 'Exception when running: ' + str (e) + '\n'
			if endcmd != -1:
				message = message [endcmd:]
			else:
				message = ''
		#DEBUG# print 'message run complete'
		return reply


	# Setup the current Container for incoming and reply traffic
	#
	def on_start (self, event):
		#DEBUG# print 'starting'
		ctr = event.container
		cnx = ctr.connect (self.broker)
		self.recver = ctr.create_receiver (cnx, self.address)
		self.sender = ctr.create_sender   (cnx, None        )
		#DEBUG# print 'started'


	# Receive an AMQP message.  Run its batch of commands,
	# collect the reply and pass it back if so desired.
	#
	def on_message (self, event):
		#DEBUG# print 'message received'
		msg = event.message
		if msg.body is None:
			return
		ans = self.run_message (msg.body)
		if msg.reply_to is not None:
			#DEBUG# print 'composing reply'
			rto = Message (
				address=msg.reply_to,
				correlation_id=msg.correlation_id,
				body=ans)
			self.sender.send (rto)
			#DEBUG# print 'sent reply'
		else:
			#DEBUG# print 'no reply requested'
			pass


# The main program creates on ARPA2ShellDaemon and runs it indefinately.
#
handler = ARPA2ShellDaemon ('amqp://localhost:5672', '/internetwide/arpa2.net/reservoir')
contain = Container (handler)
contain.run ()

