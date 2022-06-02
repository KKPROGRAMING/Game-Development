
// BallGame4View.cpp : CBallGame4View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CBallGame4View ����/����

CBallGame4View::CBallGame4View()
{
	// TODO: �ڴ˴���ӹ������

}

CBallGame4View::~CBallGame4View()
{
}

BOOL CBallGame4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

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


// CBallGame4View ���

#ifdef _DEBUG
void CBallGame4View::AssertValid() const
{
	CView::AssertValid();
}

void CBallGame4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBallGame4Doc* CBallGame4View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBallGame4Doc)));
	return (CBallGame4Doc*)m_pDocument;
}
#endif //_DEBUG


// CBallGame4View ��Ϣ�������


// CBallGame4View ����

void CBallGame4View::OnDraw(CDC* pDC)
{
	CBallGame4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// udpate

	CRect rt;
	GetClientRect(&rt);
	//CRect block(block_x, rt.bottom - block_height, block_x + block_width, rt.bottom);
	/*if (bricks[ball.getrect().top / BRICK_HEIGHT][ball.getrect().right / BRICK_WIDTH].m_exist) {

	}*/
	POINT brick;
	brick.x = int(floor(ball.getrect().top / BRICK_HEIGHT));
	brick.y = int(floor(ball.getrect().left / BRICK_WIDTH));//��ϣ����Լ�����ҳ���ײ����ש���ʱ��

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
//	// TODO: �ڴ˴����ʵ�ִ���.
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ˴����ʵ�ִ���.
	CRect rt;
	GetClientRect(&rt);
	int info_x = rt.right - 200;
	int infoHeight = 30;

	SetTextColor(*pDC,RGB(255,255,255));
	SetBkMode(*pDC, TRANSPARENT);

	// TODO: �ڴ˴����ʵ�ִ���.
	pDC->TextOutA(info_x, BRICK_ROWS*BRICK_HEIGHT + 2*infoHeight, "Game Info");

	CString glife;
	glife.Format("������%d ", ball.life);
	pDC->TextOutA(info_x, BRICK_ROWS * BRICK_HEIGHT + 3 * infoHeight, glife);

	CString gmark;
	gmark.Format("�÷֣�%d", ball.mark);
	pDC->TextOutA(info_x, BRICK_ROWS * BRICK_HEIGHT + 4 * infoHeight, gmark);
}


void CBallGame4View::promptEnd(CDC* pDC)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	CRect rt;
	GetClientRect(&rt);
	int info_x = rt.right / 2 - 60;
	int info_y = rt.bottom / 2 - 30;
	pDC->TextOutA(info_x, info_y, "��Ϸ������");

}
