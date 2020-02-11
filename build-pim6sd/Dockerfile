# This builds pim6sd, a source-specific multicast router for IPv6
#
# It is not useful inside a Docker, unless it runs unconstrained

FROM arpa2/build-bin

RUN apt-get install -y autoconf

RUN git clone https://github.com/troglobit/pim6sd /usr/local/src/pim6sd.git
RUN cd /usr/local/src/pim6sd.git && ./autogen.sh && ./configure && make && make install
RUN cp /usr/local/src/pim6sd.git/pim6sd.conf.sample /etc/pim6sd.conf
