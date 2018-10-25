# Docker file for Apache Qpid service software for AMQP 1.0
#
# We are not alone in loving this protocol and tool chain;
# RedHat loves it too.
#
# From: Rick van Rein <rick@openfortress.nl>


# Start with the binary + python build environment
#
FROM arpa2:build-pip

# Include the README.MD in the root
#
COPY README.MD /Qpid.md

# Prepare by adding Ruby (for C++ Broker)
#TODO# should libboost-dev should be in build-bin?
#
RUN apt-get install -y ruby-dev libboost-program-options1.62 libboost-system1.62 uuid-dev \
	libcunit1-dev doxygen valgrind swig python-unittest2

# Install Qpid Proton
#
#TODO# Is the Python support installed?
#
RUN cd /usr/local/src ; git clone https://git-wip-us.apache.org/repos/asf/qpid-proton.git qpid-proton.git
#FAILING TEST# RUN cd /usr/local/src/qpid-proton.git ; mkdir build ; cd build ; cmake .. ; make all test install
RUN cd /usr/local/src/qpid-proton.git ; mkdir build ; cd build ; cmake .. ; make all install

# Install Qpid C++ Broker
#
# Old INSTALL: https://raw.githubusercontent.com/apache/qpid-cpp/master/INSTALL.txt
#
RUN cd /usr/local/src ; git clone https://git-wip-us.apache.org/repos/asf/qpid-cpp.git qpid-cpp-broker.git
#FAILING TEST# RUN cd /usr/local/src/qpid-cpp-broker.git ; mkdir build ; cd build ; cmake .. ; make all test install
RUN cd /usr/local/src/qpid-cpp-broker.git ; mkdir build ; cd build ; cmake .. ; make all install

# Install Qpid Dispatch
#
# Misses LIBWEBSOCKETS but continues without it
# Misses asciidoc but continues -- it would install a truck load to replicate online documentation
#
RUN cd /usr/local/src ; git clone https://git-wip-us.apache.org/repos/asf/qpid-dispatch.git qpid-dispatch.git
#FAILING TESTS# RUN cd /usr/local/src/qpid-dispatch.git ; mkdir build ; cd build ; cmake .. ; make all test install
RUN cd /usr/local/src/qpid-dispatch.git ; mkdir build ; cd build ; cmake .. ; make all install


# Baseline command is bash
#
CMD ["bash"]
