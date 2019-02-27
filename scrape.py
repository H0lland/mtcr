#!/usr/bin/env python3.7
import sys
def main():
	#read from solution file
	inName = sys.argv[1]
	outName = sys.argv[2]
	inFile = open(inName+".sol","r")
	lines = inFile.readlines()
	inFile.close()
	apxFile = open(inName+".apx","r")
	apxLines = apxFile.readlines()
	apxFile.close()
	algCost = int(apxLines[0].split(":")[1])

	#count the number of schedule placements and the number placed on the cloud
	cloudCnt = 0
	schedCnt = 0
	cost = 0
	for i in range(len(lines)-1):
		#get objective value
		if i == 1:
			cost = int(lines[i].split("=")[1])
		line = lines[i].split()
		#check that this variable was set to 1
		if line[1] == "1":
			#get whether placement or schedule
			varType = line[0].split("[")[0]
			if varType == "scheduling":
				schedCnt += 1
				cloudlet = line[0].split(",")[1]
				#if on cloud
				if cloudlet[0] == "4":
					cloudCnt += 1

	#append data
	outFile = open(str(outName)+".csv","a+")	
	outFile.write(inName+","+str(cloudCnt)+","+str(schedCnt)+","+str(cost)+","+str(algCost)+"\n")
	outFile.close()

main()
