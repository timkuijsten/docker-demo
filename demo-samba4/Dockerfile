# build-kerberos constructs a KDC for the realm ARPA2.LAB
#
# From: Rick van Rein <rick@openfortress.nl>


FROM arpa2/build-bin

RUN apt-get install -y --no-install-recommends --assume-yes \
	samba samba-dsdb-modules winbind smbclient smbldap-tools ldb-tools bind9 bind9utils samba-vfs-modules

ENV REALM=SAMBA.ARPA2.LAB

RUN mv /etc/samba/smb.conf /root
RUN mv /etc/krb5.conf /root
COPY krb5.conf /etc/krb5.conf

RUN mkdir -p /usr/lib/x86_64-linux-gnu/samba/ldb
#WONTRUN-SO-ADDED-101# RUN samba-tool domain passwordsettings set --complexity=off 
RUN samba-tool domain provision --realm $REALM --domain SAMBA --server-role dc --dns-backend SAMBA_INTERNAL --adminpass BringYourOwnIdentity101 --use-xattrs=no --use-rfc2307 -d 5
RUN samba-tool domain passwordsettings set --complexity=off 
#CNXERR# RUN samba-tool user password --newpassword=BringYourOwnIdentity --username=Administrator --password=BringYourOwnIdentity101
RUN samba-tool user create demo  sekreet
RUN samba-tool user create demo1 sekreet1
RUN samba-tool user create demo2 sekreet2
RUN samba-tool user create demo3 sekreet3

#NOTHERE# RUN /etc/init.d/samba-ad-dc start

CMD ["bash"]
