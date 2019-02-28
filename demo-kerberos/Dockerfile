# build-kerberos constructs a KDC for the realm ARPA2.LAB
#
# From: Rick van Rein <rick@openfortress.nl>


FROM arpa2:build-bin

RUN apt-get install -y --no-install-recommends --assume-yes \
	krb5-kdc krb5-admin-server #OUT_OF_FASHION# krb5-rsh-server krb5-ftpd krb5-telnetd

ENV REALM=ARPA2.LAB

COPY krb5.conf /etc/krb5.conf
COPY kdc.conf /etc/krb5kdc/kdc.conf
COPY kadm5.acl /etc/krb5kdc/kadm5.acl

RUN ( echo 'BringYourOwnIdentity' ; echo 'BringYourOwnIdentity' ) | krb5_newrealm

# RUN /etc/init.d/krb5-kdc start
# RUN /etc/init.d/krb5-admin-server start

RUN ( echo addprinc demo  ; echo sekreet  ; echo sekreet  ; echo quit ) | kadmin.local
RUN ( echo addprinc demo1 ; echo sekreet1 ; echo sekreet1 ; echo quit ) | kadmin.local
RUN ( echo addprinc demo2 ; echo sekreet2 ; echo sekreet2 ; echo quit ) | kadmin.local
RUN ( echo addprinc demo3 ; echo sekreet3 ; echo sekreet3 ; echo quit ) | kadmin.local

CMD ["bash"]
