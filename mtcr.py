#from gurobiypy import *

#Model data

file = open("out.txt","r")
lines = file.readlines()
specs = lines[0][:-2].split(";")
for i in range(len(specs)):
    specs[i] = specs[i][:-1].split(",")
    specs[i] = list(map(int,specs[i]))
conns = lines[1][:-2].split(",")
conns = list(map(int,conns))
qos = lines[2][:-2].split(";")
for i in range(len(qos)):
    qos[i] = qos[i][:-1].split(",")
    qos[i] = list(map(int,qos[i]))
dists = lines[3][:-2].split(";")
for i in range(len(dists)):
    dists[i] = dists[i][:-1].split(",")
    dists[i] = list(map(int,dists[i]))
tasks = lines[4][:-2].split(";")
for i in range(len(tasks)):
    tasks[i] = tasks[i][:-1].split(",")
    tasks[i] = list(map(int,tasks[i]))
print(specs)
print(conns)
print(qos)
print(dists)
print(tasks)
storageCosts = [1,1,1,1,1]
cloudletStorage = [2,1]
cloudletBand = [5,5]
cloudletProcs = [4,4]
placementCosts = [1,1,1,1,1]
