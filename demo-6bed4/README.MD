# 6bed4 -- IPv6 routing for Docker

> *The 6bed4 tunnel enables IPv6 anywhere, often with direct
> connections over IPv4/UDP and only rarely with a fallback
> through server infrastructure.  This image enables IPv6
> for Docker, and can be used on any network to route on
> behalf of the connected interface.*


To start, special capabilities are required:

```
#FAILS# docker run -it --cap-add=NET_ADMIN --name 6bed4router demo-6bed4
```

This is not enough to run the `6bed4peer` command.   What seems to work is

```
#GRAB-A-LOT#
docker run -it --privileged --sysctl net.ipv6.conf.all.disable_ipv6=0 --rm --name 6bed4node demo-6bed4
```

In the container, you can now run:

```
6bed4peer -ref
```

After this, you can use IPv6 traffic.  Note that IPv6 setup for the
host network might also specify a default route, and that this will
only work if it is indeed routed properly.



## There are problems with privileges

  * Normally a container, even with IPv6 enabled, can do `ip -4 a a`
    but not `ip -6 a a`.

  * You can't set a variable before it exists, as in:

    ```
    --sysctl net.ipv6.conf.6bed4.forwarding=1
    ```

    but that's because there is no IPv6 support; you need:

    ```
    --sysctl net.ipv6.conf.all.disable_ipv6=0
    ```

  * The combination `--cap-add=NET_ADMIN --cap-add=SYS_ADMIN` does not
    work.

  * We needed `--privileged` to get this working, but it's pretty big.
    Apparently, this is a
    [known problem](https://stackoverflow.com/questions/23537560/docker-build-read-only-file-system)
    with no work-around but to be all-mighty.
