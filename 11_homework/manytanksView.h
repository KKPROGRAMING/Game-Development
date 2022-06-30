
// manytanksView.h : CmanytanksView 类的接口
//
#pragma once

#include "stdafx.h"
#include "enemy.h"
#include "map.h"

class CmanytanksView : public CView
{
protected: // 仅从序列化创建
	CmanytanksView();
	DECLARE_DYNCREATE(CmanytanksView)

// 特性
public:
	CmanytanksDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
 

// 实现
public:
	virtual ~CmanytanksView();
 

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();

	DECLARE_MESSAGE_MAP()

private:
	//-----------------------------------------------------//
	//游戏对象，可再创建类
	GameStateType m_GameState;
	CTankMap  m_Map;             //地图
	CObList   m_Bullets;         //子弹
	CObList   m_Tanks;           //坦克
	CObList   m_UserTanks;       //用户坦克
	//-----------------------------------------------------//

private:
	void DrawAllObjects(CDC* pDC);
	void DrawSplashBMP(CDC* pDC);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CPoint user_p;//玩家当前位置，用于实现追踪
};

