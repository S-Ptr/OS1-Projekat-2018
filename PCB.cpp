#include "PCB.h"
#include "kernel.h"
#include "Thread.h"

extern volatile PCB* running;

void PCB::wait2complete(){
	if(running != this){
		
	}
}

PCB::PCB(Thread* myThr){
	this.myThread = MyThr;
}