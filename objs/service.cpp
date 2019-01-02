#include "service.h"

service::service(){}

service::service(char* n, int k){
	name = n;
	key = k;
}

char* service::getName(){
	return name;
}

int service::getKey(){
	return key;
}
