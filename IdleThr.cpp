#include "IdleThr.h"

void IdleThr::run(){
#ifndef BCC_BLOCK_IGNORE
	//while(1);
	//while(something);
	itrf:
	asm{
		hlt
		jmp itrf
	}
#endif
}
