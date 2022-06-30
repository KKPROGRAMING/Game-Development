
// SubMarineView.cpp : CSubMarineView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "SubMarine.h"
#endif

#include "SubMarineDoc.h"
#include "SubMarineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//int bomb_kind = 0;

// CSubMarineView

IMPLEMENT_DYNCREATE(CSubMarineView, CView)

BEGIN_MESSAGE_MAP(CSubMarineView, CView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CSubMarineView 构造/析构

CSubMarineView::CSubMarineView()
{
	// TODO: 在此处添加构造代码
	//  装载图像资源文件
	CExplosion::LoadImage();
	CMyShip::LoadImage();
	CSubmarine::LoadImage();
	Bplus::LoadImage();
	CBomb::LoadImage();
	CTorpedo::LoadImage();

	life = 30 ;


	// 创建一艘我方军舰, 增加到数组中
	CMyObject* ship1 = new
		CMyShip(CPoint((GAME_WIDTH - SHIP_WIDTH) * 1 / 2,
			SHIP_VERT_POS), 0);

	m_Objects[MYSHIP].AddTail(ship1);

	m_bGamePause = false;
}

CSubMarineView::~CSubMarineView()
{
	// 释放对象
	for (int i = 0; i < 7; i++)
	{
		// delete all objects
		POSITION pos = m_Objects[i].GetHeadPosition();

		while (pos != NULL)
		{
			delete m_Objects[i].GetNext(pos);
		}
		m_Objects[i].RemoveAll();
	}

	// clear image lists
	CExplosion::DeleteImage();
	CMyShip::DeleteImage();
	CSubmarine::DeleteImage();
	CBomb::DeleteImage();
	Bplus::DeleteImage();
	CTorpedo::DeleteImage();

	m_VirtualBitmap.DeleteObject();
	m_VirtualDC.DeleteDC();
}

BOOL CSubMarineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	//	cs.dwExStyle |= WS_EX_CLIENTEDGE;   
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW ,
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1), NULL);

	return CView::PreCreateWindow(cs);
}

// CSubMarineView 诊断

#ifdef _DEBUG
void CSubMarineView::AssertValid() const
{
	CView::AssertValid();
}

void CSubMarineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSubMarineDoc* CSubMarineView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSubMarineDoc)));
	return (CSubMarineDoc*)m_pDocument;
}
#endif //_DEBUG


// CSubMarineView 消息处理程序


int CSubMarineView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	// 建立后台绘图缓冲 ,see "bitblt" in line 118,323
	CClientDC dc(this);
	m_VirtualBitmap.CreateCompatibleBitmap(&dc, GAME_WIDTH, GAME_HEIGHT);
	m_VirtualDC.CreateCompatibleDC(&dc);
	m_VirtualDC.SelectObject(&m_VirtualBitmap);

	// create loop
	SetTimer(0, 30, NULL); //UINT SetTimer( UINT nIDEvent, UINT nElapse, NULL)
	srand((unsigned)time(NULL)); // 设置随机数种子


	return 0;
}


void CSubMarineView::OnDraw(CDC* pDC)
{
	CSubMarineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	static int nFrame = 0;
	static int nCreator = random(15) + 15; //用作是否创建敌方潜艇

	int i, j, k;
	CRect l_Rect;
	CMyObject* pObject;

	if (m_bGamePause)
		return;

	// 创建新的敌方潜艇
	if (nCreator == 0)
	{
		m_Objects[SUBMARINE].AddTail
		(new CSubmarine(random(2) % 2 ? true : false,
			random(GAME_HEIGHT - SHIP_VERT_POS - SHIP_HEIGHT * 2 - 30) + SHIP_VERT_POS + SHIP_HEIGHT + 30,
			random(4),
			random(4) + 3));

		nCreator = random(15) + 15; //随机等待时间 
	}

	// 绘制天空、大海
	m_VirtualDC.FillSolidRect(0, 0, GAME_WIDTH, SHIP_VERT_POS + SHIP_HEIGHT - 5, SKY_COLOR);
	m_VirtualDC.FillSolidRect(0, SHIP_VERT_POS + SHIP_HEIGHT - 5, GAME_WIDTH, GAME_HEIGHT, SEA_COLOR);

	// 绘制所有物体
	POSITION pos1, pos2, pos3, pos4;
	CSubmarine* pSub;
	CMyShip* pMyShip;
	if (m_Objects[MYSHIP].GetCount() != 0)
		pMyShip = (CMyShip*)m_Objects[MYSHIP].GetHead();//Get my ship

	for (j = 0; j<7; j++) //历遍所有物体，将它们绘制出来
		for (i = 0, pos1 = m_Objects[j].GetHeadPosition();
		(pos2 = pos1) != NULL; i++)
	{
		pObject = (CMyObject*)m_Objects[j].GetNext(pos1);

		if (pObject->Draw(&m_VirtualDC, m_bGamePause))  // 绘制
		{
			m_Objects[j].RemoveAt(pos2);
			delete pObject;
		}
		//下面代码控制敌方潜艇发射鱼雷
		else if (m_Objects[MYSHIP].GetCount() != 0 && j == SUBMARINE)
		{
			//我方战船，MYSHIP
			for (k = 0; k<m_Objects[MYSHIP].GetCount(); k++)
			{
				if (0 == k) //如果是第一条船
					pMyShip = (CMyShip*)m_Objects[MYSHIP].GetHead();
				if (1 == k)
					pMyShip = (CMyShip*)m_Objects[MYSHIP].GetTail();
				//对于敌方潜艇，如果我方战舰在上方
				if (pObject->GetPos().x + SUBMARINE_WIDTH / 2 > pMyShip->GetPos().x &&
					pObject->GetPos().x + SUBMARINE_WIDTH / 2 < pMyShip->GetPos().x + SHIP_WIDTH)
				{
					//如果还没有开炮，则随机开炮（1/10概率）
					if (((CSubmarine*)pObject)->GetFireFlag() &&
						random(10) == 2)
					{
						CTorpedo *pTorpedo = new CTorpedo(CPoint(pObject->GetPos().x + SUBMARINE_WIDTH / 2 - TORPEDO_WIDTH / 2,
							pObject->GetPos().y));
						m_Objects[ENEMY_BOMB].AddTail(pTorpedo);
						((CSubmarine*)pObject)->SetFireFlag(); //已经开炮
					}
				}//if
			}//for
		}
	}//for

	 //  检测爆炸火球和潜艇之间是否发生碰撞，
	 //  因为爆炸火球附近如果有潜艇，则引起二次爆炸

	for (pos1 = m_Objects[EXPLOSION].GetHeadPosition(); (pos2 = pos1) != NULL;)
	{
		pObject = (CMyObject*)m_Objects[EXPLOSION].GetNext(pos1);

		for (pos3 = m_Objects[SUBMARINE].GetHeadPosition(); (pos4 = pos3) != NULL;)
		{
			pSub = (CSubmarine*)m_Objects[SUBMARINE].GetNext(pos3);
			l_Rect = pObject->GetRect();

			//检测是否发生碰撞
			if ((l_Rect.IntersectRect(pSub->GetRect(), l_Rect)))
			{
				// 如果碰到潜艇，则产生二次爆炸
				CExplosion *pExplosion = new CExplosion(pSub->GetPos());

				// 发生二次爆炸，加倍记分
				pExplosion->SetMulti(((CExplosion*)pObject)->GetMulti() + 1);

				// 把爆炸添加到爆炸列表中，并加分
				m_Objects[EXPLOSION].AddTail(pExplosion);
				m_Objects[SCORE].AddTail(new CScore(pSub->GetPos(), pExplosion->GetMulti(), pSub->GetType() + 1, RGB(random(255), random(255), random(255))));

				// 删除潜艇
				m_Objects[SUBMARINE].RemoveAt(pos4);
				delete pSub;
				break; //jump out

			}//if
		}//for

	}//for

	 // 检测炸弹是否击中潜艇(bomb)
	for (pos1 = m_Objects[MY_BOMB].GetHeadPosition(); (pos2 = pos1) != NULL;)
	{
		//得到炸弹对象
		pObject = (CMyObject*)m_Objects[MY_BOMB].GetNext(pos1);

		//检查所有的潜艇，看是否和该炸弹发生碰撞
		for (pos3 = m_Objects[SUBMARINE].GetHeadPosition(); (pos4 = pos3) != NULL;)
		{
			pSub = (CSubmarine*)m_Objects[SUBMARINE].GetNext(pos3);  // save for deletion
			l_Rect = ((CBomb*)pObject)->GetRect();

			//一旦发生碰撞
			if ((l_Rect.IntersectRect(pSub->GetRect(), l_Rect)))
			{
				// 首先在潜艇位置创建一个爆炸效果，并记分
				//CExplosion new_exp = CExplosion(pSub->GetPos());
				//new_exp.sound();
				m_Objects[EXPLOSION].AddTail(new CExplosion(pSub->GetPos()));
				//增加爆炸音效
				PlaySound((LPCSTR)IDR_BOMB, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				m_Objects[SCORE].AddTail(new CScore(pSub->GetPos(), 1, pSub->GetType() + 1, RGB(random(255), random(255), random(255))));

				// 删除该潜艇
				m_Objects[SUBMARINE].RemoveAt(pos4);
				delete pSub;

				// 删除该炸弹
				m_Objects[MY_BOMB].RemoveAt(pos2);
				delete pObject;
				// break is important! to avoid next recycle use a bomb not exist
				break;

			}//if
		}//for

	}//for

	// 检测炸弹是否击中潜艇(bombplus)
	for (pos1 = m_Objects[MY_BOMBPLUS].GetHeadPosition(); (pos2 = pos1) != NULL;)
	{
		//得到炸弹对象
		pObject = (CMyObject*)m_Objects[MY_BOMBPLUS].GetNext(pos1);

		//检查所有的潜艇，看是否和该炸弹发生碰撞
		for (pos3 = m_Objects[SUBMARINE].GetHeadPosition(); (pos4 = pos3) != NULL;)
		{
			pSub = (CSubmarine*)m_Objects[SUBMARINE].GetNext(pos3);  // save for deletion
			l_Rect = ((Bplus*)pObject)->GetRect();

			//一旦发生碰撞
			if ((l_Rect.IntersectRect(pSub->GetRect(), l_Rect)))
			{
				// 首先在潜艇位置创建一个爆炸效果，并记分
				//CExplosion new_exp = CExplosion(pSub->GetPos());
				//new_exp.sound();
				m_Objects[EXPLOSION].AddTail(new CExplosion(pSub->GetPos()));
				//增加爆炸音效
				PlaySound((LPCSTR)IDR_BOMB, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				m_Objects[SCORE].AddTail(new CScore(pSub->GetPos(), 1, pSub->GetType() + 1, RGB(random(255), random(255), random(255))));

				// 删除该潜艇
				m_Objects[SUBMARINE].RemoveAt(pos4);
				delete pSub;

				// 删除该炸弹
				m_Objects[MY_BOMBPLUS].RemoveAt(pos2);
				delete pObject;
				// break is important! to avoid next recycle use a bomb not exist
				break;

			}//if
		}//for

	}//for

	 //  检测潜艇发射鱼雷是否击中我方
	if (m_Objects[MYSHIP].GetCount() != 0)
		for (pos1 = m_Objects[ENEMY_BOMB].GetHeadPosition(); (pos2 = pos1) != NULL;)
		{
			// 获取敌方鱼雷对象
			pObject = (CMyObject*)m_Objects[ENEMY_BOMB].GetNext(pos1);

			//遍历我方的船只
			for (pos3 = m_Objects[MYSHIP].GetHeadPosition(); (pos4 = pos3) != NULL;)
			{
				// 得到我方船只对象
				pMyShip = (CMyShip*)m_Objects[MYSHIP].GetNext(pos3);
				l_Rect = pObject->GetRect();

				//如果敌方潜艇所发射鱼雷和我船只发生碰撞
				if ((l_Rect.IntersectRect(pMyShip->GetRect(), l_Rect)))
				{
					// 创建爆炸效果
					m_Objects[EXPLOSION].AddTail(new CExplosion(pMyShip->GetPos()));
					PlaySound((LPCSTR)IDR_BOMB, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
					//减少船只的生命值
					life -= 10;
					// 删除我方船只
					if (life <= 0) {
						m_Objects[MYSHIP].RemoveAt(pos4);
						delete pMyShip;
						//增加结束音效
						PlaySound(LPCSTR(IDR_OVER), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
					}
					// 删除敌方鱼雷
					m_Objects[ENEMY_BOMB].RemoveAt(pos2);
					delete pObject;

					// break is important! to avoid next recycle use a bomb not exist
					break;

				}//if
			}//for

		}//for

		 //  判断是否游戏结束
	if (m_Objects[MYSHIP].GetCount() == 0)
	{
		m_VirtualDC.SetTextColor(RGB(255, 0, 0));
		m_VirtualDC.SetTextAlign(TA_CENTER);
		m_VirtualDC.TextOut(GAME_WIDTH / 2, GAME_HEIGHT / 2, "GAME OVER");
		DrawBlendBkground(&m_VirtualDC);
	}

	//  恢复原来字体对齐
	m_VirtualDC.SetTextAlign(TA_LEFT);
	//  输出游戏信息
	m_VirtualDC.SetBkMode(TRANSPARENT);
	m_VirtualDC.SetTextColor(RGB(255, 0, 0));
	CString string;
	string.Format("Total Enemy: %d , Total Bomb: %d , Life:%d", m_Objects[SUBMARINE].GetCount(), m_Objects[MY_BOMB].GetCount(),life);
	m_VirtualDC.TextOut(10, 10, string);
	//  输出分数
	string.Format("Your score: %d", CScore::GetTotalScore());
	m_VirtualDC.TextOut(350, 10, string);

	nCreator--;
	nFrame++;  //每帧计数

			   // 将内存DC画到屏幕
	pDC->BitBlt(0, 0, GAME_WIDTH, GAME_HEIGHT,
		&m_VirtualDC, 0, 0, SRCCOPY);
}

void CSubMarineView::DrawBlendBkground(CDC* dc)
{
	CDC memDC;
	CBitmap bk;
	CRect rt;
	static int nFactor = 100;
	static int sign = 1;

	GetClientRect(&rt);
	memDC.CreateCompatibleDC(dc);
	bk.CreateCompatibleBitmap(dc, rt.Width(), rt.Height());
	memDC.SelectObject(&bk);
	memDC.FillSolidRect(&rt, RGB(255, 0, 0));

	BLENDFUNCTION bn;

	bn.AlphaFormat = 0;
	bn.BlendFlags = 0;
	bn.BlendOp = AC_SRC_OVER;
	bn.SourceConstantAlpha = nFactor; //透明度设置，0为不透明；255为完全透明

	dc->AlphaBlend(0, 0, rt.Width(), rt.Height(),
		&memDC, 0, 0, rt.Width(), rt.Height(), bn);

	if (nFactor >= 150) sign = -1;
	else if (nFactor <= 100) sign = 1;

	nFactor += sign;

	memDC.DeleteDC();
	bk.DeleteObject();
}

void CSubMarineView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Invalidate();
	CView::OnTimer(nIDEvent);
}


BOOL CSubMarineView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return true;
	return CView::OnEraseBkgnd(pDC);
}


void CSubMarineView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bGamePause)
		return;
	if (m_Objects[MYSHIP].GetCount() == 0)//my ship is not exist
		return;
	//获取Ship1,ship2的指针
	CMyShip* pShip1 = (CMyShip*)m_Objects[MYSHIP].GetHead(); // remember that our ship1 is the first object[MYSHIP] in object list
 	switch (nChar)
	{
	case VK_LEFT:       // turn left
		pShip1->SetMotion(-1);
		break;

	case VK_RIGHT: // turn right
		pShip1->SetMotion(1);
		break;
 

	case VK_SPACE: // throw bomb from middle
		if ((m_Objects[MY_BOMB].GetCount() + m_Objects[MY_BOMBPLUS].GetCount() )<MAX_BOMBNUM)
			m_Objects[MY_BOMB].AddTail(new CBomb(pShip1->GetPos().x + SHIP_WIDTH / 2 - BOMB_WIDTH / 2, 3));
		break;
	case 'o':
	case 'O':	// throw bomb on left side
		if ((m_Objects[MY_BOMB].GetCount() + m_Objects[MY_BOMBPLUS].GetCount()) <MAX_BOMBNUM)
			m_Objects[MY_BOMB].AddTail(new CBomb(pShip1->GetPos().x - BOMB_WIDTH / 2, 3));
		break;

	case 'p':
	case 'P':	// throw bomb on right side
		if ((m_Objects[MY_BOMB].GetCount() + m_Objects[MY_BOMBPLUS].GetCount()) <MAX_BOMBNUM)
			m_Objects[MY_BOMB].AddTail(new CBomb(pShip1->GetPos().x + SHIP_WIDTH - BOMB_WIDTH / 2, 3));
		break;


	case 'S':
	case 's'://有一定概率扔出一颗超级炸弹
			if ((m_Objects[MY_BOMB].GetCount() + m_Objects[MY_BOMBPLUS].GetCount()) < MAX_BOMBNUM)
				m_Objects[MY_BOMBPLUS].AddTail(new Bplus(pShip1->GetPos().x + SHIP_WIDTH - BOMB_WIDTH / 2, 3));
			break;


	}//switch

}


void CSubMarineView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bGamePause)
		return;

	if (m_Objects[MYSHIP].GetCount() == 0) //如果全部玩家战败
		return;

	//获取Ship1,ship2的指针
	CMyShip* pShip1 = (CMyShip*)m_Objects[MYSHIP].GetHead();
 
	switch (nChar)
	{ // 如果在onTimer没有执行前就释放按键，则放弃当前移动
 
	case VK_LEFT:
		if (pShip1->GetMotion() == -1)       // turn left
			pShip1->SetMotion(0);

		break;
	case VK_RIGHT:
		if (pShip1->GetMotion() == 1) // turn right
			pShip1->SetMotion(0);

		break;
	}
}


void CSubMarineView::OnDestroy()
{
	CView::OnDestroy();

	KillTimer(0);
}
