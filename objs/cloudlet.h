#include "user.h"
#include<vector>
using std::vector;

class cloudlet{
	private:
		int storage, bandw, procs, remStor, remBand, remProcs;
		std::vector<user> connUs;
		std::vector<int> servs;
		std::vector<task> tasks;

	public:
		cloudlet(int s, int b, int p);

		int getStor();
		int getBand();
		int getProcs();
		int getRemStor();
		int getRemBand();
		int getRemProcs();
		std::vector<user> getUsers();
		std::vector<int> getServs();
		std::vector<task> getTasks();
		void addServ(int S);
		void addUser(user U);
		void addTask(task T);
};	
