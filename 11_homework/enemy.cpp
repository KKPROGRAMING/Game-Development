#include "stdafx.h"
#include "enemy.h"
#include "Map.h"

//implementation
void CEnemyTank::SetRandomDirections()
{
	m_Dir = (Direction)((m_Dir + (rand() % 3 + 1)) % 4);

	switch (m_Dir)
	{
	case DIR_UP:{
		m_Speed.x = 0;
		m_Speed.y = -1*(rand() % 6 + 3);
		break;}
	case DIR_DOWN:{
		m_Speed.x = 0;
		m_Speed.y = 1*(rand() % 6 + 3);
		break;}
	case DIR_LEFT:{
		m_Speed.x = -1*(rand() % 6 + 3);
		m_Speed.y = 0;
		break;}
	case DIR_RIGHT:{
		m_Speed.x = 1*(rand() % 6 + 3);
		m_Speed.y = 0;
		break;}
	}

}

CEnemyTank::CEnemyTank(CPoint pos, CObList* bullets,CPoint user_p) 
{
	m_ptPos = pos;
	m_rad   = 30;
	m_Bullets = bullets;

	m_Dir = DIR_UP;
	//SetRandomDirections();	
	f_user_p = user_p;
	FindUser_n();
	cnt[0] = -1;
	cnt[1] = -1;
	cnt[2] = -1;
	cnt[3] = -1;
}

CEnemyTank::~CEnemyTank() 
{
	for(int i=0; i<4; i++)
		img[i].DeleteObject();
	memDC.DeleteDC();
}

void CEnemyTank::SetImage(CDC* pDC, UINT ImgID[4])
{
	for (int i=0; i<4; i++)
		img[i].LoadBitmapA(ImgID[i]);

	memDC.CreateCompatibleDC(pDC);

	memDC.SelectObject(&img[0]);
}

bool CEnemyTank::Draw(CDC* pDC)
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

void CEnemyTank::Back()
{
	m_ptPos.x = m_ptPos.x - m_Speed.x;
	m_ptPos.y = m_ptPos.y - m_Speed.y;
}

void CEnemyTank::GetCheckPoint(CPoint& pt)
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

//����̹��״̬
bool CEnemyTank::UpdateState(CTankMap* map,   //��ͼ����
							 CObList *objs,   //̹���б���������̹�� 
							 CRect border)    //�߽�
{
	if ((rand() % 2) == 1)
		FindUser_n();
	//�õл���1/2�ĸ������¶�λ���λ��

	m_ptPos.x = m_ptPos.x + m_Speed.x;
	m_ptPos.y = m_ptPos.y + m_Speed.y;

	CPoint ctp = m_ptPos;
	GetCheckPoint(ctp);

	m_Border = border;
	if ( (m_ptPos.x < m_Border.left) || (m_ptPos.x > m_Border.right - m_rad) ||
		 (m_ptPos.y < m_Border.top)  || (m_ptPos.y > m_Border.bottom - m_rad) ||
		 !map->CanPass(ctp.x, ctp.y) || !map->CanPass(m_ptPos.x, m_ptPos.y))
	{
		//�˻�
		Back();

		int many = 0 ;//����ͳ���޷�ͨ�еķ��������������ڵ���2�������������򷽷������������һ������Ѱ·
		for (int i = 0; i < 4; i++)
			if (cnt[i] != -1)
				many += 1;

		if (many < 2) {
			//�ų��޷�ͨ�еķ���
			Direction forbidden = m_Dir;

			//�������Ѱ�ҷ��򣬶��������̹��ΪĿ��
			FindUser(forbidden);
			cnt[forbidden] = 1;
		}
		else {
			//�������
			SetRandomDirections();
			//����һ��־�����ٴγ�ʼ��
			cnt[0] = -1;
			cnt[1] = -1;
			cnt[2] = -1;
			cnt[3] = -1;
		}
		
	}

	//�������
	if (rand()%20 == 1)
		Fire();

	return TRUE;

}


void CEnemyTank::Fire()
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

/*
typedef enum {DIR_UP=0,
			  DIR_LEFT=1,
			  DIR_DOWN=2,
			  DIR_RIGHT=3} Direction;*/

//Ѱ�����λ��
void CEnemyTank::FindUser(Direction Forbidden_d) {

	if (f_user_p.y == m_ptPos.y && f_user_p.x != m_ptPos.x) {
		if (f_user_p.x > m_ptPos.x)
			m_Dir = DIR_LEFT;
		else
			m_Dir = DIR_RIGHT;
		return;
	}

	int random = rand()%2;
	double k = (1.0*(f_user_p.x-m_ptPos.x)) / (1.0*(f_user_p.y - m_ptPos.y));
	if (k < 0) {
		if(f_user_p.x > m_ptPos.x)
			if(random)
				m_Dir = DIR_RIGHT;
			else
				m_Dir = DIR_UP;
		else
			if(random)
				m_Dir = DIR_LEFT;
			else
				m_Dir = DIR_DOWN;
	}
	else if (k > 0) {
		if (f_user_p.x > m_ptPos.x)
			if(random)
				m_Dir = DIR_RIGHT;
			else
				m_Dir = DIR_DOWN;
		else
			if(random)
				m_Dir = DIR_LEFT;
			else
				m_Dir = DIR_UP;
	}
	else {
		if (f_user_p.y > m_ptPos.y)
			m_Dir = DIR_DOWN;
		else
			m_Dir = DIR_UP;
		return;
	}

	switch (m_Dir)
	{
	case DIR_UP: {
		if (Forbidden_d == DIR_UP)
			break;
		else {
			m_Speed.x = 0;
			m_Speed.y = -1 * (rand() % 6 + 3);
			break;
		}
	}
	case DIR_DOWN: {
		if (Forbidden_d == DIR_DOWN)
			break;
		else {
			m_Speed.x = 0;
			m_Speed.y = 1 * (rand() % 6 + 3);
			break;
		}
	}
	case DIR_LEFT: {
		if (Forbidden_d == DIR_LEFT)
			break;
		else {
			m_Speed.x = -1 * (rand() % 6 + 3);
			m_Speed.y = 0;
			break;
		}
	}
	case DIR_RIGHT: {
		if (Forbidden_d == DIR_UP)
			break;
		else {
			m_Speed.x = 1 * (rand() % 6 + 3);
			m_Speed.y = 0;
			break;
		}
	}
	}
}

void CEnemyTank::FindUser_n() {
	if (f_user_p.y == m_ptPos.y && f_user_p.x != m_ptPos.x) {
		if (f_user_p.x > m_ptPos.x)
			m_Dir = DIR_LEFT;
		else
			m_Dir = DIR_RIGHT;
		return;
	}

	int random = rand() % 2;
	double k = (1.0 * (f_user_p.x - m_ptPos.x)) / (1.0 * (f_user_p.y - m_ptPos.y));
	if (k < 0) {
		if (f_user_p.x > m_ptPos.x)
			if (random)
				m_Dir = DIR_RIGHT;
			else
				m_Dir = DIR_UP;
		else
			if (random)
				m_Dir = DIR_LEFT;
			else
				m_Dir = DIR_DOWN;
	}
	else if (k > 0) {
		if (f_user_p.x > m_ptPos.x)
			if (random)
				m_Dir = DIR_RIGHT;
			else
				m_Dir = DIR_DOWN;
		else
			if (random)
				m_Dir = DIR_LEFT;
			else
				m_Dir = DIR_UP;
	}
	else {
		if (f_user_p.y > m_ptPos.y)
			m_Dir = DIR_DOWN;
		else
			m_Dir = DIR_UP;
		return;
	}

	switch (m_Dir)
	{
	case DIR_UP: {
		m_Speed.x = 0;
		m_Speed.y = -1 * (rand() % 6 + 3);
		break; }
	case DIR_DOWN: {
		m_Speed.x = 0;
		m_Speed.y = 1 * (rand() % 6 + 3);
		break; }
	case DIR_LEFT: {
		m_Speed.x = -1 * (rand() % 6 + 3);
		m_Speed.y = 0;
		break; }
	case DIR_RIGHT: {
		m_Speed.x = 1 * (rand() % 6 + 3);
		m_Speed.y = 0;
		break; }
	}
}
