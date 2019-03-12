# ShiShi Kerberos5 Demonstration

> *This is very simple; just a KDC for SHISHI.ARPA2.LAB and a few
> simple accounts.  It serves as a basis on which to build
> other demonstrations.*

To start, run these commands:

    /etc/init.d/shishi-kdc start

To obtain a ticket, run:

    kinit demo
    Password for demo@HEIMDAL.ARPA2.LAB: sekreet

This, however, is not yet working:

```
root@00020a0b8db7:/# kinit demo@SHISHI.ARPA2.LAB
shishid: Read 180 bytes from 127.0.0.1 on socket 4

shishid: Processing 180 bytes on socket 4
shishid: AS-REQ from demo@SHISHI.ARPA2.LAB for krbtgt/SHISHI.ARPA2.LAB@SHISHI.ARPA2.LAB failed: no such server
shishid: AS-REQ failed (43): Principal name syntax error.
shishid: AS-REQ failing with KRB-ERROR: Server not found in database
shishid: Generated 62 bytes response for socket 4
shishid: Sending 62 bytes to 127.0.0.1 socket 4 via UDP
```

The problem is the **no such server** message, which is
immediate so presumably based on `/etc/hosts`, but even
if that points `SHISHI.ARPA2.LAB` to the local host, it
fails.

You can now inspect your tickets with:

    klist

If you need to add or remove principals, run:

    shisa --add/... $REALM $USER --password=$PASS



