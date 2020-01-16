FROM busybox:musl
RUN /bin/busybox --install -s /bin
ADD http://tmp.vanrein.org/reservoir-demo-diskstore.tgz /root
RUN mkdir -p /var/arpa2/reservoir && \
	cd /var/arpa2/reservoir && \
	tar -xzvf /root/reservoir-demo-diskstore.tgz
RUN ln -s /var/arpa2/reservoir/README.MD /ReservoirFiles.MD
CMD ["/bin/ash"]
