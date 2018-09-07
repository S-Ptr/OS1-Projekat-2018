#include "thread.h"
#include "PCB.h"

extern volatile int csreq;

void dispatch(){ // sinhrona promena konteksta 
	asm cli;
	csreq = 1;
	timer();
	asm sti;
}

Thread::Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice){
	myPCB = new PCB(this);
}

void Thread::waitToComplete(){
	myPCB->wait2complete();
}