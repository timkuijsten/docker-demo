# Demonstration for Kerberos Realm Crossover

> *Kerberos can connect arbitrary realms across the Internet,
> based on setting up a shared key by sending and receiving
> a KX-OFFER message over a TLS-protected connection.  Realm
> and TLS certificates are validated through DNSSEC and DANE.*


TODO:

  * Two KDC processes on localhost, using different ports
  * Each KDC has its own KXOVER wrapper process, again on localhost
  * Demo realms are a hack: use DANE publications with fixed keys
  * Demo realms are local-only: _kerberos SRV points to localhost
  * TLS Pool has fixed private keys and matching certificates


## DNS configuration

DNS holds DNSSEC-secured configuration for two realms, namely:

```
PIXIE.DEMO.ARPA2.ORG
UNICORN.DEMO.ARPA2.ORG
```

Specifically,

```
shell$ dig +short TXT _kerberos.PIXIE.DEMO.ARPA2.ORG
"PIXIE.DEMO.ARPA2.ORG"

shell$ dig +short SRV _kerberos-tls._tcp.PIXIE.DEMO.ARPA2.ORG
10 10 99 kdc.pixie.arpa2.lab.
```

and

```
shell$ dig +short TXT _kerberos.UNICORN.DEMO.ARPA2.ORG
"UNICORN.DEMO.ARPA2.ORG"
shell$ dig +short SRV _kerberos-tls._tcp.UNICORN.DEMO.ARPA2.ORG
10 10 77 kdc.unicorn.arpa2.lab.
```

You should configure the name `localkdc.demo` to point
to a local IP address, on which you willl run the test software
components.  You can set it to `::1` or `127.0.0.1` in your
`/etc/hosts` or you can use the `test/demo-hosts` tool and supply
the host name to the scripts).  This should be the IP where you
run your test infrastructure on.

The test utilities all take in an explicit IP address and port
number, as this is normally supplied by the test utility
[pypeline](https://github.com/arpa2/kxover/blob/master/PYPELINE.MD).
INITIALLY_AT:
[pypeline](https://github.com/arpa2/kxover/blob/tls-based-attempt/PYPELINE.MD)


