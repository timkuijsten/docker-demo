# Dockerfile for ARPA2 build preparation (binutils)
#
# TARGET PLATFORM: armhf (used on Raspberry Pi)
# HOST   PLATFORM: amd64 or Docker's default
#
# This is arpa2/build-armhf -- an image based
# on debian:stable via arpa2/base and enhanced with
# software that we use to build ARPA2 binaries.
# It uses the multiarch code to install development
# programs with :armhf attached.  These target code
# for those platforms.  Python too?  Yes, for its
# production of library extensions we have to.
#
# Other layers can include this one as their basis,
# do some building and restart with another FROM that
# then copies from the build phase.  Tag this image
# arpa2/build-armhf to match with the FROM of such
# preparing build phases.
#
# See:
# https://docs.docker.com/develop/develop-images/multistage-build/
# https://wiki.debian.org/Multiarch
# https://wiki.debian.org/Multiarch/HOWTO
#
# From: Rick van Rein <rick@openfortress.nl>


# ARPA2 base image is the moving target Debian Stable
FROM arpa2/base

# Install required packages
RUN \
    dpkg --add-architecture armhf && \
    apt-get update && \
    apt-get -y upgrade

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get install -y python2.7-minimal:armhf python-setuptools:armhf python-pkg-resources:armhf \
	gcc:armhf g++:armhf binutils:armhf make:armhf \
	git vim nano man tmux \
	flex bison \
	rsyslog strace gdb:armhf \
	cmake cmake-curses-gui pkg-config \
	libldap-dev:armhf libsasl2-dev:armhf libkrb5-dev:armhf \
	runit
	#TODO#libgnutls-dev

# Install ARPA2CM
ADD https://github.com/arpa2/arpa2cm/archive/version-0.5.tar.gz /root/arpa2cm-version-0.5.tar.gz
RUN cd /usr/local/src && tar -xzvf /root/arpa2cm-version-0.5.tar.gz
RUN cd /usr/local/src/arpa2cm-version-0.5 && mkdir build && cd build && cmake .. && make all install

#RUNIT## Install runit-docker (base already holds runit but not activated)
#RUNIT# RUN git clone https://github.com/arpa2/runit-docker /usr/local/src/runit-docker.git
#RUNIT# RUN make -C /usr/local/src/runit-docker.git install
#RUNIT# COPY service/ /etc/service/

# Define default command
CMD ["/bin/bash"]

#RUNIT# Use runit-docker as the entrypoint
#RUNIT# ENTRYPOINT ["/sbin/runit-docker"]
