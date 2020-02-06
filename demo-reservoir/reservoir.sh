#!/bin/bash

echo
echo Welcome to the ARPA2 Reservoir Developing Demo.
echo

echo Horrible hack, overruling server hostname
echo
#LOCKOUT# sed -i -e '$,$s/^\([^ \t]*\)[ \t]/\1 reservoir.arpa2 /' /etc/hosts
sed -e '$,$s/^\([^ \t]*\)[ \t]/\1 reservoir.arpa2 /' < /etc/hosts > /etc/hosts2 && cp /etc/hosts2 /etc/hosts

echo Configuring OpenLDAP
echo
slapadd -c -f /etc/ldap/slapd.conf -l /root/initial.ldif
slapadd -c -f /etc/ldap/slapd.conf -l /var/arpa2/reservoir/index.ldif

#NOTHERE# echo 'Starting web2ldap (only over IPv4) on port 1761...'
#NOTHERE# echo
#NOTHERE# PYTHONPATH=$PYTHONPATH:/etc/web2ldap web2ldap 0.0.0.0 1761 &

echo 'Starting OpenLDAP (IPv6 and IPv4) on port 1388 and /tmp/ldap-socket...'
echo
/usr/sbin/slapd -d any -h "ldapi://%2ftmp%2fldap-socket ldap://:1388/"
RETVAL=$?

echo
echo "...exit ($RETVAL) from OpenLDAP"
echo
echo Thank you for flying the ARPA2 Reservoir Developing Demo.
echo

