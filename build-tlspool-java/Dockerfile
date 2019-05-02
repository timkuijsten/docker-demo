# Dockerfile for the TLS Pool in the ARPA2 project
#
# This is a build environment that sets up the
# TLS Pool, a daemon that serves as an intermediate
# for (un)wrapping TLS traffic.  The idea is to
# isolate long-lasting credentials from application
# processes, and thereby avoid attacks that would
# go for private keys.
#
# From: Rick van Rein <rick@openfortress.nl>


FROM build-tlspool

# Install required packages (already done in arpa2:base)
# RUN \
#     apt-get update && \
#     apt-get -y upgrade

# Install dependencies
RUN apt-get install -y openjdk-8-jdk ant wget
# Get recent cmake
RUN cd /opt && wget https://github.com/Kitware/CMake/releases/download/v3.14.2/cmake-3.14.2-Linux-x86_64.sh
# Install recent cmake
RUN cd /opt && mkdir cmake-3.14.2-Linux-x86_64 && cd cmake-3.14.2-Linux-x86_64 && sh ../cmake-3.14.2-Linux-x86_64.sh --skip-license
# clone tlspool including java support
RUN cd /usr/local/src && git clone https://github.com/hfmanson/tlspool
# checkout branch including java support and install
RUN cd /usr/local/src/tlspool && git checkout no_poolhandle_t && mkdir build && cd build && /opt/cmake-3.14.2-Linux-x86_64/bin/cmake .. && make install
# build Netbeans Tlspool.jar
RUN cd /usr/local/src/tlspool/lib/java/Tlspool && ant

CMD ["bash"]
