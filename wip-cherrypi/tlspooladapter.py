"""Implementation of the SSL Adapter for the TLS Pool."""


from cheroot.ssl import Adapter

import tlspool


class TLSPoolAdapter (Adapter):

    """The TLS Pool is a separate daemon implementing TLS in a
       separate process, so as to keep long-term credentials
       and the management of TLS away from application logic.
       This is perfect for a dynamic, pluggable environment
       that might integrate scripts from a variety of mildly
       unknown sources.  It is generally good to contain the
       problems resulting from an application breach.
    """

    def __init__ (self, *ignored):
        """Initialise this object and ignore the customary
           things: cert, key, chain, ciphers are all handled
           by the TLS Pool, so we can be blissfully ignorant.
        """
        pass

    def bind (self, sock):
        """Wrap and return the socket.
           TODO: Wrapping is not done here, as in Builtin?!?
        """
        return super (TLSPoolAdapter,self).bind (sock)

    def wrap (self, extsock):
        """Wrap the given socket in TLS and return the result,
           along with WSGI environment variables in a tuple.
        """
	fl = ( tlspool.PIOF_STARTTLS_LOCALROLE_SERVER |
	       tlspool.PIOF_STARTTLS_REMOTEROLE_CLIENT |
               tlspool.PIOF_STARTTLS_IGNORE_REMOTEID )
	hdl = tlspool.Connection (extsock, service='http', flags=fl)
	hdl.tlsdata.localid = 'tlspool.arpa2.lab'
	intsock = hdl.starttls ()
        env = {
                'wsgi.url_scheme': 'https',
                'HTTPS': 'on',
                'LOCAL_USER': hdl.tlsdata.localid,
                'REMOTE_USER': hdl.tlsdata.remoteid,
        }
        return intsock, env

    def makefile (self, sock, mode='r', bufsize=-1):
        """Turn the socket into a file object.
        """
        return sock.makefile (mode=mode, bufsize=bufsize)

    def get_environ (self, sock):
        """Return WSGI variables to be merged into each request.
        """
        return {
                'wsgi.url_scheme': 'https',
                'HTTPS': 'on',
        }

