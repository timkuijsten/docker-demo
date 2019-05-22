# ARPA2 ACL milter demo

This is a demonstration of the ARPA2 ACL milter using Postfix. Four mails are
sent and handled by a policy defined in the text file [demopolicy]:

1. A mail is sent from root@ashop.example.com to tim+ashop@example.net that is
whitelisted.

2. Another mail is sent from root@ashop.example.com to tim@example.net that is
greylisted.

3. A third mail is sent from root@milter.demo.arpa2.net to tim@example.net that
is blacklisted.

4. A last mail is sent from root@someone.tk to tim@example.net that
is abandoned.


## Pull and run the demo image
```sh
docker pull arpa2/demo-a2aclmilter
docker run -ti arpa2/demo-a2aclmilter bash

# Start logging and postfix
service rsyslog start && service postfix start

# Start a2aclmilter with policy file /demopolicy, as user id 2498 and
# chrooted to /etc/opt. Postfix communicates with the milter via
# tcp://127.0.0.1:7000
a2aclmilter /demopolicy 2498 /etc/opt inet:7000@127.0.0.1

# 1. Send a Whitelisted mail
echo hi | msmtp -v --host 127.0.0.1 --port 25 --from \
    root@ashop.example.com tim+ashop@example.net
# This should result in: 250 2.0.0 Ok: queued as XXXXXXXXXX
# Show the appended X-ARPA2-ACL: Whitelisted header:
postcat -hq XXXXXXXXXX

# 2. Send a Greylisted mail
echo hi | msmtp -v --host 127.0.0.1 --port 25 --from \
    root@ashop.example.com tim@example.net
# This should result in: 250 2.0.0 Ok: queued as XXXXXXXXXX
# Show the appended X-ARPA2-ACL: Greylisted header:
postcat -hq XXXXXXXXXX

# 3. Send a Blacklisted mail
echo hi | msmtp -v --host 127.0.0.1 --port 25 --from \
    root@milter.demo.arpa2.net tim@example.net
# This should result in: 550 5.7.1 Command rejected
# No message is queued.

# 4. Send an Abandoned mail
echo hi | msmtp -v --host 127.0.0.1 --port 25 --from \
    root@someone.tk tim@example.net
# This should result in: 250 2.0.0 Ok: queued as XXXXXXXXXX
# Although it sais a message is queued, it is in fact discarded:
postcat -hq XXXXXXXXXX
postcat: fatal: open queue file XXXXXXXXXX: No such file or directory
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
