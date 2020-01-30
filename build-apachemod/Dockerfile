# Dockerfile for apache2 tlspool module
#
# This is arpa2/wip-apachemod_arpa2.
#
# From: Henri Manson <hfmanson@gmail.com>


# ARPA2 base image is build-tlspool
FROM arpa2/build-tlspool AS apache


# Install the Apache tool chain and compile

RUN apt-get install -y automake libtool-bin libpcre3-dev liblmdb-dev lmdb-utils apache2-dev apache2-utils apache2

# Install libarpa2common

RUN cd /usr/local/src ; git clone https://gitlab.com/arpa2/libarpa2common.git && \
    cd libarpa2common && git reset v0.7.1 && \
    mkdir build && cd build && \
    cmake .. && \
    make && \
    make install && \
    ldconfig

# Build Apache modules
COPY mod_arpa2_tlspool	/usr/local/src/mod_arpa2_tlspool
COPY mod_arpa2_aclr 	/usr/local/src/mod_arpa2_aclr
COPY mod_arpa2_userdir	/usr/local/src/mod_arpa2_userdir

RUN cd /usr/local/src/mod_arpa2_userdir	&& apxs -i -a -c mod_arpa2_userdir.c
RUN cd /usr/local/src/mod_arpa2_aclr	&& apxs -i -a -c mod_arpa2_aclr.c \
                                    	        -larpa2aclr
RUN cd /usr/local/src/mod_arpa2_tlspool	&& apxs -i -a -c mod_arpa2_tlspool.c \
                                    	        -ltlspool

# Define default command
CMD ["/bin/bash"]
