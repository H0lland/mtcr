class service{
	private:
		int key, stor, placeCost, schedCost;
	public:
		service();
		service(int k,int st, int pc, int sc);
		int getKey();
		int getStor();
		int getPlace();
		int getSched();
		bool operator==(service rhs);
};
