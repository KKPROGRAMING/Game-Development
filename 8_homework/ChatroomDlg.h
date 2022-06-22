
// ChatroomDlg.h : 头文件
//

#pragma once

#include "MySocket.h"

// CChatroomDlg 对话框
class CChatroomDlg : public CDialogEx
{
private:

	CMySocket m_sListenSocket;
	CMySocket m_sConnectSocket;

	CAsyncSocket m_cConnectSocket;//from client

// 构造
public:
	CChatroomDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHATROOM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnClose;//close the dlgframe
	CButton m_btnListen;//listen the command
	CButton m_btnSend;//send msgs,OK

	CString m_strMsg;
	CString m_strClientName;
//	CString m_strServPort;
	CListBox m_listReceived;
	CListBox m_ListSent;
	//afx_msg void OnBnClickedButtonAccept();
	afx_msg void OnBnClickedButtonListen();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonSend();
	void OnClose(void);
	void OnAccept(void);
	void OnReceived(void);
	int m_nClientPort;
};
