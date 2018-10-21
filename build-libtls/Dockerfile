# Dockerfile for ARPA2 TLS-related builds
#
# This is arpa2:build-tls -- an image based on
# arpa2:build-bin and enhanced with software that
# we use to employ TLS in a number of variaties.
#
# Other layers can include this one as their basis,
# do some building and restart with another FROM that
# then copies from the build phase.  Tag this image
# arpa2:buildphase-binutils to match with the FROM
# of such preparing build phases.
#
# See:
# https://docs.docker.com/develop/develop-images/multistage-build/
#
# From: Rick van Rein <rick@openfortress.nl>


# Prepare installing Python tools with pip
#
FROM arpa2:build-pip AS pip

#NO-TLS-SUPPORT-YET# RUN pip install python-mbedtls
RUN pip install python-gnutls


# ARPA2 build-bin image on top of the moving target Debian Stable
#
FROM arpa2:build-bin

# Install required packages
RUN \
    apt-get update && \
    apt-get -y upgrade

ENV DEBIAN_FRONTEND=noninteractive

# Install mbedTLS
ADD https://github.com/ARMmbed/mbedtls/archive/mbedtls-2.13.1.tar.gz /root/mbedtls-2.13.1.tar.gz
RUN cd /usr/local/src && tar -xzvf /root/mbedtls-2.13.1.tar.gz
RUN cd /usr/local/src/mbedtls-mbedtls-2.13.1 && mkdir build && cd build && cmake .. && make all test install

# TODO: Install GnuTLS (with TLS-KDH)

# TODO: Install TLS Pool

# Copy Python tooling
COPY --from=pip /usr/local/lib/python2.7/dist-packages/ /usr/local/lib/python2.7/dist-packages

# Define default command
CMD ["/bin/bash"]

