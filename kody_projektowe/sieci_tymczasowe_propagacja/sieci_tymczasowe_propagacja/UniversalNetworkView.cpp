// UniversalNetworkView.cpp : implementation of the CUniversalNetworkView class
//

#include "stdafx.h"
#include "UniversalNetwork.h"

#include "UniversalNetworkDoc.h"
#include "UniversalNetworkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUniversalNetworkView

IMPLEMENT_DYNCREATE(CUniversalNetworkView, CView)

BEGIN_MESSAGE_MAP(CUniversalNetworkView, CView)
END_MESSAGE_MAP()

// CUniversalNetworkView construction/destruction

CUniversalNetworkView::CUniversalNetworkView()
{
	// TODO: add construction code here

}

CUniversalNetworkView::~CUniversalNetworkView()
{
}

BOOL CUniversalNetworkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CUniversalNetworkView drawing

void CUniversalNetworkView::OnDraw(CDC* /*pDC*/)
{
	CUniversalNetworkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CUniversalNetworkView diagnostics

#ifdef _DEBUG
void CUniversalNetworkView::AssertValid() const
{
	CView::AssertValid();
}

void CUniversalNetworkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUniversalNetworkDoc* CUniversalNetworkView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUniversalNetworkDoc)));
	return (CUniversalNetworkDoc*)m_pDocument;
}
#endif //_DEBUG


// CUniversalNetworkView message handlers
