#ifndef __PCB_H_
#define __PCB_H_

//flag definitions
#define FLAG_RDY 0x1
#define FLAG_RUN 0x2
#define FLAG_SLP 0x4
#define FLAG_BLK 0x8
#define FLAG_FIN 0x10
#define FLAG_NEW 0x20

class Thread;
class PCBList;

class PCB{
public:
	PCB(Thread* myThr, unsigned long ssize, unsigned int tslice);
	~PCB();
	static void sleep(unsigned int time);
	void wait2complete();
	void start();
	static void run(); //omotacka funkcija
	static PCB* getPCB(Thread* asdf);

	unsigned long stkSize;
	unsigned* stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	PCBList* waiting;
	char flag;
	unsigned int kvant;
	int ID;
	static int IDgen;
	Thread* myThread;

	static PCB* running;
};

#endif
