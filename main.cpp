#include<iostream>
#include "objs/cloudlet.h"
#include<vector>
#include<string.h>
#include<fstream>
#include<iostream>
#include<sstream>
using namespace std;

//ParseIn: Parses input file (must be of type .gcon) and returns the content in a 2-D vector
vector<vector<vector<int>>> parseIn(string filename){
	//initializations
	ifstream readfile(filename);
	vector<vector<vector<int>>> rtn;
	string section;
	string line;
	string item;
	//for each line in the file
	while(getline(readfile,section)){
		//initializations
		vector<vector<int>> tmpSec;
		stringstream iss(section,ios_base::in);
		//for each subline
		while(getline(iss,line,';')){
			vector<int> tmpLine;
			stringstream iss2(line,ios_base::in);
			//for each item in the subline
			while(getline(iss2,item,',')){
				tmpLine.push_back(std::stoi(item));
			}
			tmpSec.push_back(tmpLine);
		}
		rtn.push_back(tmpSec);
	}
	return rtn;
}

//makeCloudlets:  Takes the listed cloudlet parameters from the input file and returns a vector of cloudlets.
vector<cloudlet> makeCloudlets(vector<vector<int>> params){
	//initializations
	vector<cloudlet> rtn;
	for(int i = 0; i< params.size(); i+=1){
		vector<int> curr = params.at(i);
		//create cloudlet
		cloudlet c(curr.at(0),curr.at(1),curr.at(2));
		cout << c.getStor();
		rtn.push_back(c);
	}
	return rtn;
}

//makeTasks: Takes the listed task parameters from the input file and returns a vector of tasks.
vector<task> makeTasks(vector<vector<int>> params, vector<service> servs){
	//initializations
	vector<task> rtn;
	//for each task
	for(int i=0; i< params.size(); i+=1){
		vector<int> curr = params.at(i);
		//create task
		task t(curr.at(1),curr.at(2),curr.at(3));
		rtn.push_back(t);	
	}
	return rtn;
}

//Servible: determine if a task is servible by a cloudlet (assuming the proper service is placed on that cloudlet
bool servible(int pos, user U, cloudlet cl){
	//initializations
	bool servible = 1;
	bool local = 0;
	task tas = U.getTasks().at(pos);
	service serv = tas.getType();
	int key = U.getKey();
	vector<user> users = cl.getUsers();
	//see if user is connected to cloudlet	
	for(int i = 0; i< users.size(); i++){
		if(key == users.at(i).getKey())
			local = 1;
	}
	if(tas.getComp() > cl.getRemProcs()){ //task requires more processing than cloudlet has
		servible = 0;
	}
	if(not local && servible){ //if not local and still servible check
		//task requires more band than cloudlet has
		if(tas.getIn() + tas.getOut() > cl.getRemBand()){ 
			servible = 0;
		}
	}
	return servible;
}
/*
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
			service serv = tasks.at(k).getType();
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
*/
//takes in a string and splits it into a 2D array of ints
vector<vector<int>> arrayify(string line){
	vector<vector<int>> rtn;
	vector<int> temp;
	string str = "";
	int i = 0;
	//while you haven't traversed the string
	while(i < line.length()){
		//if you encounter a line break, add temp to rtn, clear vars
		if(line.at(i) == ';'){
			rtn.push_back(temp);
			temp.clear();
			str = "";
		}
		else{
			//if you encounter a num break, add num to temp, clear vars
			if(line.at(i) == ','){
				temp.push_back(std::stoi(str));
				str = "";
			}
			//else, add digit to num
			else{
				str += line.at(i);
			}
		}
		i += 1;
	}
	return rtn;
}
/*
int main(int argc, char** argv){
	//check for correct usage
	if(argc < 2) {
		cout << "Incorrect usage!" <<endl;
		cout << "usage: ./main.cpp inFile" <<endl;
		return 0;
	}
	//initialize stuff for inFile parsing
	vector<string> lines;
	string line;
	ifstream myFile (argv[1]);
	//getlines
	if(myFile.is_open()){
		while(getline(myFile, line)){
		lines.push_back(line);
		}
	}
	vector<vector<int>> qos = arrayify(lines[2]);
	int three = qos.at(0).at(0);
	cout<< three << endl;

	//initialization 
	cout << "Hello" << endl;
	int numUsers = 4;
	int numCloudlets = 2;
	int numServices = 5;
	int storageCosts [5] = {1,1,1,1,1};	
	int inSize[5] = {1,1,1,1,1};
	int outSize[5] = {0,0,0,0,0};
	int compTimes[5] = {1,1,1,1,1};
	
	//create tasks of service 1
	task* task1 = new task(inSize[0],outSize[0],compTimes[0],0);	
	task* task7 = new task(inSize[0],outSize[0],compTimes[0],0);
	//create tasks of service 2
	task* task4 = new task(inSize[1],outSize[1],compTimes[1],1);
	//create tasks of service 3
	task* task3 = new task(inSize[2],outSize[2],compTimes[2],2);	
	task* task5 = new task(inSize[2],outSize[2],compTimes[2],2);
	task* task6 = new task(inSize[2],outSize[2],compTimes[2],2);
	//create tasks of service 4
	task* task8 = new task(inSize[3],outSize[3],compTimes[3],3);
	task* task9 = new task(inSize[3],outSize[3],compTimes[3],3);
	//create tasks of service 5
	task* task2 = new task(inSize[4],outSize[4],compTimes[4],4);	
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
}*/

int main(int argc, char** argv){
	cout << "Enter filename: " << endl;
	string fn;
	cin >> fn;
	vector<vector<vector<int>>> in = parseIn(fn);
	for(int i=0; i< in.size(); i+=1){
		for(int j = 0; j < in.at(i).size(); j+=1){
			for(int k = 0; k< in.at(i).at(j).size(); k+=1){
				cout << in.at(i).at(j).at(k) << ',';
			}
			cout << '\t';
		}
		cout << '\n';
	}
	vector<cloudlet> cls = makeCloudlets(in.at(0));
}
