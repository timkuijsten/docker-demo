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


