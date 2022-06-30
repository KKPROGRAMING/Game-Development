
// GobangView.cpp : CGobangView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Gobang.h"
#endif

#include "GobangDoc.h"
#include "GobangView.h"
#include "Connect.h"
#include "Rule.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGobangView

IMPLEMENT_DYNCREATE(CGobangView, CView)

BEGIN_MESSAGE_MAP(CGobangView, CView)
	ON_COMMAND(ID_MENU_DRAW_GAME, &CGobangView::OnMenuDrawGame)
	ON_COMMAND(ID_MENU_EXIT_GAME, &CGobangView::OnMenuExitGame)
	ON_COMMAND(ID_MENU_NEW_GAME, &CGobangView::OnMenuNewGame)
	ON_COMMAND(ID_MENU_RETRACT_GAME, &CGobangView::OnMenuRetractGame)
	ON_COMMAND(ID_MENU_SURRENDER_GAME, &CGobangView::OnMenuSurrenderGame)
	//ON_COMMAND(ID_MENU_CHANGEBK_GAME, &CGobangView::OnMenuChangeBKGame)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CGobangView ����/����

CGobangView::CGobangView()
{
	// TODO: �ڴ˴���ӹ������
	old_msg.x = -1;
	old_msg.y = -1;

}

CGobangView::~CGobangView()
{
}

BOOL CGobangView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CGobangView ���

#ifdef _DEBUG
void CGobangView::AssertValid() const
{
	CView::AssertValid();
}

void CGobangView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGobangDoc* CGobangView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGobangDoc)));
	return (CGobangDoc*)m_pDocument;
}
#endif //_DEBUG


// CGobangView ��Ϣ�������

//================ ����ΪView��ʵ�ֺ��� ========================//

// �˴�Ϊ��Ϸ���ƺ���
void CGobangView::OnDraw(CDC* pDC)
{
	CGobangDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���

	m_board.Draw(pDC);


}


void CGobangView::OnClose()
{
	MessageBox(_T("�Է��Ѿ��뿪��Ϸ�������ٽ������١�"), _T("������"), MB_ICONINFORMATION);
	SetMenuState(TRUE);

	SetWait(TRUE);

	m_connect.Close();
	m_sock.Close();

	m_bIsConnect = FALSE;
}

void CGobangView::OnAccept()
{
		//��������
	m_connect.Accept(m_sock);
	//�������ӳɹ���־
	m_bIsConnect = TRUE;
	//���õ�ǰ������ɫ
	SetColor(BLACK);
	m_board.Clear();

	// ���õȴ���־
	m_bWait = FALSE;

	//������ʾ�Ի���
	MessageBox(_T("���ӳɹ�,���Կ�ʼ��Ϸ����������."), _T("������"), MB_ICONINFORMATION);
}
//////////////////////////////////////////////////////////////////////////
// ���ò˵�״̬
//////////////////////////////////////////////////////////////////////////
void CGobangView::SetMenuState(BOOL bEnable)
{
	UINT uEnable, uDisable;
	if (bEnable)
	{
		uEnable = MF_ENABLED;
		uDisable = MF_GRAYED | MF_DISABLED;
	}
	else
	{
		uEnable = MF_GRAYED | MF_DISABLED;
		uDisable = MF_ENABLED;
	}

	CMenu* pMenu = AfxGetMainWnd()->GetMenu();

	pMenu->EnableMenuItem(ID_MENU_NEW_GAME, uEnable);
	pMenu->EnableMenuItem(ID_MENU_DRAW_GAME, uDisable);
}
void CGobangView::OnConnect()
{
	//�������ӳɹ���־
	m_bIsConnect = TRUE;
	//���õ�ǰ������ɫ
	SetColor(WHITE);
	m_board.Clear();
	m_bWait = TRUE;

	//������ʾ�Ի���
	MessageBox(_T("���ӳɹ�,���Կ�ʼ��Ϸ."), _T("������"), MB_ICONINFORMATION);

	SetMenuState(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// �������ԶԷ�������
//////////////////////////////////////////////////////////////////////////
void CGobangView::OnReceive()
{

	MSGSTRUCT msg;
	if (m_sock.Receive((LPVOID)&msg, sizeof(MSGSTRUCT)) == SOCKET_ERROR)
	{
		AfxGetMainWnd()->MessageBox(_T("��������ʱ�����������������������ӡ�"), _T("����"), MB_ICONSTOP);
		return;
	}
	switch (msg.msgType)
	{
	case MSG_PUTSTEP:
		m_board.SetData(msg.x, msg.y, msg.color);
		Over(msg.x, msg.y);
		break;
	case MSG_DRAW:
		if (IDYES == GetParent()->MessageBox(_T("�Է�������壬�������������"),
			_T("����"), MB_ICONQUESTION | MB_YESNO))
		{
			// �������������Ϣ
			MSGSTRUCT msg;
			msg.msgType = MSG_AGREE_DRAW;
			m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
			SetWait(TRUE);
			// ʹ�����桱�˵���Ч
			SetMenuState(TRUE);
		}
		else
		{
			// ���;ܾ�������Ϣ
			MSGSTRUCT msg;
			msg.msgType = MSG_REFUSE_DRAW;
			m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
		}
		break;
	case MSG_AGREE_DRAW:
		MessageBox(_T("�������������֣��Է����������ĺ�������"), _T("����"), MB_ICONINFORMATION);
		break;
	case MSG_REFUSE_DRAW:
		MessageBox(_T("�����Է���������ȡ��ʤ�������Ծܾ������ĺ�������"),
			_T("����"), MB_ICONINFORMATION);
		RestoreWait();
		break;

	case MSG_RETRACT:
		if (IDYES == GetParent()->MessageBox(_T("�Է�������壬�������������"),
			_T("����"), MB_ICONQUESTION | MB_YESNO))
		{
			m_bWait = 1;
			// �������������Ϣ
			MSGSTRUCT send_msg;

			send_msg.msgType = MSG_AGREE_RETRACT;
			m_sock.Send((LPCVOID)&send_msg, sizeof(MSGSTRUCT));
			SetWait(TRUE);

			//ʹ�ҷ������ϡ��Է�Ҫ�����������ʧ
			m_board.Retract(msg.x, msg.y);
			
		}
		else
		{
			m_bWait = 0;
			// ���;ܾ�������Ϣ
			MSGSTRUCT msg;
			msg.msgType = MSG_REFUSE_RETRACT;
			m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
		}
		break;
	case MSG_AGREE_RETRACT:
		MessageBox(_T("�����Է�����ʮ�㣬���������Ļ�������"), _T("����"), MB_ICONINFORMATION);
		m_board.Retract(old_msg.x,old_msg.y);
		m_bWait = 0;
		//RestoreWait();
		break;
	case MSG_REFUSE_RETRACT:
		MessageBox(_T("�����Է�����ѹ�������Ծܾ������Ļ�������"),
			_T("����"), MB_ICONINFORMATION);
		m_bWait = 1;
		break;

	case MSG_SURRENDER:
		MessageBox(_T("�����Է��ʰ��·磬ѡ�����������䡣"),
			_T("����"), MB_ICONINFORMATION);
		SetWait(TRUE);
		SetMenuState(TRUE);
		break;

	case MSG_EXTERN:
		break;
	default:
		break;
	}

	Invalidate();
}

void CGobangView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_connect.setParent(this);
	m_sock.setParent(this);
	m_board.Clear();
		
	m_bIsConnect = FALSE;

}
//////////////////////////////////////////////////////////////////////////
// �������
//////////////////////////////////////////////////////////////////////////
void CGobangView::DrawGame()
{
	// ���õȴ���־
	SetWait(TRUE);
	MSGSTRUCT msg;
	msg.msgType = MSG_DRAW;
	m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
}
//////////////////////////////////////////////////////////////////////////
// ����Է����Ӻ�Ĺ���
//////////////////////////////////////////////////////////////////////////
void CGobangView::Over(int x, int y)
{
	CRule rule;

	// �ж϶Է��Ƿ�ʤ��
	if (rule.Win(1 - m_color, x, y) == _WIN)
	{
		MessageBox(_T("�����ˣ�������Ҫ���ģ�ʧ���˳ɹ�֮ĸŶ��"), _T("ʧ��"), MB_ICONINFORMATION);
		// ����������ս������Ч�����桱
		if (m_bIsConnect)
		{
			SetMenuState(TRUE);
		}
		return;
	}

	m_bWait = FALSE;
}


//����
void CGobangView::OnMenuDrawGame()
{
	DrawGame();
}

//�˳���Ϸ
void CGobangView::OnMenuExitGame()
{
	PostQuitMessage(0);
}

//����Ϸ
void CGobangView::OnMenuNewGame()
{
	//��������Ϸ���á��Ի���ͨ��m_setup_dlg.DoModal()����ʾ�öԻ���
	//����ֵIDOK��IDCANCEL
	//����û������ȷ��
	if (IDOK == m_setup_dlg.DoModal())
	{
		Restart();
		NewGameStart(m_setup_dlg.m_isHost);
	}
}

void CGobangView::NewGameStart(BOOL isHost)
{
	if (isHost)
	{//��ǰѡ����Ƿ�����
		AfxGetMainWnd()->SetWindowText("������������...");
		m_connect.Create(m_setup_dlg.m_net_port);	//�����˿ڶ���
		m_connect.Listen();							//����
	}
	else
	{//��ǰѡ����ǿͻ���
		AfxGetMainWnd()->SetWindowText("�ͻ���������...");
		m_sock.Create();							//�����˿ڶ���
													//��������
		m_sock.Connect(m_setup_dlg.m_strHostIP, m_setup_dlg.m_net_port);
	}
}

//////////////////////////////////////////////////////////////////////////
// �������������Ϣ��Ϊ�������֮��
//////////////////////////////////////////////////////////////////////////
void CGobangView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

MSGSTRUCT msg;
	CRule  rule;

	BYTE buf[1024] = { 0 };

	if (m_bWait)
	{
		MessageBeep(MB_OK);
		return;
	}
	if (m_bIsConnect)
	{
		int x, y;
		x = int((point.x - BOARD_START_X)*1.0 / BOARD_GRID_W + 0.5);
		y = int((point.y - BOARD_START_Y)*1.0 / BOARD_GRID_W + 0.5);
		// �����(0, 0)��(14, 14)��Χ�ڣ��Ҹ�����û�����ӣ�
		// �������ڴˣ����������沢�˳�����
		if (x < 0 || x > 14 || y < 0 || y > 14 || m_data[x][y] != -1)
		{
			MessageBeep(MB_OK);
			return;
		}
		else
		{
			// ���λ�úϷ���������
			m_board.SetData(x, y, m_color);
			msg.color = m_color;
			msg.x = x;
			msg.y = y;

			//������һ�ε�������Ϣ
			old_msg.x = x;
			old_msg.y = y;
		}
		
		Invalidate();  //ˢ����Ļ

		// ��ʼ�ȴ�
		m_bWait = TRUE;
		msg.msgType = MSG_PUTSTEP;
		Send(&msg);

		if (rule.Win(m_color, msg.x, msg.y) == _WIN)
		{// ʤ��
			MessageBox(_T("��ϲ���������ʤ����"), _T("ʤ��"), MB_ICONINFORMATION);
			SetMenuState(TRUE);
		}

	}
}
//////////////////////////////////////////////////////////////////////////
// ���õȴ���־
//////////////////////////////////////////////////////////////////////////
void CGobangView::SetWait(BOOL bWait)
{
	m_bOldWait = m_bWait;
	m_bWait = bWait;
}

//////////////////////////////////////////////////////////////////////////
// ���������ɫ
//////////////////////////////////////////////////////////////////////////
void CGobangView::SetColor(int color)
{
	m_color = color;
}
//////////////////////////////////////////////////////////////////////////
// ��ȡ�����ɫ
//////////////////////////////////////////////////////////////////////////
int CGobangView::GetColor() const
{
	return m_color;
}

//////////////////////////////////////////////////////////////////////////
// ����������ǰ�ĵȴ���־
//////////////////////////////////////////////////////////////////////////
void CGobangView::RestoreWait()
{
	SetWait(m_bOldWait);
}

void CGobangView::Send(MSGSTRUCT * pmsg)
{
	m_sock.Send((LPVOID)pmsg, sizeof(MSGSTRUCT));
}

void CGobangView::Restart()
{
	m_board.Clear();
	m_connect.Close();
	m_sock.Close();
	Invalidate();
}

//�������
void CGobangView::OnMenuRetractGame()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	RetractGame();
}

//����
void CGobangView::RetractGame()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	//SetWait(TRUE);
	MSGSTRUCT msg;
	msg.x = old_msg.x;
	msg.y = old_msg.y;
	msg.msgType = MSG_RETRACT;
	m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
	MessageBox(_T("���ѷ�����壡�ȴ��Է�ͬ���С�"), _T("����"), MB_ICONINFORMATION);
}

//�������
void CGobangView::OnMenuSurrenderGame()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	MSGSTRUCT msg;
	msg.msgType = MSG_SURRENDER;
	m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
	MessageBox(_T("������Է����䣡��Ϸ������"), _T("����"), MB_ICONINFORMATION);
}
