# README for Docker Image arpa2:reservoir

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

To build this image on top of the `arpa2:base` image:

```
docker build -t rv <THIS-DIRECTORY>
```

The `slapd` is on the main terminal, outputing tons of debugging
information:

```
docker run --detach --network host --hostname reservoir.arpa2 -p 1388:1388 -p 1761:1761 --name rv0 rv
docker cp py/lib/python2.7/site-packages/ rv0:/dist-packages/
docker exec rv0 mv /dist-packages/* /usr/lib/python2.7/dist-packages/
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
# Alas, this complains about envvars that are not set
docker exec -ti rv0 arpa2shell
```

This is a special shell for doing ARPA2 manipulations.
In fact, it is a meta-shell that can drop in on various
ARPA2 sub-shells and allows switching between them.
This is more powerful than just opening a single shell,

```
# Alas, this complains about envvars that are not set
docker exec -ti rv0 arpa2shell
```

You can get a normal shell with

```
docker exec -ti rv0 bash
```

On the shell, you could also run the shells,

```
arpa2reservoir
arpa2shell arpa2reservoir # and later more sub-shells
```

Note that the set of domains is independent from those
used in IdentityHub.  They reside in different parts
of the tree, to help with situations where not all
domains have the same service, where subdomains have
additional service, where customers take out only one
but not both the services, and so on.

Once on an `arpa2reservoir` shell, you can do things like

```
domain_add orvelte.nep Orvelte, Incorporated
#TODO# user_add orvelte.nep bakker Hij die bakt
#TODO# user_add orvelte.nep smid Hij die hakt
#TODO# user_del orvelte.nep bakker
#TODO# user_del orvelte.nep smid
domain_del orvelte.nep
```

Make sure to hit `<tab>` often, it should work really
pleasantly.  Use `?command` or `help command` to inquire
about a specific command's syntax.

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
ldapsearch -w sekreet -D uid=admin,ou=IdentityHub,o=arpa2.net,ou=InternetWide -b ou=InternetWide labeledURI
```

You should get the `dn` and `labeledURI` for all objects underneath the
base option `-b ou=InternetWide` that are in the database.  The initial data
is loaded from `/root/initial.ldif` as part of the startup script.

This includes the standard `rootdn` and `rootpw` as setup
in `/etc/ldap/slapd.conf` for the server, and for which the `BINDDN` but
not the password is setup in `/etc/ldap/ldap.conf` for the client.


## Image Updates

We currently expect some preparation to building, namely the
addition of `riak_2.2.3-1_amd64.deb` in the current directory.
It is rather big for inclusion in Git, and downloading it is
a bit of a problem given the download infrastructure.

To update this image, please build it with the public tag and
push it:

```
docker tag rv arpa2:reservoir
docker push arpa2:reservoir
```

Cleaning up is easy in docker, but also necessary because of
the dangling images and containers:

```
docker container prune
docker image     prune
```


