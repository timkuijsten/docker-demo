# Kerberos Demonstration with MIT krb5

> *This is very simple; just a KDC for MITKRB5.ARPA2.LAB and a few
> simple accounts.  It serves as a basis on which to build
> other demonstrations.*

To start, run these commands:

    /etc/init.d/krb5-kdc start
    /etc/init.d/krb5-admin-server start

To obtain a ticket, run:

    kinit demo
    Password for demo@MITKRB5.ARPA2.LAB: sekreet

You can now inspect your tickets with:

    klist

If you need to add or remove principals, run:

    kadmin.local

