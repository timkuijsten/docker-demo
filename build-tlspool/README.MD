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

As soon as this shell exists, its container stops.

While it is running, you can run tests on other
shells through, for instance

```
docker exec -it tipi0 testcli
```

and

```
docker exec -it tipi0 testsrv
```

**TODO:**
The `testcli` and `testsrv` programs rely on IPv6, but Docker is reluctant to support it.
User-defined networks can remedy this, it is said.  Without those, the following option is rejected:

```
--ip6 2001:db8::33 
```

