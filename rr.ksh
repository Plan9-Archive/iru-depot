#!/bin/ksh
# round-robin load balancer

[[ -z $1 ]] && exit 1
[[ -z $2 ]] && STREAMS=2 || STREAMS=$2


balance_streams()
{
	rm filelist.*
	i=0

	for files in `find $1 -print | xargs ls -ld | awk '{print $5" "$9}' | sort -nr | awk '{print $2}'`; do 
	#	if [ ! -d $files ]; then
			echo $files
			echo $files >> filelist.${i}
	#		stream[$i]="${stream[$i]} $files"
			i=$((i+1))
			[ $i -eq $STREAMS ] && i=0
	#	fi
	done
}


print_sizes()
{
	i=0
	while [ $i -lt $STREAMS ]; do
		echo "Stream $i"

		for r in `du -s ${stream[$i]} | awk '{print $1}'`; do
			size[$i]=$((size[i]+r))
		done

		echo "total ${size[$i]}" 
		i=$((i+1))
	done
}

balance_streams $1
print_sizes
