#pragma once

class CVector
{
public:

	int length;		// dlugosc tablicy
	int n;			// liczba elementow
	int * elements;	// tablica elementow

public:
	CVector(void);
	~CVector(void);
	int Insert(int num, int value);
	int Add(int value);
	int Clear(void);
	int MaxValue;
	double GetAverageValue(bool with_max);
	double GetSav(bool with_max);
};



struct s_data
{
	int rok;
	int miesiac;
	int dzien;
	int godzina;
	int minuta;
};