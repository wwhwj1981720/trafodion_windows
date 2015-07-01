#include "StdAfx.h"
#include "RowBean.h"


RowBean::RowBean(void)
{
	/*p=(RowObject *) malloc (M*sizeof(RowObject));
	this->createRowObject(p);*/
	p=new RowObject[M];
	setRowObject();
	
	
}


RowBean::~RowBean(void)
{	
	//free(this->p);
	delete[]p;
}
/*
void RowBean::createRowObject(RowObject *p)
{
	int i=0;
	for(;i<M ;i++)
	{
		createTimeFirst(p[i].timefirst);
		strcpy(p[i].type,"01");
		createDateString(p[i].timenow);
		createDateString(p[i].timediff);
		createMobile(p[i].mobile);
		createMobile(p[i].mobileout);
	}
}
*/
void RowBean::setRowObject(void)
{
	int i=0;
	for(;i<M ;i++)
	{
		p[i].setTimeFirst();
		p[i].setType();
		p[i].setTime();
		p[i].setTimeDiff();
		p[i].setMobile();
		p[i].setMobileout();
		p[i].setLac();
		p[i].setCi();
		p[i].setEmei();
		p[i].setEmsi();
	}
}
