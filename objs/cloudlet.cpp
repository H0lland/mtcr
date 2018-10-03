#include "cloudlet.h"
using std::vector;

cloudlet::cloudlet(int s, int b, int p){
	storage = s;
	bandw = b;
	procs = p;
}

int cloudlet::getStor(){
	return storage;
}

int cloudlet::getBand(){
	return bandw;
}

int cloudlet::getProcs(){
	return procs;
}

std::vector<user> cloudlet::getUsers(){
	return connUs;
}

std::vector<int> cloudlet::getServs(){
	return servs;
}

void cloudlet::addServ(int S){
	servs.push_back(S);
}

void cloudlet::addUser(user U){
	connUs.push_back(U);
}
