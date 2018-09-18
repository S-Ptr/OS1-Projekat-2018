#include "PCBList.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "defines.h"

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
		last->next = temp;
		last = temp;
	}
}

void PCBList::popFirst(){//stored in PCBtemp
	if(!first){
		PCBtemp = 0;
		return;
	}
	if(first == last){
		last = 0;
	}
	temp = first;
	first = first->next;
	PCBtemp = temp->data;
	temp->next = 0;
	temp->data = 0;
	delete temp; //maskiranje?
}

void PCBList::flush(){//used only for waitToComplete()
	this->temp = this->first;
		while(this->temp){
			this->first = this->temp->next;
	#ifndef BCC_BLOCK_IGNORE
			_lock;
			Scheduler::put(this->temp->data);
			_unlock;
	#endif
			this->temp->data = 0;
			this->temp->next = 0;
			delete this->temp;
			this->temp = this->first;
		}
}
