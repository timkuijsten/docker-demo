#!/usr/bin/env python
#
# A demonstration webserver, based on cheroot's WSGI demo
# with the TLSPoolAdapter from cheroot.ssl.tlspooladapter
#
# From: Rick van Rein <rick@openfortress.nl>


import syslog

from cheroot import wsgi
from cheroot.ssl.tlspooladapter import TLSPoolAdapter


def my_crazy_app(environ, start_response):
	syslog.syslog (syslog.LOG_INFO, 'Request received in my_crazy_app')
	status = '200 OK'
	response_headers = [('Content-type','text/plain')]
	start_response(status, response_headers)
	return [b'Hello world!\r\nThis is the TLS Pool variant of cheroot\r\n']


addr = '0.0.0.0', 8070
server = wsgi.Server(addr, my_crazy_app, server_name='tlspool.arpa2.lab')
server.ssl_adapter = TLSPoolAdapter ('tlspool.arpa2.lab')
server.start()

