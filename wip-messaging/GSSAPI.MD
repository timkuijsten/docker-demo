# Using GSSAPI with AMQP 1.0

> *Kerberos5 can be used for authentication, for instance with
> GSSAPI or GS2 in SASL.  But it can also help in encryption,
> as part of GSSAPI.  It can even give end-to-end protection,
> and use AMQP as a mere communication path.*

The proposal made here is that communicating peers use
[GSSAPI with Kerberos5](https://github.com/pythongssapi/python-gssapi/blob/master/docs/source/basic-tutorial.md),
and more specifically requiring encryption and
message integrity.  These messages can then be routed over
AMQP, effectively reaching end-to-end protection.  On the receiving
end, ACL tests and such can now be made.

The annotations offered by AMQP are of use for routing; they
might contain (well-known) exchange and queue names as well
as tag services, domains and users.  This "outer layer" may
lead to message loss or redirection, but it cannot break the
protection of the message content.  This is a normal "power"
of carrier protocols, and should not worry us.  In fact, the
flexible routing of AMQP is perfect counter-balance for our
content-based protection as specified herein.

This is useful for internal communication of the IdentityHub,
but will also facilitate plugin components such as customer's
own-hosted IdentityHub components and services plugged in
through the ServiceHub.  For these applications, it is good to
use standard technology.


## Using GSSAPI over AMQP

We use AMQP 1.0 as a communications system.  AMQP facilitates
containers, which are collective end points for individual
messaging links.  The end points are identified by addresses,
and it is a common design pattern to define a temporary end
point just to receive replies to messages sent upstream.  This
is not builtin functionality; the primitives to construct this
are simply available with a `reply_to` address pointing to a
newly created end-point address, and a `correlation_id` to see
that two messages are related.  It is usually more efficient
to setup just one `reply_to` address and use the `correlation_id`
to find the proper security context.  The only reason to have
more `reply_to` addresses could be to separate remote peers,
so they would not be able to easily hijack sessions from other
peers.

GSSAPI initiates an exchange between contexts on both ends and
continues using throughout the session.  The initiation can be
sent directly to a static remote peer, with a `reply_to` address
set to a response channel.  There is no reason why the response
could not also set a `reply_to` address for a response channel.
The communication could continue to reference the initiating
message through the `correlation_id`.  After one back-and-forth
exchange, the communication is side-tracked to these temporary
addresses, and anything passing through them is part of the
GSSAPI sessions that were selected to go through it (such as
all GSSAPI traffic between a pair of peers).

GSSAPI itself is like a filter through which data is pushed,
leading to literal bytes to send (for which AMQP is perfect)
and upon arrival a reversing filter is applied, leading to the
original data.  The same is done in the opposite direction.
It may be possible to use AMQP to reject a problematic GSSAPI
exchange, which would mimic a GSSAPI shutdown -- as closing
of connections is a meaningless concept to AMQP.  Note that
under normal operations, GSSAPI itself deletion of security
context, which effectively ends the GSSAPI exchange without
dependency on the carrier protocol.

It is up to us if the `reply_to` end points are durable or not;
if not, they might not survive a restart of the software and
so messages could be lost.  This is a choice to make with the
expectations of an application in mind.  The end points must be
cleaned up at some point, at least when they are persistently
created; the GSSAPI exchange gives timing hints for that.

Interestingly, we end up having a system with potentially
long-lasting connections between peers.  There is no need
to authenticate over and over again, but instead we can
have a connection standing by to service bulk traffic or
sparsely scattered traffic, both handled as messages pop
up and hardly any effort in keeping connections open.


## Problem Solved

The use of an AMQP message broker such as Apache Qpid
implies a dependency on its builtin system for access
control.  It may not be possible, for example, to constrain
access to queues representing a domain name for the
address of the sender.

The end-to-end use of GSSAPI solves this problem, by
ensuring the peers of their mutual identities.  So a
message cannot end up with a false party, and it cannot
have come from a wrong party.

There is one more interesting aspect, namely the
possibility to incorporate AuthorizationData in a
Kerberos ticket; this can be used to assure access
to a requested facility in the message, which must
be known in a nook or cranny; but it can be centrally
managed alongside the KDC when it travels in this
format.  The AuthorizationData can be setup
specifically for the targeted service, provided it is
one in the local realm.  An extension call is implemented
in MIT krb5 as well as Heimdal, named
`gsskrb5_extract_authz_data_from_sec_context()`
and effectively taking an `ad_type` to release the
`ad_data` for it.  In addition,
[GSS can supply attributes](https://tools.ietf.org/html/rfc6680)
for security purposes.  (References credited to
Greg Hudson of MIT krb5.)


## Problem Created

The reliance on GSSAPI may cause a need for interaction
that would not be desired in the plaintext version.
As a result, processes might get stuck waiting for a
reply while they would otherwise have continued.

We do not seem to loose the asynchronous nature in
all cases though.  Kerberos as a GSSAPI method does
not require interaction to send a request, this is
only needed to be certain of the recipient identity.

Careful use of the GSSAPI setup flags could retain
an asynchronous exchange; but it might reduce the
security level (somewhat).  In general however, no
protocol can establish mutual security when there
is no bidirectional traffic, so this is not really
surprising.  The thing is that GSSAPI enables the
asynchronous mode if so desired, as a tweak option.


## Simplicity Rules

Given that we have a default keytab configured, such
as the `KRB5_KTNAME` and/or `KRB5_CLIENT_KTNAME`
environment variables, we should be able to get to
tickets.  We might need a clever derivation scheme
to get to the ticket name, but that could simply be
derived from the AMQP address used.  We can probably
find a trick to relate exchange names and queue names
in a way conducive to such derivations, at least as
a default naming scheme that operators may override
if they are somehow bored or underworked.


## Access to Backend Services

When we gain access to a backend service like LDAP over a
shell, we can take on the identity of the one making the
change.  This is an operator in the case of an interactive
shell, and a daemon taking in shell commands would have its
own identity.

The identities used to access a service are then not the
same as those used by end users.  This is normal; it would
take a lot of repeated authentication procedures otherwise,
so this is a better bulk mechanism.  The idea is that these
identities are trusted, and that they may make certain kinds
of changes for the general public.  They may process Access
Control Lists on behalf of actual users, which is indeed
what they are meant to do.

Identities being logged are therefore not user identities,
but an extra step would be required to find those.  This
may be a benefit for privacy, but it may interfere with fast
trouble resolution.

Note that this is a good grain size for service-level
access control; a daemon might access certain attributes in
LDAP, say, but not all.  An operator shell on the other
hand, might have extra privileges but be protected from
mistakes that an automatic process would not easily make.

Having said all that, GSSAPI defines a flag for delegation
of credentials.  When specified, actions can actually be
handled at the request of an individual.  Note that this
can also support remote operator intervention.  (This is
probably what OpenSSH ticket forwarding uses too.)

