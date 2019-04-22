# Docker Image arpa2:base

> *This is the basic image that we shall use for most
> or all of the ARPA2 distribution images.*

The following instructions have been tested on:

  * Debian Linux
  * Mac OS X

## Building the ARPA2 base image

Run the following commands in a directory where you would like the GitHUB repository to go (Docker has its own storage locations for images):

    git clone https://github.com/arpa2/docker-arpa2-base
    docker build docker-arpa2-base

The process of downloading the basic Debian Stable distribution and installing a few packages (with quite a few dependencies) should start automatically.

The process should end with something like

    Successfully built 2b9ff935b388

The hash code identifies what has just been built.  You can also find it with:

    shell$ docker images
    REPOSITORY TAG    IMAGE ID     CREATED  SIZE
    <none>     <none> 2b9ff935b388 35s ago  197 MB
    debian     stable 954a3003e6c1 35h ago  124 MB

Oops &mdash; forgot to give it a name.  Let's rebuild (and see how these hashes allow for reuse):

    docker build docker-arpa2-base -t arpa2:base

If you list the images once more, you will find that the `<none>` entries have been replaced with `arpa2` and `base`.

Docker keeps those hashed images by default, but you can use `docker image rm` or `docker image prune` to cleanup.

## Running Commands in arpa2:base

The `Dockerfile` in `arpa2:base` has a shell as its default command, so you can simply run it (with an interactive terminal) to enter commands:

    shell$ docker run -it arpa2:base
    root@02fc270bb848:/# hostname
    02fc270bb848
    root@02fc270bb848:/# exit
    exit

Note how the hex code (in fact the hostname) is not the same as the image hash.  This is because we are looking at an *instance* of that image, not the image itself.  We can run many instances.

Until you run `exit`, you can view the container in another shell on the host, using `docker container ls`.  Once you quit, the container will be removed.

You can make local changes, for instance to install extra software.  These changes will not end up in the original `arpa2:base` image, but instead in a temporary image layered on top of it.  Try it!

## Minimal Core OS

The `Dockerfile` defines an `ENTRYPOINT` through `dumb-init`.  This means that PID 1 starts that program, with the main task of
[forwarding signals more intuitively](https://engineeringblog.yelp.com/2016/01/dumb-init-an-init-for-docker.html)
than a `bash` or `python` executable in PID 1.  If you don't override it, your inherited containers will use the same setup.

The second level is filled with any `CMD` that you specify in your specialising containers.  This basically runs under PID 1 if you don't change the `ENTRYPOINT`.

A default `CMD` is provided for this image too, but it does little more than starting `bash` as a fallback.  However, it leaves you with a number of very nice extra hooks:

  * `/usr/arpa2/bin/docker.go` replaces the usual `bash`.  It produces `PS1` prompts like `kip1.3#` for the first boot, third shell.
  * `/var/arpa2/docker.boots` lists the booting times.
  * `/tmp/docker.runs` lists the shell starts in the current boot.  Used for `PS1`.
  * `/var/arpa2/docker.name` can be used to set a small string with a name.  Used for `PS1`.
  * `/var/arpa2/docker.hostname` can be used to set additional, fixed hostnames.
  * `/usr/arpa2/bin/docker.install` is run as a one-time setup program during the first starting time.
  * `/usr/arpa2/bin/docker.daemon` is run when `tty` yields `not a tty` and daemons have not started.
  * `/usr/arpa2/bin/docker.interact` is run when `tty` yields a valid device name.

Note that it does not make complete sense, but fix it if you can!

  * The container will loop forever if it is run without foreground and no `docker.daemon` script.  You can use `^C` but this is locked out once you send `^Z`.
  * The `exec` command does not default to `CMD` so you would have to manually run `docker.go` -- or should we make this a startup script for `bash` instead?


## Support for ARPA2 Shells

There is a generic `arpa2shell` command, which looks through the Python system for modules named `arpa2xxx` that inherit `arpa2cmd.Cmd` into a module-specific `Cmd` class.  These classes are instantiated as sub-shells, into which `arpa2shell` can drop in.  No sub-shells have been included in this release, but images built on top of `arpa2:base` can.  In that case, they can all be addressed from `arpa2shell`.

## Connecting to a Containers

Not all containers are interactive on a terminal.  They may as well run a command like `nginx` with a port exported to the Docker operator, who may connect it as s/he pleases to the host infrastructure, including Docker's configurations of bridges and network interfaces.

Another facility that is useful during deployment, is the ability to mount a file system layer with data into the container's filesystem.  For applications such as a web server, this facilitates dynamic data and limits rebuilds of images to code changes.

## Adding layers atop arpa2:base

To develop additional layers on top of `arpa2:base`, you can run a container for `arpa2:base` and enter any commands that you think are needed.  Keep note of them in a new
[Dockerfile](https://docs.docker.com/engine/userguide/eng-image/dockerfile_best-practices/),
that adheres to the following pattern:

    FROM arpa2:base
    
    RUN apt-get update && apt-get -y upgrade
    
    # ENV DEBIAN_FRONTEND=noninteractive
    # RUN apt-get install X Y Z
    
    ADD file1 file2
    
    # ENV HOME /var/www
    # WORK /var/www
    
    CMD ["bash"]

The last line uses `bash` as the default command.  At some point you may prefer running another command, such as `nginx`.

It is probably best to isolate layers in their own GitHUB repository, named `arpa2/docker-arpa2-xxx` where the `docker-` prefix is widely adopted.
