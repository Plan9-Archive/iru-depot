#!/bin/sh
for f in *.{c,scm,sh}; do
	ext=`echo $f | sed 's/^.*\.//'`
	echo $f $ext
	./get-comments.scm $ext $f
done
