
// GobangView.h : CGobangView 类的接口
//

#pragma once

#include "GobangDoc.h"
#include "Board.h"
#include "Connect.h"
#include "SetupDlg.h"

class CGobangView : public CView
{
protected: // 仅从序列化创建
	CGobangView();
	DECLARE_DYNCREATE(CGobangView)

// 特性
public:
	CGobangDoc* GetDocument() const;

private:

	int  m_color;                   // 玩家颜色
	BOOL m_bWait;                   // 等待标志
	BOOL m_bOldWait;	            // 原来的等待状态

	CBoard   m_board;			    // 主棋盘对象

	CConnect m_connect;			    // 监听套接字
	CConnect m_sock;			    // 使用套接字
	BOOL     m_bIsConnect;		    // 连接标志

	CSetupDlg m_setup_dlg;	    	// 设置对话框对象

// 操作
public:
	//以下函数用于提供给CConect类调用
	void OnClose();
	void OnAccept();
	void OnConnect();
	void OnReceive();
		
	void NewGameStart(BOOL isHost);
	void SetMenuState(BOOL bEnable);		//设置菜单状态

	void Send(MSGSTRUCT * pmsg);			//发送数据
	void Restart();							//重新开始游戏

	void SetColor(int color);
	int  GetColor() const;
	void SetWait(BOOL bWait);
	void RestoreWait();
	void Over(int x, int y);        // 处理对方落子后的工作
	void DrawGame();                // 和棋操作


// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CGobangView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnMenuDrawGame();
	afx_msg void OnMenuExitGame();
	afx_msg void OnMenuNewGame();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMenuRetractGame();
	
	// //保存上一次的落子信息
	MSGSTRUCT old_msg;

	void RetractGame();
	void OnMenuSurrenderGame();
};

#ifndef _DEBUG  // GobangView.cpp 中的调试版本
inline CGobangDoc* CGobangView::GetDocument() const
   { return reinterpret_cast<CGobangDoc*>(m_pDocument); }
#endif

