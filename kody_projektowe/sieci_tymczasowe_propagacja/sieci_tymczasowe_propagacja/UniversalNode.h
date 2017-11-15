#pragma once

#define LICZBA_CECH			10
#define LICZBA_CECH_F		10
#define CONNECTIVITY_MAX	10
#define CONNECTIVITY_INCREASE	10


class CUniversalNode
{
public:
	CUniversalNode(void);
	~CUniversalNode(void);
	int *StateI;				// tablica stanu intiger
	float *StateF;			// tablica stanu float
	int Connectivity;		// aktualna liczba polaczen
	int ConnectivityMax;	// dlugosc wektora Connection
	int *Connection;			// tablica polaczen
	int Create(int LengthStateI, int LengthStateF, int StartConnectivity);
	int DestroyNode(void);
	inline void AddConnectionFast(int target);
	void AddConnectionSafe(int target);
	bool ConnectionExist(int target);
	int TempStateI;
	float TempStateF;
	void PreallocateMemmory(int MaxConnectivity);
	int Save(HANDLE plik, int NumberFeaturesI, int NumberFeaturesF);
	int Load(HANDLE plik, int NumberFeaturesI, int NumberFeaturesF);
	int RemoveConnection(int to);
};
