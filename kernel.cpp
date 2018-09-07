#include "kernel.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include "IdleThr.h"

volatile PCB* running;
volatile Thread* placeholder = new IdleThr();

//temporary storage
unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int counter = 20;
volatile int csreq = 0;

void interrupt timer(){	// prekidna rutina
	if (!cseq) brojac--; 
	if (brojac == 0 || csreq) {
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
  		
		if(running == 0){
			running = placeholder->myPCB;
		}

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
	if(!csreq) asm int 60h;
		                                              
	csreq = 0;
}

unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

// postavlja novu prekidnu rutinu
void setup(){
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
}

void restore(){
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
}

extern int userMain(int argc, char* argv[]);
int returnval;

int main (int argc, char* argv[]);{
	setup();

	//??
	returnval = userMain(argc, argv);

	restore();

	return returnval;
}
