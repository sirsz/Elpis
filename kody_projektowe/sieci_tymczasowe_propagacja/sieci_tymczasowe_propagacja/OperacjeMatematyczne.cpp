#include "StdAfx.h"
#include "OperacjeMatematyczne.h"
#include <Math.h>

OperacjeMatematyczne::OperacjeMatematyczne(void)
{
}

OperacjeMatematyczne::~OperacjeMatematyczne(void)
{
}

int OperacjeMatematyczne::NajmniejszeKwadraty(double * daneX, double * daneY, int LiczbaDanych, double * a, double * bladA, double * b, double * bladB)
{
	double srednieX = 0;
	double srednieY = 0;

	
	// liczymy wartosci srednie
	for(int i=0; i<LiczbaDanych; i++)
	{
		srednieX += daneX[i];
		srednieY += daneY[i];
	}

	srednieX /= double(LiczbaDanych);
	srednieY /= double(LiczbaDanych);
	

	// liczymy wspolczynniki
	double S = 0;
	double Sx = 0;
	double Sy = 0;
	double Sxx = 0;
	double Syy = 0;
	double Sxy = 0;

	for(int i=0; i<LiczbaDanych; i++)
	{
		S += 1;
		Sx += daneX[i];
		Sy += daneY[i];
		Sxx += daneX[i]*daneX[i];
		Syy += daneY[i]*daneY[i];
		Sxy += daneX[i]*daneY[i];
	}

	double Delta = S*Sxx - Sx*Sx;

	(*a) = (S*Sxy - Sx*Sy)/Delta; 
	(*b) = (Sxx*Sy - Sx*Sxy)/Delta;

	double sigmay = Syy - (*a)*Sxy - (*b)*Sy;
	
	(*bladA) =  (S/(S-2))*(sigmay/Delta);
	(*bladB) = (*bladA)*Sxx/S;

	(*bladA) = sqrt((*bladA));
	(*bladB) = sqrt((*bladB));

	
	return 0;
}
