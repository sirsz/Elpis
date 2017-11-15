#include "StdAfx.h"
#include "MyTab.h"

CMyTab::CMyTab(void)
: tabi(NULL)
, tablengthI(0)
, sumI(0)
{
}

CMyTab::~CMyTab(void)
{
	Destroy();
}

int CMyTab::Destroy(void)
{
	if(tabi)
		delete [] tabi;
	tabi = NULL;

	return 0;
}

int CMyTab::Create(int length)
{
	Destroy();

	if(length < 0)
		length = 0;
	tabi = new int [length + 1];

	for(int i=0; i<=length; i++)
		tabi[i] = 0;

	tablengthI = length;

	sumI = 0;

	return 0;
}

int CMyTab::CountSumI(void)
{
	sumI = 0;
	for(int i=0; i<tablengthI ; i++)
		sumI += tabi[i];

	return 0;
}

int CMyTab::GetRand(double rand_d)
{
	int rand_i = int( double(sumI)*rand_d );
	int sum = 0;

	for(int i=0; i<tablengthI; i++)
	{
		sum += tabi[i];
		if(rand_i <= sum)
		return	i;
	}

	return 0;
}
