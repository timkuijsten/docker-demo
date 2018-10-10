# arpa2shell is a base class for ARPA2 command interpreters.
#
# These interpreters can be told to know_about each other
# and call each other when combined with the meta-shell of
# the bare instance below.
#
# From: Rick van Rein <rick@openfortress.nl>


import cmd

import types



class Cmd (cmd.Cmd):

	prompt = 'arpa2shell> '
	intro = 'The ARPA2 shell drops in on a variety of sub-shells.'

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



