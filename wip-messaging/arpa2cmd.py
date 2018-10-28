# arpa2cmd is a base class for ARPA2 command interpreters.
# It shows itself as arpa2shell, from which it usually runs.
#
# These interpreters can be told to know_about each other
# and call each other when combined with the meta-shell of
# the bare instance below.
#
# From: Rick van Rein <rick@openfortress.nl>


import sys
import time

import cmd

import types



class Cmd (cmd.Cmd):

	version = (0,0)
	prompt = 'arpa2shell> '
	intro = 'The ARPA2 generic command shell offers basic support to actual shells.'

	gss_name = None
	gss_life = None

	"""General class for switching between shells.
	   It relies on do_arpa2xxx functions in each and
	   shares such commands for allowing switches at
	   termination of the calling shell.  These
	   functions therefore usually just return True
	   after setting the next shell name.  This is
	   even safe for references to the shell itself,
	   except in the case of ARPA2shell which has none
	   above it.
	"""
	def __init__ (self, *args, **kwargs):
		cmd.Cmd.__init__ (self, *args, **kwargs)
		self.known = [ ]
		self.next_shell = None

	def do_version (self, *ignored):
		"""Print the name and current version of this shell.
		"""
		sys.stdout.write ('%s-%d.%d\n' % (self.prompt.split ('>') [0], self.version [0], self.version [1]))

	def do_ping (self, *ignored):
		"""Respond to ping requests (with output on stderr).
		"""
		sys.stderr.write ('EPROTONOSUPPORT: Please upgrade to ping6\n')

	def do_ping6 (self, *ignored):
		"""Respond to ping6 requests (with output on stdout).
		"""
		sys.stdout.write ('pong6\n')

	def do_date (self, *ignored):
		"""Request the current time on the system running the shell.
		"""
		sys.stdout.write ('%s\n' % time.asctime (time.gmtime ()))

	def do_whoami (self, *ignored):
		"""Ask who you are, and how the shell sees you during ACL processing.
		"""
		if self.gss_name is None or self.gss_life is None:
			sys.stderr.write ('You are nobody\n')
			return
		try:
			import gssapi
		except ImportError as ie:
			sys.stderr.write ('This shell does not support GSSAPI\n')
			return
		try:
			exp = time.asctime (time.gmtime (self.gss_life))
			sys.stdout.write ('You are:    %s\nExpiration: %s\n' % (self.gss_name,exp))
		except gssapi.raw.MissingCredentialsError:
			sys.stderr.write ('You are nobody\n')
		except gssapi.raw.ExpiredCredentialsError:
			sys.stderr.write ('You have expired\n')
		except gssapi.raw.InvalidCredentialsError:
			sys.stderr.write ('Your credentials are wrong\n')
		except gssapi.raw.GSSError as ge:
			sys.stderr.write ('GSSAPI Error: %s\n' % str (ge))
		except Exception as e:
			sys.stderr.write ('General error: %s\n' % str (e))

	"""Termination commands.
	"""
	def do_EOF (self, *ignored):
		"""Exit this shell.
		"""
		return True

	do_exit = do_EOF

	do_quit = do_EOF

	"""Switch to the main command loop for arpa2shell.
	"""
	# def do_arpa2shell (self, *ignored):
	# 	# Special case: no shell above this one
	# 	return False

	"""Bind a shell to self, and make it return the
	   provided module as the shell to switch to.
	"""
	def bound_shell (self, name, module):
		def switch_shell (self, *ignored):
			self.next_shell = module
			return True
		switch_shell.__doc__ = 'Switch to the ' + name + ' shell: ' + module.intro
		return switch_shell

	"""Add a shell object by introducing it to all
	   the shells that we already know about.  This
	   transitively installs do_arpa2xxx functions
	   that return True after setting next_shell.

	   Think of this function as joining two sets by
	   pairing all pairs from the two sets, in one
	   direction only; it is explicitly repeated in
	   both directions between the two sets so as to
	   avoid infinite recursion.

	   It is assumed that at least on instance is a
	   mere ARPA2shell without subclass.  This one
	   can then be called to "just" be able to step
	   into any of the other shells.  This is not a
	   necessity, however.
	"""
	def know_about (self, shellname, shellobj):
		if (shellname,shellobj) not in self.known:
			self.known.append ( (shellname,shellobj) )
			for (knownname,knownobj) in self.known:
				knownobj.know_about (shellname, shellobj)
		bound_switch = self.bound_shell (shellname, shellobj)
		self.__class__.__dict__ ['do_' + shellname] = bound_switch



if __name__ == '__main__':
	shell = Cmd ()
	shell.cmdloop ()
