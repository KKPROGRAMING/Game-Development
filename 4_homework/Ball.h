#pragma once
#include "gameobj.h"
#include "Brick.h"


class CBall :
	public CGameObj
{
	CImage img;
public:
	CBall(void);
	~CBall(void);

	void create(int resid, int x, int y, int w, int h, double vx, double vy);
	CRect getrect();
	void setposition(int x, int y);
	void setvelocity(double vx, double vy);

	bool update(CRect win,CRect brick);
	void check(CRect brick);
	void draw(CDC* pDC);

	void invertX() { m_vx = -m_vx;}
	void invertY() { m_vy = -m_vy;}
public:
	int life;
	int mark;

};

