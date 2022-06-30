#ifndef _USER_TANK_H_
#define _USER_TANK_H_
#include "movingObject.h"
#include "Bullet.h"

class CUserTank : public CMovingObject
{

public:
	CUserTank(CPoint pos, CObList* bullets);
	virtual ~CUserTank();

	bool Draw(CDC* pDC);
	bool UpdateState(CTankMap* map, CObList* objs, CRect border);

	void SetImage(CDC* pDC, UINT ImgID[4]);
	void Back();
	void Fire();

	void Move(Direction dir);

	void getNowP(CPoint* user_p);

protected:
	void GetCheckPoint(CPoint& pt);

private:
	int m_rad;
	COLORREF  color;
	CBitmap   img[4];
	CDC	      memDC;
	CObList * m_Bullets;
	Direction m_Dir;
};

#endif
