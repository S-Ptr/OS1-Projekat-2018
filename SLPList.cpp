#include "SLPList.h"
#include "PCB.h"
#include <iostream.h>

SleepList::SleepList(): first(0), temp(0), PCBtemp(0){

}

void SleepList::insert(PCB* asdf, unsigned int time){
	//primer ubacivanja u delta listu:
	//{a,3}->{b,2}->{c,0}->{d,3}->{e,2}
	//insert(x,6);
	//{a,3}->{b,2}->{c,0}->{x,1}->{d,2}->{e,2}
	iter = first;
	if(!iter){ //u slucaju da je lista prazna
		temp = new slpnode(asdf);
		temp->delta = time;
		first = temp;
		return;
	}
	//za prvi cvor u listi
	if(time > iter->delta){
			time -= iter->delta;
	}
	else{
		temp = new slpnode(asdf);
		temp->delta = time;
		temp->next = first;
		first->delta -= time;
		first = temp;
		return;
	}
	//petlja za ostale cvorove
	while(iter->next){
		if(time > iter->next->delta){
			time -= iter->next->delta;
			iter = iter->next;
		}
		else{
			temp = new slpnode(asdf);
			temp->delta = time;
			temp->next = iter->next;
			iter->next->delta -= temp->delta;
			iter->next = temp;
			return;
		}
	}
	//ako novi cvor ima najveci time, ide na kraj
	iter->next = new slpnode(asdf);
	iter->next->delta = time;
}



SleepList::~SleepList(){
	while(first){
		temp = first;
		first = first->next;
		//??
		temp->next = 0;
		temp->data = 0;
		delete temp;
	}
}

void SleepList::prtList(){
	iter=first;
	while(iter){
		cout<<"{,"<<iter->delta<<"}->";
		iter = iter->next;
	}
	cout<<"\n";
}
