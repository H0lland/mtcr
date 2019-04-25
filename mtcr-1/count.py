#!/usr/bin/env python3.7
import sys
def main():
	inName = sys.argv[1]
	gapxFile = open(inName+".gapx","r")
	gapxLines = gapxFile.readlines()
	gapxFile.close()
	lapxFile = open(inName+".lapx","r")
	lapxLines = lapxFile.readlines()
	lapxFile.close()
	gTot = 0
	lTot = 0
	for i in range(3,len(gapxLines),3):
		gTot += gapxLines[i].count(';')
		lTot += lapxLines[i].count(';')
	print(gTot, lTot)		
main()
