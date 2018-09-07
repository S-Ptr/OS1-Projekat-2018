#ifndef _SLEEPLIST_H_
#define _SLEEPLIST_H_

class PCB;

class SleepList{
public:
	int baseCount;
	int maxTimeInList;
	typedef struct slpnode slpNode;
	struct slpnode{
		int timeDiff;
		PCB *data;
		Node *next;
		slpNode(PCB* asdf, int time): data(asdf), next(0){}
	};
	Node *first, *last;
	static Node *temp;
	static PCB *PCBtemp;
	PCBList();
	~PCBList();
	void insertFirst(PCB* asdf);
	void insertLast(PCB* asdf);
	PCB* popFirst();
};
#endif