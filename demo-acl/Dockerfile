# ARPA2 ACL demo
#
# In this container you can test a communication pair with an ACL policy file.

FROM arpa2/base

RUN apt-get update && apt-get install -y \
	man \
	gcc \
	cmake \
	git \
	nvi \
	tmux

RUN mkdir ~/arpa2 && cd ~/arpa2 && \
	git clone https://github.com/arpa2/libarpa2service.git && \
	cd libarpa2service/build && \
	cmake .. && \
	make && \
	make install && \
	ldconfig

COPY demopolicy /root/arpa2
