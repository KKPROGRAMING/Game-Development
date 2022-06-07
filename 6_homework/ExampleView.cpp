
// ExampleView.cpp : implementation of the CExampleView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Example.h"
#endif

#include "ExampleDoc.h"
#include "ExampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CExampleView

IMPLEMENT_DYNCREATE(CExampleView, CView)

BEGIN_MESSAGE_MAP(CExampleView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CExampleView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_PAINT()
	ON_COMMAND(ID_LIGHTUP, &CExampleView::OnLightup)
	ON_WM_CREATE()
	ON_COMMAND(ID_BTNBLEND, &CExampleView::OnBtnblend)
	ON_WM_TIMER()
//	ON_WM_MOUSEHWHEEL()
END_MESSAGE_MAP()

// CExampleView construction/destruction

CExampleView::CExampleView()
{
	//factor = 1;

	//初始化混合结构体成员
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = bTrans;
	bf.AlphaFormat = 0;
}

CExampleView::~CExampleView()
{
	//ReleaseDC(&memDC);
	//ReleaseDC(&memBKDC);
	//ReleaseDC(&SourceDC1);
	//ReleaseDC(&SourceDC2);
}

BOOL CExampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CExampleView drawing

void CExampleView::OnDraw(CDC* pDC)
{
	CExampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	//BitBlt(200, 20, 600, 300, &memBKDC, 0, 0, SRCCOPY);

	AlphaBlend(*pDC, 0, 0, 300,200,
		memBKDC, 0, 0, 300, 200, bf);

	AlphaBlend(*pDC, 0, 0, 300, 200,
		memDC, 0, 0, 300, 200, bf);
}


// CExampleView printing


void CExampleView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CExampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CExampleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CExampleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CExampleView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CExampleView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CExampleView diagnostics

#ifdef _DEBUG
void CExampleView::AssertValid() const
{
	CView::AssertValid();
}

void CExampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CExampleDoc* CExampleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExampleDoc)));
	return (CExampleDoc*)m_pDocument;
}
#endif //_DEBUG

void CExampleView::bilt(int tillid, int screenx, int screeny)
{
}

// CExampleView message handlers


//void CExampleView::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: Add your message handler code here
//	// Do not call CView::OnPaint() for painting messages
//
//	//BYTE *bmp_data = (BYTE*)malloc(bmif.bmHeight*bmif.bmWidth*3);
//	//unsigned int ret_bytes = pic->GetBitmapBits(bitmap_org.bmHeight*bitmap_org.bmWidth,bmp_index);
////	bmpbk.GetBitmapBits()
//
//	//dc.BitBlt(200,20,600,300,&memBKDC,0,0,SRCCOPY);
//	//dc.BitBlt(200, 20, 600, 300, &memBKDC, 0, 0, SRCCOPY);
//
//	//AlphaBlend(hdc, 0, 0, sBmp.cx, sBmp.cy,
//	//	hdcMem, 0, 0, sBmp.cx, sBmp.cy, bf);//绘制指定透明度的位图
//
//}


void CExampleView::OnLightup()
{
	// TODO: Add your command handler code here
	POINT t;
	BYTE R,G,B;

	BITMAP bmif;
	bmpbk.GetBitmap(&bmif);

	for(int j=0;j<bmif.bmHeight;j++)
		for(int i=0;i<bmif.bmWidth;i++)
		{
			t.x = i;
			t.y = j;
			COLORREF color = memBKDC.GetPixel(t);

			R = GetRValue(color);
			G = GetGValue(color);
			B = GetBValue(color);
#if 1
			R=(R+50)>255?255:R+50;
			//G=(G+50)>255?255:G+50;
			//B=(B+50)>255?255:B+50;
#else
			R=0;
			//G=0;
#endif
			memBKDC.SetPixel(t,RGB(R,G,B));

		}

	Invalidate(FALSE);
}


int CExampleView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CClientDC dc(this);

	//bmp.LoadBitmap(IDB_BITMAP2);
	//bmpbk.LoadBitmap(IDB_BITMAP1);
	bmpbk.LoadBitmap(IDB_BMPS1);

	//memDC.CreateCompatibleDC(&dc);
	memBKDC.CreateCompatibleDC(&dc);

	//memDC.SelectObject(&bmp);
	memBKDC.SelectObject(&bmpbk);

	bmp.LoadBitmap(IDB_BMPS2);

	//memDC.CreateCompatibleDC(&dc);
	memDC.CreateCompatibleDC(&dc);

	//memDC.SelectObject(&bmp);
	memDC.SelectObject(&bmp);


	//图像融合
	//sourceBMP1.LoadBitmap(IDB_BMPS1);
	//sourceBMP2.LoadBitmap(IDB_BMPS2);

	//SourceDC1.CreateCompatibleDC(&dc);
	//SourceDC2.CreateCompatibleDC(&dc);

	//SourceDC1.SelectObject(&sourceBMP1);
	//SourceDC2.SelectObject(&sourceBMP2);





	//
	return 0;
}


void CExampleView::OnBtnblend()
{

	SetTimer(1,500,NULL);
}


void CExampleView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your command handler code here
	//POINT t;
	//int  r,g,b;
	//
	//BITMAP bmif;
	//sourceBMP1.GetBitmap(&bmif);

	//factor = (factor + 10 ) % 255;  // 从1变换到255

	//for(int j=0;j<bmif.bmHeight;j++)
	//	for(int i=0;i<bmif.bmWidth;i++)
	//	{
	//		t.x = i;
	//		t.y = j;
	//		COLORREF color1 = SourceDC1.GetPixel(t);
	//		COLORREF color2 = SourceDC2.GetPixel(t);

	//		r = GetRValue(color1)*(255-factor)/255 + GetRValue(color2)*(factor)/255 ;
	//		g = GetGValue(color1)*(255-factor)/255 + GetGValue(color2)*(factor)/255 ;
	//		b = GetBValue(color1)*(255-factor)/255 + GetBValue(color2)*(factor)/255 ;

	//		if (r>255) r = 255;
	//		if (g>255) g = 255;
	//		if (b>255) b = 255;

	//		memBKDC.SetPixel(t,RGB(r,g,b));

	//	}

	bTrans += 20;
	bf.SourceConstantAlpha = bTrans;

		Invalidate(FALSE);
}

