
FROM arpa2/build-bin

RUN apt-get -y install libssl-dev libunbound-dev

ADD kip-initial.tar.gz /usr/local/src/

RUN cd /usr/local/src/kip-initial ; mkdir build ; cd build ; cmake .. || make ; cmake .. ; make all ; ctest ; make install

RUN cp /usr/local/src/kip-initial/build/src/silly-kip  /usr/local/bin/kip
RUN cp /usr/local/src/kip-initial/build/src/silly-kipd /usr/local/bin/kipd

ADD README.MD /KIP.MD
ADD kip-up-and-down.c   /tmp/kip-up-and-down.c
ADD kip-up-and-down.kip /tmp/kip-up-and-down.kip

ENV KIP_REALM=unicorn.demo.arpa2.org

CMD ["bash"]
