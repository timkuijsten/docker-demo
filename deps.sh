#!/bin/bash

cd $(dirname $0)

echo "digraph arpa2demos {"
echo "   \"debian:stable\";"
for DIRDOCK in */Dockerfile
do
	DIR=${DIRDOCK%/Dockerfile}
	STYLE=''
	case $DIR in
	base)
		STYLE="${STYLE:+$STYLE }fillcolor=white"
		;;
	build-*)
		STYLE="${STYLE:+$STYLE }fillcolor=green"
		;;
	demo-*)
		STYLE="${STYLE:+$STYLE }fillcolor=yellow"
		;;
	wip-*)
		STYLE="${STYLE:+$STYLE }fillcolor=red color=gray"
		;;
	*)
	esac
	TARGET="arpa2:$DIR"
	echo "   \"$TARGET\";"
	for SOURCE1 in $( grep -i ^'FROM\>' $DIRDOCK | sed 's/^[Ff][Rr][Oo][Mm] *//' )
	do
		SOURCE2=$( echo $SOURCE1 | sed 's/  *AS .*$//' )
		case $SOURCE2 in
		*:*)
			STYLE=''
			if [ "$SOURCE1" != "$SOURCE2" ]
			then
				STYLE="${STYLE:+$STYLE }style=dotted"
			fi
			echo "   \"$SOURCE2\" -> \"$TARGET\" ${STYLE:+[$STYLE]};"
			;;
		*)
			;;
		esac
	done
done
echo '}'
