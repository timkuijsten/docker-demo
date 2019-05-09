# build-kerberos constructs a KDC for the realm ARPA2.LAB
#
# From: Rick van Rein <rick@openfortress.nl>


FROM arpa2/build-bin

RUN apt-get install -y --no-install-recommends --assume-yes \
	libkrb5-26-heimdal heimdal-kcm heimdal-kdc heimdal-clients

ENV REALM=HEIMDAL.ARPA2.LAB

RUN mv /etc/krb5.conf /root
COPY krb5.conf /etc/krb5.conf

RUN kstash --random-key
RUN kadmin -l init --realm-max-ticket-life=8h --realm-max-renewable-life=24h $REALM
RUN kadmin -l -r $REALM add --use-defaults --password=sekreet  demo
RUN kadmin -l -r $REALM add --use-defaults --password=sekreet1 demo1
RUN kadmin -l -r $REALM add --use-defaults --password=sekreet2 demo2
RUN kadmin -l -r $REALM add --use-defaults --password=sekreet3 demo3

#NOTHERE# RUN /etc/init.d/heimdal-kdc start

CMD ["bash"]
