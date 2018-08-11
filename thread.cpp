#include "thread.h"
#include "PCB.h"

extern volatile int zahtevana_promena_konteksta;

void dispatch(){ // sinhrona promena konteksta 
	asm cli;
	zahtevana_promena_konteksta = 1;
	timer();
	asm sti;
}

Thread::Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice){

}

void Thread::waitToComplete(){
	myPCB->wait2complete();
}