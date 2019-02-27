#!/bin/bash
#usage: contr.sh start stop step numPer out
./clean.sh
num=$1
step=$3
str='outs1/out'
out=$5
qos=25
beta=30
servs=1000
#until [ $beta -gt 50 ]; do
#until [ $qos -gt 50 ]; do
until [ $num -gt $2 ]; do
	counter=0
	echo -ne "${num}\t"
	until [ "${counter}" = "${4}" ]; do	
		echo -ne "${counter} "
		./gen.py 4 $num $servs $qos $beta "${str}${num}-${beta}-${counter}"
		./mtcr.py "${str}${num}-${beta}-${counter}" 
		
		if [ -e "${str}${num}-${beta}-${counter}.sol" ]
		then
			./main "${str}${num}-${beta}-${counter}" $beta
			./scrape.py "${str}${num}-${beta}-${counter}" ${out} #&> trash.log	
		fi
		let counter+=1 	
	done
	echo -e ""
	#let beta+=$step
	#let qos+=$step	
	let num+=$step
done
./average.py ${out} &> trash.log
rm trash.log
rm gurobi.log
