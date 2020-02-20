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


### Prepare: Qpid Runtime code

FROM arpa2/build-qpid AS qpid


### Setup: OpenLDAP
#
#


#EASY_INSTALL_LDAP# FROM arpa2/base
FROM arpa2/build-bin


# First install Qpid Dispatch Router
COPY --from=qpid /usr/local/ /usr/local/


RUN apt-get update && apt-get -y upgrade
RUN apt-get install --no-install-recommends -y \
	openssh-server \
	logrotate libncurses5 libtinfo5 \
	python3-dev python3-gssapi python3-qpid-proton python3-twisted

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
COPY schema/* /etc/ldap/schema/
RUN rm -rf /etc/ldap/slapd.d
COPY slapd.conf /etc/ldap/slapd.conf
COPY ldap.conf /etc/ldap/ldap.conf
COPY reservoir.keytab /etc/krb5.keytab
COPY arpa2api.keytab /etc/arpa2/arpa2api.keytab
# RUN chmod go-rwx /etc/krb5.keytab
RUN chmod go+r /etc/krb5.keytab

RUN echo >>/etc/ssh/sshd_config PermitRootLogin yes
RUN echo >>/etc/ssh/sshd_config Subsystem arpa2shell \
                           /usr/local/bin/arpa2shell
RUN echo >>/etc/ssh/sshd_config Subsystem arpa2client \
                           /usr/local/bin/arpa2client
RUN echo >>/etc/ssh/sshd_config Subsystem arpa2reservoir \
                           /usr/local/bin/arpa2reservoir

#RATHER #COPY slapd.keytab	/etc/ldap/slapd.keytab
#RATHER #RUN chown opldap:openldap /etc/ldap/slapd.keytab


# Copy files-reservoir into /var/arpa2/reservoir
#
COPY --from=reservoir /var/arpa2/reservoir /var/arpa2/reservoir


# Install utitilities commonly used in the demo environments
#
COPY arpa2gnu /usr/bin/arpa2gnu
COPY arpa2kinit /usr/bin/arpa2kinit


# Install the arpa2shell utilities from Git
#
#TMP#DEV# RUN git clone https://gitlab.com/arpa2/shell /usr/local/src/arpa2shell.git
RUN git clone https://gitlab.com/arpa2/shell.git /usr/local/src/arpa2shell.git
RUN python /usr/local/src/arpa2shell.git/setup.py install

# Install the arpa2.reservoir core logic
#
RUN git clone https://gitlab.com/arpa2/reservoir.git /usr/local/src/arpa2reservoir.git
RUN python /usr/local/src/arpa2reservoir.git/setup.py install

# Install Henri's cute little web demo
#
COPY MusicLister.py MusicLister.xml /usr/local/src/

# Install the main script for the Reservoir service
#
COPY README.MD /Reservoir.md
COPY reservoir.sh /usr/bin/reservoir.sh
EXPOSE 1388:1388 1761:1761
CMD [ "/usr/bin/reservoir.sh" ]

