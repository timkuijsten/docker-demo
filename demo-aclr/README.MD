# ARPA2 ACL demo

This is a demonstration the resource acl library.

## Create the demo image
```sh
docker build -t a2acl .
```

## Create a new container and run the demo
```sh
docker run -it a2acl bash
cd /root/arpa2
a2aclr demopolicy example.com john@example.com 7a35d76d-a754-35a6-abe7-757c161f0263 /home/john
V
a2aclr demopolicy example.com john@example.com 7a35d76d-a754-35a6-abe7-757c161f0263 /var/empty
CRWKOV
a2aclr demopolicy example.com @example.com 7a35d76d-a754-35a6-abe7-757c161f0263 /home/john
V
a2aclr demopolicy example.com @example.com 7a35d76d-a754-35a6-abe7-757c161f0263 /var/empty

a2aclr demopolicy arpa2.org  foo+opt@arpa2.org 7a35d76d-a754-35a6-abe7-757c161f0263
CRWKO
```

## a2aclr usage
a2aclr [-pv] policyfile realm id resource [instance]

## modify policy for other experiments
nano demopolicy
