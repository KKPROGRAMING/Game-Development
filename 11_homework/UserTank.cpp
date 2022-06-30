#include "stdafx.h"
#include "UserTank.h"
#include "Map.h"

//implementation
void CUserTank::Move(Direction dir)
{
	m_Dir = dir;

	switch (m_Dir)
	{
	case DIR_UP:{
		m_Speed.x = 0;
		m_Speed.y = -1*(rand()%6+1);
		break;}
	case DIR_DOWN:{
		m_Speed.x = 0;
		m_Speed.y = 1*(rand()%6+1);
		break;}
	case DIR_LEFT:{
		m_Speed.x = -1*(rand()%8+1);
		m_Speed.y = 0;
		break;}
	case DIR_RIGHT:{
		m_Speed.x = 1*(rand()%8+1);
		m_Speed.y = 0;
		break;}
	}

	m_ptPos.x = m_ptPos.x + m_Speed.x;
	m_ptPos.y = m_ptPos.y + m_Speed.y;

	CPoint ctp = m_ptPos;
	GetCheckPoint(ctp);

	if ( (m_ptPos.x < m_Border.left) || (m_ptPos.x > m_Border.right - m_rad) ||
		(m_ptPos.y < m_Border.top)  || (m_ptPos.y > m_Border.bottom - m_rad))
	{
		//back
		Back();
	}

}

CUserTank::CUserTank(CPoint pos, CObList* bullets)
{
	m_ptPos = pos;
	m_rad   = 30;
	m_Bullets = bullets;

	m_Dir = DIR_UP;
}

CUserTank::~CUserTank() 
{
	for(int i=0; i<4; i++)
		img[i].DeleteObject();
	memDC.DeleteDC();
}

void CUserTank::SetImage(CDC* pDC, UINT ImgID[4])
{
	for (int i=0; i<4; i++)
		img[i].LoadBitmapA(ImgID[i]);

	memDC.CreateCompatibleDC(pDC);

	memDC.SelectObject(&img[0]);
}

bool CUserTank::Draw(CDC* pDC)
{
	//check the direction
	switch (m_Dir)
	{
	case DIR_UP:
		{
			memDC.SelectObject(&img[0]);
			break;
		}
	case DIR_DOWN:
		{
			memDC.SelectObject(&img[1]);
			break;
		}
	case DIR_LEFT:
		{
			memDC.SelectObject(&img[2]);
			break;
		}
	case DIR_RIGHT:
		{
			memDC.SelectObject(&img[3]);
			break;
		}
	}
	pDC->StretchBlt(m_ptPos.x, m_ptPos.y, m_rad, m_rad, &memDC, 0 , 0, 60, 60, SRCAND);//SRCCOPY );
	return TRUE;
}

void CUserTank::Back()
{
	m_ptPos.x = m_ptPos.x - m_Speed.x;
	m_ptPos.y = m_ptPos.y - m_Speed.y;
}

void CUserTank::GetCheckPoint(CPoint& pt)
{
	switch (m_Dir)
	{
	case DIR_UP:
		{
			pt.x += m_rad / 2;
			break;
		}
	case DIR_DOWN:
		{
			pt.x += m_rad / 2;
			pt.y += m_rad;
			break;
		}
	case DIR_LEFT:
		{
			pt.y += m_rad / 2;
			break;
		}
	case DIR_RIGHT:
		{
			pt.x += m_rad;
			pt.y += m_rad / 2;
			break;
		}
	}

}

bool CUserTank::UpdateState(CTankMap* map, CObList *objs, CRect border)
{
	m_Border = border;

	if (GetAsyncKeyState(VK_LEFT))
		Move(DIR_LEFT);
	else
		if (GetAsyncKeyState(VK_RIGHT))
			Move(DIR_RIGHT);
		else
			if (GetAsyncKeyState(VK_UP))
				Move(DIR_UP);
			else
				if (GetAsyncKeyState(VK_DOWN))
					Move(DIR_DOWN);
				else if (GetAsyncKeyState(VK_SPACE))
					Fire();

	return TRUE;

}


void CUserTank::Fire()
{
	CPoint firePoint;
	switch (m_Dir)
	{
	case DIR_UP:
		{
			firePoint.x = m_ptPos.x + m_rad / 2;
			firePoint.y = m_ptPos.y;
			break;
		}
	case DIR_DOWN:
		{
			firePoint.x = m_ptPos.x + m_rad / 2;
			firePoint.y = m_ptPos.y + m_rad;
			break;
		}
	case DIR_LEFT:
		{
			firePoint.x = m_ptPos.x;
			firePoint.y = m_ptPos.y + m_rad / 2;
			break;
		}
	case DIR_RIGHT:
		{
			firePoint.x = m_ptPos.x + m_rad;
			firePoint.y = m_ptPos.y + m_rad / 2;
			break;
		}
	}

	m_Bullets->AddTail(new CBullet(firePoint,m_Dir));

}

void CUserTank::getNowP(CPoint* user_p) {
	user_p->x = m_ptPos.x;
	user_p->x = m_ptPos.x;
}