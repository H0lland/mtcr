#include "service.h"

service::service(){}

service::service(char* n){
	name = n;
}

char* service::getName(){
	return name;
}
