############################################################
###                                                      ###
###   ARPA2 RESERVOIR DOCKER IMAGE CONSTRUCTION SCRIPT   ###
###                                                      ###
###   From: Rick van Rein <rick@openfortress.nl>         ###
###                                                      ###
############################################################


### Prepare: PIP
#
# Install Python dependencies for Reservoir

#PIP2EASYINSTALL# FROM arpa2/build-pip AS pip

#TEST#JSON#VARIANT# RUN pip install cmdparser python-ldap riak six

#PIP2EASYINSTALL# RUN pip install python-ldap riak six


### Prepare: Reservoir Demonstration Files

FROM arpa2/files-reservoir AS reservoir



### Setup: OpenLDAP
#
#


#EASY_INSTALL_LDAP# FROM arpa2/base
FROM arpa2/build-bin


RUN apt-get update && apt-get -y upgrade
RUN apt-get install --no-install-recommends -y \
	openssl logrotate sudo libncurses5 libtinfo5 \
	python3-dev

RUN python3 -m easy_install python3-ldap six web2ldap

#TODO:BUILD# RUN cd /usr/local/src ; git clone https://github.com/arpa2/arpa2shell-cmdparser cmdparser.git
#TODO:BUILD# RUN cd /usr/local/src/cmdparser.git/cmdparser ; python setup.py install



# Install OpenLDAP's slapd daemon (overlaps arpa2/identityhub)
#

RUN DEBIAN_FRONTEND=noninteractive SLAPD_PASSWORD=sekreet \
	apt-get install -y --no-install-recommends \
	slapd libsasl2-modules-gssapi-mit

COPY initial.ldif /root/initial.ldif
# COPY index.ldif   /root/index.ldif
COPY webdav-reservoir/schema/* /etc/ldap/schema/
RUN rm -rf /etc/ldap/slapd.d
COPY slapd.conf /etc/ldap/slapd.conf
COPY ldap.conf /etc/ldap/ldap.conf
COPY reservoir.keytab /etc/krb5.keytab
# RUN chmod go-rwx /etc/krb5.keytab
RUN chmod go+r /etc/krb5.keytab

#RATHER #COPY slapd.keytab	/etc/ldap/slapd.keytab
#RATHER #RUN chown opldap:openldap /etc/ldap/slapd.keytab


# Copy files-reservoir into /var/arpa2/reservoir
#
COPY --from=reservoir /var/arpa2/reservoir /var/arpa2/reservoir


# Install operator's shells to play around with the Reservoir
#

COPY arpa2reservoir.py /usr/local/lib/python2.7/dist-packages/arpa2reservoir.py
#PY3# COPY arpa2reservoir.py /usr/local/lib/python3/dist-packages/arpa2reservoir.py
RUN ln -s /usr/local/lib/python2.7/dist-packages/arpa2reservoir.py /usr/bin/arpa2reservoir
#PY3# RUN ln -s /usr/local/lib/python3/dist-packages/arpa2reservoir.py /usr/bin/arpa2reservoir
COPY arpa2acl.py /usr/local/lib/python2.7/dist-packages/arpa2acl.py
RUN ln -s /usr/local/lib/python2.7/dist-packages/arpa2acl.py /usr/bin/arpa2acl
RUN chmod ugo+x /usr/bin/arpa2reservoir /usr/bin/arpa2acl
COPY arpa2gnu /usr/bin/arpa2gnu
COPY arpa2kinit /usr/bin/arpa2kinit


# Install the main script for the Reservoir service
#
COPY README.MD /Reservoir.md
COPY reservoir.sh /usr/bin/reservoir.sh
EXPOSE 1388:1388 1761:1761
CMD [ "/usr/bin/reservoir.sh" ]

