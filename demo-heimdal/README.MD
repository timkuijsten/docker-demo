# Heimdal Kerberos5 Demonstration

> *This is very simple; just a KDC for HEIMDAL.ARPA2.LAB and a few
> simple accounts.  It serves as a basis on which to build
> other demonstrations.*

To start, run these commands:

    /etc/init.d/heimdal-kdc start

To obtain a ticket, run:

    kinit demo
    Password for demo@HEIMDAL.ARPA2.LAB: sekreet

You can now inspect your tickets with:

    klist

If you need to add or remove principals, run:

    kadmin -l -r $REALM ...

