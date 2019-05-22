# Raspbian emulator running under Docker
#
# From: Rick van Rein <rick@openfortress.nl>


# ARPA2 base image is the moving target Debian Stable
FROM arpa2/base

# Install required packages
RUN \
    apt-get update && \
    apt-get -y upgrade

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get install -y qemu-system-arm

ADD https://github.com/dhruvvyas90/qemu-rpi-kernel/blob/master/kernel-qemu-4.4.34-jessie?raw=true /root/kernel-qemu-4.4.34-jessie.bzImage

ADD https://downloads.raspberrypi.org/raspbian_full_latest /root/raspbian.img

RUN fdisk -l /root/raspbian.img

ENV QEMU_AUDIO_DRV=none

ADD README.MD /QEMU-RASPBERRY-PI.MD



# Define default command
CMD ["/bin/bash"]

#RUNIT# Use runit-docker as the entrypoint
#RUNIT# ENTRYPOINT ["/sbin/runit-docker"]
