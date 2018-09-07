#ifndef __PCB_H_
#define __PCB_H_

class Thread;

class PCB{
public:
	PCB(Thread* myThr);
	~PCB();
	void sleep(int time);
	void wait2complete();
	unsigned* stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	//flag guide: 
	//bit 0 = ready; bit 1 = running
	//bit 2 = sleep; bit 3 = blocked;
	//bit 4 = finished;
	char flag;
	int kvant;
	Thread* myThread;
};

#endif