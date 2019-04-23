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
	vals.append(["num","edge%","cloud%", "optimal", "Galgorithm","Lalgorithm"])
	curr = ''
	sched = 1
	cloud = 1
	opt = 0
	galg = 0
	lalg = 0
	ct = 1
	#count averages for each unique user/qos setup
	for i in range(len(lines)):
		lineLst = lines[i].split(",")
		pos = lineLst[0].rfind('-')
		name = lineLst[0][:pos]

		#same name found
		if name == curr:
			cloud += int(lineLst[1])
			sched += int(lineLst[2])
			opt += float(lineLst[3])
			galg += float(lineLst[4])
			lalg += float(lineLst[5])
			ct += 1

		#else, total averages and start over
		else:
			#total and append
			cloudPerc = cloud/sched
			edgePerc = 1 - cloudPerc
			opt /= ct
			galg /= ct
			lalg /= ct
			vals.append([curr,edgePerc,cloudPerc, opt, galg,lalg])
			#reset
			curr = name
			cloud = int(lineLst[1])
			sched = int(lineLst[2])
			opt = float(lineLst[3])
			galg = float(lineLst[4])
			lalg = float(lineLst[5])
			ct = 1

	vals.append([curr,edgePerc,cloudPerc,opt/ct,galg/ct,lalg/ct])
	#write the data
	outFile = open(inName+"_avg.csv","w")
	for i in range(len(vals)):
		outFile.write(str(vals[i][0])+","+str(vals[i][1])+","+str(vals[i][2])+","+str(vals[i][3])+","+str(vals[i][4])+","+str(vals[i][5])+"\n")
	outFile.close()
main()
