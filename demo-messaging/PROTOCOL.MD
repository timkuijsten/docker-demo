# Protocol for ARPA2 Messaging over AMQP

> *We have a number of arpa2xxx shells.  How to use them over AMQP?*

We use shells as a convenient interface to operators, with
builtin authorisation based on an operator's Kerberos
ticket and Access Control Lists that determine who may
do what on the various resources that are edited.  Since
the shells are purpose-specific, it is quite possible to
control operations in this manner.

The same mechanism can be used for remote access, though the
emphasis on the interface then leans more towards automation
than towards a pleasant human interface.  Human interfacing
constitutes such things as online help, and commandline
completion &mdash; think of domain names and UUIDs that
find their remainder after just a few characters entered.  Automation
has no use for these facilities, but thrives on boring consistency
in the interface; things like escaping and scraping information
from output suddenly become important.


## Shell and JSON

A normal shell command will display its syntax when asked
for it:

```
arpa2dns> ?dane

dane config (ca-root|pkix-end|trust-root|any-end)
            (cert|pubkey)
            (full|sha256|sha512) |

dane ( add | del ) <zone> <_proto> <port> <owner> <to-be-matched>
```

This format is entered in the documentation for each of the
shell commands, and interpreted by a special package named
[cmdparser](http://cartroo.github.io/ttrack/cmdparser.html).
This package builds a parse tree and recognises the input
accordingly.

These formats easily wrap into JSON.  As a rule, the literal
tokens end up together in a dictionary key `"do_"` which
holds a string of words or a list of word strings, prefixed
with the shell name.  So, the possible values for the second
form would be

```
"do_" : "arpa2dns dane add"
"do_" : "arpa2dns dane del"
"do_" : [ "arpa2dns", "dane", "add" ]
"do_" : [ "arpa2dns", "dane", "del" ]
```

The variable names in angular brackets become keys in the
same dictionary, for instance

```
"zone" : "orvelte.nep"
```

A complete JSON command of the second form might therefore be

```
 {
   "do_"           : [ "arpa2dns", "dane", "add" ],
   "zone"          : "orvelte.nep",
   "_proto"        : "_tcp",
   "port"          : "443",
   "owner"         : "@",
   "to-be-matched" : "AAM3as0i3rlkaf/sdfkjas4a/==="
 }
```

Note that all values are represented as strings, including numbers.
When multiple values occur, it is possible to use an array of strings;
in fact, a sole string is short-hand for an array of one string.

The first subcommand of `dane` is even easier; it constitutes completely
of literal words, so an example of that form in JSON would be

```
 {
   "do_" : [ "arpa2dns", "dane", "config", "any-end", "pubkey", "sha512" ]
 }
```

or, if that's easier,

```
 {
   "do_" : "arpa2dns dane config any-end pubkey sha512"
 }
```

The purpose of this form is to simplify things like escaping.
Had the `to-be-matched` variable been of a binary nature, or
had it included quotes, than JSON would offer a standard way
of dealing with it in your most beloved programming language.
No shell interface could beat that.

In everyday use, expect a JSON array to surround these JSON
command formats.  This allows the composition of batches of
shell commands, a bit like a shell script.  We shall need
that if and when we introduce transactions.  You can already
use it now to combine commands into one authenticated
message, as long as they are all runnable on the same
destination.


## Streaming input, output and error

The JSON interface has a few more properties relating to the
shell's normal use of `stdin`, `stdout` and `stderr` streams.

When specified as part of the command, the following variable
is fed into `stdin` for the command:

```
"stdin_" : "...data...to...feed..."
```

Similarly, the output of the command produces no return value
but a possible error string if anything was printed to
`stderr`, namely in `"stderr_"`.  Testing if this is present
is the same as testing for error conditions.  Only when this
is not present would it be generally safe to assume that the
`stdout` dumped into `"stdout_"` in the return value had any
meaning.  Not all calls will ask for this information, though,
it would also be possible to just fire away commands as a
best-effort attempt.

An attempt is made to parse the `stdout` stream before returning
the call, at least when `stderr` had nothing sent to it.  First,
HTTP-style headers are parsed in any content up to and excluding
an empty line.  These are returned as an array of pairs in JSON
output key `"headers_"`, where the pair is flattened to an array
of two components.  The `"headers_"` will be included if the
format ended in an empty line; it follows then that if `stdout`
starts with an an empty line, that `"headers_"` is set to an
empty array.  Since the output is an array, a header name may appear
as often as desired.

After the optional headers, an attempt is made to parse the
remainder as a JSON message.  Anything found there is the value
sent back, with possible new values for the said variables, which
all end in an underscore in the hope to avoid unintended clashes.
If a clash does occur, it counts as a failure to parse as JSON.


## Basic Commands for Testing

All shells derive from the `arpa2cmd.Cmd` class, and any number
of them collect in one `arpa2shell` that allows switching between
concrete shells.

The base class and its super class implement a number of useful
routines to be used for testing:

  * `help` or `?` without anything else lists available commands
  * `help` or `?` before a command word prints out its syntax
  * `version` prints the shell name and version
  * `date` prints the date as perceived by the shell server
  * `ping` prints a line on `stderr`
  * `ping6` prints a line on `stdout`
  * `whoami` prints your authentication status
  * `quit` ends the shell (returning to `arpa2shell` if applicable)

An example interaction with these shell commands follows:

```
shell$ arpa2dns

Edit zone data.  Be reported on expected times of publication, based on prior
record timings.  Throughout this shell, you can enter phone numbers in +dddddd
format where a zone is expected; this will be mapped to its ENUM counterpart.

arpa2dns> ?   

Documented commands (type help <topic>):
========================================
EOF   dane  enum  help  ping6  record   whoami
acme  date  exit  ping  quit   version  zone  

arpa2dns> ?help
List available commands with "help" or detailed help with "help cmd".

arpa2dns> help help
List available commands with "help" or detailed help with "help cmd".

arpa2dns> version
arpa2dns-0.0

arpa2dns> date
Fri Nov 23 10:27:40 2018

arpa2dns> ping
EPROTONOSUPPORT: Please upgrade to ping6

arpa2dns> ping6
pong6

arpa2dns> whoami
You are nobody

arpa2dns> quit

shell$
```


## Authentication of Shell Sessions

To authenticate to an ARPA2 shell, you setup a Kerberos credential.
Typically, this means

```
shell$ klist
klist: No credentials cache found (filename: /tmp/krb5cc_0)

shell$ kinit demo@ARPA2.NET
Password for demo@ARPA2.NET: sekreet

shell$ klist 
Ticket cache: FILE:/tmp/krb5cc_0
Default principal: demo@ARPA2.NET

Valid starting     Expires            Service principal
11/23/18 10:29:48  11/23/18 20:29:48  krbtgt/ARPA2.NET@ARPA2.NET
	renew until 11/24/18 10:29:36

shell$ arpa2dns

Edit zone data.  Be reported on expected times of publication, based on prior
record timings.  Throughout this shell, you can enter phone numbers in +dddddd
format where a zone is expected; this will be mapped to its ENUM counterpart.

arpa2dns> whoami
You are:    demo@arpa2.net
Expiration: Fri Nov 23 20:29:48 2018

arpa2dns> quit

shell$ kdestroy

shell$ klist
klist: No credentials cache found (filename: /tmp/krb5cc_0)
```


You can use SSH with Kerberos authentication, and if you include the
ticket forwarding option, you can use the ticket in the forwarded
session too.  So, if that shell starts an ARPA2 shell you should get
the same output from the `whoami` command as on your SSH client
account.  In other words, the use of `kinit` above is not needed but
instead you carry along your credentials.  Single sign-on is useful!


## Authorisation through Access Control Lists

While you enter commands on a shell, you will be making changes to
resources.  You might add a domain, or add a user to a domain, or
add an alias to a user.  Which of these rights is granted to you
is subject to authorisation.  The information to decide whether
you are authorised is implemented in access control lists (ACLs)
for each of these purposes.

Access control is generally edited with the `arpa2acl` shell on
the IdentityHub.  This is subject to access control, of course!
Changes made here dissiminate into the rest of the system, where
it ends up in
[efficient lookup tables](http://donai.arpa2.net/acl-impl.html)
spread over the various systems that help to quickly determine
who may do what.

The ARPA2 shells are dedicated to specific purposes, and are
therefore perfectly suited to make such authorisation decisions.
All we do is arrange up to date ACL databases in each of the
components running such shells.  (A second channel that we may
develop as an alternative would be to embed your personal
authorisations in the Kerberos service ticket for a shell in
[Diameter](http://idp.arpa2.net/diameter.html)
messages.)

The resulting shells are so well-protected that they can be
opened up to public access.  As long as an SSH session always
enters an ARPA2 shell that it cannot escape, there is no harm
that could be done beyond what ACLs permit.  This may sound
silly, but when we want to support service plugins, we need
all the integration that we can safely get.  It is also very
nice to provide shell access to power users.


## Authenticated JSON between Microservices

We can now define the protocol to run on top of the AMQP 1.0
infrastructure.  What we will always do, is protect the
messages with GSS-API.  The mechanism used would normally be
Kerberos5, which is efficient and protective against
[Quantum Computing](http://internetwide.org/blog/2018/02/10/quantum-crypto-1.html).

Given that GSS-API can also run other mechanisms, we might
use that for exceptional cases.  An example would be to have
password-based access for accounts independent of domain names.
Such accounts might be used as a fallback, and/or to collect
any number of domain names and sign for approval when changes
are made.

Microservices would recognise their name in the service ticket
sent as part of the GSS-API exchange.  This can be used to
detect that content should be a JSON-encoded string.

Had we chosen a JSON-specific framework for signing and/or
encryption, then we would not have been able to support other
protocols as easily.  Furthermore, the work would have been
based on static symmetric keys (difficult to share) or
public keys (inefficient, yet sensitive to Quantum Computer
attacks).  There would also have been a danger that the logic
of signing shifted into the application, where the security
model might get more cluttered.  GSS-API comes as an opaque
library, so we either take it or leave it.  (We take it!)

The layering that we have now is:

```
+------------------------+
|  ARPA2 Shell Commands  |
+------------------------+
:  Transactional Batch   :  (TODO, see below)
+------------------------+
|          JSON          |
+------------------------+
|         GSS-API        |
+------------------------+
|        AMQP 1.0        |
+------------------------+
|       TCP / SCTP       |
+------------------------+
|          IPv6          |
+------------------------+
```

We provide a library to take away the burden of using this,
of course.  On the client side, we have a library named
`arpa2amqp` and on the server side we use a program named
`arpa2amqp.d` to sit and wait for access attempts over the
stack shown above.

As with the shells, there is no real reason to shield off
this backbone from external use!  Especially given that an
AMQP connection starts off with authentication, it is easy
to see who is responsible if service-degrading traffic patterns
would ever hit us, and act on it.  This even helps when the
authentication is just for a bulk relationship such as all
work needed for a service provider; it even helps when this
service provider relays traffic from other realms; we can
allow this but effictively make the relaying party responsible
for what they send us.  We do still authorise every single
resource access that we deem worthy of protection, and for
that we use the GSS-API layer of each individual message,
not the bulk authentication.


## Distribution Model

There are two levels in distributing commands in the
ARPA2 infrastructure.  The first level is the host, the
second is a host-local shell.

  * The host is reached through its AMQP queue, usually
    with a name like `/internetwide/arpa2.net/reservoir`
  * The shell is reached by a command prefix such as
    `arpa2acl`.


## Preparing for the Future: Batch Processing and Transactions

**TODO:** We might consider processing batches as processes.
This might involve a special header printed on `stdout`, namely
`Undo:` followed by a reversing JSON structure.  The shell itself
might alternatively store these, and supply handles for an `undo`
or `rollback` command that would be retained until the shell is
exited.  The stability of the InternetWide Environment could
greatly benefit from such transactional facilitation.

To already accommodate batches of commands, we make the
following extensions to the format above:

 1. Zero or more JSON commands can be combined into a
    JSON array, to be parsed in one pass so any syntax
    problem in the batch is detected early.
 2. Every JSON command starts its `"do_"` with one
    extra word, naming the shell to run it in, for
    instance `"arpa2dns"` would work.

Responses are also collected into an array where each of the
individual responses are included.

There will probably be an extra JSON command such as
`"arpa2txn"` to control transactional behaviour.  This
shell is not currently supported, but batches of JSON
commands are.

