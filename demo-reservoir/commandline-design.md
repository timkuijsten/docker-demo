Reservoir Shell
===============

`reservoir_domain_` works on nodes in Reservoir that represent a DNS domain
name.  Resource Collections are organised underneath.  A domain name is always a
Resource Index as well.

-   `_domain_add orvelte.nep` creates a node for the given DNS domain name.
    This includes an empty Resource Index.

-   `_domain_del orvelte.nep` removes the empty node for the given DNS domain
    name.

`reservoir_collection_` works on nodes in Reservoir that are a Resource
Collection.  These are organised in a flat structure underneath a domain name.
They are identified by a UUID and usually have a descriptive name assigned as
well.

-   `_collection_add orvelte.nep Ambachten` creates a new Resource Collection by
    the given name under the given domain.  A random UUID will be created for
    it.

-   `_collection_del 1234-56-78-9abcdef` destroys an empty Resource Collection
    as identified by its UUID.

-   `_collection_list 1234-56-78-9abcdef` shows the Resources in the Resource
    Collection identified by its UUID.

`reservoir_resource_` works on individual Resources as organised in Resource
Collections.

-   `_resource_add 1234-56-78-9abcdef TODO TODO` stores a Resource in the
    Resource Collection identified by its UUID.  A random key will be created
    for it.

-   `_resource_del 1234-56-78-9abcdef MyKeyedResource` removes a keyed Resource
    from the Resource Collection identified by its UUID.

`reservoir_index_` works on nodes in Reservoir that are a Resource Index.
Indexes are lists of named pointers to Resource Collections; they need not be
Resource Collections themselves, but they may be, allowing the use of contextual
names for stepwise traversal through what would seem to be a nested structure.
The targeted Resource Collections contain an ACL, including rights to even know
about their contents and **TODO:**existence.  Indexes should not be shown
externally unless this knowledge is permitted.

-   `_index_domain orvelte.nep` starts searching in the Resource Index of a DNS
    domain name.

-   `_index_user orvelte.nep bakker` starts searching in the Resource Index of a
    user/alias/group/... under a domain.  **TODO:** Where to store the user?
    Under Reservoir or under IdentityHub?  Or maybe this is all part of the
    domain’s index, and could we simply use a naming convention like `~bakker`
    in that index?

-   `_index_collection 1234-56-78-8abcdef` starts searching in the Resource
    Index of a given Resource Collection.

-   `_index_path broden desem` steps down from a Resource Index to the appointed
    Resource Collection, and continues to see that as a Resource Index.
    Multiple such steps can be taken, together looking like a path to a Resouce
    or Resource Collection, like `~bakker/broden/desem/bruinebaksteen.jpg`

-   `_index_list` lists the current Resource Index.

-   `_index_add 1234-56-78-9abcdef krentemik` adds a reference to a Resource
    Collection to the current Resource Index, where it is entered with the given
    name.  The name is local to the current Resource Index.

-   `_index_del 1234-56-89-9abcdef` removes a reference to a Resource Collection
    from the current Resource Index.
