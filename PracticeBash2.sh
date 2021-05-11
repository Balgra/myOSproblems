#!/bin/bash
if test $# -lt 1
then
	echo "Usage scipt nr1 nr2 .. nrn"
else
	sum=0
	contor=0
	for arg in $*
	do
		sum=`expr $sum + $arg`
		(( counter++ ))
	done
	avg=`expr $sum/$counter`
	echo "$avg"
fi