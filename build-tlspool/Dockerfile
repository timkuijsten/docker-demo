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
RUN apt-get install -y libdb5.3-dev softhsm2 libgnutls28-dev gnutls-bin libldns-dev libunbound-dev
#DROPHSM# RUN apt-get install -y libdb5.3-dev libgnutls28-dev gnutls-bin libldns-dev libunbound-dev

# Install Quick DER
COPY --from=quickder /usr/local/include/quick-der /usr/local/include/quick-der
COPY --from=quickder /usr/local/lib/pkgconfig/Quick-DER.pc /usr/local/lib/pkgconfig/Quick-DER.pc
COPY --from=quickder /usr/local/lib/libquickder.so /usr/local/lib/libquickder.so
COPY --from=quickder /usr/local/lib/libquickder.a /usr/local/lib/libquickder.a
COPY --from=quickder /usr/local/share/Quick-DER/ /usr/local/share/Quick-DER
COPY --from=quickder /usr/local/share/doc/quick-der/ /usr/local/share/doc/quick-der

# Install SoftHSM2
#FROMDEB# ADD https://github.com/opendnssec/SoftHSMv2/archive/2.5.0.tar.gz /root/softhsm2-2.5.0.tar.gz
#FROMDEB# RUN cd /usr/local/src ; tar -xzvf /root/softhsm2-2.5.0.tar.gz
#FROMDEB# RUN cd /usr/local/src/SoftHSMv2-2.5.0/ ; mkdir build ; cd build ; cmake -D ENABLE_GOST:BOOL=OFF .. ; make all test install

# Install TLS Pool
ADD https://github.com/arpa2/tlspool/archive/0.20-beta2.tar.gz /root/tlspool-0.20-beta2.tar.gz
RUN useradd tlspool
RUN cd /usr/local/src && tar -xzvf /root/tlspool-0.20-beta2.tar.gz
RUN cd /usr/local/src/tlspool-0.20-beta2 && mkdir build && cd build && cmake .. && make all test install
RUN cp /usr/local/src/tlspool-0.20-beta2/etc/tlspool.conf /etc/tlspool.conf

# Setup SoftHSM2 (includes creating missing directory)
RUN chmod ugo+rx /etc/softhsm && chmod ugo+r /etc/softhsm/softhsm2.conf
RUN chmod go+rx /var/lib/softhsm && mkdir -p /var/lib/softhsm/tokens && chmod go+rwx /var/lib/softhsm/tokens
RUN su -c 'softhsm2-util --init-token --free --label TLS_Pool_dev_data --so-pin=sekreet --pin=1234' tlspool
#FROMDEB# RUN ln -s /usr/local/lib/softhsm /usr/lib/softhsm
#FROMDEB# RUN softhsm2-util --init-token --free --label 'TLS_Pool_dev_data' --so-pin=sekreet --pin=1234

# Setup TLS Pool testdata
#GITDIDIT# RUN cd /usr/local/src/tlspool-0.20-beta2/testdata && sed -i -e 's+^pkcs11_path .*$+pkcs11_path /usr/lib/softhsm/libsofthsm2.so+' ../etc/tlspool.conf
#SU-NOW-IN-INIT# RUN cd /usr/local/src/tlspool-0.20-beta2/testdata && chown tlspool . && ( su -c 'TOOLDIR=../build/tool make all' tlspool || su -c 'TOOLDIR=../build/tool make all' tlspool )
RUN cd /usr/local/src/tlspool-0.20-beta2/testdata && ( TOOLDIR=../build/tool make all || TOOLDIR=../build/tool make all )
# RUN  cd /usr/local/src/tlspool-0.20-beta2/testdata \
# 	&& sed -i -e 's+^pkcs11_path .*$+pkcs11_path /usr/lib/softhsm/libsofthsm2.so+' ../etc/tlspool.conf \
# 	&& make install
RUN sed < /usr/local/src/tlspool-0.20-beta2/etc/tlspool.conf > /etc/tlspool.conf -e 's+ \.\./+ /usr/local/src/tlspool-0.20-beta2/+'
COPY tlspool.sh /tlspool.sh

# Make sure all libraries can be found (including libquickder which was copied)
RUN ldconfig

# Define default command
CMD ["/tlspool.sh"]

