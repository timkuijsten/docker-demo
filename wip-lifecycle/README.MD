# README for Docker Image arpa2/lifecycle

> *This is an LDAP server with LifeCycle support added.
> It mostly demonstrates how this can be made to work.*

This container currently holds:

  * OpenLDAP, exported on port 1389, configured for IdentityHub.
  * SteamWorks, notably the `pulley` daemon
  * Lifecycle backend for Pulley

## Deploy the Image


## ARPA2 Shell Access


## Using LDAP

The container exports LDAP service over port 1389.
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


