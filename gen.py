from random import randrange
def main(cloudlets,users,tasks, out):
    minUsers = 1
    maxUsers = 4 * minUsers
    unassigned = users
    conns = []
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
    conns.sort()
    
    print("conns:")
    print(conns)

    #set Dist variables
    minLocalDist = 1
    maxLocalDist = 10
    maxGlobalDist = 100
    minCloudDist = 50
    
    dists = []
    #for each cloudlet
    for i in range(cloudlets):
        tmp = []
        #for each user
        for j in range(len(conns)):
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
    for i in range(len(conns)):
        elem = randrange(minCloudDist, maxGlobalDist)
        tmp.append(elem)
    dists.append(tmp)
    print("dists:")
    for i in range(len(dists)):
        print(dists[i])

    #set service variables
    inSizes = [2,3,4,3,2]
    outSizes = [1,1,1,0,0]
    compTimes = [1,1,1,1,1]
    #the number of occurrences out of 1000 for the most common service 
    zipfBases = [437,656,801,911,1000]

    tasks = []
    for i in range(len(conns)):
        #2 tasks for user
        for j in range(2):
            tmp = []
            tmp.append(i)
            serv = randrange(1,1001)
            servType = -1
            #determine where in the zipf distribution the thing fell
            for k in range(len(zipfBases)):
                if serv <= zipfBases[k] and servType == -1:
                    servType = k
            tmp.append(inSizes[servType])
            tmp.append(outSizes[servType])
            tmp.append(compTimes[servType])
            tmp.append(servType)
            tasks.append(tmp)
    print("tasks:")
    for i in range(len(tasks)):
        print(tasks[i])
    #open file for output
    file = open(out+".txt","w")
    #write output to file
    for i in range(len(conns)):
        file.write(str(conns[i]) + ",")
    file.write("\n")
    for i in range(len(dists)):
        for j in range(len(dists[i])):
            file.write(str(dists[i][j]) + ",")
        file.write("\n")
    for i in range(len(tasks)):
        for j in range(len(tasks[i])):
            file.write(str(tasks[i][j])+",")
        file.write("\n")
    file.close()
main(4,10,20,"out")
