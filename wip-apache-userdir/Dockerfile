# Dockerfile for apache2 tlspool module
#
# This is arpa2/wip-apachemod_arpa2.
#
# From: Henri Manson <hfmanson@gmail.com>


# ARPA2 Reservoir demonstration files
FROM arpa2/files-reservoir AS reservoir

# ARPA2 base image is build-tlspool
FROM arpa2/build-tlspool AS apache


# Install the source code, patch and compile.

RUN apt-get install -y automake libtool-bin libpcre3-dev liblmdb-dev lmdb-utils
# RUN cd /usr/local/src ; git clone -b trunk --single-branch https://github.com/hfmanson/httpd.git apache-httpd.git
# RUN cd /usr/local/src/apache-httpd.git/srclib ; git clone -b trunk --single-branch https://github.com/hfmanson/apr apr
RUN cd /usr/local/src ; git clone -b draft-vanrein-http-unauth-user --single-branch https://github.com/arpa2/apache-httpd.git apache-httpd.git
RUN cd /usr/local/src/apache-httpd.git/srclib ; git clone -b mod_arpa2_tlspool --single-branch https://github.com/arpa2/apache-apr apr
# libarpa2common
RUN cd /usr/local/src ; git clone https://gitlab.com/arpa2/libarpa2common.git && \
    cd libarpa2common && git reset v0.7.1 && \
    mkdir build && cd build && \
    cmake .. && \
    make && \
    make install && \
    ldconfig

RUN cd /usr/local/src/apache-httpd.git ; ./buildconf ; ./configure --prefix=/usr/local/apache2 --enable-tlspool
RUN cd /usr/local/src/apache-httpd.git ; make all test install

COPY bashrc /root/.bashrc

#TODO# Setup script to run
RUN sed -i 's/^#\(LoadModule tlspool_module\)/\1/' /usr/local/apache2/conf/httpd.conf

# Create a user or two (we force them into ARPA2 Reservoir, evil but it works)
RUN useradd -m john && su -c 'mkdir ~/public_html' john
RUN useradd -m mary && su -c 'mkdir ~/public_html' mary
COPY john.index.html /home/john/public_html/index.html
COPY mary.index.html /home/mary/public_html/index.html
COPY --from=reservoir /var/arpa2/reservoir /var/arpa2/reservoir
#TOOCRUDE# COPY john.index.html /var/arpa2/reservoir/arpa2.org/uid=john/index.html
#TOOCRUDE# COPY mary.index.html /var/arpa2/reservoir/arpa2.org/uid=mary/index.html

# Provide for something to read
COPY README.MD /ApacheHTTPD.MD
COPY apache2 /usr/local/apache2

# Define default command
CMD ["/bin/bash"]


### FUTURE: EXTRACT FROM THIS IMAGE
#
# FROM arpa2/base
# COPY --from=apache /usr/local/apache2 /usr/local/apache2
# COPY --from=apache /usr/local/lib/libtlspool.so /usr/local/lib/
# COPY README.MD /ApacheHTTPD.MD
# CMD ["/bin/bash"]
