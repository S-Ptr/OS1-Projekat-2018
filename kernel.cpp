#include "kernel.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include "IdleThr.h"
#include "SLPList.h"

Thread* placeholder;
SleepList sleeping;
extern void tick();

//temporary storage
unsigned tsp;
unsigned tss;
unsigned tbp;

volatile unsigned int counter = 9999;
volatile int csreq = 0;

void interrupt timer(){	// prekidna rutina
	if (!csreq) {
		counter--;
		--((sleeping.first)->delta);
		while((sleeping.first)->delta == 0){
			Scheduler::put((sleeping.first)->data);
			sleeping.temp = (sleeping.first)->next;
			(sleeping.first)->next = 0;
			(sleeping.first)->data = 0;
			delete sleeping.first;
			sleeping.first = sleeping.temp;
		}
	}; 
	if (counter == 0 || csreq) {
#ifndef BCC_BLOCK_IGNORE
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;
		//set flag to ready
		PCB::running->flag = 0x1;
		if(PCB::running->myThread != placeholder){
			Scheduler::put(PCB::running);
		}

		PCB::running= Scheduler::get();	// Scheduler
  		
		if(PCB::running == 0){
			PCB::running = placeholder->getPCB();
		}
		PCB::running->flag = 0x2; //set flag to PCB::running

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		counter = PCB::running->kvant;
		//set flag to PCB::running
		PCB::running->flag = 0x2;
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
		asm int 60h
	}
#endif
	csreq = 0;
}

unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

// postavlja novu prekidnu rutinu
void setup(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax	
		mov ax, word ptr es:0020h	
		mov word ptr oldTimerOFF, ax	

		mov word ptr es:0022h, seg timer	 //postavlja 
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu	
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
#endif
}

void restore(){
#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
#endif
}

extern int userMain(int argc, char* argv[]);
int returnval;

int main (int argc, char* argv[]){
	setup();
	placeholder = new IdleThr();


	//??
	returnval = userMain(argc, argv);

	restore();
	delete placeholder;

	return returnval;
}
