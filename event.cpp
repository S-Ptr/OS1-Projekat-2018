#include "event.h"
#include "KernelEv.h"
#include "PCB.h"


Event::Event(IVTNo ivtNo){
	myImpl = new KernelEv(ivtNo, PCB::running);
}

Event::~Event(){
	delete myImpl;
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}
