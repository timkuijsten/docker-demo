# Dockerfile for Quick DER and LillyDAP
#
# This is a build environment that sets up two
# small tools that are used under several ARPA2
# projects.
#
# Quick DER is a small and efficient DER parser
# and generator.
#
# LillyDAP is a small and efficient LDAP building
# block.
#
# From: Rick van Rein <rick@openfortress.nl>


# Prepare setuptools for import
#
FROM arpa2:build-pip AS pip


# Build on top of the ARPA2 build base image
#
FROM arpa2:build-bin

# Install required packages (already done in arpa2:base)
# RUN \
#     apt-get update && \
#     apt-get -y upgrade

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
# RUN apt-get install -y libdb5.3-dev libgnutls28-dev gnutls-bin libldns-dev
RUN apt-get install -y \
	python-pyparsing python-six \
	gperf

COPY --from=pip /usr/lib/python2.7/dist-packages/setuptools/ /usr/lib/python2.7/dist-packages/setuptools
COPY --from=pip /usr/lib/python2.7/dist-packages/pkg_resources/ /usr/lib/python2.7/dist-packages/pkg_resources

# Install asn1ate
ADD https://github.com/kimgr/asn1ate/archive/v0.6.tar.gz /root/asn1ate-v0.6.tar.gz
RUN cd /usr/local/src && tar -xzvf /root/asn1ate-v0.6.tar.gz
RUN cd /usr/local/src/asn1ate-0.6 && python setup.py install

# Install Quick DER
ADD https://github.com/vanrein/quick-der/archive/version-1.2-6.tar.gz /root/quick-der-version-1.2-6.tar.gz
RUN cd /usr/local/src && tar -xzvf /root/quick-der-version-1.2-6.tar.gz
#TODO#PYTHON#SETUP#INCOMPATIBLE#CHANGES# RUN cd /usr/local/src/quick-der-version-1.2-6 && mkdir build && cd build && cmake .. && make all test install
RUN cd /usr/local/src/quick-der-version-1.2-6 && mkdir build && cd build && cmake .. && make all install
#TODO# Ugly direct copy:
RUN cd /usr/local/src/quick-der-version-1.2-6 && cp python/scripts/* /usr/local/bin && cp -r python/quick_der /usr/local/lib/python2.7/dist-packages/quick_der

# Install hexio -- TODO: Temporary branch with experimental derdump.quickder
RUN cd /usr/local/src ; git clone -b quick-derdump --single-branch \
		https://github.com/vanrein/hexio hexio.git
RUN cd /usr/local/src/hexio.git ; make hexin hexout ; install hexin hexout derdump derdump.quickder /usr/local/bin
RUN cp /usr/local/src/hexio.git/README.MD /HEXIO.MD

# Install LillyDAP
ADD https://github.com/vanrein/lillydap/archive/version-0.9-1.tar.gz /root/lillydap-version-0.9-1.tar.gz
RUN cd /usr/local/src && tar -xzvf /root/lillydap-version-0.9-1.tar.gz
RUN cd /usr/local/src/lillydap-version-0.9-1 && mkdir build && cd build && cmake .. && make all test install

# Trigger library updates
RUN ldconfig

# Define default command
CMD ["/bin/bash"]

