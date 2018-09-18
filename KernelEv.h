#ifndef _KERNELEV_H_
#define _KERNELEV_H_

typedef unsigned char IVTNo;
class PCB;


class KernelEv{
public:
	KernelEv(IVTNo _ivtNo, PCB* _maker);
	~KernelEv();
	void wait();
	void signal();

	char val;
	IVTNo ivtNo;
	PCB* maker;
	PCB* blocked;

};




#endif /* KERNELEV_H_ */
