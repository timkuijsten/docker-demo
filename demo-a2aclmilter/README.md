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


## Create the demo image and run a container
```sh
docker build -t a2aclmilter .
docker run -it a2aclmilter bash
```

```sh
# Start logging, postfix and the milter.
# The milter uses policy file /demopolicy, as user id 2498 and is chrooted into
# /etc/opt. Postfix communicates with the milter via tcp://127.0.0.1:7000.
service rsyslog start && service postfix start &&
    a2aclmilter /demopolicy 2498 /etc/opt inet:7000@127.0.0.1

# 1. Send a Whitelisted mail.
# This should result in: 250 2.0.0 Ok: queued as XXXXXXXXXX
echo test1white | msmtp -v --host 127.0.0.1 --port 25 --from \
    root@ashop.example.com tim+ashop@example.net

# Watch the appended X-ARPA2-ACL: Whitelisted header in the last mail
cat /var/mail/tim

# 2. Send a Greylisted mail.
# This should result in: 250 2.0.0 Ok: queued as XXXXXXXXXX
echo test2grey | msmtp -v --host 127.0.0.1 --port 25 --from \
    root@ashop.example.com tim@example.net

# Watch the appended X-ARPA2-ACL: Greylisted header in the last mail
cat /var/mail/tim

# 3. Send a Blacklisted mail
# This should result in: 550 5.7.1 Command rejected
echo test3black | msmtp -v --host 127.0.0.1 --port 25 --from \
    root@milter.demo.arpa2.net tim@example.net

# No new message is queued and the spool should only contain the messsages from
# the previous tests.
cat /var/mail/tim

# 4. Send an Abandoned mail.
# This should result in: 250 2.0.0 Ok: queued as XXXXXXXXXX
echo test4abandon | msmtp -v --host 127.0.0.1 --port 25 --from \
    root@someone.tk tim@example.net

# Although the daemon says a message is queued, it is in fact discarded. Again,
# no new message is queued and the spool should only contain the messsages from
# the previous tests.
cat /var/mail/tim
```

## a2aclmilter usage
a2aclmilter [-dhqv] [-g group] acldb user chrootdir sockaddr

## a2aclmilter manpage
man a2aclmilter

## modify policy for other experiments
vi /demopolicy

[demopolicy]: ./demopolicy
