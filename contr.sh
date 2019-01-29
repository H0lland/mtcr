#!/bin/bash
#usage: contr.sh start stop step numPer out
num=$1
step=$3
str='outs0/out'
out=$5
qos=30
beta=4
#until [ $beta -gt 10 ]; do
until [ $num -gt $2 ]; do
	counter=0	
	until [ "${counter}" = "${4}" ]; do	
		./gen.py 4 $num "${str}${num}-${beta}-${counter}" $qos
		./mtcr.py "${str}${num}-${beta}-${counter}" $beta
		./scrape.py "${str}${num}-${beta}-${counter}" ${out}
		let counter+=1 	
	done
	#let "beta++"
	#let "qos--"	
	let num+=$step
done
./average.py ${out}
