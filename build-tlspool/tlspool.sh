#!/bin/bash
#
# Run the TLS Pool in its natural habitat.
# This means, in the building context.
#
# From: Rick van Rein <rick@openfortress.nl>

cd /usr/local/src/tlspool*/src
tlspool -kc ../etc/tlspool.conf

sleep 10
echo Enter the keepalive shell
bash
echo Left the keepalive shell
