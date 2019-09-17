# Dockerfile for the TLS Pool in the ARPA2 project
#
# This is a build environment that sets up the
# TLS Pool, a daemon that serves as an intermediate
# for (un)wrapping TLS traffic.  The idea is to
# isolate long-lasting credentials from application
# processes, and thereby avoid attacks that would
# go for private keys.
#
# From: Rick van Rein <rick@openfortress.nl>


# Include the Quick DER source for later import
#

# ARPA2 base image is the moving target Debian Stable
#
FROM arpa2/build-tlspool


# Install required packages (already done in arpa2:base)
# RUN \
#     apt-get update && \
#     apt-get -y upgrade

# Install dependencies
RUN apt-get install -y qtbase5-dev libqt5svg5-dev
RUN cd /usr/local/src ; git clone https://github.com/arpa2/tlspool-gui
RUN cd /usr/local/src/tlspool-gui && mkdir build && cd build && cmake .. && make install
# RUN  cd /usr/local/src/tlspool-0.9.2/testdata \
# 	&& sed -i -e 's+^pkcs11_path .*$+pkcs11_path /usr/lib/softhsm/libsofthsm2.so+' ../etc/tlspool.conf \
# 	&& make install
#RUN sed < /usr/local/src/tlspool-0.9.2/etc/tlspool.conf > /etc/tlspool.conf -E -e '/^db_/p' -e '/^db_/d' -e 's+ (file:)?\.\./+ \1/usr/local/src/tlspool-0.9.2/+' -e 's+^dnssec_rootkey .*+dnssec_rootkey /etc/dnsroot-tlspool+'

CMD ["bash"]
