# Build recipe for a Docker image with Cherry Pi.
#
# This demonstrates a few ideas that we would like to see
# in web services:
#
# * HTTPS security via a TLS Pool daemon
#   - independent and central management of TLS is easier
#   - detaching long-term credentials from application logic
# * Strong authentication
#   - TLS-KDH ciphersuite: post-quantum technology
#   - HTTP SASL: generic authentication support as anywhere
# * Bring Your Own IDentity
#   - Identities from the client's realm, not website-local
#   - Based on realm crossover technology
# * Efficient yet Flexible Authorisation
#   - Access control lists for patterns of remotes
#   - Authorised access to communication or resource (instance)
#   - Central management in a domain's cockpit
#
# From: Rick van Rein <rick@openfortress.nl>



### First stage: Setup Python support
#
FROM arpa2/build-pip AS pip

# RUN pip install cherrypy



### Second stage: Actually construct the image
#
# FROM arpa2/build-bin
FROM arpa2/build-tlspool


# ADD https://github.com/cherrypy/cherrypy/archive/v17.4.0.tar.gz /root/cherrypy-17.4.0.tar.gz
# RUN cd /usr/local/src ; tar -xzvf /root/cherrypy-17.4.0.tar.gz

RUN cd /usr/local/src && git clone https://github.com/arpa2/cheroot.git  cheroot.git
RUN cd /usr/local/src && git clone https://github.com/arpa2/cherrypy.git cherrypy.git

COPY cheroot-tlspool.py /usr/local/bin/
COPY tlspooladapter.py /usr/local/src/cheroot.git/cheroot/ssl/

# RUN apt-get install -y python3-pip
RUN cd /usr/local/src/cheroot.git  && python3 setup.py install
RUN cd /usr/local/src/cherrypy.git && python3 setup.py install

### Final stage: Actually construct the image
#
#TODO#SPLIT#DELIVERABLE# FROM arpa2/base

COPY README.MD		/CherryPi.md

CMD ["bash"]
