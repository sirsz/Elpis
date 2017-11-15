// UniversalNetworkDoc.h : interface of the CUniversalNetworkDoc class
//


#pragma once

#include "UniversalNetworkClass.h"

class CUniversalNetworkDoc : public CDocument
{
protected: // create from serialization only
	CUniversalNetworkDoc();
	DECLARE_DYNCREATE(CUniversalNetworkDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CUniversalNetworkDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPercolationSimulation();
	afx_msg void OnPercolationSimulationSite();
	afx_msg void OnTestTest();
	afx_msg void OnTestConvertToNetwork();
	afx_msg void OnTestBadajSiecSkyrock();
	int DrawFromPowerLaw(double gamma, int kMin, int kMax, double rand01);
	afx_msg void OnPolymodTest();
	afx_msg void OnPolymodSymulacja();
	afx_msg void OnAgedegreeSymulacja();
	afx_msg void OnAgedegreeTest();
};


