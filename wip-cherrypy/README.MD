# CherryPy demo, ARPA2 styled

This demonstrates a few ideas that we would like to see
in web services:

  * HTTPS security via a TLS Pool daemon
      - independent and central management of TLS is easier
      - detaching long-term credentials from application logic
   * Strong authentication
      - TLS-KDH ciphersuite: post-quantum technology
      - HTTP SASL: generic authentication support as anywhere
   * Bring Your Own IDentity
      - Identities from the client's realm, not website-local
      - Based on realm crossover technology
   * Efficient yet Flexible Authorisation
      - Access control lists for patterns of remotes
      - Authorised access to communication or resource (instance)
      - Central management in a domain's cockpit

## Running the Demo

First, start `syslog` service,

```
/etc/init.d/rsyslog start
```

Then, start the TLS Pool,

```
tlspool -kc /etc/tlspool.conf
```

Now start the test WSGI web server in the background,

```
cheroot-tlspool.py &
```

Finally, connect using OpenSSL,

```
openssl s_client -servername tlspool.arpa2.lab -connect 127.0.0.1:8070
```

It will connect, print a certificate -- but that's all for now.
There is no content in the test server yet.

It is worth noting that CherryPy is picky on correct line
termination, so you will have to send carriage returns before
line feeds.  You can type this on the shell by pressing `^V` `^M`
just before hitting enter.

After the handshake comes through, you can enter your query,
with `^M` denoting the extra carriage return, for instance:

```
GET / HTTP/1.0^M
Host: tlspool.arpa2.lab^M
^M
```

