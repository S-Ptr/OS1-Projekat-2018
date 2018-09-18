#include "Thread.h"

extern int userMain(int argc, char* argv[]);

class MainThr: public Thread{
public:
	MainThr():Thread(){}
	~MainThr(){waitToComplete();}
	void run();
	static int MTargc;
	static char** MTargv;
	static MainThr* mainThr;
	static int retval;
};

