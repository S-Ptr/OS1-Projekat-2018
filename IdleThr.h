#ifndef __IDLETHR_H_
#define __IDLETHR_H_
#include "Thread.h"

class IdleThr: public Thread{
public:
	void run(){
		while(1){
		}
	}
	IdleThr():Thread(Thread::defaultStackSize, 1){}
};

#endif