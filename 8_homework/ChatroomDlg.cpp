
// ChatroomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Chatroom.h"
#include "ChatroomDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatroomDlg 对话框




CChatroomDlg::CChatroomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatroomDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strMsg = _T("");
	m_strClientName = _T("");
	//  m_strServPort = _T("");
	m_nClientPort = 0;
}

void CChatroomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BUTTON_LISTEN, m_btnListen);
	DDX_Control(pDX, IDC_BUTTON_LISTEN, m_btnListen);
	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);
	DDX_Text(pDX, IDC_EDIT_SERVNAME, m_strClientName);
	//  DDX_Text(pDX, IDC_EDIT_SERVPORT, m_strServPort);
	DDX_Control(pDX, IDC_LIST_RECEIVED, m_listReceived);
	DDX_Control(pDX, IDC_LIST_SENT, m_ListSent);
	DDX_Text(pDX, IDC_EDIT_SERVPORT, m_nClientPort);
}

BEGIN_MESSAGE_MAP(CChatroomDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LISTEN, &CChatroomDlg::OnBnClickedButtonListen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CChatroomDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatroomDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CChatroomDlg 消息处理程序

BOOL CChatroomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	m_strClientName = "localhost";  //表示网络中的本机
	m_nClientPort= 1000;
	UpdateData(FALSE); //把前面的赋值更新到界面

	m_sListenSocket.SetParent(this); // this 指向自己，代表对话框，传到CMySocket类


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatroomDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChatroomDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChatroomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatroomDlg::OnBnClickedButtonListen()
{ 
	UpdateData(TRUE);
	
	GetDlgItem(IDC_BUTTON_LISTEN)->EnableWindow(FALSE);
	
	m_sListenSocket.Create();
	if (m_sListenSocket.Accept(m_cConnectSocket, NULL, NULL)) {
		//如果监听到连接的请求，则创建一个连接的套接字
		m_sConnectSocket.Create();
		m_sConnectSocket.Connect(m_strClientName, m_nClientPort);
		m_sConnectSocket.OnAccept(0);
	}
}


void CChatroomDlg::OnBnClickedButtonClose()
{
	m_sListenSocket.Close();
	m_sConnectSocket.Close();
	GetDlgItem(IDC_BUTTON_LISTEN)->EnableWindow(TRUE);
}


void CChatroomDlg::OnBnClickedButtonSend()
{
	UpdateData(TRUE);
	if (!m_strMsg.IsEmpty())
	{
		int nLen = m_strMsg.GetLength();
		int nSent = m_sConnectSocket.Send(LPCTSTR(m_strMsg),nLen);
	}
	m_strMsg.Empty();
	UpdateData(FALSE);
}


void CChatroomDlg::OnClose(void)
{
	//提示
	//提示
	m_listReceived.AddString("连接断开");
	UpdateData(FALSE);
}


void CChatroomDlg::OnAccept(void)
{
	//提示
	m_listReceived.AddString("连接成功");
	UpdateData(FALSE);
}


void CChatroomDlg::OnReceived(void)
{
	char buf[1025] = { 0 };
	int nBufSize = 1024;

	int nReceived;
	CString strReceived;
	nReceived = m_sConnectSocket.Receive(buf,nBufSize);
	if (nReceived != SOCKET_ERROR)
	{
		m_listReceived.AddString(buf);
		UpdateData(FALSE);
	}


}
