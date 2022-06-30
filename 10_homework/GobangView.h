
// GobangView.h : CGobangView ��Ľӿ�
//

#pragma once

#include "GobangDoc.h"
#include "Board.h"
#include "Connect.h"
#include "SetupDlg.h"

class CGobangView : public CView
{
protected: // �������л�����
	CGobangView();
	DECLARE_DYNCREATE(CGobangView)

// ����
public:
	CGobangDoc* GetDocument() const;

private:

	int  m_color;                   // �����ɫ
	BOOL m_bWait;                   // �ȴ���־
	BOOL m_bOldWait;	            // ԭ���ĵȴ�״̬

	CBoard   m_board;			    // �����̶���

	CConnect m_connect;			    // �����׽���
	CConnect m_sock;			    // ʹ���׽���
	BOOL     m_bIsConnect;		    // ���ӱ�־

	CSetupDlg m_setup_dlg;	    	// ���öԻ������

// ����
public:
	//���º��������ṩ��CConect�����
	void OnClose();
	void OnAccept();
	void OnConnect();
	void OnReceive();
		
	void NewGameStart(BOOL isHost);
	void SetMenuState(BOOL bEnable);		//���ò˵�״̬

	void Send(MSGSTRUCT * pmsg);			//��������
	void Restart();							//���¿�ʼ��Ϸ

	void SetColor(int color);
	int  GetColor() const;
	void SetWait(BOOL bWait);
	void RestoreWait();
	void Over(int x, int y);        // ����Է����Ӻ�Ĺ���
	void DrawGame();                // �������


// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CGobangView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnMenuDrawGame();
	afx_msg void OnMenuExitGame();
	afx_msg void OnMenuNewGame();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMenuRetractGame();
	
	// //������һ�ε�������Ϣ
	MSGSTRUCT old_msg;

	void RetractGame();
	void OnMenuSurrenderGame();
};

#ifndef _DEBUG  // GobangView.cpp �еĵ��԰汾
inline CGobangDoc* CGobangView::GetDocument() const
   { return reinterpret_cast<CGobangDoc*>(m_pDocument); }
#endif

