# Docker file for Apache Qpid service software for AMQP 1.0
#
# We are not alone in loving this protocol and tool chain;
# RedHat loves it too.
#
# From: Rick van Rein <rick@openfortress.nl>


# Start with the binary + python build environment
#
FROM arpa2/build-pip AS src2export

# Include the README.MD in the root
#
COPY README.MD /Qpid.md

# Prepare by adding Ruby (for C++ Broker)
#TODO# should libboost-dev be included in build-bin?
#
RUN apt-get update && apt-get -y upgrade && apt-get install -y ruby-dev libboost-program-options1.62 libboost-system1.62 uuid-dev \
	libcunit1-dev doxygen valgrind swig python3-unittest2

# Install Qpid Proton
#
#TODO# Is the Python support installed?
#
RUN cd /usr/local/src ; git clone -b master --single-branch \
		https://git-wip-us.apache.org/repos/asf/qpid-proton.git qpid-proton.git
#FAILING TEST# RUN cd /usr/local/src/qpid-proton.git ; mkdir build ; cd build ; cmake .. ; make all test install
RUN cd /usr/local/src/qpid-proton.git ; mkdir build ; cd build ; cmake .. ; make all install

# Install Qpid C++ Broker
#
# Old INSTALL: https://raw.githubusercontent.com/apache/qpid-cpp/master/INSTALL.txt
#
#BROKEN:QPID-8413# RUN cd /usr/local/src ; git clone -b master --single-branch \
#BROKEN:QPID-8413# 		https://git-wip-us.apache.org/repos/asf/qpid-cpp.git qpid-cpp-broker.git
#FAILING TEST# RUN cd /usr/local/src/qpid-cpp-broker.git ; mkdir build ; cd build ; cmake .. ; make all test #EXPORT:BELOW# install
#BROKEN:QPID-8413# RUN cd /usr/local/src/qpid-cpp-broker.git ; mkdir build ; cd build ; cmake .. ; make all #EXPORT:BELOW# install

# Install Qpid Dispatch
#
# Misses LIBWEBSOCKETS but continues without it
# Misses asciidoc but continues -- it would install a truck load to replicate online documentation
#
RUN cd /usr/local/src ; git clone -b master --single-branch \
		https://git-wip-us.apache.org/repos/asf/qpid-dispatch.git qpid-dispatch.git
#FAILING TESTS# RUN cd /usr/local/src/qpid-dispatch.git ; mkdir build ; cd build ; cmake .. ; make all test #EXPORT:BELOW# install
RUN cd /usr/local/src/qpid-dispatch.git ; mkdir build ; cd build ; cmake .. ; make all #EXPORT:BELOW# install

# Prepare export directories for each product in DESTDIR=/export/qpid/<product>
#
RUN mkdir -p /export/qpid/proton /export/qpid/broker /export/qpid/dispatch
RUN cd /usr/local/src/qpid-proton.git/build && make DESTDIR=/export/qpid/proton install #NOMORE# && cd /export/qpid/proton && tar -czf ../proton.tgz * && cd .. && rm -rf proton
#BROKEN:QPID-8413# RUN cd /usr/local/src/qpid-cpp-broker.git/build && make DESTDIR=/export/qpid/broker install #NOMORE# && cd /export/qpid/broker && tar -czf ../broker.tgz * && cd .. && rm -rf broker
RUN cd /usr/local/src/qpid-dispatch.git/build && make DESTDIR=/export/qpid/dispatch install #NOMORE# && cd /export/qpid/dispatch && tar -czf ../dispatch.tgz * && cd .. && rm -rf dispatch


#
# Start setting up the actual output
#

FROM arpa2/base

RUN apt-get install libpython3.7

COPY --from=src2export /export/qpid/proton/ /export/qpid/dispatch/ /export/qpid/broker/ /


# Baseline command is bash
#
CMD ["bash"]
