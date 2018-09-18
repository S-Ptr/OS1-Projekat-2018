#include "KSem.h"
#include "PCBList.h"
#include "SCHEDULE.H"
#include "thread.h"
#include "defines.h"
#include "PCB.h"

char KernelSem::semLock = 1;

KernelSem::KernelSem(int init){
	this->val = init;
	this->waiting = new PCBList();
}

KernelSem::~KernelSem(){
	delete waiting;
}

int KernelSem::wait(int toBlock){
	lockSL();
	if(toBlock == 0){
		if(val <= 0) {//nikom nista
			unlockSL();
			return -1;
		}
		else{//smanjuje se vrednost semafora
			--val;
			unlockSL();
			return 0;
		}

	}else{
		if(--val<0){//nit se blokira
			PCB::running->flag = FLAG_BLK;
			waiting->insertFirst(PCB::running);
			unlockSL();
			dispatch();
			return 1;
		} else{//nit se ne blokira
			unlockSL();
			return 0;
		}
	}
}

void KernelSem::signal(){
	lockSL();
	if(val++ < 0){
		waiting->popFirst();
		waiting->PCBtemp->flag = FLAG_RDY;
		Scheduler::put(waiting->PCBtemp);
	}
	unlockSL();
}
