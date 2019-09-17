Reservoir supports WebDAV
=========================

>   *The WebDAV system is a very nice interface to our Reservoir component. It
>   enables lots and lots of already-existing front-ends to communicate with our
>   infrastructure.*

The following things are interesting about WebDAV:

-   Assigning a path to each Resource/Collection

-   Resource properties, specified in XML files

-   Searching for properties

-   Locking Resources

-   Making and removing Resource Collections

WebDAV URI \~ LDAP DN
---------------------

We will indicate a user name in the resource path, which functions as
authorisation identifier; this is different from the authenticated user name, as
an individual user may authenticate and proceed to authorise for group access.

WebDAV locations will be specified as

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
https://user@domain.name/dav/name/col0/col1/res2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Here, the `domain.name` represents the domain for which WebDAV is being offered;
there is no SRV record definition to use another host name, so we use `/dav` to
discriminate the use of WebDAV for a subtree. After login, this option appears
on the user's start page.

To support the idea of publishing a website that is authored over WebDAV, we
shall allow readonly mappings to other locations as well.

The `user` is mentioned tongue-in-cheeck, as it will not work in all places. The
idea is to support authentication with the given identity, but this may be done
over TLS just as well as at the HTTP level. Basic and Digest authentication will
be insufficient, but SCRAM-SHA-256 is worth considering; HTTP SASL with
GS2-KRB5-PLUS is however preferred because it is a single-signon system. It is
still a mystery how we can convince others about the benefits of these
variations \&mdash; security is the obvious reason, but may be insufficient to
some?

The `name` is the authorisation identity, so it may represent a user, pseudonym,
alias or group. It also represents a node in the Global Directory under which we
will search for Resource Collection objects.

The names `col0` and `col1` mimic two levels of Resource Collections. Their
names are stored in LDAP nodes as local (nick)names to other collections; so
Resource Collection `col0` has a nickname `col1` referencing another Resource
Collection (presumably by its UUID) and `res2` is looked up in that latter
referenced collection. It will be required (and assumed, as these are internally
managed structures) that `col0` and `col1` have the object class of a Resource
Collection.  Nested paths acquire the conjugate access rights of each of the
Resource Collections, so in this case to do something it must be permitted under
both `col0` and `col1` and not just by the first or last of them.

The `res2` finally, represents the Resource found in the `col1` Resource
Collection. It is yet another level underneath the LDAP structure:
`cn=res2,ou=col0,ou=Reservoir,dc=,dc=` will be its distinguishedName.

WebDAV Property \~ LDAP Attribute
---------------------------------

The properties that describe a collection or resource in WebDAV end up being
stored in LDAP attributes. What is needed is a mapping and a set of definitions
for useful attributes (in an LDAP schema) and the matching properties (in an XML
schema).

The location of object descriptions in LDAP looks like

`cn=res2,ou=col1,ou=Reservoir,dc=,dc=`

Although each resource will have unique number assigned to it, the ACL-locating
`resourceInstance` with the Reservoir’s fixed `resourceClass` UUIDs are assigned
to the Resource Collection `ou=coll` and that is also where the rules of the ACL
are.  Resource Collections may holds links to other Resource Collections by
referencing their UUID and setting a local name to them; this can be used for
seemingly nested paths such as `col0/col1/res2` that might therefore end up on
the same set of objects as another path.  This might be used to share a
collection with less than its naturally assigned privileges, but never with more
privileges as that would be insecure.

WebDAV Object \~ Riak KV Object
-------------------------------

We intend to store the bulk of data objects in an object store, and have
selected [Riak KV](http://docs.basho.com/riak/kv/2.2.3/developing/usage/) for
this task.  It works with an HTTP wrapper with a few [predefined
layers](http://docs.basho.com/riak/kv/2.2.3/developing/usage/creating-objects/):

-   Bucket Type is the owning domain name

-   Bucket Name is a Resource Collection (each with its own ACL)

-   Keys are object names assigned to Resources, perhaps also as UUIDs

Access to Riak KV is based on a fixed Resource UUID for the Reservoir objects
and metadata; and a Resource Instance UUID is a Resource Collection.  Individual
objects may also be named by a UUID that might be seen as a third level of
indexing, but these object names have no meaning in terms of access control; the
combination of the Reservoir’s fixed UUID and a Resource Collection define the
`resourceClass` and `resourceInstance` to use to locate the
`accessControlledObject` and its access rules.

This ACL model based on resource instances is more general than one based on
user and group identities.  Users and groups will still want to have any number
of resource collections, but this is easy enough; by default, they will only be
accessible by the user or group for which the resource collection was created,
but changes to the ACL can then be modified per Resource Collection.  Setting
rights for individual stored objects is not supported; it would quickly get too
cumbersome to manage, and perhaps also too expensive to store and process.  It
should be easy enough to add a Resource Collection for a different access
policy; the various Resource Collections also send informal hints to users about
the sharing policy in use.

Python Processing
-----------------

We shall create a module `arpa2.reservoir`, with and main class `Reservor` and
tree element classes `ResourceCollection` and `Resource` to hold the logic in
relation to an LDAP and Riak KV backend connections.

WebDAV operations such as `MKCOL` may be directly supported on these tree nodes
and the Reservoir's attributes may be represented by dictionaries in Python.

The data stored may be more complete than the access rights for one particular
user.  Such access rights can be requested as needed, and used to form
`WebCollection` trees with `WebResource` leafs, linking to the various objects
in the Reservoir.

Making Resource Collections
---------------------------

WebDAV allows the creation of Resource Collections through the `MKCOL` method,
with a path to assign. This maps directly to an LDAP path, so the implications
are trivial. Although not strictly defined, there is a suggestion that
properties may be uploaded as a body to `MKCOL`, which makes so much sense that
it will indeed be supported.

The new Resource Collection is setup with a UUID, stored in its
`resourceInstance` LDAP attribute alongside the `resourceClass` attribute
covering the Reservoir’s fixed UUID.  The latter UUID is ignored for Riak KV
because it would apply to everything; the instance is used as the bucket name in
Riak KV.

To remove a Resource Collection, the `DELETE` method is used. It removes
references to other Resource Collections too, which may seem like recursive
deletion, so the user should be warned about what they are about to do.

The `MOVE` and `COPY` operations can be used to redirect Resources/Collections,
which all have mappings to LDAP. It is especially important to note that `MOVE`
updates any references to the old location, so it does involve a complete search
through the repository. The `COPY` method involves some focus on reference
counting of the entry in the backing Object Store.

WebDAV is permitted to reject attempts like these, for reasons that relate to
the service setup. In this case, it will mean that a user must be privileged to
make the changes, and that the URI paths mentioned must indeed be part of the
Reservoir, not of anything else.

Locking Resources
-----------------

The WebDAV methods `LOCK` and `UNLOCK` can be used by a user to gain
more-or-less exclusive access to a Resource.

It is possible to map this to an atomic operation in LDAP, where an old
attribute value (explicitly stating *this resource is free to be locked*) can be
replaced with another (stating something like *this resource is locked for X
until T*). Such attribute modifications are atomic, and will check on the
presence of the old value being replaced with the new. As such, they are the
perfect carriers for a locking mechanism. Note that no locking can succeed
without the attribute stating that the object is free; this is by design, and
permits Resources that cannot be locked at all.

When locks are thought to be in high demand, we can decide to split off any such
attributes using LillyDAP, and deal with them independently from the LDAP
infrastructure itself. The only vital thing is that all interfaces to the
Reservoir acknowledge the locking mechanism, and so go through this same split.

Output Queues
-------------

Some Resource Collections are not data sets, but output queues. It is still
possible to dump Resources in them, but there is a need to state the recipient
in a property, and once sent, the Resource will go. In other cases, the Resource
Collection may not even show any contents at all. It’s undefined, really.

In a WebDAV setup, we can imagine dealing with this by flagging a Resource
Collection as an Output Queue, and once information is stored in them to relay
it to the backend queue. It is a nice guesture to the user to reveal the
Resource until it is gone (or while no recipient has been set for it, though
refusal would be the better response in that case), also to allow queue
management. For now, Output Queues are a big TODO, other than the understanding
that their queue coordinates can be stored in the LDAP object for the Resource
Collection (for which it would be a subclass).

Further Integration
-------------------

It is often desired to notify people on updates, through a mechanism of their
choice. To this end, a Resource Collection may host notification attributes.
These would indicate the target URI of an interested party.

It may be desirable to support detailed instructions for rejection or
redirection of uploads. This matches with the desired behaviour for AMQP
arrivals. A suitable manner of handling this may be to support Sieve scripts,
which are generally good at inspecting headers and deciding on acceptance or
rejection, as well as alternate filing locations. For WebDAV an alternative
filing location causes redirection through a `301` response with a more
desirable `Location`, a process that may repeat a few times. The match between
WebDAV and AMQP may not be perfect however, since AMQP is external and WebDAV is
internal, but given that AMQP has passed through authentication and
authorisation, this may not actually make a difference. The most important
information to process in these scripts will be the `Content-Type` and the
originating `user@domain.name` along with their access rights to the Resource
Collection, all of which is available over either protocol that reaches into the
Reservoir.

Final Thoughts
--------------

The concept of WebDAV clearly maps on the Reservoir choices of LDAP for metadata
and an Object Store for Resource storage.

What may be worth considering is that LDAP was designed for slow-to-change data
sets, whereas WebDAV can be so hyperactive that it is even mounted as file
system. It is useful to realise that such use is not the design intention of the
Reservoir.

Having said that, LDAP is in fact not so specifically geared towards reading,
but implementations often are. There are ways of caching or offloading such a
write-rarely infrastructure, including the use of a completely different server
mechanism, possibly based on LillyDAP. There should be no real blockage on
growth due to this.

What is particularly useful about this setup, is the ability to search through
the data set. This is where the preference for reading shines, or can shine: by
heavily indexing all the Resource Collections and Resources for their attributes
(or WebDAV properties).
