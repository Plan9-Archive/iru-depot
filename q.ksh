#!/bin/ksh
# draw a square
# Iruata Souza (muzgo)
# Public Domain

[ -z $1 ] && exit 1

l=$1
[ $l -gt 1 ] && l=$((l-1))
c=$(echo $2 | awk '{print substr($1,1,1)}')
: ${c:="x"}

i=0
while [ $i -le $l ]; do
	n=0
	
	if [ $i -eq 0 ] || [ $i -eq $l ]; then
		while [ $n -le $l ]; do
			echo -ne "$c"
			n=$((n+1))
		done
	else
		while [ $n -le $l ]; do
			[ $n -eq 0 ] || [ $n -eq $l ] && echo -ne "$c" || echo -ne " "
			n=$((n+1))
		done
	fi
	echo
	i=$((i+1))
done

