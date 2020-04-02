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
    apt-get install -y libssl-dev libunbound-dev python3-setuptools python3-dev freediameter libfreediameter-dev
RUN python3 /usr/lib/python3/dist-packages/easy_install.py six pyparsing asn1ate

RUN git clone https://github.com/vanrein/quick-der /usr/local/src/quick-der.git
RUN mkdir /usr/local/src/quick-der.git/build ; cd /usr/local/src/quick-der.git/build ; cmake -DDEBUG:BOOL=OFF ..
RUN make -C /usr/local/src/quick-der.git/build all install
RUN python /usr/local/src/quick-der.git/setup.py install

RUN python -m easy_install arpa2.quickder_tools
RUN python -m easy_install arpa2.quickder
RUN git clone -b sxover-diameter https://gitlab.com/arpa2/kip /usr/local/src/kip.git
RUN mkdir /usr/local/src/kip.git/build ; cd /usr/local/src/kip.git/build ; cmake ..
RUN make -C /usr/local/src/kip.git/build
RUN mkdir /etc/unbound ; cp /usr/local/src/kip.git/test/bin/root.key /etc/unbound/root.key
#RUN cd /usr/local/src/kip.git/build ; ctest

RUN cp /usr/local/src/kip.git/build/src/kip  /usr/local/bin/kip
RUN cp /usr/local/src/kip.git/build/src/kipd /usr/local/bin/kipd

ADD README.MD /KIP.MD
#ADD kip-up-and-down.c   /tmp/kip-up-and-down.c
#ADD kip-up-and-down.kip /tmp/kip-up-and-down.kip
#NOTYET# RUN cp /usr/local/src/kip.git/src/cmdline.c /tmp/kip-up-and-down.c
#NOTYET# RUN /usr/local/bin/kip up /tmp/kip-up-and-down.c /tmp/kip-up-and-down.kip

ENV KIP_REALM=unicorn.demo.arpa2.org
RUN cp /etc/hosts /tmp/hosts ; sed -i "\${s/\$/a kipsvc.unicorn.demo.arpa2.lab/}" /tmp/hosts ; echo 'cp /tmp/hosts /etc/hosts' >> /root/.bashrc
CMD ["bash"]
