# README for Docker Image arpa2/demo-reservoir

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

  * MusicLister op port 8811 is Henri's minimal-yet-functional Python Twisted webinterface to music
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
docker run --detach --hostname reservoir.arpa2 -p 1388:1388 -p 1761:1761 -p 5672:5672 -p 8811:8811 -p 1883:1883 -p 2020:22 --name rv0 rv
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

Or, you could use SSH from your host and request to start
the appropriate subsystem:

```
ssh -p 2020 -s localhost arpa2shell
```

This is a special shell for doing ARPA2 manipulations.
In fact, it is a meta-shell that can drop in on various
ARPA2 sub-shells and allows switching between them.
This is more powerful than just opening a single shell,

```
docker exec -ti rv0 arpa2reservoir
```

or,

```
ssh -p 2020 -s localhost arpa2reservoir
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

Alternatively, if you use SSH from your host with Kerberos
access (UNTESTED), you might use your existing single-signon
account or the same one as above:

```
kinit
#or#
kinit demo@ARPA2.NET
```

To drop Kerberos in the container, instead run

```
arpa2kinit @
```

on the host system, you would instead use

```
kdestroy
```


On the commandline of the container, you could also run the shells,

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


## Listen for Changes with MQTT

The demo runs Mosquitto as an MQTT Broker in the background.
The Reservoir API, which is used in pretty much everything
described here, will send notifications whenever a Resource
is added, deleted or has changed.  The message content for
these events is `ADD`, `DEL` and `CHG`, respectively.

The topics used are formed like `/reservoiruuid/domain/colluuid/resuuid`,
where `reservoiruuid` is the fixed value that we use across
protocols, `904dfdb5-6b34-3818-b580-b9a0b4f7e7a9` and the
`colluuid` and `resuuid` are Collection and Resource UUID
values.

When the ACL is enforced, the required access right for
listening for changes will be `K`, for Knowing-About,
and it will be bound to `/reservoiruuid/colluuid` and
thus inform the client about any resource changes in the
indicated Collection.  Until this is in place, you can:

```
mosquitto_sub -t /904dfdb5-6b34-3818-b580-b9a0b4f7e7a9/arpa2.org
```

We have a few ideas for extensions in the future:

  * Inform about Index name additions and removals
  * Allow users to form an (authoritative) path
  * Allow the `user@domain` form instead of just `domain`


## Use from JSON

There is (EARLY) support for standardised
[JSON messaging with the shells](https://github.com/arpa2/arpa2shell/blob/master/PROTOCOL.MD)
for which we start an `arpa2api.d` in the
background.  If you run `arpa2api` as a command, or
ask SSH for that subsystem, you will have an input/output
system for exchanging JSON packages.  These will look
slightly different from the commandline format presented
in the examples below, as explained at the link above.

In addition, there is an AMQP interface to the
`arpa2apid` which runs the
[complete stack](https://github.com/arpa2/arpa2shell/blob/master/PROTOCOL.MD#authenticated-json-between-microservices)
in service of microservices.  Those microservices
could run their own `arpa2api`, either as a
standalong or through 

FUTURE versions will use your credentials from the environment
that invokes the `arpa2api`, so your host when you call
it yourself or otherwise the SSH session to the container.
These credentials are passed on via the stack to the
`arpa2apid`, which passes them on to the shells.  The
shells finally use the credentials for access control decisions.

FUTURE versions will export these services, both over SSH and
AMQP, to anyone who wants to have them.  Whether they get
anything done will be completely up to access control.  This
[backbone innovation](http://www.internetwide.org/blog/2018/11/22/backbone-innovations.html)
allows us to accept requests from microservices hosted by
others, which we will need in the upcoming
[ServiceHub phase](http://internetwide.org/blog/2016/06/24/iwo-phases.html)
of the project.


## Use the prepared Reservoir Domain

The music collection from the `arpa2/files-reservoir` Docker Demo are loaded
in `/var/arpa2/reservoir` and the shell `arpa2reservoir` uses those as
file store, with metadata in LDAP.  The additional links and a few
descriptive files from the demo are ignored.

The following commands can now be tried to start at the
default collection for domain `arpa2.org`, then follow
index links, first `Negrin` then `Whispering`, and list
resources.

```
index domain arpa2.org
index path Negrin Whispering
resource list
resource get efd7a299-2b35-457d-83af-25fd54697e99
```

The latter dumps a file on standard output, which we should
improve on, but in this case it is a simple text file, so no
harm done.  Negrin has a habit of using Creative Commons
licenses, yet deliver interesting guitar music with classical
inspirational links.  Enjoy!


## Setup your own Reservoir Domain

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
instance in the Reservoir, we have a nesting
`resource=, resins=, ..., ou=Reservoir,o=arpa2.net,ou=InternetWide`
that can be helpful.  Although `rescls` is not shown in the
DN, the `ou=Reservoir,o=arpa2.net,ou=InternetWide` says the
same thing, it marks the boundaries of the application, which
in turn specifies the `resourceClass`, which is also part of
the `resins=` LDAP objects.

Finally, resources are specific to a domain name, which serves
as a realm.  This can be found in the `dn` as the user domain.
For Reservoir, one would have `associatedDomain=orvelte.nep,
ou=Reservoir, o=arpa2.net, ou=InternetWide` if `orvelte.nep`
is the user domain.  Directly underneath would be `resource=,
resins=` levels.  Note that there is no more nesting than
this, even when the cross-references between `rescls` objects
may suggest otherwise.

```
shell# arpa2acl 
Edit Access Control Lists as saved in LDAP.

arpa2acl> acl_dn resins=0878c424-e544-43cd-b1c4-71f8c7f76237,associatedDomain=arpa2.org,ou=Reservoir,o=arpa2.net,ou=InternetWide

resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: 0878c424-e544-43cd-b1c4-71f8c7f76237
cn: Negrin
reservoirRef: 9673fd11-6d10-48a1-905b-8f8599a0584f Whispering
accessControlList: 999 %wr demo@arpa2.org

arpa2acl> acl_add %r bakker@orvelte.nep

arpa2acl> acl_del demo@arpa2.org
Removing rights %wr from selector demo@arpa2.org

arpa2acl> acl_save

arpa2acl> acl_dn resins=0878c424-e544-43cd-b1c4-71f8c7f76237,associatedDomain=arpa2.org,ou=Reservoir,o=arpa2.net,ou=InternetWide

resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: 0878c424-e544-43cd-b1c4-71f8c7f76237
cn: Negrin
reservoirRef: 9673fd11-6d10-48a1-905b-8f8599a0584f Whispering
accessControlList: %r bakker@orvelte.nep

```

Instead of this other `acl_dn` call, you could also try with `slapcat`
what has changed.  Or you could of course query LDAP, for instance
assuming that you have a Kerberos login after `arpa2kinit` succeeded,

```
shell$ ldapsearch -H ldapi://%2ftmp%2fldap-socket \
	-Y gssapi \
	-b associatedDomain=arpa2.org,ou=Reservoir,o=arpa2.net,ou=InternetWide \
	'(&(objectClass=accessControlledObject)(objectClass=resourceInstanceObject))' \
	resourceClass resourceInstance accessControlList
```

which would yield the ACL definitions underneath this domain's Reservoir:

```
SASL/GSSAPI authentication started
SASL username: demo@ARPA2.NET
SASL SSF: 256
SASL data security layer installed.
# extended LDIF
#
# LDAPv3
# base <associatedDomain=arpa2.org,ou=Reservoir,o=arpa2.net,ou=InternetWide> with scope subtree
# filter: (&(objectClass=accessControlledObject)(objectClass=resourceInstanceObject))
# requesting: resourceClass resourceInstance accessControlList 
#

# 0878c424-e544-43cd-b1c4-71f8c7f76237, arpa2.org, Reservoir, arpa2.net, Intern
 etWide
dn: resourceInstance=0878c424-e544-43cd-b1c4-71f8c7f76237,associatedDomain=arp
 a2.org,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: 0878c424-e544-43cd-b1c4-71f8c7f76237
accessControlList: %r bakker@orvelte.nep

# 9673fd11-6d10-48a1-905b-8f8599a0584f, arpa2.org, Reservoir, arpa2.net, Intern
 etWide
dn: resourceInstance=9673fd11-6d10-48a1-905b-8f8599a0584f,associatedDomain=arp
 a2.org,ou=Reservoir,o=arpa2.net,ou=InternetWide
resourceClass: 904dfdb5-6b34-3818-b580-b9a0b4f7e7a9
resourceInstance: 9673fd11-6d10-48a1-905b-8f8599a0584f
accessControlList: 999 %wr demo@arpa2.org

# search result
search: 4
result: 0 Success

# numResponses: 3
# numEntries: 2
```

A similar query, albeit not domain-specific, is done by the software that
downloads access rigths for checking at the sites where they are needed.
The difference is that this is not a `search` operation, but a synchronising
variant, that continues to receive updates to the original search result.
This is done fast, so the synchronised listeners (which we implement with
SteamWorks Pulley) continually receive additions and removals that they can
disperse to their backends for (virtually) immediate changes to access control
in the nooks and crannies of the InternetWide Architecture.  Such fast updates
are helpful when permissions must be retracted as fast as possible.

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


