# Build recipe for a Docker image with the KeyMaster demo.
#
# This image is meant to control credentials, such as private
# keys and certificates, but also the many variations that
# these have.
#
# We do our best to tie credentials in with validation infra
# such as ACME and DANE, so there is a need for controlling
# the processes for rolling them out.  This is done with an
# event-based structure around managedState: attributes in
# LDAP.
#
# This is meant to grow into a live KeyMaster, just run on
# Docker for developer convenience and, who knows one day,
# for some deployers too.  The Dockerfiles double as a live
# and quickly maintained HOWTO document for installing the
# KeyMaster service.
#
# From: Rick van Rein <rick@openfortress.nl>


### First stage: Import SteamWorks build environment
#
FROM arpa2/build-steamworks AS steamworks



### Second stage: Incorporate Python dependencies
#
FROM arpa2/build-pip AS pip



### Final stage: Construct the runtime image
#
FROM arpa2/base

RUN DEBIAN_FRONTEND=noninteractive SLAPD_PASSWORD=sekreet \
	apt-get install --no-install-recommends -y \
	slapd libsasl2-modules-gssapi-mit \
	libsqlite3-dev liblog4cpp5v5 libfcgi-bin

COPY --from=steamworks /usr/local/bin/pulley       /usr/local/bin/pulley
COPY --from=steamworks /usr/local/bin/crank        /usr/local/bin/crank
COPY --from=steamworks /usr/local/share/steamworks /usr/local/share/steamworks/

COPY schema/* /etc/ldap/schema/
RUN rm -rf /etc/ldap/slapd.d
COPY slapd.conf /etc/ldap/slapd.conf
COPY ldap.conf /etc/ldap/ldap.conf
# COPY slapd.keytab /etc/krb5.keytab
RUN chmod go+w /var/run /run
RUN chmod o+t  /var/run /run

COPY arpa2keys.py /usr/local/lib/python2.7/dist-packages/arpa2keys.py
RUN ln -s /usr/local/lib/python2.7/dist-packages/arpa2keys.py /usr/bin/arpa2keys
RUN chmod ugo+x /usr/bin/arpa2keys
COPY --from=pip /usr/bin/arpa2kinit /usr/bin/arpa2kinit
COPY --from=pip /usr/bin/arpa2gnu   /usr/bin/arpa2gnu

CMD ["bash"]

