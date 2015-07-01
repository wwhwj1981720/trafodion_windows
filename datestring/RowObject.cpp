#include <stdio.h>
#include<time.h>
#include<stdlib.h>
#include<String.h>
#include <windows.h>
#include "RowObject.h"



RowObject::RowObject(void)
{
}
void RowObject::setTimeFirst()
{
	struct tm *ptm = NULL;
	int N=14;
	time_t lt;
	lt = time(NULL);
	ptm = localtime(&lt);
	//(*ptr)=(char *)malloc(N*sizeof(char));
	sprintf((this->timefirst),"%02d%02d%02d", (1900+ptm->tm_year)-2000, ptm->tm_mon+1, ptm->tm_mday);	
}
void RowObject::setType()
{
	strcpy(this->type,"01");
}
void RowObject::setTime()
{
	struct tm *ptm = NULL;
	int N=14;
	time_t lt;
	lt = time(NULL);
	ptm = localtime(&lt);

	sprintf((this->timenow),"%04d%02d%02d%02d%02d%02d", 1900+ptm->tm_year, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);	
}
void RowObject::setTimeDiff()
{
	struct tm *ptm = NULL;
	int N=14;
	time_t lt;
	lt = time(NULL);
	ptm = localtime(&lt);
	sprintf((this->timediff),"%04d%02d%02d%02d%02d%02d", 1900+ptm->tm_year, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);	
}
void RowObject::setMobile()
{
	createMobileInteger(&mobile);
}
void RowObject::setMobileout()
{
	createMobileInteger(&mobileout);
}

RowObject::~RowObject(void)
{
}



void RowObject::setLac(void)
{
	createLacInteger(&lac);
} 


void RowObject::setCi(void)
{	
	createCiInteger(&ci);
}


void RowObject::setEmei(void)
{
	createEmei(this->emei);
}


void RowObject::setEmsi(void)
{
	createEmsi(this->emsi);
}
