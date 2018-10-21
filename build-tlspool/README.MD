# TLS Pool build environment

This environment contains a working TLS Pool.

Since setting up the TLS Pool is difficult, due
to its many dependencies, we demonstrate how it
can be done.

The configuration run is based on testdata, and
not fit for live environments.  But it is useful
for testing and experimentation.  Hence the name
of this Docker image as build-* not demo-*

The main process started in this environment
runs the TLS Pool and, 10 seconds later, a shell
that should be kept alive.  This means that it
should be run with -it,

```
docker run -it --name tipi0 arpa2:build-tlspool
```

or, if you want to use `gdb` or `strace`,

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

## Test Client and Server

While it is running, you can run tests on other
shells through, for instance

```
docker exec -it tipi0 testcli
```

and

```
docker exec -it tipi0 testsrv
```

## Need IPv6 for Test

The `testcli` and `testsrv` programs rely on IPv6, but Docker can be reluctant to support it.
User-defined networks can remedy this, it is said.  Without those, the following option is rejected:

```
--ip6 2001:db8::33 
```

Note, there is an
[IPv6 setting](https://docs.docker.com/config/daemon/ipv6/)
in the Docker configuration JSON file.  It is wrong however,
and one should
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

