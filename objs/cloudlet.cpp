#include "cloudlet.h"
using std::vector;

cloudlet::cloudlet(int s, int b, int p, int k){
	storage = s;
	bandw = b;
	procs = p;
	remStor = s;
	remBand = b;
	remProcs = p;
	key = k;
}

int cloudlet::getKey(){
	return key;
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

void cloudlet::reduceStor(int n){
	remStor -= n;
}

void cloudlet::reduceBand(int n){
	remBand -= n;
}

void cloudlet::reduceProcs(int n){
	remProcs -= n;
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
