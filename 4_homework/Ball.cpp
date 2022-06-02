#include "StdAfx.h"
#include "Ball.h"


CBall::CBall(void)
{
}


CBall::~CBall(void)
{
	img.Destroy();
}

void CBall::create(int resid, int x, int y, int w, int h, double vx, double vy)
{
	img.Destroy();
	img.LoadFromResource(AfxGetInstanceHandle(),resid);
	img.SetTransparentColor(RGB(255,0,0));

	if (img.GetWidth() > w)
	{
		m_totalframe = img.GetWidth() / w; //总帧数
	}
	m_frame = 0;
	m_x = x;
	m_y = y;
	m_vx = vx;
	m_vy = vy;
	m_size.cx = w;
	m_size.cy = h;
	mark = 0;

	life = 3;
}

CRect CBall::getrect()
{
	CRect rt;
	rt.left = m_x;
	rt.top = m_y;
	rt.right = m_x + m_size.cx;
	rt.bottom = m_y + m_size.cy;
	return rt;
}

void CBall::setposition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CBall::setvelocity(double vx, double vy)
{
	m_vx = vx;
	m_vy = vy;
}

bool CBall::update(CRect win,CRect block)
{
	m_x = m_x + m_vx;
	m_y = m_y + m_vy;

	//POINT ball_left;
	//ball_left.x = m_x;
	//ball_left.y = m_y + (m_size.cy/2);

	//POINT ball_right;
	//ball_right.x = m_x + m_size.cx;
	//ball_right.y = m_y + (m_size.cy / 2);

	//POINT ball_top;
	//ball_top.x = m_x + m_size.cx;
	//ball_top.y = m_y ;

	//POINT ball_bottom;
	//ball_bottom.x = m_x + (m_size.cx / 2);
	//ball_bottom.y = m_y + m_size.cy;

	POINT ball_left;
	ball_left.x = m_x;
	ball_left.y = m_y;

	POINT ball_right;
	ball_right.x = m_x + m_size.cx;
	ball_right.y = m_y;

	POINT ball;
	ball.x = m_x + m_size.cx;
	ball.y = m_y + m_size.cy;

	if (block.PtInRect(ball)) {
		m_vy = -m_vy;
		mark += 10;
	}
	if (m_x < 0)
	{
		m_x = 0;
		m_vx = -m_vx;
	}
	if (m_x > win.right- m_size.cx)
	{
		m_x = win.right-m_size.cx;
		m_vx = -m_vx;
	}
	if (m_y < 0)
	{
		m_y = 0;
		m_vy = -m_vy;
	}
	if (m_y > win.bottom - m_size.cy)
	{
		life--;
		m_x = 0;
		m_y = BALL_START_Y;
		if (life <=0)
			return false;
	}
	return true;
}

void CBall::draw(CDC* pDC)
{
	img.TransparentBlt(pDC->GetSafeHdc(),m_x,m_y,m_size.cx,m_size.cy,
		m_frame * m_size.cx, 0, m_size.cx, m_size.cy);
	if (m_totalframe > 0)
	{
		m_frame = (m_frame + 1) % m_totalframe; //帧数循环切换，以此达到循环的效果
	}
}

void CBall::check(CRect brick) {
	m_x = m_x + m_vx;
	m_y = m_y + m_vy;

	POINT ball_left;
	ball_left.x = m_x;
	ball_left.y = m_y;

	POINT ball_right;
	ball_right.x = m_x + m_size.cx;
	ball_right.y = m_y;

	if (brick.PtInRect(ball_left)|| brick.PtInRect(ball_right)) {
	m_y += 5;
	m_vy = -m_vy;
	}
}
