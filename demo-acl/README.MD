# ARPA2 ACL demo

This is a demonstration of testing whether two A2IDs may communicate with each
other by using a policy defined in the file `demopolicy`.

## Create the demo image
```sh
docker build -t a2acl .
```

## Create a new container and run the demo
```sh
docker run -it a2acl bash
cd /root/arpa2
a2acl demopolicy order@ashop.example.com tim@dev.arpa2.org
a2acl demopolicy order@ashop.example.com tim+ashop@dev.arpa2.org
a2acl demopolicy some@one.com tim+asignature+@dev.arpa2.org
a2acl demopolicy some@one.com tim+nosig@dev.arpa2.org
a2acl demopolicy jane@somedomain.tk tim@dev.arpa2.org
a2acl demopolicy jane@somedomain.tk tim+asignature+@dev.arpa2.org
```

## a2acl usage
a2acl [-qv] policyfile remoteid localid

## a2acl manpage
man a2acl

## modify policy for other experiments
nano demopolicy
