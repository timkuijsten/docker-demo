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

RUN apt-get install -y krb5-user openldap-utils python2.7 ldapvi nano lrzsz

RUN ln -s /usr/bin/python2.7 /usr/bin/python

# Add files relative to this Dockerfile
# ADD index.html /var/www/index.html

# Set environment variables
# ENV HOME /var/www

# Define working directory
# WORK /var/www

# Define default command
CMD ["bash"]
