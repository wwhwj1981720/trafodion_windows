#pragma once
#include "globalFunction.h"
class RowObject
{
public:
	char timefirst[7];
	char  type[3];
	char  timenow[15];
	char  timediff[15];
	/*char  mobile[12];
	char  mobileout[12];*/
	long  mobile;
	long  mobileout;
	int   lac;
	int   ci;
	char  emei[16];
	char  emsi[16];
	RowObject(void);
	void setTimeFirst();
	void setType();
	void setTime();
	void setTimeDiff();
	void setMobile();
	void setMobileout();
	~RowObject(void);
	void setLac(void);
	void setCi(void);
	void setEmei(void);
	void setEmsi(void);
};

