# build-kerberos constructs a KDC for the realm ARPA2.LAB
#
# From: Rick van Rein <rick@openfortress.nl>


FROM arpa2/build-bin

RUN apt-get install -y --no-install-recommends --assume-yes \
	shishi-kdc shisa

ENV REALM=SHISHI.ARPA2.LAB

RUN mv /etc/krb5.conf /root
COPY krb5.conf /etc/krb5.conf
COPY shishi.conf /etc/shishi.conf

RUN shisa --add SHISHI.ARPA2.LAB
RUN shisa --add SHISHI.ARPA2.LAB krbtgt/SHISHI.ARPA2.LAB
RUN shisa --add SHISHI.ARPA2.LAB demo  --password=sekreet
RUN shisa --add SHISHI.ARPA2.LAB demo1 --password=sekreet1
RUN shisa --add SHISHI.ARPA2.LAB demo2 --password=sekreet2
RUN shisa --add SHISHI.ARPA2.LAB demo3 --password=sekreet3

COPY README.MD /README.MD

#NOTHERE# RUN /etc/init.d/shishi-kdc start

CMD ["bash"]
