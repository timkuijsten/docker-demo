# Build recipe for a Docker image with the IdentityHub demo.
#
# This image is meant to add and remove domains, users, aliase,
# groups and so on.  It is where to setup communication ACL
# entries between any of these user@domain.name local identities
# and arbitrary remote selectors, and assign access rights to
# them.
#
# The software in this image is mostly OpenLDAP, hosting a tree
# under ou=IdentityHub,o=arpa2.net,ou=InternetWide.  It also
# reveals this host through web2ldap.  TODO: web2ldap is not yet Py3
#
# This image should grow to extract ACL data, and add them to
# and remove them from key->value database entries.  And to
# allow not just operator shell interactions, but also user
# interactions through a website, and possibly other means.
#
# This is meant to grow into a live IdentityHub, just run on
# Docker for developer convenience.
#
# From: Rick van Rein <rick@openfortress.nl>



### First stage: Setup Python support
#
#PIP2EASYINSTALL# FROM arpa2/build-pip AS pip



### Second stage: Actually construct the image
#
FROM arpa2/base

COPY README.MD		/IdentityHub.md

#PIP2EASYINSTALL# COPY --from=pip /usr/local/lib/python3.7/dist-packages /usr/local/lib/python3.7/dist-packages

RUN DEBIAN_FRONTEND=noninteractive SLAPD_PASSWORD=sekreet \
	apt-get install --no-install-recommends -y \
	slapd libsasl2-modules-gssapi-mit python3-setuptools


COPY identityhub.sh /usr/bin/identityhub.sh
COPY initial.ldif /root/initial.ldif

RUN python3 /usr/lib/python3/dist-packages/easy_install.py python3-ldap #TODO:AWAIT:PY3#web2ldap

#TODO:BUILD# RUN cd /usr/local/src ; git clone https://github.com/arpa2/arpa2shell-cmdparser cmdparser.git
#TODO:BUILD# RUN cd /usr/local/src/cmdparser.git/cmdparser ; python setup.py install

COPY arpa2id.py /usr/local/lib/python3.7/dist-packages/arpa2identityhub.py
COPY arpa2acl.py /usr/local/lib/python3.7/dist-packages/arpa2acl.py
RUN ln -s /usr/local/lib/python3.7/dist-packages/arpa2identityhub.py /usr/bin/arpa2identityhub
RUN ln -s /usr/local/lib/python3.7/dist-packages/arpa2acl.py /usr/bin/arpa2acl
COPY arpa2kinit /usr/bin/arpa2kinit
COPY arpa2gnu   /usr/bin/arpa2gnu

COPY schema/*		/etc/ldap/schema/
RUN rm -rf /etc/ldap/slapd.d
COPY slapd.conf		/etc/ldap/slapd.conf
COPY ldap.conf		/etc/ldap/ldap.conf
#TODO:AWAIT:PY3# COPY etc/web2ldap	/etc/web2ldap/
COPY slapd.keytab	/etc/krb5.keytab

RUN chmod go+r          /etc/krb5.keytab
#RATHER #RUN chmod go-rwx        /etc/krb5.keytab
#RATHER #COPY slapd.keytab	/etc/ldap/slapd.keytab
#RATHER #RUN chown opldap:openldap /etc/ldap/slapd.keytab
#RATHER #RUN chmod go-rwx          /etc/ldap/slapd.keytab
#RATHER #ENV ... OpenLDAP only ...

RUN chmod ugo+x /usr/bin/arpa2identityhub

EXPOSE 1389:1389 1760:1760

CMD [ "/usr/bin/identityhub.sh" ]

