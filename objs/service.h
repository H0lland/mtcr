class service{
	private:
		int key, inSize, outSize, comps, placeCost, schedCost;
	public:
		service();
		service(int k, int inSize, int outSize, int comps, int pc, int sc);
		int getKey();
		int getIn();
		int getOut();
		int getComp();
		int getPlace();
		int getSched();
		bool operator==(service rhs);
};
