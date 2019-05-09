# Typical session with IdentityHub

> *Nothing improves the joy of play more than
> tightly woven instructions.  So here they are!*

Run a detached Docker image, assuming you don't want
to lock your shell to OpenLDAP's output:

```
docker run -d --name id0 arpa2/identityhub
```

Now start a session in the container you just created:

```
docker run -it id0 bash
```

Your first action is usually to install `gnureadline`
for comfort,

```
arpa2gnu
```

You can dump the OpenLDAP database at any time, without the
need to enter a password, by retrieving it straight from the
database and thus bypassing the LDAP protocol,

```
slapcat | less
```

The output is in LDAP Interchange Format, or LDIF, which prints
objects separated by empty lines and each line holding
attribute-colon-value.  The first line of an object is
always the `dn:` attribute, or object location.

If you're curious about the meaning of the entries, please
read its commented source,

```
less /root/initial.ldif
```

Then, you can start the metashell, using password `sekreet`
(which I'm only telling you so please don't tell anyone!)

```
arpa2shell
```

You can now switch to a sub-shell for identity management,

```
arpa2id
```

There are several things you can do now, for instance,

```
domain_add orvelte.nep Orvelte, Inc.
user_add orvelte.nep bakker Hij die bakt
user_add orvelte.nep smid Hij die hakt
alias_add orvelte.nep bakker koeke
```

If you dump the database once more, you will see the newly
added entries, domains as `associatedDomain=` directly
under `ou=IdentityHub,o=arpa2.net,ou=InternetWide` and
the users and alias as `uid=` entries underneath the
domain object for the `orvelte.nep` domain.  You should
also see the `bakker%2bkoeke`, which is escaped from
the actual form `bakker+koeke`, has an additional
`uidAlias:` attribute to indicate it is an alias of the
user in this attribute.

This is the format used.  A bit more complexity follows
for groups, because we need to set a group member alias
and tie it to specific users; this is represented as a
`uidMember:` and `uidGroup:` reference back and forth, and
a special object for each group member underneath the
`uid=` object of the group.


## Switching to Kerberos

You can try to get a Kerberos ticket from our demo
server, using the password `sekreet` to one of

```
kinit demo
kinit demo@ARPA2.NET
```

You can see the ticket in your name for service
`krbtgt/ARPA2.NET@ARPA2.NET` which is the ticket-granting
ticket (or TGT) or, more prozaically, a meta-ticket that
will grant you further tickets to actual services.

Now edit `/etc/ldap/ldap.conf` and change the `BINDDN`
in the one ending in `cn=gssapi,cn=auth` as this is the
way to tell OpenLDAP that you want to authenticate with
GSSAPI and Kerberos5.

Try again accessing a shell,

```
arpa2shell
```

and it may take some time (especially if networking for
your Docker container is slow; on real hardware it tends
to be lightning fast) but let you in without a password.
When you terminate the shell and start it again it will
already have the ticket and you can get in immediately.
You can review the extra ticket for the service named
`ldap/reservoir.arpa2@ARPA2.NET` with

```
klist
```


## Access through LDAP

You can use the same credentials to access OpenLDAP
records over the LDAP protocol.  Let's search for
all `uidAlias:` attributes underneath the `orvelte.nep`
domain.  To search it, we will login as the admin and
use the appropriate authentication system.

With password access, again using `sekreet`, you would
use

```
ldapsearch -H ldapi://%2ftmp%2fldap-socket \
	-D uid=admin,ou=IdentityHub,o=arpa2.net,ou=InternetWide -W \
	-b associatedDomain=orvelte.nep,ou=IdentityHub,o=arpa2.net,ou=InternetWide \
	-s sub \
	'(&(uid=*)(uidAlias=*))' \
	uidAlias
```

The lines represent the UNIX domain socket for the
local `ldapi` URI scheme (which is OpenLDAP specific),
then the object as whom to login and to ask for a password,
then the base object to search under, the `sub` tree search
scope, a filter that wants both an attribute `uid:` and
`uidAlias:` with any value, and finally a request for the
`uidAlias:` attribute value(s).  The answer is listed with
the object location (`dn`) preceding it,

```
dn: uid=bakker%2bkoeke,associatedDomain=orvelte.nep,ou=IdentityHub,o=arpa2.net
 ,ou=InternetWide
uidAlias: bakker
```

To search through the network, change the URI in the first line to

```
ldap://127.0.0.1:1389
```

You should see no difference, but take note that authenticated based on local
user accounts only works over the former URI.

Now, assuming you're pretty much fed up with typing passwords, let's switch
to using Kerberos.  This is done by replacing the options on the second line,
so `-D` and `-W` with

```
	-D uid=demo,cn=arpa2.net,cn=gssapi,cn=auth -Y GSSAPI
```

You should expect the same results, except that you do not need to enter a
password.

Normally, you would not even login on the IdentityHub system, but instead on
your personal desktop.  You would then use OpenSSH with Kerberos authentication
while forwarding the ticket for use on the other system.  And you will have no
need to login again, as long as the ticket has not expired.


## Coming Soon: External Access

Something odd with Docker and ports and IP addresses... on Mac OS X.


## Coming Soon: Access Control

Access privileges will also be specified.  In the `uid=`
objects of aliases, groups and roles,
you can find one or more `accessControlLine:` attributes
that mention a strictly-visual line number, a set of
rights flags granted and any number of ARPA2 Identity
Selectors to which this applies.  For a group, this is
only done at the `uid=` object for the group, not for
individual group members.

The reason why `accessControlLine:` attributes occur
on aliases but not on primary `uid=` objects nor on
their pseudonyms, is that aliases serve as split
channels for the traffic with an alias, and the
ACL process finds the alias to apply while filtering.

This format for `accessControlLine:` keeps information
together just as the users would edit it.  This is
conceptually the most straightforward model.  The actual
ACL implementation however, groups pairs consisting of a
remote selector and alias-stripped local address as keys,
and lists the rights and alias that would apply to it.
The ACL mechanism iterates through the keys going from the
most concrete remote selector that matches a remote address
to the most abstract `@.` remote selector.  This is like
a transposed matrix; a chore that we establish through
SyncRepl subscriptions by our SteamWorks Pulley tool.

