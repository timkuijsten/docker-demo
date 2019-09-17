# README for the ARPA2 Apache Modules

> *These modules link the Apache Web Server to a number of ARPA2 projects.*

```
/etc/init.d/rsyslog start
tlspool-daemon -kc /etc/tlspool.conf
export PATH=$PATH:/usr/local/apache2/bin
LD_PRELOAD=/usr/lib/libtlspool.so apachectl start
/usr/local/src/apache-httpd.git/test/webdemo.py
```
