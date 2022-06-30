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

	//Ѱ��Ŀ�����
	void FindUser(Direction Forbidden_d);//�в���
	void FindUser_n();//�޲���

	CPoint f_user_p;//����Ŀ�����λ��

	int cnt[4];//�����ռ��޷�ͨ�еķ�������

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
