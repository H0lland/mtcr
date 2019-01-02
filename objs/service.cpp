#include "service.h"

service::service(){}

service::service(int k){
	key = k;
}

int service::getKey(){
	return key;
}
