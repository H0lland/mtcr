class task {
	private: 
		int inSize, outSize, compTime, type;	

	public:
	task(int in, int out, int comp, int serv);
	int getIn();
	int getOut();
	int getComp();
	int getType();
};
