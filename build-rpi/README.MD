# Raspberry Pi build environment for ARPA2

> *This is a base image for building on Raspberry Pi, through a
> QEMU layer.  We use Raspbian as the underlying OS.*

The instructions below are for the Lite version of Raspbian.  In most
places you can substitute `full` for `lite` to have an X server running
in the background of your compiler.  Nice if it makes you feel better :)


## Prepare a Docker Volume

You need to download an image for Raspbian.  Perhaps we will change that
one day, if we switch to `debootstrap`.  For now, we simply use an image
from the Raspberry Pi software distribution -- because that platform is
also our target.

Note that the following instructions do not bring this image up to the
same level as `arpa2/build-bin` yet.  That would be better, of course,
and is a necessity before we can start integrating Raspberry Pi builds
smoothly into the other flows that we support.

The setup instructions are:

 1. Tag your build as `build-rpi` -- **and remember to change this after
    any change** so we may need to do better than this in the future, as
    it is a breaking point; handy now, troubling later
 2. Run `fill-docker-volume` to setup the image in the Docker.


## Start the Docker environment

Run

```
docker run --name rpi0 --mount src=raspbian-image,dst=/root/raspbian-image -t -d build-rpi
```

Then

```
docker exec -it rpi0 bash
```

Finally

```
docker rm rpi0
```


## Start Raspbian under QEMU

In the Docker, run:
 
```
qemu-system-arm \
	-M versatilepb \
	-cpu arm1176 \
	-m 256 \
	-drive format=raw,file=/root/raspbian-image/2019-04-08-raspbian-stretch-lite.img \
	-net nic \
	-net user,hostfwd=tcp::5022-:22 \
	-dtb /root/versatile-pb.dtb \
	-kernel /root/kernel-qemu-4.14.79-stretch \
	-append "root=/dev/sda2 panic=1 rootfstype=ext4 rw" \
	-serial stdio \
	-display none \
	-no-reboot
```

This should land you with a prompt on which to continue.


## All in one call

To avoid managing Docker containers, you might also make one call.
State will be kept in the volume, which is lucky because you might
not like that a simple press of Ctrl-C already brings down the
entire environment!

```
docker run -it \
    --mount src=raspbian-image,dst=/root/raspbian-image \
    build-rpi \
    qemu-system-arm \
        -M versatilepb \
        -cpu arm1176 \
        -m 256 \
        -drive format=raw,file=/root/raspbian-image/2019-04-08-raspbian-stretch-lite.img \
        -net nic \
        -net user,hostfwd=tcp::5022-:22 \
        -dtb /root/versatile-pb.dtb \
        -kernel /root/kernel-qemu-4.14.79-stretch \
        -append "root=/dev/sda2 panic=1 rootfstype=ext4 rw" \
        -serial stdio \
        -display none \
        -no-reboot
```

Now all we need to do is stop the bootstrapping nonsense and deliver you
a prompt where you can play, and finally to make it all into a Docker
`ENTRYPOINT` and you will be able to use it as you do with all other
Docker images.  Yeah, dreams are good to have.  (But time would be good too!)
