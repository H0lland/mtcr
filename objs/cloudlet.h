#include "user.h"
#include<vector>
using std::vector;

class cloudlet{
	private:
		int storage, bandw, procs;
		std::vector<user> connUs;
		std::vector<int> servs;

	public:
		cloudlet(int s, int b, int p);

		int getStor();
		int getBand();
		int getProcs();
		std::vector<user> getUsers();
		std::vector<int> getServs();
		void addServ(int S);
		void addUser(user U);
		
};	
