#!/usr/bin/env python
#
# A demonstration webserver, based on cheroot's WSGI demo
# with the SSL Adapter cheroot.ssl.tlspool
#
# From: Rick van Rein <rick@openfortress.nl>


from cheroot import wsgi
# from cheroot.server import HTTPServer
from cheroot.ssl.tlspooladapter import TLSPoolAdapter


def my_crazy_app(environ, start_response):
	status = '200 OK'
	response_headers = [('Content-type','text/plain')]
	start_response(status, response_headers)
	return [b'Hello world!\r\nThis is the TLS Pool variant of cheroot\r\n']


addr = '0.0.0.0', 8070
server = wsgi.Server(addr, my_crazy_app)
server.ssl_adapter = TLSPoolAdapter ()
server.prepare()
server.serve()

# adr = ('::', 8070)
# srv = HTTPServer (adr)
# srv.prepare ()
# srv.serve ()

