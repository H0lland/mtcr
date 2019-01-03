#include "service.h"

service::service(){}

service::service(int k,int st,int pc,int sc){
	key = k;
	stor = st;
	placeCost = pc;
	schedCost = sc;
}

int service::getKey(){
	return key;
}

int service::getStor(){
	return stor;
}

int service::getPlace(){
	return placeCost;
}

int service::getSched(){
	return schedCost;
}
