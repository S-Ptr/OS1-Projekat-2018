#include "kernel.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include "IdleThr.h"
#include "SLPList.h"
#include "MainThr.h"
#include "defines.h"
#include <iostream.h>
#include <dos.h>
#include <STDIO.h>
#include "KSem.h"

Thread* placeholder;
SleepList* sleeping;
extern void tick();
PCB* masterPCB; //cuva kontekst main funkcije

//temporary storage
unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int counter = 20;
volatile int csreq = 0;

void interrupt timer(...){	// prekidna rutina
	if (!csreq) {
		counter--;
		if(sleeping->first){
			--((sleeping->first)->delta);
			while((sleeping->first)->delta == 0){
				Scheduler::put(sleeping->first->data);
				sleeping->temp = sleeping->first->next;
				sleeping->first->next = 0;
				sleeping->first->data = 0;
				delete sleeping->first;
				sleeping->first = sleeping->temp;
			}
		}
	}
	if ((counter == 0 || csreq)&&KernelSem::semLock) {
#ifndef BCC_BLOCK_IGNORE
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif
		if(PCB::running != 0){
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;
			if(PCB::running->myThread != placeholder && PCB::running->flag == FLAG_RUN && PCB::running != masterPCB){
				PCB::running->flag = FLAG_RDY;
				Scheduler::put(PCB::running);
			}
		}
		PCB::running= Scheduler::get();	// Scheduler
  		
		if(PCB::running == 0){
			PCB::running = PCB::getPCB(placeholder);
		}
		PCB::running->flag = FLAG_RUN;

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		counter = PCB::running->kvant;
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}
#endif
	} 
    
	// poziv stare prekidne rutine koja se 
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida	
#ifndef BCC_BLOCK_IGNORE
	if(!csreq)
	{
		tick();
		asm int 60h;
	}

	csreq = 0;
#endif
}

pISR oldTimer; // stara prekidna rutina

// postavlja novu prekidnu rutinu
void setup(){
#ifndef BCC_BLOCK_IGNORE
	oldTimer = getvect(0x8);
	setvect(0x8, timer);
	setvect(0x60, oldTimer);
#endif
}

void restore(){
#ifndef BCC_BLOCK_IGNORE
	setvect(0x8, oldTimer);
#endif
}

int main(int argc, char* argv[]){
	placeholder = new IdleThr();
	sleeping = new SleepList();
	masterPCB=new PCB(0,defaultStackSize,1);
	PCB::running=masterPCB;
	//PCB::running = PCB::getPCB(placeholder);

	MainThr::mainThr = new MainThr();
	MainThr::MTargc = argc;
	MainThr::MTargv = argv;
	setup();
	MainThr::mainThr->start();
	delete MainThr::mainThr;
	restore();
	delete placeholder;
	return MainThr::retval;
}
