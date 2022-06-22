
// FlappyBirdTestView.cpp : CFlappyBirdTestView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "FlappyBirdTest.h"
#endif

#include "FlappyBirdTestDoc.h"
#include "FlappyBirdTestView.h"

#include "NameDlg.h"

#define IDB_BG2 321

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CFlappyBirdTestView

IMPLEMENT_DYNCREATE(CFlappyBirdTestView, CView)

BEGIN_MESSAGE_MAP(CFlappyBirdTestView, CView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CFlappyBirdTestView 构造/析构

CFlappyBirdTestView::CFlappyBirdTestView()
{
	// TODO: 在此处添加构造代码

}

CFlappyBirdTestView::~CFlappyBirdTestView()
{
}

BOOL CFlappyBirdTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFlappyBirdTestView 绘制
void CFlappyBirdTestView::DrawStartInfo(CDC* pDC)
{
	CFont font;

	font.CreateFontA(25,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		TRUE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"Tekton Pro");

	CFont* old = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255,0,0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutA(WIN_WIDTH / 5, WIN_HEIGHT/5,"Click Left Button to Start!");

	pDC->SelectObject(old);
	font.DeleteObject();
}
 

void CFlappyBirdTestView::DrawScoreLife(CDC* pDC, int Score, int Life)
{
	CFont font;
	
	font.CreateFontA(25,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		TRUE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"Tekton Pro");

	CFont* old = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->SetBkMode(TRANSPARENT);

	CString score;
	score.Format("Score: %d, life: %d", Score, Life);
	pDC->TextOutA(10, 10, score);

	//m_list.draw(pDC, 10, 50);

	pDC->SelectObject(old);
	font.DeleteObject();
}


void CFlappyBirdTestView::OnDraw(CDC* pDC)
{

	// 绘制背景
	CBitmap bmp;
	CDC memDC;
	CRect rect;
	CPoint point;
	BITMAP bmpinfo;

	if (m_score < 100)
		bmp.LoadBitmapA(IDB_BKGROUND);
	else
		bmp.LoadBitmapA(IDB_BG2);

	memDC.CreateCompatibleDC(pDC);
	CBitmap* old = memDC.SelectObject(&bmp);
	

	if (!isGameStart)
	{
		DrawStartInfo(&memDC);
		goto swap_exit;
	}
	//====画其他物体
	
	if (isGameOver)
	{
		//跳过其他物体绘制
		goto swap_exit;
	}

	for(int i = 0; i < 2; i++)
	{
	
		m_pipe[i].draw(&memDC);
		m_pipe[i].move(weight);

		if (m_pipe[i].isHit(m_bird.getRect()))
		{
			
			m_life--;
			InitGame();

			if (m_life <= 0)
			{
				isGameStart = false;
				CString msg;
				msg.Format("Game Over!Your Score is :%d\nDo you want to try again ?", m_score);

				CNameDlg aDlg;
				aDlg.m_Msg = msg;
				
				if (IDOK == aDlg.DoModal())
				{
					NameInfo newuser;
					newuser.name = aDlg.m_Name;
					newuser.score = m_score;
					m_list.update(newuser);

					RestartGame();
				}
				//if (IDYES == MessageBox(msg, "Game Over", MB_YESNO + MB_ICONINFORMATION))
				//{
				//	RestartGame();
				//}
				else
				{
					PostQuitMessage(0);
				}
				break;//防止循环
			}
			m_bird.init(IDB_BIRD, WIN_WIDTH / 2, WIN_HEIGHT / 2, 2.0);
		}
	
	}

	m_bird.draw(&memDC);
	m_bird.move();

	m_road.draw(&memDC);
	m_road.move();

	m_score++;
	weight *= m_score * 0.05;
	DrawScoreLife(&memDC, m_score,m_life);

swap_exit:
	//真正开始画到屏幕
	pDC->BitBlt(0,0,WIN_WIDTH,WIN_HEIGHT,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(old);
	bmp.DeleteObject();
}

void CFlappyBirdTestView::DrawBackground(CDC* pDC)
{

	CBitmap bmp;
	CDC memDC;
	BITMAP bmpinfo;

	bmp.LoadBitmapA(IDB_BKGROUND);
	bmp.GetBitmap(&bmpinfo);
	memDC.CreateCompatibleDC(pDC);
	CBitmap* old = memDC.SelectObject(&bmp);

	//draw
	pDC->BitBlt(0,0,bmpinfo.bmWidth,bmpinfo.bmHeight,&memDC,0,0,SRCCOPY);
	memDC.SelectObject(old);
	bmp.DeleteObject();

}

void CFlappyBirdTestView::InitGame()
{
	m_road.init(IDB_ROAD, 0, 450, 3.0);
	m_pipe[0].init(IDB_PIPE, WIN_WIDTH, -220, 3.0);
	m_pipe[1].init(IDB_PIPE, WIN_WIDTH + WIN_WIDTH / 2,
		-300, 3.0);

	m_bird.init(IDB_BIRD, WIN_WIDTH / 2, WIN_HEIGHT / 2,
		2.0);
}

void CFlappyBirdTestView::RestartGame()
{
	m_score = 0;
	isGameStart = true;
	m_life = 3;
	InitGame();
}

int CFlappyBirdTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	isGameOver = false;
	isGameStart = false;

	m_score = 0;
	m_life = 3;
	weight = 0;

	InitGame();
	SetTimer(1,100,NULL);
	mciSendString("play res/mus/bkmusic.mp3 repeat",NULL,0,NULL);

	return 0;
}


void CFlappyBirdTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Invalidate(TRUE);
	CView::OnTimer(nIDEvent);
}


void CFlappyBirdTestView::OnDestroy()
{
	KillTimer(1);
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


BOOL CFlappyBirdTestView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}


void CFlappyBirdTestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!isGameStart)
	{
		isGameStart = true;
	}
	else
	{
		m_bird.hit();

		PlaySound((LPCSTR)IDR_SWING, AfxGetInstanceHandle(),
			SND_RESOURCE | SND_ASYNC);
	}
	CView::OnLButtonDown(nFlags, point);
}


void CFlappyBirdTestView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CView::OnRButtonDown(nFlags, point);
}
