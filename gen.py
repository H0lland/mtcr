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
    
    minDist = 1
    maxDist = 10
    
main(4,10,20)