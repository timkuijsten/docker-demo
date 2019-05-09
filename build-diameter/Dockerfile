# build-diameter downloads and builds FreeDiameter
#
# We can use Diameter in a number of access control scenarios:
#  - as a backend SASL validator
#  - to discover ARPA2 ACL rights and unfolding aliases
#  - to resolve identity inheritance questions
#
# From: Rick van Rein <rick@openfortress.nl>


FROM arpa2/build-bin

RUN apt-get install -y libgnutls28-dev gnutls-bin libidn11-dev libsctp-dev lksctp-tools libgcrypt11-dev # mercurial

ADD http://www.freediameter.net/hg/freeDiameter/archive/1.2.0.tar.gz /root/freediameter-1.2.0.tar.gz

RUN cd /usr/local/src ; tar -xzvf /root/freediameter-1.2.0.tar.gz
RUN cd /usr/local/src/freeDiameter-1.2.0 ; mkdir build ; cd build ; cmake -Wno-dev ..
#TEST_FAILS_ON_SCTP_IN_DOCKER# RUN cd /usr/local/src/freeDiameter-1.2.0/build ; make all test install ; ldconfig
RUN cd /usr/local/src/freeDiameter-1.2.0/build ; make all install ; ldconfig


CMD ["bash"]
