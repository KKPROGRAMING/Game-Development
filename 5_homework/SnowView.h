
// SnowView.h : CSnowView 类的接口
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
protected: // 仅从序列化创建
	CSnowView();
	DECLARE_DYNCREATE(CSnowView)

	// 特性
public:
	CSnowDoc* GetDocument() const;

	// 操作
public:

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	// 实现
public:
	virtual ~CSnowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//protected:

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual void OnInitialUpdate();

	void drawBackground(CDC* pDC);
};

#ifndef _DEBUG  // SnowView.cpp 中的调试版本
inline CSnowDoc* CSnowView::GetDocument() const
   { return reinterpret_cast<CSnowDoc*>(m_pDocument); }
#endif

