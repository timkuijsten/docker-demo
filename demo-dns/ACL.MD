# DNS Access Control Lists

> *Through AMQP, we allow arbitrary parties to change DNS data.
> Not all parties may write all records, however.  Usually, access
> is explicitly granted to owner names combined with a RRtype and
> an optional priority.  Higher-level features may be more accessible,
> because they can do their own dedicated filtering, even key-based.*


**Work In Progress:** This is an initial design, and serves to pin
down ideas, not a final design.  Feedback is welcomed.


## Resource Instance Keys for Resource Records

Resource records are usually accessible to a unique party, or when
a priority field is part of the definition, then at least one such
value may be assigned to a party.  Generally, the access is then
granted to a space-separated combination of the following two or three
words:

 1. *Owner Name:* fully qualified, lower case, no trailing dot.
 2. *Resource Record Type:* official name, not numeric, lower case,
    and replacing `a` by `aaaa`.
 3. *Priority:* only if applicable, shortest decimal representation

For example:

  * `_xmpp-client._tcp.example.com srv 10`
  * `www.example.com aaaa`
  * `example.com mx 50`

These are the resource instance keys that serve to lookup the desired
access, for a given resource UUID.  The UUID is defined by the following
name, with corresponding value:

```
String: default.acl.arpa2dns.arpa2.org
UUID:   TODO
```

The other parts of the DNS resource records must still be supplied:

  * *Class:* only access to class `in` for Internet.
  * *Time To Live:* supplied by the `arpa2dns` shell.
  * *Resource Data:* with the exception of priority, if it is supplied.


## Special-Purpose Access Control

Resource Records may be produced from special-purpose goals.
For those, `arpa2dns` offers higher-level processing primitives.
This allows more application-specific forms of filtering, and
may facilitate things that are not defined yet.

Strings end with `.x.acl.arpa2dns.arpa2.org` and are mapped to a UUID.

**Note:** It is currently undecided if we will need this facility.


## Key-Protected Access Control

Resource Records may be supplied by multiple parties, but then
end up competing, and possibly interfering.  It is important that
one party cannot remove records added by another, or vice versa.

A *half* solution to this might be to supply an authorisation key
for a future (reverse) action.  This allows one party to add, and
another to remove a record.

This is not completely safe, though.  Not even when the name of a
publishing party is included.  One party might see another coming,
and prepare with a temporary addition of the other's records, and
through that gain the key to remove the records when they are
later added by the actual party.  Since public records are not
published with an annotation of their origin, this will not help
to protect from rogue removal.

**Note:** It is currently undecided if we will need this facility.


## Host-Restricted Access Control

Several records hold a host name or domain name, that would
normally point into a service provider's realm of control.
To allow a provider to add and remove such records, there is
a final form of access control that constrains the referencing
host name.

This form has the following string and UUID values:

```
String: hostref.acl.arpa2dns.arpa2.org
UUID:   TODO
```

The contents are the same words as for `default.acl.arpa2dns.arpa2.org`,
but with one space and word added:

  * *Domain:* service domain, no host name, dot-separated, lower case.

