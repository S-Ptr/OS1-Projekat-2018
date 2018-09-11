#include "thread.h"
#include "PCB.h"
#include "kernel.h"

extern volatile int csreq;

void dispatch(){ // sinhrona promena konteksta 
#ifndef BCC_BLOCK_IGNORE
	asm cli;
#endif
	csreq = 1;
	timer();
#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif
}

Thread::Thread(StackSize stackSize, Time timeSlice){
	if(stackSize > 65535) myPCB = new PCB(this, 65535, timeSlice);
	else myPCB = new PCB(this, stackSize, timeSlice);
}

void Thread::waitToComplete(){
	myPCB->wait2complete();
}

void Thread::start(){
	myPCB->start();
}

void Thread::sleep(Time timeToSleep){
	PCB::sleep(timeToSleep);
}

PCB* Thread::getPCB(){
	return myPCB;
}
