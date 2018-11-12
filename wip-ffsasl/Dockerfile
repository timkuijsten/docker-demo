# FireFox with SASL plugin
#
# This makes use of Native Messaging, a method for browser extensions
# to be tightly coupled to a desktop application over a simple serial
# line.  We use it to extend browser authentication with SASL.
#
# From: Rick & Henri


FROM arpa2:base

RUN apt-get update && \
	apt-get install -y firefox-esr openssh-server \
		libsasl2-2 libsasl2-dev libsasl2-modules \
		libsasl2-modules-gssapi-mit


RUN /etc/init.d/ssh start && /etc/init.d/ssh stop

EXPOSE 22

CMD ["/usr/sbin/sshd", "-d"]
