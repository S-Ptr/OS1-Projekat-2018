#ifndef __PCBLIST_H_
#define __PCBLIST_H_

class PCB;
class PCBList{
public:
	typedef struct node Node;
	struct node{
		PCB *data;
		Node *next;
	};
	Node *first, *last;

	PCBList();
	~PCBList();
	void insertFirst(PCB* asdf);
	void insertLast(PCB* asdf);
	PCB* popFirst();
	PCB* popLast();


};

#endif