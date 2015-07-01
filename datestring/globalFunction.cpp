#include "StdAfx.h"
#include "globalFunction.h"
void createDateString(char * ptr)
{
	struct tm *ptm = NULL;
	int N=14;
	time_t lt;
	lt = time(NULL);
	ptm = localtime(&lt);

	sprintf((ptr),"%04d%02d%02d%02d%02d%02d", 1900+ptm->tm_year, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);	

}
void  createTimeFirst(char * ptr)
{
	struct tm *ptm = NULL;
	int N=14;
	time_t lt;
	lt = time(NULL);
	ptm = localtime(&lt);
	//(*ptr)=(char *)malloc(N*sizeof(char));
	sprintf((ptr),"%02d%02d%02d", (1900+ptm->tm_year)-2000, ptm->tm_mon+1, ptm->tm_mday);	

}
void createMobile(char *mobilestr)
{
	int mobile=0;
	int n=8;
	mobile=createFixedRandom(n);
	char mobilesuffix[9];
	sprintf(mobilesuffix,"%d",mobile);
	char mobiletmp[12];
	strcpy(mobiletmp,"186");
	strcat(mobiletmp,mobilesuffix);
	strncpy(mobilestr,mobiletmp,12);
}
void createMobileInteger(long *mobilestr)
{
	int mobile=0;
	int n=8;
	long base=18600000000;
	mobile=createFixedRandom(n);
	(*mobilestr)=base+mobile;
}
int createSingleRandom(int max)
{
	int r=0;
	r=rand()%max;
	return r;
}
int createFixedRandom (int n)
{
	int sum=0;
	int i=0;
	int r=0;
	for(;i<n;i++)
	{
		sum=sum*10;
		r=createSingleRandom(10);
		sum+=r;
	}
	return sum;
}
void createLacInteger(int *lac)
{
	int lacpre=0;
	int n=2;
	long base=11800;
	lacpre=createFixedRandom(n);
	(*lac)=base+lacpre;
}
void createCiInteger(int *ci)
{
	int cipre=0;
	int n=2;
	long base=22000;
	cipre=createFixedRandom(n);
	(*ci)=base+cipre;
}
void createEmei(char *emeistr)
{
	int emei=0;
	int n=9;
	emei=createFixedRandom(n);
	char emeisuffix[10];
	sprintf(emeisuffix,"%d",emei);
	char emeitmp[16];
	strcpy(emeitmp,"355224");
	strcat(emeitmp,emeisuffix);
	strncpy(emeistr,emeitmp,16);
}
void createEmsi(char *emsistr)
{
	char preemsi[6][8]={"4600105","4600173","4600119","4600111","4600149","4600166"};
	int iprepos=createSingleRandom(6);
	int emsi=0;
	int n=8;
	emsi=createFixedRandom(n);
	char emsisuffix[9];
	sprintf(emsisuffix,"%d",emsi);
	char emsitmp[16];
	strcpy(emsitmp,preemsi[iprepos]);
	strcat(emsitmp,emsisuffix);
	strncpy(emsistr,emsitmp,16);
}