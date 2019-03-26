# Docker file for Kerberos Realm Crossover demonstration
#
# This builds the necessary programs, and runs two KDCs
# on different ports, each with its own KXOVER wrapper,
# and can be used to build shared keys between realms
# on the two KDCs.
#
# From: Rick van Rein <rick@openfortress.nl>


# Include the Quick DER build for later import
#
FROM arpa2:build-quickder-lillydap AS quickder


# Include the TLS Pool build for later import
#
FROM arpa2:build-tlspool AS tlspool


# Include the KDC demonstration for later import
#
FROM arpa2:demo-kerberos AS kdc


# Start with the binary + python build environment
#
FROM arpa2:build-bin

# Install Debian software packages
#
RUN apt-get -y install libev-dev python-pyparsing python-six libunbound-dev krb5-kdc krb5-admin-server
RUN apt-get install -y --no-install-recommends --assume-yes \
	krb5-kdc krb5-admin-server #OUT_OF_FASHION# krb5-rsh-server krb5-ftpd krb5-telnetd

# Install Quick DER + asn2quickder + asn1literate
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
COPY --from=quickder /usr/local/bin/hexin /usr/local/bin/hexin
COPY --from=quickder /usr/local/bin/hexout /usr/local/bin/hexout
COPY --from=quickder /usr/local/bin/derdump /usr/local/bin/derdump
COPY --from=quickder /usr/local/bin/derdump.quickder /usr/local/bin/derdump.quickder
COPY --from=quickder /HEXIO.MD /HEXIO.MD

# Install TLS Pool build dependencies
#
COPY --from=tlspool /etc/tlspool.conf /etc/tlspool.conf
COPY --from=tlspool /usr/include/tlspool/ /usr/include/tlspool
COPY --from=tlspool /usr/local/lib/libtlspool.so /usr/local/lib/libtlspool.so
COPY --from=tlspool /usr/local/lib/libtlspool.a  /usr/local/lib/libtlspool.a

# Setup the client and service Kerberos realms

COPY krb5.conf /etc/krb5.conf
COPY kdc.conf /etc/krb5kdc/kdc.conf
COPY kadm5.acl.pixie   /etc/krb5kdc/kadm5.acl.pixie
COPY kadm5.acl.unicorn /etc/krb5kdc/kadm5.acl.unicorn
#
RUN ( echo 'BringYourOwnIdentity' ; echo 'BringYourOwnIdentity' ) | kdb5_util -r UNICORN.DEMO.ARPA2.ORG create -s
RUN ( echo 'BringYourOwnIdentity' ; echo 'BringYourOwnIdentity' ) | kdb5_util -r   PIXIE.DEMO.ARPA2.ORG create -s
#
# RUN /etc/init.d/krb5-kdc start
# RUN /etc/init.d/krb5-admin-server start

# Create client identities
RUN ( echo addprinc demo  ; echo sekreet  ; echo sekreet  ; echo quit ) | kadmin.local -r PIXIE.DEMO.ARPA2.ORG
RUN ( echo addprinc demo1 ; echo sekreet1 ; echo sekreet1 ; echo quit ) | kadmin.local -r PIXIE.DEMO.ARPA2.ORG
RUN ( echo addprinc demo2 ; echo sekreet2 ; echo sekreet2 ; echo quit ) | kadmin.local -r PIXIE.DEMO.ARPA2.ORG
RUN ( echo addprinc demo3 ; echo sekreet3 ; echo sekreet3 ; echo quit ) | kadmin.local -r PIXIE.DEMO.ARPA2.ORG

# Create service identities
RUN ( echo addprinc  www.unicorn.demo.arpa2.lab ; echo sekreet  ; echo sekreet  ; echo quit ) | kadmin.local -r UNICORN.DEMO.ARPA2.ORG
RUN ( echo addprinc imap.unicorn.demo.arpa2.lab ; echo sekreet1 ; echo sekreet1 ; echo quit ) | kadmin.local -r UNICORN.DEMO.ARPA2.ORG
RUN ( echo addprinc smtp.unicorn.demo.arpa2.lab ; echo sekreet2 ; echo sekreet2 ; echo quit ) | kadmin.local -r UNICORN.DEMO.ARPA2.ORG
RUN ( echo addprinc ldap.unicorn.demo.arpa2.lab ; echo sekreet3 ; echo sekreet3 ; echo quit ) | kadmin.local -r UNICORN.DEMO.ARPA2.ORG

# Create kadmin identities
RUN ( echo addprinc kadmin       ; echo sekreet3 ; echo sekreet3 ; echo quit ) | kadmin.local -r UNICORN.DEMO.ARPA2.ORG
RUN ( echo addprinc kadmin/admin ; echo sekreet3 ; echo sekreet3 ; echo quit ) | kadmin.local -r UNICORN.DEMO.ARPA2.ORG
RUN ( echo addprinc kadmin       ; echo sekreet3 ; echo sekreet3 ; echo quit ) | kadmin.local -r PIXIE.DEMO.ARPA2.ORG
RUN ( echo addprinc kadmin/admin ; echo sekreet3 ; echo sekreet3 ; echo quit ) | kadmin.local -r PIXIE.DEMO.ARPA2.ORG
RUN ( echo addprinc demo/admin   ; echo sekreet3 ; echo sekreet3 ; echo quit ) | kadmin.local -r PIXIE.DEMO.ARPA2.ORG

# Create the KXOVER service (and its ACL privileges)
RUN mkdir /etc/kxover
RUN ( echo addprinc -randkey kxover/public ; echo ktadd -k /etc/kxover/public.keytab kxover/public ; echo quit ) | kadmin.local -r UNICORN.DEMO.ARPA2.ORG


# Install KXOVER from source
#
#TODO# will later be "-b master"
#
RUN cd /usr/local/src ; git clone -b tls-based-attempt --single-branch \
		https://github.com/arpa2/kxover kxover.git
RUN cd /usr/local/src/kxover.git ; mkdir build ; cd build ; cmake -DDEBUG:BOOL=ON -DFAKE_TLS:BOOL=ON ..
RUN make -C /usr/local/src/kxover.git/build all
RUN make -C /usr/local/src/kxover.git/build test
RUN make -C /usr/local/src/kxover.git/build install


# Include the README.MD in the root
#
COPY README.MD /KXOVER.md

# Baseline command is bash
#
CMD ["bash"]
