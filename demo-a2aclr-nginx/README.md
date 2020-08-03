# ARPA2 Resource ACL nginx demo

This is a demonstration of the ARPA2 Resource ACL module for nginx.

What follows is an example of a web application to manage the stock
of truck tyres. It has the imaginative name *Truck Tyre Stock* and
is running on the ficticious domain *trucktyres.example*.  This
application is used by a service provider named "Worldmaster" and
a tyre manufacturer named "Fineyear". Tyres enter stock whenever
Fineyear produced new tyres, and leave stock whenever Worldmaster
has mounted some tyres on a truck. The main two entities of this
system are *tyres* and *mountings*.

The web application offers a classic REST API with a standard CRUD
interface (create, read, update, delete) so that tyres and mountings
can be easily created, modified and deleted. The two main urls used
in this system are *http://trucktyres.example/api/tyres* and
*http://trucktyres.example/api/mountings*.

First we define an access right policy. We let anyone from Fineyear
create new tyres. Any employee from Worldmaster may read and update
tyres and will delete them once they're mounted on a truck.
Furthermore, anyone at Worldmaster may create, read and update
mountings, but only admin@worldmaster.example may delete a mounting.
This policy is stored in a text file in */root/[demopolicy]* inside the container:


```
# realm			selector		rights	resource				instance
trucktyres.example	@worldmaster.example	RWD	Truck-Tyre-Stock-XXXXXXXXXXXXXXXXXXX	/api/tyres
trucktyres.example	@worldmaster.example	CRW	Truck-Tyre-Stock-XXXXXXXXXXXXXXXXXXX	/api/mountings
trucktyres.example	admin@worldmaster.example	CRWD	Truck-Tyre-Stock-XXXXXXXXXXXXXXXXXXX	/api/mountings
trucktyres.example	@fineyear.example	C	Truck-Tyre-Stock-XXXXXXXXXXXXXXXXXXX	/api/tyres
```

Note that *C*, *R*, *W* and *D* map to create, read, update and
delete, respectively. *Truck-Tyre-Stock-XXXXXXXXXXXXXXXXXXX* is a
pseudo UUID picked for this web application but can be any randomly
generated UUID. Each field is separated by a TAB character.  The
exact syntax of a policy file, as well as all possible access rights
are documented in [a2aclr(3)].

Next we show the nginx configuration file that applies this policy
on the aforementioned urls.

```sh
load_module modules/ngx_http_auth_a2aclr_module.so;

events {
	worker_connections  1024;
}

http {
	auth_a2aclr_db "/root/demopolicy";

	server {
		listen       80;
		server_name  trucktyres.example;
		root         /srv/empty;

		auth_a2aclr_realm $server_name;

		# UUID for the application we're serving, this can be any
		# randomly generated (pseudo) UUID.
		auth_a2aclr_class "Truck-Tyre-Stock-XXXXXXXXXXXXXXXXXXX";

		location ~ /api/(?<collection>[a-z]+)/ {
			auth_a2aclr_instance /api/$collection;
			proxy_pass http://127.0.0.1:8080;
		}
	}
}
```

Lets create and instantiate it and fire some requests at nginx.

## Create the demo image and run a container

```sh
docker build -t a2aclrnginx .
docker run --add-host 'trucktyres.example:127.0.0.1' -it a2aclrnginx bash
```

Now from within the container start nginx and a simple [CRUD
application] that takes and stores JSON objects, and serves them
on request. The nginx configuration is stored in `/etc/nginx/nginx.conf` and
is configured to use policy file `/root/demopolicy`.

```sh
nginx
crudapp /srv/trucktyres.example &
serving /srv/trucktyres.example/ on 127.0.0.1:8080
```

First we impersonate as Sean, an employee of Fineyear, and try to
put a new tyre in stock that has just rolled out of the factory:

```sh
curl \
    -u sean@fineyear.example: \
    -H 'Content-Type: application/json' \
    -X POST \
    -d '@-' \
    http://trucktyres.example/api/tyres <<'EOF'
{
  "serial_number":   "6012021194",
  "service_provider": "Worldmaster",
  "brand": "Fineyear",
  "size":  "385/65R22.5",
  "type":  "KMAX T"
}
EOF
```

The above request should succeed and return a 201 Created the first
time it is run. The nginx log which is stored in */var/log/nginx/error.log*
contains the details:

```
looking up trucktyres.example C in /root/demopolicy, client: XXX, server: trucktyres.example, request: "POST /api/tyres HTTP/1.1", host: "XXX"
W: PERMITTED, client: XXX, server: trucktyres.example, request: "POST /api/tyres HTTP/1.1", host: "XXX"
```

Then we try to get the tyre that we just created, which should fail with a 403
because sean@fineyear.example only has the create permission ("C") and not the
read permission ("R").

```sh
curl -u sean@fineyear.example: http://trucktyres.example/api/tyres/demotyre
<html>
<head><title>403 Forbidden</title></head>
<body>
<center><h1>403 Forbidden</h1></center>
<hr><center>nginx/1.19.1</center>
</body>
</html>
```

Let's retry as brian@worldmaster.example, which does have the "R" permission. This should return a 200 with the document.

```sh
curl -u brian@worldmaster.example: http://trucktyres.example/api/tyres/demotyre
{"serial_number":"6012021194","service_provider":"Worldmaster","brand":"Fineyear","size":"385/65R22.5","type":"KMAX T"}
```

Note that the nginx configuration does not have basic auth enabled
which means anyone can impersonate as anyone.

## modify policy for other experiments
vi /root/demopolicy

[demopolicy]: ./demopolicy
[a2aclr(3)]: https://netsend.nl/arpa2/a2aclr.3.html
[CRUD application]: ./demo-a2aclr-nginx/crudapp
