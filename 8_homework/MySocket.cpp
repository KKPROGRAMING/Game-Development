// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Chatroom.h"
#include "MySocket.h"
#include "ChatroomDlg.h"

// CMySocket

CMySocket::CMySocket()
	: m_Dlg(NULL)
{
}

CMySocket::~CMySocket()
{
}


// CMySocket 成员函数


void CMySocket::OnClose(int nErrorCode)
{
	//nErrorCode == 0 表示成功
	if (nErrorCode == 0) m_Dlg->OnClose();
}


void CMySocket::OnAccept(int nErrorCode)
{
	if (nErrorCode == 0) m_Dlg->OnAccept();
}


void CMySocket::OnReceive(int nErrorCode)
{
	if (nErrorCode == 0) m_Dlg->OnReceived();
}


void CMySocket::SetParent(CChatroomDlg* pDlg)
{
	//给套接字类的对话框指针赋值
	m_Dlg = pDlg;
}
