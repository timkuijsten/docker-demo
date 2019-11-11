# ARPA2 Communication and Resource ACL demo
#
# In this container you can test both the communication and the resource acl
# with simple policy examples.

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
	cd libarpa2common && git checkout v0.7.0 && \
	mkdir build && cd build && \
	cmake .. && \
	make && \
	make install && \
	ldconfig

COPY aclcpolicy aclrpolicy /root/arpa2/
