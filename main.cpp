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
		cloudlet c(curr.at(0),curr.at(1),curr.at(2),i);	
		rtn.push_back(c);
	}
	return rtn;
}

//makeUsers: Takes the user QoSes and the created task vector and creates the users.
vector<user> makeUsers(vector<vector<int>> qos, vector<task> tasks){
	//initializations
	vector<user> rtn;
	//for each user
	int count = 0;
	for(int i = 0; i < qos.size(); i+=1){
		user u(i);
		//for each task
		for(int j = 0; j< qos.at(i).size(); j+=1){
			u.addQos(qos.at(i).at(j));
			u.addTask(tasks.at(count));	
			//iterate task counter
			count += 1;
		}
		rtn.push_back(u);
	}
	return rtn;
}

//connect: connects the cloudlets to the users based on connection input
void connect(vector<cloudlet> &cls, vector<user> users, vector<vector<int>> params){
	//for each user
	for(int i = 0; i < params.at(0).size(); i+=1){
		int cl = params.at(0).at(i);
		cls.at(cl).addUser(users.at(i));
	}
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
		t.setType(servs.at(curr.at(4)));
		rtn.push_back(t);	
	}
	return rtn;
}

//Servible: determine if a task is servible by a cloudlet (assuming the proper service is placed on that cloudlet
bool servible(int pos, user U, cloudlet cl, vector<vector<int>> dists){
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
	/*if(not local && servible){ //if not local and still servible check
		//task requires more band than cloudlet has
		if(tas.getIn() + tas.getOut() > cl.getRemBand()){ 
			servible = 0;
		}
	}
	if(servible){ 
		//if task requires more storage than cloudlet has
		if(serv.getStor() > cl.getRemStor()){
			servible = 0;
		}
	}*/
	if(servible){
		//if task cannot be completed in time
		double dist = double(dists.at(cl.getKey()).at(key));
		double up = tas.getIn() * .001 * dist;
		double down = tas.getOut() * .001 * dist;
		double total = up + down + tas.getComp();
		if(total > U.getQos().at(pos)){
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
*/
//maxElement: returns max int in a vector of ints excluding the last element
int maxElement(vector<double> vec){
	int max = 0;
	for(int i = 1; i< vec.size(); i++){
		if(vec.at(i) > vec.at(max)){
			max = i;
		}
	}	
	return max;
}

//Greedy Global Algorithm 1: Select services based on number of tasks of a type of service across the entire system
std::vector<int> selectServices1(vector<cloudlet> cls, vector<service> servs){
	int numSer = servs.size();
	vector<float> buckets;
	//initialize buckets
	for(int i = 0; i < numSer; i++){
		buckets.push_back(0);
	}
	//variable to store total remaining storage
	vector<double> remains;
	int remain = 0;
	for(int i = 0; i < cls.size(); i++){
		vector<int> clServs = cls.at(i).getServs();
		vector<user> users = cls.at(i).getUsers();
		remains.push_back(cls.at(i).getRemStor());
		remain += cls.at(i).getRemStor();
		//count occurences for each service
		for(int j = 0; j < users.size(); j++){
			vector<task> tasks = users.at(j).getTasks();
			for(int k = 0; k < tasks.size(); k++){
				int serv = tasks.at(k).getType().getKey();
				buckets.at(serv)+=1.0;
				}
		}
	}
	vector<int> rtn;
	bool placed = true;
	int largest = maxElement(remains);
	//make this take into account the storage restraint
	while(remain > 0 && placed){	
		placed = false;
		float max = 0;
		int pos = -1;
		//pick the next service needed
		for(int j = 0; j < numSer; j++){
			float profit = buckets.at(j)/servs.at(j).getPlace(); //determine profit as tasks over cost
			//find the maximum that fits and servs enough tasks to be worth it
			if(profit > max && remains.at(largest) >= servs.at(j).getStor() && profit > 1){
				max = profit;
				pos = j;
			}
		}
		//service could be placed
		if(pos > -1){
			//set flag
			placed = true;
			rtn.push_back(pos);	
			//adjust values
			buckets.at(pos)/=2;	
			remain -= servs.at(pos).getStor();
			remains.at(largest) -= servs.at(pos).getStor();
			largest = maxElement(remains);
		}
	}
	return rtn;
}

//scheduleGlobal: takes cloudlets, users, dists, and the chosen services and distributes services and schedules tasks
vector<vector<vector<vector<int>>>> scheduleGlobal(vector<cloudlet> cls, vector<user> users, vector<vector<int>> dists, vector<service> servs, vector<int> pool, int alpha){
	//initializations
	vector<vector<vector<vector<int>>>> rtn;
	//create a 2d vec for each cloudlet
	for(int i = 0 ; i < cls.size(); i++){
		vector<vector<vector<int>>> clVec(2);
		rtn.push_back(clVec);
	}
	//create a 2d vec for the status of each task
	vector<vector<bool>> scheded;
	for(int i =0; i < users.size(); i++){
		vector<bool> temp;
		for(int j = 0; j < users.at(i).getTasks().size(); j++){
			temp.push_back(false);
		}
		scheded.push_back(temp);
	}
	//for each service
	for(int i = 0; i < servs.size(); i++){
		vector<double> profits;
		vector<vector<vector<int>>> tasks;
		//for each cloudlet
		for(int j = 0; j < cls.size(); j++){
			cout << j << '\t';
			double prof = 0;
			vector<vector<int>> jTasks;
			//for each user
			for(int k = 0; k < users.size(); k++){
				//for each of that user's tasks
				for(int l = 0; l < users.at(k).getTasks().size(); l++){	
					//if the task hasn't been scheduled and is of type i
					if(!scheded.at(k).at(l)&&users.at(k).getTasks().at(l).getType()==servs.at(i)){
						//if the task is servible by cloudlet
						if(servible(l, users.at(k), cls.at(j),dists)){	
							//add profit
							prof += 1;
							vector<int> t{ k, l};
							jTasks.push_back(t);
						}
					}
				}
			}	
			tasks.push_back(jTasks);
			//if on cloudlet, consider alpha cost
			if(j <4){
				prof = prof / alpha;
			}
			profits.push_back(prof);
			cout << prof << endl;
		}
		//choose largest
		int chosen = maxElement(profits);
		cout << chosen << endl;
		//add the service and tasks to that cloudlet's lists
		vector<int> s;
		s.push_back(servs.at(i).getKey());		
		rtn.at(chosen).at(0).push_back(s);
		cout << tasks.at(chosen).size() << endl;
		for(int x = 0; x < tasks.at(chosen).size(); x++){
			rtn.at(chosen).at(1).push_back(tasks.at(chosen).at(x));
		}
		cout << '\n';
	}
	return rtn;
}

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

int costOf(vector<vector<vector<vector<int>>>> answer, vector<int> placeCosts, vector<vector<int>> schedCosts, vector<user> users){
	//initializations
	int cost = 0;
	//loops for each cloudlet
	for(int i = 0; i < answer.size(); i++){
		//loops for place and schedule
		for(int j = 0; j < 2; j++){
			//loop over nested list
			for(int k = 0; k < answer.at(i).at(j).size(); k++){
				for(int l = 0; l < answer.at(i).at(j).at(k).size(); l++){
					int val = answer.at(i).at(j).at(k).at(l);
					//if scheduling
					if(j == 0){
						cost += placeCosts.at(val);
					}
					//if placing
					else{
						//at task level
						if(l == 1){
							int currU = answer.at(i).at(j).at(k).at(0);
							int serv = users.at(currU).getTasks().at(val).getType().getKey();
							cost += schedCosts.at(i).at(serv);
						}
					}
				}
			}
		}
	}
	return cost;
}
int main(int argc, char** argv){
	cout << "Enter filename: " << endl;
	string fn;
	cin >> fn;
	int alpha = 2;
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
	vector<service> servs;
	servs.push_back(service(0,1,3,1));
	servs.push_back(service(1,1,2,2));
	servs.push_back(service(2,1,1,3));
	servs.push_back(service(3,1,2,1));
	servs.push_back(service(4,1,1,3));
	vector<cloudlet> cls = makeCloudlets(in.at(0));
	vector<vector<int>> dists = in.at(3);
	vector<task> tasks = makeTasks(in.at(4),servs);
	vector<user> users = makeUsers(in.at(2),tasks);
	connect(cls,users,in.at(1));
	vector<int> chosen = selectServices1(cls,servs);
	/*for(int i = 0; i < chosen.size(); i++){
		cout << chosen.at(i) << endl;
	}*/
	//make schedCost vector
	vector<int> place;
	for(int i = 0; i < servs.size(); i++){
		sched.push_back(servs.at(i).getPlace());
	}
	//make placeCost vector
	vector<vector<int>> sched;
	for(int i = 0; i < cls.size(); i++){
		vector<int> temp;
		for(int j = 0; j < servs.size(); j++){
			temp.push_back(servs.at(j).getSched());
		}
		place.push_back(temp);
	}
	vector<vector<vector<vector<int>>>> answer = scheduleGlobal(cls, users, dists, servs, chosen, 2);
	for(int i = 0; i < answer.size(); i++){
		cout << i << ":" << endl;
		for(int j = 0; j < 2; j++){
			if(j == 0)
				cout << "placed:" << '\t';
			else
				cout << "sched:" << '\t';
			for(int k = 0; k < answer.at(i).at(j).size(); k++){
				for(int l = 0; l < answer.at(i).at(j).at(k).size(); l++){
					cout << answer.at(i).at(j).at(k).at(l) << ",";
				}
				cout << ";";
			}
			cout << endl;
		}
	}
	cout << "Algorithm Cost: " << costOf(answer, place, sched,  users) << endl;
}
