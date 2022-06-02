#include "stdafx.h"
#include "Block.h"

Block::Block(void) {

}
Block::~Block(void) {
}

void Block::create(int img, int x, int y, int width, int height, int v) {

	b_x = x;
	b_y = y;
	b_width = width;
	b_height = height;
	b_v = v;
	//b_img = img;

	b_img.Destroy();
	b_img.LoadFromResource(AfxGetInstanceHandle(), img);
	//b_img.SetTransparentColor(RGB(0, 0, 0));
}

void Block::move(CRect win, char ch) {
	if (ch == 'l') {
		b_x -= b_v;
		if (b_x < 0)
			b_x = 0;
	}
	if (ch == 'r') {
		b_x += b_v;
		if (b_x > win.right - b_width)
			b_x = win.right - b_width;
	}

}

void Block::draw(CDC* pDC) {

	b_img.BitBlt(pDC->GetSafeHdc(),b_x, b_y, b_width, b_height, 0, 0);
	//img.BitBlt(b_x, b_y, b_width, b_height, 0, 0,);
	//img.TransparentBlt(pDC->GetSafeHdc(), b_x, b_y, b_width, b_height,0, 0, b_width, b_height);

	//CBitmap bmp;
	//bmp.LoadBitmapA(b_img);
	////bmp.LoadBitmapA(323);

	//CDC memDC;
	//memDC.CreateCompatibleDC(pDC);
	//CBitmap* pold = memDC.SelectObject(&bmp);

	//HWND h = AfxGetMainWnd()->m_hWnd;
	//CRect rt;
	//GetClientRect(h,&rt);

	//pDC->BitBlt(b_x, rt.bottom - b_height, b_width, b_height, &memDC, 0, 0, SRCCOPY);

	////pDC->TransparentBlt
	////pDC->StretchBlt()

	//memDC.SelectObject(pold);
	//memDC.DeleteDC();
	//bmp.DeleteObject();

	//b_img.TransparentBlt(pDC->GetSafeHdc(), b_x, b_y, b_width, b_height,
		//0, 0,b_width, b_height);
}

CRect Block::getrect() {
	CRect rt;
	rt.left = b_x;
	rt.top = b_y;
	rt.right = b_x + b_width;
	rt.bottom = b_y +b_height;
	return rt;
}
