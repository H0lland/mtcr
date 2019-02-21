#!/bin/bash
#usage: contr.sh start stop step numPer out
./clean.sh
num=$1
step=$3
str='outs1/out'
out=$5
qos=30
beta=50
servs=1000
#until [ $beta -gt 50 ]; do
#until [ $qos -gt 50 ]; do
until [ $num -gt $2 ]; do
	counter=0	
	until [ "${counter}" = "${4}" ]; do	
		./gen.py 4 $num $servs $qos "${str}${num}-${beta}-${counter}"
		./mtcr.py "${str}${num}-${beta}-${counter}" $beta
		./scrape.py "${str}${num}-${beta}-${counter}" ${out}
		let counter+=1 	
	done
	#let beta+=$step
	#let qos+=$step	
	let num+=$step
done
./average.py ${out}
