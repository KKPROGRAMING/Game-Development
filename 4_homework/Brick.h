#pragma once
#include "gameobj.h"

class CBrick :
	public CGameObj
{
	CImage img;
public:
	CBrick(void);
	~CBrick(void);

	void create(int resid,int type,int x, int y, int w, int h);
	CRect getrect();
	void draw(CDC* pDC);
	void change(int tmp);
public:
	int m_type;
	bool m_isVisible;
	bool m_exist;//¼ì²â¸Ã×©¿éÊÇ·ñ´æÔÚ
};

