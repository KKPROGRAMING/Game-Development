#pragma once

//#include "ChatroomDlg.h"

// CMySocket 命令目标
class CChatroomDlg;

class CMySocket : public CAsyncSocket
{
public:
	CMySocket();
	virtual ~CMySocket();
	virtual void OnClose(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
private:
	CChatroomDlg* m_Dlg;//this is a pointer!
public:
	void SetParent(CChatroomDlg* pDlg);
};


