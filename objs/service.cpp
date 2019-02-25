#include "service.h"

service::service(){}

service::service(int k,int iS,int oS,int cp,int pc,int sc){
	key = k;
	inSize = iS;
	outSize = oS;
	comps = cp;
	placeCost = pc;
	schedCost = sc;
}

int service::getKey(){
	return key;
}

int service::getIn(){
	return inSize;
}

int service::getOut(){
	return outSize;
}

int service::getComp(){
	return comps;
}

int service::getPlace(){
	return placeCost;
}

int service::getSched(){
	return schedCost;
}

bool service::operator==(service rhs){
	return this->getKey() == rhs.getKey();
}
