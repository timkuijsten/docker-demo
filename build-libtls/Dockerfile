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
#RUN apt-get install -y dash git-core autoconf libtool gettext autopoint \
#automake autogen nettle-dev libp11-kit-dev libtspi-dev libunistring-dev \
#guile-2.0-dev libtasn1-6-dev libidn2-0-dev gawk gperf \
#libunbound-dev dns-root-data bison help2man gtk-doc-tools \
#texinfo texlive texlive-generic-recommended texlive-extra-utils
#ADD https://ftp.gnu.org/gnu/nettle/nettle-3.4.tar.gz /usr/local/src
#RUN cd /usr/local/src && tar -xzvf nettle-3.4.tar.gz && cd nettle-3.4 && ./configure && make && make install
#ADD https://www.gnupg.org/ftp/gcrypt/gnutls/v3.6/gnutls-3.6.4.tar.xz /usr/local/src
#RUN cd /usr/local/src && tar -xJvf gnutls-3.6.4.tar.xz && cd gnutls-3.6.4 && ./configure && make && make install
# README: the above code work until the make command. Make fails on some guile modules. No clue why.

# TODO: Install TLS Pool

# Copy Python tooling
COPY --from=pip /usr/local/lib/python2.7/dist-packages/ /usr/local/lib/python2.7/dist-packages

# Define default command
CMD ["/bin/bash"]

