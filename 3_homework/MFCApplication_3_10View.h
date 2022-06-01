
// MFCApplication_3_10View.h: CMFCApplication310View 类的接口
//

#pragma once


class CMFCApplication310View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication310View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication310View)

// 特性
public:
	CMFCApplication310Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMFCApplication310View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	void drawBall(CDC* pDC);
	int x = 0;//ellipse()中的四个参数是包围椭圆的矩形的左上角和右下角坐标
	int y = 0;
	int diameter = 50;
	int init_vx = 8;
	int init_vy = 5;
	int init_b_width = 200;
	int vx = 8;
	int vy = 5;
	float dv = 1.2 ;
	int block_x = 50;
	int block_width = 240;
	int block_height = 30;
	int block_vx = 20;
	float decrease = 0.1;
	//int block_dvx = 0.5;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void drawBlock(CDC* pDC);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void drawGameInfo(CDC* pDC);
	int life = 3;
	int mark = 0;
	int dmark = 10;

	void promptEnd(CDC* pDC);
};

#ifndef _DEBUG  // MFCApplication_3_10View.cpp 中的调试版本
inline CMFCApplication310Doc* CMFCApplication310View::GetDocument() const
   { return reinterpret_cast<CMFCApplication310Doc*>(m_pDocument); }
#endif

