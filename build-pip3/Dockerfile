# Dockerfile for ARPA2 build preparation (binutils)
#
# This is arpa2/buildphase-binutils -- an image based
# on debian:stable via arpa2/base and enhanced with
# software that we use to build ARPA2 binaries.
#
# Other layers can include this one as their basis,
# do some building and restart with another FROM that
# then copies from the build phase.  Tag this image
# arpa2/buildphase-pip to match with the FROM of such
# preparing build phases.
#
# See:
# https://docs.docker.com/develop/develop-images/multistage-build/
#
# From: Rick van Rein <rick@openfortress.nl>


# ARPA2 base image is the moving target Debian Stable
FROM arpa2/build-bin

# Install required packages
RUN \
    apt-get update && \
    apt-get -y upgrade

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get install -y python3 python3-pip

RUN pip3 install gnureadline colored

# Sigh.  Work around zeal of GNU licensing.  Not perfect.
COPY arpa2gnu /usr/bin/arpa2gnu
RUN base64 /usr/local/lib/python3*/dist-packages/gnureadline*.so >> /usr/bin/arpa2gnu
RUN echo EOF >> /usr/bin/arpa2gnu
RUN rm /usr/local/lib/python3*/dist-packages/gnureadline*.so

COPY arpa2kinit /usr/bin/arpa2kinit


# Define default command
CMD ["/bin/bash"]

