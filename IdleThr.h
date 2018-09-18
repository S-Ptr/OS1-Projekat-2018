#ifndef __IDLETHR_H_
#define __IDLETHR_H_
#include "Thread.h"
#include <iostream.h>
#include "defines.h"

class IdleThr: public Thread{
public:
	volatile int something;
	void run();
	IdleThr():Thread(defaultStackSize, 1), something(1){}
};

#endif
