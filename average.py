#!/usr/bin/env python3.7
import sys
def main():
	#read in file name
	inName = sys.argv[1]
	inFile = open(inName+".csv","r")
	lines = inFile.readlines()
	inFile.close()

	#initialize
	vals = []
	vals.append(["num","edge%","cloud%"])
	curr = ''
	sched = 1
	cloud = 1

	#count averages for each unique user/qos setup
	for i in range(len(lines)):
		lineLst = lines[i].split(",")
		pos = lineLst[0].rfind('-')
		name = lineLst[0][:pos]
		
		#same name found
		if name == curr:
			cloud += int(lineLst[1])
			sched += int(lineLst[2])

		#else, total averages and start over
		else:
			#total and append
			cloudPerc = cloud/sched
			edgePerc = 1 - cloudPerc
			vals.append([curr,edgePerc,cloudPerc])
			#reset
			curr = name
			cloud = int(lineLst[1])
			sched = int(lineLst[2])
	vals.append([curr,edgePerc,cloudPerc])	
	#write the data
	outFile = open(inName+"_avg.csv","w")
	for i in range(len(vals)):
		outFile.write(str(vals[i][0])+","+str(vals[i][1])+","+str(vals[i][2])+"\n")
	outFile.close()
main()
