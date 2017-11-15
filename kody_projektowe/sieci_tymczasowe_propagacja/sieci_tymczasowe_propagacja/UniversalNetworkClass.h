#pragma once

#include "UniversalNode.h"
#include "Vector.h"
#include "Connections.h"
#include "OperacjeMatematyczne.h"
#include "MLE.h"




// dla SkyRock
#define DSTART		0		// data dodania do grona	
#define DSTOP		1		// data pierwszego logowania
#define ALLK		2		// Liczba wszystkich polaczen
#define DOWNLOADDATE	3	// data wczytania danych z sieci
#define NUMBERVISITS	4	// liczba wizyt na stronie

#define DSTARTBLOG			5	// 
#define DSTOPBLOG			6	// 
#define DOWNLOADDATEBLOG	7	// data wczytania danych z sieci
#define NUMBERVISITSBLOG	8	// liczba wizyt na stronie
#define NUMBERVISITSBLOGMONTH	9	// liczba wizyt na stronie
#define NUMBERARTICLESBLOG		10 // liczba arytykulow w blogu
#define NUMBERCOMMENTSBLOG		11	// liczba komentarzy do bloga



#define	GRONO_NDAYS		4		// life-span		
#define GRONO_DSTART	5		// data dodania do grona	
#define GRONO_DSTOP		6		// data pierwszego logowania



#define BOND_PERCOLATION	0
#define SITE_PERCOLATION	1

class CUniversalNetwork
{
public:
	CUniversalNetwork(void);
	~CUniversalNetwork(void);
	int NumberNodes;
	int NumberConnections;
	CUniversalNode *Node;
	CVector Komponenty; // skladowe sieci
	CConnections Connections;


	// dane opisujace wezel
	int NumberFeaturesI;
	int NumberFeaturesF;
	int StartConnectivity;

	// inicjalizacja generatora liczb losowych
	void rstart(int i, int j, int k, int l);
	// zmienne generatora
	double uu[97];
	int ip;
	int jp;
	double cc;
	double cd;
	double cm;
	
	
	int CreateNetwork(int N, int LiczbaCechInt, int LiczbaCechFloat, int PoczatkowaLiczbaPolaczen);
	int DestroyNetwork(void);
	int ZapiszSkladoweSieci(HANDLE plik);
	int SeekComponents(void);
	int SeekComponentsFast(void);
	void ConnectFast(int From, int To);
	void ConnectSafe(int From, int To);
	double GetRand(void);
	int PercolateBond(double Probability);
	int PercolateSite(double Probability);
	int LoadNetworkConnections(const char * filename);
	int SaveNetworkConnections(const char * filename);
	int PercoaltionSimulation(int percolationtype = BOND_PERCOLATION);
	double GetKappa(void);
	int CountNumberOfEdgesInCluster(int cluster);
	int CountNumberOfEdges(void);
	int CountMaxDistanceInCluster(int cluster);
	int CountTau(double * a, double * da);
	int LoadNetworkWithActivity(void);
	int LiczZmodyfikowanyWspolczynnikKlasteryzacji(void);
	int LiczbLiczbePolaczenPomiedzySasiadami(int num);
	void Sortuj(int * tab, int dlugosc);
	int ZapiszZaleznoscIntOdConnectivity(HANDLE plik);
	int ZapiszZaleznoscFloatOdConnectivity(HANDLE plik);
	int ZapiszZaleznoscFOdI(HANDLE plik);
	void ConvertToNetwork(void);
	int ReadInt(HANDLE plik, bool * endfile);
	void Save(char * txt);
	void Load(char * txt);
	// liczy i zapisuje do pliku dystrybucje TempStateInt
	void ZapiszRozkladInt(HANDLE plik);
	void BadajSiecSkyRock(void);
	int RemoveConnection(int from, int to);
	float LiczWspolczynnikKlasteryzacji(int num);
	float LiczSredniaLiczbePolaczenSasiadow(int num);
	int BadajSiecSkyRockArticle(void);
	int BadajSiecSkyRockDynamika(void);
	int BadajSiecGrono(void);
	float LiczWspKlasteryzacjiWezla(int id);
	double LiczWspKlasteryzacjiSieci(void);
	float LiczSredniaDroge(int giantcomponent, HANDLE plik);
	};
