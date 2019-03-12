# ShiShi Kerberos5 Demonstration

> *This is very simple; just a KDC for SHISHI.ARPA2.LAB and a few
> simple accounts.  It serves as a basis on which to build
> other demonstrations.*

To start, run these commands:

    /etc/init.d/shishi-kdc start

To obtain a ticket, run:

    kinit demo
    Password for demo@SHISHI.ARPA2.LAB: sekreet

You can now inspect your tickets with:

    klist

If you need to add or remove principals, run:

    shisa --add/... $REALM $USER --password=$PASS

An interesting thing to note is that `shisa` is used
to add *everything*, even the realm and its TGS ticket,

    shisa --add $REALM
    shisa --add $REALM krbtgt/$REALM

After this, user accounts can be added that could then
request the TGS ticket.

**Note:** We have installed `krb5.conf` not because
ShiShi needs it; it exists to service the MIT krb5
client programs, such as `kinit`, `klist`, `kdestroy`.
