#include "StdAfx.h"
#include "NetworkPOLYMOD.h"

CNetworkPOLYMOD::CNetworkPOLYMOD(void)
: MaksymalnaLiczbaPolaczenPM(150)
, RozmiarSieciPM(10000)
, PstwoItoR(0)
, PstwoStoI(0)
, stepPM(0)
, MaxLiczbakrokow(1000)
, NumberInfected(0)
, NumberInfectedMax(0)
, TimeNumberInfectedMax(0)
, InitiallyInfected(1)
, m_bWiekWezla(false)
, m_bSiecDynamiczna(true)
, StaleWszystkie(0)
, m_dSredniaWaga(0)
, m_boolSredniaWaga(false)
, m_iLiczbaPowtorzenPM(100)
, m_iZasieg(0)
, PstwoSzczepien(0)
, m_hLogFile(NULL)
, m_iLifeSpan(0)
, m_fLifeSpan(0)
, m_bIsingTest(false)
, m_fMagetyzacjaCzas(NULL)
{
	// wagi polaczen
	/* ³¹czny czas spêdzony z osob¹ w ci¹gu dnia (
	1 'Mniej ni¿ 5 min'; 
	2 '5 - 15 min'; 
	3 '15 min - 1 godz.';   
	4 '1 - 4 godz.'; 
	5 'Ponad 4 godz.';     
	9 'Brak danych'
	*/

	double maxtime = 24 * 60.0;
	waga_polaczenia[0] = 5.0 / maxtime;			//1 'Mniej ni¿ 5 min'; - 5 / 4*60;
	waga_polaczenia[1] = 10.0 / maxtime;		//2 '5 - 15 min';
	waga_polaczenia[2] = 40.0 / maxtime;		//'15 min - 1 godz.'
	waga_polaczenia[3] = 2.5*60.0 / maxtime;	//'1 - 4 godz.
	waga_polaczenia[4] = 4.0*60.0 / maxtime;	//'Ponad 4 godz.' 
	waga_polaczenia[5] = 0;
	waga_polaczenia[6] = 0;
	waga_polaczenia[7] = 0;
	waga_polaczenia[8] = 0;
	waga_polaczenia[9] = 0;


	lTrollOutputFile = NULL;
}

CNetworkPOLYMOD::~CNetworkPOLYMOD(void)
{
	if(lTrollOutputFile)
		CloseHandle(lTrollOutputFile);
}

int CNetworkPOLYMOD::BudujSiecPM(void)
{
	int LiczbaCech = MaksymalnaLiczbaPolaczenPM + LICZBACECHMAX;
	int LiczbaCechFloat = MaksymalnaLiczbaPolaczenPM;
	int PoczatkowaLiczbaPolaczen = MaksymalnaLiczbaPolaczenPM;
	int N = RozmiarSieciPM;
	
	this->DestroyNetwork();
	this->CreateNetwork(N,LiczbaCech,LiczbaCechFloat,PoczatkowaLiczbaPolaczen);
	
	return 0;
}


int CNetworkPOLYMOD::NextStepADepidemic(void)
{
	
	// ustawiamy nextstate
	for(int i=0; i<NumberNodes; i++)
		Node[i].StateI[NEXTSTATE] = Node[i].StateI[STATE];
	
	
	// po dodaniu polaczen obliczamy pstwo zmiany stanu
	int nInfected = 0; // liczba zainfekowanych wezlow
	int kall;
	for(int i=0; i<NumberNodes; i++)
	{
		switch(Node[i].StateI[STATE])
		{
		case SUSCEPTIBLE:
			kall = Node[i].Connectivity;
			for(int k=0; k<kall; k++)
			{
				int sasiad = Node[i].Connection[k];
				if( Node[sasiad].StateI[STATE] == INFECTED )
				{
					// tu trzeba dodac dodatkowo zaleznosc od wieku
					if( GetRand() < PstwoStoI )
					{
						Node[i].StateI[NEXTSTATE] = INFECTED;
						Node[i].StateI[WASINFECTED] = 1;
						
						m_iAgeInfection[ Node[sasiad].StateI[AGE] ] ++;

						k = kall + 1;
						break;
					}
				}
			}
			break;
		
		case INFECTED:
			if(GetRand() < PstwoItoR)
				Node[i].StateI[NEXTSTATE] = RESISTED;
			break;
		
		case RESISTED:
			break;
		}
	}

	// uaktualniamy stan i przy okazji zliczamy liczbe zainfekowanych wezlow
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].StateI[STATE] = Node[i].StateI[NEXTSTATE];

		if(Node[i].StateI[STATE] == INFECTED)
			nInfected++;
	}

	NumberInfected = nInfected;

	return nInfected;
}


int CNetworkPOLYMOD::NextStepPM(void)
{
	// tworzymy polaczenia przypadkowe
	TworzPolaczeniaTymczasowePM();

	// sprawdzamy czy mamy zastosowac srednia wage
	if(m_boolSredniaWaga)
	{
		for(int i=0; i<NumberNodes; i++)
		{
			int kall = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1] + Node[i].StateI[TEMPCONNECTIONS2];
			for(int k=0; k<kall; k++)
			{
				Node[i].StateF[k] = float( m_dSredniaWaga );
			}
		}
	}

	// ustawiamy nextstate
	for(int i=0; i<NumberNodes; i++)
		Node[i].StateI[NEXTSTATE] = Node[i].StateI[STATE];
	
	
	// po dodaniu polaczen obliczamy pstwo zmiany stanu
	int nInfected = 0; // liczba zainfekowanych wezlow
	int kall;
	for(int i=0; i<NumberNodes; i++)
	{
		switch(Node[i].StateI[STATE])
		{
		case SUSCEPTIBLE:
			kall = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1] + Node[i].StateI[TEMPCONNECTIONS2];
			for(int k=0; k<kall; k++)
			{
				int sasiad = Node[i].Connection[k];
				if( Node[sasiad].StateI[STATE] == INFECTED )
				{
					// tu trzeba dodac dodatkowo zaleznosc od wieku
					if( GetRand() < PstwoStoI * Node[i].StateF[k])
					{
						Node[i].StateI[NEXTSTATE] = INFECTED;
						Node[i].StateI[WASINFECTED] = 1;
						
						k = kall +1;
					}
				}
			}
			break;
		
		case INFECTED:
			if(GetRand() < PstwoItoR)
				Node[i].StateI[NEXTSTATE] = RESISTED;
			break;
		
		case RESISTED:
			break;
		}
	}

	// uaktualniamy stan i przy okazji zliczamy liczbe zainfekowanych wezlow
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].StateI[STATE] = Node[i].StateI[NEXTSTATE];

		if(Node[i].StateI[STATE] == INFECTED)
			nInfected++;
	}

	this->NumberInfected = nInfected;

	return nInfected;
}



int CNetworkPOLYMOD::SymulacjaADEpidemie(void)
{
	bool targetVaccination = false;
	VaccinatedNodesAverageK = 0;
	NumberOfVaccinated = 0;

	// inicjalizacja stanu wezlow
	for(int i =0; i<NumberNodes; i++)
	{
		Node[i].StateI[STATE] = SUSCEPTIBLE;
		Node[i].StateI[WASINFECTED] = 0;

		// losujemy szczepienia
		if(!targetVaccination)
		if(GetRand() < PstwoSzczepien)
		{
			Node[i].StateI[STATE] = RESISTED;
			NumberOfVaccinated++;
			VaccinatedNodesAverageK += Node[i].Connectivity;
		}
	}

	if(NumberOfVaccinated>0)
			VaccinatedNodesAverageK /= float(NumberOfVaccinated);

	if(targetVaccination && PstwoSzczepien > 0)
	{
		// szczepimy najpierw te z najwieksza liczba polaczen
		int * id = new int [NumberNodes];
		for(int i=0; i<NumberNodes; i++)
			id[i] = 0;
		
		// sortujemy
		int n = 0;
		
		
		for(int k = 50; k>0; k--)
		{
			for(int i=0; i<NumberNodes  &&  n<NumberNodes; i++)
			{
				//if(Node[i].StateI[MAXK] == k)
				if(Node[i].Connectivity == k)
				{
					id[n] = i;
					n++;
				}
			}
		}
		

		// szczepimy
		int N = int( float(NumberNodes) * PstwoSzczepien );
		if(N >= NumberNodes)
			N = NumberNodes - 1;
		

		

		for(int i=0; i<N; i++)
		{
			if(id[i] > 0 && id[i] < NumberNodes)
			{
				Node[id[i]].StateI[STATE] = RESISTED;

				NumberOfVaccinated++;
				VaccinatedNodesAverageK += Node[id[i]].Connectivity;
				//VaccinatedNodesAverageK += Node[id[i]].StateI[MAXK];
				
			}
		}

		if(NumberOfVaccinated>0)
			VaccinatedNodesAverageK /= float(NumberOfVaccinated);

		// zwlaniamy pamiec
		delete [] id;
	}

	
	if(m_iSourceAge>=0)
	{
		// zliczamy wezly o odpowiednim wieku
		int agecount = 0;
		for(int i =0; i<NumberNodes; i++)
		{
			if(Node[i].StateI[AGE] == m_iSourceAge)
				agecount++;
		}

		if(agecount > 0)
		for(int j=0; j<InitiallyInfected; j++)
		{
			int num = int( GetRand() * double(agecount-1) );
			int n = 0;

			for(int i =0; i<NumberNodes; i++)
			{
				if(Node[i].StateI[AGE] == m_iSourceAge)
				{
					if(n == num)
					{
						Node[i].StateI[STATE] = INFECTED;
						i = NumberNodes;
					}
					n++;
				}
			}
		}
	}
	else
	{
		for(int i=0; i<InitiallyInfected; i++)
		{
			int num = int( GetRand() * double(NumberNodes-1) );
			Node[num].StateI[STATE] = INFECTED;
		}
	}

	
	NumberInfectedMax = 0;
	TimeNumberInfectedMax = 0;
	stepPM = 0;

	// krok symulacji
	while(NextStepADepidemic() > 0 && stepPM < MaxLiczbakrokow)
	{
		if(NumberInfected > NumberInfectedMax)
		{
			NumberInfectedMax = NumberInfected;
			TimeNumberInfectedMax = stepPM;
		}
		stepPM++;
	}
	
	m_iZasieg = 0;
	for(int i =0; i<NumberNodes; i++)
	{
		m_iZasieg += Node[i].StateI[WASINFECTED];
	}

	return 0;
}


int CNetworkPOLYMOD::SymulacjaPM(void)
{

	bool targetVaccination = true;
	VaccinatedNodesAverageK = 0;
	NumberOfVaccinated = 0;

	// inicjalizacja stanu wezlow
	for(int i =0; i<NumberNodes; i++)
	{
		Node[i].StateI[STATE] = SUSCEPTIBLE;
		Node[i].StateI[WASINFECTED] = 0;

		// losujemy szczepienia
		if(!targetVaccination)
		if(GetRand() < PstwoSzczepien)
		{
			Node[i].StateI[STATE] = RESISTED;
			NumberOfVaccinated++;
			VaccinatedNodesAverageK += Node[i].Connectivity;
		}
	}

	if(NumberOfVaccinated>0)
			VaccinatedNodesAverageK /= float(NumberOfVaccinated);

	if(targetVaccination)
	{
		// szczepimy najpierw te z najwieksza liczba polaczen
		int * id = new int [NumberNodes];
		for(int i=0; i<NumberNodes; i++)
			id[i] = 0;
		
		// sortujemy
		int n = 0;
		

		

		
		for(int k = 50; k>0; k--)
		{
			for(int i=0; i<NumberNodes  &&  n<NumberNodes; i++)
			{
				//if(Node[i].StateI[MAXK] == k)
				if(Node[i].Connectivity == k)
				{
					id[n] = i;
					n++;
				}
			}
		}
		

		// szczepimy
		int N = int( float(NumberNodes) * PstwoSzczepien );
		if(N >= NumberNodes)
			N = NumberNodes - 1;
		

		

		for(int i=0; i<N; i++)
		{
			if(id[i] > 0 && id[i] < NumberNodes)
			{
				Node[id[i]].StateI[STATE] = RESISTED;

				NumberOfVaccinated++;
				VaccinatedNodesAverageK += Node[id[i]].Connectivity;
				//VaccinatedNodesAverageK += Node[id[i]].StateI[MAXK];
				
			}
		}

		if(NumberOfVaccinated>0)
			VaccinatedNodesAverageK /= float(NumberOfVaccinated);

		// zwlaniamy pamiec
		delete [] id;
	}

	
	for(int i=0; i<InitiallyInfected; i++)
	{
		int num = int( GetRand() * double(NumberNodes-1) );
		Node[num].StateI[STATE] = INFECTED;
	}

	
	NumberInfectedMax = 0;
	TimeNumberInfectedMax = 0;
	stepPM = 0;

	// krok symulacji
	while(NextStepPM() > 0 && stepPM < MaxLiczbakrokow)
	{
		if(NumberInfected > NumberInfectedMax)
		{
			NumberInfectedMax = NumberInfected;
			TimeNumberInfectedMax = stepPM;
		}
		stepPM++;
	}
	
	m_iZasieg = 0;
	for(int i =0; i<NumberNodes; i++)
	{
		m_iZasieg += Node[i].StateI[WASINFECTED];
	}

	return 0;
}

int CNetworkPOLYMOD::TworzPolaczeniaAgeDegree(void)
{
	int sumaK = 0;

	

	// tworzymy wektor identyfikatorow w grupach wiekowych
	std::vector< std::vector<int> > ageID;
	std::vector<int> intv;

	for(int i=0; i<100; i++)
	{
		ageID.push_back(intv);
	}

	for(int ii=0; ii<m_vecIdAge.size(); ii++)
	{
		int a = m_vecIdAge[ii];
		if(a>=0 && a<100)
			ageID[a].push_back(ii);
	}
	//std::vector<int> m_vecIdAge;
	//std::vector<std::vector<int>> m_vecIdContactAge;
	
	int maxAge = 0;

	// zerujemy polaczenia
	// generujemy maksymalna liczbe polaczen
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].Connectivity = 0;
		Node[i].StateI[MAXK] = 0;//LosujMaxK_PM(StaleWszystkie,Node[i].StateI[AGE]);
		
		// losujemy wiek
		int age = mtAge.GetRand(GetRand());
		Node[i].StateI[AGE] = age;

		if(age > maxAge)
			maxAge = age;

		if(age>=0 && age<100)
		{
			int n = int( ageID[age].size() * GetRand() );
			if(n < ageID[age].size())
			{
				int id = ageID[age][n];
				Node[i].StateI[MAXK] = m_vecIdContactAge[id].size();
				Node[i].PreallocateMemmory(Node[i].StateI[MAXK] + 5);
				for(int k=0; k<m_vecIdContactAge[id].size(); k++)
				{
					Node[i].Connection[k] = m_vecIdContactAge[id][k];
				}
			}
		}

		// wypelniamy tabele kontaktow na podstawie wieku
		// TO DO !!!
		
		
		sumaK += Node[i].StateI[MAXK];
	}

	// w przypadku braku korelacji age-degree ustawiamy wszystkie grupy wiekowe na 0
	if(!m_bWiekWezla)
	{
		for(int i=0; i<NumberNodes; i++)
			Node[i].StateI[AGE] = 0;
	}


	// tworzemy tablice wezlow na podstawie grup wiekowych
	// TO DO !!!
	std::vector< std::vector <int> > ageTab;
	std::vector< int > ageTabSize;

	for(int i=0; i<100; i++)
	{
		std::vector <int> v;
		ageTab.push_back(v);
		ageTabSize.push_back(0);
	}
	
	for(int i=0; i<NumberNodes; i++)
	{
		int age = Node[i].StateI[AGE];
		if(age >= 0 && age < 100)
		{
			ageTab[age].push_back(i);
			ageTabSize[age]++;
		}
	}



	// teraz dodajemy polaczenia
	// uwaga trzeba zoptymalizowac ta procedure
	int NN = NumberNodes;
	int * tab = new int [NumberNodes+1];
	for(int i=0; i<NumberNodes; i++)	tab[i] = i;




	for(int a=0; a<maxAge; a++)
		for(int  ac=0; ac<maxAge; ac++)
		{
			aacSize[a][ac]=0;
			vecAgeConnectionAge[a][ac].clear();
		}

	

	for(int i=0; i<NumberNodes; i++)	
	{
		int maxk = Node[i].StateI[MAXK];
		int a = Node[i].StateI[AGE];
		for(int k=0; k<maxk; k++)
		{
			int ac = Node[i].Connection[k];
			vecAgeConnectionAge[a][ac].push_back(i);
			aacSize[a][ac]++;
		}
	}

	char ttt[500];

	//sprintf(ttt,"sumK: %d",sumaK);
	//lWriteToLog(ttt);

	//for(int i=0; i<20; i++)
	{
		//sprintf(ttt,"");
		//for(int j=0; j<20; j++)
		//	sprintf(ttt,"%s\t%d",ttt,aacSize[i][j]);
		//lWriteToLog(ttt);
	}

	int minK = sumaK * 0.001;

	if(!m_bWiekWezla)
		minK = sumaK * 0.13;

	if(m_bWiekWezla)
	while(sumaK > minK)
	{
		int dk = sumaK;

		for(int iter=0; iter < 10; iter++)
		for(int a=0; a<maxAge; a++)
		for(int ac=0; ac<maxAge; ac++)
		{
			//int a = int( double(maxAge) * GetRand() );
			//int ac = int( double(maxAge) * GetRand() );

				if(aacSize[a][ac] > 0)
				{
					int n = aacSize[a][ac] - 1;
					int from = vecAgeConnectionAge[a][ac][n];
								
					if(aacSize[ac][a] > 0)
					{
						int n2 = aacSize[ac][a];
						n2 = int( double(n2) * GetRand() );
						if(n2 >= aacSize[ac][a])
							n2 = aacSize[ac][a] - 1;
					
						int r = TworzPolaczenieAgeDegree(n, n2, a, ac);
						if(r>0)
							sumaK -= r;
					}
					else
					{
						// nie ma sasiada, usuwamy polaczenie
						for(int k = Node[from].Connectivity; k < Node[from].StateI[MAXK]; k++)
							if(Node[from].Connection[k] == ac)
							{
								Node[from].StateI[MAXK]--;
								int last = Node[from].StateI[MAXK];
								Node[from].Connection[k] = Node[from].Connection[last];
								sumaK--;
								aacSize[a][ac]--;
								break;
							}
					
					}
				}
			
		}

		dk -= sumaK;

		
		if(dk <= 0 && m_bWiekWezla)
		{
			//sprintf(ttt,"sumK: %d; \t its impossible to make new connections",sumaK);
			//lWriteToLog(ttt);
			
			int r0 = 0;
			int r1 = 0;
			int r2 = 0;
			int r3 = 0;

			//for(int ii=0; ii<5; ii++)
			for(int a=0; a<maxAge; a++)
			for(int ac=0; ac<maxAge; ac++)
			for(int fromIter = aacSize[a][ac]-1; fromIter>=0; fromIter--)
			{
				int from = vecAgeConnectionAge[a][ac][fromIter];
								
					for(int n2 = aacSize[ac][a]-1; n2>=0 ; n2 --)
					{
						int result = TworzPolaczenieAgeDegree(fromIter, n2, a, ac);
						
						if(result==0)
							r0 ++;

						if(result==-1)
							r1 ++;

						if(result==-2)
							r2 ++;

						if(result > 0)
						{
							sumaK -= result;
							r3++;
							break;
						}
					}
			}

			

			//sprintf(ttt,"sumK: %d; \t its impossible to make new connections",sumaK);
			//lWriteToLog(ttt);

			//sprintf(ttt,"r: \t %d \t %d \t %d \t %d",r0,r1,r2,r3);
			//lWriteToLog(ttt);
			
			sumaK = 0;
		}
		
	}
	


	if(!m_bWiekWezla)
	while(sumaK > minK)
	{
		for(int i=0; i<NN; i++)
			{
				// uwaga!!! trzeba poprawic warunek stopu!!!
				int from = tab[i];
				int id = int( double(NN) * GetRand() );
					if(id >= NN)	id = NN-1;
				int to = tab[id];
				int zwrot = ProbujPolaczyc(from,to,true);
				sumaK -= 2*zwrot;

				// jezeli wezel nie ma juz wolnych polaczen, to wykreslamy go z listy
				if(Node[from].Connectivity >= Node[from].StateI[MAXK])
				{
					NN--;
					tab[i] = tab[NN];
				}

				if(Node[to].Connectivity >= Node[to].StateI[MAXK])
				{
					NN--;
					tab[id] = tab[NN];
				}
			}
	}

	//lWriteToLog("delete [] tab");
	delete [] tab;

	
	//lWriteToLog("FINISHED\n");
	

	//for(int i=0; i<20; i++)
	//{
	//	sprintf(ttt,"");
	//	for(int j=0; j<20; j++)
	//		sprintf(ttt,"%s\t%d",ttt,aacSize[i][j]);
	//	lWriteToLog(ttt);
	//}
	

	return 0;
}


int CNetworkPOLYMOD::TworzPolaczenieAgeDegree(int n, int n2, int a, int ac)
{
	int from = vecAgeConnectionAge[a][ac][n];
	int to = vecAgeConnectionAge[ac][a][n2];

						// probujemy polaczyc from z to
						int kfrom = -1;
						int kto = -1;

						if(from == to)
							return -1;

						
					
						for(int k = 0; k < Node[from].Connectivity; k++)
							if(Node[from].Connection[k] == to)
							{
								return -2;
								break;
							}
					
						{
							for(int k = Node[from].Connectivity; k < Node[from].StateI[MAXK]; k++)
							if(Node[from].Connection[k] == ac)
							{
								kfrom = k;
								break;
							}

							for(int k = Node[to].Connectivity; k < Node[to].StateI[MAXK]; k++)
							if(Node[to].Connection[k] == a)
							{
								kto = k;
								break;
							}

							if(kfrom >=0 && kto >= 0)
							{
								// laczymy
								// from
								int first = Node[from].Connectivity;
								Node[from].Connection[kfrom] = Node[from].Connection[first];
								Node[from].Connection[first] = to;
								Node[from].Connectivity++;
								// to
								first = Node[to].Connectivity;
								Node[to].Connection[kto] = Node[to].Connection[first];
								Node[to].Connection[first] = from;
								Node[to].Connectivity++;
								//
								//sumaK -= 2;
								aacSize[a][ac]--;
								aacSize[ac][a]--;

								
								vecAgeConnectionAge[a][ac][n] = vecAgeConnectionAge[a][ac][aacSize[a][ac]];
								vecAgeConnectionAge[ac][a][n2] = vecAgeConnectionAge[ac][a][aacSize[ac][a]];
								

								//sprintf(ttt,"%d",sumaK);
								//lWriteToLog(ttt);
								
								int result = 2;
								

								// probujemy laczyc sasiadow
								// TO DO !!!

								
									for(int k2 = 0; k2 < Node[to].Connectivity; k2 ++)
									{
										int to2 = Node[to].Connection[k2];

										// probujemy laczyc from z to2
										if(from != k2)
										{
											// sprawdzamy czy wezly sa juz ze soba polaczone
											bool flag = true;

											for(int kk = 0; kk < Node[from].Connectivity; kk++)
											if(Node[from].Connection[kk] == to2)
											{
												flag = false;
												kk = Node[from].Connectivity;
												break;
											}

											if(flag)
											for(int k = Node[from].Connectivity; k < Node[from].StateI[MAXK]; k++)
											{
												if(Node[from].Connection[k] == Node[to2].StateI[AGE])
												{
													for(int k2 = Node[to2].Connectivity; k2 < Node[to].StateI[MAXK]; k2++)
													if(Node[to2].Connection[k2] == Node[from].StateI[AGE])
													{
														// mamy polaczenie
														// tworzymy polaczenie
														result += 2;

														Node[from].Connection[k] = Node[from].Connection[Node[from].Connectivity];
														Node[from].Connection[Node[from].Connectivity] = to2;
														Node[from].Connectivity++;

														Node[to2].Connection[k2] = Node[to2].Connection[Node[to2].Connectivity];
														Node[to2].Connection[Node[to2].Connectivity] = from;
														Node[to2].Connectivity++;

														// zmianiamy wartosc tablic
														int a2 = Node[from].StateI[AGE];
														int ac2 = Node[to2].StateI[AGE];

														for(int ii=0; ii<aacSize[a2][ac2]; ii++)
															if(vecAgeConnectionAge[a2][ac2][ii] == from)
															{
																aacSize[a2][ac2]--;
																vecAgeConnectionAge[a2][ac2][ii] = vecAgeConnectionAge[a2][ac2][aacSize[a2][ac2]];
																ii = aacSize[a2][ac2] + 10;
																break;
															}

														a2 = Node[to2].StateI[AGE];
														ac2 = Node[from].StateI[AGE];
														for(int ii=0; ii<aacSize[a2][ac2]; ii++)
															if(vecAgeConnectionAge[a2][ac2][ii] == to2)
															{
																aacSize[a2][ac2]--;
																vecAgeConnectionAge[a2][ac2][ii] = vecAgeConnectionAge[a2][ac2][aacSize[a2][ac2]];
																ii = aacSize[a2][ac2] + 10;
																break;
															}
														//vecAgeConnectionAge[a][ac][n] = vecAgeConnectionAge[a][ac][aacSize[a][ac]];
														//vecAgeConnectionAge[ac][a][n2] = vecAgeConnectionAge[ac][a][aacSize[ac][a]];

														// wychodzimy z petli
														k = Node[from].StateI[MAXK];
														k2 = Node[to].StateI[MAXK];
													}
												}
											}
										}
									}


									return result;
							}
						}

						return 0;
}



int CNetworkPOLYMOD::TworzPolaczeniaPM(void)
{
	int sumaK = 0;

	// sprawdzamy czy siec jest dynamiczna
	if(m_bSiecDynamiczna)
		StaleWszystkie = STALE;
	else
		StaleWszystkie = WSZYSTKIE;
	
	// generujemy rozklad wiekowy wezlow
	LosujWiekPM();
	 

	
	// zerujemy polaczenia
	// generujemy maksymalna liczbe polaczen
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].Connectivity = 0;
		
		Node[i].StateI[MAXK] = LosujMaxK_PM(StaleWszystkie,Node[i].StateI[AGE]);
		Node[i].StateI[TEMPCONNECTIONS1] = 0;
		Node[i].StateI[TEMPCONNECTIONS2] = 0;
		sumaK += Node[i].StateI[MAXK];

		// generujemy odpowiednie parametry polaczenia
		for(int k=0; k<Node[i].StateI[MAXK]; k++)
		{
			Node[i].StateF[k] = LosujWagePolaczeniaPM(StaleWszystkie);	
			Node[i].StateI[k + LICZBACECHMAX] = LosujWiekSasiadaPM(i,StaleWszystkie);	
		}
	}

	


	// teraz dodajemy polaczenia
	// uwaga trzeba zoptymalizowac ta procedure
	int NN = NumberNodes;
	int * tab = new int [this->NumberNodes+1];
	for(int i=0; i<NumberNodes; i++)	tab[i] = i;

	while(sumaK > 100  &&  NN > 50)
	{
		for(int i=0; i<NN; i++)
			{
				// uwaga!!! trzeba poprawic warunek stopu!!!
				int from = tab[i];
				int id = int( double(NN) * GetRand() );
					if(id >= NN)	id = NN-1;
				int to = tab[id];
				int zwrot = ProbujPolaczyc(from,to,true);
				sumaK -= 2*zwrot;

				// jezeli wezel nie ma juz wolnych polaczen, to wykreslamy go z listy
				if(Node[from].Connectivity >= Node[from].StateI[MAXK])
				{
					NN--;
					tab[i] = tab[NN];
				}

				if(Node[to].Connectivity >= Node[to].StateI[MAXK])
				{
					NN--;
					tab[id] = tab[NN];
				}
			}
	}
	delete [] tab;

	

	// -------------------------------------------------------------------------------------------------------------------------------
	// TEST !!!
	/*
	for(int i=0; i<NumberNodes; i++)
	{
		int kall = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1] + Node[i].StateI[TEMPCONNECTIONS2];
		
		// generujemy odpowiednie parametry polaczenia
		for(int k=0; k<kall; k++)
		{
			Node[i].StateF[k] = LosujWagePolaczeniaPM(StaleWszystkie);	
		}
	}
	*/
	// TEST !!!
	// -------------------------------------------------------------------------------------------------------------------------------

	// sprawdzamy czy mamy zastosowac srednia wage
	if(m_boolSredniaWaga)
	{
		for(int i=0; i<NumberNodes; i++)
		{
			int kall = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1] + Node[i].StateI[TEMPCONNECTIONS2];
			for(int k=0; k<kall; k++)
			{
				Node[i].StateF[k] = float( m_dSredniaWaga );
			}
		}
	}


	return 0;
}



int CNetworkPOLYMOD::LosujWiekPM(void)
{
	// zerujemy grupe wiekowa
	if(m_bWiekWezla)
	{
		for(int i=0; i<NumberNodes; i++)
		{
			Node[i].StateI[AGE] = mtAge.GetRand(GetRand());
		}
	}
	else
		for(int i=0; i<NumberNodes; i++)
			Node[i].StateI[AGE] = 0;

	return 0;
}


int CNetworkPOLYMOD::LosujMaxK_PM(int id, int age)
{
	double r = GetRand();
	if(!m_bWiekWezla)
	{
		// nie bierzemy pod uwage struktury wiekowej
		return mtPodK[id].GetRand(r);
	}
	else
	{
		// UWAGA !!! to trzeba zmienic - funkcja powinna wiedziec jaki jest wiek wezla
		return this->mtPodAK[id][age].GetRand(r);
		// return mtPodK[id].GetRand(r);
	}
}


float CNetworkPOLYMOD::LosujWagePolaczeniaPM(int id)
{
	double r = GetRand();
	int num = mtPodW[id].GetRand(r);

	
	return waga_polaczenia[num];
}


int CNetworkPOLYMOD::LosujWiekSasiadaPM(int nodeID, int typ)
{
	
	if(m_bWiekWezla)
	{
		// bierzemy pod uwage wiek wezlow
		int a = Node[nodeID].StateI[AGE];
		double r = GetRand();

		return mtPodAA[typ][a].GetRand( r );
	}
	else	
	{
		// tylko jedna mozliwa wartosc - brak struktury wiekowej
		return 0;
	}
	
	
	return 0; //mtAge.GetRand(GetRand());
}

int CNetworkPOLYMOD::ProbujPolaczyc(int from, int to, bool deeper)
{
	int liczbapolaczen = 0;
	
	if(from == to)
		return 0;

	if(Node[to].Connectivity >= Node[to].StateI[MAXK])
		return 0;

	if(Node[from].Connectivity >= Node[from].StateI[MAXK])
		return 0;

	
	// sprawdzamyc czy nie jest on juz naszym sasiadem
	for(int k=0; k<Node[from].Connectivity; k++)
	{
		if(Node[from].Connection[k] == to)
			return 0;
	}


	// probujemy sie polaczyc z tym wezlem
	int new_neighbor = -1;

	// sprawdzamy czy ktores polaczenie odpowiada pod wzgledem wieku i wagi
	for(int k=Node[from].Connectivity; k<Node[from].StateI[MAXK]; k++)
	{
		for(int kto=Node[to].Connectivity; kto<Node[to].StateI[MAXK]; kto++)
		{

			// sprawdzamy wiek
			//if(Node[from].StateI[AGE] == Node[to].StateI[LICZBACECHMAX + kto])
			//if(Node[to].StateI[AGE] == Node[from].StateI[LICZBACECHMAX + k])
			{
				// sprawdzamy wage
				//if(Node[from].StateF[k] == Node[to].StateF[kto])
				{
					
					// laczymy
					liczbapolaczen++;
					new_neighbor = to;
					ConnectSafe(from,to);
					
					// losujemy wage
					float w = LosujWagePolaczeniaPM(StaleWszystkie);
					
					// przesuwamy wage i wiek
					//FROM
					int connectivity = Node[from].Connectivity - 1;
					//int age = Node[from].StateI[LICZBACECHMAX + k];
					//Node[from].StateI[LICZBACECHMAX + k] = Node[from].StateI[LICZBACECHMAX + connectivity];
					//Node[from].StateI[LICZBACECHMAX + connectivity] = age;
					//float waga = Node[from].StateF[k];
					//Node[from].StateF[k] = Node[from].StateF[connectivity]; 
					//Node[from].StateF[connectivity] = waga;
					Node[from].StateF[connectivity] = w;
					
					// TO
					connectivity = Node[to].Connectivity - 1;
					//age = Node[to].StateI[LICZBACECHMAX + kto];
					//Node[to].StateI[LICZBACECHMAX + kto] = Node[to].StateI[LICZBACECHMAX + connectivity];
					//Node[to].StateI[LICZBACECHMAX + connectivity] = age;
					//waga = Node[to].StateF[kto];
					//Node[to].StateF[kto] = Node[to].StateF[connectivity]; 
					//Node[to].StateF[connectivity] = waga;
					Node[to].StateF[connectivity] = w;

					// wychodzimy z petel
					k = Node[from].StateI[MAXK] + 1;
					kto = Node[to].StateI[MAXK] + 1;
				}

			}

		}
	}



	// probujemy sie polaczyc z ssasiadami tego wezla
	if(deeper && new_neighbor >= 0)
	{
		// petla po sasiadach nowego sasiada
		for(int k=0; k<Node[new_neighbor].Connectivity; k++)
		{
			liczbapolaczen += ProbujPolaczyc(from,Node[new_neighbor].Connection[k],false);
		}
	}

	
	return liczbapolaczen;
}


int CNetworkPOLYMOD::ProbujPolaczycAgeDegree(int from, int to, bool deeper)
{
	int liczbapolaczen = 0;
	
	if(from == to)
		return 0;

	if(Node[to].Connectivity >= Node[to].StateI[MAXK])
		return 0;

	if(Node[from].Connectivity >= Node[from].StateI[MAXK])
		return 0;

	
	// sprawdzamyc czy nie jest on juz naszym sasiadem
	for(int k=0; k<Node[from].Connectivity; k++)
	{
		if(Node[from].Connection[k] == to)
			return 0;
	}


	// probujemy sie polaczyc z tym wezlem
	int new_neighbor = -1;

	// sprawdzamy czy ktores polaczenie odpowiada pod wzgledem wieku i wagi
	for(int k = Node[from].Connectivity; k<Node[from].StateI[MAXK]; k++)
	if(Node[from].Connection[k] == Node[to].StateI[AGE])
	{
		for(int kto=Node[to].Connectivity; kto<Node[to].StateI[MAXK]; kto++)
		{

			// sprawdzamy wiek
			if(Node[from].StateI[AGE] == Node[to].Connection[kto])
			{
					// laczymy
					liczbapolaczen++;
					new_neighbor = to;

					// przesuwamy wage i wiek
					//FROM
					int connectivity = Node[from].Connectivity;
					Node[from].Connection[connectivity] = to;
					Node[from].Connectivity++;
					
					// TO
					connectivity = Node[to].Connectivity;
					Node[to].Connection[kto] = Node[to].Connection[connectivity];
					Node[to].Connection[connectivity] = from;
					Node[to].Connectivity++;
					
					// wychodzimy z petel
					kto = Node[to].StateI[MAXK] + 1;
					k = Node[from].StateI[MAXK] + 1;
					break;
			}

		}
	}



	// probujemy sie polaczyc z sasiadami tego wezla
	if(deeper && new_neighbor >= 0)
	{
		// petla po sasiadach nowego sasiada
		for(int k=0; k<Node[new_neighbor].Connectivity; k++)
		{
			liczbapolaczen += ProbujPolaczycAgeDegree(from,Node[new_neighbor].Connection[k],false);
		}
	}

	
	return liczbapolaczen;
}


int CNetworkPOLYMOD::WczytajDaneAgeDegree(void)
{
	HANDLE poda = CreateFile("p_od_a.txt",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	HANDLE IdAge = CreateFile("IdAge.txt",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	HANDLE IdContactAge = CreateFile("IdContactAge.txt",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	bool endfile;


	//std::vector<int> m_vecIdAge;
	//std::vector<std::vector<int>> m_vecIdContactAge;
	m_vecIdAge.clear();
	m_vecIdContactAge.clear();

	int maxID = 0;

	std::vector<int> ageVec;
	std::vector<int> idVec;


	//----------------------------------------------------------------------------------------------------
	// teraz struktura demograficzna
	int numk = ReadInt(poda,&endfile);
	mtAge.Create(numk);
	for(int i = 0; i<numk; i++)
		mtAge.tabi[i] = ReadInt(poda,&endfile);
	mtAge.CountSumI();
	
	//----------------------------------------------------------------------------------------------------
	// IdAge
	idVec.clear();
	ageVec.clear();
	numk = ReadInt(IdAge,&endfile);
	for(int i = 0; i<numk; i++)
	{
		idVec.push_back(ReadInt(IdAge,&endfile));
		ageVec.push_back(ReadInt(IdAge,&endfile));
	}
	for(int i = 0; i<idVec.size(); i++)
	{
		if(idVec[i] > maxID)
			maxID = idVec[i];
	}

	for(int i = 0; i<=maxID; i++)
		m_vecIdAge.push_back(0);

	for(int i = 0; i<idVec.size(); i++)
	{
		m_vecIdAge[ idVec[i] ] = ageVec[i];
	}

	//----------------------------------------------------------------------------------------------------
	// IdContactAge
	idVec.clear();
	ageVec.clear();
	numk = ReadInt(IdContactAge,&endfile);
	for(int i = 0; i<numk; i++)
	{
		idVec.push_back(ReadInt(IdContactAge,&endfile));
		ageVec.push_back(ReadInt(IdContactAge,&endfile));
	}
	for(int i = 0; i<idVec.size(); i++)
	{
		if(idVec[i] > maxID)
			maxID = idVec[i];
	}
	std::vector<int> v;
	for(int i = 0; i<=maxID; i++)
		m_vecIdContactAge.push_back(v);

	for(int i = 0; i<idVec.size(); i++)
	{
		m_vecIdContactAge[ idVec[i] ].push_back( ageVec[i] );
	}
	//----------------------------------------------------------------------------------------------------
	
	CloseHandle(poda);
	CloseHandle(IdAge);
	CloseHandle(IdContactAge);
	

	return 0;
}


int CNetworkPOLYMOD::WczytajDanePM(void)
{
	HANDLE poda = CreateFile("p_od_a.txt",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	HANDLE podk = CreateFile("p_od_k.txt",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	HANDLE podw = CreateFile("p_od_w.txt",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	HANDLE podak = CreateFile("p_od_age_and_k.txt",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	HANDLE podaa = CreateFile("p_od_age_and_age.txt",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	bool endfile;

	//----------------------------------------------------------------------------------------------------
	// wczytujemy podk, najpierw kontakty stale i ich liczbe
	int numk = 0;
	for(int c=0; c<4; c++)
	{
		numk = ReadInt(podk,&endfile);
		mtPodK[c].Create(numk);
		for(int i = 0; i<numk; i++)
			mtPodK[c].tabi[i] = ReadInt(podk,&endfile);
		mtPodK[c].CountSumI();
	}
	
	//----------------------------------------------------------------------------------------------------

	// teraz wagi
	for(int c=0; c<4; c++)
	{
		numk = ReadInt(podw,&endfile);
		mtPodW[c].Create(numk);
		for(int i = 0; i<numk; i++)
			mtPodW[c].tabi[i] = ReadInt(podw,&endfile);
		mtPodW[c].CountSumI();
	}

	//----------------------------------------------------------------------------------------------------

	// teraz zaleznosc od k i wieku
	// WSZYSTKIE, STALE, SPORADYCZNE, PRZYPADKOWE
	
	for(int typ=0; typ<4; typ++)
	{
		for(int c=0; c<20; c++)
		{
			numk = ReadInt(podak,&endfile);
			mtPodAK[typ][c].Create(numk);
			for(int i = 0; i<numk; i++)
				mtPodAK[typ][c].tabi[i] = ReadInt(podak,&endfile);
			mtPodAK[typ][c].CountSumI();
		}
	}

	
	//----------------------------------------------------------------------------------------------------

	// teraz zaleznosc od wieku i wieku
	// WSZYSTKIE, STALE, SPORADYCZNE, PRZYPADKOWE
	
	for(int typ=0; typ<4; typ++)
	{
		for(int c=0; c<20; c++)
		{
			numk = ReadInt(podaa,&endfile);
			mtPodAA[typ][c].Create(numk);
			for(int i = 0; i<numk; i++)
				mtPodAA[typ][c].tabi[i] = ReadInt(podaa,&endfile);
			mtPodAA[typ][c].CountSumI();
		}
	}

	//----------------------------------------------------------------------------------------------------

	// teraz struktura demograficzna
	numk = ReadInt(poda,&endfile);
	mtAge.Create(numk);
	for(int i = 0; i<numk; i++)
		mtAge.tabi[i] = ReadInt(poda,&endfile);
	mtAge.CountSumI();
	
	//----------------------------------------------------------------------------------------------------

	CloseHandle(podk);
	CloseHandle(poda);
	CloseHandle(podw);
	CloseHandle(podak);
	CloseHandle(podaa);



	// na koniec liczymy srednia wage
	m_dSredniaWaga = 0;
	for(int i = 0; i<mtPodW[0].tablengthI; i++)
		m_dSredniaWaga += double( mtPodW[0].tabi[i] ) * waga_polaczenia[i]; 
	m_dSredniaWaga /= double (mtPodW[0].sumI);
	



	return 0;
}

// w kazdym kroku tworzy polaczenia sporadyczne z sasiadami sasiadow
int CNetworkPOLYMOD::PrzylaczDoSasiadowPM(int id)
{
	if(Node[id].StateI[TEMPCONNECTIONS1] >= Node[id].StateI[TEMPCONNECTIONS_MAX1])
		return 0;

	// UWAGA!!! nalezaloby randomizowac liste sasiadow, zeby nie zaczynac zawsze od tego samego
	
	for(int k=0; k<Node[id].Connectivity; k++)
	{
		int sasiad = Node[id].Connection[k];
		for(int n=0; n<Node[sasiad].Connectivity; n++)
		{
			// sasiad sasiada
			int nn = Node[sasiad].Connection[n];

			// sprawdzamy czy mozemy sie polaczyc
			if(Node[nn].StateI[TEMPCONNECTIONS1] < Node[nn].StateI[TEMPCONNECTIONS_MAX1])
			if(id != nn)
			{
				bool flaga = true;
				// teraz sprawdzamy czy nie jestesmy juz polaczeni
				int d = Node[nn].Connectivity + Node[nn].StateI[TEMPCONNECTIONS1];

				for(int jj=0; jj< d; jj++)
				{
						if(Node[nn].Connection[jj + d] == id)
						{
							flaga = false;
							break;
						}
				}

				// sprawdzamy czy mozemy sie polaczyc ze wzgledu na wiek
				int na_id = -1; // id polaczenia wiekowego
				int na_nn = -1;
				if(flaga)
				{
					int start_jj = Node[id].Connectivity + Node[id].StateI[TEMPCONNECTIONS1] + LICZBACECHMAX;
					int max_jj = Node[id].Connectivity + Node[id].StateI[TEMPCONNECTIONS_MAX1] + LICZBACECHMAX;
					for(int jj = start_jj; jj < max_jj; jj++)
					{
						if(Node[id].StateI[jj] == Node[nn].StateI[AGE])
						{
							na_id = jj;
							break;
						}
					}

					start_jj = Node[nn].Connectivity + Node[nn].StateI[TEMPCONNECTIONS1] + LICZBACECHMAX;
					max_jj = Node[nn].Connectivity + Node[nn].StateI[TEMPCONNECTIONS_MAX1] + LICZBACECHMAX;
					for(int jj = start_jj; jj < max_jj; jj++)
					{
						if(Node[nn].StateI[jj] == Node[id].StateI[AGE])
						{
							na_nn = jj;
							break;
						}
					}
				}
				

						if(flaga && na_id > 0 && na_nn > 0)
						{
							// generujemy wage
							float w = this->LosujWagePolaczeniaPM(SPORADYCZNE);
							
							// dodajemy polaczenie i jego wage
							int num = Node[nn].Connectivity + Node[nn].StateI[TEMPCONNECTIONS1];
							Node[nn].StateI[TEMPCONNECTIONS1]++;
							Node[nn].Connection[num] = id;
							Node[nn].StateF[num] = w;

							num = Node[id].Connectivity + Node[id].StateI[TEMPCONNECTIONS1];
							Node[id].StateI[TEMPCONNECTIONS1]++;
							Node[id].Connection[num] = nn;
							Node[id].StateF[num] = w;

							// jezeli nie ma wiecej wolnych miejsc na polaczenia wychodzimy
							if(Node[id].StateI[TEMPCONNECTIONS1] >= Node[id].StateI[TEMPCONNECTIONS_MAX1])
								return 0;
						}
					
			}
		}
	}
	
	return 0;
}




int CNetworkPOLYMOD::DodajPolaczeniePrzypadkowe(int from, int to)
{
	if(from == to)	return 0;

	// sprawdzamy czy wezly nie sa juz polaczone
	int kfrom = Node[from].Connectivity + Node[from].StateI[TEMPCONNECTIONS1]  + Node[from].StateI[TEMPCONNECTIONS2];
	for(int i = 0; i<kfrom; i++)
	{
		if(Node[from].Connection[i] == to)
			return 0;
	}


		// sprawdzamy czy wiek sie zgadza
	int nn_from = -1;
	int nn_to = -1;
	
	int start_to = Node[to].Connectivity + Node[to].StateI[TEMPCONNECTIONS1] + Node[to].StateI[TEMPCONNECTIONS2];
	int start_from = Node[from].Connectivity + Node[from].StateI[TEMPCONNECTIONS1] + Node[from].StateI[TEMPCONNECTIONS2];

	int kto = Node[to].Connectivity + Node[to].StateI[TEMPCONNECTIONS1] + Node[to].StateI[TEMPCONNECTIONS_MAX2];
	kfrom = Node[from].Connectivity + Node[from].StateI[TEMPCONNECTIONS1] + Node[from].StateI[TEMPCONNECTIONS_MAX2];
	
	for(int i = start_from; i<kfrom; i++)
		for(int j = start_to; j<kto; j++)
		{
			if( Node[from].StateI[AGE] == Node[to].StateI[LICZBACECHMAX + j])
				if( Node[to].StateI[AGE] == Node[from].StateI[LICZBACECHMAX + i])
				{
					nn_from = i;
					nn_to = j;
					i = kfrom+1;
					break;
				}
		}

	if(nn_from < 0 || nn_to < 0)
		return 0;

	// zamieniamy wiek wezla
	int aget = Node[from].StateI[LICZBACECHMAX + nn_from];
	Node[from].StateI[LICZBACECHMAX + nn_from] = Node[from].StateI[LICZBACECHMAX + start_from];
	Node[from].StateI[LICZBACECHMAX + start_from] = aget;

	aget = Node[to].StateI[LICZBACECHMAX + nn_to];
	Node[to].StateI[LICZBACECHMAX + nn_to] = Node[to].StateI[LICZBACECHMAX + start_to];
	Node[to].StateI[LICZBACECHMAX + start_to] = aget;



	// losujemy wage
	float w = this->LosujWagePolaczeniaPM(PRZYPADKOWE);
	
	// dodajemy polaczenie
	int num = Node[from].Connectivity + Node[from].StateI[TEMPCONNECTIONS1]  + Node[from].StateI[TEMPCONNECTIONS2];
	Node[from].StateI[TEMPCONNECTIONS2]++;
	Node[from].Connection[num] = to;
	Node[from].StateF[num] = w;

	num = Node[to].Connectivity + Node[to].StateI[TEMPCONNECTIONS1]  + Node[to].StateI[TEMPCONNECTIONS2];
	Node[to].StateI[TEMPCONNECTIONS2]++;
	Node[to].Connection[num] = to;
	Node[to].StateF[num] = w;

	return 1;
}




int CNetworkPOLYMOD::DodajPolaczenieSporadyczne(int from, int to)
{
	if(from == to)	return 0;

	// sprawdzamy czy wezly nie sa juz polaczone
	int kfrom = Node[from].Connectivity + Node[from].StateI[TEMPCONNECTIONS1];
	for(int i = 0; i<kfrom; i++)
	{
		if(Node[from].Connection[i] == to)
			return 0;
	}

	// sprawdzamy czy wiek sie zgadza
	int nn_from = -1;
	int nn_to = -1;
	
	int start_to = Node[to].Connectivity + Node[to].StateI[TEMPCONNECTIONS1];
	int start_from = Node[from].Connectivity + Node[from].StateI[TEMPCONNECTIONS1];

	int kto = Node[to].Connectivity + Node[to].StateI[TEMPCONNECTIONS_MAX1];
	kfrom = Node[from].Connectivity + Node[from].StateI[TEMPCONNECTIONS_MAX1];
	
	for(int i = start_from; i<kfrom; i++)
		for(int j = start_to; j<kto; j++)
		{
			if( Node[from].StateI[AGE] == Node[to].StateI[LICZBACECHMAX + j])
				if( Node[to].StateI[AGE] == Node[from].StateI[LICZBACECHMAX + i])
				{
					nn_from = i;
					nn_to = j;
					i = kfrom+1;
					break;
				}
		}

	if(nn_from < 0 || nn_to < 0)
		return 0;

	// zamieniamy wiek wezla
	int aget = Node[from].StateI[LICZBACECHMAX + nn_from];
	Node[from].StateI[LICZBACECHMAX + nn_from] = Node[from].StateI[LICZBACECHMAX + start_from];
	Node[from].StateI[LICZBACECHMAX + start_from] = aget;

	aget = Node[to].StateI[LICZBACECHMAX + nn_to];
	Node[to].StateI[LICZBACECHMAX + nn_to] = Node[to].StateI[LICZBACECHMAX + start_to];
	Node[to].StateI[LICZBACECHMAX + start_to] = aget;


	// losujemy wage
	float w = LosujWagePolaczeniaPM(SPORADYCZNE);
	
	// dodajemy polaczenie
	int num = Node[from].Connectivity + Node[from].StateI[TEMPCONNECTIONS1];//  + Node[from].StateI[TEMPCONNECTIONS2];
	Node[from].StateI[TEMPCONNECTIONS1]++;
	Node[from].Connection[num] = to;
	Node[from].StateF[num] = w;

	num = Node[to].Connectivity + Node[to].StateI[TEMPCONNECTIONS1];
	Node[to].StateI[TEMPCONNECTIONS1]++;
	Node[to].Connection[num] = to;
	Node[to].StateF[num] = w;

	return 1;
}




int CNetworkPOLYMOD::TworzPolaczeniaTymczasowePM(void)
{
	
	// sprawdzamy czy siec jest dynamiczna
	if(!m_bSiecDynamiczna)
	{
		// siec nie jest dynamiczna, wiec nie tworzymy polaczen
		for(int i = 0; i<NumberNodes; i++)
		{
			// sporadyczne
			Node[i].StateI[TEMPCONNECTIONS_MAX1] = 0;
			Node[i].StateI[TEMPCONNECTIONS1] = 0;

			Node[i].StateI[TEMPCONNECTIONS_MAX2] = 0;
			Node[i].StateI[TEMPCONNECTIONS2] = 0;
		}
		return 0;
	}
	
	
	
	
	
	// Siec jest dynamiczna - tworzymy polaczenia	
	// potrzebujemy tabeli identyfikatorow wezlow, ktore maja wolne polaczenia przypadkowe
	int freenodes = 0;
	int * tab = new int [NumberNodes];
	int * tf = new int [NumberNodes];
	
	// najpierw dodajemy polaczenia 
	// kontakty sporadyczne i przypadkowe
	for(int i = 0; i<NumberNodes; i++)
	{
		int ageid = Node[i].StateI[AGE];
		// sporadyczne
		if(!m_bWiekWezla)
			Node[i].StateI[TEMPCONNECTIONS_MAX1] = mtPodK[SPORADYCZNE].GetRand( GetRand() );
		else
			Node[i].StateI[TEMPCONNECTIONS_MAX1] = mtPodAK[SPORADYCZNE][ageid].GetRand( GetRand() );
		Node[i].StateI[TEMPCONNECTIONS1] = 0;
		
		// przypadkowe
		if(!m_bWiekWezla)
			Node[i].StateI[TEMPCONNECTIONS_MAX2] = mtPodK[PRZYPADKOWE].GetRand( GetRand() );
		else
			Node[i].StateI[TEMPCONNECTIONS_MAX2] = mtPodAK[PRZYPADKOWE][ageid].GetRand( GetRand() );
		Node[i].StateI[TEMPCONNECTIONS2] = 0;

		
		
		// losujemy wiek sasiadow
		int maxk = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS_MAX1];
		for(int k = Node[i].Connectivity; k< maxk; k++)
			Node[i].StateI[k + LICZBACECHMAX] = LosujWiekSasiadaPM(i,SPORADYCZNE);

		maxk = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS_MAX1] + Node[i].StateI[TEMPCONNECTIONS_MAX2];
		for(int k = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS_MAX1]; k< maxk; k++)
			Node[i].StateI[k + LICZBACECHMAX] = LosujWiekSasiadaPM(i,PRZYPADKOWE);

		// dodajemy wezly do tablicy
		if(Node[i].StateI[TEMPCONNECTIONS_MAX2] > 0)
		{
			tab[freenodes] = i;
			freenodes++;
		}
	}

	// laczymy te wezly, ktore maja wolne kontakty i generujemy wage z odpowiedniego rozkladu
	for(int i = 0; i<NumberNodes; i++)
		PrzylaczDoSasiadowPM(i);
	
	// te ktorym nie udalo sie przylaczyc laczymy przypadkowo
	int fn = 0;
	for(int i = 0; i<NumberNodes; i++)
		if(Node[i].StateI[TEMPCONNECTIONS1] < Node[i].StateI[TEMPCONNECTIONS_MAX1])
		{
			tf[fn] = i;
			fn++;
		}
	
	int deltafn = 1;

	while(deltafn > 0)//fn > 50)
	{
		deltafn = fn;

		for(int i = 0; i<fn; i++)
		{
			int id = tf[i];
			int sasiadnum = int(GetRand() * double(fn));
			if(sasiadnum >= fn)	sasiadnum = fn-1;
			int sasiad = tf[ sasiadnum ];
			
			if(DodajPolaczenieSporadyczne(id,sasiad) > 0)
			{
			}
			
			{
				// udalo sie polaczyc, wiec sprawdzamy czy nie trzeba usunac wezly z list freenodes
				if(Node[id].StateI[TEMPCONNECTIONS1] >= Node[id].StateI[TEMPCONNECTIONS_MAX1])
				{
					fn--;
					tf[i] = tab[fn];
				}

				if(Node[sasiad].StateI[TEMPCONNECTIONS1] >= Node[sasiad].StateI[TEMPCONNECTIONS_MAX1])
				{
					fn--;
					tf[sasiadnum] = tf[fn];
				}
			}
		}

		deltafn -= fn;
	}
	// uruchamiamy brutalne przylaczanie na zasadzie, kazdy z kazdym
	TworzPolaczeniaTymczasoweSporadycznePM();
	
	//-----------------------------------------------------------------------------------------------------------------	
		
	// teraz tworzymy polaczenia przypadkowe
	int	deltafreenodes = 1;
	while(deltafreenodes > 0)	//freenodes > 50   &&   
	{
		deltafreenodes = freenodes;
		
		for(int i = 0; i<freenodes; i++)
		{
			int id = tab[i];
			int sasiadnum = int(GetRand() * double(freenodes));
				if(sasiadnum >= freenodes)	sasiadnum = freenodes-1;
			int sasiad = tab[ sasiadnum ];
			
			if(DodajPolaczeniePrzypadkowe(id,sasiad) > 0)
			{
			}

			{
				// udalo sie polaczyc, wiec sprawdzamy czy nie trzeba usunac wezly z list freenodes
				if(Node[id].StateI[TEMPCONNECTIONS2] >= Node[id].StateI[TEMPCONNECTIONS_MAX2])
				{
					freenodes--;
					tab[i] = tab[freenodes];
				}

				if(Node[sasiad].StateI[TEMPCONNECTIONS2] >= Node[sasiad].StateI[TEMPCONNECTIONS_MAX2])
				{
					freenodes--;
					tab[sasiadnum] = tab[freenodes];
				}
			}
		}

		deltafreenodes -= freenodes;
	}
	// uruchamiamy brutalne przylaczanie na zasadzie, kazdy z kazdym
	TworzPolaczeniaTymczasowePrzypadkowePM();


	delete [] tab;
	delete [] tf;

	
	
	return 0;
}

int CNetworkPOLYMOD::UruchomSymulacje(void)
{
	// otwieramy plik
	char text[500];
	DWORD ile;
	sprintf(text,"wyniki - dynamiczna=%d - srednie wagi=%d.txt",this->m_bSiecDynamiczna, this->m_boolSredniaWaga);
	HANDLE plik = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	sprintf(text,"log - dynamiczna=%d - srednie wagi=%d.txt",this->m_bSiecDynamiczna, this->m_boolSredniaWaga);
	HANDLE log = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	PstwoItoR = 0.2;
	PstwoStoI = 0.5;
	PstwoSzczepien = 0.0;

	// petla po parametrze
	for(PstwoItoR = 0.2; PstwoItoR < 1.05; PstwoItoR += 0.4)
	for(PstwoStoI = 0.1; PstwoStoI < 1.00; PstwoStoI += 0.05)
	//for(PstwoSzczepien = 0.0; PstwoSzczepien < 1.05; PstwoSzczepien += 0.05)
	{
		int Zasieg = 0;
		int CzasMax = 0;
		// jeden punkt wykresu
		for(int i=0; i<m_iLiczbaPowtorzenPM; i++)
		{
			// tworzymy siec
			TworzPolaczeniaPM();

			float K1 = 0;
			float K2 = 0;
			//for(int i=0; i<NumberNodes; i++)
			//	K1 += Node[i].Connectivity;
			//K1 /= float(NumberNodes);

			////////////////////////////////////////////////////////////
			//for(int i=0; i<NumberNodes; i++)
			//{
			//		Node[i].TempStateI = Node[i].Connectivity;
			//}
			//ZapiszRozkladInt(plik);
			////////////////////////////////////////////////////////////

			// uruchamiamy symulacje
			SymulacjaPM();


			for(int i=0; i<NumberNodes; i++)
				K2 += Node[i].Connectivity;
			K2 /= float(NumberNodes);
			
			// dane do loga
			sprintf(text,"%lf \t %d \t %d \t %d \t %d \t %lf \t %d \t %lf \t%lf \t %lf\n",PstwoStoI,i,stepPM,m_iZasieg,TimeNumberInfectedMax,PstwoSzczepien,NumberOfVaccinated,VaccinatedNodesAverageK,K1,K2);
			WriteFile(log,text,strlen(text),&ile,NULL);

			Zasieg += this->m_iZasieg;
			CzasMax += this->TimeNumberInfectedMax;
		}

		double z = double(Zasieg) / double(m_iLiczbaPowtorzenPM) ;
		double c = double(CzasMax) / double(m_iLiczbaPowtorzenPM);

		z /= double( NumberNodes );

		// zapisujemy
		sprintf(text,"%lf \t %lf \t %lf \t %lf\n",PstwoItoR,PstwoStoI,z,c);
		WriteFile(plik,text,strlen(text),&ile,NULL);
	}


	CloseHandle(plik);
	CloseHandle(log);
	
	
	return 0;
}





int CNetworkPOLYMOD::UruchomSymulacjeRumour(void)
{
	// otwieramy plik
	char text[500];
	DWORD ile;
	sprintf(text,"wyniki - dynamiczna=%d - srednie wagi=%d.txt",this->m_bSiecDynamiczna, this->m_boolSredniaWaga);
	HANDLE plik = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	sprintf(text,"log - dynamiczna=%d - srednie wagi=%d.txt",this->m_bSiecDynamiczna, this->m_boolSredniaWaga);
	HANDLE log = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	IgnorantSpreader = 0.5;
	SpreaderStifler = 0.5;

	// petla po parametrze
	//for(PstwoStoI = 0.0; PstwoStoI < 0.45; PstwoStoI += 0.05)
	for(SpreaderStifler = 0.2; SpreaderStifler < 1.05; SpreaderStifler += 0.2)
	for(IgnorantSpreader = 0.1; IgnorantSpreader < 1; IgnorantSpreader += 0.8)
	{
		//if(IgnorantSpreader > 0.1)
		//	IgnorantSpreader += 0.04;
		

		for(int q=0; q<10; q++)
			for(int qq=0; qq<10; qq++)
			{
				aktywacjaWezla[q][qq] = 0;
				dezaktywacjaWezla[q][qq] = 0;
			}


		int Zasieg = 0;
		int CzasMax = 0;
		float avgLifeSpan = 0;

		// jeden punkt wykresu
		for(int i=0; i<m_iLiczbaPowtorzenPM; i++)
		{
			// tworzymy siec
			TworzPolaczeniaPM();

			// uruchamiamy symulacje
			SymulacjaPM_RumorPropagation();

			float avgK = 0;
			float avgK0 = 0;
			float avgK1 = 0;
			float avgK2 = 0;
			float avgW = 0;
			float avgW1 = 0;
			float avgW2 = 0;
			float avgW3 = 0;
			float avgWA = 0;
			int kallzero = 0;

			////////////////////////////////////////////////////////////
			/*for(int n=0; n<NumberNodes; n++)
			{
					Node[n].TempStateI = Node[n].Connectivity + Node[n].StateI[TEMPCONNECTIONS1] + Node[n].StateI[TEMPCONNECTIONS2];
			}
			ZapiszRozkladInt(plik);

			for(int n=0; n<NumberNodes; n++)
			{
					Node[n].TempStateI = Node[n].StateI[TEMPCONNECTIONS1];
			}
			ZapiszRozkladInt(plik);

			for(int n=0; n<NumberNodes; n++)
			{
					Node[n].TempStateI = Node[n].StateI[TEMPCONNECTIONS2];
			}
			ZapiszRozkladInt(plik);*/
			////////////////////////////////////////////////////////////

			
			
			/*
			for(int n=0; n<NumberNodes; n++)
			{
				
				
				int kall = Node[n].Connectivity + Node[n].StateI[TEMPCONNECTIONS1] + Node[n].StateI[TEMPCONNECTIONS2];
				float w = 0;
				float w1 = 0;
				float w2 = 0;
				float w3 = 0;

				for(int k=0; k<kall; k++)
					avgWA += LosujWagePolaczeniaPM(StaleWszystkie);
				
				for(int k=0; k<Node[n].Connectivity; k++)
				{
					w += Node[n].StateF[k];
					w1 += Node[n].StateF[k];
				}

				for(int k=Node[n].Connectivity; k<Node[n].Connectivity + Node[n].StateI[TEMPCONNECTIONS1]; k++)
				{
					w += Node[n].StateF[k];
					w2 += Node[n].StateF[k];
				}

				for(int k=Node[n].Connectivity + Node[n].StateI[TEMPCONNECTIONS1]; k<kall; k++)
				{
					w += Node[n].StateF[k];
					w3 += Node[n].StateF[k];
				}


				if(kall > 0)
					avgW += w;// / float(kall);
				else
					kallzero ++;
				
				if(Node[n].Connectivity > 0)
					avgW1 += w1;// / float(Node[n].Connectivity);

				if(Node[n].StateI[TEMPCONNECTIONS1] > 0)
					avgW2 += w2;// / float(Node[n].StateI[TEMPCONNECTIONS1]);

				if(Node[n].StateI[TEMPCONNECTIONS2] > 0)
					avgW3 += w3;// / float(Node[n].StateI[TEMPCONNECTIONS2]);

			

				avgK += kall;
				avgK0 += Node[n].Connectivity;
				avgK1 += Node[n].StateI[TEMPCONNECTIONS1];
				avgK2 += Node[n].StateI[TEMPCONNECTIONS2];
			}

			avgW /= avgK;//float(NumberNodes - kallzero);
			avgW1 /= avgK0;//float(NumberNodes - kallzero);
			avgW2 /= avgK1;//float(NumberNodes - kallzero);
			avgW3 /= avgK2;//float(NumberNodes - kallzero);
			avgWA /= float(avgK);

			avgK /= float(NumberNodes - kallzero);
			avgK0 /= float(NumberNodes - kallzero);
			avgK1 /= float(NumberNodes - kallzero);
			avgK2 /= float(NumberNodes - kallzero);
			*/

			// dane do loga
			sprintf(text,"%lf \t %lf \t %d \t %d \t %d \t %d \t avgK \t %lf \t %lf \t %lf \t %lf \t avgW \t %lf \t %lf \t %lf \t %lf \t %d \t %lf \t %lf \n",
				SpreaderStifler,IgnorantSpreader,i,stepPM,m_iZasieg,TimeNumberInfectedMax,avgK,avgK0,avgK1,avgK2,avgW,avgW1,avgW2,avgW3,kallzero,m_dSredniaWaga,avgWA);
			WriteFile(log,text,strlen(text),&ile,NULL);

			for(int q=0; q<3; q++)
			{
				sprintf(text,"%d \t %d \t %d \t %d \t %d \t\t %lf\n",
					aktywacjaWezlaTemp[q][0],aktywacjaWezlaTemp[q][1],aktywacjaWezlaTemp[q][2],aktywacjaWezlaTemp[q][3],aktywacjaWezlaTemp[q][4],m_fLifeSpan);
				WriteFile(log,text,strlen(text),&ile,NULL);
			}


			for(int q=0; q<10; q++)
			for(int qq=0; qq<10; qq++)
			{
				aktywacjaWezla[q][qq] += aktywacjaWezlaTemp[q][qq];
				dezaktywacjaWezla[q][qq] += dezaktywacjaWezla[q][qq];
			}

			Zasieg += this->m_iZasieg;
			CzasMax += this->TimeNumberInfectedMax;
			avgLifeSpan += m_fLifeSpan;
		}

		double z = double(Zasieg) / double(m_iLiczbaPowtorzenPM) ;
		double c = double(CzasMax) / double(m_iLiczbaPowtorzenPM);

		z /= double( NumberNodes );
		avgLifeSpan /= double(m_iLiczbaPowtorzenPM) ;



		// zapisujemy
		sprintf(text,"%lf \t %lf \t %lf \t %lf \t %lf \t",SpreaderStifler,IgnorantSpreader,z,c,avgLifeSpan);
		WriteFile(plik,text,strlen(text),&ile,NULL);

		for(int q=0; q<3; q++)
			{
				sprintf(text,"%lf \t %lf \t %lf \t %lf \t %lf \t",
					float(aktywacjaWezla[q][0])/float(m_iLiczbaPowtorzenPM),
					float(aktywacjaWezla[q][1])/float(m_iLiczbaPowtorzenPM),
					float(aktywacjaWezla[q][2])/float(m_iLiczbaPowtorzenPM),
					float(aktywacjaWezla[q][3])/float(m_iLiczbaPowtorzenPM),
					float(aktywacjaWezla[q][4])/float(m_iLiczbaPowtorzenPM)  );
				WriteFile(plik,text,strlen(text),&ile,NULL);
			}

		sprintf(text,"\n");
		WriteFile(plik,text,strlen(text),&ile,NULL);
	}


	CloseHandle(plik);
	CloseHandle(log);
	
	
	return 0;
}




int CNetworkPOLYMOD::UruchomSymulacjeAgeDegreeEpidemie(void)
{
	// otwieramy plik
	char text[500];
	DWORD ile;
	sprintf(text,"wyniki - age=%d.txt", this->m_bWiekWezla);
	HANDLE plik = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	sprintf(text,"log - age=%d.txt", this->m_bWiekWezla);
	HANDLE log = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	PstwoItoR = 1.0;
	PstwoStoI = 0.5;
	PstwoSzczepien = 0.0;


	m_iSourceAge = -1;

	// petla po parametrze
	//for(PstwoItoR = 0.2; PstwoItoR < 1.05; PstwoItoR += 0.4)
	//for(PstwoStoI = 0.05; PstwoStoI < 1.00; PstwoStoI += 0.05)
	//for(PstwoSzczepien = 0.0; PstwoSzczepien < 1.05; PstwoSzczepien += 0.05)
	
	for(PstwoStoI = 0.1; PstwoStoI < 1.00; PstwoStoI += 0.2)
	for(m_iSourceAge = 0; m_iSourceAge<20; m_iSourceAge++)
	{
		
		for(int i = 0; i<200; i++)
		{
			m_iAgeInfection[i] = 0; // liczba wezlow zainfekowanych przez wezel o wieku Age
			m_dAgeInfection[i] = 0; // liczba wezlow zainfekowanych przez wezel o wieku Age
			m_iAgeInfected[i] = 0; // liczba wezlow o wieku Age, ktore zostaly zainfekowane
			m_iAgeCount[i] = 0;	// liczba wezlow o wieku Age
			m_dAgeInfected[i] = 0; // odsetek wezlow o wieku age, ktore zostaly zainfekowane
		}

		
		int Zasieg = 0;
		int CzasMax = 0;
		// jeden punkt wykresu
		for(int i=0; i<m_iLiczbaPowtorzenPM; i++)
		{
			// tworzymy siec
			TworzPolaczeniaAgeDegree();

			float K1 = 0;
			float K2 = 0;
			//for(int i=0; i<NumberNodes; i++)
			//	K1 += Node[i].Connectivity;
			//K1 /= float(NumberNodes);

			////////////////////////////////////////////////////////////
			//for(int i=0; i<NumberNodes; i++)
			//{
			//		Node[i].TempStateI = Node[i].Connectivity;
			//}
			//ZapiszRozkladInt(plik);
			////////////////////////////////////////////////////////////

			for(int i = 0; i<200; i++)
			{
				m_iAgeInfection[i] = 0; // liczba wezlow zainfekowanych przez wezel o wieku Age
				m_iAgeInfected[i] = 0; // liczba wezlow o wieku Age, ktore zostaly zainfekowane
				m_iAgeCount[i] = 0;	// liczba wezlow o wieku Age
				//m_dAgeInfected[i] = 0; // odsetek wezlow o wieku age, ktore zostaly zainfekowane
			}

			// uruchamiamy symulacje
			SymulacjaADEpidemie();


			for(int i=0; i<NumberNodes; i++)
			{
				K2 += Node[i].Connectivity;
				
				int age = Node[i].StateI[AGE];
				m_iAgeCount[ age ] ++;
				if(Node[i].StateI[STATE] > SUSCEPTIBLE || Node[i].StateI[WASINFECTED] > 0)
					m_iAgeInfected[age]++;
			}

			K2 /= float(NumberNodes);

			for(int i=0; i<200; i++)
			{
				if(m_iAgeCount[i] > 0)
					m_dAgeInfected[i] += double(m_iAgeInfected[i]) / double(m_iAgeCount[i]);
			
				m_dAgeInfection[i] += double(m_iAgeInfection[i]);
			}
			
			// dane do loga
			sprintf(text,"%lf \t %d \t %d \t %d \t %d \t %lf \t %d \t %lf \t%lf \t %lf\n",PstwoStoI,i,stepPM,m_iZasieg,TimeNumberInfectedMax,PstwoSzczepien,NumberOfVaccinated,VaccinatedNodesAverageK,K1,K2);
			WriteFile(log,text,strlen(text),&ile,NULL);

			Zasieg += this->m_iZasieg;
			CzasMax += this->TimeNumberInfectedMax;
		}

		double z = double(Zasieg) / double(m_iLiczbaPowtorzenPM) ;
		double c = double(CzasMax) / double(m_iLiczbaPowtorzenPM);

		z /= double( NumberNodes );

		for(int i=0; i<25; i++)
		{
			m_dAgeInfected[i] /= double(m_iLiczbaPowtorzenPM) ;
			m_dAgeInfection[i] /= double(m_iLiczbaPowtorzenPM) ;

			sprintf(text,"%d \t %lf \t %lf\n",i,m_dAgeInfected[i],m_dAgeInfection[i]);
			WriteFile(log,text,strlen(text),&ile,NULL);
		}

		// zapisujemy
		sprintf(text,"%lf \t %lf \t %d \t %lf \t %lf\n",PstwoItoR,PstwoStoI,m_iSourceAge,z,c);
		WriteFile(plik,text,strlen(text),&ile,NULL);
	}


	CloseHandle(plik);
	CloseHandle(log);
	
	
	return 0;
}





int CNetworkPOLYMOD::UruchomSymulacjeAgeDegreeIsing(void)
{
	// otwieramy plik
	char text[500];
	DWORD ile;
	sprintf(text,"wyniki - age=%d.txt", this->m_bWiekWezla);
	HANDLE plik = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	sprintf(text,"log - age=%d.txt", this->m_bWiekWezla);
	HANDLE log = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	m_hLog = log;

	Temperatura = 0.0;
	PoleZewnetrzne = 0.0;

	m_fMagetyzacjaCzas = new float [2*MaxLiczbakrokow+10];
	for(int i=0; i<MaxLiczbakrokow; i++)
		m_fMagetyzacjaCzas[i] = 0;


	// alokacja pamieci
	m_fMagnetyzacjaTab = new float [10*m_iLiczbaPowtorzenPM];


	m_iSourceAge = -1;

	// petla po parametrze
	//for(PstwoItoR = 0.2; PstwoItoR < 1.05; PstwoItoR += 0.4)
	//for(PstwoStoI = 0.05; PstwoStoI < 1.00; PstwoStoI += 0.05)
	//for(PstwoSzczepien = 0.0; PstwoSzczepien < 1.05; PstwoSzczepien += 0.05)
	
	for(Temperatura = 20.0; Temperatura < 20.05; Temperatura += 0.2)
	//for(PoleZewnetrzne = 0.0; PoleZewnetrzne < 2; PoleZewnetrzne += 0.01)
	{
		/*
		if(Temperatura > 1.0f)
			Temperatura += 0.05f;

		if(Temperatura > 2.0f)
			Temperatura += 0.1f;
		

		if(PoleZewnetrzne > 1.0f)
			PoleZewnetrzne += 0.01f;

		if(PoleZewnetrzne > 10.0f)
			PoleZewnetrzne += 0.5f;
		*/

		int Zasieg = 0;
		int CzasMax = 0;
		double Magnetyzacja = 0;
		int krokPM = 0;
		// jeden punkt wykresu
		for(int i=0; i<m_iLiczbaPowtorzenPM; i++)
		{
			// tworzymy siec
			TworzPolaczeniaAgeDegree();

			float K1 = 0;
			float K2 = 0;
			//for(int i=0; i<NumberNodes; i++)
			//	K1 += Node[i].Connectivity;
			//K1 /= float(NumberNodes);

			////////////////////////////////////////////////////////////
			//for(int i=0; i<NumberNodes; i++)
			//{
			//		Node[i].TempStateI = Node[i].Connectivity;
			//}
			//ZapiszRozkladInt(plik);
			////////////////////////////////////////////////////////////

			

			// uruchamiamy symulacje
			SymulacjaADIsing();

			
			// dane do loga
			sprintf(text,"%lf \t %lf \t %d \t %d \t %d \t %d \t %lf\n",Temperatura, PoleZewnetrzne,i,stepPM,m_iZasieg,TimeNumberInfectedMax,m_fMagnetyzacja);
			WriteFile(log,text,strlen(text),&ile,NULL);

			Zasieg += this->m_iZasieg;
			CzasMax += this->TimeNumberInfectedMax;
			Magnetyzacja += abs(m_fMagnetyzacja);
			krokPM += stepPM;

			if(m_bIsingTest)
			{
				sprintf(text,"logTest - age=%d.txt", this->m_bWiekWezla);
				HANDLE logTest = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

				sprintf(text,"%d \n\n",i+1);
				WriteFile(logTest,text,strlen(text),&ile,NULL);

				for(int step = 0; step<MaxLiczbakrokow; step++)
				{
					sprintf(text,"%d \t %lf\n",step,m_fMagetyzacjaCzas[step]/float(i+1));
					WriteFile(logTest,text,strlen(text),&ile,NULL);
				}
				
				CloseHandle(logTest);
			}
		}

		double z = double(Zasieg) / double(m_iLiczbaPowtorzenPM) ;
		double c = double(Magnetyzacja) / double(m_iLiczbaPowtorzenPM);
		double k = double(krokPM) / double(m_iLiczbaPowtorzenPM);

		z /= double( NumberNodes );

	

		// zapisujemy
		sprintf(text,"%lf \t %lf \t %lf \t %lf \t %lf\n",PoleZewnetrzne,Temperatura,c,z,k);
		WriteFile(plik,text,strlen(text),&ile,NULL);
	}


	CloseHandle(plik);
	CloseHandle(log);
	
	delete [] m_fMagnetyzacjaTab;
	delete [] m_fMagetyzacjaCzas;
	
	return 0;
}


int CNetworkPOLYMOD::UruchomSymulacjeIsing(void)
{
	
	
	// otwieramy plik
	char text[500];
	DWORD ile;
	sprintf(text,"wyniki - dynamiczna=%d - srednie wagi=%d.txt",this->m_bSiecDynamiczna, this->m_boolSredniaWaga);
	HANDLE plik = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	sprintf(text,"log - dynamiczna=%d - srednie wagi=%d.txt",this->m_bSiecDynamiczna, this->m_boolSredniaWaga);
	HANDLE log = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	m_hLog = log;

	Temperatura = 0.0;
	PoleZewnetrzne = 0.0;


	// alokacja pamieci
	m_fMagnetyzacjaTab = new float [10*m_iLiczbaPowtorzenPM];

	m_fMagetyzacjaCzas = new float [2*MaxLiczbakrokow+10];
	for(int i=0; i<MaxLiczbakrokow; i++)
		m_fMagetyzacjaCzas[i] = 0;


	// petla po parametrze
	//for(PstwoStoI = 0.0; PstwoStoI < 0.45; PstwoStoI += 0.05)
	//for(SpreaderStifler = 0.0; SpreaderStifler < 1.05; SpreaderStifler += 0.05)
	for(Temperatura = 0.01; Temperatura < 0.02; Temperatura += 0.05)
	//for(PoleZewnetrzne = 0.0; PoleZewnetrzne < 2; PoleZewnetrzne += 0.01)
	{
		
		/*if(Temperatura > 1.0f)
			Temperatura += 0.05f;

		if(Temperatura > 2.0f)
			Temperatura += 0.1f;
		

		if(PoleZewnetrzne > 1.0f)
			PoleZewnetrzne += 0.01f;

		if(PoleZewnetrzne > 10.0f)
			PoleZewnetrzne += 0.5f;*/

		int Zasieg = 0;
		int CzasMax = 0;
		double Magnetyzacja = 0;
		int krokPM = 0;
		// jeden punkt wykresu
		for(int i=0; i<m_iLiczbaPowtorzenPM; i++)
		{
			// tworzymy siec
			TworzPolaczeniaPM();

			// uruchamiamy symulacje
			SymulacjaPM_Ising();

			// dane do loga
			sprintf(text,"%lf \t %lf \t %d \t %d \t %d \t %d \t %lf\n",Temperatura, PoleZewnetrzne,i,stepPM,m_iZasieg,TimeNumberInfectedMax,m_fMagnetyzacja);
			WriteFile(log,text,strlen(text),&ile,NULL);

			Zasieg += this->m_iZasieg;
			CzasMax += this->TimeNumberInfectedMax;
			Magnetyzacja += abs(m_fMagnetyzacja);
			krokPM += stepPM;


			if(m_bIsingTest)
			{
				sprintf(text,"logTest - T=%lf.txt", Temperatura);
				HANDLE logTest = CreateFile(text,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

				sprintf(text,"%d \n\n",i+1);
				WriteFile(logTest,text,strlen(text),&ile,NULL);

				for(int step = 0; step<MaxLiczbakrokow; step++)
				{
					sprintf(text,"%d \t %lf\n",step,m_fMagetyzacjaCzas[step]/float(i+1));
					WriteFile(logTest,text,strlen(text),&ile,NULL);
				}
				
				CloseHandle(logTest);
			}
		}

		double z = double(Zasieg) / double(m_iLiczbaPowtorzenPM) ;
		double c = double(Magnetyzacja) / double(m_iLiczbaPowtorzenPM);
		double k = double(krokPM) / double(m_iLiczbaPowtorzenPM);

		z /= double( NumberNodes );

		// zapisujemy
		sprintf(text,"%lf \t %lf \t %lf \t %lf \t %lf\n",PoleZewnetrzne,Temperatura,c,z,k);
		WriteFile(plik,text,strlen(text),&ile,NULL);
	}


	CloseHandle(plik);
	CloseHandle(log);
	

	delete [] m_fMagnetyzacjaTab;
	delete [] m_fMagetyzacjaCzas;

	return 0;
}





int CNetworkPOLYMOD::TworzPolaczeniaTymczasoweSporadycznePM(void)
{
	// potrzebujemy tabeli identyfikatorow wezlow, ktore maja wolne polaczenia przypadkowe
	int * tf = new int [NumberNodes];
	int fn = 0;
	
	for(int i = 0; i<NumberNodes; i++)
		if(Node[i].StateI[TEMPCONNECTIONS1] < Node[i].StateI[TEMPCONNECTIONS_MAX1])
		{
			tf[fn] = i;
			fn++;
		}
	
	
	for(int i = 0; i<fn; i++)
		for(int j = i+1; j<fn; j++)
		{
			int id = tf[i];
			int sasiad = tf[ j ];
			
			DodajPolaczenieSporadyczne(id,sasiad);
		}

	delete [] tf;

	return 0;
}



int CNetworkPOLYMOD::TworzPolaczeniaTymczasowePrzypadkowePM(void)
{
	// potrzebujemy tabeli identyfikatorow wezlow, ktore maja wolne polaczenia przypadkowe
	int * tf = new int [NumberNodes];
	int fn = 0;
	
	for(int i = 0; i<NumberNodes; i++)
		if(Node[i].StateI[TEMPCONNECTIONS2] < Node[i].StateI[TEMPCONNECTIONS_MAX2])
		{
			tf[fn] = i;
			fn++;
		}
	
	
	for(int i = 0; i<fn; i++)
		for(int j = i+1; j<fn; j++)
		{
			int id = tf[i];
			int sasiad = tf[ j ];
			
			DodajPolaczeniePrzypadkowe(id,sasiad);
		}

	delete [] tf;

	return 0;
}



int CNetworkPOLYMOD::NextStepPM_Rumour(void)
	{

	// tworzymy polaczenia przypadkowe
	TworzPolaczeniaTymczasowePM();

	// sprawdzamy czy mamy zastosowac srednia wage
	if(m_boolSredniaWaga)
	{
		for(int i=0; i<NumberNodes; i++)
		{
			int kall = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1] + Node[i].StateI[TEMPCONNECTIONS2];
			for(int k=0; k<kall; k++)
			{
				Node[i].StateF[k] = float( m_dSredniaWaga );
			}
		}
	}

	// ustawiamy nextstate
	for(int i=0; i<NumberNodes; i++)
		Node[i].StateI[NEXTSTATE] = Node[i].StateI[STATE];
	
	
	// po dodaniu polaczen obliczamy pstwo zmiany stanu
	int nInfected = 0; // liczba zainfekowanych wezlow
	int kall;
	for(int i=0; i<NumberNodes; i++)
	{
		switch(Node[i].StateI[STATE])
		{
		case IGNORANT:
			kall = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1] + Node[i].StateI[TEMPCONNECTIONS2];
			for(int k=0; k<kall; k++)
			{
				int sasiad = Node[i].Connection[k];
				if( Node[sasiad].StateI[STATE] == SPREADER )
				{
					// tu trzeba dodac dodatkowo zaleznosc od wieku
					if( GetRand() < IgnorantSpreader * Node[i].StateF[k])
					{
						Node[i].StateI[NEXTSTATE] = SPREADER;
						Node[i].StateI[WASINFECTED] = 1;
						
						//-------------------------------------------------------------------------------------------------
						int contactType = 0;
						int weightType = 0;

						if(k < Node[i].Connectivity)
						{
							// daily contact
							contactType = 0;
						}
						else
							if(k < Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1])
							{
								// rare contact
								contactType = 1;
							}
							else
								if(k < kall)
								{
									// random contact
									contactType = 2;
								}

						// check weight
							if(m_boolSredniaWaga)
							{
								weightType = 0;
							}
							else
							{
								float cw1 = Node[i].StateF[k] * 0.95f; 
								float cw2 = Node[i].StateF[k] * 1.05f; 
								for(int w=0; w<5; w++)
								{
									if(waga_polaczenia[w] > cw1 && waga_polaczenia[w] < cw2)
									{
										weightType = w;
										w = 100;
									}
								}
							}

						// update
						aktywacjaWezlaTemp[contactType][weightType]++;
						//-------------------------------------------------------------------------------------------------

						k = kall +1;
					}
				}
			}
			break;
		
		case SPREADER:
			kall = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1] + Node[i].StateI[TEMPCONNECTIONS2];
			
			Node[i].StateI[LIFESPAN]++;
			
			if(SpreaderStifler > 0.0f)
			for(int k=0; k<kall; k++)
			{
				int sasiad = Node[i].Connection[k];
				if( Node[sasiad].StateI[STATE] == SPREADER || Node[sasiad].StateI[STATE] == STIFLER)
				{
					// tu trzeba dodac dodatkowo zaleznosc od wieku
					if( GetRand() < SpreaderStifler * Node[i].StateF[k])
					{
						//-------------------------------------------------------------------------------------------------
						int contactType = 0;
						int weightType = 0;

						if(k < Node[i].Connectivity)
						{
							// daily contact
							contactType = 0;
						}
						else
							if(k < Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1])
							{
								// rare contact
								contactType = 1;
							}
							else
								if(k < kall)
								{
									// random contact
									contactType = 2;
								}

						// check weight
							if(m_boolSredniaWaga)
							{
								weightType = 0;
							}
							else
							{
								float cw1 = Node[i].StateF[k] * 0.95f; 
								float cw2 = Node[i].StateF[k] * 1.05f; 
								for(int w=0; w<5; w++)
								{
									if(waga_polaczenia[w] > cw1 && waga_polaczenia[w] < cw2)
									{
										weightType = w;
										w = 100;
									}
								}
							}

						// update
						dezaktywacjaWezlaTemp[contactType][weightType]++;
						//-------------------------------------------------------------------------------------------------
						
						Node[i].StateI[NEXTSTATE] = STIFLER;
						k = kall +1;

						
					}
				}
			}
			break;
		
		case STIFLER:
			break;
		}
	}

	// uaktualniamy stan i przy okazji zliczamy liczbe zainfekowanych wezlow
	int nIgnorant = 0;
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].StateI[STATE] = Node[i].StateI[NEXTSTATE];

		if(Node[i].StateI[STATE] == SPREADER)
			nInfected++;

		if(Node[i].StateI[STATE] == IGNORANT)
			nIgnorant++;
	}

	NumberInfected = nInfected;

	if(nInfected > 0 && nIgnorant > 0)
		return nInfected;

	return 0;

	}


int CNetworkPOLYMOD::SymulacjaPM_RumorPropagation(void)
	{
		// #define	IGNORANT		0
		//#define	SPREADER		1
		//#define	STIFLER			2

		for(int q=0; q<10; q++)
			for(int qq=0; qq<10; qq++)
			{
				aktywacjaWezlaTemp[q][qq] = 0;
				dezaktywacjaWezlaTemp[q][qq] = 0;
			}



		// inicjalizacja stanu wezlow
	for(int i =0; i<NumberNodes; i++)
	{
		Node[i].StateI[STATE] = IGNORANT;
		Node[i].StateI[WASINFECTED] = 0;
		Node[i].StateI[LIFESPAN] = 0;
		// losujemy szczepienia
		//if(GetRand() < PstwoSzczepien)
		//	Node[i].StateI[STATE] = STIFLER;
	}

	
	for(int i=0; i<InitiallyInfected; i++)
	{
		int num = int( GetRand() * double(NumberNodes-1) );
		Node[num].StateI[STATE] = SPREADER;
	}

	
	NumberInfectedMax = 0;
	TimeNumberInfectedMax = 0;
	stepPM = 0;

	// krok symulacji
	while(NextStepPM_Rumour() > 0 && stepPM < MaxLiczbakrokow)
	{
		if(NumberInfected > NumberInfectedMax)
		{
			NumberInfectedMax = NumberInfected;
			TimeNumberInfectedMax = stepPM;
		}
		stepPM++;
	}
	
	m_iZasieg = 0;
	m_iLifeSpan = 0;
	for(int i =0; i<NumberNodes; i++)
	{
		m_iZasieg += Node[i].StateI[WASINFECTED];
		if(Node[i].StateI[WASINFECTED])
			m_iLifeSpan += Node[i].StateI[LIFESPAN];
	}

	m_fLifeSpan = 0;
	if(m_iZasieg > 0)
		m_fLifeSpan = float(m_iLifeSpan) / float(m_iZasieg);

	return 0;

	}


int CNetworkPOLYMOD::SymulacjaPM_Ising(void)
	{

	for(int i =0; i<NumberNodes; i++)
	{
		if(GetRand() < 0.5)
			Node[i].StateI[STATE] = 1;
		else
			Node[i].StateI[STATE] = -1;
	}

	
	NumberInfectedMax = 0;
	TimeNumberInfectedMax = 0;
	stepPM = 0;

	// krok symulacji
	while(NextStepPM_Ising() > 0 && stepPM < MaxLiczbakrokow)
	{
		stepPM++;
	}
	
	m_fMagnetyzacja = 0;
	m_iZasieg = 0;
	for(int i =0; i<NumberNodes; i++)
	{
		m_iZasieg += Node[i].StateI[STATE];
	}

	m_fMagnetyzacja = float(m_iZasieg)/float(NumberNodes);

	

	//m_iZasieg = abs(m_iZasieg);

	return 0;

	
	}


int CNetworkPOLYMOD::SymulacjaADIsing(void)
	{

	for(int i =0; i<NumberNodes; i++)
	{
		if(GetRand() < 0.5)
			Node[i].StateI[STATE] = 1;
		else
			Node[i].StateI[STATE] = -1;
	}

	
	NumberInfectedMax = 0;
	TimeNumberInfectedMax = 0;
	stepPM = 0;

	// krok symulacji
	while(NextStepADIsing() > 0 && stepPM < MaxLiczbakrokow)
	{
		stepPM++;
	}
	
	m_fMagnetyzacja = 0;
	m_iZasieg = 0;
	for(int i =0; i<NumberNodes; i++)
	{
		m_iZasieg += Node[i].StateI[STATE];
	}

	m_fMagnetyzacja = float(m_iZasieg)/float(NumberNodes);

	

	//m_iZasieg = abs(m_iZasieg);

	return 0;

	
	}


int CNetworkPOLYMOD::NextStepPM_Ising(void)
	{

	// tworzymy polaczenia przypadkowe
	TworzPolaczeniaTymczasowePM();

	// sprawdzamy czy mamy zastosowac srednia wage
	if(m_boolSredniaWaga)
	{
		for(int i=0; i<NumberNodes; i++)
		{
			int kall = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1] + Node[i].StateI[TEMPCONNECTIONS2];
			for(int k=0; k<kall; k++)
			{
				Node[i].StateF[k] = float( m_dSredniaWaga );
			}
		}
	}

	// ustawiamy nextstate
	for(int i=0; i<NumberNodes; i++)
		Node[i].StateI[NEXTSTATE] = Node[i].StateI[STATE];
	
	
	// po dodaniu polaczen obliczamy pstwo zmiany stanu
	int nInfected = 0; // liczba zainfekowanych wezlow
	int kall;
	for(int i=0; i<NumberNodes; i++)
	{
		// liczymy pole lokalne
		float h = 0.0f;
		
	
		kall = Node[i].Connectivity + Node[i].StateI[TEMPCONNECTIONS1] + Node[i].StateI[TEMPCONNECTIONS2];
		for(int k=0; k<kall; k++)
		{
			int sasiad = Node[i].Connection[k];
			h += Node[i].StateF[k] * float(Node[sasiad].StateI[STATE]);
		}
		h *= float(Node[i].StateI[STATE]);


		// uwzgledniamy pole zewnetrzne
		h += float(Node[i].StateI[STATE]) * PoleZewnetrzne;

			// liczymy teraz prawdopodobienstwa
			double p = 0;
			
			if( Temperatura > 0)
			{
				double ph = h/Temperatura;
				if(ph > 1000.0) ph = 1000.0;
				
				p = ( exp( double(ph) ) ) ; 
				p /= ( exp( double(ph)) + exp(double(-ph)) );
				
				if( GetRand() < p || h == 0.0)
					Node[i].StateI[NEXTSTATE] = Node[i].StateI[STATE];
				else
					Node[i].StateI[NEXTSTATE] = -Node[i].StateI[STATE];
			}
			else
			{
				if(h>=0)
					Node[i].StateI[NEXTSTATE] = Node[i].StateI[STATE];
				else
					Node[i].StateI[NEXTSTATE] = -Node[i].StateI[STATE];
			}
		
	}

	// uaktualniamy stan i przy okazji zliczamy liczbe zainfekowanych wezlow
	double avg_spin = 0;
	double avg_spin_sign = 0;
	int as = 0;
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].StateI[STATE] = Node[i].StateI[NEXTSTATE];

		as += Node[i].StateI[STATE];

		if(Node[i].StateI[STATE] > 0)
			nInfected++;
	}
	
	avg_spin_sign = double(as) / double(NumberNodes);
	avg_spin = double(abs(as)) / double(NumberNodes);
	NumberInfected = nInfected;


	
	m_fMagnetyzacja = avg_spin;
	

	if(m_fMagetyzacjaCzas)
		m_fMagetyzacjaCzas[stepPM] += avg_spin;
	
	/*m_fMagnetyzacjaTab[stepPM] = m_fMagnetyzacja;

	if(stepPM > 100)
	{
		float diff = abs(m_fMagnetyzacjaTab[stepPM] - m_fMagnetyzacjaTab[stepPM-50]);

		if(diff < 0.05f)
			return -1;
	}
	*/
	//char t[200];
	//sprintf(t,"%lf\n",avg_spin);
	//WriteToLog(t);

	//char txt[200];
	//sprintf(txt,"%d\t%d\t%lf\n",stepPM,nInfected,avg_spin);
	//WriteToLog(txt);

	//if(avg_spin >= 0.98)
	//	return -1;

	return 1;
	return nInfected;



	}



int CNetworkPOLYMOD::NextStepADIsing(void)
	{

	
	// ustawiamy nextstate
	for(int i=0; i<NumberNodes; i++)
		Node[i].StateI[NEXTSTATE] = Node[i].StateI[STATE];
	
	
	// po dodaniu polaczen obliczamy pstwo zmiany stanu
	int nInfected = 0; // liczba zainfekowanych wezlow
	int kall;
	for(int i=0; i<NumberNodes; i++)
	{
		// liczymy pole lokalne
		float h = 0.0f;
		
	
		kall = Node[i].Connectivity;
		for(int k=0; k<kall; k++)
		{
			int sasiad = Node[i].Connection[k];
			h += float(Node[sasiad].StateI[STATE]);
		}
		h *= float(Node[i].StateI[STATE]);


		// uwzgledniamy pole zewnetrzne
		h += float(Node[i].StateI[STATE]) * PoleZewnetrzne;

			// liczymy teraz prawdopodobienstwa
			double p = 0;
			
			if( Temperatura > 0)
			{
				double ph = h/Temperatura;
				if(ph > 1000.0) ph = 1000.0;
				
				p = ( exp( double(ph) ) ) ; 
				p /= ( exp( double(ph)) + exp(double(-ph)) );
				
				if( GetRand() < p || h == 0.0)
					Node[i].StateI[NEXTSTATE] = Node[i].StateI[STATE];
				else
					Node[i].StateI[NEXTSTATE] = -Node[i].StateI[STATE];
			}
			else
			{
				if(h>=0)
					Node[i].StateI[NEXTSTATE] = Node[i].StateI[STATE];
				else
					Node[i].StateI[NEXTSTATE] = -Node[i].StateI[STATE];
			}
		
	}

	// uaktualniamy stan i przy okazji zliczamy liczbe zainfekowanych wezlow
	double avg_spin = 0;
	double avg_spin_sign = 0;
	int as = 0;
	for(int i=0; i<NumberNodes; i++)
	{
		Node[i].StateI[STATE] = Node[i].StateI[NEXTSTATE];

		as += Node[i].StateI[STATE];

		if(Node[i].StateI[STATE] > 0)
			nInfected++;
	}
	
	avg_spin_sign = double(as) / double(NumberNodes);
	avg_spin = double(abs(as)) / double(NumberNodes);
	NumberInfected = nInfected;

	if(m_fMagetyzacjaCzas)
		m_fMagetyzacjaCzas[stepPM] += avg_spin;

	m_fMagnetyzacja = avg_spin;
	/*m_fMagnetyzacjaTab[stepPM] = m_fMagnetyzacja;

	if(stepPM > 100)
	{
		float diff = abs(m_fMagnetyzacjaTab[stepPM] - m_fMagnetyzacjaTab[stepPM-50]);

		if(diff < 0.05f)
			return -1;
	}
	*/
	//char t[200];
	//sprintf(t,"%lf\n",avg_spin);
	//WriteToLog(t);

	//char txt[200];
	//sprintf(txt,"%d\t%d\t%lf\n",stepPM,nInfected,avg_spin);
	//WriteToLog(txt);

	//if(avg_spin >= 0.98)
	//	return -1;

	return 1;
	return nInfected;



	}


void CNetworkPOLYMOD::WriteToLog(char * txt)
	{
		if(txt)
			{
				if(m_hLogFile == NULL)
					m_hLogFile = CreateFile("universal_network.log",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

				DWORD ile;
				WriteFile(m_hLogFile,txt,strlen(txt),&ile,NULL);
			}
	}






void CNetworkPOLYMOD::lWriteToLog(char *tekst)
{
	if(tekst)
	{
		if(lTrollOutputFile == NULL)
			lTrollOutputFile = CreateFile("_UniversalNetwork_Debug_File_.txt",GENERIC_WRITE,FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
		DWORD ile;
		if(lTrollOutputFile)
		{
			// zapisujemy czas
			char txt[100];
			SYSTEMTIME st;
			GetSystemTime(&st);
			sprintf(txt,"\n %d : %d : %d \t",st.wMinute,st.wSecond,st.wMilliseconds);
			WriteFile(lTrollOutputFile,txt,strlen(txt),&ile,NULL);

			// zapisujemy info
			WriteFile(lTrollOutputFile,tekst,strlen(tekst),&ile,NULL);
		}

		
		/*
		if(strcmp(tekst,"CloseHandle") == 0)
		{
			CloseHandle(gTrollOutputFile);
			gTrollOutputFile = NULL;
		}
		*/
	}
}