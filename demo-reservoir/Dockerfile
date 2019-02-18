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

#TEST#JSON#VARIANT# RUN pip install cmdparser python-ldap riak six
RUN pip install python-ldap riak six

RUN cd /usr/local/src ; git clone https://github.com/arpa2/arpa2shell-cmdparser cmdparser.git

RUN cd /usr/local/src/cmdparser.git/cmdparser ; python setup.py install


### Prepare: Build Riak KV from source
#
# Riak KV may not be maintained any longer; its binaries
# have not been updated since November 2017 and are now
# lagging behind Debian Stable.  Its source code has had
# a preview on 2.2.4 ever since that same period, but it
# has not been put forward.  It looks like development
# has come to a sudden stop.  Building from source also
# reveals cracks, namely incompatibility with Erlang as
# distributed nowadays in Debian Stable, causing
#
### ERROR: OTP release 19 does not match required regex R16|17
### ERROR: compile failed while processing /usr/local/src/riak-riak-2.2.3/deps/node_package: rebar_abort
#
# This is why binaries for Riak KV and libssl1.0.0 have
# been included.  We should monitor Riak KV because it
# may be an upcoming problem.  There are more kinds of
# object store, so there is no cause for panic.  New
# installations (not this mere demo) may wonder what to
# do before sending customer objects to their store.
#
# Note that tiot.jp references newer Riak KV versions,
# https://files.tiot.jp/riak/kv/2.2/2.2.6/
# No idea who they are; found them only on IRC at
# freenode.net #riak -- so doubtful as a source.

# FROM arpa2:build-bin AS bin
# 
# RUN apt-get install -y erlang
# 
# ADD https://github.com/basho/riak/archive/riak-2.2.3.tar.gz /root/riak-2.2.3.tar.gz
# 
# RUN cd /usr/local/src ; tar -xzvf /root/riak-2.2.3.tar.gz
# RUN cd /usr/local/src/riak-riak-2.2.3 ; make rel install


### Prepare: Riak KV
#
# First, a preparation for Reservoir: installing Riak KV.
#
# The instructions for doing this are of the "wget | su" type,
# which isn't generally acceptable.
#
# We therefore run the necessary steps in a build image, and
# copy Riak KV from there.


#BELOW# FROM arpa2:base AS riakkv

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

#BELOW# CMD [ "/bin/bash" ]


### Setup: Riak KV (.deb), OpenLDAP
#
#


FROM arpa2:base


# Install Riak KV from .deb (moved from option 2 above)
#

# Install libssl1.0.0 old-school dependency from binary
#TRY_TIOT_JP# COPY libssl1.0.0_*_amd64.deb /root
#TRY_TIOT_JP# RUN dpkg -i /root/libssl1.0.0_1.0.1t-1+deb7u4_amd64.deb

RUN apt-get update && apt-get -y upgrade
RUN apt-get install --no-install-recommends -y \
	openssl logrotate sudo

#TRY_TIOT_JP# COPY riak_*_amd64.deb /root
#TRY_TIOT_JP# RUN ( dpkg -i --force-depends /root/riak_*_amd64.deb || /bin/true )

ADD https://files.tiot.jp/riak/kv/2.2/2.2.6/debian/9/riak_2.2.6-1_amd64.deb /root/riak_2.2.6-1_amd64.deb
RUN dpkg -i /root/riak_2.2.6-1_amd64.deb

#SATISFIED-DEPENDENCY# # Upgrade Riak KV dependency on libssl1.0.0 to Debian Stable libssl1.0.2l+
#SATISFIED-DEPENDENCY# RUN grep libssl1.0.0 /var/lib/dpkg/status
#SATISFIED-DEPENDENCY# RUN sed -i -e 's+libssl1\.0\.0 (>= 1\.0\.0)+libssl1.0.2 (>= 1.0.2l)+' /var/lib/dpkg/status
#SATISFIED-DEPENDENCY# #WILLNOWFAIL!# RUN grep libssl1.0.0 /var/lib/dpkg/status



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

COPY arpa2reservoir.py /usr/local/lib/python2.7/dist-packages/arpa2reservoir.py
#PY3# COPY arpa2reservoir.py /usr/local/lib/python3/dist-packages/arpa2reservoir.py
RUN ln -s /usr/local/lib/python2.7/dist-packages/arpa2reservoir.py /usr/bin/arpa2reservoir
#PY3# RUN ln -s /usr/local/lib/python3/dist-packages/arpa2reservoir.py /usr/bin/arpa2reservoir
COPY arpa2acl.py /usr/local/lib/python2.7/dist-packages/arpa2acl.py
RUN ln -s /usr/local/lib/python2.7/dist-packages/arpa2acl.py /usr/bin/arpa2acl
RUN chmod ugo+x /usr/bin/arpa2reservoir /usr/bin/arpa2acl
COPY --from=pip /usr/bin/arpa2gnu /usr/bin/arpa2gnu
COPY --from=pip /usr/bin/arpa2kinit /usr/bin/arpa2kinit


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

