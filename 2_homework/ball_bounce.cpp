
// MFCApplication1View.cpp: CMFCApplication1View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMFCApplication1View 构造/析构

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: 在此处添加构造代码

}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View 绘图

void CMFCApplication1View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CMFCApplication1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication1View 诊断

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View 消息处理程序


void CMFCApplication1View::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CView::OnPaint()
	drawBall(&dc,CPoint(m_circle.center.x, m_circle.center.y), m_circle.radius);
}


void CMFCApplication1View::drawBall(CDC* pDC, CPoint Center, int radius)
{
	// TODO: 在此处添加实现代码.

		CBrush brush, * pOldBrush; //旧画刷的指针
		brush.CreateSolidBrush(RGB(255, 0, 0));
		pOldBrush = pDC->SelectObject(&brush);
		pDC->Ellipse(Center.x - radius, Center.y - radius, Center.x + radius, Center.y + radius);
		brush.DeleteObject();
		pDC->SelectObject(pOldBrush);

}


int CMFCApplication1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	SetTimer(0, 200, NULL);
	return 0;
}


void CMFCApplication1View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		// TODO: Add your message handler code here and/or call default
		RECT m_rect;
		GetClientRect(&m_rect); //获得屏幕显示区域大小结构体
		moveAdjust(m_circle, m_rect);
		m_circle.center.x += m_circle.vx;
		m_circle.center.y -= m_circle.vy; //改变当前小球的位置
		Invalidate(true);
		CView::OnTimer(nIDEvent);

}


//main change!!
void CMFCApplication1View::moveAdjust(MyCircle& circle, RECT rect)
{
	// TODO: 在此处添加实现代码.
		if (circle.center.y + circle.radius >= rect.bottom)
		{
			circle.vy = -circle.vy;
		} //碰底
		if (circle.center.x + circle.radius >= rect.right)
		{
			circle.vx = -circle.vx;
		} //碰右
		if (circle.center.y - circle.radius <= rect.top)
		{
			circle.vy = -circle.vy;
		} //碰顶
		if (circle.center.x - circle.radius <= rect.left)
		{
			circle.vx = -circle.vx;
		} //碰左
		return;

}
