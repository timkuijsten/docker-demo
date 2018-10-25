# AMQP 1.0 software from Apache Qpid

> *We intend to perform most messaging with AMQP 1.0 -- a standard protocol
> far passing around annotated blobs.  It is useful for internal communication,
> but even as a part of the user experience, as a less cluttered alternative
> to sharing through email attachments.*

[Qpid](https://qpid.apache.org/proton/index.html)
is a collection of software.  This image includes:

  * Qpid Proton Libraries for
    [C](https://qpid.apache.org/releases/qpid-proton-0.26.0/proton/c/api/files.html),
    [C++](https://qpid.apache.org/releases/qpid-proton-0.26.0/proton/cpp/api/index.html),
    [Python](https://qpid.apache.org/releases/qpid-proton-0.26.0/proton/python/book/overview.html).
  * Qpid Electron Libraries for
    [Go](https://godoc.org/qpid.apache.org/electron).
  * [Qpid Broker](https://qpid.apache.org/releases/qpid-cpp-1.38.0/cpp-broker/book/index.html)
    and
    [Qpid Dispatch](https://qpid.apache.org/components/dispatch-router/overview.html)
    are message-passing programs.

Each of these tools has its own purpose:

  * AMQP 1.0 in general is a *messaging protocol*, but it does not define
    any queueing discipline.  Queueing is one possible implementation,
    and a very likely one, but broader options exist; Qpid embraces those.
  * Qpid Proton and Qpid Electron are libraries to be used in programming
    languages.  The represent end points.  Qpid Electron is a variation tailored
    towards co-routines, which is why it was designed for Go as an alternative
    to the more classical interface of Qpid Proton.
  * Qpid Broker implements the queueing discipline that ruled the entire
    AMQP 0.9-1 approach.  The power of using queues is that micro-services
    can be connected with a rather high pace of communication, but without
    requiring the ends to be online at the same time.  Qpid Broker takes
    ownership of a message, or stated differently it takes the responsibility
    of delivering it, by placing it in a queue and committing to keeping it
    there until delivered.  Note that AMQP 1.0 includes end-to-end delivery
    notifications, so such a discpline can be more reliable than email as a
    messaging transport, not to forget more efficient.
  * Qpid Dispatch facilitates dynamic plugging of components.  It does not
    store messages, so it cannot take ownership, but it will find components
    that need to connect, and pass messages between them.  When Qpid Broker
    is like an Ethernet router, think of Qpid Dispatch as a switch.  Mindless
    yet functional for added flexibility/pluggability.


## Use of AMQP in Reservoir

The [Reservoir](http://reservoir.arpa2.net)
design includes messages that are pushed to users.
We use AMQP 1.0 for that, as it is fit for connections
between domains, and can indeed be declared in an SRV record
so the server to address for a domain can be found in DNS.

Recipients may indicate a Resource Collection as their incoming
point for new documents, possibly classified further based on
the remote sender, MIME-type and so on.  Only when a place is
found will the communication be accepted.

Thanks to [BYOID](http://internetwide.org/blog/2015/04/22/id-2-byoid.html)
we can ask uploaders to authenticate.  The AMQP protocol employs
TLS and SASL, as all but one protocol does.  This leaves a number
of mechanisms through which BYOID can work:

  * [TLS-KDH](http://tls-kdh.arpa2.net) with 
    [KXOVER](http://realm-xover.arpa2.net/kerberos.html) and
    SASL EXTERNAL would easily be the most efficient;
  * TLS with a certificate and SASL GS2 with
    [KXOVER](http://realm-xover.arpa2.net/kerberos.html) is
    probably the best we can have with today's protocols;
    for BYOID, it would also lean on
    [KXOVER](http://realm-xover.arpa2.net/kerberos.html);
  * TLS with a verifiable client certificate from the sender
    domain should not involve shocking changes.

The work will integrate with ARPA2
[Access Control Lists](http://donai.arpa2.net/acl.html)
for which
[efficient implementations](http://donai.arpa2.net/acl-impl.html)
exist, even for
[group and role access](http://donai.arpa2.net/acl-impl-group.html).


## Use of AMQP in IdentityHub

The IdentityHub design is
[built with blocks](http://internetwide.org/blog/2017/05/03/building-with-blocks.html)
that connect through some delivery channel.  Inside the hosting stack, this may well
be the Qpid Dispatch, which takes no ownership over messages but finds end points
that it connects as a pass-through.

The intention of a set of blocks is mostly to allow domains to take out blocks and
run them locally.  This means that messages must identify the domain that they are
working for, in a manner that can influence routing.  Based on that, a part of the
IdentityHub service might simply not be setup for a given domain name, and an
external component can take over.  This external component would probably connect
to a Qpid Broker, as that might queue messages when the remote customer component
goes offline.  Any protocols that require no interactive responsiveness could then
continue to work.  The customer's tooling may go through power cycles without
disrupting service or otherwise disadvantage the IdentityHub and its other domains.


## User of AMQP in ServiceHub

The ServiceHub design has not been worked out; this is merely a preview of things
that are likely to come.

In the [ARPA2 Identities](http://donai.arpa2.net/)
we left room for [services](http://donai.arpa2.net/idtypes.html)
and we formalised their notation as local parts that start with a `+` symbol.

Messaging to a service name may take a shape like this:

  * The local part is split at the `+` separator; the empty initial part is removed;
  * The first word names a service and determines routing of the request, subject
    to the domain's settings; it is possible to enhance this mechanism by looking
    at more than just the first word to determine a route;
  * The remainder of the words count as arguments, much like on the command line,
    which may involve subcommands that may have been routed independently; at some
    point, it ends up in a queue.

The entire interconnect is made through AMQP 1.0, whose standard nature would also
helpful to connect with a plugin service.  Such connections are always managed as
bulk connections, never per-domain, so as to enable large-scale operation.  There
will probably be a routing tag of some kind that allows the plugin provider to
further route distinctly based on the protocol in use.

Again, it should be possible for domain owners to bypass AMQP messages before they
are routed to a plugin service, and handle them localy.  Since some form of route
must be setup to indicate the plugin service provider, this should be simple to do.

Other protocols are likely to talk to this ServiceHub as well.  For instance, an
email might arrive at a service name, be delivered over LMTP, picked up and forwarded
on as an AMQP message.  A good setup would even allow replies to arrive back at such
an end point, which then maps the result back to the original protocol.  Such a
mapping with another protocol may be generic to a high degree, and agnostic to the
actual service that is being addressed.

In every case, agents that drop AMQP messages on the ServiceHub should be permitted
to address a service.  So, the sender and recipient identities are checked
against the [Communication ACL](http://donai.arpa2.net/acl.html) (if we don't discover
that we need a separate Service ACL, that is).

