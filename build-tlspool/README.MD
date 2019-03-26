# TLS Pool build environment

This environment contains a working TLS Pool.

Since setting up the TLS Pool is difficult, due
to its many dependencies, we demonstrate how it
can be done.

The main difficulties lie in the configuration
of things like credential access rights, in the
somewhat shifty underlying software caused by
the complexity of TLS and PKCS \#11 management
through p11-kit.

Finally, currently an unsolved problem is the
integration of the TLS Pool into concrete
applications.  We have started work on a module
for the Apache HTTP server, however.  When we
add TLS to LDAP solutions based on LillyDAP, we
will also use the TLS Pool for doing this.


## Warnings

The configuration run is based on testdata, and
not fit for live environments.  But it is useful
for testing and experimentation.  Hence the name
of this Docker image as `build-*` not `demo-*`.

Keep in mind that test data holds expiration datas.
Without a solution (such as IdentityHub) to cycle
certificates and deliver them in the database of
the TLS Pool, you may find it an awful tool to
work with.  Once you have your identity infras
setup however, it should ease operations immensely,
especially because TLS can be centralised.

The current TLS Pool is designed for servers,
not clients.  Integration with desktops is a
level of problem that we are not actively
addressing, though solutions may exist through
SOCKS5 and/or HTTPS proxies.  To manage such an
infrastructure, one might consider the Pulley or
a regular check of the certificates that the
IdentityHub is intended to manage.  We are always
happy to talk to technical people who invest in
open source developent, and will happily guide
them; it is just not our main focus in our work
on an InternetWide Architecture of a hosting
stack that helps users regain control over their
online identity.  Despite that focus, our group
is not closed to your wider application of our
work!

The TLS Pool currently relies on GnuTLS and its
underlying p11-kit.  This may change in the
future.  Specifically the p11-kit does not impress
in documentation, and given a thorough understanding
of PKCS \#11 it is not clear what problem it solves.
Iteration over multiple tokens may be more of an
end user thing than a server thing.

The TLS Pool itself is also a bit fidgety.  It
won't run well when the syslog is not present,
for example.  And it relies on BerkeleyDB, which
can get in a desparate mode, requiring fixing.


## Future Directions

The TLS Pool is a research project, aimed at
testing the concept of a detached solution for
TLS in a variety of applications.  The intention
was to make life easy on new applications that
want to focus on application logic, rather than
on security.  Indeed, this is very easy with the
TLS Pool, so security can really be everywhere.

Applications that currently embed an existing
TLS layer are more difficult to transition
however, because they make strong assumptions such
as the use of X.509 only and setting it all up in
the application.  Such applications are not the
direct purpose of the TLS Pool, however we are
making a run for a module for the Apache HTTP
server.

We are considering to alter the communication
protocol to the TLS Pool, so it can be used in
more performant applications such as the Nginx
proxy.  We are working on Symmetric TLS, which
means that peer-to-peer systems are not forced
to make a decision who serves as client and who
as server just to be able to use TLS.  And we
are still working on a validation control language
to be used in trust settings.

TLS-KDH is a serious direction of work, and it
is also one of the reasons we think a TLS Pool
can greatly enhance user experience.  This exchange
has hundreds to thousands of times faster authentication
based on Kerberos5; in fact, it is the only option
that we are aware of that can get you protection
against Quantum Computers that crack any session
captured today with the intention of decrypting
it once this new technology arrives.

We do have an idea about TLS Pool usage in a
client platform setting, but this has not been
worked out yet.  First, we need to see results
on the server side.


## Starting sessions

You probably want one main process,

```
docker run -it --name tipi0 arpa2:build-tlspool
```

or, if you want to be able to use `gdb` or `strace`,

```
docker run -it --name tipi0 --security-opt seccomp=unconfined build-tlspool
```

other processes should not restart the TLS Pool,
as that would kill the other one running, and
should have better results from running an
explicit `bash` process:

```
docker exec -it tipi0 bash
```

As soon as this shell exists, its container stops.


### Need IPv6 for Test

The `testcli` and `testsrv` programs rely on IPv6, but Docker can be old-fashioned and seems to not support it by default.
There is an
[IPv6 setting](https://docs.docker.com/config/daemon/ipv6/)
in the Docker configuration JSON file.  This documentation is incomplete however, and one should
[read this instead](https://docs.docker.com/v17.09/engine/userguide/networking/default_network/ipv6/#how-ipv6-works-on-docker).

In brief, you should set something like this in the JSON configuration:

```
{
  ...
  "ipv6": true,
  "fixed-cidr-v6": "2001:db8:1::/64"
}
```

You can probably use another prefix if you have it.

On Mac OS X, this is found in the menu bar, under Docker preferences.
If you screw up, you can always reset Docker to factory defaults, but
you won't be able to save your settings first `:-S`  and oh, it will
also free all your containers and images...

Note that if you don't have routeable IPv6, you may run into problems
with the Web Demo.  The Test Client and Test Server use IPv6 locally,
so they should not experience routing difficulties.  But to be honest,
it is a bit embarrassing to still have to explain backward compatibility
to IPv6 `:'-(`


## Starting the TLS Pool

The TLS Pool logs plenty of information.
You have to first start logging through

```
/etc/init.d/rsyslog start
```

You can now start the TLS Pool with

```
tlspool-daemon -kc /etc/tlspool.conf
```

For your convenience, we provide a script
`/tlspool.sh` that will do both but not
return to the prompt.  It is quite useful
as a command to `docker run` too, as it
will continue to run independent of being
run interactively, but it will continue
to output the TLS Pool messages.


## Test Client and Server

While the TLS Pool is running, you can run tests on other
shells through, for instance

```
docker exec -it tipi0 tlspool-test-client
```

and

```
docker exec -it tipi0 tlspool-test-server
```

The two programs function as a line-based chat system.
The exchange is protected by the TLS Pool, and it will
refuse to start if something is wrong (like, expired or
absent credentials).

The identities setup by these utilities are

```
testcli@tlspool.arpa2.lab
testsrv@tlspool.arpa2.lab
```

## Web Demo in Python

There is a Python support library, and it
comes with a web demonstration.  It downloads
a page from a website, going through the
TLS Pool.  The same concept of wrapping a
socket into TLS is also what the built-in
SSL library does!

You would run this demo with

```
webdemo.py nlnet.nl [/var/run/tlspool.sock]
```

If your IPv6 network is local to your
Docker container, then please accept that
we need you to do some backward compatibility
work.  You need to change the `AF_INET6` in the
script into `AF_INET` to continue to use IPv4.
While doing so, please ask yourself if it is
fair that you should have to do this.  Why
didn't you make a point out of upgrading the
core protocol of the planet's most vital
infrastructure?  The upgrade that will get
you to peer-to-peer technology and away from
constant prying into your activities through
intermediate servers that are now needed to
bounce traffic between end users because
NAT traversal is not a generally solvable
problem?  The impact of not being IPv6
compliant is rather dire, but it is an
individual choice whether you are part of
that problem.

