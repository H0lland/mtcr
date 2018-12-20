#include "service.h"
class task {
	private: 
		int inSize, outSize, compTime;	
		service type;
	public:
	task(int in, int out, int comp);
	int getIn();
	int getOut();
	int getComp();
	service getType();
	void setType(service serv);
};
