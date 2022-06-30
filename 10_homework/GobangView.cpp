
// GobangView.cpp : CGobangView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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

// CGobangView 构造/析构

CGobangView::CGobangView()
{
	// TODO: 在此处添加构造代码
	old_msg.x = -1;
	old_msg.y = -1;

}

CGobangView::~CGobangView()
{
}

BOOL CGobangView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGobangView 诊断

#ifdef _DEBUG
void CGobangView::AssertValid() const
{
	CView::AssertValid();
}

void CGobangView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGobangDoc* CGobangView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGobangDoc)));
	return (CGobangDoc*)m_pDocument;
}
#endif //_DEBUG


// CGobangView 消息处理程序

//================ 以下为View类实现函数 ========================//

// 此处为游戏绘制函数
void CGobangView::OnDraw(CDC* pDC)
{
	CGobangDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	m_board.Draw(pDC);


}


void CGobangView::OnClose()
{
	MessageBox(_T("对方已经离开游戏，改日再较量不迟。"), _T("五子棋"), MB_ICONINFORMATION);
	SetMenuState(TRUE);

	SetWait(TRUE);

	m_connect.Close();
	m_sock.Close();

	m_bIsConnect = FALSE;
}

void CGobangView::OnAccept()
{
		//接受连接
	m_connect.Accept(m_sock);
	//设置连接成功标志
	m_bIsConnect = TRUE;
	//设置当前棋子颜色
	SetColor(BLACK);
	m_board.Clear();

	// 设置等待标志
	m_bWait = FALSE;

	//弹出提示对话框
	MessageBox(_T("连接成功,可以开始游戏，请你先行."), _T("五子棋"), MB_ICONINFORMATION);
}
//////////////////////////////////////////////////////////////////////////
// 设置菜单状态
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
	//设置连接成功标志
	m_bIsConnect = TRUE;
	//设置当前棋子颜色
	SetColor(WHITE);
	m_board.Clear();
	m_bWait = TRUE;

	//弹出提示对话框
	MessageBox(_T("连接成功,可以开始游戏."), _T("五子棋"), MB_ICONINFORMATION);

	SetMenuState(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 接收来自对方的数据
//////////////////////////////////////////////////////////////////////////
void CGobangView::OnReceive()
{

	MSGSTRUCT msg;
	if (m_sock.Receive((LPVOID)&msg, sizeof(MSGSTRUCT)) == SOCKET_ERROR)
	{
		AfxGetMainWnd()->MessageBox(_T("接收数据时发生错误，请检查您的网络连接。"), _T("错误"), MB_ICONSTOP);
		return;
	}
	switch (msg.msgType)
	{
	case MSG_PUTSTEP:
		m_board.SetData(msg.x, msg.y, msg.color);
		Over(msg.x, msg.y);
		break;
	case MSG_DRAW:
		if (IDYES == GetParent()->MessageBox(_T("对方请求和棋，接受这个请求吗？"),
			_T("和棋"), MB_ICONQUESTION | MB_YESNO))
		{
			// 发送允许和棋消息
			MSGSTRUCT msg;
			msg.msgType = MSG_AGREE_DRAW;
			m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
			SetWait(TRUE);
			// 使“重玩”菜单生效
			SetMenuState(TRUE);
		}
		else
		{
			// 发送拒绝和棋消息
			MSGSTRUCT msg;
			msg.msgType = MSG_REFUSE_DRAW;
			m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
		}
		break;
	case MSG_AGREE_DRAW:
		MessageBox(_T("看来真是棋逢对手，对方接受了您的和棋请求。"), _T("和棋"), MB_ICONINFORMATION);
		break;
	case MSG_REFUSE_DRAW:
		MessageBox(_T("看来对方很有信心取得胜利，所以拒绝了您的和棋请求。"),
			_T("和棋"), MB_ICONINFORMATION);
		RestoreWait();
		break;

	case MSG_RETRACT:
		if (IDYES == GetParent()->MessageBox(_T("对方请求悔棋，接受这个请求吗？"),
			_T("悔棋"), MB_ICONQUESTION | MB_YESNO))
		{
			m_bWait = 1;
			// 发送允许悔棋消息
			MSGSTRUCT send_msg;

			send_msg.msgType = MSG_AGREE_RETRACT;
			m_sock.Send((LPCVOID)&send_msg, sizeof(MSGSTRUCT));
			SetWait(TRUE);

			//使我方棋盘上、对方要悔棋的棋子消失
			m_board.Retract(msg.x, msg.y);
			
		}
		else
		{
			m_bWait = 0;
			// 发送拒绝悔棋消息
			MSGSTRUCT msg;
			msg.msgType = MSG_REFUSE_RETRACT;
			m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
		}
		break;
	case MSG_AGREE_RETRACT:
		MessageBox(_T("看来对方信心十足，接受了您的悔棋请求。"), _T("悔棋"), MB_ICONINFORMATION);
		m_board.Retract(old_msg.x,old_msg.y);
		m_bWait = 0;
		//RestoreWait();
		break;
	case MSG_REFUSE_RETRACT:
		MessageBox(_T("看来对方倍感压力，所以拒绝了您的悔棋请求。"),
			_T("悔棋"), MB_ICONINFORMATION);
		m_bWait = 1;
		break;

	case MSG_SURRENDER:
		MessageBox(_T("看来对方甘拜下风，选择了向您认输。"),
			_T("认输"), MB_ICONINFORMATION);
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
// 和棋操作
//////////////////////////////////////////////////////////////////////////
void CGobangView::DrawGame()
{
	// 设置等待标志
	SetWait(TRUE);
	MSGSTRUCT msg;
	msg.msgType = MSG_DRAW;
	m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
}
//////////////////////////////////////////////////////////////////////////
// 处理对方落子后的工作
//////////////////////////////////////////////////////////////////////////
void CGobangView::Over(int x, int y)
{
	CRule rule;

	// 判断对方是否胜利
	if (rule.Win(1 - m_color, x, y) == _WIN)
	{
		MessageBox(_T("您输了，不过不要灰心，失败乃成功之母哦！"), _T("失败"), MB_ICONINFORMATION);
		// 如果是网络对战，则生效“重玩”
		if (m_bIsConnect)
		{
			SetMenuState(TRUE);
		}
		return;
	}

	m_bWait = FALSE;
}


//和棋
void CGobangView::OnMenuDrawGame()
{
	DrawGame();
}

//退出游戏
void CGobangView::OnMenuExitGame()
{
	PostQuitMessage(0);
}

//新游戏
void CGobangView::OnMenuNewGame()
{
	//弹出“游戏设置”对话框，通过m_setup_dlg.DoModal()来显示该对话框
	//返回值IDOK，IDCANCEL
	//如果用户点击了确定
	if (IDOK == m_setup_dlg.DoModal())
	{
		Restart();
		NewGameStart(m_setup_dlg.m_isHost);
	}
}

void CGobangView::NewGameStart(BOOL isHost)
{
	if (isHost)
	{//当前选择的是服务器
		AfxGetMainWnd()->SetWindowText("服务器运行中...");
		m_connect.Create(m_setup_dlg.m_net_port);	//建立端口对象
		m_connect.Listen();							//监听
	}
	else
	{//当前选择的是客户机
		AfxGetMainWnd()->SetWindowText("客户机运行中...");
		m_sock.Create();							//建立端口对象
													//建立连接
		m_sock.Connect(m_setup_dlg.m_strHostIP, m_setup_dlg.m_net_port);
	}
}

//////////////////////////////////////////////////////////////////////////
// 处理左键弹起消息，为玩家落子之用
//////////////////////////////////////////////////////////////////////////
void CGobangView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
		// 如果在(0, 0)～(14, 14)范围内，且该坐标没有落子，
		// 则落子于此，否则发声警告并退出过程
		if (x < 0 || x > 14 || y < 0 || y > 14 || m_data[x][y] != -1)
		{
			MessageBeep(MB_OK);
			return;
		}
		else
		{
			// 如果位置合法，则落子
			m_board.SetData(x, y, m_color);
			msg.color = m_color;
			msg.x = x;
			msg.y = y;

			//保存这一次的落子信息
			old_msg.x = x;
			old_msg.y = y;
		}
		
		Invalidate();  //刷新屏幕

		// 开始等待
		m_bWait = TRUE;
		msg.msgType = MSG_PUTSTEP;
		Send(&msg);

		if (rule.Win(m_color, msg.x, msg.y) == _WIN)
		{// 胜利
			MessageBox(_T("恭喜，您获得了胜利！"), _T("胜利"), MB_ICONINFORMATION);
			SetMenuState(TRUE);
		}

	}
}
//////////////////////////////////////////////////////////////////////////
// 设置等待标志
//////////////////////////////////////////////////////////////////////////
void CGobangView::SetWait(BOOL bWait)
{
	m_bOldWait = m_bWait;
	m_bWait = bWait;
}

//////////////////////////////////////////////////////////////////////////
// 设置玩家颜色
//////////////////////////////////////////////////////////////////////////
void CGobangView::SetColor(int color)
{
	m_color = color;
}
//////////////////////////////////////////////////////////////////////////
// 获取玩家颜色
//////////////////////////////////////////////////////////////////////////
int CGobangView::GetColor() const
{
	return m_color;
}

//////////////////////////////////////////////////////////////////////////
// 重新设置先前的等待标志
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

//悔棋操作
void CGobangView::OnMenuRetractGame()
{
	// TODO: 在此处添加实现代码.
	RetractGame();
}

//悔棋
void CGobangView::RetractGame()
{
	// TODO: 在此处添加实现代码.
	//SetWait(TRUE);
	MSGSTRUCT msg;
	msg.x = old_msg.x;
	msg.y = old_msg.y;
	msg.msgType = MSG_RETRACT;
	m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
	MessageBox(_T("您已发起悔棋！等待对方同意中。"), _T("悔棋"), MB_ICONINFORMATION);
}

//认输操作
void CGobangView::OnMenuSurrenderGame()
{
	// TODO: 在此处添加实现代码.
	MSGSTRUCT msg;
	msg.msgType = MSG_SURRENDER;
	m_sock.Send((LPCVOID)&msg, sizeof(MSGSTRUCT));
	MessageBox(_T("您已向对方认输！游戏结束。"), _T("认输"), MB_ICONINFORMATION);
}
