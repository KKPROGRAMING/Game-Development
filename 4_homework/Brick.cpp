#include "StdAfx.h"
#include "Brick.h"


CBrick::CBrick(void)
{
}


CBrick::~CBrick(void)
{
	img.Destroy();
}

void CBrick::create(int resid,int type,int x, int y, int w, int h)
{
	img.Destroy();
	img.LoadFromResource(AfxGetInstanceHandle(), resid);

	m_size.cx = w;
	m_size.cy = h;
	m_x = x;
	m_y = y;
	
	m_type = type;
	m_isVisible = true;
	m_exist = false;
}

CRect CBrick::getrect()
{
	CRect rt;
	rt.left = m_x;
	rt.top = m_y;
	rt.right = m_x + m_size.cx;
	rt.bottom = m_y + m_size.cy;
	return rt;
}

void CBrick::draw(CDC* pDC)
{
	img.TransparentBlt(pDC->GetSafeHdc(),m_x,m_y,m_size.cx,m_size.cy,
		0,m_type * m_size.cy, m_size.cx, m_size.cy, RGB(255,255,255));

	m_exist = true;
}

void CBrick::change(int tmp) {
	img.Destroy();
	img.LoadFromResource(AfxGetInstanceHandle(), tmp);
}