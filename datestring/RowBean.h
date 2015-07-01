#pragma once
#define M 1000
#define L 40
class RowBean
{
public:
	RowObject *p;
	RowBean(void);
	void createRowObject(RowObject *p);
	~RowBean(void);
	void setRowObject(void);
};

