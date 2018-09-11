#ifndef _SLEEPLIST_H_
#define _SLEEPLIST_H_

//delta list implementation

class PCB;

class SleepList{
public:
	struct slpnode{
		unsigned int delta;
		PCB *data;
		slpnode *next;
		slpnode(PCB* asdf): data(asdf), next(0){}
	};
	slpnode *first;
	slpnode *temp;
	slpnode *iter;
	PCB *PCBtemp;
	SleepList();
	~SleepList();
	void insert(PCB* asdf, unsigned int time);
	/*test functions*/
	void prtList();

};
#endif
