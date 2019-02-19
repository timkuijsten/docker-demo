# Docker file for Kerberos Realm Crossover demonstration
#
# This builds the necessary programs, and runs two KDCs
# on different ports, each with its own KXOVER wrapper,
# and can be used to build shared keys between realms
# on the two KDCs.
#
# From: Rick van Rein <rick@openfortress.nl>


# Include the Quick DER source for later import
#
FROM arpa2:build-quickder-lillydap AS quickder

# Start with the binary + python build environment
#
FROM arpa2:build-bin

# Install Debian software packages
#
RUN apt-get -y install libev-dev python-pyparsing python-six libunbound-dev

# Install Quick DER + asn2quickder
#
#TODO# asn2quickder does not have a working installer
#
COPY --from=quickder /usr/local/include/quick-der /usr/local/include/quick-der
COPY --from=quickder /usr/local/lib/pkgconfig/Quick-DER.pc /usr/local/lib/pkgconfig/Quick-DER.pc
COPY --from=quickder /usr/local/lib/libquickder.so /usr/local/lib/libquickder.so
COPY --from=quickder /usr/local/lib/libquickder.a /usr/local/lib/libquickder.a
COPY --from=quickder /usr/local/share/Quick-DER/ /usr/local/share/Quick-DER
COPY --from=quickder /usr/local/share/doc/quick-der/ /usr/local/share/doc/quick-der
#
COPY --from=quickder /usr/local/lib/python2.7/dist-packages/ /usr/local/lib/python2.7/dist-packages
COPY --from=quickder /usr/local/bin/asn1literate /usr/local/bin/asn1literate
COPY --from=quickder /usr/local/bin/asn2quickder /usr/local/bin/asn2quickder
COPY --from=quickder /usr/local/src/quick-der-version-1.2-6/python/quick_der/ /usr/local/lib/python2.7/dist-packages/quick_der
#
#TODO# Manual extensions not yet part of Quick DER 1.2-6
COPY rfc4556.h /usr/local/include/quick-der/rfc4556.h
COPY KXOVER.h /usr/local/include/quick-der/KXOVER.h


# Prepare by adding Ruby (for C++ Broker)
#
#TODO# RUN apt-get install -y doxygen


# Install KXOVER from source
#
#TODO# will later be "-b master"
#
RUN cd /usr/local/src ; git clone -b tls-based-attempt --single-branch \
		https://github.com/arpa2/kxover kxover.git
#TODO# RUN cd /usr/local/src/kxover.git ; mkdir build ; cd build ; cmake .. ; make all test install


# Include the README.MD in the root
#
COPY README.MD /KXOVER.md
COPY KXOVER.asn1 /root/KXOVER.asn1

# Baseline command is bash
#
CMD ["bash"]
