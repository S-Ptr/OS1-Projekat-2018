#include "PCBList.h"
#include "PCB.h"

PCBList::PCBList(): first(0), last(0){}

PCBList::~PCBList(){
	while(first){
		popFirst();
	}
}

void PCBList::insertFirst(PCB* asdf){
	if(!asdf) return;
	temp = new node(asdf);
	if(!first){
		first = last = temp;
	}
	else {
		temp->next = first;
		first = temp;
	}
}

void PCBList::insertLast(PCB* asdf){
	if(!asdf) return;
	temp = new node(asdf);
	if(!last){
		first = last = temp;
	}
	else {
		last->next = first;
		last = temp;
	}
}

void PCBList::popFirst(){
	if(!first){
		PCBtemp = 0;
		return;
	}
	temp = first;
	first = first->next;
	PCBtemp = temp->data;
	temp->next = 0;
	temp->data = 0;
	delete temp; //maskiranje?
}
