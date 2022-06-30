
// SubMarineView.cpp : CSubMarineView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CSubMarineView ����/����

CSubMarineView::CSubMarineView()
{
	// TODO: �ڴ˴���ӹ������
	//  װ��ͼ����Դ�ļ�
	CExplosion::LoadImage();
	CMyShip::LoadImage();
	CSubmarine::LoadImage();
	Bplus::LoadImage();
	CBomb::LoadImage();
	CTorpedo::LoadImage();

	life = 30 ;


	// ����һ���ҷ�����, ���ӵ�������
	CMyObject* ship1 = new
		CMyShip(CPoint((GAME_WIDTH - SHIP_WIDTH) * 1 / 2,
			SHIP_VERT_POS), 0);

	m_Objects[MYSHIP].AddTail(ship1);

	m_bGamePause = false;
}

CSubMarineView::~CSubMarineView()
{
	// �ͷŶ���
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	//	cs.dwExStyle |= WS_EX_CLIENTEDGE;   
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW ,
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1), NULL);

	return CView::PreCreateWindow(cs);
}

// CSubMarineView ���

#ifdef _DEBUG
void CSubMarineView::AssertValid() const
{
	CView::AssertValid();
}

void CSubMarineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSubMarineDoc* CSubMarineView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSubMarineDoc)));
	return (CSubMarineDoc*)m_pDocument;
}
#endif //_DEBUG


// CSubMarineView ��Ϣ�������


int CSubMarineView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	// ������̨��ͼ���� ,see "bitblt" in line 118,323
	CClientDC dc(this);
	m_VirtualBitmap.CreateCompatibleBitmap(&dc, GAME_WIDTH, GAME_HEIGHT);
	m_VirtualDC.CreateCompatibleDC(&dc);
	m_VirtualDC.SelectObject(&m_VirtualBitmap);

	// create loop
	SetTimer(0, 30, NULL); //UINT SetTimer( UINT nIDEvent, UINT nElapse, NULL)
	srand((unsigned)time(NULL)); // �������������


	return 0;
}


void CSubMarineView::OnDraw(CDC* pDC)
{
	CSubMarineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	static int nFrame = 0;
	static int nCreator = random(15) + 15; //�����Ƿ񴴽��з�Ǳͧ

	int i, j, k;
	CRect l_Rect;
	CMyObject* pObject;

	if (m_bGamePause)
		return;

	// �����µĵз�Ǳͧ
	if (nCreator == 0)
	{
		m_Objects[SUBMARINE].AddTail
		(new CSubmarine(random(2) % 2 ? true : false,
			random(GAME_HEIGHT - SHIP_VERT_POS - SHIP_HEIGHT * 2 - 30) + SHIP_VERT_POS + SHIP_HEIGHT + 30,
			random(4),
			random(4) + 3));

		nCreator = random(15) + 15; //����ȴ�ʱ�� 
	}

	// ������ա���
	m_VirtualDC.FillSolidRect(0, 0, GAME_WIDTH, SHIP_VERT_POS + SHIP_HEIGHT - 5, SKY_COLOR);
	m_VirtualDC.FillSolidRect(0, SHIP_VERT_POS + SHIP_HEIGHT - 5, GAME_WIDTH, GAME_HEIGHT, SEA_COLOR);

	// ������������
	POSITION pos1, pos2, pos3, pos4;
	CSubmarine* pSub;
	CMyShip* pMyShip;
	if (m_Objects[MYSHIP].GetCount() != 0)
		pMyShip = (CMyShip*)m_Objects[MYSHIP].GetHead();//Get my ship

	for (j = 0; j<7; j++) //�����������壬�����ǻ��Ƴ���
		for (i = 0, pos1 = m_Objects[j].GetHeadPosition();
		(pos2 = pos1) != NULL; i++)
	{
		pObject = (CMyObject*)m_Objects[j].GetNext(pos1);

		if (pObject->Draw(&m_VirtualDC, m_bGamePause))  // ����
		{
			m_Objects[j].RemoveAt(pos2);
			delete pObject;
		}
		//���������Ƶз�Ǳͧ��������
		else if (m_Objects[MYSHIP].GetCount() != 0 && j == SUBMARINE)
		{
			//�ҷ�ս����MYSHIP
			for (k = 0; k<m_Objects[MYSHIP].GetCount(); k++)
			{
				if (0 == k) //����ǵ�һ����
					pMyShip = (CMyShip*)m_Objects[MYSHIP].GetHead();
				if (1 == k)
					pMyShip = (CMyShip*)m_Objects[MYSHIP].GetTail();
				//���ڵз�Ǳͧ������ҷ�ս�����Ϸ�
				if (pObject->GetPos().x + SUBMARINE_WIDTH / 2 > pMyShip->GetPos().x &&
					pObject->GetPos().x + SUBMARINE_WIDTH / 2 < pMyShip->GetPos().x + SHIP_WIDTH)
				{
					//�����û�п��ڣ���������ڣ�1/10���ʣ�
					if (((CSubmarine*)pObject)->GetFireFlag() &&
						random(10) == 2)
					{
						CTorpedo *pTorpedo = new CTorpedo(CPoint(pObject->GetPos().x + SUBMARINE_WIDTH / 2 - TORPEDO_WIDTH / 2,
							pObject->GetPos().y));
						m_Objects[ENEMY_BOMB].AddTail(pTorpedo);
						((CSubmarine*)pObject)->SetFireFlag(); //�Ѿ�����
					}
				}//if
			}//for
		}
	}//for

	 //  ��ⱬը�����Ǳ֮ͧ���Ƿ�����ײ��
	 //  ��Ϊ��ը���򸽽������Ǳͧ����������α�ը

	for (pos1 = m_Objects[EXPLOSION].GetHeadPosition(); (pos2 = pos1) != NULL;)
	{
		pObject = (CMyObject*)m_Objects[EXPLOSION].GetNext(pos1);

		for (pos3 = m_Objects[SUBMARINE].GetHeadPosition(); (pos4 = pos3) != NULL;)
		{
			pSub = (CSubmarine*)m_Objects[SUBMARINE].GetNext(pos3);
			l_Rect = pObject->GetRect();

			//����Ƿ�����ײ
			if ((l_Rect.IntersectRect(pSub->GetRect(), l_Rect)))
			{
				// �������Ǳͧ����������α�ը
				CExplosion *pExplosion = new CExplosion(pSub->GetPos());

				// �������α�ը���ӱ��Ƿ�
				pExplosion->SetMulti(((CExplosion*)pObject)->GetMulti() + 1);

				// �ѱ�ը��ӵ���ը�б��У����ӷ�
				m_Objects[EXPLOSION].AddTail(pExplosion);
				m_Objects[SCORE].AddTail(new CScore(pSub->GetPos(), pExplosion->GetMulti(), pSub->GetType() + 1, RGB(random(255), random(255), random(255))));

				// ɾ��Ǳͧ
				m_Objects[SUBMARINE].RemoveAt(pos4);
				delete pSub;
				break; //jump out

			}//if
		}//for

	}//for

	 // ���ը���Ƿ����Ǳͧ(bomb)
	for (pos1 = m_Objects[MY_BOMB].GetHeadPosition(); (pos2 = pos1) != NULL;)
	{
		//�õ�ը������
		pObject = (CMyObject*)m_Objects[MY_BOMB].GetNext(pos1);

		//������е�Ǳͧ�����Ƿ�͸�ը��������ײ
		for (pos3 = m_Objects[SUBMARINE].GetHeadPosition(); (pos4 = pos3) != NULL;)
		{
			pSub = (CSubmarine*)m_Objects[SUBMARINE].GetNext(pos3);  // save for deletion
			l_Rect = ((CBomb*)pObject)->GetRect();

			//һ��������ײ
			if ((l_Rect.IntersectRect(pSub->GetRect(), l_Rect)))
			{
				// ������Ǳͧλ�ô���һ����ըЧ�������Ƿ�
				//CExplosion new_exp = CExplosion(pSub->GetPos());
				//new_exp.sound();
				m_Objects[EXPLOSION].AddTail(new CExplosion(pSub->GetPos()));
				//���ӱ�ը��Ч
				PlaySound((LPCSTR)IDR_BOMB, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				m_Objects[SCORE].AddTail(new CScore(pSub->GetPos(), 1, pSub->GetType() + 1, RGB(random(255), random(255), random(255))));

				// ɾ����Ǳͧ
				m_Objects[SUBMARINE].RemoveAt(pos4);
				delete pSub;

				// ɾ����ը��
				m_Objects[MY_BOMB].RemoveAt(pos2);
				delete pObject;
				// break is important! to avoid next recycle use a bomb not exist
				break;

			}//if
		}//for

	}//for

	// ���ը���Ƿ����Ǳͧ(bombplus)
	for (pos1 = m_Objects[MY_BOMBPLUS].GetHeadPosition(); (pos2 = pos1) != NULL;)
	{
		//�õ�ը������
		pObject = (CMyObject*)m_Objects[MY_BOMBPLUS].GetNext(pos1);

		//������е�Ǳͧ�����Ƿ�͸�ը��������ײ
		for (pos3 = m_Objects[SUBMARINE].GetHeadPosition(); (pos4 = pos3) != NULL;)
		{
			pSub = (CSubmarine*)m_Objects[SUBMARINE].GetNext(pos3);  // save for deletion
			l_Rect = ((Bplus*)pObject)->GetRect();

			//һ��������ײ
			if ((l_Rect.IntersectRect(pSub->GetRect(), l_Rect)))
			{
				// ������Ǳͧλ�ô���һ����ըЧ�������Ƿ�
				//CExplosion new_exp = CExplosion(pSub->GetPos());
				//new_exp.sound();
				m_Objects[EXPLOSION].AddTail(new CExplosion(pSub->GetPos()));
				//���ӱ�ը��Ч
				PlaySound((LPCSTR)IDR_BOMB, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				m_Objects[SCORE].AddTail(new CScore(pSub->GetPos(), 1, pSub->GetType() + 1, RGB(random(255), random(255), random(255))));

				// ɾ����Ǳͧ
				m_Objects[SUBMARINE].RemoveAt(pos4);
				delete pSub;

				// ɾ����ը��
				m_Objects[MY_BOMBPLUS].RemoveAt(pos2);
				delete pObject;
				// break is important! to avoid next recycle use a bomb not exist
				break;

			}//if
		}//for

	}//for

	 //  ���Ǳͧ���������Ƿ�����ҷ�
	if (m_Objects[MYSHIP].GetCount() != 0)
		for (pos1 = m_Objects[ENEMY_BOMB].GetHeadPosition(); (pos2 = pos1) != NULL;)
		{
			// ��ȡ�з����׶���
			pObject = (CMyObject*)m_Objects[ENEMY_BOMB].GetNext(pos1);

			//�����ҷ��Ĵ�ֻ
			for (pos3 = m_Objects[MYSHIP].GetHeadPosition(); (pos4 = pos3) != NULL;)
			{
				// �õ��ҷ���ֻ����
				pMyShip = (CMyShip*)m_Objects[MYSHIP].GetNext(pos3);
				l_Rect = pObject->GetRect();

				//����з�Ǳͧ���������׺��Ҵ�ֻ������ײ
				if ((l_Rect.IntersectRect(pMyShip->GetRect(), l_Rect)))
				{
					// ������ըЧ��
					m_Objects[EXPLOSION].AddTail(new CExplosion(pMyShip->GetPos()));
					PlaySound((LPCSTR)IDR_BOMB, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
					//���ٴ�ֻ������ֵ
					life -= 10;
					// ɾ���ҷ���ֻ
					if (life <= 0) {
						m_Objects[MYSHIP].RemoveAt(pos4);
						delete pMyShip;
						//���ӽ�����Ч
						PlaySound(LPCSTR(IDR_OVER), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
					}
					// ɾ���з�����
					m_Objects[ENEMY_BOMB].RemoveAt(pos2);
					delete pObject;

					// break is important! to avoid next recycle use a bomb not exist
					break;

				}//if
			}//for

		}//for

		 //  �ж��Ƿ���Ϸ����
	if (m_Objects[MYSHIP].GetCount() == 0)
	{
		m_VirtualDC.SetTextColor(RGB(255, 0, 0));
		m_VirtualDC.SetTextAlign(TA_CENTER);
		m_VirtualDC.TextOut(GAME_WIDTH / 2, GAME_HEIGHT / 2, "GAME OVER");
		DrawBlendBkground(&m_VirtualDC);
	}

	//  �ָ�ԭ���������
	m_VirtualDC.SetTextAlign(TA_LEFT);
	//  �����Ϸ��Ϣ
	m_VirtualDC.SetBkMode(TRANSPARENT);
	m_VirtualDC.SetTextColor(RGB(255, 0, 0));
	CString string;
	string.Format("Total Enemy: %d , Total Bomb: %d , Life:%d", m_Objects[SUBMARINE].GetCount(), m_Objects[MY_BOMB].GetCount(),life);
	m_VirtualDC.TextOut(10, 10, string);
	//  �������
	string.Format("Your score: %d", CScore::GetTotalScore());
	m_VirtualDC.TextOut(350, 10, string);

	nCreator--;
	nFrame++;  //ÿ֡����

			   // ���ڴ�DC������Ļ
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
	bn.SourceConstantAlpha = nFactor; //͸�������ã�0Ϊ��͸����255Ϊ��ȫ͸��

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Invalidate();
	CView::OnTimer(nIDEvent);
}


BOOL CSubMarineView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	//��ȡShip1,ship2��ָ��
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
	case 's'://��һ�������ӳ�һ�ų���ը��
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

	if (m_Objects[MYSHIP].GetCount() == 0) //���ȫ�����ս��
		return;

	//��ȡShip1,ship2��ָ��
	CMyShip* pShip1 = (CMyShip*)m_Objects[MYSHIP].GetHead();
 
	switch (nChar)
	{ // �����onTimerû��ִ��ǰ���ͷŰ������������ǰ�ƶ�
 
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
