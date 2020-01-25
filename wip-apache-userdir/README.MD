# README for the ARPA2 Apache Modules

> *These modules link the Apache Web Server to a number of ARPA2 projects.*

## Starting ##
```
$ docker run --rm --name apache -p 8000:80 -it build-apachemod bash
```
## Inside docker ##
```
root@9b0050a13502:/# export PATH=$PATH:/usr/local/apache2/bin
root@9b0050a13502:/# apachectl start
```

## On PC ##

Access control (REMOVED FROM THIS DEMO):

```
$ curl -L -u hfmanson@localhost:henri1234 http://localhost:8000/secret/
<html><body><h1>Secret!</h1></body></html>
```

Distinction through the `User:` header:

```
$ curl -L -H 'User: john' http://localhost:8000
<html>
<head><title>John's web space</title></head>
<body>
<h1>John's web space</h1>
</body>
</html>
```

...versus...

```
$ curl -L -H 'User: mary' http://localhost:8000
<html>
<head><title>Mary's web space</title></head>
<body>
<h1>Mary's web space</h1>
</body>
</html>
```

**Note:**
A variant is also available.  Edit `/usr/local/apache2/conf/extra/httpd-userdir.conf` and switch to the `UserDir "/var/arpa2/reservoir/uid=*"` line.  This allows retrieval with the following sequence:

```
curl -L -H 'User: rick' http://localhost:8000

curl -L -H 'User: rick' http://localhost:8000/Negrin%20-%20Whispering/

curl -L -H 'User: rick' -o /tmp/sonata.mp3 http://localhost:8000/Negrin%20-%20Whispering/01%20-%20Negrin%20-%20Sonata%20in%20A%20Minor%20for%20Guitar%20and%20%20Harpsichord%20_Allegro_.mp3
```

You can now play the file, `/tmp/sonata.mp3`, with your favourite music player.  Creative Commons!

## Inside docker ##
```
root@9b0050a13502:/# tail -n 3 /usr/local/apache2/logs/error_log 
[Wed Jan 08 20:58:20.205190 2020] [aclr:notice] [pid 11:tid 140307310507776] AH03297: x_check_authz(): user = hfmanson@localhost, uri = /secret/, realm = myrealm, ap_auth_type = Basic, method = GET
[Wed Jan 08 20:58:20.205251 2020] [aclr:notice] [pid 11:tid 140307310507776] AH03297: looking up R in /usr/local/apache2/conf/aclrpolicy
[Wed Jan 08 20:58:20.205286 2020] [aclr:notice] [pid 11:tid 140307310507776] AH03297: R: PERMITTED
```
