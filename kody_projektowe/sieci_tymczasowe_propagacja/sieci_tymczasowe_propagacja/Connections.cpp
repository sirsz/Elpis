#include "StdAfx.h"
#include "Connections.h"

CConnections::CConnections(void)
: NumberConnections(0)
, From(NULL)
, Target(NULL)
{
}

CConnections::~CConnections(void)
{
	Destroy();
}

int CConnections::Create(int N)
{
	Destroy();
	
	if(N>0)
	{
		NumberConnections = N;
		From = new int [N];
		Target = new int [N];
	}

	for(int n=0; n<N; n++)
	{
		From[n] = -3;
		Target[n] = -3;
	}
	
	return 0;
}

int CConnections::Destroy(void)
{
	if(From) delete [] From;
	From = NULL;

	if(Target) delete [] Target;
	Target = NULL;

	return 0;
}
