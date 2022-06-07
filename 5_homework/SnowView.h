
// SnowView.h : CSnowView ��Ľӿ�
//

#pragma once
#include "snow.h"
#include "cloud.h"
#define SNOW_NUMBERS 120
#define CLOUD_NUMBERS 4

class CSnowView : public CView
{
private:
	//void InitSnow(SNOW &snow);
public:
	int t;
	CSnow snow[SNOW_NUMBERS];
	CCloud cloud[CLOUD_NUMBERS];
	//SNOW snow[SNOW_NUMBERS];
protected: // �������л�����
	CSnowView();
	DECLARE_DYNCREATE(CSnowView)

	// ����
public:
	CSnowDoc* GetDocument() const;

	// ����
public:

	// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	// ʵ��
public:
	virtual ~CSnowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//protected:

	// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual void OnInitialUpdate();

	void drawBackground(CDC* pDC);
};

#ifndef _DEBUG  // SnowView.cpp �еĵ��԰汾
inline CSnowDoc* CSnowView::GetDocument() const
   { return reinterpret_cast<CSnowDoc*>(m_pDocument); }
#endif

