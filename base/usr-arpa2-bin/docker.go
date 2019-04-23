#!/bin/bash

mkdir -p /var/arpa2

uptime -s >> /var/arpa2/docker.boots

BOOTS=`wc -l < /var/arpa2/docker.boots`
if [ $BOOTS -eq 1 ]
then

	sed -i /etc/bash.bashrc -e 's/^PS1=/# PS1=/'

	if [ -x /usr/arpa2/bin/docker.install ]
	then
		# echo 'First boot, running install script'
		/usr/arpa1/bin/docker.install
	fi
fi

NAME="docker"
if [ -r /var/arpa2/docker.hostname ]
then
	NAME=`cat /var/arpa2/docker.hostname`
	hostname "$NAME"
fi

if [ -r /var/arpa2/docker.name ]
then
	NAME=`cat /var/arpa2/docker.name`
fi

if [ "`tty`" == "not a tty" ]
then
	if [ -x /usr/arpa2/bin/docker.daemon ]
	then
		# echo 'Jumping into docker.daemon program'
		exec /usr/arpa2/bin/docker.daemon
	fi
	# echo 'Looping forever'
	echo 'Staying alive, you can use ^C but not ^Z'
	while true ; do sleep 86400 ; done
else

	uptime -s >> /tmp/docker.runs

	RUNS=`wc -l < /tmp/docker.runs`
	if [ $RUNS -eq 1 ]
	then
		if [ -r /var/arpa2/docker.hostname ]
		then
			HOST=`cat /var/arpa2/docker.hostname`
			# echo "Additional host name(s) $HOST"
			sed -i /etc/hosts -e '$s/$/ '"$HOST"'/'
		fi
	fi

	export PS1="$NAME$RUNS\\$ "']0;'$NAME${BOOTS# }''
	# echo "Setting shell prompt to $PS1 and window title to $NAME${BOOTS# }"
	echo -n ']0;'$NAME${BOOTS# }''

	if [ -x /usr/arpa2/bin/docker.interact ]
	then
		# echo 'Jumping into interactive program'
		exec /usr/arpa2/bin/docker.interact ]
	else
		# echo 'Jumping into interactive bash'
		exec /bin/bash
	fi
fi
echo 'End of docker.go'
exit
