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
FROM arpa2:build-quickder-lillydap AS quickder

# ARPA2 base image is the moving target Debian Stable
#
FROM arpa2:build-bin

# Install required packages (already done in arpa2:base)
# RUN \
#     apt-get update && \
#     apt-get -y upgrade

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
# libgnutls30 is present without a -dev for it?
RUN apt-get install -y libdb5.3-dev libgnutls28-dev gnutls-bin libldns-dev libunbound-dev

# Install Quick DER
COPY --from=quickder /usr/local/include/quick-der /usr/local/include/quick-der
COPY --from=quickder /usr/local/lib/pkgconfig/Quick-DER.pc /usr/local/lib/pkgconfig/Quick-DER.pc
COPY --from=quickder /usr/local/lib/libquickder.so /usr/local/lib/libquickder.so
COPY --from=quickder /usr/local/lib/libquickder.a /usr/local/lib/libquickder.a
COPY --from=quickder /usr/local/share/Quick-DER/ /usr/local/share/Quick-DER
COPY --from=quickder /usr/local/share/doc/quick-der/ /usr/local/share/doc/quick-der

# Install TLS Pool
ADD https://github.com/arpa2/tlspool/archive/0.20-beta1.tar.gz /root/tlspool-0.20-beta1.tar.gz
RUN cd /usr/local/src && tar -xzvf /root/tlspool-0.20-beta1.tar.gz
RUN cd /usr/local/src/tlspool-0.20-beta1 && mkdir build && cd build && cmake .. && make all test install

# Define default command
CMD ["/bin/bash"]

