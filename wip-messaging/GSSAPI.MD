# Using GSSAPI with AMQP 1.0

> *Kerberos5 can be used for authentication, for instance with
> GSSAPI or GS2 in SASL.  But it can also help in encryption,
> as part of GSSAPI.  It can even give end-to-end protection,
> and use AMQP as a mere communication path.*

The proposal made here is that communicating peers use GSSAPI
with Kerberos5, and more specifically requiring encryption and
message integrity.  These messages can then be routed over
AMQP, effectively reaching end-to-end protection.  On the receiving
end, ACL tests and such can now be made.

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
that two messages are related.

GSSAPI initiates an exchange between contexts on both ends and
continues using throughout the session.  The initiation is
best sent directly to a target queue, with a `reply_to` address
set to a response channel.  There is no reason why the response
could not also set a `reply_to` address for a new response channel.
The communication could continue to reference the original
message through the `correlation_id`.  After one back-and-forth
exchange, the communication is side-tracked to these temporary
addresses, and anything passing through them is part of one
GSSAPI session.

GSSAPI itself is a filter through which you push your data,
only to retain literal bytes to send (for which AMQP is perfect)
and upon arrival a reversing filter is applied, leading to the
original data.  The same is done in the opposite direction.

It is up to us if the temporary end points are durable or not;
if not, they might not survive a restart of the software and
so messages could be lost.  This is a choice to make with the
expectations of a protocol in mind.  The end points must be
cleaned up at some point, at least when they are persistently
created.

Note that there are actually two ways of doing this; one
is to use a `corellation_id` that repeats the `id` from the
initiator message; another is to create temporary addresses.


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
one in the local realm.


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
can also support remote operator intervention.  (Indeed,
this is presumably what an SSH connection with ticket
forwarding does too.)

