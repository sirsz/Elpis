#include "StdAfx.h"
#include "MLE.h"

#include <Math.h>

CMLE::CMLE(void)
: LiczbaDanych(0)
, DlugoscTablic(0)
, DistributionType(0)
{

	DlugoscTablic = 1000000;
	wartosc = new int [DlugoscTablic];
	krotnosc = new int [DlugoscTablic];
	DistributionType = POWERLAW;
}

CMLE::~CMLE(void)
{
	if(krotnosc)
		delete [] krotnosc;

	if(wartosc)
		delete [] wartosc;
}

void CMLE::Analiza(void)
{
	
	
	DWORD ile;
	char txt[300];
	
	switch(DistributionType)
	{
	case POWERLAW:
		sprintf(txt,"wyniki_powerlaw.txt");
		break;
	case EXPONENTIAL:
		sprintf(txt,"wyniki_exponential.txt");
		break;
	}
	
	HANDLE save = CreateFile(txt,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	long int zakres = 10000000;

	for(int x=0; x<6; x++)
	{
		// ustalamy minx
		int minx = wartosc[x];

		bool flaga = true;

		double alpha = 0.5;
		double maxL = -9999.99;
		double L = 0;
		double bestAlpha = 0;

		// wykonujemy obliczenia
		for(alpha = 0.1; alpha < 5; alpha += 0.5)
		{
			L = LiczL(alpha,minx,zakres,x);

			if(flaga) 
			{
				flaga = false;
				maxL = L;
				bestAlpha = alpha;
			}

			if(L > maxL)
			{
				maxL = L;
				bestAlpha = alpha;
			}
		}

		// przeszukujemy dokladniej
		double alphaprim; 
		alpha = bestAlpha;
		for(alphaprim = alpha - 0.4; alphaprim <= alpha + 0.4; alphaprim += 0.1)
		{
			L = LiczL(alphaprim,minx,zakres,x);
			if(L > maxL)
			{
				maxL = L;
				bestAlpha = alphaprim;
			}
		}

		alpha = bestAlpha;
		for(alphaprim = alpha - 0.09; alphaprim <= alpha + 0.09; alphaprim += 0.01)
		{
			L = LiczL(alphaprim,minx,zakres,x);
			if(L > maxL)
			{
				maxL = L;
				bestAlpha = alphaprim;
			}
		}

		// zapisujemy wyniki
		double sigma = LiczBlad(bestAlpha,minx,zakres);
		double dopasowanie = LiczDopasowanie(bestAlpha,x,zakres);

		sprintf(txt,"%d \t %lf \t %lf \t %lf \t %lf\n",minx,bestAlpha,sigma,dopasowanie,maxL);
		WriteFile(save,txt,strlen(txt),&ile,NULL);
	}


	CloseHandle(save);
}


double CMLE::LiczWykladnik(double * delta)
{
	
	
	DWORD ile;
	double bestAlpha;
	
	
	long int zakres = 100000;

	for(int x=0; x<1; x++)
	{
		// ustalamy minx
		int minx = wartosc[x];

		bool flaga = true;

		double alpha = 0.5;
		double maxL = -99999.99;
		double L = 0;
		bestAlpha = 0;

		// wykonujemy obliczenia
		for(alpha = 1.1; alpha < 9; alpha += 0.5)
		{
			L = LiczL(alpha,minx,zakres,x);

			if(flaga) 
			{
				flaga = false;
				maxL = L;
				bestAlpha = alpha;
			}

			if(L > maxL)
			{
				maxL = L;
				bestAlpha = alpha;
			}
		}

		// przeszukujemy dokladniej
		double alphaprim; 
		alpha = bestAlpha;
		for(alphaprim = alpha - 0.5; alphaprim <= alpha + 0.5; alphaprim += 0.1)
		{
			L = LiczL(alphaprim,minx,zakres,x);
			if(L > maxL)
			{
				maxL = L;
				bestAlpha = alphaprim;
			}
		}

		alpha = bestAlpha;
		for(alphaprim = alpha - 0.09; alphaprim <= alpha + 0.09; alphaprim += 0.01)
		{
			L = LiczL(alphaprim,minx,zakres,x);
			if(L > maxL)
			{
				maxL = L;
				bestAlpha = alphaprim;
			}
		}

		// zapisujemy wyniki
		double sigma = LiczBlad(bestAlpha,minx,zakres);
		//double dopasowanie = LiczDopasowanie(bestAlpha,x,zakres);

		if(delta)
			*delta = sigma;
	}



	return bestAlpha;
}



double CMLE::LiczZeta(int xmin, double alfa,long int zakres)
{
	double wynik = 0;
	
	int granica = 0;
	if(xmin <= 0)
		granica = 1;

	// rozwazamy przypadek nie calkowalny
	//if(alfa <= 1.01 && DistributionType == POWERLAW)
	//{
	//	zakres = wartosc[LiczbaDanych-1];	
	//}
	

	switch(DistributionType)
	{
	case POWERLAW:
		for(long int i = zakres; i>= granica; i--)
			wynik += pow(double(i+xmin),-alfa);
		break;

	case EXPONENTIAL:
		for(long int i = zakres; i>= granica; i--)
			wynik += exp( - alfa * double (i+xmin));
		break;
	}
	
	return wynik;
}

double CMLE::LiczL(double alpha, int xmin, long zakres, int przesuniecie)
{
	double L = 0;
	double zeta = 0;

	zeta = LiczZeta(xmin,alpha,zakres);
	double suma = 0;
	int start = przesuniecie;
	
	if(wartosc[start] == 0) start++;
		


	switch(DistributionType)
	{
	case POWERLAW:
		for(int i = start; i< LiczbaDanych; i++)
		{
			//suma += double(krotnosc[i]) * log( double(wartosc[i]) );
			L += double(krotnosc[i]) * log( pow( double(wartosc[i]), - alpha ) / zeta );
		}
		break;

	case EXPONENTIAL:
		for(int i = start; i< LiczbaDanych; i++)
		{
			//suma += double(krotnosc[i]) * log( double(wartosc[i]) );
			L += double(krotnosc[i]) * log( exp( - alpha * double(wartosc[i]) ) / zeta );
		}
		break;
	}
	


	

	//L = - double(LiczbaDanych - przesuniecie) * log(zeta);
	//L -= alpha * suma;
	
	return L;
}

double CMLE::LiczBlad(double alpha, int xmin, long zakres)
{
	double sigma = 0;
	long int N = 0;

	for(int i=0; i<LiczbaDanych; i++)
		N += krotnosc[i];

	double zeta = LiczZeta(xmin,alpha,zakres);
	double zetaprim = -alpha * LiczZeta(xmin,alpha+1,zakres);
	double zetaprimprim = alpha * alpha * LiczZeta(xmin,alpha+2,zakres);

	zetaprim /= zeta;
	zetaprim *= zetaprim;
	zetaprimprim /= zeta;
	
	sigma = 1 / sqrt( double(N) * (zetaprimprim - zetaprim) );


	return sigma;
}

double CMLE::LiczDopasowanie(double alpha, int x, long zakres)
{
	int minx = wartosc[x];
	double D = 0;
	double dtemp = 0;

	double stemp = 0;
	double ztemp = 0;

	double zeta = LiczZeta(minx,alpha,zakres);
	
	double suma = 0;
	for(int i=x; i<LiczbaDanych; i++)
		suma += double(krotnosc[i]);

	ztemp = zeta;
	stemp = suma;

	//for(int i=0; i<x; i++)
	//	stemp -= double(krotnosc[i]) * double(wartosc[i]);



	for(int i=x; i<LiczbaDanych; i++)
	{
				
		double S = ztemp/zeta;
		double P = stemp/suma;

		dtemp = abs(S-P);
		//if( S < 1.0  &&  i != x)
		//	dtemp /= sqrt( S * (1.0 - S) );

		if(dtemp > D)
			D = dtemp;

		switch(DistributionType)
			{
			case POWERLAW:
				ztemp -= pow(double(wartosc[i]),-alpha);
				stemp -= double(krotnosc[i]) ;
				break;

			case EXPONENTIAL:
				ztemp -= exp(-alpha*double(wartosc[i]));
				stemp -= double(krotnosc[i]) ;
				break;
			}

	}
	
	return D;
}
