#include "task.h"

task::task(int in, int out, int comp, int serv){
	inSize = in;
	outSize = out;
	compTime = comp;
	type = serv;
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

int task::getType(){
	return type;
}
