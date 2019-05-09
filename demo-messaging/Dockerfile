# Dockerfile for Qpid Broker as the AMQP exhanges & queues
#
# From: Rick van Rein <rick@openfortress.nl>



### Prepare a name for the Apache Qpid image
#
FROM arpa2/build-qpid AS qpid


### Prepare Python packages with pip -- and forego building it
#
FROM arpa2/build-pip AS pip

#TOO_NEW# RUN pip install qpid-python qpid-tools qpid-bow qpid-qmf 

#SUPERSEDED# RUN pip install python-gssapi

#TODO#DEPSFORPYGSSAPIBUTDOCONLY# pip install sphinx sphinx-rtd-theme recommonmark # sphinxcontrib-napolean ??

#PIPPED# ADD https://github.com/pythongssapi/python-gssapi/archive/v1.5.1.tar.gz /root/python-gssapi-1.5.1.tar.gz
#PIPPED# RUN cd /usr/local/src ; tar -xzvf /root/python-gssapi-1.5.1.tar.gz ; \
#PIPPED# 	cd /usr/local/src/python-gssapi-1.5.1 ; python setup.py install

RUN pip install gssapi

### Start off with the base image
#
FROM arpa2/base


### Install basic toys
#
#TODO# Weird to have to install qpid-python on top of all the source-based build (but it doesn't work there either)
#
RUN apt-get install -y \
	libboost-program-options1.62 libboost-system1.62 libuuid1 python-qpid python-qpid-proton


### Copy the python-gssapi code from the build preparation
#
# From: https://github.com/pythongssapi/python-gssapi
#
RUN apt-get install -y python-six python-enum34
COPY --from=pip /usr/local/lib/python2.7/dist-packages/ /usr/local/lib/python2.7/dist-packages/


### Copy the Qpid Broker to this image
#
#TODO# Add Python libraries for mgt utils
#
#TOO_NEW# COPY --from=pip /usr/local/lib/python2.7/dist-packages /usr/local/lib/python2.7/dist-packages/
#
COPY --from=qpid /usr/local/lib  /usr/local/lib/
COPY --from=qpid /usr/local/bin  /usr/local/bin/
COPY --from=qpid /usr/local/sbin /usr/local/sbin/
#
#TOO_FEW# COPY --from=qpid /usr/local/lib/libqpid*.so  /usr/local/lib/
#TOO_FEW# COPY --from=qpid /usr/local/bin/qpid-*       /usr/local/bin/
#TOO_FEW# COPY --from=qpid /usr/local/sbin/qpid*       /usr/local/sbin/
#
RUN ldconfig
#
COPY services /etc/services
#
COPY qpid.keytab /etc/krb5.keytab
RUN chmod go-rwx /etc/krb5.keytab


### Add local commands
#
#TODO# arpa2amqpshell.d should be run on other hosts!
#TODO# arpa2amqpshell   should be run on other hosts!
#
COPY arpa2amqp.d /usr/local/bin/
COPY arpa2amqp   /usr/local/bin/


### Add explanation files
#
COPY README.MD            /Messaging.md
COPY PROTOCOL.MD          /ShellCommands.md
COPY --from=qpid /Qpid.md /Qpid.md


### Default command is just the shell
#
CMD ["bash"]
