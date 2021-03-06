# * build-tls-kdh *
#
# Builds an image with GnuTLS with TLS-KDH support enabled.
#
#
# From: Tom Vrancken <dev@tomvrancken.nl>

FROM arpa2/build-bin

LABEL net.arpa2.project="tls-kdh"

## Install the required packages from the official repos
RUN apt update && apt install -y \
    autoconf \
    autogen \
    automake \
    autopoint \
    bison \
    curl \
    dash \
    dns-root-data \
    gawk \
    gettext \
    git-core \
    gperf \
    guile-2.0-dev \
    libidn2-0-dev \
    libp11-kit-dev \
    libtasn1-6-dev \
    libtool \
    libtspi-dev \
    libunbound-dev \
    libunistring-dev \
    lzip \
    nettle-dev \
    wget

## Install packages that we have to build ourselves 
# libnettle
RUN cd /usr/local/src \
 && curl https://ftp.gnu.org/gnu/nettle/nettle-3.4.1.tar.gz | tar -zx \
 && cd nettle-3.4.1/ \
 && ./configure --disable-documentation \
 && make \
 && make check \
 && make install

# libidn2
RUN cd /usr/local/src \
 && curl https://ftp.gnu.org/gnu/libidn/libidn2-2.2.0.tar.lz | tar --lzip -xv \
 && cd libidn2-2.2.0/ \
 && ./configure \
 && make \
 && make check \
 && make install

## Retrieve, build and install the latest stable tls-kdh source
RUN cd /usr/local/src \
 && git clone --single-branch --branch tls-kdh https://gitlab.com/Vrancken/gnutls.git \
 && cd gnutls \
 && ./bootstrap \
 && ./configure --disable-doc --disable-guile \
 && make \
 && make install \
 && ldconfig


CMD ["bash"]
