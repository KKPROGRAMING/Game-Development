
// manytanksView.h : CmanytanksView ��Ľӿ�
//
#pragma once

#include "stdafx.h"
#include "enemy.h"
#include "map.h"

class CmanytanksView : public CView
{
protected: // �������л�����
	CmanytanksView();
	DECLARE_DYNCREATE(CmanytanksView)

// ����
public:
	CmanytanksDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
 

// ʵ��
public:
	virtual ~CmanytanksView();
 

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();

	DECLARE_MESSAGE_MAP()

private:
	//-----------------------------------------------------//
	//��Ϸ���󣬿��ٴ�����
	GameStateType m_GameState;
	CTankMap  m_Map;             //��ͼ
	CObList   m_Bullets;         //�ӵ�
	CObList   m_Tanks;           //̹��
	CObList   m_UserTanks;       //�û�̹��
	//-----------------------------------------------------//

private:
	void DrawAllObjects(CDC* pDC);
	void DrawSplashBMP(CDC* pDC);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CPoint user_p;//��ҵ�ǰλ�ã�����ʵ��׷��
};

