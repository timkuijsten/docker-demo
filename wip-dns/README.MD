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
tlsa config pkix-end pubkey sha512
tlsa add orvelte.nep _443 tcp web 666999aaabbbcccdddeeefff
zone del orvelte.nep
exit
```

## Current and Future Work

This DNS demonstration takes in zone changes and publishes
them with DNSSEC signatures on everything.  For now, there
is one master.  Future versions will also support multiple
masters that sign independently, with only the DNSSEC keys
as shared state.  Yes, DNSSEC can do that :-)


