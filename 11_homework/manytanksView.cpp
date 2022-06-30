
// manytanksView.cpp : CmanytanksView 类的实现
//

#include "stdafx.h"
#include "mmsystem.h"
#ifndef SHARED_HANDLERS
#include "manytanks.h"
#include "Bullet.h"
#include "UserTank.h"
#include "map.h"
#endif

#include "manytanksDoc.h"
#include "manytanksView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//为简化演示，此处直接赋值地图数组
//0 : 石头
//1 : 水
//2 : 墙
//3 : 草地
//4 : 沙地
static int g_Map[] = { 0, 0, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    // 30
					   0, 0, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    // 2
					   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    // 3
					   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 4, 4, 4, 4, 4,    // 4
					   4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 4, 2, 2, 4, 4, 4, 2, 4, 4, 4, 2, 4, 2, 4, 2, 2, 4, 4, 4, 4,    // 5
					   2, 2, 2, 4, 4, 4, 4, 2, 4, 4, 4, 2, 2, 4, 4, 4, 2, 2, 4, 4, 2, 4, 2, 2, 4, 4, 4, 4, 4, 4,    // 6
					   2, 3, 2, 4, 4, 4, 4, 2, 4, 4, 2, 4, 4, 2, 4, 4, 2, 4, 2, 4, 2, 4, 2, 4, 4, 4, 4, 4, 4, 4,    // 7
					   2, 3, 2, 4, 4, 4, 4, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 4, 4, 2, 2, 4, 2, 2, 4, 4, 4, 4, 4, 4,    // 8
					   2, 3, 2, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 2, 4, 2, 4, 2, 4, 4, 4, 4, 4,    // 9
					   2, 3, 2, 4, 4, 4, 4, 2, 4, 2, 0, 0, 0, 0, 2, 4, 0, 0, 4, 4, 2, 4, 2, 4, 4, 2, 4, 4, 4, 4,    // 10
					   2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    // 11
					   4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 2, 4,    // 12
					   4, 4, 4, 4, 4, 4, 4, 4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 4, 4, 4, 4, 4, 2, 2, 4,    // 13
					   4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 4, 4, 4, 4, 0, 0, 0, 0, 0,    // 14
					   4, 4, 2, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4,    // 15
					   4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4,    // 16
					   4, 4, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 4, 4, 4, 4, 4,    // 17
					   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 4, 4, 4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4, 4,    // 18
					   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    // 19
					   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4     // 20

};
// CmanytanksView

IMPLEMENT_DYNCREATE(CmanytanksView, CView)

BEGIN_MESSAGE_MAP(CmanytanksView, CView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CmanytanksView 构造/析构

CmanytanksView::CmanytanksView()
{
	// TODO: 在此处添加构造代码
	//简单利用MCI播放背景音乐
	m_GameState = GAME_INIT_STATE;
	mciSendString("open bk.mp3 alias mp3",NULL,0,NULL);
	mciSendString("play mp3 repeat",NULL,0,NULL);
}

CmanytanksView::~CmanytanksView()
{
	POSITION pos = m_Bullets.GetHeadPosition();
	
	//release bullets list
	while(pos!=NULL)
	{
		delete m_Bullets.GetNext(pos);
	}
	
	m_Bullets.RemoveAll();

	//release tanks list
	pos = m_Tanks.GetHeadPosition();

	while(pos!=NULL)
	{
		delete m_Tanks.GetNext(pos);
	}

	m_Tanks.RemoveAll();

	//release tanks list
	pos = m_UserTanks.GetHeadPosition();

	while(pos!=NULL)
	{
		delete m_UserTanks.GetNext(pos);
	}

	m_UserTanks.RemoveAll();
}

BOOL CmanytanksView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CView::PreCreateWindow(cs);
}

// CmanytanksView 绘制

//游戏中绘制代码开始
void CmanytanksView::DrawSplashBMP(CDC* pDC)
{
	CRect rt;

	CBitmap bmp;
	CDC memDC;
	BITMAP bi;

	GetClientRect(&rt);

	bmp.LoadBitmapA(IDB_SPLASH);
	bmp.GetBitmap(&bi);

	memDC.CreateCompatibleDC(pDC);
	CBitmap* hOld = memDC.SelectObject(&bmp);

	pDC->StretchBlt(0,0,rt.Width(),rt.Height(),&memDC,0,0,bi.bmWidth,bi.bmHeight,SRCCOPY);	
	memDC.SelectObject(hOld);
	bmp.DeleteObject();
}

//绘制所有物体
void CmanytanksView::DrawAllObjects(CDC* pDC)
{
	CRect rt;

	POSITION pos;
	for (pos=m_Tanks.GetHeadPosition(); pos!=NULL;)
	{
		CEnemyTank* pTank = (CEnemyTank*)m_Tanks.GetNext(pos);
		pTank->Draw(pDC);
	}

	for (pos=m_UserTanks.GetHeadPosition(); pos!=NULL;)
	{
		CEnemyTank* pTank = (CEnemyTank*)m_UserTanks.GetNext(pos);
		pTank->Draw(pDC);
	}

	for (pos=m_Bullets.GetHeadPosition(); pos !=NULL;)
	{
		CBullet* pObj = (CBullet*)m_Bullets.GetNext(pos);

		if (!pObj->Draw(pDC))
		{
			delete pObj;
			m_Bullets.RemoveAt(pos);
		}
	}
}

void CmanytanksView::OnDraw(CDC* pDC)
{
	CmanytanksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rt;
	GetClientRect(&rt);

	//根据不同的游戏状态
	switch (m_GameState)
	{
	//首次，显示游戏LOGO
	case GAME_INIT_STATE:
		{
			DrawSplashBMP(pDC);
			break;
		}
    //游戏菜单
	case GAME_MENU_STATE:
		{
			break;
		}
	//游戏运行状态
	case GAME_RUN_STATE:
		{
			m_Map.Draw(pDC, rt);
			DrawAllObjects(pDC);
			break;
		}
	//游戏退出
	case GAME_EXIT_STATE:
		{
			break;
		}

	}
	
}


CmanytanksDoc* CmanytanksView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmanytanksDoc)));
	return (CmanytanksDoc*)m_pDocument;
}
 
// CmanytanksView 消息处理程序
int CmanytanksView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CClientDC dc(this);

	//设定四个不同方向的图像
	UINT imgID[] = {IDB_TANKU,IDB_TANKD,IDB_TANKL,IDB_TANKR};
	UINT userimgID[] = {IDB_U_TANKU,IDB_U_TANKD,IDB_U_TANKL,IDB_U_TANKR};

	//初始化地图
	UINT imgID_map[] = {IDB_MAP_1,
					    IDB_MAP_2,
						IDB_MAP_3,
						IDB_MAP_4,
						IDB_MAP_5};

	m_Map.SetImage(&dc, imgID_map);
	m_Map.SetMap(g_Map, _MAP_X_DIM_, _MAP_Y_DIM_);


	CPoint pt;
 
	//增加一辆用户坦克
	CUserTank* pTank = new CUserTank(CPoint(500,500), &m_Bullets);
	user_p.x = 500;
	user_p.y = 500;
	pTank->SetImage(&dc, userimgID);
	m_UserTanks.AddTail(pTank);

	//设置定时器
	SetTimer(1,100,NULL);

	return 0;
}


void CmanytanksView::OnTimer(UINT_PTR nIDEvent)
{

	CRect rt;
	GetClientRect(&rt);
	CClientDC dc(this);

	switch (m_GameState)
	{
	case GAME_INIT_STATE:
		{
			DrawSplashBMP(&dc);
			if(GetAsyncKeyState(VK_RETURN) || GetAsyncKeyState(VK_LBUTTON))
			{
				m_GameState = GAME_RUN_STATE;
			}
			break;
		}
	case GAME_MENU_STATE:
		{
			break;
		}
	case GAME_RUN_STATE:
		{
			POSITION pos,pos2;
			
			for(pos=m_Tanks.GetHeadPosition(); pos!=NULL;)
			{
				CEnemyTank* pTank = (CEnemyTank*)m_Tanks.GetNext(pos);

				//更新坦克状态
				pTank->f_user_p = user_p;//获取到当前玩家位置
				pTank->UpdateState(&m_Map, &m_Tanks, rt);
			}

			//玩家坦克
			for(pos=m_UserTanks.GetHeadPosition(); pos!=NULL;)
			{
				CUserTank* pTank = (CUserTank*)m_UserTanks.GetNext(pos);

				pTank->UpdateState(&m_Map, &m_Tanks, rt);
				pTank->getNowP(&user_p);//获取当前玩家位置并存放于user_p中，用来完成enemy的寻路操作
			}

			//更新子弹
			for(pos=m_Bullets.GetHeadPosition(); (pos2=pos)!=NULL; )
			{
				CBullet* pObject=(CBullet*)m_Bullets.GetNext(pos);

				if(!pObject->UpdateState(&m_Map, &m_Tanks, rt))
				{
					m_Bullets.RemoveAt(pos2);
					delete pObject;
				}
			}

			//设定四个不同方向的图像
			UINT imgID[] = {IDB_TANKU,IDB_TANKD,IDB_TANKL,IDB_TANKR};

			//产生坦克
			if (m_Tanks.GetCount() < 1)
			{
				CPoint pt;
				if (rand() % 20 == 1)
				{
					pt.x = (rand() % _MAP_X_DIM_) * _MAP_BLOCK_SIZE_;
					pt.y = (rand() % 5) * _MAP_BLOCK_SIZE_;

					while (!m_Map.CanPass(pt.x, pt.y))
					{
						pt.x = (rand() % _MAP_X_DIM_) * _MAP_BLOCK_SIZE_;
						pt.y = (rand() % 5) * _MAP_BLOCK_SIZE_;
					}

					CEnemyTank* pTank = new CEnemyTank(pt, &m_Bullets,user_p);
					m_Tanks.AddTail(pTank);

					pTank->SetImage(&dc, imgID);
				}
			}

			Invalidate();
			break;
		}
	case GAME_EXIT_STATE:
		{
			break;
		}

	}

	CView::OnTimer(nIDEvent);
}


BOOL CmanytanksView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
