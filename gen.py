from random import randrange
def main(cloudlets,users,tasks):
    minUsers = 1
    maxUsers = 4 * minUsers
    unassigned = users
    conns = []
    cl = 0
    #while there are unassigned users
    while unassigned > 0:
        print(unassigned)
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
    print(conns)
    
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

    print(dists)
main(4,10,20)
