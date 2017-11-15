#pragma once

#define POWERLAW		0
#define EXPONENTIAL		1

class CMLE
{
public:
	int * wartosc;
	int * krotnosc;
	
public:
	CMLE(void);
	~CMLE(void);
	int LiczbaDanych;
	int DlugoscTablic;
	void Analiza(void);
	double LiczZeta(int xmin, double alfa, long int zakres);
	double LiczL(double alpha, int xmin, long zakres, int przesuniecie);
	double LiczBlad(double alpha, int xmin, long zakres);
	double LiczDopasowanie(double alpha, int x, long zakres);
	double LiczWykladnik(double * blad = NULL);
	int DistributionType;
};
