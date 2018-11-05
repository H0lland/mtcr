#from gurobiypy import *

#Model data

file = open("out.txt","r")
lines = file.readlines()
conns = lines[0][:-2].split(",")
conns = list(map(int,conns))
dists = lines[1][:-2].split(";")
for i in range(len(dists)):
    dists[i] = dists[i][:-1].split(",")
    dists[i] = list(map(int,dists[i]))
tasks = lines[2][:-1].split(";")
for i in range(len(tasks)):
    tasks[i] = tasks[i][:-1].split(",")
    tasks[i] = list(map(int,tasks[i]))
storageCosts = [1,1,1,1,1]
cloudletStorage = [2,1]
cloudletBand = [5,5]
cloudletProcs = [4,4]
placementCosts = [1,1,1,1,1]
