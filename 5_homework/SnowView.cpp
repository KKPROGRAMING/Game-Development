
// SnowView.cpp : CSnowView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "SnowApp.h"
#endif

#include "SnowDoc.h"
#include "SnowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSnowView

IMPLEMENT_DYNCREATE(CSnowView, CView)

BEGIN_MESSAGE_MAP(CSnowView, CView)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

CSnowView::CSnowView()
{
}

CSnowView::~CSnowView()
{
}

BOOL CSnowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	HBRUSH br = CreateSolidBrush(RGB(0, 0, 0));
	LPCTSTR lp = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, 0, br);
	cs.lpszClass = lp;
	return CView::PreCreateWindow(cs);
}

// CSnowView ����

 
void CSnowView::OnDraw(CDC* pDC)
{
	drawBackground(pDC);
	for (int i = 0; i < CLOUD_NUMBERS; i++) {
		cloud[i].drawCloud(pDC, IDB_CLOUD);
	}
	for (int i = 0; i < SNOW_NUMBERS; i++) {
		snow[i].drawSnow(pDC, IDB_SNOW);
	}
}


// CSnowView ���

#ifdef _DEBUG
void CSnowView::AssertValid() const
{
	CView::AssertValid();
}

void CSnowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSnowDoc* CSnowView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSnowDoc)));
	return (CSnowDoc*)m_pDocument;
}
#endif //_DEBUG


// CSnowView ��Ϣ�������


int CSnowView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(0, 50, NULL);
	srand((unsigned)time(NULL));

	return 0;
}


void CSnowView::OnTimer(UINT_PTR nIDEvent)
{
	CRect rt;
	GetClientRect(&rt);

	//c_x += c_vx;

	t++;
	for (int i = 0; i < SNOW_NUMBERS; i++) {
		snow[i].moveSnow(rt);
	}
	for (int i = 0; i < CLOUD_NUMBERS; i++) {
		cloud[i].moveCloud(rt);
	}
	Invalidate();

	CView::OnTimer(nIDEvent);
}


void CSnowView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	t = 0;

	for (int i = 0; i < SNOW_NUMBERS; i++) {
		double x = rand() % 1500;
		double y = rand() % 1500 + 200;
		double vx = rand() % 5 - 3;
		double vy = rand() % 5 + 2 ;
		double ax = rand()%1;
		double ay = rand()%1 + 0.1;
		double life = rand() % 10 + 20;
		double radius = rand();
		double decay = 0.01;
		snow[i].createSnow(x, y, vx, vy, ax, ay, life, decay, radius);
	}

	for (int i = 0; i < CLOUD_NUMBERS; i++) {
		cloud[i].createCloud();
	}
}



void CSnowView::drawBackground(CDC* pDC)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BACKGROUND);

	//memory Device Contexgt
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pold = memDC.SelectObject(&bmp);

	//
	pDC->BitBlt(0, 0, 1500, 1000, &memDC, 0, 0, SRCCOPY);

	//pDC->TransparentBlt
	//pDC->StretchBlt()

	memDC.SelectObject(pold);
	memDC.DeleteDC();
	bmp.DeleteObject();
}
