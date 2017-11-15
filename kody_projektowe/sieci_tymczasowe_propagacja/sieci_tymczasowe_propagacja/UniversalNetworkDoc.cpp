// UniversalNetworkDoc.cpp : implementation of the CUniversalNetworkDoc class
//

#include "stdafx.h"
#include "UniversalNetwork.h"
#include "NetworkPOLYMOD.h"
#include "Math.h"

#include "UniversalNetworkDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUniversalNetworkDoc

IMPLEMENT_DYNCREATE(CUniversalNetworkDoc, CDocument)

BEGIN_MESSAGE_MAP(CUniversalNetworkDoc, CDocument)
	ON_COMMAND(ID_PERCOLATION_SIMULATION, &CUniversalNetworkDoc::OnPercolationSimulation)
	ON_COMMAND(ID_PERCOLATION_SIMULATION32772, &CUniversalNetworkDoc::OnPercolationSimulationSite)
	ON_COMMAND(ID_TEST_TEST, &CUniversalNetworkDoc::OnTestTest)
	ON_COMMAND(ID_TEST_CONVERTTONETWORK, &CUniversalNetworkDoc::OnTestConvertToNetwork)
	ON_COMMAND(ID_TEST_BADAJSIECSKYROCK, &CUniversalNetworkDoc::OnTestBadajSiecSkyrock)
	ON_COMMAND(ID_POLYMOD_TEST, &CUniversalNetworkDoc::OnPolymodTest)
	ON_COMMAND(ID_POLYMOD_SYMULACJA, &CUniversalNetworkDoc::OnPolymodSymulacja)
	ON_COMMAND(ID_AGEDEGREE_SYMULACJA, &CUniversalNetworkDoc::OnAgedegreeSymulacja)
	ON_COMMAND(ID_AGEDEGREE_TEST, &CUniversalNetworkDoc::OnAgedegreeTest)
END_MESSAGE_MAP()


// CUniversalNetworkDoc construction/destruction

CUniversalNetworkDoc::CUniversalNetworkDoc()
{
	// TODO: add one-time construction code here

}

CUniversalNetworkDoc::~CUniversalNetworkDoc()
{
}

BOOL CUniversalNetworkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CUniversalNetworkDoc serialization

void CUniversalNetworkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CUniversalNetworkDoc diagnostics

#ifdef _DEBUG
void CUniversalNetworkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUniversalNetworkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUniversalNetworkDoc commands

void CUniversalNetworkDoc::OnPercolationSimulation()
{
	CUniversalNetwork net;

	net.PercoaltionSimulation(BOND_PERCOLATION);
}

void CUniversalNetworkDoc::OnPercolationSimulationSite()
{ 
	CUniversalNetwork net;

	net.PercoaltionSimulation(SITE_PERCOLATION);
}

void CUniversalNetworkDoc::OnTestTest()
{
	CUniversalNetwork net;
	
	HANDLE plik = CreateFile("test.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	// jedna cecha int to lifespan
	int N = 100000;//000;
	net.CreateNetwork(N,1,0,5);

	
	// najpierw mamy 3 calkowicie polaczone
	int n = 3;
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
		{
			net.ConnectSafe(i,j);
		}

	int K = 2 * net.CountNumberOfEdges();
	int ActiveNodes = n;

	// prawdopodobienstwo utworzenia polaczenia
	double pstwo = 0.01;
	bool flaga_dodawanie_polaczen = true;

	//aktywni sasiedzi
	int LiczbaAktywnych;
	int max_aktywni = 10000;
	int aktywni_id[10000];

	int liczbanowych = 100;

	int MaxLifeSpan = 1000;//N;///liczbanowych;

	
	for(int i=0; i<n; i++)
		net.Node[i].StateI[0] = this->DrawFromPowerLaw(1.0,1,MaxLifeSpan,net.GetRand()) + 1;

	for(int i=n; i<N; )
	{
		
		for(int ln=0; ln < liczbanowych; ln++)
		if(i < N)
		{
		
			// dodajemy wezel i jedno polaczenie
			net.Node[i].StateI[0] = this->DrawFromPowerLaw(1.0,1,MaxLifeSpan,net.GetRand());
			
			{
				int k = 0;
				int an = 0;

				// zliczamy liczbe krawedzi aktywnych graczy
				for(int j=0; j<i; j++)
					if(net.Node[j].StateI[0] > 0)
					{
						k += net.Node[j].Connectivity; 
						an++;
					}
				
				// laczymy z wezlem aktywnym proporcjonalnie do k
				int num = int( net.GetRand() * double(k) );
				int num2 = int( net.GetRand() * double(an) );
				k = 0;
				an = 0;
				for(int j=0; j<i; j++)
					if(net.Node[j].StateI[0] > 0)
					{
						k += net.Node[j].Connectivity; 
						an++;
						
						//if(k >= num)
						if(an >= num2)
						{
							net.ConnectSafe(i,j);
							j = i + 10;
							break;
						}
					}

				// zwiekszamy liczbe aktywnych wezlow bo dodalismy jeden nowy
				ActiveNodes++;
			}

			// nastepna jednostka
			i++;
		}

			

		// zwiekszamy liczbe polaczen: tylko dla aktywnych
		if(flaga_dodawanie_polaczen)
		for(int j=0; j<i-1; j++)
			if(net.Node[j].StateI[0] > 0)
			if(net.GetRand() < pstwo / double(net.Node[j].Connectivity) )
			{
				// robimy liste aktywnych sasiadow aktywnych sasiadow
				LiczbaAktywnych = 0;
				
				// petla po sasiadach
				for(int k=0; k < net.Node[j].Connectivity; k++)
				{
					int target = net.Node[j].Connection[k];
					if(net.Node[target].StateI[0] > 0)
					{
						// sasiedzi sasiadow
						for(int kk=0; kk < net.Node[target].Connectivity; kk++)
						{
							int nn = net.Node[target].Connection[kk];
							if(net.Node[nn].StateI[0] > 0)
							{
								if( LiczbaAktywnych < max_aktywni )
								{
									aktywni_id[LiczbaAktywnych] = nn;
									LiczbaAktywnych++;
								}
							}
						}

					}
				}


				int rand_neighbour = int( net.GetRand() * double(LiczbaAktywnych) );
				int target = aktywni_id[rand_neighbour];
				net.ConnectSafe(j,target);
			}


		// aktualizujemy stan wezlow
		
		for(int j=0; j<i-1; j++)
		if(net.Node[j].StateI[0] > -10)
		{
			net.Node[j].StateI[0]--;
			if(net.Node[j].StateI[0] == 0)
			{
				
				//while(net.Node[j].Connectivity > 0)
				//	net.RemoveConnection(j,net.Node[j].Connection[0]);
				
				// K -= 2 * net.Node[j].Connectivity;
				

				//K -= net.Node[j].Connectivity;
				ActiveNodes --;
			}
		}
		
		
	}

	// zapisujemy rozklad liczby polaczen
	for(int i=0; i<N; i++)
		net.Node[i].TempStateI = net.Node[i].Connectivity;
	net.ZapiszRozkladInt(plik);

	/*
	//net.LoadNetworkWithActivity();
	net.LoadNetworkWithActivity();
	net.SaveNetworkConnections("allseron_percolation.dat");

	net.LoadNetworkConnections("network.dat");
	net.PercolateBond(2.0f);
	net.LiczZmodyfikowanyWspolczynnikKlasteryzacji();
	net.ZapiszZaleznoscFloatOdConnectivity(plik);
	*/

	CloseHandle(plik);
}

void CUniversalNetworkDoc::OnTestConvertToNetwork()
{
	CUniversalNetwork net;
	net.ConvertToNetwork();
	//net.Load("net.dat");
	//net.SaveNetworkConnections("percolation.dat");
}

void CUniversalNetworkDoc::OnTestBadajSiecSkyrock()
{
	CUniversalNetwork net;
	//net.BadajSiecSkyRock();
	//net.BadajSiecSkyRockDynamika();
	//net.Load("network.dat");
	//net.SaveNetworkConnections("skyrock_percolation.dat");
	net.BadajSiecSkyRockArticle();
	//net.BadajSiecGrono();
}

int CUniversalNetworkDoc::DrawFromPowerLaw(double gamma, int kMin, int kMax, double rand01)
{
	double a,b,potega;
	double temp;
	int k = kMin;

	if(gamma > 1.0f)
	{
		a = 1.0f/powf(double(kMax),gamma-1.0f)-1.0f/powf(double(kMin),gamma-1.0f);
		b = 1.0f/powf(double(kMin),gamma-1.0f);
		potega = 1.0f / (gamma-1.0f);
	}

	if(gamma == 1.0)
	{
		potega = logf(float(kMax)/float(kMin));
	}

	
	
	// generujemy odpowiednia wartosc
	{
		double LiczbaLosowa = rand01;
		
		
		if(gamma>1.0)
		{
			temp = a*LiczbaLosowa + b;
			k = int (1.0f / powf(double(temp),potega) + 0.5f);
		}

		if(gamma == 1.0f)
		{
			temp = expf(double(LiczbaLosowa)*potega);
			k = int ( kMin*temp + 0.5f);
		}
			
	}
	
	return k;
}

void CUniversalNetworkDoc::OnPolymodTest()
{
	HANDLE plik = CreateFile("test.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	char tekst[500];
	char txt[500];
	DWORD ile;
	CNetworkPOLYMOD net;
	unsigned long tab[500];

	net.m_bSiecDynamiczna = true;
	net.RozmiarSieciPM = 1000000;

	sprintf(tekst,"BudujSiecPM\n");
	WriteFile(plik,tekst,strlen(tekst),&ile,NULL);

	net.BudujSiecPM();

	sprintf(tekst,"WczytajDanePM\n");
	WriteFile(plik,tekst,strlen(tekst),&ile,NULL);

	net.WczytajDanePM();

	sprintf(tekst,"TworzPolaczeniaPM\n");
	WriteFile(plik,tekst,strlen(tekst),&ile,NULL);

	net.TworzPolaczeniaPM();

	if(net.m_bSiecDynamiczna)
		{
			// tworzymy polaczenia przypadkowe
			net.TworzPolaczeniaTymczasowePM();
		}
	
	net.PstwoStoI = 1;
	net.PstwoItoR = 0.1;
		
/*
	// badamy wlasciwosci sieci
	// WLASCIWOSCI CALEJ SIECI

	sprintf_s(txt,"\n\n WLASCIWOSCI CALEJ SIECI\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	// P(k)
	sprintf_s(txt,"\n\n P(k) \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<net.NumberNodes; i++)
	{
		net.Node[i].TempStateI = net.Node[i].Connectivity;
	}
	net.ZapiszRozkladInt(plik);
	
	// wspolczynnik klasteryzacji
	sprintf_s(txt,"\n\n C(k) - wspolczynnik klasteryzacji od k \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	double cluster = 0.0;
	for(int i=0; i<net.NumberNodes; i++)
	{
		net.Node[i].TempStateI = net.Node[i].Connectivity;
		net.Node[i].TempStateF = net.LiczWspolczynnikKlasteryzacji(i);
		cluster += double(net.Node[i].TempStateF);
	}
	sprintf_s(txt,"C = \t %lf \n",cluster/double(net.NumberNodes));
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	net.ZapiszZaleznoscFOdI(plik);


	// sredni stopien najblizszych sasiadow
	sprintf_s(txt,"\n\n k_NN(k) - sredni stopien najblizszych sasiadow od k \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<net.NumberNodes; i++)
	{
		net.Node[i].TempStateI = net.Node[i].Connectivity;
		net.Node[i].TempStateF = net.LiczSredniaLiczbePolaczenSasiadow(i);
	}
	net.ZapiszZaleznoscFOdI(plik);

	// Komponenty
	sprintf_s(txt,"\n\n komponenty \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	net.SeekComponents();
	net.ZapiszSkladoweSieci(plik);
	sprintf_s(txt,"\n koniec komponentow \n\n\n");

	//
*/

	/*
	net.MaxLiczbakrokow = 0;
	net.InitiallyInfected = 10;
	net.SymulacjaPM();


	for(int i =0; i<100; i++)
	{
		int inf = 0;
		int sus = 0;
		int res = 0;
		for(int n=0; n<net.NumberNodes; n++)
		{
			switch(net.Node[n].StateI[STATE])
			{
				case SUSCEPTIBLE:	sus++; break;
				case INFECTED:		inf++; break;
				case RESISTED:		res++; break;
			}
		}
		
		sprintf(tekst,"%d\t%d \t\t %d \t %d \t %d \n",i,net.NumberInfected, sus, inf, res);
		WriteFile(plik,tekst,strlen(tekst),&ile,NULL);
		net.NextStepPM();
	}
*/

	
	
	// zliczmy rozklad liczby kontaktow
	sprintf(tekst,"zerowanie\n");
	WriteFile(plik,tekst,strlen(tekst),&ile,NULL);

	for(int i =0; i<500; i++)	tab[i] = 0;

	
	sprintf(tekst,"zliczanie\n");
	WriteFile(plik,tekst,strlen(tekst),&ile,NULL);

	for(int i =0; i<net.NumberNodes; i++)
		{
			int kall = net.Node[i].Connectivity + net.Node[i].StateI[TEMPCONNECTIONS1] + net.Node[i].StateI[TEMPCONNECTIONS2];
			if(kall >=0 && kall<500)
				tab[ kall ] ++;
		}
	
	for(int i =0; i<500; i++)
	if(tab[i] != 0)
	{
		sprintf(tekst,"%d\t%d\t%lf\n",i,tab[i],double(tab[i])/double(net.RozmiarSieciPM));
		WriteFile(plik,tekst,strlen(tekst),&ile,NULL);
	}


	/*
	for(int i =0; i<500; i++)	tab[i] = 0;
	for(int i =0; i<net.NumberNodes; i++)
		tab[ net.Node[i].StateI[TEMPCONNECTIONS1] ] ++;
	for(int i =0; i<500; i++)
	if(tab[i] != 0)
	{
		sprintf(tekst,"%d\t%d\n",i,tab[i]);
		WriteFile(plik,tekst,strlen(tekst),&ile,NULL);
	}


	for(int i =0; i<500; i++)	tab[i] = 0;
	for(int i =0; i<net.NumberNodes; i++)
		tab[ net.Node[i].StateI[TEMPCONNECTIONS2] ] ++;
	for(int i =0; i<500; i++)
	if(tab[i] != 0)
	{
		sprintf(tekst,"%d\t%d\n",i,tab[i]);
		WriteFile(plik,tekst,strlen(tekst),&ile,NULL);
	}
	*/


	
	
	

	CloseHandle(plik);
}

void CUniversalNetworkDoc::OnPolymodSymulacja()
{
	CNetworkPOLYMOD net;
	net.m_iLiczbaPowtorzenPM = 1000;
	net.RozmiarSieciPM = 10000;
	net.PstwoItoR = 1.0;
	net.MaxLiczbakrokow = 100000;
	net.InitiallyInfected = 1;

	
	net.m_bSiecDynamiczna = false;
	net.m_boolSredniaWaga = true;

	net.m_bIsingTest = true;

	net.BudujSiecPM();
	net.WczytajDanePM();
	//net.UruchomSymulacjeRumour();
	net.UruchomSymulacjeIsing();
	//net.UruchomSymulacje();
	
	/*

	// ZAPIS DANYCH: <C> i <l>
	net.TworzPolaczeniaPM();

	char text[500];
	DWORD ile;
	HANDLE plik = CreateFile("network_data_C_L.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	double C = net.LiczWspKlasteryzacjiSieci();
	sprintf(text,"C = %lf\n",C);
	WriteFile(plik,text,strlen(text),&ile,NULL);

	double L = net.LiczSredniaDroge(-1,plik);
	sprintf(text,"L = %lf\n",L);
	WriteFile(plik,text,strlen(text),&ile,NULL);

	CloseHandle(plik);

	*/
	
}


void CUniversalNetworkDoc::OnAgedegreeSymulacja()
{
	CNetworkPOLYMOD net;
	net.m_iLiczbaPowtorzenPM = 100;
	net.RozmiarSieciPM = 100000;
	net.PstwoItoR = 1.0;
	net.MaxLiczbakrokow = 1000;
	net.InitiallyInfected = 1;

	net.m_bWiekWezla = true;

	net.BudujSiecPM();
	net.WczytajDaneAgeDegree();

	net.UruchomSymulacjeAgeDegreeIsing();

	
}


void CUniversalNetworkDoc::OnAgedegreeTest()
{
	HANDLE plik = CreateFile("test.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	char tekst[500];
	char txt[500];
	DWORD ile;
	
	CNetworkPOLYMOD net;
	net.m_iLiczbaPowtorzenPM = 100;
	net.RozmiarSieciPM = 100000;
	net.PstwoItoR = 1.0;
	net.MaxLiczbakrokow = 10000;
	net.InitiallyInfected = 1;

	net.m_bWiekWezla = false;
	net.m_bIsingTest = true;

	net.BudujSiecPM();
	net.WczytajDaneAgeDegree();
	
	net.UruchomSymulacjeAgeDegreeIsing();


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	net.TworzPolaczeniaAgeDegree();

		
	// badamy wlasciwosci sieci
	// WLASCIWOSCI CALEJ SIECI

	sprintf(txt,"\n\n WLASCIWOSCI CALEJ SIECI\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	// P(k)
	sprintf_s(txt,"\n\n P(k) \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<net.NumberNodes; i++)
	{
		net.Node[i].TempStateI = net.Node[i].Connectivity;
	}
	net.ZapiszRozkladInt(plik);
	
	// wspolczynnik klasteryzacji
	sprintf_s(txt,"\n\n C(k) - wspolczynnik klasteryzacji od k \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	double cluster = 0.0;
	for(int i=0; i<net.NumberNodes; i++)
	{
		net.Node[i].TempStateI = net.Node[i].Connectivity;
		net.Node[i].TempStateF = net.LiczWspolczynnikKlasteryzacji(i);
		cluster += double(net.Node[i].TempStateF);
	}
	sprintf_s(txt,"C = \t %lf \n",cluster/double(net.NumberNodes));
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	net.ZapiszZaleznoscFOdI(plik);


	// sredni stopien najblizszych sasiadow
	sprintf_s(txt,"\n\n k_NN(k) - sredni stopien najblizszych sasiadow od k \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<net.NumberNodes; i++)
	{
		net.Node[i].TempStateI = net.Node[i].Connectivity;
		net.Node[i].TempStateF = net.LiczSredniaLiczbePolaczenSasiadow(i);
	}
	net.ZapiszZaleznoscFOdI(plik);

	// Komponenty
	sprintf_s(txt,"\n\n komponenty \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	net.SeekComponents();
	net.ZapiszSkladoweSieci(plik);
	sprintf_s(txt,"\n koniec komponentow \n\n\n");

	//

}
