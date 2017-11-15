#pragma once
#include "universalnetworkclass.h"
#include "mytab.h"

// opis cech wezla
#define MAXK			0
#define AGE				1
#define WASINFECTED		2
#define	STATE			3
#define	NEXTSTATE			4
#define TEMPCONNECTIONS1	5
#define TEMPCONNECTIONS2	6
#define TEMPCONNECTIONS_MAX1	7
#define TEMPCONNECTIONS_MAX2	8
#define LIFESPAN				9
// liczba cech wezla
#define	LICZBACECHMAX	10



// rodzaje kontaktow
#define WSZYSTKIE	0
#define STALE		1
#define SPORADYCZNE	2
#define	PRZYPADKOWE	3

// stany wezla
#define	SUSCEPTIBLE		0
#define	INFECTED		1
#define	RESISTED		2

// PLOTKI
#define	IGNORANT		0
#define	SPREADER		1
#define	STIFLER			2

#include <vector>

class CNetworkPOLYMOD :
	public CUniversalNetwork
{
public:
	CNetworkPOLYMOD(void);
	~CNetworkPOLYMOD(void);
	int BudujSiecPM(void);
	int NextStepPM(void);
	int NextStepADepidemic(void);
	int SymulacjaPM(void);
	int MaksymalnaLiczbaPolaczenPM;
	int RozmiarSieciPM;
	int TworzPolaczeniaPM(void);
	int TworzPolaczeniaAgeDegree(void);
	int LosujWiekPM(void);
	int LosujMaxK_PM(int id, int age);
	float LosujWagePolaczeniaPM(int id);
	int LosujWiekSasiadaPM(int nodeID, int typ);
	int ProbujPolaczyc(int from, int to, bool deeper);
	int ProbujPolaczycAgeDegree(int from, int to, bool deeper);
	int WczytajDanePM(void);
	int WczytajDaneAgeDegree(void);
	CMyTab mtPodK[4];	// 4 rodzaje kontaktow: wszystkie, stale, sporadyczne, przypadkowe
	CMyTab mtPodW[4];
	CMyTab mtPodAK[4][20];
	CMyTab mtPodAA[4][20];
	CMyTab mtAge;
	std::vector<int> m_vecIdAge;
	std::vector<std::vector<int>> m_vecIdContactAge;
	float waga_polaczenia[10];
	// w kazdym kroku tworzy polaczenia sporadyczne z sasiadami sasiadow
	int PrzylaczDoSasiadowPM(int id);
	int DodajPolaczeniePrzypadkowe(int from, int to);
	int DodajPolaczenieSporadyczne(int from, int to);
	int TworzPolaczeniaTymczasowePM(void);
	double PstwoItoR;
	double PstwoStoI;
	int stepPM;
	int MaxLiczbakrokow;
	int NumberInfected;
	int NumberInfectedMax;
	int TimeNumberInfectedMax;
	int InitiallyInfected;
	bool m_bWiekWezla;
	bool m_bSiecDynamiczna;
	int StaleWszystkie;
	double m_dSredniaWaga;
	bool m_boolSredniaWaga;
	int m_iLiczbaPowtorzenPM;
	int UruchomSymulacje(void);
	int TworzPolaczeniaTymczasoweSporadycznePM(void);
	int TworzPolaczeniaTymczasowePrzypadkowePM(void);
	int m_iZasieg;
	float PstwoSzczepien;
	float m_fMagnetyzacja;

	// rumor propgataion
	float IgnorantSpreader;
	float SpreaderStifler;
	int UruchomSymulacjeRumour(void);
	int NextStepPM_Rumour(void);
	int SymulacjaPM_RumorPropagation(void);

	// ISING
	float Temperatura;
	float PoleZewnetrzne;
	int UruchomSymulacjeIsing(void);
	int UruchomSymulacjeAgeDegreeEpidemie(void);
	int NextStepPM_Ising(void);
	int SymulacjaPM_Ising(void);
	int SymulacjaADEpidemie(void);
	void WriteToLog(char * txt);
	float * m_fMagnetyzacjaTab;
	float * m_fMagetyzacjaCzas;
	HANDLE m_hLogFile;
	HANDLE m_hLog;
	

	// aktywacja i dezaktywacja wezlow
	// ze wzgledu na typ kontaktu oraz wagi
	int aktywacjaWezla[10][10];
	int dezaktywacjaWezla[10][10];
	int aktywacjaWezlaTemp[10][10];
	int dezaktywacjaWezlaTemp[10][10];

	float VaccinatedNodesAverageK;
	int NumberOfVaccinated;

	int m_iLifeSpan;
	float m_fLifeSpan;

	HANDLE lTrollOutputFile;
	void lWriteToLog(char *tekst);

	std::vector<int> vecAgeConnectionAge[100][100];
	int aacSize[100][100];
	int TworzPolaczenieAgeDegree(int n, int n2, int a, int ac);
	int m_iAgeInfection[200]; // liczba wezlow zainfekowanych przez wezel o wieku Age
	double m_dAgeInfection[200]; // odsetek wezlow zainfekowanych przez wezel o wieku Age
	int m_iAgeInfected[200]; // liczba wezlow o wieku Age, ktore zostaly zainfekowane
	int m_iAgeCount[200];	// liczba wezlow o wieku Age
	double m_dAgeInfected[200]; // odsetek wezlow o wieku age, ktore zostaly zainfekowane
	int m_iSourceAge;

	int UruchomSymulacjeAgeDegreeIsing(void);
	int SymulacjaADIsing(void);
	int NextStepADIsing(void);

	// ising test
	bool m_bIsingTest;

};
