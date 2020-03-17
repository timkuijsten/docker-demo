# build-diameter downloads and builds FreeDiameter
#
# We can use Diameter in a number of access control scenarios:
#  - as a backend SASL validator
#  - to discover ARPA2 ACL rights and unfolding aliases
#  - to resolve identity inheritance questions
#
# From: Rick van Rein <rick@openfortress.nl>


FROM arpa2/build-bin

RUN apt-get update && \
    apt-get install -y libgnutls28-dev gnutls-bin libidn11-dev libsctp-dev lksctp-tools libgcrypt20-dev # mercurial

ADD http://www.freediameter.net/hg/freeDiameter/archive/1.3.2.tar.gz /root/freediameter-1.3.2.tar.gz
ADD fd-132-oldapi.patch /root/fd-132-oldapi.patch

RUN cd /usr/local/src && tar -xzvf /root/freediameter-1.3.2.tar.gz && cd freeDiameter-1.3.2 && patch -p1 < /root/fd-132-oldapi.patch
RUN cd /usr/local/src/freeDiameter-1.3.2 ; mkdir build ; cd build ; cmake -Wno-dev ..
#TEST_FAILS_ON_SCTP_IN_DOCKER# RUN cd /usr/local/src/freeDiameter-1.3.2/build ; make all test install ; ldconfig
RUN cd /usr/local/src/freeDiameter-1.3.2/build ; make all install ; ldconfig

RUN python3 -m easy_install pyDiameter


CMD ["bash"]
