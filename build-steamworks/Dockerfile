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
FROM arpa2/build-bin AS src2export

# Install required packages (already done in arpa2/base)
# RUN \
#     apt-get update && \
#     apt-get -y upgrade

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get install -y liblog4cpp5-dev libsqlite3-dev libfcgi-bin

# Install ARPA2CM
RUN git clone https://gitlab.com/arpa2/arpa2cm.git /usr/local/src/arpa2cm.git
#test# RUN cd /usr/local/src/arpa2cm.git && mkdir build && cd build && cmake .. && make all test install
RUN cd /usr/local/src/arpa2cm.git && mkdir build && cd build && cmake .. && make all install

# Install SteamWorks
# ADD https://github.com/arpa2/steamworks/archive/version-0.93-beta4.tar.gz /root/steamworks-version-0.93-beta4.tar.gz
RUN git clone https://gitlab.com/arpa2/steamworks.git /usr/local/src/steamworks.git
# RUN cd /usr/local/src && tar -xzvf /root/steamworks-version-0.93-beta4.tar.gz
# RUN cd /usr/local/src/steamworks-version-0.93-beta4 && mkdir build && cd build && cmake .. && make all test install
RUN cd /usr/local/src/steamworks.git && mkdir build && cd build && cmake .. && make all test install
RUN make -C /usr/local/src/steamworks.git/build DESTDIR=/export/steamworks install

# Install PulleyBack-LifeCycle
# RUN cd /usr/local/src ; git clone https://github.com/arpa2/pulleyback-lifecycle pulleyback-lifecycle.git
RUN git clone https://gitlab.com/arpa2/steamworks-pulleyback /usr/local/src/steamworks-pulleyback.git
# RUN cd /usr/local/src/pulleyback-lifecycle.git ; mkdir build ; cd build ; cmake .. ; make all test install
RUN cd /usr/local/src/steamworks-pulleyback.git ; mkdir build ; cd build ; cmake .. ; make all test install
RUN make -C /usr/local/src/steamworks-pulleyback.git/build DESTDIR=/export/steamworks install
# RUN mkdir -p /usr/local/share/steamworks/pulleyscript/ && cp /usr/local/src/pulleyback-lifecycle.git/example/certflow/* /usr/local/share/steamworks/pulleyscript/
RUN mkdir -p /usr/local/share/steamworks/pulleyscript/ && cp /usr/local/src/steamworks-pulleyback.git/lifecycle/example/certflow/* /usr/local/share/steamworks/pulleyscript/
RUN cd /usr/local/share/steamworks/pulleyscript && ln -s a2lc_null a2lc_x509 && ln -s a2lc_null a2lc_acme && ln -s a2lc_null a2lc_dane && ln -s a2lc_null a2lc_tlspool
RUN mkdir -p /export/steamworks/usr/local/share/steamworks/pulleyscript/ && cp /usr/local/src/steamworks-pulleyback.git/lifecycle/example/certflow/* /export/steamworks/usr/local/share/steamworks/pulleyscript/
RUN cd /export/steamworks/usr/local/share/steamworks/pulleyscript && ln -s a2lc_null a2lc_x509 && ln -s a2lc_null a2lc_acme && ln -s a2lc_null a2lc_dane && ln -s a2lc_null a2lc_tlspool

RUN sed -i 's+a2lc_+/usr/local/share/steamworks/pulleyscript/a2lc_+g' /usr/local/share/steamworks/pulleyscript/certflow.ply
RUN sed -i 's+a2lc_+/usr/local/share/steamworks/pulleyscript/a2lc_+g' /export/steamworks/usr/local/share/steamworks/pulleyscript/certflow.ply

# RUN mkdir -p /var/db/pulley && cp /usr/local/src/steamworks-version-0.93-beta4/src/pulley/examples/pulley.properties /
RUN mkdir -p /var/db/pulley && cp /usr/local/src/steamworks.git/src/pulley/examples/pulley.properties /
RUN mkdir -p /export/steamworks/var/db/pulley && cp /usr/local/src/steamworks.git/src/pulley/examples/pulley.properties /export/steamworks

# Add a few Pulley Scripts
ADD pulley/ /root/pulley
ADD pulley/ /export/steamworks/root/pulley


#
# Harvest from src2export
#
FROM arpa2/base

RUN apt-get install libpython3.7

COPY --from=src2export /export/steamworks /

# Define default command
CMD ["/bin/bash"]

