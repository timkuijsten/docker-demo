# ARPA2 ACL milter demo
#
# In this container you can test the milter with an ACL policy file.

FROM arpa2:base

RUN apt-get update && apt-get install -y \
	man \
	gcc \
	cmake \
	rsyslog \
	git \
	nvi \
	tmux

COPY .bashrc /root

RUN git clone https://github.com/arpa2/libarpa2service.git && \
	cd libarpa2service/build && \
	cmake .. && \
	make && \
	make install && \
	ldconfig

RUN echo "postfix postfix/main_mailer_type string 'Internet Site'" | debconf-set-selections && \
	echo "postfix postfix/mailname string milter.demo.arpa2.net" | debconf-set-selections && \
	apt-get install -y libmilter-dev postfix msmtp && \
	echo 'smtpd_milters = inet:127.0.0.1:7000' >> /etc/postfix/main.cf

RUN git clone https://github.com/arpa2/arpa2mail.git && \
	mkdir arpa2mail/a2aclmilter/build && \
	cd arpa2mail/a2aclmilter/build && \
	cmake .. && \
	make && \
	make install

COPY demopolicy /app
