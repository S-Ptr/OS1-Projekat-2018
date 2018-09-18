#include "thread.h"
#include "PCB.h"
#include "kernel.h"
#include "defines.h"
#include <iostream.h>

extern volatile int csreq;

int Thread::idgen = 0;

void Thread::start(){
	myPCB->start();
}

void Thread::waitToComplete(){
	myPCB->wait2complete();
}

Thread::~Thread(){
	this->waitToComplete();
	delete myPCB;
}

void Thread::sleep(Time timeToSleep){
	PCB::sleep(timeToSleep);
}


Thread::Thread(StackSize stackSize, Time timeSlice){
	this->id = idgen++;
	if(stackSize > 65535) myPCB = new PCB(this, 65535, timeSlice);
	else myPCB = new PCB(this, stackSize, timeSlice);
}

void dispatch(){ // sinhrona promena konteksta
#ifndef BCC_BLOCK_IGNORE
	csreq=1;
	asm int 8h;
#endif
}

int Thread::getRunningId(){
	return PCB::running->ID;
}
