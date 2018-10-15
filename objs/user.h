#include "task.h"
#include<vector>
using std::vector;

class user{
	private:
		int key;
		std::vector<int> qos;
		std::vector<task> tasks;
	public:
		user(int n);
		int getKey();
		std::vector<int> getQos();
		std::vector<task> getTasks();
		void addTask(task t);
		void addQos(int q);
};
