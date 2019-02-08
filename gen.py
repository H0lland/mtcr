#!/usr/bin/env python3.7
#usage: ./gen.py cloudlets users services qos out
from random import randrange,random,uniform
import sys
def main():
	#reading in commandlines
	cloudlets = int(sys.argv[1])
	users = int(sys.argv[2])	
	servs = int(sys.argv[3])
	qosFactor = int(sys.argv[4])/10
	out = sys.argv[5]
	#set variables for constructing conns
	minUsers = 1
	maxUsers = 4 * minUsers
	unassigned = users
	conns = []
	'''
	cl = 0
	#while there are unassigned users
	while unassigned > 0:
		#randomly select number to connect to current cloudlet
		if maxUsers <= unassigned:
			connUs = randrange(1,maxUsers)
		elif unassigned == minUsers:
			connUs = minUsers
		else:
			connUs = randrange(1,unassigned)
		for i in range(connUs):
			conns.append(cl)
		cl += 1
		#wrap back around if there are still unassigned users
		if cl == cloudlets:
			cl = 0
		unassigned -= connUs
	'''
	#distribute users evenly
	for i in range(cloudlets):
		#divide users evenly among unvisited cloudlets
		conned = unassigned//(cloudlets-i)
		unassigned -= conned
		for j in range(conned):
			conns.append(i)
	conns.sort()
	
	degs = []
	minUsers = users//(2*cloudlets)
	maxUsers = 4*minUsers
	#create degredation values
	for i in range(cloudlets):
		thresh = randrange(minUsers,maxUsers)
		degs.append(thresh)

	#set variables for dists construction
	minLocalDist = 10
	maxLocalDist = 30
	maxGlobalDist = 100
	minCloudDist = 70
	dists = []
	#for each cloudlet
	for i in range(cloudlets):
		tmp = []
		#for each user
		for j in range(users):
			#check if user j connected to cloudlet i
			if conns[j] == i:
				elem = randrange(minLocalDist,maxLocalDist+1)
			#cloudlet not connected to user
			else:
				elem = randrange(maxLocalDist+1, maxGlobalDist)
			tmp.append(elem)
		dists.append(tmp)
	#get the distances from the user to the cloud
	tmp = []
	for i in range(users):
		elem = randrange(minCloudDist, maxGlobalDist)
		tmp.append(elem)
	dists.append(tmp)

	#set variables for service construction
	servLst = []
	minIn = 4
	maxIn = 8
	minOut = 2
	maxOut = 4
	minComps = 2
	maxComps = 7
	minPlace = 1
	maxPlace = 3
	minSched = 1
	maxSched = 3
	#construct servs
	for i in range(servs):
		tmp = []
		#append key, inSize, outSize, comps, placement cost, schedule cost
		tmp.append(i)
		tmp.append(randrange(minIn,maxIn))
		tmp.append(randrange(minOut,maxOut))
		tmp.append(randrange(minComps,maxComps))
		tmp.append(randrange(minPlace,maxPlace))
		tmp.append(randrange(minSched,maxSched))
		servLst.append(tmp)

	#set service variables for tasks construction
	#inSizes = [40,60,80,60,40]
	#outSizes = [9,11,15,10,8]
	#compTimes = [3,5,2,7,2]
	#the number of occurrences out of 1000 for the most common service
	#zipfBases = [437,656,801,911,1000]
	tasks = []
	for i in range(users):
		#tasks for user
		#numTasks = randrange(1,4)
		numTasks = 2 
		for j in range(numTasks):
			tmp = []
			tmp.append(i)
			#servType = uniform(1,servs)
			#servType = randServ
			servType = randrange(0,servs)
			tmp.append(servLst[servType][1])
			tmp.append(servLst[servType][2])
			tmp.append(servLst[servType][3])
			tmp.append(servType)
			tasks.append(tmp)

	#set variables for specs construction
	minStor = 2 * minPlace
	maxStor = 2 * minStor
	minBand = 5 * minIn
	maxBand = 4 * minBand
	minProcs = 2 * minComps
	maxProcs = 2 * minProcs
	specs = []
	for i in range(cloudlets):
		#determine importance of the cloudlet
		scale = random()
		#choose specs based on importance
		stor = minStor+round((maxStor-minStor)*scale)
		band = minBand+round((maxBand-minBand)*scale)
		procs = minProcs+round((maxProcs-minProcs)*scale)
		specs.append([stor,band,procs])
	'''specs.append([3,34,8])
	specs.append([2,15,4])
	specs.append([2,14,4])
	specs.append([3,28,7])
	'''
	specs.append([10000,10000,10000])

	#set variables for QoS construction
	qos = []
	#make a row for each user
	for i in range(users):
		tmp = []
		for j in range(len(tasks)):
			#if task is assigned to user i
			if tasks[j][0] == i:
				#append qosFactor times the computation time for qos
				tmp.append(qosFactor*tasks[j][3])
		qos.append(tmp)


	#open file for output
	file = open(out+".gcon","w")
	#write output to file
	#write specs
	for i in range(len(specs)):
		for j in range(len(specs[i])):
			file.write(str(specs[i][j])+",")
		file.write(";")
	file.write("\n")
	#write conns
	for i in range(len(conns)):
		file.write(str(conns[i]) + ",")
	file.write("\n")
	#write qos
	for i in range(len(qos)):
		for j in range(len(qos[i])):
			file.write(str(qos[i][j])+",")
		file.write(";")
	file.write("\n")
	#write dists
	for i in range(len(dists)):
		for j in range(len(dists[i])):
			file.write(str(dists[i][j]) + ",")
		file.write(";")
	file.write("\n")
	#write tasks
	for i in range(len(tasks)):
		for j in range(len(tasks[i])):
			file.write(str(tasks[i][j])+",")
		file.write(";")
	file.write("\n")
	#write servs
	for i in range(len(servLst)):
		for j in range(len(servLst[i])):
			file.write(str(servLst[i][j])+",")
		file.write(";")
	file.write("\n")
	file.close()
main()
