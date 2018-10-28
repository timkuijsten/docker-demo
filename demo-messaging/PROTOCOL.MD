# Protocol for ARPA2 Messaging over AMQP

> *We have a number of arpa2xxx shells.  How to use them over AMQP?*

Shells relay commands, usually as a batch.  We should construct messages
that implement such batches.  We can present the output in an easily
parsed manner.

**TODO:** Shell `arpa2ok` could be created to `ping` and `pong` on
success, or to `echo` and `fail`.

**Commands** are formatted as `arpa2xxx> ` followed by a command.
Potential input is presented on lines starting with `> `.
This indicates that the command is to be run, until the end of line.
Interpretation is left to the shell at hand, as named `arpa2xxx`.
Similar selection mechanisms are also in `arpa2shell`, but we would
instead add it to a new wrapper named `arpa2amqp` and not call
`cmdloop()` but instead `onecmd()`.

**Results** are described as `arpa2xxx>> ` followed by the command.
Following lines present output and standard error, prefixed with
`> ` and `>> ` respectively.  When the shell indicates it wants to
finish, then so it does.

**Attribute-style** is the output format `<attr>: <value>`
from a command.  It supports simple parsing and should be the
standard output format to be sent to the `stdout` stream.
Standard errors should be format such that they can be logged;
possibly as a single line to the `stderr` stream.  Some care
would be handle empty output properly, not as an error case.

**TODO:** Pattern matching of expected output may some day be
sent downstream as part of a Batch.  Using the output format
can be helpful with that.  But let's not add such complexity
yet.

**Batches** are processed one by one.  Errors such as exceptions
being raised are fatal to the further running of the batch, but
reported errors need not be.  This really is calling shell commands.

**Requests** hold a Batch of Commands.

**Responses** hold a Batch of Results.

**Example** would be sending

```
arpa2id> domain add orvelte.nep
arpa2id> user add orvelte.nep bakker Hij die bakt
```

and receiving

```
arpa2id>> domain add orvelte.nep
arpa2id>> user add orvelte.nep bakker Hij die bakt
>> Error: no such user
```

You can easily check for the `>>` at the beginning of a line
to see error messages, and decide whether to panic.  Normal
commands should not produce such lines.  Note that the last
command will run to completion so any such line produced
would show up.  Just text that is explicitly printed to a
`tty` or `/dev/stdout` or such will not be caught (as the
only thing done will be redirection of `sys.stdout` and
`sys.stderr` -- and `sys.stdin` to read any continued
lines of input, or perhaps avoid doing so).

The recipient of the batch of shell commands is a component
in the IdentityHub and/or future additions.  It is assumed
that the receiving component will be able to run the shells
being asked for.  This allows the same shell in more than
one location, but not necessarily all running everywhere.

