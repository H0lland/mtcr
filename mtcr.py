#!/usr/bin/env python3.7
from gurobipy import *
import sys
#Data
#read in from scenario file
#inFile = input("Config name: ")
inFile = sys.argv[1]
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
    qos[i] = list(map(int,qos[i]))

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

#other constants
services = 5
storageCosts = [1,1,1,1,1]
placementCosts = [1,1,1,1,1]
schedulingCosts = [1,1,1,1,1]

#create the model
model = Model('MTCR Optimization')
#create placement matrix
placing = model.addVars(services,len(dists),vtype = GRB.BINARY, name = "placing")

#create scheduling matrix
schedule = model.addVars(len(tasks),len(dists),vtype = GRB.BINARY, name = "scheduling")

#cloudlet spec constraints
#storage
for j in range(0,len(specs)):
	sumM = 0
	for m in range(0,len(storageCosts)):
		sumM += storageCosts[m]*placing[m,j]
	model.addConstr(sumM <= specs[j][0],"Storage" + str(j))

#processing
for j in range(0,len(specs)):
	sumT = 0
	for t in range(0,len(tasks)):
		sumT += tasks[t][3]*schedule[t,j]
	model.addConstr(sumT <= specs[j][2],"Processing" + str(j))

#Completion constraints
model.addConstr(schedule.sum() == len(tasks),"All tasks")

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

#qos constraints
for t in range(0,len(tasks)):
		user = tasks[t][0]
		taskNum = t % 2
		#for each cloudlet
		for k in range(0,len(specs)):
			upTime = .0001 * tasks[t][1] * dists[k][user]
			downTime = .0001 * tasks[t][2] * dists[k][user]
			procTime = tasks[t][3]
			tot = upTime + float(procTime) + downTime
			model.addConstr(tot*schedule[t,j] <= qos[user][taskNum], "QoS Constraint")

#objective function
obj = model.getObjective() 


#add placement terms
for m in range(services):
    for j in range(0,len(specs)):
        obj.add(placing[m,j],placementCosts[m])

#add scheduling terms
for j in range(0,len(specs)):
    for t in range(0,len(tasks)):
        ty = tasks[t][4]
        obj.add(schedule[t,j],schedulingCosts[ty])

#set objective and optimize
model.setObjective(obj, GRB.MINIMIZE)
model.optimize()

#print variables
for v in model.getVars():
		if v.X != 0:
			print(v.Varname, v.X)
fName = inFile.split('.')[0]
model.write(fName + ".sol")
