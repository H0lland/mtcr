#include "task.h"

task::task(int in, int out, int comp){
	inSize = in;
	outSize = out;
	compTime = comp;
}

int task::getIn(){
	return inSize;
}

int task::getOut(){
	return outSize;
}

int task::getComp(){
	return compTime;
}

service task::getType(){
	return type;
}

void task::setType(service serv){
	type = serv;
}
