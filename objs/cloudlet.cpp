#include "cloudlet.h"
using std::vector;

cloudlet::cloudlet(int s, int b, int p){
	storage = s;
	bandw = b;
	procs = p;
	remStor = s;
	remBand = b;
	remProcs = p;
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

int cloudlet::getRemStor(){
	return remStor;
}

int cloudlet::getRemBand(){
	return remBand;
}

int cloudlet::getRemProcs(){
	return remProcs;
}

std::vector<user> cloudlet::getUsers(){
	return connUs;
}

std::vector<int> cloudlet::getServs(){
	return servs;
}

std::vector<task> cloudlet::getTasks(){
	return tasks;
}

void cloudlet::addServ(int S){
	servs.push_back(S);
}

void cloudlet::addUser(user U){
	connUs.push_back(U);
}

void cloudlet::addTask(task T){
	tasks.push_back(T);
}
