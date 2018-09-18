#ifndef _KSEM_H_
#define _KSEM_H_

class Semaphore;
class PCBList;

class KernelSem{
public:
	int val;
	PCBList* waiting;

	static char semLock;

	KernelSem(int init);
	~KernelSem();
	int wait(int toBlock);
	void signal();

	static void lockSL(){semLock = 0;}
	static void unlockSL(){semLock = 1;}

};



#endif /* KSEM_H_ */
