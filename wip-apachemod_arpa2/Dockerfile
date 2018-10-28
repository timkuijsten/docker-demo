# Dockerfile for apache2 tlspool module
#
# This is arpa2:wip-apachemod_arpa2.
#
# From: Henri Manson <hfmanson@gmail.com>


# ARPA2 base image is build-tlspool
FROM arpa2:build-tlspool


# Install the source code, patch and compile.

RUN apt install -y automake libtool-bin libpcre3-dev
RUN cd /usr/local/src ; git clone https://github.com/hfmanson/httpd.git ; cd httpd/srclib ; git clone https://github.com/hfmanson/apr ; cd .. ; ./buildconf ; ./configure --prefix=/usr/local/apache2 --enable-tlspool ; make install

#TODO# Setup script to run

# Define default command
CMD ["/bin/bash"]
