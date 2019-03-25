#!/bin/bash
#usage: contr.sh start stop step numPer out
./clean.sh
num=$1
step=$3
str='outs1/out'
out=$5
qos=15
beta=30
servs=1000
#until [ $beta -gt 80 ]; do
until [ $qos -gt 30 ]; do
#until [ $num -gt $2 ]; do
	counter=0
	echo -ne "${qos}\t"
	until [ "${counter}" = "${4}" ]; do	
		echo -ne "${counter} "
		./gen.py 4 $num $servs $qos $beta "${str}${num}-${qos}-${counter}"  &> trash.log
	./mtcrq.py "${str}${num}-${qos}-${counter}" &> trash.log
	mv "${str}${num}-${qos}-${counter}.sol" "${str}${num}-${qos}-${counter}.qsol"
	./mtcr.py "${str}${num}-${qos}-${counter}" &> trash.log
	
		
		if [ -e "${str}${num}-${qos}-${counter}.sol" ]
		then
			./main "${str}${num}-${qos}-${counter}" $beta &> trash.log
			./scrape.py "${str}${num}-${qos}-${counter}" ${out} &> trash.log	
		fi
		let counter+=1 	
	done
	echo -e ""
	#let beta+=$step
	let qos+=$step	
	#let num+=$step
done
./average.py ${out} #&> trash.log
rm trash.log
rm gurobi.log
