# Internal AMQP Messaging

> *We connect components over AMQP and use it for optimal flexibility.
> This image is the Qpid Broker, a system of input to Exchanges and output
> to Queues, connected through flexible Bindings.*

A bit of terminology is flying in our faces here, but nothing to worry
about:

  * **AMQP** is a protocol that allows peers to connect, authenticate and
    exchange messages.  Unless explicitly mentioned otherwise, we always
    refer to the latest standard, which is AMQP 1.0 at the time.
  * **Messages** are the annotated blobs that form the elementary unit of
    communication in AMQP.  They are somewhat reminiscent of email but
    targeted towards automatic exchange and much more performant.
  * **Exchanges** are inputs over which messages are offered.  They are
    not explicitly defined in AMQP 1.0 because the specification focusses
    on the protocol.  Implementations of older specification versions,
    such as RabbitMQ, use this concept too.  In Apache Qpid, the concept
    of Exchange is defined by the Qpid Broker.
  * **Queues** are outputs into which messages are stored before being
    delivered.  This intermediate store allows the protocol to respond
    with a leg acknowledgement, even when the recipient is not online.
    Queues are also not explicitly defined in AMQP 1.0 but they are
    a vital part of implementations such as RabbitMQ and replicated in
    Qpid Broker.
  * **Virtual Hosts** represent a collection of Exchanges and Queues,
    as well as Access Control and possibly Users and Groups.
  * **Acknowledgement** in AMQP is end-to-end acknowledgement.  It is
    normal to acknowledge the proper receipt or handling of a Message.

As stated
[by Apache Qpid](https://qpid.apache.org/amqp/):
*AMQP 1.0 is fully symmetric (peer-to-peer) as opposed to its asymmetric (client-server) predecessors. This means it can be used with or without intermediaries such as brokers and offers new possibilities for messaging applications.*
This is what we like about it -- it offers an opportunity to pass messages between realms.
Especially when combined with
[BYOID](http://internetwide.org/blog/2015/04/22/id-2-byoid.html)
and the related
[realm crossover](http://realm-xover.arpa2.net)
technologies.

A few good documents about AMQP can be found at RedHat:

  * [AMQP Model Overview](https://access.redhat.com/documentation/en-us/red_hat_enterprise_mrg/3/html/messaging_programming_reference/chap-amqp_model_overview)
  * [Apache Qpid](https://access.redhat.com/documentation/en-us/red_hat_enterprise_mrg/3/html/messaging_programming_reference/apache_qpid)
  * [Differences between predecessors and AMQP 1.0](https://access.redhat.com/documentation/en-us/red_hat_enterprise_mrg/3/html/messaging_programming_reference/differences_between_amqp_0-10_and_amqp_1.0)
  * [Getting started Developing for AMQP](https://access.redhat.com/documentation/en-us/red_hat_enterprise_mrg/3/html/messaging_programming_reference/chap-getting_started)


## Exchanges and Queues

**TODO:** The naming scheme described here is provisionary.  For example, there may be more use in mentioning a domain name in a routing key than in each Exchange or Queue name.  Since these keys split on a dot, we may want to change the notation somewhat, so as to support domain.user notation and matching on that.

Components deliver messages to an exchange.  A few exchanges are defined
for the InternetWide Architecture.

They mirror naming in LDAP, where

```
associatedDomain=orvelte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide`
```

translates to a multi-level name, where the structure is removed because
it is static:

```
internetwide/arpa2.net/reservoir/orvelte.nep
```

this can be matched with patterns, and that is helpful for flexible
delivery.  For example, we might match a particular pattern that we
need to route to a customer's own component.  The customer is also
assigned their own exchange, for instance:

```
customer/downlink01.orvelte.nep/reservoir/orvelte.nep
```

The second component is a service provider on behalf of the domain
in the fourth and last component.

Queues have similar names, except that they drop the last component.
**TODO:** or are these the message routing keys?
Dedicated rules to override pattern-based
defaults are made to bypass traffic to such queues, and dedicated rules
will also be routed from these exchanges.

The following names are used:

  * `internetwide/arpa2.net/identityhub` is an interface over which
    command batches for the `arpa2identityhub` and `arpa2acl`
    demonstration shells can be sent.
  * `internetwide/arpa2.net/reservoir` is an interface over which
    command batches for the `arpa2reservoir` and `arpa2acl`
    demonstration shells can be sent.
  * `internetwide/arpa2.net/acme` for ACME-style challenges
    and responses that must be added to DNS.


## Playing Around with this Image

Note that this image builds upon a large, long-winded build
in `build-qpid`.  Run it before trying to build this one.
We skip building it by default at the moment because it is so
big and takes so long.  But for the image described herein,
you will need the Qpid Broker, which is what is taking so long.

In the image, you can start the Qpid Broker with

```
qpidd &
```

It will report on the terminal that started it, so you might
as well choose

```
docker run -it demo-messaging qpidd
```

and use `docker exec` for other shells running commands.

You can now run `qpid-config` to add, remove and list the
various queues, exchanges and bindings.  And you can use
`qpid-route` to configure links and link routing rules.
And you can use `qpid-tool` for some general snooping
around.

Bindings for topic exchanges are most interesting to our
use case; we should employ keys that hold information that
permits routing.  Specifically, side-stepping routes for
certain customer domains, so we avoid overtaking the parts
of the domain that the customer prefers to control by
himself.  (Note that topic routers may route messages to
multiple destinations; the internal target should find a
way to skip that, for instance by not finding the domain
configured or by explicitly seeing it banned.)  (This is
important because
[acknowledgement is for at-least-once delivery](https://qpid.apache.org/releases/qpid-cpp-1.36.0/messaging-api/book/acknowledgements.html)
and we should not report successes when the real party to
do the work is the customer.

Work that can be offloaded to external components must not
block our normal flow.  So, failure to download messages
by one customer must not block transactions for Pulley
transactions, for instance.  We'd better require explicit
reply messages that end up cancelling a work-to-be-done
flag and as long as it is still activated run a regular
new attempt to get some work done.  Or something along
those lines; getting work done eventually being the idea
of saying `aye` to a transaction.


## Intended Use of AMQP

There are a few demonstrations of shells that can run
commands.  They are simple, but do demonstrate the kind
of work that takes place with certain changes.

The intention is to make queue subscribers out of each
of these shells.  In fact, the `arpa2shell` being an
interactive wrapper, we also want an `arpa2amqp` as a
queue-tapping command executor.

Components in the IdentityHub can use this to exchange
various messages; we will probably need an overall
process to control the entire thing, and this will
probably be our IdentityHub master process.  This will
be the one that forks off a bundle of work in response
to a mere addition of a user to a domain.  It would
orchestrate the creation of a PKCS \#11 layer, creating
keys in it, using it to construct certificates, store
the customary `pkcs11:` URIs and public key products
in LDAP, and so on.

One of the most contrived examples is how to start using
a new certificate for a server.  Really, it's mind-blowing:

 1. First, you figure out that you need a new certificate.
 2. You generate the certificate based on a private key
    that may be new or reused.
 3. You now start an ACME exchange with a CA of choice.
    This will pose a challenge.
 4. Since you don't want to limit yourself to HTTP, you
    choose to post a certificate challenge/response in DNS.
 5. Once published, you continue the ACME exchange, and
    you receive a certificate.
 6. Only now do you have the information to mention the
    certificate in a TLSA record, for DANE.
 7. You need to wait until all authoritatives have the
    new TLSA record, and then you need to await their
    omnipresence.  This means waiting the TTL of the
    former TLSA record, or the NACK timeout from the
    SOA record.
 8. Now all clients can see your TLSA record, you can
    tell the service to start using it.  In case of the
    TLS Pool, this is when you add a value in `localid.db`.
    After this, you can inform your user that the secure
    service has gone live with the new certificate.
 9. At the same time, any old certificate can be taken
    away from the service.  The corresponding private key
    may be removed, if it is no longer used.  You may
    need to look into multiple services, or else need a
    strict procedure for registration only locally.
 9. Now you can remove any TLSA record representing the
    old certificate.

And, will you believe it, there is a truck load of
ACME implementations, none of them yet aware of this
operational complexity.  It all seems to focus on
manual processing for small numbers of certificates.

One might argue that Let's Encrypt adds only a little
when certificates are already in DANE.  Indeed, the
validation procedures are in no way stronger.  But
browsers and other client software tends to rely on
a CA more than on self-help through DNS.  This is due
to all sorts of circumstances, including lack of access
to the low-level DNS protocol in JavaScript, that have
no bearing on security but just on technology.


## External AMQP Messaging (Under Consideration)

A few mechanisms exist that we might use to distinguish users.

 1. Kerberos can be used to exchange directly between users,
    but only when the recipient reveals a TGT.  This is not
    generally possible.
 2. Kerberos can use tickets with multiple levels, such as
    `amqp/orvelte.nep/bakker@ORVELTE.NEP`
    though this is not commonly done in other protocols.
    No standard exists to date, so it is possible and might
    even map to a useful AMQP path.  We could expand on that
    pattern to gain access to an exchange too!
 3. We may connect between a client and server with the same
    name, and thus have a domain-to-domain bulk session.
    This is very much in line with the idea of AMQP.  The
    tickets would be named `amqp/orvelte.nep@ORVELTE.NEP`
    and user names would be trusted to be set right by
    the authenticated domain.  **TODO:**  Now we need a way
    to link user names in message sender and recipients.


## Programming Notes

We can use Python to talk to
`qpidtoollibs.broker.BrokerAgent()`
and other elements of this module, and/or use the
[Qpid Management Framework](https://qpid.apache.org/releases/qpid-cpp-1.38.0/cpp-broker/book/ch02s02.html)
and we'll end up doing much the same as the legible Python script
[qpid-config](https://qpid.apache.org/releases/qpid-cpp-1.38.0/cpp-broker/book/chapter-Managing-CPP-Broker.html#section-Managing-CPP-Broker)
does: add, remove and list queues, exchanges and bindings.

Properties
[of exchanges](https://qpid.apache.org/releases/qpid-java-trunk/java-broker/book/Java-Broker-Management-Managing-Exchanges.html)
and
[of queues](https://qpid.apache.org/releases/qpid-java-trunk/java-broker/book/Java-Broker-Management-Managing-Queues.html)
are better defined for the Qpid Java Broker than for the Qpid C++ Broker.
This is certainly true where
[types of exchanges](https://qpid.apache.org/releases/qpid-java-6.0.4/java-broker/book/Java-Broker-Concepts-Exchanges.html#Java-Broker-Concepts-Exchanges-BindingArguments-JMSSelector)
are explained.

There is an
[ACL description](https://qpid.apache.org/releases/qpid-java-trunk/java-broker/book/Java-Broker-Security-ACLs.html)
to be used after
[autentication](https://qpid.apache.org/releases/qpid-java-trunk/java-broker/book/Java-Broker-Management-Managing-Authentication-Providers.html)
passes a Kerberos5 user through GSSAPI.  No GS2, it seems.  Authentication must be switched on, otherwise Qpid Broker may accept unauthenticated sessions.

Read about the distinction between
[link routing](https://qpid.apache.org/releases/qpid-dispatch-0.6.0/book/link_routing.html)
which is used to form infrastructure, and
[message routing](https://qpid.apache.org/releases/qpid-cpp-1.38.0/cpp-broker/book/chap-Messaging_User_Guide-Broker_Federation.html#tabl-Messaging_User_Guide-The_qpid_route_Utility-qpid_route_options)
which determines a route based on individual messages; this can be from queue to exchange, exchange to exchange.
Commandline management is done through the `qpid-route` utility.

