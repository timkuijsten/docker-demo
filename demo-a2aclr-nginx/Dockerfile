# ARPA2 Resource ACL nginx demo
#
# In this container you can test nginx with a resource acl policy

FROM arpa2/build-bin

RUN apt-get update && apt-get install -y curl libpcre3-dev zlib1g-dev nodejs

COPY README.md /README.md

COPY .bashrc /root
COPY .tmux.conf /root

RUN cd /root && \
	git clone https://gitlab.com/arpa2/libarpa2common.git && \
	cd libarpa2common && \
	git checkout v0.7.1 && \
	mkdir build && \
	cd build && \
	cmake .. && \
	make && \
	make install && \
	ldconfig

RUN cd /root && \
	git clone https://gitlab.com/arpa2/nginx-auth-a2aclr.git && \
	curl -O https://nginx.org/download/nginx-1.19.1.tar.gz && \
	tar zxf nginx-1.19.1.tar.gz

RUN cd /root && \
	cd nginx-1.19.1 && \
	./configure \
	--with-debug \
	--sbin-path=/usr/local/sbin/nginx \
	--conf-path=/etc/nginx/nginx.conf \
	--http-log-path=/var/log/nginx/access.log \
	--error-log-path=/var/log/nginx/error.log \
	--lock-path=/var/lock/nginx.lock \
	--pid-path=/run/nginx.pid \
	--with-cc-opt="-O0 -I/root/libarpa2common/src -I/root/nginx-auth-a2aclr/" \
	--add-dynamic-module="/root/nginx-auth-a2aclr" && \
	make && \
	make install

RUN mkdir -p /srv/trucktyres.example/tyres && \
	mkdir -p /srv/empty && \
	touch /var/log/nginx/error.log && \
	touch /var/log/nginx/access.log

COPY demopolicy /root/
COPY nginx.conf /etc/nginx/
COPY crudapp /usr/local/bin

# dev:
# docker run -v /Users/tim/code/nginx-auth-a2aclr/:/root/nginx-auth-a2aclr/: --add-host 'trucktyres.example:127.0.0.1' -it a2aclrnginx bash
# tmux a
# tail -f /var/log/nginx/error.log
# crudapp /srv/trucktyres.example/
# make modules && make install; pkill nginx; nginx
# curl -v -usean@fineyear.com: -H'Content-Type: application/json' http://127.0.0.1:80/api/tyres/2389
