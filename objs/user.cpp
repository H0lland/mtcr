#include "user.h"
using std::vector;

user::user(){};

std::vector<int> user::getQos(){
	return qos;
}

std::vector<task> user::getTasks(){
	return tasks;
}

void user::addTask(task t){
	tasks.push_back(t);
}

void user::addQos(int q){
	qos.push_back(q);
}
