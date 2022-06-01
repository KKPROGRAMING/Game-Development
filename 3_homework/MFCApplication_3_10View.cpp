
// MFCApplication_3_10View.cpp: CMFCApplication310View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication_3_10.h"
#endif

#include "MFCApplication_3_10Doc.h"
#include "MFCApplication_3_10View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication310View

IMPLEMENT_DYNCREATE(CMFCApplication310View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication310View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFCApplication310View 构造/析构

CMFCApplication310View::CMFCApplication310View() noexcept
{
	// TODO: 在此处添加构造代码

}

CMFCApplication310View::~CMFCApplication310View()
{
}

BOOL CMFCApplication310View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication310View 绘图

void CMFCApplication310View::OnDraw(CDC* pDC)
{
	CMFCApplication310Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	/*pDC->TextOutA(0,50,"hello world!");*/
	if (life > 0) {
		drawBall(pDC);
		drawBlock(pDC);
		drawGameInfo(pDC);
	}
	else {
		promptEnd(pDC);
	}
}

void CMFCApplication310View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication310View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication310View 诊断

#ifdef _DEBUG
void CMFCApplication310View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication310View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication310Doc* CMFCApplication310View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication310Doc)));
	return (CMFCApplication310Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication310View 消息处理程序


void CMFCApplication310View::drawBall(CDC* pDC)
{
	// TODO: 在此处添加实现代码.
	CBrush ball_brush(RGB(127,255,211));
	CBrush* pOld = pDC->SelectObject(&ball_brush);
	pDC->Ellipse(x, y, x + diameter , y + diameter);

	pDC->SelectObject(pOld);
	ball_brush.DeleteObject();
}


int CMFCApplication310View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetTimer(0, 100, NULL);//Settimer是设置一个计时器并开始执行计时器Ontimer中的代码，Ontimer是计时器所执行的代码。

	return 0;
}


void CMFCApplication310View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//UPDATE GAME STATE
		x = x + vx;
		y = y + vy;

		//collision detect碰撞检测
		CRect rt;
		GetClientRect(&rt);
		CRect block(block_x, rt.bottom - block_height, block_x + block_width + diameter, rt.bottom);
		POINT ball;
		int radius = (diameter * 1.0) / 2.0;

		ball.x = x + diameter;
		ball.y = y + diameter;
		if (block.PtInRect(ball)) {

			mark += dmark;
			vx += dv;
			vy += dv;
			vy = -vy;
			block_width -= decrease;
		}
		if (x < 0) {
			x = 0;
			vx = -vx;
		}
		if (x > rt.right - diameter) {
			x = rt.right - diameter;
			vx = -vx;
			//x = x + vx;
		}
		if (y < 0) {
			y = 0;
			vy = -vy;
		}
		if (y > rt.bottom - diameter) {
			x = y = 0;
			vx = init_vx;
			vy = init_vy;
			block_width = init_b_width;
			life--;
		}

	Invalidate();//刷新，让其重绘
	CView::OnTimer(nIDEvent);
}


void CMFCApplication310View::drawBlock(CDC* pDC)
{
	// TODO: 在此处添加实现代码.
	CBrush b_brush(RGB(238,230,133));
	CRect rt;
	GetClientRect(&rt);
	CBrush *pOld = pDC->SelectObject(&b_brush);
	pDC->Rectangle(block_x, rt.bottom - block_height, block_x + block_width, rt.bottom);

	pDC->SelectObject(pOld);
	b_brush.DeleteObject();
}


void CMFCApplication310View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rt;
	GetClientRect(&rt);
	if (nChar == VK_LEFT) {
		block_x = block_x - block_vx;
		if(block_x < 0)
			block_x = 0;
	}
	if (nChar == VK_RIGHT) {
		block_x = block_x + block_vx;
		if (block_x+block_width > rt.right)
			block_x = rt.right-block_width;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMFCApplication310View::drawGameInfo(CDC* pDC)
{
	CRect rt;
	GetClientRect(&rt);
	int info_x = rt.right - 100;
	int infoHeight = 30;

	// TODO: 在此处添加实现代码.
	pDC->TextOutA(info_x, rt.top+infoHeight, "Game Info");

	CString glife;
	glife.Format("生命：%d ", life);
	pDC->TextOutA(info_x, rt.top + 2*infoHeight, glife);

	CString gmark;
	gmark.Format("得分：%d", mark);
	pDC->TextOutA(info_x, rt.top + 3*infoHeight, gmark);
}


void CMFCApplication310View::promptEnd(CDC* pDC)
{
	// TODO: 在此处添加实现代码.
	CRect rt;
	GetClientRect(&rt);
	int info_x = rt.right/2 - 60;
	int info_y = rt.bottom / 2 - 30;
	pDC->TextOutA(info_x, info_y, "游戏结束！");

}
