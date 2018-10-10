# Dockerfile for ARPA2 base image
#
# This is arpa2:base -- an image based on debian:stable and
# enhanced with software that we generally use within the
# ARPA2 projects.
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

RUN apt-get install -y krb5-user openldap-utils ldapvi nano procps less lrzsz net-tools netcat python2.7 # python3

RUN ln -s /bin/nano /usr/bin/vi

RUN ln -s /usr/bin/python2.7 /usr/bin/python
# RUN ln -s /usr/bin/python3 /usr/bin/python

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

# Define default command
CMD ["/bin/bash"]
