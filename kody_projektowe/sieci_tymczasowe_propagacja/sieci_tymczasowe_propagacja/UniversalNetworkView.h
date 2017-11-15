// UniversalNetworkView.h : interface of the CUniversalNetworkView class
//


#pragma once


class CUniversalNetworkView : public CView
{
protected: // create from serialization only
	CUniversalNetworkView();
	DECLARE_DYNCREATE(CUniversalNetworkView)

// Attributes
public:
	CUniversalNetworkDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CUniversalNetworkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in UniversalNetworkView.cpp
inline CUniversalNetworkDoc* CUniversalNetworkView::GetDocument() const
   { return reinterpret_cast<CUniversalNetworkDoc*>(m_pDocument); }
#endif

