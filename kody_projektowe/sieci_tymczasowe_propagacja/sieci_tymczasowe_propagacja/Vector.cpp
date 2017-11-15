#include "stdafx.h"
#include ".\vector.h"

CVector::CVector(void)
: MaxValue(0)
{
	elements = NULL;
	length = 0;
	n = 0;
}

CVector::~CVector(void)
{
	if(elements != NULL)
		delete [] elements;

	elements = NULL;
	length = 0;
	n = 0;
}

int CVector::Insert(int num, int value)
{
	int * tab = NULL;
	if( num < 0 ) return 0;
	
	if(num < length)
	{

	}


	return 1;
}


int CVector::Add(int value)
{
	int * tab;
	
	if(n == 0) MaxValue = value;
	else
		if(value > MaxValue)	MaxValue = value;
	
	if(n < length && elements != NULL)
	{
		elements[n] = value;
		n++;
	}
	else
	{
		length += 10;
		length *= 2;
		tab = new int [length];
		
		for(int i=0; i<n; i++)
			tab[i] = elements[i];

		if(elements != NULL) delete [] elements;
		elements = tab;
		
		elements[n] = value;
		n++;
	}


	return 1;
	
}

int CVector::Clear(void)
{
	for(int i=0; i<length; i++)
		elements[i] = 0;

	n = 0;

	return 0;
}

double CVector::GetAverageValue(bool with_max)
{
	int average = 0;
	int mianownik = 1;
	
	for(int i=0; i<n; i++)
		average += elements[i];

	if(!with_max)
	{
		average -= MaxValue;
		mianownik = n - 1;
	}
	else
		mianownik = n;
	
	double wynik = 0;

	if(mianownik > 0)
		wynik = double(average)/double(mianownik);

	return wynik;
}

double CVector::GetSav(bool with_max)
{
	double licznik = 0;
	double mianownik = 0;

	for(int i=0; i<n; i++)
	{
		licznik += double(elements[i])*double(elements[i]);
		mianownik += double(elements[i]);
	}

	if(!with_max)
	{
		licznik -= MaxValue * MaxValue;
		mianownik -= MaxValue;
	}

	double wynik = 0;
	if(mianownik > 0 && licznik > 0) wynik = licznik/mianownik;
	return wynik;
}
