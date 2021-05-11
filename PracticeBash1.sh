#!/bin/bash
`gcc -Wall -o PracticeTest1.o PracticeTest1.c`
if test $# -lt 1
then
	echo "Usage scipt file1 file2"
else
	sum=0
	tmp=0
	for arg in $*
	do
		if test -f $arg
		then
			tmp=`./PracticeTest1.o $arg`
			#echo "$tmp"
			sum=`expr $sum + $tmp`
		fi
	done
	echo "$sum"
fi
