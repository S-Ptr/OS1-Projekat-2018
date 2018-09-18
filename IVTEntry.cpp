#include "IVTEntry.h"
#include <dos.h>
#include "KernelEv.h"

IVTEntry** IVTEntry::IVTref = new IVTEntry*[256];

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
}

void IVTEntry::callOldRoutine(){
	(*oldEntry)();
}

void IVTEntry::signal(){
	myEv->signal();
}

IVTEntry* IVTEntry::fetchEntry(unsigned char ivtNo){
	return IVTref[ivtNo];
}
