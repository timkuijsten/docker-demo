# ARPA2 KeyMaster

> *The KeyMaster is the central management unit of keys and
> certificates in the IdentityHub.*

The tasks of the KeyMaster center around its LDAP repository,
which is used internally but of which parts can be made
available externally.  Its contents include certificates that
may be released to those who ask for them and private key
references that may be released to those who own them.

Keys may be managed elsewhere, and certificates are spread
to other places too.  This is done with shells in various
places.  For example, a web server using a TLS Pool for HTTPS
would have a locally defined HSM with private keys, and a
database with local identities that holds the currently
active certificate.

The KeyMaster coordinates generation and destruction of
keys and certificates, as well as their times of deployment.
It also takes care of central tasks such as interaction
with ACME for getting keys signed, and with DNS to add
them as DANE confirmations.


**BIG BAD BOLD WARNING:**
The current release may use OIDs that are temporary,
and that may change without notice.  You are welcome
to try the ideas (and report back on oversightings)
but generally the OIDs starting with
`1.3.6.1.4.1.44469.666`
indicate
`ARPA2-Experimental`
and MUST NOT be used in production releases, and using
them in your code and products means that you accept that
they MUST be replaced once final versions come into
existence.  It is up to you to see that your applications
are sufficiently covered by changes in requirements.
The OIDs are owned by ARPA2, and dictated by it --
not that we intend to abuse that, we merely want to
be free to clean up temporary OIDs at will.


## Keys and Certificates in LDAP

The LDAP structure is rooted under

```
ou=KeyMaster,o=arpa2.net,ou=InternetWide
```

Under this, each domain name has an `associatedDomain=`
RDN to rigidly separate the stored data according to
their realms of control.

The fifth level differs for users and hosts.  Their
respective DN forms representing key ownership are

```
uid=,associatedDomain=,ou=KeyMaster,...
dc=,associatedDomain=,ou=KeyMaster,...
```

Note that the `uid=` form applies to everything that
is assigned such a value in the IdentityHub; this
includes aliases, groups and so on.

Note that the `dc=` form may include `dc=@` and although
it is not considered proper LDAP, the code all supports a
hack that represents multiple labels in one `dc=` field.
Nesting of `dc=,dc=` is not supported.  Normally, it
is expected that one level suffices, and wards off
problems with unused intermediate levels and recursion.

Under these identifiers, private and secret keys as
well as public-key based credentials are stored.


## Private Keys and Secret Keys

Private keys *and* secret keys are both represented
with a `pkcs11PrivateKeyObject` that has at least a
`pkcs11PrivateKeyURI` URI according to the syntax
in [RFC 7512](https://tools.ietf.org/html/rfc7512)
so the `pkcs11:...` form.

These objects may have an auxiliary object attached
to indicate their location; for `pkcs11PrivateKeyLocal`
this is the `pkcs11LocalProviderModule` attribute and
for `pkcs11PrivateKeyRemote` these are the
`pkcs11RemoteServiceProtocol` and `pkcs11RemoteServiceLocator`
attributes.  Since the ARPA2 KeyMaster controls the
keys held elsewhere, the latter is always used.

Private keys may be held, but replicated by another.
This may be used to support signature validation and
decryption with keys that are no longer used for
signing or as encryption targets.  An attribute
`pkcs11DeprecatedBy` points to the newer and better
private key object throught its DN or RDN.  It is
an error to allow cycles in these.


## Certificates and other Public Credentials

There are various LDAP definitions for X.509 certificates,
OpenPGP public keys, OpenSSH public keys, Kerberos
credentials and perhaps SRP verifiers.  In general,
the idea is that the `objectClass` defines these forms
and we usually have no need to define our own.

The general attribute `pkcs11Certificate` is used as a
reference to each of these, and takes the form of a DN;
alternatively, an attribute `pkcs11CertificateURI` is
usable under `labeledURI` syntax, so a URI with a
possible space and comment.  Note that the name is once
again suggestive of a particular use, namely X.509
certificates, but other forms are equally usable.

Any number of these attributes can be attached to
the `pkcs11PrivateKeyObject` to be able to iterate
the public forms attached to the private forms.
The opposite is not done, but may be implemented
through an efficient index on the `pkcs11Certificate`
attribute.


### Optimised for Search

A common problem with PKCS \#11 implementation is
that their key search is a linear procedure, partly
due to the structure of the API itself.  This can
lead to noticeable delays in any application that
uses as few as a few thousands keys.

Private and public keys are usually linked through
a matching `CKA_ID` which is a practice that can be
replicated in `pkcs11:` URIs.  This allows us to
use the much faster and much more scalable search
mechanisms in LDAP for public-key searches, and only
use PKCS \#11 for private-key access.

LDAP also tends to be a more usable form for
applications.  That does not mean that the search
facilities could not be ported into a PKCS \#11
interface along with the actually stored private
key objects.


## Life Cycle Management

Keys come and go, and their management is not trivial.
This is especially true when their life cycle involves
such things as ACME and DANE.  Doing the right things
in the right order is vitally important to avoid ever
going down due to, say, a stray entry in a DNS cache
somewhere.

The task of life cycle management comes down to the
gradual progress that keys and public credentials
go through when they are created and destroyed.
This is all about timing, even more than for dry wit.
The Key Master centralises this kind of knowledge,
including the variations that may exist for the
different kinds of public forms.  This is why it is
the Key Master that, well, is the master of keys.

Life cycles are attached to private key objects
as well as public credential objects as an
auxiliary class `managedObject` with any number
of `managedState` attributes.  The form of
these attributes is a number of space-separated
words; the intention being that these start with
an identifier for a kind of treatment, and grow
with procedural updates related to the treatment.
Example identifiers are `dane`, `acme` and
`inuse`.  Identifiers SHALL NOT just append to
other identifiers, to maintain an accurate result
from something like `(managedState=acme*)` in an
LDAP search filter.

The additional words are of one of three forms;
either `name=value` to assign attributes or,
as a special form, `name@timestamp` to setup
for a timestamp that would trigger a step in
the treatment procedure.  The third form is
`identifier?name` to indicate waiting for the
event `name@timestamp` in `managedState: identifier*`
if it exists, or to skip otherwise.

The reason that a special format is used for
timestamped values and their impact elsewhere
is that this allows generic treatment of
deferred actions.  For instance, to wait until
a TTL in DNS has expired.  Timestamps for this
format take the form of a UNIX timestamp, so
the number of seconds since the midnight that
started Januari 1st, 1970, in UTC time.  It is
written in decimal form and is therefore not
restricted to any number of bits.

During steps of the treatment, other attribute
values than the `managedState` may be modified,
as part of the treatment in progress.  Note that
the replacement of at least the `managedState`
automatically implies a unique change-maker, as
a result of LDAP's atomic object changes.

We now continue to write out the various forms
of life cycle that are supported by the
Key Master.

All life cycles are informed when one of them
fires.  It is indicated which fires, and at
what timestamp.  This is how one life cycle
can influence another.  When `dane` is used,
the `cred` life cycle needs to await some of
it, for example.  The idea is that this kind
of logic is localised into the various life
cycle handlers.

### Public Credential Life Cycles

    cred req@TS cert@TS pub@TS old@TS gone@TS

Public credentials are recognised by the identifier
`cred` and they generally go through a number of stages:

  * `req` marks a certification request;
  * `cert` marks the desired certification;
  * `pub` marks suitability for public use;
  * `old` marks deprecation (or "choice of last resort") for public use;
  * `gone` marks retirement (or "no further use allowed") from public use.

During retirement, historic data may still call for the
public credential, for instance to decrypt or to validate
signatures.

The flow is most visible for X.509 certificates, but there
is no reason why OpenPGP and OpenSSH keys could not go
through a similar flow, possibly taking multiple steps at
some transitions.

### The ACME Life Cycle

    acme ask@TS dns=xxxx show@TS got@TS clean@TS

ACME is a standardised exchange for getting certificates
signed.  It involves a challenge for which we place a
requested `TXT` record value in DNS.  The identifier is
`acme` and the stages are:

  * `ask` marks the first exchange in ACME, where a
    certificate request is uploaded and a challenge is
    sent back;
  * `dns` provides the text to show in a `TXT` record
    in DNS;
  * `show` marks the publication of the ACME-requested
    action, in our case the `TXT` record in DNS;
  * `got` marks the retrieval of a signed certificate;
  * `clean` marks having undone the temporary action,
    in our case the removal of the `TXT` record from
    DNS.

### The DANE Life Cycle

    dane form=x,y,z add@TS serve@TS del@TS

DANE supports the validation of certificates via TLSA records
in DNS, under protection of DNSSEC.  It MUST only be initiated
on LDAP objects whose port and protocol are known.

  * `form` details the three configuration parameters for the
    record; see the `arpa2dns` shell documentation for
    details;
  * `add` marks when the `TLSA` record can be assumed
    to be known in all DNS caches;
  * `serve` marks the start of use in a server;
  * `del` marks the latest removal of the `TLSA` record
    from DNS.

**Caveat:** Reuse of a public key may lead to idempotent
additions to DNS being cancelled by a single deletion.
When re-using public keys, be sure to set `del@TS` at the
time of re-use, without actually removing the `TLSA` record.

