#!/bin/bash
#
# Run the TLS Pool in its natural habitat.
# This means, in the building context.
#
# From: Rick van Rein <rick@openfortress.nl>

cd /usr/local/src/tlspool*/src
tlspool -kc /etc/tlspool.conf

# Sleep forever, allowing the TLS Pool to spill its beans
while true
do
	sleep 10
done
# echo Enter the keepalive shell
# bash
# echo Left the keepalive shell
