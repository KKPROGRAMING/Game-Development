
// SubMarineView.h : CSubMarineView ��Ľӿ�
//

#pragma once

#include "Explosion.h"
#include "MyObject.h"
#include "Score.h"
#include "MyShip.h"
#include "Submarine.h"
#include "Bomb.h"
#include "Bplus.h"
#include "Torpedo.h"
#include "Subm.h"

#include "resource.h"
#include <Windows.h>
#include <iostream>
#include "string.h"
#include <process.h> 
#include<iomanip>
#include <MMSystem.h>
#pragma comment (lib,"Winmm.lib")
#pragma comment(lib, "WINMM.LIB")

class CSubMarineView : public CView
{
//��Ա����
private:
	//	0: MyShip
	//	1: Enemy Ship
	//	2: Bomb
	//	3: Enemy weapons
	//	4: explosion
	//	5: score
	//  6: Bombplus !!
	//CObList m_Objects[6];
	CObList m_Objects[7]; //������һ��������ը�� bomb_plus


	CBitmap m_VirtualBitmap;
	CDC		m_VirtualDC;

	bool    m_bGamePause;


private:
	int random(int __num) { return(int)(((long)rand()*__num) / (RAND_MAX + 1)); }
	void  DrawBlendBkground(CDC* dc);

protected: // �������л�����
	CSubMarineView();
	DECLARE_DYNCREATE(CSubMarineView)

// ����
public:
	CSubMarineDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CSubMarineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();

	// ��ֻ������ֵ
	int life;
};

#ifndef _DEBUG  // SubMarineView.cpp �еĵ��԰汾
inline CSubMarineDoc* CSubMarineView::GetDocument() const
   { return reinterpret_cast<CSubMarineDoc*>(m_pDocument); }
#endif

