#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_
#include "defines.h"

class KernelEv;

class IVTEntry{
public:
	IVTEntry(unsigned char _entryNo, pISR newEntry);
	~IVTEntry();
	void callOldISR();
	void signal();
	static IVTEntry* fetchEntry(unsigned char ivtNo);

	pISR oldEntry;
	unsigned char entryNo;
	KernelEv* myEv;
	static IVTEntry* IVTref[256];
};

#endif
