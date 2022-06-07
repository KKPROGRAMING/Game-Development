
// Demo_mapView.cpp : CDemo_mapView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Demo_map.h"
#endif

#include "Demo_mapDoc.h"
#include "Demo_mapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAXBALL 100

typedef struct BALL
{
	float vx, vy, vz;
	float x, y, z;
	float r, g, b;
	int   stoped;
} CBALL;

static CBALL allBall[MAXBALL];

static float g = -9.8;
static float delta = 1;

static int   ticks = 1;
static int   nums  = 1;

// CDemo_mapView

IMPLEMENT_DYNCREATE(CDemo_mapView, CView)

BEGIN_MESSAGE_MAP(CDemo_mapView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CDemo_mapView 构造/析构

CDemo_mapView::CDemo_mapView()
{
	// TODO: 在此处添加构造代码
	// 初始化为0 

}

CDemo_mapView::~CDemo_mapView()
{
	KillTimer(0);
}

BOOL CDemo_mapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDemo_mapView 绘制

void CDemo_mapView::OnDraw(CDC* pDC)
{
	CDemo_mapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//DrawMap(pDC);
}

void CDemo_mapView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDemo_mapView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDemo_mapView 诊断

#ifdef _DEBUG
void CDemo_mapView::AssertValid() const
{
	CView::AssertValid();
}

void CDemo_mapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemo_mapDoc* CDemo_mapView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemo_mapDoc)));
	return (CDemo_mapDoc*)m_pDocument;
}
#endif //_DEBUG


// CDemo_mapView 消息处理程序

#if 1
void CDemo_mapView::DrawMap(CDC* pDC) //屏幕DC
{

	m_map[2][3] = -1;
	m_map[3][3] = -1;
	m_map[4][3] = -1;
	m_map[5][3] = -1;
	m_map[6][3] = -1;
	m_map[4][4] = -1;
	m_map[4][5] = -1;
	m_map[4][6] = -1;
	m_map[8][3] = -1;
	m_map[9][3] = -1;
	m_map[10][3] = -1;

	m_map[8][4] = -1;
	m_map[10][4] = -1;
	m_map[7][5] = -1;
	m_map[8][5] = -1;
	m_map[9][5] = -1;
	m_map[10][5] = -1;
	m_map[11][5] = -1;
	m_map[7][6] = -1;
	m_map[11][6] = -1;

	m_map[13][3] = -1;
	m_map[16][3] = -1;
	m_map[13][4] = -1;
	m_map[14][4] = -1;
	m_map[16][4] = -1;
	m_map[13][5] = -1;
	m_map[15][5] = -1;
	m_map[16][5] = -1;
	m_map[13][6] = -1;
	m_map[16][6] = -1;

	m_map[19][3] = -1;
	m_map[21][3] = -1;
	m_map[19][4] = -1;
	m_map[20][4] = -1;
	m_map[19][5] = -1;
	m_map[21][5] = -1;
	m_map[19][6] = -1;
	m_map[22][6] = -1;


	CBitmap bmp;
	CDC memDC;  //内存虚拟画布，可以画，但看不到，从内存复制到屏幕才显示

	memDC.CreateCompatibleDC(pDC);  //内存dc创建和屏幕dc兼容

	for (int row = 0; row < MAX_BLOCK_NUM; row++)
		for (int col = 0; col < MAX_BLOCK_NUM; col++)
		{
			//int bmpID = IDB_BITMAP1;// rand() % 5 + IDB_BITMAP1;  //随机取ID1 rand:0-4
			//m_map[row][col] = bmpID;
			if (m_map[row][col] != -1)
				m_map[row][col] = IDB_BITMAP1;
			else
				m_map[row][col] = IDB_BITMAP4;

			bmp.LoadBitmap(m_map[row][col]);
			memDC.SelectObject(&bmp);

			int y = col * BLOCK_SIZE;
			int x = row * BLOCK_SIZE;
			pDC->BitBlt(x, y, BLOCK_SIZE, BLOCK_SIZE,
				&memDC, 0, 0, SRCCOPY);

			bmp.DeleteObject();

		}
}
#else
void CDemo_mapView::DrawMap(CDC* pDC)
{

	CRect rt;
	GetClientRect(&rt);

	for (int i = 0; i < nums % MAXBALL; i++)  //loop 100 balls
	{
		if (!allBall[i].stoped)
		{
			allBall[i].vy = allBall[i].vy + g * delta;  //update delta v = a * delta t
														//vx,vz can also be updated

			allBall[i].x += allBall[i].vx * delta;    //delta s  = v * delta t;
			allBall[i].y -= allBall[i].vy * delta;

			if (allBall[i].y >= rt.Height() - 10)
			{
				allBall[i].stoped = 1;
				nums++;
			}
		}
		pDC->Ellipse(allBall[i].x, allBall[i].y, allBall[i].x + 10, allBall[i].y + 10);
	}
}

#endif

void CDemo_mapView::OnPaint()
{
	CPaintDC dc(this); 
	DrawMap(&dc);
}


void CDemo_mapView::OnTimer(UINT_PTR nIDEvent)
{
	CView::OnTimer(nIDEvent);
	Invalidate(FALSE);
}

void init()
{
	delta = 0.1;
	for (int i = 0; i < MAXBALL; i++)
	{

		allBall[i].x = 10;
		allBall[i].y = 500;

		allBall[i].vx = (rand() % 20) * 5 + 2;
		allBall[i].vy = -((rand() % 20) * 2  -  2);
	}
}
int CDemo_mapView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetTimer(0, 100, NULL);
	//init();

	return 0;
}
