# README for Docker Image arpa2/reservoir

> *This is a simple, initial version of the ARPA2 Reservoir
> as designed in the InternetWide.org project, and implemented
> in a variety of ARPA2.net projects.  It should serve as a
> basis for further development and layering.*

This container currently holds:

  * OpenLDAP, exported on port 1388, configured for Reservoir.
  * arpa2shell, the meta-shell for arpa2xxx sub-shells.
  * arpa2reservoir, the sub-shell for resource management.
    **TODO:** The latter does not integrate with `arpa2shell`
    as it does not derive from `arpa2cmd`; it is entirely
    built from scratch.  That may change `;-)`

There are several ARPA2 idea that could be added:

  * WebDAV on port 1761 through [wsgidav](http://wsgidav.readthedocs.io)
  * AMQP 1.0 upload interface for external users
  * SFTP access? for `user@domain.name@host` to a simulated tree
  * LDAP subscription? to (selections of) Resource Collections
  * ATOM retrieval? of (selections of) Resource Collections
  * The SteamWorks Crank to view the repository via JSON
  * A frontend to the SteamWorks Crank
  * Authentication through TLS-KDH, OpenSSH, ...
  * Authorisation of actions via arpa2service
  * An `arpa2acl` shell for ACLs of Resource Collections
  * A SteamWorks Pully Script for ACL subscription
  * An LMDB instance holding subscirbed ACL information
  * Uses of the libarpa2service package

There are many things open for tinkering:

  * Gaining external access to OpenLDAP (and, once built, `wsgidav`)
  * Improving the shells in any way we like
  * Start using the `cmdparser` package in shells


## Deploy the Image

To build this image on top of the `arpa2/base` image:

```
docker build -t rv <THIS-DIRECTORY>
```

The `slapd` is on the main terminal, outputing tons of debugging
information; you can avoid seeing it with the `--detach` option:

```
docker run --detach --network host --hostname reservoir.arpa2 -p 1388:1388 -p 1761:1761 --name rv0 rv
```

You can stop and start it at any time using

```
docker stop rv0
docker start rv0
```

At some point, you will want to forget about the container,

```
docker rm rv0
```

## ARPA2 Shell Access

Shells for poking around can be started as desired:

```
docker exec -ti rv0 arpa2shell
```

This is a special shell for doing ARPA2 manipulations.
In fact, it is a meta-shell that can drop in on various
ARPA2 sub-shells and allows switching between them.
This is more powerful than just opening a single shell,

```
docker exec -ti rv0 arpa2reservoir
```

You can get a normal shell with

```
docker exec -ti rv0 bash
```

If you would to setup GNU readline for more pleasant
editing, you would run this on the shell:

```
arpa2gnu
```

And if you would like to switch to Kerberos authentication
you could use `demo@ARPA2.NET` with password `sekreet`:

```
arpa2kinit
```

To drop Kerberos, instead run

```
arpa2kinit @
```


On the shell, you could also run the shells,

```
arpa2reservoir
arpa2shell
```

Note that the set of domains is independent from those
used in IdentityHub.  They reside in different parts
of the tree, to help with situations where not all
domains have the same service, where subdomains have
additional service, where customers take out only one
but not both the services, and so on.

Once on an `arpa2reservoir` shell, you can do things like
adding domains, Resource Collections underneath domains,
and since both the domain and each Resource Collection
count as a Resource Index, you can use `index add` to
link to a Resource Collection throug its UUID (shown
symbolically below, as it is a random value).  You can
start at a domain's Resource Index with `index domain`
and the traverse locally added names with `index path`.
There may be cycles, such as the `buurman` below that
points from `Smid` to `Bakker` and back again.  The
paths are always finite-sized so you can make any
steps that seem good.

```
domain add orvelte.nep
collection add orvelte.nep Ambachten
collection add orvelte.nep Kunstenaars
collection add orvelte.nep Bakker
collection add orvelte.nep Brood
collection add orvelte.nep Banket
collection add orvelte.nep Smid
collection add orvelte.nep Hoefijzers
collection add orvelte.nep Schilden
collection add orvelte.nep Zwaarden
index domain orvelte.nep
index add {UUID_Kunstenaars} kunst
index add {UUID_Ambachten} ambacht
index path kunst
index add {UUID_Schilden} schilden
index add {UUID_Zwaarden} zwaarden
index add {UUID_Banket} banket
index add {UUID_Bakker} bakker
index add {UUID_Smid} smid
inded domain orvelte.nep
index path ambacht
index add {UUID_Schilden} schilden
index add {UUID_Zwaarden} zwaarden
index add {UUID_Hoefijzers} hoefijzers
index add {UUID_Brood} brood
index add {UUID_Banket} banket
index add {UUID_Bakker} bakker
index add {UUID_Smid} smid
index domain orvelte.nep
index collection {UUID_Smid}
index add {UUID_Schilden} schilden
index add {UUID_Zwaarden} zwaarden
index add {UUID_Hoefijzers} hoefijzers
index add {UUID_Bakker} buurman
index add {UUID_Kunstenaars} kunst
index add {UUID_Ambachten} ambacht
index path buurman
index add {UUID_Brood} brood
index add {UUID_Banket} banket
index add {UUID_Smid} buurman
index add {UUID_Kunstenaars} kunst
index add {UUID_Ambachten} ambacht
index domain orvelte.nep
index path kunst bakker brood
resource add file=/tmp/recept-krentemik.txt type=text/plain name=Recept_Krentemik
```

Make sure to hit `<tab>` often, it should work really
pleasantly.  Use `?command` or `help command` to inquire
about a specific command's syntax.

You can also use the `arpa2acl` shell to set the ACL for
the two kinds of Reservoir Index objects: the client's
domain as represented under the Reservoir root and the
various Reservoir Collections underneath the domain.
Resource are one step further down; these do not have
individual ACL settings but follow Resource Collections.

You can run `slapcat` from another shell to see the
changes in the database.  This command is basically
a dump of the contents of the OpenLDAP database in a
standardised exchange format, LDIF.  This is a rogue
utility, bypassing the protocol and authentication,
so it can only be run locally.

The shells are a bit rudimentary, but they may grow
into management utilities.  By that time we should
probably split the words over the underscore characters,
perhaps using the `cmdparser` package that was installed.


## Using LDAP

The container exports LDAP service over port 389.
It also runs locally on `/tmp/ldap-socket`, reachable with LDAP URI
`ldapi://%2ftmp%2fldap-socket` -- mind the scheme's extra `i` and note
that path separators are escaped to `%2f` to not mix them with URI syntax.

A query that could be made would be

```
ldapsearch -w sekreet -D uid=demo,ou=IdentityHub,o=arpa2.net,ou=InternetWide -b ou=InternetWide labeledURI
```

You should get the `dn` and `labeledURI` for all objects underneath the
base option `-b ou=InternetWide` that are in the database.  The initial data
is loaded from `/root/initial.ldif` as part of the startup script.

This includes the standard `rootdn` and `rootpw` as setup
in `/etc/ldap/slapd.conf` for the server, and for which the `BINDDN` but
not the password is setup in `/etc/ldap/ldap.conf` for the client.


## Using the ACL Shell

The `arpa2acl` shell can be used to select a node in LDAP.
Reservoir nodes are an example, but others are just as valid.
In general, any object with a default setup as an
`objectClass: accessControlledObject` can be edited, and
the Reservoir is just one example of an application that
creates such objects.  The IdentityHub should do the same
to `uid` style objects, to make their access controllable.

There are a few types of ACL, and they relate to the
[kinds of objects controlled](http://donai.arpa2.net/acl-impl.html):

  * `uidXXX` style objects are usually access-controlled
    through a Communication ACL;
  * `resourceClassObject` typed objects are representative
    of a resource class (perhaps the general idea of Git service);
  * `resourceInstanceObject` typed objects are representative
    of a resource instance (perhaps a Git repository).

To setup an ACL, one specifies the LDAP path of an object,
so its `dn` or `distinguishedName`.  Interestingly, the
first/lowest RDN in the path may give away details to identify
the ACL object; an attribute (possibly inherited) of `uid`
for a Communication ACL object, or a `rescls` or `resins` for
a resource class and resource instance, respectively.  For
instance in the Reservoir, we have a nesting `resins=, rescls=, ...`
that can be helpful.

Finally, resources are specific to a domain name, which serves
as a realm.  This can be found in the `dn` as the user domain.
For Reservoir, one would have `associatedDomain=orvelte.nep,
ou=Reservoir, o=arpa2.net, ou=InternetWide` if `orvelte.nep`
is the user domain.  Directly underneath would be `resins=,
rescls=` levels.  Note that there is no more nesting than
this, even when the cross-references between `rescls` objects
may suggest otherwise.

```
shell# arpa2acl 
Edit Access Control Lists as saved in LDAP.

arpa2acl> acl_dn resourceInstance={UUID_Zwaarden},associatedDomain=orvelte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
#EXAMPLE# acl_dn resourceInstance=bcc718d8-a25f-4580-8259-2cdb7af20e92,associatedDomain=orvelte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: bcc718d8-a25f-4580-8259-2cdb7af20e92
cn: Zwaarden

arpa2acl> acl_add %k @orvelte.nep
('Adding rights', '%k', 'to selector', '@orvelte.nep')

arpa2acl> acl_add %w smid@orvelte.nep
('Adding rights', '%w', 'to selector', 'smid@orvelte.nep')

arpa2acl> acl_save
('OLD =', [])
('NEW =', ['0', '%w', 'smid@orvelte.nep', '%k', '@orvelte.nep'])
('MOD =', [(0, 'accessControlList', '0 %w smid@orvelte.nep %k @orvelte.nep')])
```

After this action, `slapcat` would reveal something like this for the `Zwaarden`
object:

```
dn: resourceInstance=bcc718d8-a25f-4580-8259-2cdb7af20e92,associatedDomain=o
 rvelte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
objectClass: reservoirCollection
objectClass: resourceInstanceObject
objectClass: accessControlledObject
objectClass: reservoirIndex
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: bcc718d8-a25f-4580-8259-2cdb7af20e92
cn: Zwaarden
accessControlList: 999 %wr demo@orvelte.nep
accessControlList: 0 %w smid@orvelte.nep %k @orvelte.nep
structuralObjectClass: reservoirCollection
entryUUID: 74b5a61c-6da8-1039-9f07-67b874f43b45
creatorsName: uid=demo,associatedDomain=arpa2.net,ou=identityhub,o=arpa2.net
 ,ou=internetwide
createTimestamp: 20190917150622Z
entryCSN: 20190917153219.162745Z#000000#000#000000
modifiersName: uid=demo,associatedDomain=arpa2.net,ou=identityhub,o=arpa2.ne
 t,ou=internetwide
modifyTimestamp: 20190917153219Z
```

Much better would of course be to query using LDAP, for instance assuming that
you have a Kerberos login,

```
shell$ ldapsearch -H ldapi://%2ftmp%2fldap-socket \
	-Y gssapi \
	-b associatedDomain=orvelte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide \
	'(&(objectClass=accessControlledObject)(objectClass=resourceInstanceObject))' \
	resourceClass resourceInstance accessControlList
```

which could yield something like

```
# extended LDIF
#
# LDAPv3
# base <associatedDomain=orvelte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide> with scope subtree
# filter: (&(objectClass=accessControlledObject)(objectClass=resourceInstanceObject))
# requesting: resourceClass resourceInstance accessControlList 
#

# cad30662-6d31-479e-810c-022af25e04db, orvelte.nep, Reservoir, arpa2.net, Inte
 rnetWide
dn: resourceInstance=cad30662-6d31-479e-810c-022af25e04db,associatedDomain=orv
 elte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: cad30662-6d31-479e-810c-022af25e04db
accessControlList: 999 %wr demo@orvelte.nep

# 58113855-5c25-4472-a37b-c62768e8ed9d, orvelte.nep, Reservoir, arpa2.net, Inte
 rnetWide
dn: resourceInstance=58113855-5c25-4472-a37b-c62768e8ed9d,associatedDomain=orv
 elte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: 58113855-5c25-4472-a37b-c62768e8ed9d
accessControlList: 999 %wr demo@orvelte.nep

# 7c63433e-c603-4e62-b309-b89c008495b1, orvelte.nep, Reservoir, arpa2.net, Inte
 rnetWide
dn: resourceInstance=7c63433e-c603-4e62-b309-b89c008495b1,associatedDomain=orv
 elte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: 7c63433e-c603-4e62-b309-b89c008495b1
accessControlList: 999 %wr demo@orvelte.nep

# cdc98770-b09e-41c7-972c-eea34dada600, orvelte.nep, Reservoir, arpa2.net, Inte
 rnetWide
dn: resourceInstance=cdc98770-b09e-41c7-972c-eea34dada600,associatedDomain=orv
 elte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: cdc98770-b09e-41c7-972c-eea34dada600
accessControlList: 999 %wr demo@orvelte.nep

# 3cfde35a-98aa-4289-beb1-1649c52dba13, orvelte.nep, Reservoir, arpa2.net, Inte
 rnetWide
dn: resourceInstance=3cfde35a-98aa-4289-beb1-1649c52dba13,associatedDomain=orv
 elte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: 3cfde35a-98aa-4289-beb1-1649c52dba13
accessControlList: 999 %wr demo@orvelte.nep

# 5d6f9da0-6867-47c6-8b3e-d67a519eb31f, orvelte.nep, Reservoir, arpa2.net, Inte
 rnetWide
dn: resourceInstance=5d6f9da0-6867-47c6-8b3e-d67a519eb31f,associatedDomain=orv
 elte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: 5d6f9da0-6867-47c6-8b3e-d67a519eb31f
accessControlList: 999 %wr demo@orvelte.nep

# 7d9c75a4-6c6d-4f3b-8c53-f5c95dd0e6c4, orvelte.nep, Reservoir, arpa2.net, Inte
 rnetWide
dn: resourceInstance=7d9c75a4-6c6d-4f3b-8c53-f5c95dd0e6c4,associatedDomain=orv
 elte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: 7d9c75a4-6c6d-4f3b-8c53-f5c95dd0e6c4
accessControlList: 999 %wr demo@orvelte.nep

# c0743b9d-826d-43ac-83c2-ab915d974e60, orvelte.nep, Reservoir, arpa2.net, Inte
 rnetWide
dn: resourceInstance=c0743b9d-826d-43ac-83c2-ab915d974e60,associatedDomain=orv
 elte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: c0743b9d-826d-43ac-83c2-ab915d974e60
accessControlList: 999 %wr demo@orvelte.nep

# bcc718d8-a25f-4580-8259-2cdb7af20e92, orvelte.nep, Reservoir, arpa2.net, Inte
 rnetWide
dn: resourceInstance=bcc718d8-a25f-4580-8259-2cdb7af20e92,associatedDomain=orv
 elte.nep,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: bcc718d8-a25f-4580-8259-2cdb7af20e92
accessControlList: 999 %wr demo@orvelte.nep
accessControlList: 0 %w smid@orvelte.nep %k @orvelte.nep

# search result
search: 4
result: 0 Success

# numResponses: 10
# numEntries: 9
```

Similar queries can be made in other languages.  The example given is for
querying ACL instances, which in terms of Reservoir are Resource Collections
as they define individual ACL setups.  If you were to program tooling to
extract this information for management purposes covering all domains, you
might want to be a bit more general, and say

```
shell$ ldapsearch -H ldapi://%2ftmp%2fldap-socket \
	-Y gssapi \
	-b ou=Reservoir,o=arpa2.net,ou=InternetWide \
	'(&(objectClass=accessControlledObject)(objectClass=resourceInstanceObject))' \
	resourceClass resourceInstance accessControlList
```

If you wanted to address ACL instances in general, you could use

```
shell$ ldapsearch -H ldapi://%2ftmp%2fldap-socket \
	-Y gssapi \
	-b ou=InternetWide \
	'(&(objectClass=accessControlledObject)(objectClass=resourceInstanceObject))' \
	resourceClass resourceInstance accessControlList
```

Similarly, for ACL classes covering many applications you could use

```
shell$ ldapsearch -H ldapi://%2ftmp%2fldap-socket \
	-Y gssapi \
	-b ou=InternetWide \
	'(&(objectClass=accessControlledObject)(objectClass=resourceClassObject))' \
	resourceClass accessControlList
```

And finally, Communication ACL entries would all be part of IdentityHub, and could
be found using something like

```
shell$ ldapsearch -H ldapi://%2ftmp%2fldap-socket \
	-Y gssapi \
	-b ou=IdentityHub,o=arpa2.net,ou=InternetWide \
	'(&(objectClass=accessControlledObject)(|(objectClass=identityHubUser)(objectClass=identityHubGroup)(objectClass=identityHubRole))' \
	accessControlList uid uidAlias uidPseudonym uidGroup uidRole uidMember uidOccupant uidDecline
```


## Image Updates

We currently expect some preparation to building, namely the
addition of `riak_2.2.3-1_amd64.deb` in the current directory.
It is rather big for inclusion in Git, and downloading it is
a bit of a problem given the download infrastructure.

To update this image, please build it with the public tag and
push it:

```
docker tag rv arpa2/reservoir
docker push arpa2/reservoir
```

Cleaning up is easy in docker, but also necessary because of
the dangling images and containers:

```
docker container prune
docker image     prune
```


