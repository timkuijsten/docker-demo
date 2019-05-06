# ARPA2 ACL milter demo

This is a demonstration of the ARPA2 ACL milter using Postfix. Three mails are
sent and handled by a policy defined in the text file [demopolicy]:

1. A mail is sent from root@ashop.example.com to tim+ashop@example.net that is
whitelisted.

2. Another mail is sent from root@ashop.example.com to tim@example.net that is
greylisted.

3. A last mail is sent from root@milter.demo.arpa2.net to tim@example.net that
is blacklisted.


## Create the demo image
```sh
docker build -t a2aclmilter .
```

## Create a new container and run the demo
```sh
docker run -it a2aclmilter bash

# Start logging and postfix
service rsyslog start && service postfix start

# Start a2aclmilter with policy file /demopolicy, as user id 2498 and
# chrooted to /etc/opt. Postfix communicates with the milter via
# tcp://127.0.0.1:7000
a2aclmilter /demopolicy 2498 /etc/opt inet:7000@127.0.0.1

# 1. Send a Whitelisted mail
echo hi | msmtp -v --host 127.0.0.1 --port 25 --from \
root@ashop.example.com tim+ashop@example.net
# This should result in: 221 2.0.0 Bye

# 2. Send a Greylisted mail
echo hi | msmtp -v --host 127.0.0.1 --port 25 --from \
root@ashop.example.com tim@example.net
# This should result in: 451 4.7.1 Service unavailable - try again later

# 3. Send a Blacklisted mail
echo hi | msmtp -v --host 127.0.0.1 --port 25 --from \
root@milter.demo.arpa2.net tim@example.net
# This should result in: 550 5.7.1 Command rejected
```

## a2aclmilter usage
a2aclmilter [-dhqv] [-g group] acldb user chrootdir sockaddr

## a2aclmilter manpage
man a2aclmilter

## modify policy for other experiments
vi /demopolicy

## testing with a local repository
docker run -v /Users/tim/code/libarpa2service:/usr/local/src/libarpa2service -it a2aclmilter bash

[demopolicy]: ./demopolicy
