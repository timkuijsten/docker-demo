# FireFox with SASL plugin
#
# This makes use of Native Messaging, a method for browser extensions
# to be tightly coupled to a desktop application over a simple serial
# line.  We use it to extend browser authentication with SASL.
#
# From: Henri Manson <info@mansoft.nl>


FROM arpa2/base

RUN apt-get update && \
	apt-get install -y firefox-esr openssh-server \
		libsasl2-2 libsasl2-dev libsasl2-modules \
		libsasl2-modules-gssapi-mit maven git openjdk-11-jdk

RUN mkdir /var/run/sshd

# Install the SASL plugin, which is a proof-of-concept in Java with a simplistic user/pass GUI
RUN mkdir ~/arpa2 && cd ~/arpa2 && git clone https://github.com/arpa2/http-sasl-plugin && cd http-sasl-plugin/app && mvn package -Dmaven.wagon.http.retryHandler.count=10 -DskipTests

# Setup the native messaging plugin; you will need to set it up yourself
RUN mkdir -p  ~/.mozilla/native-messaging-hosts && cat ~/arpa2/http-sasl-plugin/app/sasl.json | sed 's/\/home\/manson/\/root/' > ~/.mozilla/native-messaging-hosts/sasl.json

# If your X-Windows does not have GLX (like Xquartz) you should leave this:
#RUN sed -i 's/^java /java -Dprism.order=j2d /' ~/arpa2/http-sasl-plugin/app/sasl.sh

# Use X11UseLocalhost no
RUN sed -i 's/^#X11UseLocalhost yes/X11UseLocalhost no/' /etc/ssh/sshd_config

# Replace this file with your own: ssh-add -L > authorized_keys
COPY authorized_keys /root/.ssh/

# SSH will output on the console; you might run with -dit to not see it
CMD ["/usr/sbin/sshd", "-D"]
