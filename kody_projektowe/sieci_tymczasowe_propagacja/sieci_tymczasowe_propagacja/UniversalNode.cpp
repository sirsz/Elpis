#include "StdAfx.h"
#include "UniversalNode.h"

CUniversalNode::CUniversalNode(void)
: StateI(NULL)
, StateF(NULL)
, Connectivity(0)
, ConnectivityMax(0)
, Connection(NULL)
, TempStateI(0)
, TempStateF(0)
{
}

CUniversalNode::~CUniversalNode(void)
{
	DestroyNode();
}

int CUniversalNode::Create(int LengthStateI, int LengthStateF, int StartConnectivity)
{
	DestroyNode();
	
	if(LengthStateI > 0)
		StateI = new int [LengthStateI];

	if(LengthStateF > 0)
		StateF = new float [LengthStateF];

	if(StartConnectivity > 0)
	{
		ConnectivityMax = StartConnectivity;
		Connection = new int [ConnectivityMax];
	}
	else
		ConnectivityMax = 0;

	
	return 0;
}

int CUniversalNode::DestroyNode(void)
{
	if(StateI)
		delete [] StateI;
	StateI = NULL;

	if(StateF)
		delete [] StateF;
	StateI = NULL;

	if(Connection)
		delete [] Connection;
	Connection = NULL;

	Connectivity = 0;

	return 0;
}

void CUniversalNode::AddConnectionFast(int target)
{
	Connection[Connectivity] = target;
	Connectivity++;
}

void CUniversalNode::AddConnectionSafe(int target)
{
	if(ConnectionExist(target)) return;
	
	// sprawdzamy czy realokacja jest konieczna
	if(Connectivity >= ConnectivityMax)
	{
		ConnectivityMax += CONNECTIVITY_INCREASE;
		int * tab = new int [ConnectivityMax];

		for(int k=0; k<Connectivity; k++)
			tab[k] = Connection[k];

		if(Connection)	delete [] Connection;
		Connection = tab;
	}

	// dodajemy polaczenie
	AddConnectionFast(target);


}

bool CUniversalNode::ConnectionExist(int target)
{
	if(Connection == NULL) return false;

	for(int k=0; k<Connectivity; k++)
		if(Connection[k] == target)
			return true;
	
	return false;
}

void CUniversalNode::PreallocateMemmory(int MaxConnectivity)
{

	// alokujemy pamiec na przyszle polaczenia

	if(MaxConnectivity > ConnectivityMax)
	{
		ConnectivityMax = MaxConnectivity;
		
		int * tab = new int [ConnectivityMax];

		for(int k=0; k<Connectivity; k++)
			tab[k] = Connection[k];

		if(Connection)	delete [] Connection;
		Connection = tab;
	}

}

int CUniversalNode::Save(HANDLE plik, int NumberFeaturesI, int NumberFeaturesF)
{
	DWORD ile;

	if(NumberFeaturesI > 0)
	{
		int len = NumberFeaturesI * sizeof(this->StateI[0]);
		WriteFile(plik,StateI,len,&ile,NULL);
	}

	if(NumberFeaturesF > 0)
	{
		int len = NumberFeaturesF * sizeof(this->StateF[0]);
		WriteFile(plik,StateF,len,&ile,NULL);
	}

	WriteFile(plik,&Connectivity,sizeof(Connectivity),&ile,NULL);
	if(Connectivity > 0)
	{
		int len = Connectivity * sizeof(this->Connection[0]);
		WriteFile(plik,Connection,len,&ile,NULL);
	}
	
	return 0;
}



int CUniversalNode::Load(HANDLE plik, int NumberFeaturesI, int NumberFeaturesF)
{
	DWORD ile;

	if(NumberFeaturesI > 0)
	{
		int len = NumberFeaturesI * sizeof(this->StateI[0]);
		ReadFile(plik,StateI,len,&ile,NULL);
	}

	if(NumberFeaturesF > 0)
	{
		int len = NumberFeaturesF * sizeof(this->StateF[0]);
		ReadFile(plik,StateF,len,&ile,NULL);
	}

	ReadFile(plik,&Connectivity,sizeof(Connectivity),&ile,NULL);
	if(Connectivity > 0)
	{
		if(Connection)
		{
			if(Connectivity >= ConnectivityMax)
			{
				delete [] Connection;
				ConnectivityMax = Connectivity;
				Connection = new int [ConnectivityMax];
			}
		}
		else
		{
			ConnectivityMax = Connectivity;
			Connection = new int [ConnectivityMax];
		}
		
		int len = Connectivity * sizeof(this->Connection[0]);
		ReadFile(plik,Connection,len,&ile,NULL);
	}
	
	return 0;
}

int CUniversalNode::RemoveConnection(int to)
{
	int zwrot = 0;
	if(Connection)
	{
		for(int i=0; i<Connectivity; i++)
		{
			if(Connection[i] == to)
			{
				// znalezlismy szukanego sasiada, wiec usuwamy polaczenie
				Connectivity --;
				Connection[i] = Connection[Connectivity];
				i--;
				zwrot++;
			}
		}
	}
	
	// zwracamy liczbe usunietych polaczen
	return zwrot;
}
