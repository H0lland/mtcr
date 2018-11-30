#!/usr/bin/env python3.7
import sys
def main():
	#read from solution file
	#inName = input("Sol file: ")
	inName = sys.argv[1]
	inFile = open(inName+".sol","r")
	lines = inFile.readlines()
	inFile.close()

	#count the number of schedule placements and the number placed on the cloud
	cloudCnt = 0
	schedCnt = 0
	for i in range(len(lines)):
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
	outFile = open("sol.cnt","a+")
	outFile.write(inName+":\t"+str(cloudCnt)+"/"+str(schedCnt)+"\n")
	outFile.close()

main()
