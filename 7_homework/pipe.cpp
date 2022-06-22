#include "StdAfx.h"
#include "FlappyBirdTest.h"
#include "Pipe.h"


CPipe::CPipe(void)
{
}


CPipe::~CPipe(void)
{
	bmp.DeleteObject();
}

void CPipe::init(int ID, int x, int y, double speed)
{
	resID = ID;
	pos.x = x;
	pos.y = y;
	speedx = speed;
	speedy = 0;
	ax = ay = 0;
	SPACE = 100;

	BITMAP bmpinfo;
	if (!bmp.GetSafeHandle())
		bmp.LoadBitmapA(resID);
	bmp.GetBitmap(&bmpinfo);

	size.cx = bmpinfo.bmWidth;
	size.cy = bmpinfo.bmHeight;

}

void CPipe::draw(CDC* pDC)
{
	CDC memDC;

	memDC.CreateCompatibleDC(pDC);
	CBitmap *old = memDC.SelectObject(&bmp);

	
	pDC->TransparentBlt(pos.x,pos.y,size.cx,size.cy,&memDC,
		0,0,size.cx,size.cy, RGB(0,0,0));
	pDC->TransparentBlt(pos.x,pos.y + size.cy + SPACE,size.cx,size.cy,&memDC,
		0,0,size.cx,size.cy, RGB(0,0,0));

	

	//space可以修改成随机数
}

void CPipe::move(double weight)
{
	double speed_now = speedx * ( 1 + weight);
	pos.x = pos.x - speedx * 1.0;
	if (pos.x < -size.cx)
	{
		pos.x = WIN_WIDTH;
		pos.y = rand() % 200 - 300;
		SPACE = rand() % 40 + 80;
	}
		
}

bool CPipe::isHit(CRect obj)
{
	bool ret = false;
	
	CRect checkrt;
	CRect uRect(pos.x,pos.y,pos.x+size.cx,pos.y+size.cy);
	CRect dRect(pos.x,pos.y+size.cy + SPACE,pos.x+size.cx,pos.y+size.cy+SPACE+size.cy);

	if (checkrt.IntersectRect(&uRect, &obj) ||
		checkrt.IntersectRect(&dRect, &obj) || obj.bottom > WIN_HEIGHT) 
	{
		
		
		if(checkrt.IntersectRect(&uRect, &obj) ||checkrt.IntersectRect(&dRect, &obj))
			PlaySound((LPCSTR)IDR_HIT, AfxGetInstanceHandle(),SND_RESOURCE | SND_ASYNC);
		else
			PlaySound((LPCSTR)IDR_JUMP, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		return true;
	}
	
	else
		return false;

	return false;
}
