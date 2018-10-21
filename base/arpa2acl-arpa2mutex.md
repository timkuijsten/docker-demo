# The shell arpa2acl

> *The ARPA2 shell for ACL editing allows adding and
> removing access control rights for users, resources,
> resource instances and, more directly, on LDAP
> objects through their distinguishedName.*

There are bound to be multiple ACL changes at once,
so it is probably good to have a current working node.
Until that is set, no ACL changes should pass.

  * `goto dn <distinguishedName>` sets the LDAP object location to work on.
  * `goto comm user@domain.name` sets the LDAP object location to work on to the `uid=,associatedDomain=,ou=IdentityHub,o=arpa2.net,ou=InternetWide` object.
  * `goto res <uuid>` sets the LDAP object location as the node for the given resource.  **TODO:** There may be multiple, what to do then?
  * `goto resins <uuid> <key>` sets the LDAP object location as the node for the given resource and instance.  **TODO:** There may be multiple, what to do then?

These options can also be provided on the `arpa2acl` command line.  **TODO:** In that case the function is limited to just those records.

  * `constrain <rights>` means that only the indicated rights can be altered.  **TODO:** This may be used in the `arpa2acl` command line to limit the shell function.

Not all objects already have access control setup.  The following statements can arrange this:

  * `create <rights>` adds a minimal ACL to the LDAP object.  This consists of an objectClass and default rights for the catch-all `@.` DoNAI Selector.
  * `destroy` removes the minimal ACL from the LDAP object.
  * `reset <rights>` removes a complete ACL from the LDAP object, and replaces it with the default rights for the catch-all `@.` DoNAI Selector.

The things one may want to do with an ACL include:

  * `show` to see the current ACL lists.
  * `add [<lineno>] <rights> <selector>...` to add a series of DoNAI Selector entries.  When the first word is not numeric, it defaults to 0.
  * `del <rights> <selector>...` removes a series of DoNAI Selector entries under the given rights.  The rights should match accurately, but the selectors may be partially listed.

# The shell arpa2mutex

We can lock objects in LDAP, and the shell `arpa2mutex` can be used to see and possibly remove locks.

  * `<action> dn <distinguishedName>` sets the LDAP object location to work on.
  * `<action> comm user@domain.name` sets the LDAP object location to work on to the `uid=,associatedDomain=,ou=IdentityHub,o=arpa2.net,ou=InternetWide` object.
  * `<action> res <uuid>` sets the LDAP object location as the node for the given resource.  **TODO:** There may be multiple, what to do then?
  * `<action> resins <uuid> <key>` sets the LDAP object location as the node for the given resource and instance.  **TODO:** There may be multiple, what to do then?

To review and manipulate the lock, the following values for `<action>` are available:

  * `show` to see who currently holds the lock, if it is locked.
  * `lock <user>@<domain.name>` to set the lock held by the given user.
  * `lock_force <user>@<domain.name>` is similar to `lock`, but it will take the lock out of the hands of a current owner (and display the old owner).  **TODO:** It may fail if it is changed while we look at it.
  * `free` to grab the lock from the hands of a current owner (and display that party).  **TODO:** It may fail if it is changed while we look at it.
 