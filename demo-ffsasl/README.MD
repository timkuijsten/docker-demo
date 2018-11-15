# FireFox with HTTP SASL

> *This is a proof of concept where FireFox talks to a native desktop application
> that runs SASL.  Though minimalistic, it's just DIGEST-MD5 used here, this is a
> demonstration of a generally usable principle.*

This is a proof-of-concept:

  * HTTP SASL is a [draft specification](https://tools.ietf.org/html/draft-vanrein-httpauth-sasl)
    that compensates the most urgent need in HTTP with proper authentication.
  * The SASL protocol is commonly used: IMAP, POP3, SMTP, LDAP, AMQP, and many more already use it.
  * SASL is extensible *without* updating the surrounding protocol (like HTTP in this case).
  * SASL authentication is done *at the HTTP level* so applications just need to tap credentials.

## Get it going

Before making the demo, please set your SSH public key in the `authorized_keys` file which would
be installed in the `root` account of the image.  Then build.

```
git clone https://github.com/arpa2/docker-demo
cd docker-demo
cmake .
ssh-add -L > demo-ffsasl/authorized_keys
make demo-ffsasl
```

Then, deploy it in the background with

```
docker run -p 2002:22 -dit --name ff0 demo-ffsasl
```

You can now connect to it, forwarding your session's X-Window session:

```
echo $DISPLAY   # must exist
ssh -X -p 2002 root@localhost
echo $DISPLAY   # must exist
firefox
```

Once started, you can enter the following URL of a demonstration server,

```
http://mansoft.nl:8080/HttpSasl/SaslServlet
```

You should see a page that presents a 401 error.  That's because you failed
to authenticate; you did not load the plugin yet.  This must be done
explicitly by the user (which is strength, given the power unleashed).

Open another tab (`^T`) and enter

```
about:debugging
```

You will see a button `[Load Temporary Add-On]` -- click it.  Then, browse to
`Home`, `arpa2`, `http-sasl-plugin` and select `manifest.json` and click the
`[Open]` button on the bottom.  You should now see `sasl-credentials` listed
under Temporary Extensions.

Go back to the tab that produced the 401 error, and reload.  You should now
get a popup.
**This popup is an external program, communicating with the browser extension that side-tracks HTTP SASL traffic to the program.**
Type the following into the two fields:

```
Username: henri
Password: 1234
```

You should be rewarded with a page with actual content, something along the
lines of

```
charset=utf-8,username="henri",realm="test-realm.nl",
nonce="QHMn5QC5YK0gupDRWvLje+/NQlFDT0L87EgruAls",nc=00000001,
cnonce="Y2qMvprgtFAj45PXQH4XfxTFASmF+nIfdcCisqAs",
digest-uri="http/test-realm.nl",
maxbuf=65536,response=5cb96e49f465cc94692bf02bedb46392,qop=auth,
authzid="henri"
```

It's a clunky way of saying *Welcome* to user `henri`.  He can now
do anything on this server (which means he gets to reload the page).


## Further Exploration

You can now use the Web Console or Web Inspector to see more of the
exchange.  You will be happy to see that not the details above are
relayed, because that would mean that choices of SASL mechanism had
been baked into the HTTP SASL exchange.  Instead, you would see

  * `mech` with one or more SASL mechanism names;
  * `c2s` with data traveling from client to server;
  * `s2c` with data traveling from server to client;
  * `s2s` for server data retained in the communication;
  * ...and possibly a few more, details of which are in the spec.


## Conclusion

From this demonstration it should be pretty obvious that HTTP SASL can
be implemented on FireFox right now, and since it relies on the general
idea of [native messaging](https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/Native_messaging)
from a [browser extension](https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions),
this is expected to soon arrive on other platforms too.

Also note that what we are doing here is not shocking to a browser.
It intercepts HTTP headers (much like extensions that alter the `User-Agent`)
and communicates with an external program (much like a password manager)
and these are all things expected to stay.  They are also highly potent,
which means that their installation will continue to be an explicit action
taken by the user.  This is proper protection, and worth the effort.

