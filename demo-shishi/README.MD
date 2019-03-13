# ShiShi Kerberos5 Demonstration

> *This is very simple; just a KDC for SHISHI.ARPA2.LAB and a few
> simple accounts.  It serves as a basis on which to build
> other demonstrations.*

To start, run these commands:

    /etc/init.d/shishi-kdc start

To obtain a ticket, run:

    kinit demo
    Password for demo@SHISHI.ARPA2.LAB: sekreet

You can now inspect your tickets with:

    klist

If you need to add or remove principals, run:

    shisa --add/... $REALM $USER --password=$PASS

An interesting thing to note is that `shisa` is used
to add *everything*, even the realm and its TGS ticket,

    shisa --add $REALM
    shisa --add $REALM krbtgt/$REALM

After this, user accounts can be added that could then
request the TGS ticket.

**Note:** We have installed `krb5.conf` not because
ShiShi needs it; it exists to service the MIT krb5
client programs, such as `kinit`, `klist`, `kdestroy`.


## Realm Crossover

When two realms want to crossover, they need to traverse
a path of tickets.  We have already instructed the
Kerberos utilities that the realm `PORTAL.ARPA2.LAB` also
runs on `localhost`, as does the `SHISHI.ARPA2.LAB` realm.
But the realm does not yet exist.

To create the second realm, enter the following:

```
shisa --add PORTAL.ARPA2.LAB
```

Realms need a ticket for their ticket granting service,
to be held upon login, so we shall install that ticket
(though it is only used for the realm's own clients)
because this is normal but not automatically done by
ShiShi upon realm creation:

```
shisa --add PORTAL.ARPA2.LAB krbtgt/PORTAL.ARPA2.LAB
```

Note that we did not specify a password or key, so ShiShi
generates one at random.  The same can be done to create
the key of an imaginary webserver running under this
secondary realm:

```
shisa --add PORTAL.ARPA2.LAB HTTP/www.demo.arpa2.lab
```

The client resides under another realm, so it cannot
access this key.  To get to it, there would have to
be a sequence of links that connects the client to
this service ticket.

We need a crossover key.  This is one that also allows
the use of the ticket granting service, but this time
it should be reachable under the realm of our
`demo@SHISHI.ARPA2.LAB` account:

```
shisa --add SHISHI.ARPA2.LAB krbtgt/PORTAL.ARPA2.LAB
```

Normally, the realms `SHISHI` and `PORTAL` would now
have to exchange this key, because it needs to be
present on the KDC of the client's realm and the KDC
of the service realm.  In this simple setup, both
realms run on the same KDC, so it is trivially taken
care of.  Sheesh!

We can use a default client `kvno` to retrieve a
service ticket, which we first ask for the service
host in the local realm, which will fail:

```
kvno HTTP/www.demo.arpa2.lab
```

(Normally, the protocol is in lowercase, but `HTTP`
is a historic wild-growth exception.)

You can see that no ticket was found by running
`klist` and seeing no service ticket for this
host.

The client or its KDC are responsible for finding
out the realm of the service.  This might be done
with a clever trick in DNS, but we shall simulate
its effect and add it manually:

```
kvno HTTP/www.demo.arpa2.lab@PORTAL.ARPA2.LAB
```

After this statement, you would have a ticket for
the `HTTP/www.demo.arpa2.lab` service, which can
only be had through realm crossover.

Now inspect your keys with `klist` and see the
following:

```
Ticket cache: FILE:/tmp/krb5cc_0
Default principal: demo@SHISHI.ARPA2.LAB

Valid starting     Expires            Service principal
03/13/19 16:29:31  03/14/19 16:29:31  krbtgt/SHISHI.ARPA2.LAB@SHISHI.ARPA2.LAB
03/13/19 17:24:47  03/14/19 16:29:31  krbtgt/PORTAL.ARPA2.LAB@SHISHI.ARPA2.LAB
03/13/19 17:24:57  03/14/19 16:29:31  HTTP/www.demo.arpa2.lab@PORTAL.ARPA2.LAB
```

You can see the path, consisting of:

  * `demo@SHISHI.ARPA2.LAB`
  * `krbtgt/SHISHI.ARPA2.LAB@SHISHI.ARPA2.LAB`
  * `krbtgt/PORTAL.ARPA2.LAB@SHISHI.ARPA2.LAB`
  * `HTTP/www.demo.arpa2.lab@PORTAL.ARPA2.LAB`

Note that you would normally not do all this work
manually; the realms and tickets would have been
prepared by an administrator or tooling, and the
last statement reflects what would have been done
right away.  In other words, you would silently
have obtained the crossover ticket.  This also
works when the client KDC and service KDC are on
different hosts; it does not matter.
