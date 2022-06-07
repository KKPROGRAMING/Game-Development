
// ExampleView.h : interface of the CExampleView class
//
#ifndef _EXAMPLEVIEW_H_
#define _EXAMPLEVIEW_H_

//µ¼Èëmsimg32.lib¿â
#pragma comment(lib, "msimg32.lib")

class CExampleView : public CView
{
	int mapping_matrix[4][4];
	void bilt(int tillid, int screenx, int screeny);
	CDC memBKDC,memDC;
	CBitmap bmpbk,bmp;

	// Í¼ÏñÈÚºÏ
	int factor;
	CDC SourceDC1,SourceDC2;
	CBitmap sourceBMP1,sourceBMP2;

protected: // create from serialization only
	CExampleView();
	DECLARE_DYNCREATE(CExampleView)

// Attributes
public:
	CExampleDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CExampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnPaint();
	afx_msg void OnLightup();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnblend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BLENDFUNCTION bf;
	int bTrans;
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in ExampleView.cpp
inline CExampleDoc* CExampleView::GetDocument() const
   { return reinterpret_cast<CExampleDoc*>(m_pDocument); }
#endif

#endif