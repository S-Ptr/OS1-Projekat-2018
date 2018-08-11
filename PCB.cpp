#include "PCB.h"
#include "kernel.h"

extern volatile PCB* running;

void PCB::wait2complete(){
	if(running != this){
		
	}
}