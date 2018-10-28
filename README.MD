# Docker Demos for ARPA2

These are demonstration images based on Docker.
They are a basis for our own development, and for
sharing the results.

Dependencies are shown in the graphic below.

![Build dependencies](deps.png)

## Quick start

To see a usage video in your console, run

```shell
script -p demo-identityhub.script
```

To build the images, run

```shell
build.sh
```

The directories contain individual README files.
Generally, the `demo-*` directories are targets
and `wip-*` is work in progress.  The `build-*`
are helpful temporary images during building;
we build stuff in them and then `COPY --from`
to a target image.

## Adding images

Please commit a new demo as `wip-*` until it can
actually be tried, at which time it moves to
`demo-*` -- this may save some disappointments.

A good approach is probably to start off with
`FROM arpa2:build-xxx` and when it all works, strip away
the unnecessary build environment by switching to
`FROM arpa2:base` or another runtime base, and
using `COPY --from=xxx` to copy necessary elements
from the original `arpa2:build-xxx` that is now
a preliminary phase `FROM arpa2:build-xxx AS xxx`

You can create a typescript console "video" with

```shell
script -r <imagename>.script
```

To recreate the dependency graphs, there is a simple
scripted procedure,

```shell
./deps.sh > deps.dot
dot -T png deps.dot > deps.png
```

Perhaps it is helpful to do even this work in
CMake.

## Using on Windows

Docker images can be built on Windows without
problem. You can use Docker for Windows, as described below, but it is easier to create a Linux VM (eg. using VirtualBox) and use Docker for Linux instead, because then Hyper-V is not required and one can use IPv6.

### Using Docker for Windows
When using Docker for Windows remember that Hyper-V should be enabled and running, and that
files are being copied into containers, and those
files should not get LF translated to CR-LF, as is
the default when cloning a Git repository.

The solution is to setup your repository with

```
git clone https://github.com/arpa2/docker-demo --config core.autocrlf=input
```

[[credit]](https://willi.am/blog/2016/08/11/docker-for-windows-dealing-with-windows-line-endings/)
