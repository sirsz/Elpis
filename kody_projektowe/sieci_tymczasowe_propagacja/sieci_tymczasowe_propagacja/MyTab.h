#pragma once

class CMyTab
{
public:
	CMyTab(void);
	~CMyTab(void);
	int *tabi;
	int tablengthI;
	int sumI;
	int Destroy(void);
	int Create(int length);
	int CountSumI(void);
	int GetRand(double rand_d);
};
