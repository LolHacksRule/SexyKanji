// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// SexyKanjiEditorView.cpp : implementation of the CSexyKanjiEditorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SexyKanjiEditor.h"
#endif

#include "SexyKanjiEditorDoc.h"
#include "SexyKanjiEditorView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSexyKanjiEditorView

IMPLEMENT_DYNCREATE(CSexyKanjiEditorView, CScrollView)

BEGIN_MESSAGE_MAP(CSexyKanjiEditorView, CScrollView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSexyKanjiEditorView construction/destruction

CSexyKanjiEditorView::CSexyKanjiEditorView()
{
	// TODO: add construction code here

}

CSexyKanjiEditorView::~CSexyKanjiEditorView()
{
}

BOOL CSexyKanjiEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CSexyKanjiEditorView drawing

void CSexyKanjiEditorView::OnDraw(CDC* /*pDC*/)
{
	CSexyKanjiEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CSexyKanjiEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CSexyKanjiEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSexyKanjiEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSexyKanjiEditorView diagnostics

#ifdef _DEBUG
void CSexyKanjiEditorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSexyKanjiEditorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSexyKanjiEditorDoc* CSexyKanjiEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSexyKanjiEditorDoc)));
	return (CSexyKanjiEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CSexyKanjiEditorView message handlers


void CSexyKanjiEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Add your specialized code here and/or call the base class
	CSexyKanjiEditorDoc* aDoc = GetDocument();

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if(pMainFrame && bActivate && pMainFrame->GetKanjiWindow()->getWindowHandle() && GetSafeHwnd())
	{
		::SetParent(pMainFrame->GetKanjiWindow()->getWindowHandle(), GetSafeHwnd());
		::SetWindowLongA(pMainFrame->GetKanjiWindow()->getWindowHandle(), GWL_STYLE, WS_VISIBLE | WS_CHILD);
		::SetWindowPos(pMainFrame->GetKanjiWindow()->getWindowHandle(), NULL, 0, 0, 800, 600, SWP_FRAMECHANGED | SWP_ASYNCWINDOWPOS | SWP_NOSIZE);

		RECT aWindowRect;
		if(::GetWindowRect(pMainFrame->GetKanjiWindow()->getWindowHandle(), &aWindowRect))
		{
			SetScrollSizes(MM_TEXT, CSize(aWindowRect.right - aWindowRect.left, aWindowRect.bottom - aWindowRect.top));
		}

		
	}

	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
