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

FROM arpa2:build-pip AS pip

RUN pip install cmdparser python-ldap riak six



### Prepare: Riak KV
#
# First, a preparation for Reservoir: installing Riak KV.
#
# The instructions for doing this are of the "wget | su" type,
# which isn't generally acceptable.
#
# We therefore run the necessary steps in a build image, and
# copy Riak KV from there.


FROM arpa2:base AS riakkv

#
# Option 1. Setting up an instable environment
#

# RUN apt-get update && apt-get -y upgrade
# RUN apt-get install --no-install-recommends -y \
# 	gnupg apt-transport-https
# 
# COPY basho-distro.asc /root
# COPY basho-sources.list /etc/apt/sources.list.d/basho-sources.list
# 
# RUN apt-key add /root/basho-distro.asc &>/dev/null
# 
# RUN apt-get update && apt-get -y upgrade
# 
# RUN DEBIAN_FRONTEND=noninteractive \
# 	apt-get install --no-install-recommends -y \
# 	riak

#
# Option 2. Install from dpkg
#

#BELOW# RUN apt-get update && apt-get -y upgrade
#BELOW# RUN apt-get install --no-install-recommends -y \
#BELOW# 	openssl logrotate sudo
#BELOW# 
#BELOW# COPY riak_2.2.3-1_amd64.deb /root
#BELOW# RUN dpkg -i --force-depends /root/riak_*_amd64.deb || /bin/true

CMD [ "/bin/bash" ]


### Setup: Riak KV (.deb), OpenLDAP
#
#


FROM arpa2:base


# Install Riak KV from .deb (moved from option 2 above)
#

RUN apt-get update && apt-get -y upgrade
RUN apt-get install --no-install-recommends -y \
	openssl logrotate sudo

COPY riak_*_amd64.deb /root
RUN ( dpkg -i --force-depends /root/riak_*_amd64.deb || /bin/true )

# Upgrade Riak KV dependency on libssl1.0.0 to Debian Stable libssl1.0.2l+
RUN grep libssl1.0.0 /var/lib/dpkg/status
RUN sed -i -e 's+libssl1\.0\.0 (>= 1\.0\.0)+libssl1.0.2 (>= 1.0.2l)+' /var/lib/dpkg/status
#WILLNOWFAIL!# RUN grep libssl1.0.0 /var/lib/dpkg/status



# Install OpenLDAP's slapd daemon (overlaps arpa2:identityhub)
#

RUN DEBIAN_FRONTEND=noninteractive SLAPD_PASSWORD=sekreet \
	apt-get install -y --no-install-recommends \
	slapd libsasl2-modules-gssapi-mit

COPY initial.ldif /root/initial.ldif
COPY webdav-reservoir/schema/* /etc/ldap/schema/
RUN rm -rf /etc/ldap/slapd.d
COPY slapd.conf /etc/ldap/slapd.conf
COPY ldap.conf /etc/ldap/ldap.conf
COPY reservoir.keytab /etc/krb5.keytab
# RUN chmod go-rwx /etc/krb5.keytab
RUN chmod go+r /etc/krb5.keytab

#RATHER #COPY slapd.keytab	/etc/ldap/slapd.keytab
#RATHER #RUN chown opldap:openldap /etc/ldap/slapd.keytab


# Install operator's shells to play around with the Reservoir
#

COPY webdav-reservoir/arpa2resv /usr/lib/python2.7/dist-packages/arpa2resv.py
#PY3# COPY arpa2resv.py /usr/lib/python3/dist-packages/arpa2resv.py
RUN ln -s /usr/lib/python2.7/dist-packages/arpa2resv.py /usr/bin/arpa2resv
#PY3# RUN ln -s /usr/lib/python3/dist-packages/arpa2resv.py /usr/bin/arpa2resv
RUN chmod ugo+x /usr/bin/arpa2resv
COPY --from=pip /usr/bin/arpa2gnu /usr/bin/arpa2gnu


# Install some Python extras
#

# COPY py/lib/python2.7/site-packages/ /usr/lib/python2.7/dist-packages/
# COPY py/lib/python2.7/site-packages/ /site-packages/

COPY --from=pip /usr/local/lib/python2.7/dist-packages/ /usr/local/lib/python2.7/dist-packages
COPY --from=pip       /usr/lib/python2.7/dist-packages/       /usr/lib/python2.7/dist-packages


# Install the main script for the Reservoir service
#
COPY README.MD /Reservoir.md
COPY reservoir.sh /usr/bin/reservoir.sh
EXPOSE 1388:1388 1761:1761
CMD [ "/usr/bin/reservoir.sh" ]

