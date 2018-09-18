#ifndef __PCBLIST_H_
#define __PCBLIST_H_

class PCB;
class PCBList{
public:
	struct node{
		PCB *data;
		node *next;
		node(PCB* asdf): data(asdf), next(0){}
	};
	node *first, *last;
	node *temp;
	PCB *PCBtemp;
	PCBList();
	~PCBList();
	void insertFirst(PCB* asdf);
	void insertLast(PCB* asdf);
	void popFirst(); //PCB iz prvog se smesta u PCBtemp
	void flush();

};

#endif
