# WebDAV interfacing to ARPA2 Reservoir

> *This document describes how ARPA2 Reservoir publishes
> resources over WebDAV, with metadata stored in LDAP and
> object data in Riak KV.  This is not the only interface
> to these protocols; AMQP and even SMTP equally useful.*

The component
[Reservoir](http://reservoir.arpa2.org)
represents a generic object store with
access control protected by the
[InternetWide Identity Architecture](TODO)
which facilitates a
[Bring Your Own IDentity](TODO)
mode of welcoming external users.

This work is part of the
[hosting stack](TODO)
designed in what we call the
[InternetWide Architecture](TODO)
and for which we bundle the open source
realisation projects under the name ARPA2.


## Concepts of Reservoir

Nothing shocking here... just a few words
about concepts and access control to protect them.

The objects dumped in the Reservoir are called
*Resources* and they are supposed to have a
MIME-type and possibly more descriptive information.
Resources are raked together into *Resource Collections*
which is the scope at which we define Access Control
Lists.

Resource Collections are usually created by a user and
grant access to their identity with or without alias.
Alternatively, a group can have Resource Collections
created, in which case group members can access it,
making use of their group membership identity.
All this is not so much a matter of user management
as of setting up an initial ACL for the Resource
Collection, because these are not so much owned by
users as they are made available to users.

Every Reservoir is a labeled reference to a Resource
Collection.  They are stored in LDAP with a `reservoir`
attribute that holds the UUID and optionally a space
and a human-readable description or name.  Since a
Resource Collection can also have such attributes,
a Reservoir emerges under the first point of
reference.  A typical place for this first point of
reference is an object that represents a user.

The form with just an UUID and no space and
description is meant as a for system-internal
purposes.  It is most useful when is the default
Reservoir for the containing object.  One could
use this as the home of a user, for instance.
The default should not be made accessible in any
explicitly addressed manner; there is a form
with only a space, so an empty string as a
description, and that form could be addressed
explicitly.

There is no system-imposed constraint on unique
descriptions, so ambiguity may sometimes arise
when displaying just the descriptions.  In any
such ambiguous situations, the UUID must be
prefixed.  This form can be distinguished because
no word in the description may "look like" a UUID
(that term is somewhat open to human interpretation,
but think along the lines of no more than two dashes,
using many non-hex characters, mixed case, being of
clearly different length, and so on).

There is no system-imposed restriction like a tree
shape for a Reservoir; it will look like a tree
when unwound from any point, but it may actually
overlap other Reservoirs, and there can even
be back-references, causing nesting cycles.
Overlap can be useful to hang someone else's
Reservoir into your own, for instance a group
member could refer to a group's Reservoir.
Cycles are not a problem when unwound manually,
and automated tools should be mindful about the
UUIDs that it has already seen to detect cycles.
There is usually no need to constrain the depth
of traversal.

So even though Resource Collections are not stored with
nesting, their `reservoir` attributes help to build an
impression of a nested structure.  These paths can be
traversed only when the names are accessible, so the
intermediate Resource Collections on a path constrain
the visibility of the path elements.  They do not
constrain the eventual object however; that might also
be found by searching, and a most direct path is then
constructed.  At that point, only access control to
the last Resource Collection, the one holding the
targeted Resource, is a true security barrier!  This
choice allows us to perform deep searches, such as
over an exported LDAP interface.

As on a file system, paths begin with a home node derived
from one's authorisation identity as determined by the
access protocol used; for instance in WebDAV, one would
see the authorisation identity in the protocol itself,
followed by any number of nesting levels of names that
are looked up by `reservoir` attributes, and finally the
UUID of an individual Resource to deliver.  Other paths to
the same Resource may exist, and administrators have
commandline tools that bypass authorisation to get to
the Resource directly through the UUID.


## Storage of Reservoir

The metadata of Reservoir is meant to be searched
easily and flexibly.  For this reason, it metadata
is stored in LDAP.  This has the added benefit that
a direct interface to this search interface can be
made available to other applications, because LDAP
is a proper standard that can be serviced with,
once again, tight access control.

The object data of Reservoir is not stored in LDAP,
as that is not a suitable engine for large objects.
Instead, LDAP holds information that can be used as
a location specifier in Riak KV.  This object store
is widely supported in the open source community, and
it can be used as a redundantly hosted and therefore
rock-solid and highly available service; the same
holds for LDAP as well.

WebDAV is merely an interface for getting to these
storage components, integrating their logic and
appearing to end users as whatever WebDAV interface
they like; browsed over a GUI tool, mounted as a
filesystem or pushed and pulled over HTTP in a
dedicated application.  WebDAV is a popular standard,
and its structures align naturally with those of
Reservoir.



