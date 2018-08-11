#include "kernel.h"
#include "SCHEDULE.H"
#include "PCB.h"

volatile PCB* running;

//temporary storage
unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int brojac = 20;
volatile int zahtevana_promena_konteksta = 0;

void interrupt timer(){	// prekidna rutina
	if (!zahtevana_promena_konteksta) brojac--; 
	if (brojac == 0 || zahtevana_promena_konteksta) {
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		running->sp = tsp;
		running->ss = tss;
		running->bp = tbp;
		//set flag to ready
		running->flag = 0x1;

		running= Scheduler::get();	// Scheduler
  
		tsp = running->sp;
		tss = running->ss;
		tbp = running->bp;

		brojac = running->kvant;
		//set flag to running
		running->flag = 0x2;

		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}
	} 
    
	// poziv stare prekidne rutine koja se 
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida	
	if(!zahtevana_promena_konteksta) asm int 60h;
		                                              
	zahtevana_promena_konteksta = 0;
}