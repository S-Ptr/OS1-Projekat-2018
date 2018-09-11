#include "PCB.h"
#include "kernel.h"
#include "Thread.h"
#include <dos.h>
#include "defines.h"
#include "PCBList.h"
#include "SLPList.h"
#include "SCHEDULE.H"


extern SleepList sleeping;
PCB* PCB::running = 0;

void PCB::wait2complete(){
	if(PCB::running != this || flag == FLAG_FIN){
		PCB::running->flag = 0x8;
		this->waiting->insertFirst(PCB::running);
	}
}

PCB::PCB(Thread* myThr, unsigned long ssize, unsigned int tslice): stkSize(ssize), myThread(myThr), flag(0x400), waiting(), kvant(tslice){
	stkSize /= sizeof(unsigned);
}

void PCB::start(){
	if(flag == FLAG_NEW){//samo se jednom moze pozvati start
		//u slucaju da se nit ne pokrene
		//nece se zauzeti stek za nju
		stack = new unsigned[stkSize];
		if(!stack){
			//abort
		}
		//redosled postavljanja podataka na stek u slucaju prekida:
		//PSW(Flag) -> CS -> IP
		stack[stkSize - 1] = 0x200; // I = 1, ostalo = 0
#ifndef BCC_BLOCK_IGNORE
		stack[stkSize - 2] = FP_SEG(PCB::wrapper);
		stack[stkSize - 3] = FP_OFF(PCB::wrapper);
#endif
		//init SP
		//izmedju [a-d]x registri, si, di, es, ds = 8 reg ukupno
#ifndef BCC_BLOCK_IGNORE
		this->ss = FP_SEG(stack + stkSize - 12);
		this->sp = FP_OFF(stack + stkSize - 12);
#endif
		this->bp = sp;
		//deo koji smo svi cekali
#ifndef BCC_BLOCK_IGNORE
		_lock
#endif
		this->flag = FLAG_RDY;
		//??
		//put nije atomicna operacija, zato se stavlja lock/unlock
		Scheduler::put(this);
#ifndef BCC_BLOCK_IGNORE
		_unlock
#endif
	}
}

void PCB::run(){
	PCB::running->myThread->run();
	PCB::running->flag = FLAG_FIN;
	PCB::running->waiting->temp = PCB::running->waiting->first;
	while(PCB::running->waiting->temp){
		PCB::running->waiting->first = PCB::running->waiting->temp->next;
		Scheduler::put(PCB::running->waiting->temp->data);
		PCB::running->waiting->temp->data = 0;
		PCB::running->waiting->temp->next = 0;
		delete PCB::running->waiting->temp;
		PCB::running->waiting->temp = PCB::running->waiting->first;
	}
	dispatch();
}

void PCB::sleep(unsigned int time){
	running->flag = 0x4; //Sleeping
	sleeping.insert(running, time);
	dispatch();
}

