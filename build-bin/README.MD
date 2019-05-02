
## Starting and Stopping with runit-docker

This image adds `runit-docker` so that services can be added
in `/etc/service/xxx/run` and `.../finish` scripts.  This is
a Docker-specific wrapper, renumbering some signals, around
[runsvdir](http://smarden.org/runit/runsvdir.8.html)
which triggers
[runsv](http://smarden.org/runit/runsv.8.html)
on each of these `/etc/service/xxx` directories.

Note that the `xxx/run` scripts must run in the foreground,
and so keep a process occupied.  The `xxx/finish` script is
optional.

So, how to use it?  Well, you would start a container with

```
docker run --name base0 arpa2:base &
```

and then enter it later with

```
docker exec -it base0 bash
```


## Debug settings

If you need to use `gdb` you may run into an error

```
Error disabling address space randomization: Operation not permitted
```

All you need to do is add a flag when you run the image:

```
docker run --security-opt seccomp=unconfined -ti <image> bash
```


