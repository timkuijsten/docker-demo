# 6bed4 -- IPv6 routing for Docker

> *The 6bed4 tunnel enables IPv6 anywhere, often with direct
> connections over IPv4/UDP and only rarely with a fallback
> through server infrastructure.  This image enables IPv6
> for Docker, and can be used on any network to route on
> behalf of the connected interface.*


To start, special capabilities are required:

```
docker run -it --cap-add=NET_ADMIN --name 6bed4router demo-6bed4
```

In the container, you can now run:

```
6bed4peer -erf
```

There are problems with privileges:

  * You can't set a variable before it exists, as in:

    ```
    --sysctl net.ipv6.conf.6bed4.forwarding=1
    ```

  * We considered `--privileged` but that's pretty big.
    It will get you to the point that you get an IPv6 address assigned,
    but fail assigning it to the tunnel:

    ```
    6bed4peer[55]: 6bed4peer: Assigning address XXXX to tunnel
    RTNETLINK answers: Permission denied
    ```

