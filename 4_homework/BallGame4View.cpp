
// BallGame4View.cpp : CBallGame4View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "BallGame4.h"
#endif

#include "BallGame4Doc.h"
#include "BallGame4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBallGame4View

IMPLEMENT_DYNCREATE(CBallGame4View, CView)

BEGIN_MESSAGE_MAP(CBallGame4View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CBallGame4View 构造/析构

CBallGame4View::CBallGame4View()
{
	// TODO: 在此处添加构造代码

}

CBallGame4View::~CBallGame4View()
{
}

BOOL CBallGame4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

void CBallGame4View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBallGame4View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBallGame4View 诊断

#ifdef _DEBUG
void CBallGame4View::AssertValid() const
{
	CView::AssertValid();
}

void CBallGame4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBallGame4Doc* CBallGame4View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBallGame4Doc)));
	return (CBallGame4Doc*)m_pDocument;
}
#endif //_DEBUG


// CBallGame4View 消息处理程序


// CBallGame4View 绘制

void CBallGame4View::OnDraw(CDC* pDC)
{
	CBallGame4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (ball.life > 0) {
		DrawBackground(pDC);
		ball.draw(pDC);
		block.draw(pDC);
		//drawBlock(pDC);
		showLife(pDC);
		//draw bricks
		for (int r = 0; r < BRICK_ROWS; r++)
			for (int c = 0; c < BRICK_COLS; c++)
			{
				bricks[r][c].draw(pDC);
			}
	}
	else {
		promptEnd(pDC);
	}


}


void CBallGame4View::DrawBackground(CDC* pDC)
{
	CBitmap bmp;
	bmp.LoadBitmapA(IDB_BACKGROUND);

	//memory Device Contexgt
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pold = memDC.SelectObject(&bmp);

	//
	pDC->BitBlt(0,0,WIN_WIDTH,WIN_HEIGHT,&memDC,0,0,SRCCOPY);

	//pDC->TransparentBlt
	//pDC->StretchBlt()

	memDC.SelectObject(pold);
	memDC.DeleteDC();
	bmp.DeleteObject();
}



void CBallGame4View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	SetTimer(0,100,NULL);

	CRect rt;
	GetClientRect(&rt);
	ball.create(IDB_BALL,0,BALL_START_Y,BALL_WIDTH,BALL_HEIGHT,4.0,4.0);
	block.create(IDB_BLOCK,BLOCK_X,rt.bottom-BLOCK_HEIGHT,BLOCK_WIDTH,BLOCK_HEIGHT,BLOCK_VX);
	

	for(int r = 0; r < BRICK_ROWS; r++)
		for (int c = 0; c < BRICK_COLS; c++)
		{
			int type = rand() % 5;
			bricks[r][c].create(IDB_BRICK, type, c*BRICK_WIDTH, r*BRICK_HEIGHT,BRICK_WIDTH,BRICK_HEIGHT);
		}


}


void CBallGame4View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// udpate

	CRect rt;
	GetClientRect(&rt);
	//CRect block(block_x, rt.bottom - block_height, block_x + block_width, rt.bottom);
	/*if (bricks[ball.getrect().top / BRICK_HEIGHT][ball.getrect().right / BRICK_WIDTH].m_exist) {

	}*/
	POINT brick;
	brick.x = int(floor(ball.getrect().top / BRICK_HEIGHT));
	brick.y = int(floor(ball.getrect().left / BRICK_WIDTH));//哈希表，节约遍历找出被撞击的砖块的时间

	if (bricks[brick.x][brick.y].m_exist==true) {
		ball.check(bricks[brick.x][brick.y].getrect());
		bricks[brick.x][brick.y].m_exist = false;
		bricks[brick.x][brick.y].change(327);
	}
	else {
		ball.update(rt, block.getrect());
	}

	Invalidate();
	CView::OnTimer(nIDEvent);
}

//
//void CBallGame4View::drawBlock(CDC* pDC)
//{
//
//	// TODO: 在此处添加实现代码.
//	CBitmap bmp;
//	bmp.LoadBitmapA(IDB_BLOCK);
//	//bmp.LoadBitmapA(323);
//
//	CDC memDC;
//	memDC.CreateCompatibleDC(pDC);
//	CBitmap* pold = memDC.SelectObject(&bmp);
//
//	CRect rt;
//	GetClientRect(&rt);
//
//	pDC->BitBlt(block_x, rt.bottom - block_height, block_width, block_height, &memDC, 0, 0, SRCCOPY);
//
//	memDC.SelectObject(pold);
//	memDC.DeleteDC();
//	bmp.DeleteObject();
//}


void CBallGame4View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rt;
	//GetClientRect(&rt);
	//if (nChar == VK_LEFT) {
	//	block_x -= block_vx;
	//	if (block_x < 0) {
	//		block_x = 0;
	//	}
	//	
	//}
	//if (nChar == VK_RIGHT) {
	//	//block.move(rt, 'r');
	//	block_x += block_vx;
	//	if (block_x > rt.right-block_width) {
	//		block_x = rt.right - block_width;
	//	}
	//	
	//}

	CRect rt;
	GetClientRect(&rt);
	if (nChar == VK_LEFT) {
		block.move(rt, 'l');

	}
	if (nChar == VK_RIGHT) {
		block.move(rt, 'r');
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CBallGame4View::showLife(CDC* pDC)
{
	// TODO: 在此处添加实现代码.
	CRect rt;
	GetClientRect(&rt);
	int info_x = rt.right - 200;
	int infoHeight = 30;

	SetTextColor(*pDC,RGB(255,255,255));
	SetBkMode(*pDC, TRANSPARENT);

	// TODO: 在此处添加实现代码.
	pDC->TextOutA(info_x, BRICK_ROWS*BRICK_HEIGHT + 2*infoHeight, "Game Info");

	CString glife;
	glife.Format("生命：%d ", ball.life);
	pDC->TextOutA(info_x, BRICK_ROWS * BRICK_HEIGHT + 3 * infoHeight, glife);

	CString gmark;
	gmark.Format("得分：%d", ball.mark);
	pDC->TextOutA(info_x, BRICK_ROWS * BRICK_HEIGHT + 4 * infoHeight, gmark);
}


void CBallGame4View::promptEnd(CDC* pDC)
{
	// TODO: 在此处添加实现代码.
	CRect rt;
	GetClientRect(&rt);
	int info_x = rt.right / 2 - 60;
	int info_y = rt.bottom / 2 - 30;
	pDC->TextOutA(info_x, info_y, "游戏结束！");

}
