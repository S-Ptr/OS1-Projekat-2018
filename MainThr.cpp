#include "MainThr.h"
#include <iostream.h>

int MainThr::MTargc = 0;
char** MainThr::MTargv = 0;
int MainThr::retval = 5;
MainThr* MainThr::mainThr = 0;

void MainThr::run(){
	retval = userMain(MTargc, MTargv);
}


