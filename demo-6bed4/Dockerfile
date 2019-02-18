# Dockerfile for 6bed4 demonstration
#
# This is arpa2:demo-6bed4.
#
# TODO: This does not seem to be possible in a
#       Docker container?  Also, gdb breaks out.
#
# The 6bed4 tunnel enables IPv6 on any IPv4 network
# that supports UDP.  Where possible, connections are
# made directly; where necessary, connections can be
# made through a fallback server, such as the one
# donated by SURFnet for a long period of time.
#
# This is a transition technology.  It exists to stop
# reasonings of the kind "I will have to wait until
# I can be sure that the others do IPv6 too" by
# actually making it possible, regardless of ISP choice,
# to run IPv6 on any decent network.  In addition, it
# is the first IPv6 tunnel that is usable for RTP
# streams between direct peers, such as for SIP.
#
# Officially, 6bed4 predates ARPA2, but it comes from
# the same source and intentions, so whatever.
#
# See:
# http://devel.0cpm.org/6bed4/
#
# From: Rick van Rein <rick@openfortress.nl>


# ARPA2 base image is the moving target Debian Stable
FROM arpa2:build-bin

# Add tshark, tcpdump and several ping utilities for traffic analysis
RUN apt-get install -y tshark tcpdump mtr-tiny \
	2ping mailping echoping fping hping3 httping ssmping #\
	# radvd wide-dhcpv6-server

# Install the source code, patch and compile.

# ADD https://github.com/vanrein/6bed4/archive/new-stable.tar.gz /root/6bed4-new-stable.tar.gz
# RUN cd /usr/local/src ; tar -zxvf /root/6bed4-new-stable.tar.gz ; cd 6bed4-new-stable ; ./configure ; sed -i nonstd.h -e 's/^#error\>/#warning/' ; make all #NOTADDED:install

RUN cd /usr/local/src ; git clone https://github.com/vanrein/6bed4 6bed4.git
RUN cd /usr/local/src/6bed4.git ; ./configure ; sed -i nonstd.h -e 's/^#error\>/#warning/' ; make all install

RUN mkdir /dev/net ; mknod /dev/net/tun c 10 200

#TODO# Setup script to run

# Define default command
CMD ["/usr/local/sbin/6bed4peer", "-ref"]

