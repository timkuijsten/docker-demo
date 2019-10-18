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
FROM arpa2/build-bin

COPY README.MD		/IdentityHub.md

#PIP2EASYINSTALL# COPY --from=pip /usr/local/lib/python3.7/dist-packages /usr/local/lib/python3.7/dist-packages

RUN DEBIAN_FRONTEND=noninteractive SLAPD_PASSWORD=sekreet \
	apt-get install --no-install-recommends -y \
	slapd libsasl2-modules-gssapi-mit python3-setuptools \
	liblog4cpp5-dev libfcgi-dev libsqlite3-dev


#
# Build SteamWorks and Pulley Backend for Lifecycles
#

RUN cd /usr/local/src && git clone https://gitlab.com/arpa2/arpa2cm arpa2cm.git && mkdir arpa2cm.git/build && cd arpa2cm.git/build && cmake .. && make && make install
ADD steamworks-test-buildfailure.patch /tmp/steamworks-test-buildfailure.patch
# RUN cd /usr/local/src && git clone https://gitlab.com/arpa2/steamworks steamworks.git && mkdir steamworks.git/build && cd steamworks.git/build && cmake .. && make && make install
RUN cd /usr/local/src && git clone https://gitlab.com/arpa2/steamworks steamworks.git && cd steamworks.git && ( patch -p1 < /tmp/steamworks-test-buildfailure.patch ) && mkdir build && cd build && cmake .. && make && make install
RUN cd /usr/local/src && git clone https://github.com/arpa2/pulleyback-lifecycle pulleyback-lifecycle.git && cd pulleyback-lifecycle.git && mkdir build && cd build && cmake .. && make && make install


#
# Setup the environment
#

COPY lifecycle.sh /usr/bin/lifecycle.sh
COPY initial.ldif /root/initial.ldif

RUN python3 /usr/lib/python3/dist-packages/easy_install.py python3-ldap #TODO:AWAIT:PY3#web2ldap

#TODO:BUILD# RUN cd /usr/local/src ; git clone https://github.com/arpa2/arpa2shell-cmdparser cmdparser.git
#TODO:BUILD# RUN cd /usr/local/src/cmdparser.git/cmdparser ; python setup.py install


#
# Misc files
#
COPY schema/*		/etc/ldap/schema/
RUN rm -rf /etc/ldap/slapd.d
COPY slapd.conf		/etc/ldap/slapd.conf
COPY ldap.conf		/etc/ldap/ldap.conf
COPY slapd.keytab	/etc/krb5.keytab

RUN chmod go+r          /etc/krb5.keytab

EXPOSE 1389:1389

CMD [ "/usr/bin/lifecycle.sh" ]

