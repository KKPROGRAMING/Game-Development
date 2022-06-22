
// FlappyBirdTestView.h : CFlappyBirdTestView 类的接口
//

#pragma once
#include "road.h"
#include "Pipe.h"
#include "Bird.h"
#include "NameList.h"

class CFlappyBirdTestView : public CView
{
	bool  isGameOver, isGameStart;
	CString m_name;
	int   m_score ;
	int m_life ;
	double weight;//分数对柱子速度影响的参数
	CRoad m_road;   //申请马路类
	CPipe m_pipe[2];  //两个柱子
	CBird m_bird;     //小鸟
	CNameList m_list;

protected: // 仅从序列化创建
	CFlappyBirdTestView();
	DECLARE_DYNCREATE(CFlappyBirdTestView)

// 操作
public:
	
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CFlappyBirdTestView();

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	void InitGame();
	void RestartGame();
	void DrawBackground(CDC* pDC);
	void DrawStartInfo(CDC* pDC);
	void DrawScoreLife(CDC* pDC, int Score,int Life);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

};

 

