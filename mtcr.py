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
placing = model.addVars(services-1,len(dists))
#create scheduling matrix
schedule = model.addVars(len(tasks)-1,len(dists))
#include cloudlet spec constraints
model.addConstrs(quicksum(storageCosts[m]*placing[m,j] for m in range(0,len(storageCosts))) <= specs[j][0] for j in range(0,len(specs)))
#model.addConstrs(quicksum(tasks[t][3
