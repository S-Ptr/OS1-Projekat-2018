#include "PCBList.h"
#include "PCB.h"

PCBList::PCBList(): first(0), last(0){}

void PCBList::insertFirst(PCB* asdf){
	temp = new Node(asdf);//tek tako?
	if(!first){
		first = last = temp;
	}
	else {
		temp->next = first;
		first = temp;
	}
}

void PCBList::insertLast(PCB* asdf){
	temp = new Node(asdf);
	if(!last){
		first = last = temp;
	}
	else {
		last->next = first;
		last = temp;
	}
}

PCB* PCBList::popFirst(){
	temp = first;
	first = first->next;
	PCBtemp = temp->data;
	temp->next = 0;
	temp->data = 0;
	delete temp; //maskiranje?
	return PCBtemp
}
