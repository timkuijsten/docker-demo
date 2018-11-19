# Dockerfile for ARPA2 SteamWorks (Crank and Pulley)
#
# These are the LDAP tools for ARPA2, together called
# SteamWorks.  The Crank is a JSON-interface over
# FastCGI to directly access LDAP directory contents;
# The Pulley is a scripted tool to gather information
# from LDAP, combine them and deliver tuple additions
# and removals to a backend library, while staying
# tuned to new developments.
#
# From: Rick van Rein <rick@openfortress.nl>


# ARPA2 base image is the moving target Debian Stable
FROM arpa2:build-bin

# Install required packages (already done in arpa2:base)
# RUN \
#     apt-get update && \
#     apt-get -y upgrade

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get install -y liblog4cpp5-dev libsqlite3-dev

# Install SteamWorks
ADD https://github.com/arpa2/steamworks/archive/version-0.93-beta4.tar.gz /root/steamworks-version-0.93-beta4.tar.gz
RUN cd /usr/local/src && tar -xzvf /root/steamworks-version-0.93-beta4.tar.gz
RUN cd /usr/local/src/steamworks-version-0.93-beta4 && mkdir build && cd build && cmake .. && make all test install

# Install PulleyBack-LifeCycle
RUN cd /usr/local/src ; git clone https://github.com/arpa2/pulleyback-lifecycle pulleyback-lifecycle.git
RUN cd /usr/local/src/pulleyback-lifecycle.git ; mkdir build ; cd build ; cmake .. ; make all test install
RUN mkdir -p /usr/local/share/steamworks/pulleyscript && cp /usr/local/src/pulleyback-lifecycle.git/example/certflow/certflow.ply /usr/local/share/steamworks/pulleyscript/certflow.ply

# Add a few Pulley Scripts
ADD pulley/ /root/pulley

# Define default command
CMD ["/bin/bash"]

