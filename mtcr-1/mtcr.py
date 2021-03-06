#!/usr/bin/env python3.7
#usage: ./mtcr.py fileName penalty
from gurobipy import *
from random import randrange
import sys
#Data
#read in from scenario file
#inFile = input("Config name: ")
inFile = sys.argv[1]
#beta = int(sys.argv[2])/10
penalty = float(sys.argv[2])
file = open(inFile+".gcon","r")
lines = file.readlines()

#parse cloudlet specs
specs = lines[0][:-2].split(";")
for i in range(len(specs)):
    specs[i] = specs[i][:-1].split(",")
    specs[i] = list(map(int,specs[i]))

#parse user-cloudlet connections
conns = lines[1][:-2].split(",")
conns = list(map(int,conns))

#parse user qos constraints
qos = lines[2][:-2].split(";")
for i in range(len(qos)):
    qos[i] = qos[i][:-1].split(",")
    qos[i] = list(map(float,qos[i]))

#parse user-cloudlet distances
dists = lines[3][:-2].split(";")
for i in range(len(dists)):
    dists[i] = dists[i][:-1].split(",")
    dists[i] = list(map(int,dists[i]))

#parse task data
tasks = lines[4][:-2].split(";")
for i in range(len(tasks)):
    tasks[i] = tasks[i][:-1].split(",")
    tasks[i] = list(map(int,tasks[i]))

#parse servs data
servs = lines[5][:-2].split(";")
for i in range(len(servs)):
    servs[i] = servs[i][:-1].split(",")
    servs[i] = list(map(int,servs[i]))

#parse scheds data
schedulingCosts = lines[6][:-2].split(";")
for i in range(len(schedulingCosts)):
	schedulingCosts[i] = schedulingCosts[i][:-1].split(",")
	schedulingCosts[i] = list(map(int,schedulingCosts[i]))

print(len(schedulingCosts))
#construct storage and placement costs
#storageCosts = [2,2,2,2,2]
placementCosts = []
cloudSched = []
edgeSched = []
for i in range(len(specs)-1):
	edgeSched.append([])
for i in range(len(servs)):
	placementCosts.append(servs[i][4])
'''	cloudSched.append(servs[i][5])
	for j in range(len(specs)-1):
		#edgeSched[j].append(randrange(1,beta)*servs[i][5])
		#edgeSched[j].append(beta*servs[i][5])


#for each edge
#schedulingCosts = edgeSched.copy()
#schedulingCosts.append(cloudSched.copy())
'''
#create the model
model = Model('MTCR Optimization')

#create placement matrix
placing = model.addVars(len(servs),len(dists),vtype = GRB.BINARY, name = "placing")

#create scheduling matrix
schedule = model.addVars(len(tasks),len(dists),vtype = GRB.BINARY, name = "scheduling")

#cloudlet spec constraints
#storage
for j in range(0,len(specs)):
	sumM = 0
	for m in range(0,len(placementCosts)):
		sumM += placementCosts[m]*placing[m,j]
	model.addConstr(sumM <= specs[j][0],"Storage" + str(j))

#processing
for j in range(0,len(specs)):
	sumT = 0
	for t in range(0,len(tasks)):
		sumT += tasks[t][3]*schedule[t,j]
	model.addConstr(sumT <= specs[j][2],"Processing" + str(j))
'''
#bandwidth
for j in range(0,len(specs)):
	sumE = 0
	for t in range(0,len(tasks)):
		user = tasks[t][0]
		#in and out sizes
		inSize = tasks[t][1]
		outSize = tasks[t][2]
		#assume locality
		local = 1
		remote = 0
		localCl = conns[user]
		#if task is non-local
		if(conns[user] != j):
			#swap values
			local = 0
			remote = 1
		sumE += (local * inSize + remote * outSize) * .001 * dists[j][localCl] * schedule[t,j]
	model.addConstr(sumE <= float(specs[j][1]), "Bandwidth" + str(j))
'''
#uniqueness constraints
for t in range(0,len(tasks)):
	sumCom = 0
	for j in range(0,len(specs)):
		sumCom += schedule[t,j]
	model.addConstr(sumCom == 1, "Uniqueness")

#service existance constraint
for j in range(0,len(specs)):
	for t in range(0,len(tasks)):
		ty = tasks[t][4]
		model.addConstr(schedule[t,j] <= placing[ty,j],"Service Existance")

#objective function
obj = model.getObjective()

#add placement terms
for m in range(len(servs)):
    for j in range(0,len(specs)):
        obj.add(placing[m,j],placementCosts[m])

#add scheduling terms
for j in range(0,len(specs)):
    for t in range(0,len(tasks)):
        ty = tasks[t][4]
        obj.add(schedule[t,j],schedulingCosts[ty][j])

#qos terms
currUser = tasks[0][0]
taskNum = -1
for t in range(0,len(tasks)):
		user = tasks[t][0]
		if user == currUser:
			taskNum += 1
		else:
			taskNum = 0
			currUser = user
		localCl = conns[user]
		#for each cloudlet
		for k in range(0,len(specs)):
			upTime = tasks[t][1] * .001 * dists[k][localCl]
			downTime = tasks[t][2] * .001 * dists[k][localCl]
			procTime = tasks[t][3]
			tot = upTime + float(procTime) + downTime
			diff = tot-float(qos[user][taskNum])
			if diff > 0:
				print(t,k,diff)
			obj.add(schedule[t,k],max(0,diff*penalty))

'''#qos terms
currUser = tasks[0][0]
taskNum = -1
for t in range(0,len(tasks)):
		user = tasks[t][0]
		if user == currUser:
			taskNum += 1
		else:
			taskNum = 0
			currUser = user
		localCl = conns[user]
		#for each cloudlet
		for k in range(0,len(specs)):
			upTime = tasks[t][1] * .01 * dists[k][localCl]
			downTime = tasks[t][2] * .01 * dists[k][localCl]
			procTime = tasks[t][3]
			tot = upTime + float(procTime) + downTime
			diff = float(qos[user][taskNum])-tot
			obj.add(schedule[t,j],diff)

#add communication terms
for j in range(0,len(specs)):
	for t in range(0,len(tasks)):
		user = tasks[t][0]
		localCl = conns[user]
		remote = 1
		if j == localCl:
			remote = 0
		#if not local, add cost of sending packets (normalized to be similar to the schedule/placement costs
		sent = remote * (tasks[t][1] + tasks[t][2])//4
		obj.add(schedule[t,j],sent)
'''

#set objective and optimize
model.setObjective(obj, GRB.MINIMIZE)
model.optimize()

#print variables
for v in model.getVars():
		if v.X != 0:
			print(v.Varname, v.X)
fName = inFile.split('.')[0]
model.write(fName + ".sol")
