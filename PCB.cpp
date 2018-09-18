#include "PCB.h"
#include "kernel.h"
#include <dos.h>
#include "defines.h"
#include "PCBList.h"
#include "SLPList.h"
#include "SCHEDULE.H"
#include "IdleThr.h"
#include "thread.h"
#include <iostream.h>

extern SleepList* sleeping;
extern Thread* placeholder;
PCB* PCB::running = 0;

PCB::PCB(Thread* myThr, unsigned long ssize, unsigned int tslice): stkSize(ssize), myThread(myThr), flag(FLAG_NEW), kvant(tslice){
	stkSize /= sizeof(unsigned);
	this->waiting = new PCBList();
	stack = new unsigned[stkSize];
	if(!stack){
		//abort
	}
	//redosled postavljanja podataka na stek u slucaju prekida:
	//PSW(Flag) -> CS -> IP
	stack[stkSize - 1] = 0x200; // I = 1, ostalo = 0
#ifndef BCC_BLOCK_IGNORE
	stack[stkSize - 2] = FP_SEG(PCB::run);
	stack[stkSize - 3] = FP_OFF(PCB::run);
#endif
	//init SP
	//izmedju [a-d]x registri, si, di, es, ds = 8 reg ukupno
#ifndef BCC_BLOCK_IGNORE
	this->ss = FP_SEG(stack + stkSize - 12);
	this->sp = FP_OFF(stack + stkSize - 12);
#endif
	this->bp = sp;
}

PCB::~PCB(){
	delete waiting;
	delete stack;
}

void PCB::sleep(unsigned int time){
	if(PCB::running){
		running->flag = FLAG_SLP;
		sleeping->insert(running, time);
		dispatch();
	}
}

void PCB::wait2complete(){
	if(PCB::running != this && flag != FLAG_FIN && myThread != placeholder){
		PCB::running->flag = FLAG_BLK;
		if(PCB::running){
			this->waiting->insertFirst(PCB::running);
		}
		dispatch();
	}
}

void PCB::start(){
	if(flag == FLAG_NEW){//samo se jednom moze pozvati start
		this->flag = FLAG_RDY;
		//??
		Scheduler::put(this);
	}
}

void PCB::run(){
	PCB::running->myThread->run();
	PCB::running->flag = FLAG_FIN;
	PCB::running->waiting->flush();
	dispatch();
}

PCB* PCB::getPCB(Thread* asdf){
	return asdf->myPCB;
}
