#!/usr/bin/env python3.7
from gurobipy import *

#Data
#read in from scenario file
file = open("out.txt","r")
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
placing = model.addVars(services,len(dists),vtype = GRB.BINARY)

#create scheduling matrix
schedule = model.addVars(len(tasks),len(dists),vtype = GRB.BINARY)
#include cloudlet spec constraints
#storage
for j in range(0,len(specs)):
	sumM = 0
	for m in range(0,len(storageCosts)):
		sumM += storageCosts[m]*placing[m,j]
	model.addConstr(sumM <= specs[j][0],"Storage" + str(j))
#model.addConstrs(for j in range(0,len(specs):for m in range(0,len(storageCosts)):quicksum(storageCosts[m]*placing[m,j]) <= specs[j][0] ,"Storage")
#processing
for j in range(0,len(specs)):
	sumT = 0
	for t in range(0,len(tasks)):
		sumT += tasks[t][3]*schedule[t,j]
	model.addConstr(sumT <= specs[j][1],"Processing" + str(j))

#Completion constraints
model.addConstr(schedule.sum() == len(tasks),"All tasks")

for t in range(0,len(tasks)):
	sumCom = 0
	for j in range(0,len(specs)):
		sumCom += schedule[t,j]
	model.addConstr(sumCom == 1, "Uniqueness")

#objective function
obj = quicksum(quicksum(placementCost[m]
'''model.addConstrs(quicksum((tasks[t][3] * schedule[t][j] for t in range(0,len(tasks))) <= specs[j][1]) for j in range(0,len(specs)),"Processing")
#bandwidth
#model.addConstrs(quicksum(tasks[t][]))

#Completion constraints
model.addConstrs(quicksum(schedule) == len(tasks),"All tasks completed")
model.addConstrs(((quicksum(schedule[t]) == 1) for t in range(0,len(tasks))),"Uniqueness")

#Time constraints
#model.addConstrs(quicksum(schedule[t][j]*delt[t][j] for j in range (0,len('''
