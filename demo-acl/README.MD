# ARPA2 ACL demo

This container contains two demos, one for the communication acl and one for the
resource acl.

### Create the demo image and run a container
```sh
docker build -t a2acl .
docker run -it a2acl bash
```

## Communication ACL

This is a demonstration of testing whether two A2IDs may communicate with each
other by using a policy defined in the file `aclcpolicy`.

### Run the demo in the container
```sh
# cd /root/arpa2
# a2aclc aclcpolicy order@ashop.example.com tim@dev.arpa2.org
B
# a2aclc aclcpolicy order@ashop.example.com tim+ashop@dev.arpa2.org
W
# a2aclc aclcpolicy some@one.com tim+asignature+@dev.arpa2.org
B
# a2aclc aclcpolicy some@one.com tim+nosig@dev.arpa2.org
B
# a2aclc aclcpolicy jane@somedomain.tk tim@dev.arpa2.org
A
# a2aclc aclcpolicy jane@somedomain.tk tim+asignature+@dev.arpa2.org
A
```

### a2aclc(1) usage and manpage
a2aclc -h
man a2aclc

### modify policy for other experiments
vi aclcpolicy


## Resource ACL

This is a demonstration of the resource acl library that shows the rights a user
under a certain realm has over a resource UUID and resource instance by using a
policy defined in the file `aclrpolicy`.

### Run the demo in the container
```sh
# cd /root/arpa2
# a2aclr -i aclrpolicy example.com john@example.com 7a35d76d-a754-35a6-abe7-757c161f0263 /home/john
newly imported ACL rules 3
V
# a2aclr -i aclrpolicy example.com john@example.com 7a35d76d-a754-35a6-abe7-757c161f0263 /var/empty
newly imported ACL rules 3
CRWKOV
# a2aclr -i aclrpolicy example.com @example.com 7a35d76d-a754-35a6-abe7-757c161f0263 /home/john
newly imported ACL rules 3
V
# a2aclr -i aclrpolicy example.com @example.com 7a35d76d-a754-35a6-abe7-757c161f0263 /var/empty
newly imported ACL rules 3

# a2aclr -i aclrpolicy arpa2.org foo+opt@arpa2.org 7a35d76d-a754-35a6-abe7-757c161f0263
newly imported ACL rules 3
CRWKO
```

## a2aclr(1) usage and manpage
a2aclr -h
man a2aclr

## modify policy for other experiments
vi aclrpolicy
