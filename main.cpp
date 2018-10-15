#include<iostream>
#include "objs/cloudlet.h"
#include<vector>
using namespace std;

//Servible: determine if a task is servible by a cloudlet (assuming the proper service is placed on that cloudlet
bool servible(int pos, user U, cloudlet cl){
	//initializations
	bool servible = 1;
	bool local = 0;
	task* tas = U.getTasks().at(pos);
	int serv = tas->getType();
	int key = U.getKey();
	vector<user> users = cl.getUsers();
	//see if user is connected to cloudlet	
	for(int i = 0; i< users.size(); i++){
		if(key == users.at(i).getKey())
			local = 1;
	}
	if(local){
		
	}
	return servible;
}
//Greedy Local Algorithm 1: pick tasks by tasks/storage metric
int nextService1(cloudlet cl, int numSer, int stor[]){
	int buckets[numSer];
	int remain = cl.getRemStor(); 
	//initialize buckets
	for(int i = 0; i < numSer; i++){
		buckets[i] = 0;
	}
	vector<int> servs = cl.getServs();
	vector<user> users = cl.getUsers();
	//count occurences for each service
	for(int j = 0; j < users.size(); j++){
		vector<task> tasks = users.at(j).getTasks();
		for(int k = 0; k < tasks.size(); k++){
			int serv = tasks.at(k).getType();
			buckets[serv-1]++;
		}
	}
	float max = 0;
	int pos = 0;
	//pick the next service needed
	for(int i = 0; i < numSer; i++){
		float profit = buckets[i]/stor[i]; //determine profit as tasks over cost
		if(profit > max && stor[i] <= remain){
			max = profit;
			pos = i;
		}
	}
	return (pos+1);
}

//Greedy Local Algorithm 2: pick tasks by num users served/storage metric
int nextService2(cloudlet cl, int numSer, int stor[]){
	int buckets[numSer];
	int remain = cl.getRemStor();
	//initialize buckets
	for(int i = 0; i < numSer; i++){
		buckets[i] = 0;
	}
	vector<int> servs = cl.getServs();
	vector<user> users = cl.getUsers();
	//count occurences for each service
	for(int j = 0; j < users.size(); j++){
		vector<task> tasks = users.at(j).getTasks();
		int tempArr [numSer];
		//make a temp array for each user
		for(int k = 0; k < numSer; k++){
			tempArr[k]=0;
		}
		for(int k = 0; k < tasks.size(); k++){
			int serv = tasks.at(k).getType();
			tempArr[serv-1]=1;
		}
		for(int k = 0; k < numSer; k++){
			buckets[k] += tempArr[k];
		}
	}
	float max = 0;
	int pos = 0;
	//pick the next service needed
	for(int i = 0; i < numSer; i++){
		float profit = buckets[i]/stor[i]; //determine profit as tasks over cost
		if(profit > max && stor[i] <= remain){
			max = profit;
			pos = i;
		}
	}
	return (pos+1);
}

//Greedy Global Algorithm 1: Select services based on number of tasks of a type of service across the entire system
std::vector<int> selectServices1(vector<cloudlet> cls, int numSer, int stor[]){
	int buckets[numSer];
	//initialize buckets
	for(int i = 0; i < numSer; i++){
		buckets[i] = 0;
	}
	int remain = 0; //variable to store total remaining storage
	for(int i = 0; i < cls.size(); i++){
		vector<int> servs = cls.at(i).getServs();
		vector<user> users = cls.at(i).getUsers();
		remain += cls.at(i).getRemStor();	
		//count occurences for each service
		for(int j = 0; j < users.size(); j++){
			vector<task> tasks = users.at(j).getTasks();
			for(int k = 0; k < tasks.size(); k++){
				int serv = tasks.at(k).getType();
				buckets[serv-1]++;
				}
		}
	}
	int i = 0;
	vector<int> rtn;
	//make this take into account the storage restraint
	while(i < 3){
		float max = 0;
		int pos = 0;
		//pick the next service needed
		for(int j = 0; j < numSer; j++){
			float profit = buckets[j]/stor[j]; //determine profit as tasks over cost
			if(profit > max){
				max = profit;
				pos = j;
			}
		}
		rtn.push_back(pos+1);
		i+=1;
	}
	return rtn;
}

int main(){
	cout << "Hello" << endl;
	int numUsers = 4;
	int numCloudlets = 2;
	int numServices = 5;
	int storageCosts [5] = {1,1,1,1,1};	
	//base parameters for a task of type service 1
	int serv1In = 1;
	int serv1Out = 0;
	int serv1Comp = 1;
	//base parameters for a task of type service 2	
	int serv2In = 1;
	int serv2Out = 0;
	int serv2Comp = 1;
	//base parameters for a task of type service 3
	int serv3In = 1;
	int serv3Out = 0;
	int serv3Comp = 1;
	//base parameters for a task of type service 4
	int serv4In = 1;
	int serv4Out = 0;
	int serv4Comp = 1;
	//base parameters for a task of type service 5
	int serv5In = 1;
	int serv5Out = 0;
	int serv5Comp = 1;

	//create tasks of service 1
	task* task1 = new task(serv1In, serv1Out, serv1Comp, 0);
	task* task7 = new task(serv1In, serv1Out, serv1Comp, 0);	
	//create tasks of service 2
	task* task4 = new task(serv2In, serv2Out, serv2Comp, 1);
	//create tasks of service 3
	task* task3 = new task(serv3In, serv3Out, serv3Comp, 2);	
	task* task5 = new task(serv3In, serv3Out, serv3Comp, 2);
	task* task6 = new task(serv3In, serv3Out, serv3Comp, 2);
	//create tasks of service 4
	task* task8 = new task(serv4In, serv4Out, serv4Comp, 3);
	task* task9 = new task(serv4In, serv4Out, serv4Comp, 3);
	//create tasks of service 5
	task* task2 = new task(serv5In, serv5Out, serv5Comp, 4);

	//create user1
	user* user1 = new user(1);
	user1->addTask(*task1);
	user1->addQos(0);
	user1->addTask(*task2);
	user1->addQos(0);
	//create user2
	user*user2 = new user(2);
	user2->addTask(*task3);
	user2->addQos(0);
	user2->addTask(*task4);
	user2->addQos(0);
	//create user3
	user* user3 = new user(3);
	user3->addTask(*task5);
	user3->addQos(0);
	user3->addTask(*task6);
	user3->addQos(0);
	//create user4
	user* user4 = new user(4);
	user4->addTask(*task7);
	user4->addQos(0);
	user4->addTask(*task8);
	user4->addQos(0);
	user4->addTask(*task9);
	user4->addQos(0);

	//create cloudlet1
	cloudlet* cl1 = new cloudlet(2,5,4);
	cl1->addUser(*user1);
	cl1->addUser(*user2);
	//create cloudlet2
	cloudlet* cl2 = new cloudlet(1,5,4);
	cl2->addUser(*user3);
	cl2->addUser(*user4);

	int next1 = nextService1(*cl1, numServices, storageCosts);		
	cout << next1 << endl;
	int next = nextService2(*cl1, numServices, storageCosts);		
	cout << next << endl;
}
