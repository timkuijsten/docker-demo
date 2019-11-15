# ARPA2 ACL milter demo
#
# In this container you can test the milter with an ACL policy file.

FROM arpa2/build-bin

COPY README.md /A2ACLMILTER.md

COPY .bashrc /root

RUN apt-get install -y liblmdb-dev

RUN cd /usr/local/src && \
	git clone https://gitlab.com/arpa2/libarpa2common.git && \
	cd libarpa2common && git checkout v0.7.0 && \
	mkdir build && cd build && \
	cmake .. && \
	make && \
	make install && \
	ldconfig

RUN echo "postfix postfix/main_mailer_type string 'Internet Site'" | debconf-set-selections && \
	echo "postfix postfix/mailname string example.net" | debconf-set-selections && \
	apt-get install -y libmilter-dev postfix msmtp && \
	echo 'smtpd_milters = inet:127.0.0.1:7000' >> /etc/postfix/main.cf && \
	useradd tim

RUN cd /usr/local/src && \
	git clone https://gitlab.com/arpa2/arpa2mail.git && \
	cd arpa2mail && git checkout v0.6.0 && \
	mkdir a2aclmilter/build && \
	cd a2aclmilter/build && \
	cmake .. && \
	make && \
	make install

COPY demopolicy /
