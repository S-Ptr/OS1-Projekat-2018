#include "IVTEntry.h"
#include <dos.h>
#include "KernelEv.h"

IVTEntry* IVTEntry::IVTref[256] = {0};

IVTEntry::IVTEntry(unsigned char _entryNo, pISR newEntry):entryNo(_entryNo){
#ifndef BCC_BLOCK_IGNORE
	oldEntry = getvect(entryNo);
	setvect(entryNo, newEntry);
	IVTref[entryNo] = this;
#endif
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	setvect(entryNo, oldEntry);
#endif
	myEv = 0;
}

void IVTEntry::callOldISR(){
	(*oldEntry)();
}

void IVTEntry::signal(){
	if(myEv){
		myEv->signal();
	}
}

IVTEntry* IVTEntry::fetchEntry(unsigned char ivtNo){
	return IVTref[ivtNo];
}
