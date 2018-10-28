* nano /usr/local/apache2/conf/httpd.conf
* uncomment #LoadModule tlspool_module modules/mod_tlspool.so
* /etc/init.d/rsyslog start
* tlspool -kc /etc/tlspool.conf
* export PATH=$PATH:/usr/local/apache2/bin
* LD_PRELOAD=/usr/local/lib/libtlspool.so apachectl start
* /usr/local/src/httpd/test/webdemo.py
