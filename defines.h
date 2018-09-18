#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#define _lock asm pushf; asm cli;
#define _unlock asm popf;

typedef void interrupt(*pISR)(...);

#define PREPAREENTRY(numEntry, callOld) \
	void interrupt inter##numEntry(...); \
	IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
	void interrupt inter##numEntry(...) { \
		newEntry##numEntry.signal(); \
		if (callOld == 1) newEntry##numEntry.callOldRoutine(); \
	}

#endif
