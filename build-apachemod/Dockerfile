# Dockerfile for apache2 tlspool module
#
# This is arpa2:wip-apachemod_arpa2.
#
# From: Henri Manson <hfmanson@gmail.com>


# ARPA2 base image is build-tlspool
FROM arpa2:build-tlspool AS apache


# Install the source code, patch and compile.

RUN apt-get install -y automake libtool-bin libpcre3-dev
# RUN cd /usr/local/src ; git clone -b trunk --single-branch https://github.com/hfmanson/httpd.git apache-httpd.git
# RUN cd /usr/local/src/apache-httpd.git/srclib ; git clone -b trunk --single-branch https://github.com/hfmanson/apr apr
RUN cd /usr/local/src ; git clone -b mod_arpa2_tlspool --single-branch https://github.com/arpa2/apache-httpd.git apache-httpd.git
RUN cd /usr/local/src/apache-httpd.git/srclib ; git clone -b mod_arpa2_tlspool --single-branch https://github.com/arpa2/apache-apr apr
RUN cd /usr/local/src/apache-httpd.git ; ./buildconf ; ./configure --prefix=/usr/local/apache2 --enable-tlspool
RUN cd /usr/local/src/apache-httpd.git ; make all test install

#TODO# Setup script to run
RUN sed -i 's/^#\(LoadModule tlspool_module\)/\1/' /usr/local/apache2/conf/httpd.conf

# Provide for something to read
COPY README.MD /ApacheHTTPD.MD

# Define default command
CMD ["/bin/bash"]


### FUTURE: EXTRACT FROM THIS IMAGE
#
# FROM arpa2:base
# COPY --from=apache /usr/local/apache2 /usr/local/apache2
# COPY --from=apache /usr/local/lib/libtlspool.so /usr/local/lib/
# COPY README.MD /ApacheHTTPD.MD
# CMD ["/bin/bash"]
