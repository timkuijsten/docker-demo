#!/bin/bash

echo
echo Welcome to the ARPA2 IdentityHub Developing Demo.
echo

#### BETTER SET WITH `docker run -h identityhub.arpa2 ...`
echo Horrible hack, moving site-packages
echo
mkdir -p /usr/lib/python2.7/dist-packages
cp -r /site-packages/* /usr/lib/python2.7/dist-packages/

echo Horrible hack, overruling server hostname
echo
#LOCKOUT# sed -i -e '$,$s/^\([^ \t]*\)[ \t]/\1 identityhub.arpa2 /' /etc/hosts
sed -e '$,$s/^\([^ \t]*\)[ \t]/\1 identityhub.arpa2 /' < /etc/hosts > /etc/hosts2 && cp /etc/hosts2 /etc/hosts

echo Installing basic data
echo
slapadd -c -f /etc/ldap/slapd.conf -l /root/initial.ldif

echo 'Starting web2ldap (only over IPv4) on port 1760...'
echo
PYTHONPATH=$PYTHONPATH:/etc/web2ldap web2ldap 0.0.0.0 1760 &

echo 'Starting OpenLDAP (IPv6 and IPv4) on port 1389 and /tmp/ldap-socket...'
echo
/usr/sbin/slapd -d any -h "ldapi://%2ftmp%2fldap-socket ldap://:1389/"
RETVAL=$?

echo
echo "...exit ($RETVAL) from OpenLDAP"
echo
echo Thank you for flying the ARPA2 IdentityHub Developing Demo.
echo

