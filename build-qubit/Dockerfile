FROM arpa2:build-bin

# RUN apt-get -y install binutils-gold libboost-all-dev libgmp-dev libgmpxx4ldbl libmpfr-dev
# RUN cd /usr/local/src ; git clone https://github.com/epiqc/ScaffCC.git scaffcc.git
# RUN make -C /usr/local/src/scaffcc.git

RUN apt-get -y install libreadline-dev libplot-dev ncurses-dev
ADD http://tph.tuwien.ac.at/~oemer/tgz/qcl-0.6.4.tgz /root/qcl-0.6.4.tgz
RUN cd /usr/local/src ; tar -xzvf /root/qcl-0.6.4.tgz qcl-0.6.4
RUN make -C /usr/local/src/qcl-0.6.4 install

CMD ["qcl"]
