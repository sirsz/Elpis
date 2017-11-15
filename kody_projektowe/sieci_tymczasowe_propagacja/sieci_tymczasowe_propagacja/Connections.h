#pragma once

class CConnections
{
public:
	CConnections(void);
	~CConnections(void);
	int NumberConnections;
	int *From;
	int *Target;
	int Create(int N);
	int Destroy(void);
};
