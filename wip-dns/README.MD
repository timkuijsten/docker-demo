# README for ARPA2 DNS

> *This image contains a master DNS node.  We generally send
> all DNS inquiries to multiple masters and let them work
> in parallel, with the exception of key management operations
> which must be co-ordinated so the signers produce exchangeable
> signatures.*

You first need to start Knot DNS through

```
/etc/init.d/knot start
```

Next, you can start the DNS shell,

```
arpa2dns
```

In the shell, you can do things like (more to come)

```
zone add orvelte.nep ns1.orvelte.nep. admin.orvelte.nep.
dane config pkix-end pubkey sha512
dane add orvelte.nep _tcp 443 web 666999aaabbbcccdddeeefff
acme add orvelte.nep www 1122334455
zone del orvelte.nep
exit
```

If you hadn't deleted the zone, you could have queried it
with commands like

```
kdig @localhost _443._tcp.web.orvelte.nep tlsa
kdig @localhost _acme-challenge.www.orvelte.nep txt
```

But alas, you had to go and delete that zone.  That ought
to teach you, following up orders without thinking about
their consequences ;-)

**ENUM support.**
There is some special support for ENUM.  Any place that
receive a zone can also process +ddddd form numbers,
which will be mapped to the ENUM form d.d.d.d.d.e164.arpa
before working with it:

```
zone add +12345 ns1.example.com admin@example.com
enum add +12345          1 E2U+xmpp     xmpp:1\\1@jabber.nep
enum add +12345 wildcard 1 E2U+xmpp xmpp:1\\1+\\2@jabber.nep
enum del +12345          1 E2U+xmpp     xmpp:1\\1@jabber.nep
enum del +12345 wildcard 1 E2U+xmpp xmpp:1\\1+\\2@jabber.nep
zone del +12345
```

The form without `wildcard` simply adds an NAPTR to reference
the service field and URI for the indicated number; the form
with `wildcard` adds an NAPTR for all numbers with additional
digits, like `+123456789`.  The `\\1` variable expands to
the matched domain minus the skip (here set to 1) and the
`\\2` variable expands to the wildcarded part.

You could query for

```
kdig @localhost         5.4.3.2.1.e164.arpa naptr
kdig @localhost 9.8.7.6.5.4.3.2.1.e164.arpa naptr
```

## Current and Future Work

This DNS demonstration takes in zone changes and publishes
them with DNSSEC signatures on everything.  For now, there
is one master.  Future versions will also support multiple
masters that sign independently, with only the DNSSEC keys
as shared state.  Yes, DNSSEC can do that :-)


