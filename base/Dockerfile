# Dockerfile for ARPA2 base image
#
# This is arpa2/base -- an image based on debian:stable and
# enhanced with software that we generally use within the
# ARPA2 projects.
#
# It defines a default ENTRYPOINT based on dumb-init to
# allow you to just specify a CMD that will run if you
# start the container.  Signals are caught by PID 1 and
# propagated to your programs.
#
# From: Rick van Rein <rick@openfortress.nl>


# ARPA2 base image is the moving target Debian Stable
FROM debian:stable

# Install required packages
RUN \
    apt-get update && \
    apt-get -y upgrade

# ENV DEBIAN_FRONTEND=noninteractive

ADD krb5.conf /etc/krb5.conf
# ADD selections.debconf /tmp/selections.debconf
# RUN debconf-set-selections /tmp/selections.debconf

RUN apt-get install -y dumb-init runit ca-certificates krb5-user openldap-utils ldapvi nano procps less lrzsz net-tools netcat python2.7 python-setuptools python3 python3-setuptools
# python-setuptools # python3

RUN ln -s /bin/nano /usr/bin/vi

RUN ln -s /usr/bin/python2.7 /usr/bin/python
# RUN ln -s /usr/bin/python3 /usr/bin/python

ADD sharetty /usr/bin/sharetty
ADD https://github.com/anishathalye/seashells/archive/v0.1.2.tar.gz /usr/local/src/seashells-0.1.2.tar.gz
RUN cd /usr/local/src ; tar -xzvf seashells-0.1.2.tar.gz

ADD arpa2shell /usr/bin/arpa2shell
ADD arpa2cmd.py /usr/lib/python2.7/dist-packages/arpa2cmd.py
# ADD readline.py /usr/lib/python2.7/dist-packages/readline.py
# ADD arpa2cmd.py /usr/lib/python3/dist-packages/arpa2cmd.py
# ADD readline.py /usr/lib/python3/dist-packages/readline.py

# Add files relative to this Dockerfile
# ADD index.html /var/www/index.html

# Set environment variables
# ENV HOME /var/www

# Define working directory
# WORK /var/www

# Install the ARPA2 install / daemon / shell startup context
ADD usr-arpa2-bin /usr/arpa2/bin
RUN mkdir -p /var/arpa2
RUN echo base > /var/arpa2/docker.name
RUN echo /usr/arpa2/bin/docker.go >> /root/.bash_login

# Your default command could be anything beyond a shell
CMD [ "/usr/arpa2/bin/docker.go" ]

# Vim is quickly becoming unusable.  Be conservative to at least be stable.
COPY vimrc /root/.vimrc

# Define default entry point via dumb-init
# ENTRYPOINT [ "/usr/bin/dumb-init", "-v", "--" ]
ENTRYPOINT [ "/usr/bin/dumb-init", "--" ]
#STUCK-PITTY# ENTRYPOINT [ "/sbin/runit-init" ]
