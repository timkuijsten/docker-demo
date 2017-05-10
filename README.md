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
