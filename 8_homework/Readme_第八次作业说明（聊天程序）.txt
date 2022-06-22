## Readme_第八次作业说明（聊天程序）

​	由于代码是基于客户端的源代码更改的，所以思路主要在于将客户端与服务端之间的不同体现出来。以下是我的更改内容：

##### 1.设置不同的套接字变量

​	我在服务器端的对话框类中设置了三个不同的套接字变量。

​	前两个的类型为自己定义的MySocket套接字类，分别为Listen(监听)套接字以及Connect(连接)套接字，另外一个则为CAsyncSocket类的变量，用来接收客户端传来的连接套接字。因为我需要使用以下的函数来监听和建立连接，其中需要传入一个CAsyncSocket&类的参数：

```c
virtual BOOL Accept(CAsyncSocket& rConnectedSocket,
		SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);
```

##### 2.更改按钮文字及其点击后效果

​	我将源代码中的“连接”按钮更改为了“监听”按钮。

​	而连接时传入参数的参数名也由原来客户端的m_strServName、m_nServPort更改为了现在服务端需要获取和传入的m_strClientName, m_nClientPort。而以下代码则用于“监听”按钮被点击后的情况：

```c
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
```

##### 3.调整界面布局及更改变量命名

​	这些调整都是根据从客户端到服务端的转换而进行的，不做过多赘述。最后界面效果见图片文件。
