# Dockerfile for apache2 tlspool module
#
# This is arpa2/wip-apachemod_arpa2.
#
# From: Henri Manson <hfmanson@gmail.com>


# Incorporate files from build-apachemod and files-reservoir
FROM arpa2/build-apachemod AS build
FROM arpa2/files-reservoir AS resv

# ARPA2 base image is build-tlspool
FROM arpa2/base


# Install packages

RUN apt-get install -y apache2

# Install files from files-reservoir
COPY --from=resv /var/arpa2/reservoir /var/arpa2/reservoir

# Install binaries from the build environment

COPY --from=build /usr/lib/apache2/modules/mod_arpa2_*.so /usr/lib/apache2/modules/

#TODO# Setup script to run
#TODO# RUN sed -i 's/^#\(LoadModule tlspool_module\)/\1/' /etc/apache2/conf/httpd.conf

# Provide for something to read
COPY README.MD /ApacheHTTPD.MD

# Install configuration
COPY apache2/* /etc/apache2/

# Define default command
CMD ["/bin/bash"]


### FUTURE: EXTRACT FROM THIS IMAGE
#
# FROM arpa2/base
# COPY --from=apache /usr/local/apache2 /usr/local/apache2
# COPY --from=apache /usr/local/lib/libtlspool.so /usr/local/lib/
# COPY README.MD /ApacheHTTPD.MD
# CMD ["/bin/bash"]
