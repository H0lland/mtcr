#!/bin/bash
#usage: contr.sh start stop step numPer
echo "${1}"
echo "${2}"
echo "${3}"
echo "${4}"
num=$1
step=$3
str='outs/out'
until [ $num -gt $2 ]; do
	counter=0
	echo $num	
	until [ "${counter}" = "${4}" ]; do
		./gen.py 4 $num "${str}${num}-${counter}"
		./mtcr.py "${str}${num}-${counter}"
		./scrape.py "${str}${num}-${counter}"
		let counter+=1 
	done
	let num+=$step
done
