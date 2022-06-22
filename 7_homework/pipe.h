#pragma once
#include "gameobj.h"
class CPipe :
	public CGameObj
{
	int resID;
	int SPACE;
	
	CBitmap bmp;

public:
	CPipe(void);
	~CPipe(void);

	void init(int ID, int x, int y, double speed);
	virtual void draw(CDC* pDC);
	virtual void move(double weight);

	bool isHit(CRect obj);
};

