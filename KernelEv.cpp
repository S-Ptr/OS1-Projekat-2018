#include "KernelEv.h"
#include "PCB.h"
#include "KSem.h"
#include "SCHEDULE.H"
#include "thread.h"
#include "IVTEntry.h"

KernelEv::KernelEv(IVTNo _ivtNo, PCB* _maker): ivtNo(_ivtNo), maker(_maker), blocked(0), val(1){
	(IVTEntry::fetchEntry(ivtNo))->myEv = this;
}

KernelEv::~KernelEv(){
	maker = blocked = 0;
	(IVTEntry::fetchEntry(ivtNo))->myEv = 0;
}

void KernelEv::signal(){
	if(!blocked) val = 1;
	else{
		KernelSem::lockSL();
		blocked->flag = FLAG_RDY;
		Scheduler::put(blocked);
		blocked = 0;
		KernelSem::unlockSL();
	}
	/*
	KernelSem::lockSL();
	if(val++ < 0){
		blocked->flag = FLAG_RDY;
		Scheduler::put(blocked);
		blocked = 0;
	}
	KernelSem::unlockSL();
	*/
}

void KernelEv::wait(){
	if(!val){
		KernelSem::lockSL();
		PCB::running->flag = FLAG_BLK;
		blocked = PCB::running;
		KernelSem::unlockSL();
		dispatch();
		return;
	}
	else{
		val = 0;
	}
	/*
	KernelSem::lockSL();
	if(PCB::running == maker){
		if(--val < 0){
			PCB::running->flag = FLAG_BLK;
			blocked = PCB::running;
			KernelSem::unlockSL();
			dispatch();
			return;
		}
	}
	KernelSem::unlockSL();
	*/
}


