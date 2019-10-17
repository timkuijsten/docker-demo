# ARPA2 Resource ACL demo
#
# In this container you can test the resource acl library.

FROM arpa2/base

RUN apt-get update && apt-get install -y \
	man \
	gcc \
	cmake \
	git \
	nvi \
	tmux

RUN mkdir ~/arpa2 && cd ~/arpa2 && \
	git clone https://gitlab.com/arpa2/libarpa2common.git && \
	cd libarpa2common && git checkout aclr && \
	mkdir build && cd build && \
	cmake .. && \
	make && \
	make install && \
	ldconfig

COPY demopolicy /root/arpa2
