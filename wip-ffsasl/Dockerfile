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
		libsasl2-modules-gssapi-mit maven git openjdk-8-jdk openjfx

COPY authorized_keys /root/.ssh/

RUN mkdir ~/arpa2 && cd ~/arpa2 && git clone https://github.com/arpa2/http-sasl-plugin && cd http-sasl-plugin/app && mvn package -DskipTests

RUN mkdir -p  ~/.mozilla/native-messaging-hosts && cat ~/arpa2/http-sasl-plugin/app/sasl.json | sed 's/\/home\/manson/\/root/' > ~/.mozilla/native-messaging-hosts/sasl.json

RUN mkdir /var/run/sshd
CMD ["/usr/sbin/sshd", "-D"]
