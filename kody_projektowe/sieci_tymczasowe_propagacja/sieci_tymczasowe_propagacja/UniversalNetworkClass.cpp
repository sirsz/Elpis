#include "StdAfx.h"
#include "UniversalNetworkClass.h"
#include <Math.h>

CUniversalNetwork::CUniversalNetwork(void)
: NumberNodes(0)
, Node(NULL)
{
	NumberFeaturesI = 0;
	NumberFeaturesF = 0;
	NumberConnections = 0;
	StartConnectivity = 0;



	// zmienne generatora liczb losowych
	ip=97;
	jp=33;
	cc=362436.0/16777216;
	cd=7654321.0/16777216.0;
	cm=16777213.0/16777216.0;

	rstart(10,35,51,78);
	// koniec fragmentu kodu zwiazanego z generatorem liczb losowych
}

CUniversalNetwork::~CUniversalNetwork(void)
{

}

int CUniversalNetwork::CreateNetwork(int N, int LiczbaCechInt, int LiczbaCechFloat, int PoczatkowaLiczbaPolaczen)
{
	if(N < 1) return 0;
	
	DestroyNetwork();

	NumberNodes = N;
	NumberFeaturesI = LiczbaCechInt;
	NumberFeaturesF = LiczbaCechFloat;
	StartConnectivity = PoczatkowaLiczbaPolaczen;

	Node = new CUniversalNode [N];
	for(int n=0; n< NumberNodes; n++)
		Node[n].Create(NumberFeaturesI,NumberFeaturesF,StartConnectivity);

	return 1;
}

int CUniversalNetwork::DestroyNetwork(void)
{
	if(Node)
	{
		delete [] Node;
		Node = NULL;
	}
	
	return 0;
}


int CUniversalNetwork::ZapiszSkladoweSieci(HANDLE plik)
{
	DWORD ile;
	char text[500];
	
	// skladowe
	sprintf_s(text,"\n skladowe sieci \n");
	WriteFile(plik,text,strlen(text),&ile,NULL); 
	
	
	int MaxKonponent = 0;
	for(int i=0; i<Komponenty.n; i++)
	{
		//sprintf(text,"%d \t %d \n",i,Komponenty.elements[i]);
		//WriteFile(plik,text,strlen(text),&ile,NULL); 

		if( Komponenty.elements[i] > MaxKonponent) MaxKonponent = Komponenty.elements[i];
		
	}


	sprintf_s(text,"\n MaxKomponent (GC): \t %d \n",MaxKonponent);
	WriteFile(plik,text,strlen(text),&ile,NULL);


	int * klaster;
	klaster = new int [MaxKonponent+10];

	for(int i=0; i<=MaxKonponent; i++)
	{
		klaster[i] = 0;
	}

	for(int i=0; i<Komponenty.n; i++)
	{
		klaster[ Komponenty.elements[i] ] ++;
	}


	int dlugosc = 0;
	for(int i=0; i<=MaxKonponent; i++)
		if(klaster[i] > 0)
			dlugosc++;


	// skladowe
	sprintf_s(text,"\n dlugosc:\n%d\n",dlugosc);
	WriteFile(plik,text,strlen(text),&ile,NULL); 
	for(int i=0; i<=MaxKonponent; i++)
	{
		if(klaster[i] > 0)
		{
			sprintf_s(text,"%d \t %d \n",i,klaster[i]);
			WriteFile(plik,text,strlen(text),&ile,NULL); 
		}
	}

	delete [] klaster;
	return 0;
}




int CUniversalNetwork::SeekComponents(void)
{
	int i;
	int CurrentNumber = 0;
	int RozmiarKlastera = 0;
	int RozmiarKlasteraMax = 0;
	int GiantComponent = 0;
	int nowi = 0;
	int cel;
//	char tekst[200];
	
	for(i=0; i<NumberNodes; i++)
		Node[i].TempStateI = 0;

	Komponenty.Clear();
	Komponenty.Add(0);

	for(i=0; i<NumberNodes; i++)
	{
		if(Node[i].TempStateI == 0 && Node[i].Connectivity > 0)
		{
			RozmiarKlastera = 1;
			
			CurrentNumber ++;
			
			Node[i].TempStateI = CurrentNumber;
			nowi = 1;
			
			while (nowi > 0)
			{			
				nowi = 0;
			
				for(int n=i; n<NumberNodes; n++)
				{
					if(Node[n].TempStateI == 0)
					{
						// sprawdzamy czy wsrod sasiadow jest ten co ma odpowiedni stan
						for(int k=0; k<Node[n].Connectivity; k++)
						{
							cel = Node[n].Connection[k];
							if( Node[cel].TempStateI == CurrentNumber )
							{
								Node[n].TempStateI = CurrentNumber;
								RozmiarKlastera++;
								nowi++;
								
								k = Node[n].Connectivity;
							}
						}
					}
				}

			} 
			
			//sprintf_s(tekst,"dodajemy kolejny klaster: \t %d",RozmiarKlastera);
			//WriteToLog(tekst,0);
			Komponenty.Add(RozmiarKlastera);

			if(RozmiarKlastera > RozmiarKlasteraMax)
			{
				RozmiarKlasteraMax = RozmiarKlastera;
				GiantComponent = CurrentNumber;
			}

		}
	}

	return GiantComponent;
}






int CUniversalNetwork::SeekComponentsFast(void)
{
	int i;
	int CurrentNumber = 0;
	int RozmiarKlastera = 0;
	int RozmiarKlasteraMax = 0;
	int GiantComponent = 0;
	int nowi = 0;
	int aktualni = 0;
	int cel;
//	char tekst[200];

	
	int * aktualnewezly;
	int * nowewezly;
	aktualnewezly = new int [NumberNodes];
	nowewezly = new int [NumberNodes];
	
	for(i=0; i<NumberNodes; i++)
		Node[i].TempStateI = 0;

	Komponenty.Clear();
	Komponenty.Add(0);

	for(i=0; i<NumberNodes; i++)
	{
		if(Node[i].TempStateI == 0 && Node[i].Connectivity > 0)
		{
			RozmiarKlastera = 1;
			
			CurrentNumber ++;
			
			Node[i].TempStateI = CurrentNumber;
			aktualnewezly[0] = i;
			aktualni = 1;
			
			while (aktualni > 0)
			{			
				nowi = 0;
			
				// przechodzimy przez aktualnych
				for(int n=0; n<aktualni; n++)
				{
					int aktualny = aktualnewezly[n];
					// aby sprawdzic czy nie kontaktuja sie z jakimis niezarazonymi
					for(int k=0; k<Node[aktualny].Connectivity; k++)
					{
						cel = Node[aktualny].Connection[k];
						if(Node[cel].TempStateI == 0)
						{
							// dodajemy kolejnego do klastera
							Node[cel].TempStateI = CurrentNumber;
							nowewezly[nowi] = cel;
							RozmiarKlastera++;
							nowi++;
						}
					}
				}

				// zamieniamy tak aby w nastepnej petli przejsc przez nowe wezly
				aktualni = nowi;
				int * temppointer = aktualnewezly;
				aktualnewezly = nowewezly;
				nowewezly = temppointer;
				
			} 
			
			//sprintf_s(tekst,"dodajemy kolejny klaster: \t %d",RozmiarKlastera);
			//WriteToLog(tekst,0);
			Komponenty.Add(RozmiarKlastera);

			if(RozmiarKlastera > RozmiarKlasteraMax)
			{
				RozmiarKlasteraMax = RozmiarKlastera;
				GiantComponent = CurrentNumber;
			}

		}
	}

	
	delete [] aktualnewezly;
	delete [] nowewezly;

	return GiantComponent;
}




void CUniversalNetwork::ConnectFast(int From, int To)
{
	Node[From].AddConnectionFast(To);
	Node[To].AddConnectionFast(From);
}

void CUniversalNetwork::ConnectSafe(int From, int To)
{
	if(From < NumberNodes && From >= 0)
		if(To < NumberNodes && To >= 0)
		{
			Node[From].AddConnectionSafe(To);
			Node[To].AddConnectionSafe(From);
		}
}


double CUniversalNetwork::GetRand(void)
{
	double pom;

	pom=uu[ip-1] - uu[jp-1];
	if (pom < 0.0) pom+=1;
	uu[ip-1]=pom;
	ip--;
	if (ip == 0) ip=97;
	jp--;
	if (jp == 0) jp=97;
	cc -= cd;
	if(cc < 0.0) cc+=cm;
	pom -= cc;
	if(pom < 0.0) pom+=1;
	
	if(pom >= 1.0) pom-=1.0;

	return (pom);
}


void CUniversalNetwork::rstart(int i, int j, int k, int l)
{
	int ii,jj,m,wi,wj,wk,wl; double s,t;
	wi=i; wj=j; wk=k; wl=l;
	for(ii=0; ii<97; ii++)
	{
		s=0; t=0.5;
		for(jj=1; jj<=24; jj++)
		{
			m=(((wi*wj)%179)*wk)%179;
			wi=wj; wj=wk; wk=m;
			wl=(53*wl+1)%169;
			if( (wl*m)%64 >= 32 ) s += t;
			t *= 0.5;
		}
		uu[ii] = s;
	}
}


int CUniversalNetwork::PercolateBond(double Probability)
{
	int K = 0;
	// usuwamy stare polaczenia
	for(int n=0; n<NumberNodes; n++)
		Node[n].Connectivity = 0;

	// perkolujemy
	for(int k=0; k<Connections.NumberConnections; k++)
	{
		if( GetRand() <= Probability)
		{
			// dodajemy polaczenie
			ConnectFast(Connections.From[k],Connections.Target[k]);
			K++;
		}
	}

	return K;
}



int CUniversalNetwork::PercolateSite(double Probability)
{
	int S = 0; // liczba 'istniejacych' wezlow
	
	// usuwamy stare polaczenia
	for(int n=0; n<NumberNodes; n++)
	{
		Node[n].Connectivity = 0;
		Node[n].TempStateI = 0;
	}

	// perkolujemy
	// najpierw wybieramy te wezly ktore beda "zrealizowane"
	for(int n=0; n<NumberNodes; n++)
		if( GetRand() <= Probability)
		{
			Node[n].TempStateI = 1;
			S++;
		}

	// teraz dodajemy polaczenia
	for(int k=0; k<Connections.NumberConnections; k++)
	{
		int from = Connections.From[k];
		int target = Connections.Target[k];
		
		// sprawdzamy czy oba wezly sa przewidziane do tego aby istniec
		if( Node[from].TempStateI + Node[target].TempStateI >= 2 )
		{
			// dodajemy polaczenie
			ConnectFast(from,target);
		}
	}

	return S;
}




int CUniversalNetwork::LoadNetworkConnections(const char * filename)
{
	
	char file[500];
	DWORD ile;
	sprintf_s(file,"%s",filename);
	HANDLE binarydata = CreateFile((file),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(binarydata == INVALID_HANDLE_VALUE)  return -1;

	// odczytujemy liczbe uzytkownikow i polaczen
	int N = 0;
	int K = 0;
	ReadFile(binarydata,&N,sizeof(N),&ile,NULL);
	ReadFile(binarydata,&K,sizeof(K),&ile,NULL);

	// tworzymy siec
	this->CreateNetwork(N,0,0,0);
	this->NumberConnections = K;
	this->Connections.Create(K);


	// wczytujemy liczbe polaczen wezlow
	for(int n=0; n<N; n++)
	{
		int k = 0;
		ReadFile(binarydata,&(k),sizeof(k),&ile,NULL);
		Node[n].PreallocateMemmory(k+3);
	}

	// wczytujemy polaczenia
	for(int k=0; k<K; k++)
	{
		int from = 0;
		int target = 0;
		ReadFile(binarydata,&from,sizeof(from),&ile,NULL);
		ReadFile(binarydata,&(target),sizeof(target),&ile,NULL);
		Connections.From[k] = from;
		Connections.Target[k] = target;
	}

	CloseHandle(binarydata);
	return 0;
}






int CUniversalNetwork::SaveNetworkConnections(const char * filename)
{
	
	char file[500];
	DWORD ile;
	sprintf_s(file,"%s",filename);
	HANDLE binarydata = CreateFile((file),GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if(binarydata == INVALID_HANDLE_VALUE)  return -1;

	// odczytujemy liczbe uzytkownikow i polaczen
	int N = NumberNodes;
	int K = this->CountNumberOfEdges();

	WriteFile(binarydata,&N,sizeof(N),&ile,NULL);
	WriteFile(binarydata,&K,sizeof(K),&ile,NULL);

	// tworzymy siec
	/*
	this->CreateNetwork(N,0,0,0);
	this->NumberConnections = K;
	this->Connections.Create(K);
	*/

	// zapisujemy liczbe polaczen wezlow
	for(int n=0; n<N; n++)
	{
		int k = Node[n].Connectivity;
		WriteFile(binarydata,&(k),sizeof(k),&ile,NULL);
	}

	// zapisujemy polaczenia
	for(int n=0; n<N; n++)
	for(int k=0; k<Node[n].Connectivity; k++)
	{
		int from = n;
		int target = Node[n].Connection[k];

		if(target > from)
		{
			WriteFile(binarydata,&from,sizeof(from),&ile,NULL);
			WriteFile(binarydata,&(target),sizeof(target),&ile,NULL);
		}
//		Connections.From[k] = from;
//		Connections.Target[k] = target;
	}

	CloseHandle(binarydata);
	return 0;
}




int CUniversalNetwork::PercoaltionSimulation(int percolationtype)
{
	int LiczbaPowtorzen = 100;
	char text[600];
	DWORD ile;

	CMLE mle;

	
	// tworzymy pliki
	if(BOND_PERCOLATION == percolationtype)	sprintf_s(text,"perkolacja_bond.txt");
	else	sprintf_s(text,"perkolacja_site.txt");
	HANDLE plik = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if(BOND_PERCOLATION == percolationtype)	sprintf_s(text,"rozklady_bond.txt");
	else	sprintf_s(text,"rozklady_site.txt");
	HANDLE rozklady = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	

	sprintf_s(text,"liczba powtorzen: \t %d\n",LiczbaPowtorzen);
	WriteFile(plik,text,strlen(text),&ile,NULL); 

	sprintf_s(text,"\n p \t S \t K \t <s> \t <s> bez S \t <kk>/(2<k>) \t liczba klasterow \t Sav \t Sav_bez_S \t AllK \t K/AllK \t S/AllS \t MaxDistance \t <MaxDistance> \t Lav \t tau \t blad tau\n\n");
	WriteFile(plik,text,strlen(text),&ile,NULL); 

	// wczytujemy siec
	LoadNetworkConnections("percolation_grono.dat");
 
	// symulacja
	double p = 1;
	double kappa = 1;
	double avg_s = 1;
	double avg_s_bezS = 1;
	double LiczbaKlasterow = 1;
	double S = 1;
	double Sav = 1;
	double SavBezS = 1;
	double K = 1;
	double AllK = 1;
	double AllS = 1;
	double Distance = 1;
	double Tau = 1;
	double dTau = 1;
	double DistanceAV = 1;
	double Lav = 1;
	
	for(p=0.000001; p<=1; p+=0.000005)
	//for(p=0.01; p<=0.2; p+=0.0005)
	//for(p=0.006; p<=1; p+=0.0002)
	{
		if(p>0.0001)
			p+=0.00005;
		
		if(p>0.01)
			p+=0.0005;
		
		if(p>0.01)
			p+=0.0005;

		if(p>0.1)
			p+=0.005;

		if(p>0.4)
			p+=0.01;

		kappa = 0;
		avg_s = 0;
		avg_s_bezS = 0;
		LiczbaKlasterow = 0;
		S = 0;
		Sav = 0;
		SavBezS = 0;
		K = 0;
		AllK = 0;
		AllS = 0;
		Distance = 0;
		DistanceAV = 0;
		Tau = 0;
		dTau = 0;
		Lav = 0;

		for(int i=0; i<LiczbaPowtorzen; i++)
		{
			// obliczenia
			switch(percolationtype)
			{
				case BOND_PERCOLATION: PercolateBond(p); break;
				case SITE_PERCOLATION: AllS += PercolateSite(p); break;
			}
			int GC = SeekComponentsFast();

			// zliaczmy wyniki
			K += double(CountNumberOfEdgesInCluster(GC));
			kappa += GetKappa();
			avg_s += Komponenty.GetAverageValue(true);
			avg_s_bezS += Komponenty.GetAverageValue(false);
			S += double(Komponenty.MaxValue);
			LiczbaKlasterow += double(Komponenty.n);
			Sav += Komponenty.GetSav(true);
			SavBezS += Komponenty.GetSav(false);
			AllK += CountNumberOfEdges();
			
			
			
			
			//if(p < 0.004)
			/*
			{
				// liczymy rozmiar GC
				double distMax = CountMaxDistanceInCluster(GC);
				Distance += distMax;

				//liczymy sredni rozmiar
				double distAV = 0;
				double currentDist = 0;
				double licznik = 0;
				double mianownik = 0;
				for(int n=1; n<Komponenty.n; n++)
				{
					if(Komponenty.elements[i] > 2)
					{
						if(n == GC)		currentDist = distMax;
						else	 currentDist = CountMaxDistanceInCluster(n);
					}	 else currentDist = 1;

					distAV += currentDist;

					licznik += double(Komponenty.elements[n])*currentDist;
					mianownik += double(Komponenty.elements[n]);
				}

				if(Komponenty.n > 1)
					DistanceAV += distAV/double(Komponenty.n-1);

				if(mianownik > 0)
					Lav += licznik / mianownik;
			}
			*/


			//double a,da;
			//CountTau(&a,&da);

			

			//-------------------------------------------------------------------------------------
			int MaxKonponent = 0;
			for(int i=0; i<Komponenty.n; i++)
				if( Komponenty.elements[i] > MaxKonponent) MaxKonponent = Komponenty.elements[i];
			
			int * klaster = new int [MaxKonponent+10];

			for(int i=0; i<=MaxKonponent; i++)
					klaster[i] = 0;
			
			for(int i=0; i<Komponenty.n; i++)
				klaster[ Komponenty.elements[i] ] ++;
			

			// kopiujemy dane
			int numer = 0;
			int liczba_jedynek = 0;
			for(int i=2; i<MaxKonponent; i++)
			{
				if(klaster[i] > 0)
				{
					mle.krotnosc[ numer ] = klaster[i];
					mle.wartosc[ numer ] = i;
					numer++;
					if(klaster[i] == 1)
						liczba_jedynek++;
					if(liczba_jedynek > 2)
					{
						i = MaxKonponent;
						break;
					}
				}
			}

			mle.LiczbaDanych = numer;
			delete [] klaster;
			//-------------------------------------------------------------------------------------
			
			//liczymy wartosc wykladnika MLE
			double da;
			mle.DistributionType = POWERLAW;
			Tau += mle.LiczWykladnik(&da);
			dTau += da;
		}

		// normowanie 
		kappa /= LiczbaPowtorzen;
		avg_s /= LiczbaPowtorzen;
		avg_s_bezS /= LiczbaPowtorzen;
		LiczbaKlasterow /= LiczbaPowtorzen;
		S /= LiczbaPowtorzen;
		Sav /= LiczbaPowtorzen;
		SavBezS /= LiczbaPowtorzen;
		K /= LiczbaPowtorzen;
		AllK /= LiczbaPowtorzen;
		AllS /= LiczbaPowtorzen;
		Distance /= LiczbaPowtorzen;
		DistanceAV /= LiczbaPowtorzen;
		Tau /= LiczbaPowtorzen;
		dTau /= LiczbaPowtorzen;
		Lav /= LiczbaPowtorzen;
		
		// zapis wynikow
		sprintf_s(text,"%lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \n",p,S,K,avg_s,avg_s_bezS,kappa,LiczbaKlasterow,Sav,SavBezS,AllK,K/AllK,S/AllS,Distance,DistanceAV,Lav,Tau,dTau);
		WriteFile(plik,text,strlen(text),&ile,NULL); 
		
		sprintf_s(text,"\n p = \t %lf \n",p);
		WriteFile(rozklady,text,strlen(text),&ile,NULL); 
		ZapiszSkladoweSieci(rozklady);
	}

	CloseHandle(plik);
	CloseHandle(rozklady);
	return 0;
}

double CUniversalNetwork::GetKappa(void)
{
	double k = 0;
	double kk = 0;
	
	for(int n=0; n<NumberNodes; n++)
	{
		k += double(Node[n].Connectivity);
		kk += double(Node[n].Connectivity*Node[n].Connectivity);
	}

	k /= double(NumberNodes);
	kk /= double(NumberNodes);

	
	return kk/(2*k);
}

int CUniversalNetwork::CountNumberOfEdgesInCluster(int cluster)
{
	int suma = 0;
	for(int n=0; n<NumberNodes; n++)
	{
		if(Node[n].TempStateI == cluster)
		{
			suma += Node[n].Connectivity;
		}
	}

	return suma/2;
}

int CUniversalNetwork::CountNumberOfEdges(void)
{
	int suma = 0;

	for(int n=0; n<NumberNodes; n++)
	{
		suma += Node[n].Connectivity;
	}
	
	return suma/2;
}

int CUniversalNetwork::CountMaxDistanceInCluster(int cluster)
{

	int size = 0;
	int wezly[100];
	int malaliczbawezlow = 0;
	//zliczmy wezly w klasterze
	for(int n=0; n<NumberNodes; n++)
	{
		if(Node[n].TempStateI == cluster)
		{
			size++;
			if(malaliczbawezlow < 100)
			{
				wezly[malaliczbawezlow] = n;
				malaliczbawezlow++;
			}
		}
	}
	

	// przypadki szczegolne:
	if(size == 2)
		return 1;
	
	if(size == 3)
	{
		for(int n=0; n<3; n++)
			if(Node[wezly[n]].Connectivity == 1) return 2;
		return 1;
	}

	if(size == 4)
	{
		int kkk = 0;
		for(int n=0; n<4; n++)
			if(Node[wezly[n]].Connectivity == 3) kkk++;
		if(kkk >= 2)	return 1;
		if(kkk == 1)	return 2;
		return 3;
	}
	// koniec przypadkow szczegolnych



	int MaxDistance = 0;
	int Distance = 0;
	int * aktualnewezly = new int [size+3];
	int * nowewezly = new int [size+3];
	int * wklusterze = new int [size+3];

	// zapamietujemy te wezly, ktore sa w klasterze
	int iter = 0;
	if(size == malaliczbawezlow)
	{
		for(int n=0; n<malaliczbawezlow; n++)
			wklusterze[n] = wezly[n];
	}
	else
	{
		for(int n=0; n<NumberNodes; n++)
			if(Node[n].TempStateI == cluster)
			{
				wklusterze[iter] = n;
				iter++;
			}
	}
	// przechodzmiy przez wszystkie wezly z klastera
	for(int n=0; n<size; n++)
	{
		// ustawiamy wszystkim odpowiednia wartosc TempInt
		for(int m=0; m<size; m++)
			Node[wklusterze[m]].TempStateI = cluster;
		
		
		int visited = cluster + 1;
		
		int num = wklusterze[n];
		int aktualni = 1;
		aktualnewezly[0] = num;
		Node[num].TempStateI = visited;

		Distance = 0;

		while(aktualni)
		{
			int nowi = 0;

			
			// przechodzimy przez aktualne wezly
			for(int a=0; a<aktualni; a++)
			{
				int id = aktualnewezly[a];
				int target = 0;
				// oraz ich sasiadow
				for(int k=0; k<Node[id].Connectivity; k++)
				{
					target = Node[id].Connection[k];
					// by sprawdzic czy sasiada juz odwiedzilismy
					if(Node[target].TempStateI != visited)
					{
						Node[target].TempStateI = visited;
						nowewezly[nowi] = target;
						nowi++;
					}
				}

			}


			// jezeli byli jacys sasiedzi to zwiekszamy Distance
			if(nowi > 0)	Distance++;

			// wymiana
			int * temppointer = aktualnewezly;
			aktualnewezly = nowewezly;
			nowewezly = temppointer;

			aktualni = nowi;
		}

		// sprawdzamy czy zwiekszyl sie distance
		if(Distance > MaxDistance) MaxDistance = Distance;
			
	}
	
	// przywracamy stan poprzedni
	for(int m=0; m<size; m++)
			Node[wklusterze[m]].TempStateI = cluster;
	
	
	// zwalniamy pamiec
	delete [] aktualnewezly;
	delete [] nowewezly;
	delete [] wklusterze;

	return MaxDistance;
}

int CUniversalNetwork::CountTau(double * a, double * da)
{
	OperacjeMatematyczne om;

	double b,db;

	// zerujemy
	*a = 0;
	*da = 0;

	int * klaster;
	klaster = new int [Komponenty.MaxValue+3];

	for(int i=0; i<=Komponenty.MaxValue; i++)
		klaster[i] = 0;
	
	for(int i=0; i<Komponenty.n; i++)
		klaster[ Komponenty.elements[i] ] ++;
	
	int dlugosc = 0;
	for(int i=1; i<Komponenty.MaxValue; i++)
		if(klaster[i] > 0)
			dlugosc++;

	double * daneX = new double [dlugosc+5]; 
	double * daneY = new double [dlugosc+5]; 

	// przepisujemy dane
	int num = 0;
	for(int i=1; i<Komponenty.MaxValue; i++)
		if(klaster[i] > 1)
		{
			// i od razu liczymy logarytm
			daneX[num] = log(double(i));
			daneY[num] = log(double(klaster[i]));
			num++;
		}


	if(num > 3)
	 om.NajmniejszeKwadraty(daneX,daneY,num,a,da,&b,&db);


	delete [] klaster;
	delete [] daneX;
	delete [] daneY;
	return 0;
}

int CUniversalNetwork::LoadNetworkWithActivity(void)
{
	HANDLE plik;
	DWORD ile;
		
	plik = CreateFile("network.dat",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(plik == INVALID_HANDLE_VALUE) return -1;
	
	DestroyNetwork();
	
	// liczbe osobnikow
	int NumberOfNodes=0;
	ReadFile(plik,&NumberOfNodes,sizeof(NumberOfNodes),&ile,NULL); 
    
	// alokujemy pamiec
	CreateNetwork(NumberOfNodes,1,1,10);
	
	float activity;
	int liczbapolaczen;
	int num;
	// w petli po osobnikach zapisujemy 
	// najpierw: liczbe polaczen i aktywnosc
	// potem numery jednostek do ktorych jest polaczenie
	
	for(int i = 0; i < NumberOfNodes; i++)
	{
		ReadFile(plik,&liczbapolaczen,sizeof(liczbapolaczen),&ile,NULL); 
		ReadFile(plik,&activity,sizeof(activity),&ile,NULL);
		
		Node[i].StateF[0] = activity;
		
		for(int k = 0; k < liczbapolaczen; k++)
		{
			ReadFile(plik,&num,sizeof(num),&ile,NULL); 
			this->ConnectSafe(i,num);
		}
	}


	CloseHandle(plik);


	// sprawdzamy zapis
	//HANDLE pliktxt;
	//DWORD ile;
	//char text[300];

	
	/*
	pliktxt = CreateFile("info.txt",GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	sprintf(text,"NumberOfNodes = %d \n",NumberOfNodes);
		WriteFile(plik,text,strlen(text),&ile,NULL); 

	for(int i=0; i<NumberOfNodes; i++)
	{
		sprintf(text," %d \t %lf\n",Node[i].k, Node[i].activity);
			WriteFile(plik,text,strlen(text),&ile,NULL); 
	}

	CloseHandle(pliktxt);
	*/
	

	return 0;
}

int CUniversalNetwork::LiczZmodyfikowanyWspolczynnikKlasteryzacji(void)
{
	for(int i=0; i<this->NumberNodes; i++)
	{
		int k = Node[i].Connectivity;
		
		int * lista = new int [k];
		for(int j=0; j<k; j++)
		{
			int sasiad = Node[i].Connection[j];
			if(k < Node[sasiad].Connectivity)		lista[j] = k - 1;
			else									lista[j] = Node[sasiad].Connectivity - 1;
		}

		int dlugosclisty = k;
		int liczbamozliwychpolaczen = 0;
		
		while(dlugosclisty > 1)
		{
			// sortujemy liste
			Sortuj(lista,dlugosclisty);


			// dodajemy polaczenia
			for(int i=1; i<dlugosclisty; i++)
			{
				if(lista[0] > 0 && lista[i] > 0)
				{
					lista[0]--;
					lista[i]--;
					liczbamozliwychpolaczen++;
				}
			}

			// usuwamy pierwszy element oraz zera
			lista[0] = 0;
			for(int i=0; i<dlugosclisty; i++)
			{
				if(lista[i] <= 0)
				{
					dlugosclisty--;
					lista[i] = lista[dlugosclisty];
				}
			}
		}

		delete [] lista;

		int e = LiczbLiczbePolaczenPomiedzySasiadami(i);
		if(e > 0 && liczbamozliwychpolaczen > 0)
			Node[i].TempStateF = float(e) / float(liczbamozliwychpolaczen);
		else
			Node[i].TempStateF = 0.0f;

	}
	
	
	return 0;
}

int CUniversalNetwork::LiczbLiczbePolaczenPomiedzySasiadami(int num)
{
	int liczbapolaczen = 0;
	int k = Node[num].Connectivity;

	int s1,s2;

	for(int i=0; i<k; i++)
	{
		s1 = Node[num].Connection[i];

		for(int j=i+1; j<k; j++)
		{
			s2 = Node[num].Connection[j];
			
			// teraz sprawdzamy czy pomiedzy s1 i s2 jest polaczenie
			for(int n=0; n<Node[s1].Connectivity; n++)
			{
				if(s2 == Node[s1].Connection[n])
				{
					liczbapolaczen++;
					n = Node[s1].Connectivity;
				}
			}
		}
	}

	return liczbapolaczen;
}

void CUniversalNetwork::Sortuj(int * tab, int dlugosc)
{
	for(int i=0; i<dlugosc; i++)
	{
		int max = tab[i];
		int maxi = i;

		// szukamy maksimum
		for(int j=i+1; j<dlugosc; j++)
		{
			if(tab[j] > max)
			{
				max = tab[j];
				maxi = j;
			}
		}

		// wymieniamy
		int temp = tab[i];
		tab[i] = max;
		tab[maxi] = temp;
	}
}

int CUniversalNetwork::ZapiszZaleznoscIntOdConnectivity(HANDLE plik)
{
	DWORD ile;
	char txt[300];

	// liczymy maksymalna liczbe polaczen
	int maxk = 0;
	for(int i=0; i<NumberNodes; i++)
		if(Node[i].Connectivity > maxk)		maxk = Node[i].Connectivity;

	if(maxk <= 0)	return 0;

	// zerujemy tablice
	int * tab = new int [maxk+1];
	int * tabile = new int [maxk+1];
	for(int i=0; i<=maxk; i++)	
	{
		tab[i] = 0;
		tabile[i] = 0;
	}

	// zliczamy wartosci TempStateInt
	for(int i=0; i<NumberNodes; i++)
	{
		int k = Node[i].Connectivity;
		tab[k] += Node[i].TempStateI;
		tabile[k]++;
	}
	
	for(int i = 0; i <= maxk; i ++)
	if(tabile[i] > 0)
	{
		float wartosc = float(tab[i]) / float (tabile[i]);
		sprintf_s(txt,"%d\t%lf\n",i,wartosc);
		WriteFile(plik,txt,strlen(txt),&ile,NULL);
	}


	delete [] tab;
	delete [] tabile;
	return 0;
}




int CUniversalNetwork::ZapiszZaleznoscFloatOdConnectivity(HANDLE plik)
{
	DWORD ile;
	char txt[300];

	// liczymy maksymalna liczbe polaczen
	int maxk = 0;
	for(int i=0; i<NumberNodes; i++)
		if(Node[i].Connectivity > maxk)		maxk = Node[i].Connectivity;

	if(maxk <= 0)	
		return 0;

	// zerujemy tablice
	float * tab = new float [maxk+1];
	int * tabile = new int [maxk+1];
	for(int i=0; i<=maxk; i++)	
	{
		tab[i] = 0;
		tabile[i] = 0;
	}

	// zliczamy wartosci TempStateInt
	for(int i=0; i<NumberNodes; i++)
	{
		int k = Node[i].Connectivity;
		tab[k] += Node[i].TempStateF;
		tabile[k]++;
	}
	
	for(int i = 0; i <= maxk; i ++)
	if(tabile[i] > 0)
	{
		float wartosc = float(tab[i]) / float (tabile[i]);
		sprintf_s(txt,"%d\t%lf\n",i,wartosc);
		WriteFile(plik,txt,strlen(txt),&ile,NULL);
	}


	delete [] tab;
	delete [] tabile;
	return 0;
}





int CUniversalNetwork::ZapiszZaleznoscFOdI(HANDLE plik)
{
	DWORD ile;
	char txt[300];

	// liczymy maksymalna liczbe polaczen
	int maxk = 0;
	for(int i=0; i<NumberNodes; i++)
		if(Node[i].TempStateI > maxk)		maxk = Node[i].TempStateI;

	if(maxk <= 0)	
		return 0;

	sprintf_s(txt,"\tmax=\t%d\n",maxk);
	WriteFile(plik,txt,strlen(txt),&ile,NULL);

	if(maxk > 1000000) maxk = 1000000;

	// zerujemy tablice
	float * tab = new float [maxk+1];
	float * maxvalue = new float [maxk+1];
	int * tabile = new int [maxk+1];
	float *stddev = new float [maxk+1];
	for(int i=0; i<=maxk; i++)	
	{
		tab[i] = 0.0f;
		tabile[i] = 0;
		maxvalue[i] = 0.0f;
		stddev[i] = 0.0f;
	}


	float tf = 0;
	// zliczamy wartosci TempStateFloat
	for(int i=0; i<NumberNodes; i++)
	{
		int k = Node[i].TempStateI;
		
		if(k >= 0 && k <= maxk)
		{
			tf = Node[i].TempStateF;
			if(tf > 1000000.0f)
				tf = 0.0f;
			
			{
				if(tf > maxvalue[k])
					maxvalue[k] = tf;
				
				tab[k] += tf;
				tabile[k]++;
			}
		}
	}

	// liczymy Standard Deviation
	for(int i=0; i<NumberNodes; i++)
	{
		int k = Node[i].TempStateI;
		
		if(k >= 0 && k <= maxk)
		{
			tf = Node[i].TempStateF;
			
			if(tf < 1000000.0f)
			if(tabile[k] > 0)
			{
				float avg = tab[k] / float(tabile[k]);
				stddev[k] += (tf - avg)*(tf - avg);
			}
		}
	}
	
	for(int i = 0; i <= maxk; i ++)
	if(tabile[i] > 0 && tab[i] > 0)
	{
		float wartosc = float(tab[i]) / float (tabile[i]);
		sprintf_s(txt,"%d\t%lf \t\t %d\t%lf \t\t %lf \t\t %lf\n",i,wartosc,tabile[i],tab[i],maxvalue[i],sqrtf(stddev[i]/float(tabile[i])));
		WriteFile(plik,txt,strlen(txt),&ile,NULL);
	}


	delete [] tab;
	delete [] tabile;
	delete [] maxvalue;
	delete [] stddev;
	return 0;
}




void CUniversalNetwork::ConvertToNetwork(void)
{
	
	long int maxUser = 9823233;
	int liczbaCech = 12;

	CreateNetwork(maxUser+1,liczbaCech,0,0);


	HANDLE log = CreateFile("log_convert.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	
	// WCZYTUJEMY DETALE
	char text[500];
	char logt[500];
		int LiczbaPlikow = 20;
		int LiczbaPodPlikow = 20;
		DWORD ile;
		
		
		LiczbaPlikow = 20;
		LiczbaPodPlikow = 20;
		for(int numerpliku = 0; numerpliku<LiczbaPlikow; numerpliku++)
		for(int pn = 0; pn<LiczbaPodPlikow; pn++)
		{
			sprintf(text,"int_skyrock_details_%d_%d.txt",numerpliku,pn);
			HANDLE plik = CreateFile(text,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if(plik > 0 && plik != INVALID_HANDLE_VALUE)
			{
				sprintf(logt,"test_%s",text);
				HANDLE plik2 = CreateFile(logt,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				
				
				bool endfile = false;
				while(!endfile)
				{
					int id = ReadInt(plik,&endfile);
					if(id >= 0 && id < NumberNodes && !endfile)
					{
						Node[id].StateI[DOWNLOADDATE] = ReadInt(plik,&endfile);
						Node[id].StateI[DSTOP] = ReadInt(plik,&endfile);
						Node[id].StateI[DSTART] = ReadInt(plik,&endfile);
						Node[id].StateI[NUMBERVISITS] = ReadInt(plik,&endfile);
						
						sprintf(logt,"%d\n%d\n%d\n%d\n%d\n",id,Node[id].StateI[DOWNLOADDATE],Node[id].StateI[DSTOP],Node[id].StateI[DSTART],Node[id].StateI[NUMBERVISITS]);
						WriteFile(plik2,logt,strlen(logt),&ile,NULL);

						if( Node[id].StateI[NUMBERVISITS] > 100000)
						{
							sprintf(logt,"%s \t id=\t %d\n",text,id);
							WriteFile(log,logt,strlen(logt),&ile,NULL);
						}

						if(Node[id].StateI[NUMBERVISITS] > 100000)	Node[id].StateI[NUMBERVISITS] = 100000;


					}
					else
						for(int i=0; i<4; i++)	ReadInt(plik,&endfile);


					sprintf(logt,"\n\n\n");
					WriteFile(plik2,logt,strlen(logt),&ile,NULL);

					// nastepny uzytkownik
					ReadInt(plik,&endfile);
					ReadInt(plik,&endfile);
					ReadInt(plik,&endfile);
				}
				CloseHandle(plik);
				CloseHandle(plik2);
			}
		}
		
		
	

		// Details Blog

		LiczbaPlikow = 20;
		LiczbaPodPlikow = 20;
		for(int numerpliku = 0; numerpliku<LiczbaPlikow; numerpliku++)
		for(int pn = 0; pn<LiczbaPodPlikow; pn++)
		{
			sprintf(text,"int_skyrock_details_blog_%d_%d.txt",numerpliku,pn);
			HANDLE plik = CreateFile(text,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if(plik > 0 && plik != INVALID_HANDLE_VALUE)
			{
				sprintf(logt,"test_%s",text);
				HANDLE plik2 = CreateFile(logt,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				
				
				bool endfile = false;
				while(!endfile)
				{
					
					
					int id = ReadInt(plik,&endfile);
					if(id >= 0 && id < NumberNodes && !endfile)
					{
						Node[id].StateI[DOWNLOADDATEBLOG] = ReadInt(plik,&endfile);
						Node[id].StateI[DSTARTBLOG] = ReadInt(plik,&endfile);
						Node[id].StateI[DSTOPBLOG] = ReadInt(plik,&endfile);
						Node[id].StateI[NUMBERVISITSBLOG] = ReadInt(plik,&endfile);
						Node[id].StateI[NUMBERVISITSBLOGMONTH] = ReadInt(plik,&endfile);
						Node[id].StateI[NUMBERARTICLESBLOG] = ReadInt(plik,&endfile);
						Node[id].StateI[NUMBERCOMMENTSBLOG] = ReadInt(plik,&endfile);

							if( Node[id].StateI[NUMBERVISITSBLOG] > 100000 
							|| Node[id].StateI[NUMBERVISITSBLOGMONTH] > 100000 || Node[id].StateI[NUMBERARTICLESBLOG] > 100000 || Node[id].StateI[NUMBERCOMMENTSBLOG] > 100000 )
						{
							sprintf(logt,"%s \t id=\t %d\n",text,id);
							WriteFile(log,logt,strlen(logt),&ile,NULL);
						}


						sprintf(logt,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",id,Node[id].StateI[DOWNLOADDATEBLOG],Node[id].StateI[DSTARTBLOG],Node[id].StateI[DSTOPBLOG],Node[id].StateI[NUMBERVISITSBLOG],
							Node[id].StateI[NUMBERVISITSBLOGMONTH],Node[id].StateI[NUMBERARTICLESBLOG],Node[id].StateI[NUMBERCOMMENTSBLOG]);
						WriteFile(plik2,logt,strlen(logt),&ile,NULL);


						if(Node[id].StateI[NUMBERVISITSBLOG] > 100000)	Node[id].StateI[NUMBERVISITSBLOG] = 100000;
						if(Node[id].StateI[NUMBERVISITSBLOGMONTH] > 100000)	Node[id].StateI[NUMBERVISITSBLOGMONTH] = 100000;
						if(Node[id].StateI[NUMBERARTICLESBLOG] > 100000)	Node[id].StateI[NUMBERARTICLESBLOG] = 100000;
						if(Node[id].StateI[NUMBERCOMMENTSBLOG] > 100000)	Node[id].StateI[NUMBERCOMMENTSBLOG] = 100000;

					


					}
					else
						for(int i=0; i<7; i++)	ReadInt(plik,&endfile);

					// nastepny uzytkownik
					ReadInt(plik,&endfile);
					ReadInt(plik,&endfile);
					ReadInt(plik,&endfile);

					sprintf(logt,"\n\n\n");
					WriteFile(plik2,logt,strlen(logt),&ile,NULL);

				}
				CloseHandle(plik);
				CloseHandle(plik2);
			}
		}
		
		
		
		

		
		// zas teraz polaczenia uzytkownikow
		LiczbaPlikow = 20;
		LiczbaPodPlikow = 20;
		for(int numerpliku = 0; numerpliku<LiczbaPlikow; numerpliku++)
		for(int pn = 0; pn<LiczbaPodPlikow; pn++)
		{
			sprintf(text,"int_skyrock_friends_%d_%d.txt",numerpliku,pn);
			HANDLE plik = CreateFile(text,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if(plik > 0 && plik != INVALID_HANDLE_VALUE)
			{
				sprintf(logt,"test_%s",text);
				HANDLE plik2 = CreateFile(logt,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				
				
				bool endfile = false;
				while(!endfile)
				{
					int id = ReadInt(plik,&endfile);
					if(id >= 0 && id < NumberNodes && !endfile)
					{
						int neighbor = 0;
						Node[id].StateI[ALLK] = ReadInt(plik,&endfile);

						sprintf(logt,"%d\n%d\n",id,Node[id].StateI[ALLK]);
						WriteFile(plik2,logt,strlen(logt),&ile,NULL);

						if(Node[id].StateI[ALLK] > 100000 )
						{
							sprintf(logt,"%s \t id=\t %d\n",text,id);
							WriteFile(log,logt,strlen(logt),&ile,NULL);
						}
						
						for(int i=0; i<Node[id].StateI[ALLK]; i++)	
						{
							neighbor = ReadInt(plik,&endfile);
							if(endfile)
								i = Node[id].StateI[ALLK];
							else
							{
								ConnectSafe(id,neighbor);
								
								sprintf(logt,"%d\n",neighbor);
								WriteFile(plik2,logt,strlen(logt),&ile,NULL);
							}
						}
					}
					else
					{
						int k = ReadInt(plik,&endfile);
						for(int i=0; i<k; i++)	ReadInt(plik,&endfile);
					}

					// nastepny uzytkownik
					ReadInt(plik,&endfile);
					ReadInt(plik,&endfile);
					ReadInt(plik,&endfile);

					sprintf(logt,"\n\n\n");
					WriteFile(plik2,logt,strlen(logt),&ile,NULL);
				}
				CloseHandle(plik);
				CloseHandle(plik2);
			}
		}

		for(int numerpliku = 0; numerpliku<LiczbaPlikow; numerpliku++)
		for(int pn = 0; pn<LiczbaPodPlikow; pn++)
		{
			sprintf(text,"int_skyrock_friends_blog_%d_%d.txt",numerpliku,pn);
			HANDLE plik = CreateFile(text,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if(plik > 0 && plik != INVALID_HANDLE_VALUE)
			{
				sprintf(logt,"test_%s",text);
				HANDLE plik2 = CreateFile(logt,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			
				
				bool endfile = false;
				while(!endfile)
				{
					int id = ReadInt(plik,&endfile);
					if(id >= 0 && id < NumberNodes && !endfile)
					{
						int neighbor = 0;
						Node[id].StateI[ALLK] = ReadInt(plik,&endfile);

						sprintf(logt,"%d\n%d\n",id,Node[id].StateI[ALLK]);
						WriteFile(plik2,logt,strlen(logt),&ile,NULL);

						if(Node[id].StateI[ALLK] > 100000 )
						{
							sprintf(logt,"%s \t id=\t %d\n",text,id);
							WriteFile(log,logt,strlen(logt),&ile,NULL);
						}
						
						for(int i=0; i<Node[id].StateI[ALLK]; i++)	
						{
							neighbor = ReadInt(plik,&endfile);
							if(endfile)
								i = Node[id].StateI[ALLK];
							else
							{
								ConnectSafe(id,neighbor);
							
								sprintf(logt,"%d\n",neighbor);
								WriteFile(plik2,logt,strlen(logt),&ile,NULL);
							}
						}
					}
					else
					{
						int k = ReadInt(plik,&endfile);
						for(int i=0; i<k; i++)	ReadInt(plik,&endfile);
					}


					// nastepny uzytkownik
					ReadInt(plik,&endfile);
					ReadInt(plik,&endfile);
					ReadInt(plik,&endfile);

					
					sprintf(logt,"\n\n\n");
					WriteFile(plik2,logt,strlen(logt),&ile,NULL);
				}
				CloseHandle(plik);
				CloseHandle(plik2);
			}
		}


		CloseHandle(log);

		// zapisujemy siec
		Save("network.dat");
}

int CUniversalNetwork::ReadInt(HANDLE plik, bool * endfile)
{
	int n = 0;
	char znak,txt[100];
	DWORD ile;
	bool wynik = false;
	
	*endfile = false;

	wynik = ReadFile(plik,&znak,1,&ile,NULL);
	
	while(  (znak<'0' || znak>'9') )//&& znak != '\n' ) // UWAGA!!! znak konca lini byl do czagos potrzebny
	{
		wynik = ReadFile(plik,&znak,1,&ile,NULL);

		if(!wynik || ile == 0)
		{
			*endfile = true;
			return 0;
		}
	}
	
	
	n = 0;
	while(znak >= '0' && znak <= '9' && n<90)
	{
		txt[n] = znak;
		n++;
		wynik = ReadFile(plik,&znak,1,&ile,NULL);

		if(wynik && ile == 0)
		{
			*endfile = true;
			txt[n] = NULL;
			return atoi(txt);
		}
	}
	txt[n] = NULL;

	int num = atoi(txt);
	
	
	if(wynik && ile == 0)
	{
		// koniec pliku
		*endfile = true;
	}

	
	return num;
}

void CUniversalNetwork::Save(char * txt)
{
	if(txt == NULL) return;

	DWORD ile;

	// otwieramy plik
	HANDLE plik = CreateFile(txt,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	//zapisujemy
	WriteFile(plik,&NumberNodes,sizeof(NumberNodes),&ile,NULL);
	WriteFile(plik,&NumberFeaturesI,sizeof(NumberFeaturesI),&ile,NULL);
	WriteFile(plik,&NumberFeaturesF,sizeof(NumberFeaturesF),&ile,NULL);

	// teraz wezly
	for(int i=0; i<NumberNodes; i++)
		Node[i].Save(plik,NumberFeaturesI,NumberFeaturesF);

	CloseHandle(plik);
}

void CUniversalNetwork::Load(char * filename)
{
	if(filename == NULL) return;

	DWORD ile;
	char t[300];

	// otwieramy plik
	HANDLE plik = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	HANDLE log = CreateFile("log.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	int numberNodes,numberFeaturesI,numberFeaturesF;
	
	//odczytujemy
	ReadFile(plik,&numberNodes,sizeof(NumberNodes),&ile,NULL);
	ReadFile(plik,&numberFeaturesI,sizeof(NumberFeaturesI),&ile,NULL);
	ReadFile(plik,&numberFeaturesF,sizeof(NumberFeaturesF),&ile,NULL);


	//tworzymy siec
	this->CreateNetwork(numberNodes,numberFeaturesI,numberFeaturesF,2);

	// teraz wezly
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].Load(plik,NumberFeaturesI,NumberFeaturesF);

		if( (i<100) || (i > numberNodes - 100) )
		{
			sprintf(t,"\n\n\n\n%d\n\n",i);
			WriteFile(log,t,strlen(t),&ile,NULL);
			
			int k = Node[i].Connectivity;
			sprintf(t,"k=\t%d\n",k);
			WriteFile(log,t,strlen(t),&ile,NULL);

			for(int j=0; j<k; j++)
			{
				sprintf(t,"\t%d\n",Node[i].Connection[j]);
				WriteFile(log,t,strlen(t),&ile,NULL);
			}

			for(int j=0; j<NumberFeaturesI; j++)
			{
				sprintf(t,"\t%d\n",Node[i].StateI[j]);
				WriteFile(log,t,strlen(t),&ile,NULL);
			}
		}
	}
	

	sprintf(t,"\n\n\n\nSIEC WCZYTANA\n\n");
	WriteFile(log,t,strlen(t),&ile,NULL);

	CloseHandle(plik);
	CloseHandle(log);
}

// liczy i zapisuje do pliku dystrybucje TempStateInt
void CUniversalNetwork::ZapiszRozkladInt(HANDLE plik)
{
	DWORD ile;
	char txt[300];

	// liczymy maksymalna wartosc
	int maxk = 0;
	for(int i=0; i<NumberNodes; i++)
		if(Node[i].TempStateI > maxk)		maxk = Node[i].TempStateI;

	if(maxk <= 0)	
		return;


	sprintf_s(txt,"\tmax=\t%d\n",maxk);
	WriteFile(plik,txt,strlen(txt),&ile,NULL);

	if(maxk > 1000000) maxk = 1000000;

	// zerujemy tablice
	int * tab = new int [maxk+3];
	for(int i=0; i<=maxk; i++)	
	{
		tab[i] = 0;
	}

	// zliczamy wartosci TempStateInt
	for(int i=0; i<NumberNodes; i++)
	{
		int k = Node[i].TempStateI;
		if(k>=0 && k<=maxk)
			tab[k] ++;
	}
	
	for(int i = 0; i <= maxk; i ++)
	if(tab[i] > 0)
	{
		float wartosc = float(tab[i]) / float (NumberNodes);
		sprintf_s(txt,"%d\t%d\t%lf\n",i,tab[i],wartosc);
		WriteFile(plik,txt,strlen(txt),&ile,NULL);
	}

	delete [] tab;
	return;
}

void CUniversalNetwork::BadajSiecSkyRock(void)
{
	Load("network.dat");


	DWORD ile;
	char txt[300];

	// otwieramy plik
	HANDLE plik = CreateFile("wyniki-skyrock.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	
	sprintf_s(txt,"skyrock - wyniki\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);

	sprintf_s(txt,"DYSTRYBUCJE\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);

	
	// rozklad k
	sprintf_s(txt,"\n\nN(k)\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
		Node[i].TempStateI = Node[i].Connectivity;
	ZapiszRozkladInt(plik);


	// rozklad allk
	sprintf_s(txt,"\n\nN(all_k)\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
		Node[i].TempStateI = Node[i].StateI[ALLK];
	ZapiszRozkladInt(plik);


	// rozklad Liczby Wizyt w profilu
	sprintf_s(txt,"\n\n N(NUMBERVISITS) - liczba odwiedzin profilu\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
		Node[i].TempStateI = Node[i].StateI[NUMBERVISITS];
	ZapiszRozkladInt(plik);


	// rozklad Liczby Wizyt w blogu
	sprintf_s(txt,"\n\n N(NUMBERVISITSBLOG) - liczba odwiedzin bloga\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
		Node[i].TempStateI = Node[i].StateI[NUMBERVISITSBLOG];
	ZapiszRozkladInt(plik);


	// rozklad Liczby Wizyt w blogu w miesiacu
	sprintf_s(txt,"\n\n N(NUMBERVISITSBLOGMONTH) - liczba odwiedzin bloga w miesiacu ostatnim\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
		Node[i].TempStateI = Node[i].StateI[NUMBERVISITSBLOGMONTH];
	ZapiszRozkladInt(plik);

	// rozklad Liczby Artykulow w blogu
	sprintf_s(txt,"\n\n N(NUMBERARTICLESBLOG) - liczba artykulow w blogu\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
		Node[i].TempStateI = Node[i].StateI[NUMBERARTICLESBLOG];
	ZapiszRozkladInt(plik);

	// rozklad Liczby Artykulow w blogu
	sprintf_s(txt,"\n\n N(NUMBERCOMMENTSBLOG) - liczba komentarzy do bloga\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
		Node[i].TempStateI = Node[i].StateI[NUMBERCOMMENTSBLOG];
	ZapiszRozkladInt(plik);


	// rozklad czasu zycia - lifespan: profil
	sprintf_s(txt,"\n\n N(T_L) - rozklad czasu zycia: profil \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOP] - Node[i].StateI[DSTART];
		Node[i].TempStateI = lifespan;
	}
	ZapiszRozkladInt(plik);

	
	// rozklad czasu zycia - lifespan: blog
	sprintf_s(txt,"\n\n N(T_L) - rozklad czasu zycia: blog \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
		Node[i].TempStateI = lifespan;
	}
	ZapiszRozkladInt(plik);




	// rozklad calkowitego czasu zycia - lifespan: blog + profil
	sprintf_s(txt,"\n\n N(T_L) - rozklad czasu zycia: blog \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan_p = Node[i].StateI[DSTOP] - Node[i].StateI[DSTART];
		int lifespan_b = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];

		if(lifespan_p > 10000)	lifespan_p = 0;
		if(lifespan_b > 10000)	lifespan_b = 0;

		int lifespan = lifespan_p;
		if(lifespan_b > lifespan_p)
			lifespan = lifespan_b;

		Node[i].TempStateI = lifespan;
	}
	ZapiszRozkladInt(plik);



	// nieaktywni 30 dni
	
	// rozklad czasu zycia - lifespan: profil
	sprintf_s(txt,"\n\n N(T_L) - rozklad czasu zycia dla nieaktywnych 30 dni: profil \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOP] - Node[i].StateI[DSTART];
		if(Node[i].StateI[DOWNLOADDATE] - Node[i].StateI[DSTOP] > 30)
			Node[i].TempStateI = 0;
		else
			Node[i].TempStateI = lifespan;
	}
	ZapiszRozkladInt(plik);

	
	// rozklad czasu zycia - lifespan: blog
	sprintf_s(txt,"\n\n N(T_L) - rozklad czasu zycia dla nieaktywnych 30 dni: blog \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
		if(Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTOPBLOG] > 30)
			Node[i].TempStateI = 0;
		else
			Node[i].TempStateI = lifespan;
	}
	ZapiszRozkladInt(plik);


	// rozklad czasu zycia - lifespan: blog
	sprintf_s(txt,"\n\n N(T_L) - rozklad czasu zycia dla nieaktywnych 60 dni: blog \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
		if(Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTOPBLOG] > 60)
			Node[i].TempStateI = 0;
		else
			Node[i].TempStateI = lifespan;
	}
	ZapiszRozkladInt(plik);


	// rozklad czasu zycia - lifespan: blog
	sprintf_s(txt,"\n\n N(T_L) - rozklad czasu zycia dla nieaktywnych 90 dni: blog \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
		if(Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTOPBLOG] > 90)
			Node[i].TempStateI = 0;
		else
			Node[i].TempStateI = lifespan;
	}
	ZapiszRozkladInt(plik);


	// rozklad czasu istnienia: profil
	sprintf_s(txt,"\n\n N(T_L) - rozklad czasu istnienia: profil \t\t (download-start) \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DOWNLOADDATE] - Node[i].StateI[DSTART];
		Node[i].TempStateI = lifespan;
	}
	ZapiszRozkladInt(plik);






	// rozklad czasu istnienia: blog
	sprintf_s(txt,"\n\n N(T_L) - rozklad czasu istnienia: blog \t\t (download-start) \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
		Node[i].TempStateI = lifespan;
	}
	ZapiszRozkladInt(plik);



	// zaleznosc od czasu zycia
	sprintf_s(txt,"\n\n ZALEZNOSC OD CZASU ZYCIA (T_L - LIFESPAN)\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);


	// k (T_L)
	sprintf_s(txt,"\n\n k(T_L) - zaleznosc liczby polaczen od czasu zycia \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan_p = Node[i].StateI[DSTOP] - Node[i].StateI[DSTART];
		int lifespan_b = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];

		if(lifespan_p > 10000)	lifespan_p = 0;
		if(lifespan_b > 10000)	lifespan_b = 0;

		int lifespan = lifespan_p;
		if(lifespan_b > lifespan_p)
			lifespan = lifespan_b;
		
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].Connectivity);
	}
	this->ZapiszZaleznoscFOdI(plik);


	// k (T_L)
	sprintf_s(txt,"\n\n allk(T_L) - zaleznosc allk od czasu zycia \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan_p = Node[i].StateI[DSTOP] - Node[i].StateI[DSTART];
		int lifespan_b = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];

		if(lifespan_p > 10000)	lifespan_p = 0;
		if(lifespan_b > 10000)	lifespan_b = 0;

		int lifespan = lifespan_p;
		if(lifespan_b > lifespan_p)
			lifespan = lifespan_b;
		
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].StateI[ALLK]);
	}
	this->ZapiszZaleznoscFOdI(plik);



	// LiczbaWizytProfil (T_L)
	sprintf_s(txt,"\n\n LiczbaWizytProfil(T_L) - zaleznosc liczby wizyt w profilu od czasu istnienia profilu \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DOWNLOADDATE] - Node[i].StateI[DSTART];
				
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].StateI[NUMBERVISITS]);
	}
	this->ZapiszZaleznoscFOdI(plik);


	// LiczbaWizytProfil (T_L)
	sprintf_s(txt,"\n\n LiczbaWizytProfil(T_L) - zaleznosc liczby wizyt w profilu od czasu zycia \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOP] - Node[i].StateI[DSTART];
				
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].StateI[NUMBERVISITS]);
	}
	this->ZapiszZaleznoscFOdI(plik);


	// LiczbaWizytBlog (T_L)
	sprintf_s(txt,"\n\n LiczbaWizytBlog(T_L) - zaleznosc liczby wizyt w blogu od czasu istnienia bloga \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].StateI[NUMBERVISITSBLOG]);
	}
	this->ZapiszZaleznoscFOdI(plik);


	// LiczbaWizytBlog (T_L)
	sprintf_s(txt,"\n\n LiczbaWizytBlog(T_L) - zaleznosc liczby wizyt w blogu od czasu zycia \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].StateI[NUMBERVISITSBLOG]);
	}
	this->ZapiszZaleznoscFOdI(plik);




	// LiczbaWizytBlog w miesiacu (T_L)
	sprintf_s(txt,"\n\n LiczbaWizytBlog(T_L) - zaleznosc liczby wizyt w blogu w misiacu od czasu istnienia bloga \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].StateI[NUMBERVISITSBLOGMONTH]);
	}
	this->ZapiszZaleznoscFOdI(plik);


	// LiczbaWizytBlog w miesiacu (T_L)
	sprintf_s(txt,"\n\n LiczbaWizytBlog(T_L) - zaleznosc liczby wizyt w blogu w misiacu od czasu zycia \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].StateI[NUMBERVISITSBLOGMONTH]);
	}
	this->ZapiszZaleznoscFOdI(plik);




	// LiczbaArtykulow w blogu (T_L)
	sprintf_s(txt,"\n\n LiczbaArtykulowBlog(T_L) - zaleznosc liczby artykulow w blogu od czasu zycia \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].StateI[NUMBERARTICLESBLOG]);
	}
	this->ZapiszZaleznoscFOdI(plik);
	
	
	
	// LiczbaKomentarzy w blogu (T_L)
	sprintf_s(txt,"\n\n LiczbaKomentarzyBlog(T_L) - zaleznosc liczby komentarzy do bloga od czasu istnienia bloga \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].StateI[NUMBERCOMMENTSBLOG]);
	}
	this->ZapiszZaleznoscFOdI(plik);



	// LiczbaKomentarzy w blogu (T_L)
	sprintf_s(txt,"\n\n LiczbaKomentarzyBlog(T_L) - zaleznosc liczby komentarzy do bloga od czasu zycia \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateI = lifespan;
		Node[i].TempStateF = float(Node[i].StateI[NUMBERCOMMENTSBLOG]);
	}
	this->ZapiszZaleznoscFOdI(plik);




	// WLASCIWOSCI CALEJ SIECI

	sprintf_s(txt,"\n\n WLASCIWOSCI CALEJ SIECI\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);

	
	// wspolczynnik klasteryzacji
	sprintf_s(txt,"\n\n C(k) - wspolczynnik klasteryzacji od k \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	double cluster = 0.0;
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].TempStateI = Node[i].Connectivity;
		Node[i].TempStateF = LiczWspolczynnikKlasteryzacji(i);
		cluster += double(Node[i].TempStateF);
	}
	sprintf_s(txt,"C = \t %lf \n",cluster/double(NumberNodes));
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	ZapiszZaleznoscFOdI(plik);


	// sredni stopien najblizszych sasiadow
	sprintf_s(txt,"\n\n k_NN(k) - sredni stopien najblizszych sasiadow od k \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].TempStateI = Node[i].Connectivity;
		Node[i].TempStateF = LiczSredniaLiczbePolaczenSasiadow(i);
	}
	ZapiszZaleznoscFOdI(plik);





	// WLASCIWOSCI AKTYWNYCH UZYTKOWNIKOW
	
	sprintf_s(txt,"\n\n WLASCIWOSCI SIECI AKTYWNYCH UZYTKOWNIKOW\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);

	// oznaczamy uzytkownikow: 1 - aktywni, 0 - nieaktywni
	int aktywnewezly = 0;
	for(int i=0; i<NumberNodes; i++)
	{
		if( ((Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTOPBLOG] < 30) && (Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG]>0) )
			|| 
			((Node[i].StateI[DOWNLOADDATE] - Node[i].StateI[DSTOP] < 30) && (Node[i].StateI[DSTOP] - Node[i].StateI[DSTART]>0) ) )
		{
			Node[i].TempStateI = 1;
			aktywnewezly++;
		}
		else
			Node[i].TempStateI = 0;

	}
	sprintf_s(txt,"\n Liczba aktywnych wezlow \t %d \n\n",aktywnewezly);
	WriteFile(plik,txt,strlen(txt),&ile,NULL);


	// teraz uzuwamy polaczenia z nieaktywnymi wezlami
	for(int i=0; i<NumberNodes; i++)
	{
		for(int j=0; j<Node[i].Connectivity; j++)
		{
			int k = Node[i].Connection[j];

			if(Node[i].TempStateI == 0 || Node[k].TempStateI == 0)
			{
				// usuwamy polaczenie
				RemoveConnection(i,k);
			}
		}
	}
	
	
	// rozklad k
	sprintf_s(txt,"\n\nN(k)\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
		Node[i].TempStateI = Node[i].Connectivity;
	ZapiszRozkladInt(plik);
	

	// wspolczynnik klasteryzacji
	sprintf_s(txt,"\n\n C(k) - wspolczynnik klasteryzacji od k \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	cluster = 0.0;
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].TempStateI = Node[i].Connectivity;
		Node[i].TempStateF = LiczWspolczynnikKlasteryzacji(i);
		cluster += double(Node[i].TempStateF);
	}
	sprintf_s(txt,"C = \t %lf \n",cluster/double(NumberNodes));
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	ZapiszZaleznoscFOdI(plik);
	

	// sredni stopien najblizszych sasiadow
	sprintf_s(txt,"\n\n k_NN(k) - sredni stopien najblizszych sasiadow od k \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].TempStateI = Node[i].Connectivity;
		Node[i].TempStateF = LiczSredniaLiczbePolaczenSasiadow(i);
	}
	ZapiszZaleznoscFOdI(plik);


	
	// zamykamy plik
	CloseHandle(plik);
}

int CUniversalNetwork::RemoveConnection(int from, int to)
{
	if(from >= 0 && from < NumberNodes)
		if(to >= 0 && to < NumberNodes)
		{
			Node[from].RemoveConnection(to);
			Node[to].RemoveConnection(from);
		}
	return 0;
}

float CUniversalNetwork::LiczWspolczynnikKlasteryzacji(int num)
{
	float cluster = 0.0f;
	
	if(Node[num].Connectivity >= 2)
	{
		int liczbapolaczen = this->LiczbLiczbePolaczenPomiedzySasiadami(num);
		int k = Node[num].Connectivity;
		cluster = float(2 * liczbapolaczen) / float ( k*(k-1) );
	}
	else
		cluster = 0.0f;

	
	return cluster;
}

float CUniversalNetwork::LiczSredniaLiczbePolaczenSasiadow(int num)
{
	float knn = 0.0f;
	int kn = 0;
	
	if(Node[num].Connectivity > 0)
	{
		for(int i=0; i<Node[num].Connectivity; i++)
		{
			int target = Node[num].Connection[i];
			kn += Node[target].Connectivity;
		}

		knn = float(kn)/float(Node[num].Connectivity);
	}
	
	return knn;
}

int CUniversalNetwork::BadajSiecSkyRockArticle(void)
{
	
	Load("network.dat");


	DWORD ile;
	char txt[300];

	// otwieramy plik
	HANDLE plik = CreateFile("wyniki-skyrock.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);


	sprintf_s(txt,"skyrock - wyniki\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);

	sprintf_s(txt,"KORELACJE dla ARTICLE\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);

	//-------------------------------------------------------------------------------------------------------------------------------
	
	// LiczbaWizyt (LiczbaArtykulow)
	sprintf_s(txt,"\n\n LiczbaWizyt(LiczbaArtykulow); LiczbaWizyt>0 \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		//int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateF = float(Node[i].StateI[NUMBERVISITSBLOG]);

		if(Node[i].TempStateF > 0)
			Node[i].TempStateI = Node[i].StateI[NUMBERARTICLESBLOG];
		else
			Node[i].TempStateI = -1;

		if(Node[i].TempStateI > 100)
			Node[i].TempStateI = 10 * ( Node[i].TempStateI/10 );
		if(Node[i].TempStateI > 1000)
			Node[i].TempStateI = 100 * ( Node[i].TempStateI/100 );
	}
	this->ZapiszZaleznoscFOdI(plik);


	//-------------------------------------------------------------------------------------------------------------------------------
	/*
	// LiczbaWizyt (LiczbaArtykulow)
	sprintf_s(txt,"\n\n LiczbaWizyt(LiczbaArtykulow) \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		//int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateF = float(Node[i].StateI[NUMBERVISITSBLOG]);
		
		Node[i].TempStateI = Node[i].StateI[NUMBERARTICLESBLOG];
	}
	this->ZapiszZaleznoscFOdI(plik);
	*/
	

	//-------------------------------------------------------------------------------------------------------------------------------
	
	// LiczbaKomentarzy (LiczbaArtykulow)
	sprintf_s(txt,"\n\n LiczbaKomentarzy(LiczbaArtykulow); LiczbaKomentarzy>0 \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		//int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateF = float(Node[i].StateI[NUMBERCOMMENTSBLOG]);

		if(Node[i].TempStateF > 0)
			Node[i].TempStateI = Node[i].StateI[NUMBERARTICLESBLOG];
		else
			Node[i].TempStateI = -1;

		if(Node[i].TempStateI > 100)
			Node[i].TempStateI = 10 * ( Node[i].TempStateI/10 );
		if(Node[i].TempStateI > 1000)
			Node[i].TempStateI = 100 * ( Node[i].TempStateI/100 );
	}
	this->ZapiszZaleznoscFOdI(plik);


	//-------------------------------------------------------------------------------------------------------------------------------
	/*
	// LiczbaWizyt (LiczbaArtykulow)
	sprintf_s(txt,"\n\n LiczbaKomentarzy(LiczbaArtykulow) \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		//int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateF = float(Node[i].StateI[NUMBERCOMMENTSBLOG]);
		
		Node[i].TempStateI = Node[i].StateI[NUMBERARTICLESBLOG];
	}
	this->ZapiszZaleznoscFOdI(plik);
	*/
	
	/*
	// LiczbaArtykulow w blogu (T_L)
	for(int tl=0; tl<100; tl++)
	{
		sprintf_s(txt,"\n\n LiczbaArtykulowBlog - T_L = %d \n",tl);
		WriteFile(plik,txt,strlen(txt),&ile,NULL);
		for(int i=0; i<NumberNodes; i++)
		{
			int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
					
			if(lifespan == tl)
				Node[i].TempStateI = Node[i].StateI[NUMBERARTICLESBLOG];
			else
				Node[i].TempStateI = -1;
		}
		ZapiszRozkladInt(plik);
	}
	*/

	CloseHandle(plik);
	
	
	return 0;
}



int CUniversalNetwork::BadajSiecSkyRockDynamika(void)
{
	
	Load("network.dat");


	DWORD ile;
	char txt[300];

	// otwieramy plik
	HANDLE plik = CreateFile("wyniki-skyrock.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);


	sprintf_s(txt,"skyrock - wyniki\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);

	sprintf_s(txt,"ZALEZNOSC OD CZASU Z PODZIALEM NA GRUPY\n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);



//-------------------------------------------------------------------------------------------------------------------------------
	
	// LiczbaWizytBlog (T_L)
	sprintf_s(txt,"\n\n LiczbaWizytBlog(T_L) - zaleznosc liczby wizyt w blogu od czasu istnienia bloga, X<100 \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateF = float(Node[i].StateI[NUMBERVISITSBLOG]);

		if(Node[i].TempStateF < 100)
			Node[i].TempStateI = lifespan;
		else
			Node[i].TempStateI = -1;
	}
	this->ZapiszZaleznoscFOdI(plik);


	// LiczbaWizytBlog (T_L)
	sprintf_s(txt,"\n\n LiczbaWizytBlog(T_L) - zaleznosc liczby wizyt w blogu od czasu istnienia bloga, X > 100 \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateF = float(Node[i].StateI[NUMBERVISITSBLOG]);

		if(Node[i].TempStateF > 100)
			Node[i].TempStateI = lifespan;
		else
			Node[i].TempStateI = -1;
	}
	this->ZapiszZaleznoscFOdI(plik);



//-------------------------------------------------------------------------------------------------------------------------------
	

	
	// LiczbaArtykulow w blogu (T_L)
	sprintf_s(txt,"\n\n LiczbaArtykulowBlog(T_L) - zaleznosc liczby artykulow w blogu od czasu zycia; X < 100 \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateF = float(Node[i].StateI[NUMBERARTICLESBLOG]);

		if(Node[i].TempStateF < 100)
			Node[i].TempStateI = lifespan;
		else
			Node[i].TempStateI = -1;
	}
	this->ZapiszZaleznoscFOdI(plik);
	
	
	// LiczbaArtykulow w blogu (T_L)
	sprintf_s(txt,"\n\n LiczbaArtykulowBlog(T_L) - zaleznosc liczby artykulow w blogu od czasu zycia; X > 100 \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DSTOPBLOG] - Node[i].StateI[DSTARTBLOG];
				
		Node[i].TempStateF = float(Node[i].StateI[NUMBERARTICLESBLOG]);

		if(Node[i].TempStateF > 100)
			Node[i].TempStateI = lifespan;
		else
			Node[i].TempStateI = -1;
	}
	this->ZapiszZaleznoscFOdI(plik);
	


//-------------------------------------------------------------------------------------------------------------------------------

	// LiczbaKomentarzy w blogu (T_L)
	sprintf_s(txt,"\n\n LiczbaKomentarzyBlog(T_L) - zaleznosc liczby komentarzy do bloga od czasu istnienia bloga; X < 100 \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		
		Node[i].TempStateF = float(Node[i].StateI[NUMBERCOMMENTSBLOG]);

		if(Node[i].TempStateF < 100)
			Node[i].TempStateI = lifespan;
		else
			Node[i].TempStateI = -1;
	}
	this->ZapiszZaleznoscFOdI(plik);


	// LiczbaKomentarzy w blogu (T_L)
	sprintf_s(txt,"\n\n LiczbaKomentarzyBlog(T_L) - zaleznosc liczby komentarzy do bloga od czasu istnienia bloga; X > 100 \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		int lifespan = Node[i].StateI[DOWNLOADDATEBLOG] - Node[i].StateI[DSTARTBLOG];
				
		
		Node[i].TempStateF = float(Node[i].StateI[NUMBERCOMMENTSBLOG]);

		if(Node[i].TempStateF > 100)
			Node[i].TempStateI = lifespan;
		else
			Node[i].TempStateI = -1;
	}
	this->ZapiszZaleznoscFOdI(plik);

//-------------------------------------------------------------------------------------------------------------------------------
	
	




	CloseHandle(plik);
	
	
	return 0;
}
int CUniversalNetwork::BadajSiecGrono(void)
{
	Load("network.dat");


	// otwieramy plik
	char txt[300];
	DWORD ile;
	HANDLE plik = CreateFile("wyniki - grono active nodes.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	
	sprintf_s(txt,"grono - wyniki dla aktywnych wezlow \n\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);

	
	
	int today = 2007*365 + 4*30;
	int maxday = 0;

	for(int i=0; i<NumberNodes; i++)
	{
		if(Node[i].StateI[GRONO_DSTOP] > maxday) 
			maxday = Node[i].StateI[GRONO_DSTOP];
	}

	if(maxday > today)	maxday = today;

	
	// oznaczamy uzytkownikow: 1 - aktywni, 0 - nieaktywni
	int aktywnewezly = 0;
	for(int i=0; i<NumberNodes; i++)
	{
		if( (maxday - Node[i].StateI[GRONO_DSTOP] < 30) && (Node[i].StateI[GRONO_DSTOP] - Node[i].StateI[GRONO_DSTART] > 0) )
		{
			Node[i].TempStateI = 1;
			aktywnewezly++;
		}
		else
			Node[i].TempStateI = 0;

	}
	sprintf_s(txt,"\n Liczba aktywnych wezlow \t %d \n\n",aktywnewezly);
	WriteFile(plik,txt,strlen(txt),&ile,NULL);


	// teraz uzuwamy polaczenia z nieaktywnymi wezlami
	for(int i=0; i<NumberNodes; i++)
	{
		for(int j=0; j<Node[i].Connectivity; j++)
		{
			int k = Node[i].Connection[j];

			if(Node[i].TempStateI == 0 || Node[k].TempStateI == 0)
			{
				// usuwamy polaczenie
				RemoveConnection(i,k);
			}
		}
	}
	


	// zapisujemy
	Save("grono_active_nodes.dat");



	
	// rozklad k
	sprintf_s(txt,"\n\nN(k)\n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
		Node[i].TempStateI = Node[i].Connectivity;
	ZapiszRozkladInt(plik);
	

	// wspolczynnik klasteryzacji
	sprintf_s(txt,"\n\n C(k) - wspolczynnik klasteryzacji od k \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	double cluster = 0.0;
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].TempStateI = Node[i].Connectivity;
		Node[i].TempStateF = LiczWspolczynnikKlasteryzacji(i);
		cluster += double(Node[i].TempStateF);
	}
	sprintf_s(txt,"C = \t %lf \n",cluster/double(aktywnewezly));
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	ZapiszZaleznoscFOdI(plik);
	

	// sredni stopien najblizszych sasiadow
	sprintf_s(txt,"\n\n k_NN(k) - sredni stopien najblizszych sasiadow od k \n");
	WriteFile(plik,txt,strlen(txt),&ile,NULL);
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].TempStateI = Node[i].Connectivity;
		Node[i].TempStateF = LiczSredniaLiczbePolaczenSasiadow(i);
	}
	ZapiszZaleznoscFOdI(plik);


	
	// zamykamy plik
	CloseHandle(plik);
	
	return 0;
}

float CUniversalNetwork::LiczWspKlasteryzacjiWezla(int id)
	{
	if(id < 0 || id >= NumberNodes)
		return 0;

	
	if(Node[id].Connectivity > 0)
		{
			int E = 0;
			int k = Node[id].Connectivity;

			for(int j=0; j<k; j++)	// petla po sasiadach num-tego
			{
				// sasiad j-ty sprawdza czy nie jest polaczony do innego sasiada num-tego
				int numTemp = Node[id].Connection[j]; // jest to numer j-tego sasiada num-tego
				for(int i=j+1; i<k; i++)
				{
					for(int z=0;z<Node[numTemp].Connectivity; z++) //petla po sasiadach j-tego
					{
						if(Node[numTemp].Connection[z] == Node[id].Connection[i]) 
							E++;
						// sprawdzamy czy j-ty sasiad jest polaczony z i-tym sasiadem
					}
				} // end i
			} // end j

			float C = double(2*E)/double(k*(k-1));
			
			return C;
		}


	return 0;
	}

double CUniversalNetwork::LiczWspKlasteryzacjiSieci(void)
	{

	double C = 0;

	if(NumberNodes > 0)
		{
		for(int i = 0; i<NumberNodes; i++)
			C += LiczWspolczynnikKlasteryzacji(i);//LiczWspKlasteryzacjiWezla(i);
		C /= double(NumberNodes);
		}

	return C;
	}






float CUniversalNetwork::LiczSredniaDroge(int giantcomponent, HANDLE plik)
{
	
	// Alokacja pamieci, poniewaz ilosc osobnikow mogla sie zmienic
	int * NowiSasiedzi = new int[NumberNodes];
	if(NowiSasiedzi == NULL) return 0;

	int * Sasiedzi = new int[NumberNodes];
	if(Sasiedzi == NULL) return 0;

	_int64 * PstwoDroga = new _int64[NumberNodes];
	if(PstwoDroga == NULL) return 0;

	long double suma;
	
	_int64 SredniaDrogaInt;
	_int64 sumaInt;

		
	int AktualnyProcent = 0;
	int AktualnyProcentL = 0;

	_int64 i,j,x,Num,Droga;
	_int64 LiczbaSasiadow,NowaLiczbaSasiadow;
	int * tempPointer;
	_int64 SumaDrog; // to zawiera  wysumowane drogi
	_int64 LiczbaDrog; //

	

	for(i=0;i<NumberNodes; i++)	PstwoDroga[i] = 0;

	_int64 MaxDroga = 0;
	SumaDrog = 0;
	LiczbaDrog = 0;



	if(giantcomponent < 0)
		giantcomponent = SeekComponentsFast();
	
	

	for(Num=0; Num<NumberNodes; Num++)
	if(Node[Num].TempStateI == giantcomponent)
	{
	
		//	sav<<endl<<Num<<endl<<int(OsobnikIloscPolaczen[Num])<<endl<<endl;
		
				// zerowanie stanu
		for(i=0; i<NumberNodes; i++) Node[i].TempStateF = -1;

		Droga = 0;
		// warunek poczatkowy - jeden wezel , droga 0
		Sasiedzi[0] = Num;
		LiczbaSasiadow = 1;
		Node[Num].TempStateF = 1.0f;
		
		// liczymy tak dlugo dopuki nie bedzie nowych sasiadow
		
		do
		{
			NowaLiczbaSasiadow = 0;
			// numerujemy po aktualnym rozprzestrzeniajacym sie stanie 1

			for(i=0;i<LiczbaSasiadow;i++)
			{
			for(j=0;j<Node[Sasiedzi[i]].Connectivity; j++)
				{
					// szukamy najblizszych sasiadow aktualnego frontu
					// ktorych jeszcze nie zliczylismy
					int x = Node[Sasiedzi[i]].Connection[j];
					if( Node[x].TempStateF < 0.0f) 
					{	
						Node[x].TempStateF = 1.0f;
						NowiSasiedzi[NowaLiczbaSasiadow] = x;
						NowaLiczbaSasiadow ++ ;
					}

				} // end j
			} // end i
			
			// akwizycja danych , wymiana pointerow
			Droga++;
			SumaDrog += unsigned long(Droga*NowaLiczbaSasiadow);
			LiczbaDrog ++;
			
			PstwoDroga[Droga] += NowaLiczbaSasiadow;
			LiczbaSasiadow = NowaLiczbaSasiadow;
			
			tempPointer = Sasiedzi;
			Sasiedzi = NowiSasiedzi;
			NowiSasiedzi = tempPointer;

	//		sav<<Droga<<"\t"<<NowaLiczbaSasiadow<<endl;


		} while (NowaLiczbaSasiadow!=0);


	if(Droga>MaxDroga) MaxDroga = Droga;


	
	if(plik)
	if(NumberNodes > 10000)
	if( int(float(Num)/float(NumberNodes)*10000.0f) > AktualnyProcentL )
		{
			AktualnyProcentL = int(float(Num)/float(NumberNodes)*10000.0f);
			char text[200];
			DWORD ile;
			
				
				{
					suma = 0;
					long double SredniaDroga = 0;
					SredniaDrogaInt = 0;
					sumaInt = 0;
						for(i=1;i<=MaxDroga; i++)
						{
							SredniaDrogaInt += _int64(i)*_int64(PstwoDroga[i]);
							sumaInt += _int64(PstwoDroga[i]);

							int pd = int(PstwoDroga[i]);
							float fpd = float(PstwoDroga[i]);

							sprintf(text,"\t \t %d \t %d \t %lf \n",i,pd,fpd);
							WriteFile(plik,text,strlen(text),&ile,NULL);
						}
	
					SredniaDroga = long double(SredniaDrogaInt)/long double(sumaInt);
					long double SumaWezlow = double( suma/double( (NumberNodes-1)*(NumberNodes) ) );
					
					sprintf(text," %.2lf %% \t <L> = %lf \t %lf \t %lf \n\n\n",float(Num)/float(NumberNodes)*100.0f,SredniaDroga,SumaWezlow,
						long double(SumaDrog) / long double(LiczbaDrog) );
					WriteFile(plik,text,strlen(text),&ile,NULL);

				}
				
		
		}

		

	} // end num

	
	//SredniaDroga = double(SumaDrog)/double( (IloscOsobnikow-1)*	(IloscOsobnikow) );
	
	
	suma = 0;
	long double SredniaDroga = 0;
	SredniaDrogaInt = 0;
	sumaInt = 0;
	
	//ofstream save;
	//save.open("spr.txt",ios::out | ios::app );


	for(i=1;i<=MaxDroga; i++)
	{
		//SredniaDroga += double(i*PstwoDroga[i]);
		SredniaDrogaInt += _int64(i)*_int64(PstwoDroga[i]);
		
		//suma += double(PstwoDroga[i]);
		sumaInt += _int64(PstwoDroga[i]);
	}
	
	SredniaDroga = long double(SredniaDrogaInt)/long double(sumaInt);
	
	double SumaWezlow = double( suma/double( (NumberNodes-1)*(NumberNodes) ) );




	delete [] NowiSasiedzi;
	delete [] Sasiedzi;
	delete [] PstwoDroga;

	return SredniaDroga;


}





