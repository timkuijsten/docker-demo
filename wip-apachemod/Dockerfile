# Dockerfile for apache2 tlspool module
#
# This is arpa2/wip-apachemod_arpa2.
#
# From: Henri Manson <hfmanson@gmail.com>


# Incorporate files from build-apachemod and files-reservoir
FROM arpa2/build-apachemod AS build
FROM arpa2/files-reservoir AS reservoir

# ARPA2 base image is build-tlspool
FROM arpa2/base


# Install packages

RUN apt-get install -y apache2

# Install files from files-reservoir
COPY --from=reservoir /var/arpa2/reservoir /var/arpa2/reservoir

# Install binaries from the build environment

COPY --from=build /usr/lib/apache2/modules/mod_arpa2_*.so /usr/lib/apache2/modules/

#TODO# Setup script to run
#TODO# RUN sed -i 's/^#\(LoadModule tlspool_module\)/\1/' /etc/apache2/conf/httpd.conf

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
