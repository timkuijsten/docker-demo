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
```
$ curl -L -u hfmanson@localhost:henri1234 http://localhost:8000/secret/
<html><body><h1>Secret!</h1></body></html>
```
## Inside docker ##
```
root@9b0050a13502:/# tail -n 3 /usr/local/apache2/logs/error_log 
[Wed Jan 08 20:58:20.205190 2020] [aclr:notice] [pid 11:tid 140307310507776] AH03297: x_check_authz(): user = hfmanson@localhost, uri = /secret/, realm = myrealm, ap_auth_type = Basic, method = GET
[Wed Jan 08 20:58:20.205251 2020] [aclr:notice] [pid 11:tid 140307310507776] AH03297: looking up R in /usr/local/apache2/conf/aclrpolicy
[Wed Jan 08 20:58:20.205286 2020] [aclr:notice] [pid 11:tid 140307310507776] AH03297: R: PERMITTED
```
