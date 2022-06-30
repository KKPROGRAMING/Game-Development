#ifndef _MOVING_BALL_
#define _MOVING_BALL_
#include "movingObject.h"
#include "Bullet.h"

/*
typedef enum {DIR_UP=0,
			  DIR_LEFT=1,
			  DIR_DOWN=2,
			  DIR_RIGHT=3} Direction;*/

class CEnemyTank : public CMovingObject
{

public:
	CEnemyTank(CPoint pos, CObList* bullets,CPoint user_p) ;
	virtual ~CEnemyTank();

	bool Draw(CDC* pDC);
	bool UpdateState(CTankMap* map, CObList* objs, CRect border);

	void SetImage(CDC* pDC, UINT ImgID[4]);
	void Back();
	void Fire();

	void SetRandomDirections();

	//寻找目标玩家
	void FindUser(Direction Forbidden_d);//有参数
	void FindUser_n();//无参数

	CPoint f_user_p;//保存目标玩家位置

	int cnt[4];//用于收集无法通行的方向数量

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
