#include "task.h"
#include<vector>
using std::vector;

class user{
	private:
		std::vector<int> qos;
		std::vector<task> tasks;
	public:
		user();
		std::vector<int> getQos();
		std::vector<task> getTasks();
		void addTask(task t);
		void addQos(int q);
};
