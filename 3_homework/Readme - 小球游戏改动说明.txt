#### 小球游戏 - 改进说明

1.在头文件中，对小球直径（**diameter**）、木板宽度（**block_width**)、木板厚度（**block_height**)等变量都作了统一的声名与赋值，方便对其数值进行修改。

2.对**SetTimer()**中所传参数的值进行了调整，缩短图像重绘的时间间隔，让物体的移动在视觉上更加流畅。

3.设置**加分机制**，当小球碰撞到木板成功反弹时可获得加分。

4.设置**木板缩短机制**，当小球碰撞到木板成功反弹时木板会缩短一定宽度。

4.增加**小球变速机制**，当小球碰到木板并反弹时，运动速度增加。

6.重绘前增加对生命值的判断：当其减一时，游戏恢复至初始状态；当其减至零时，**提示游戏结束**。

7.对游戏窗口的相关操作进行了限制，**防止用户改变窗口大小**和最小化。

<!--代码贴在下方，就不传MainFrm.cpp文件了-->

```c++
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_MAXIMIZEBOX; //禁止窗口最大化
	cs.style &= ~WS_MINIMIZEBOX; //禁止窗口最小化
	// cs.style &= ~WS_SYSMENU; //取消Title上的按钮
	cs.style &= ~WS_THICKFRAME;//使窗口不能用鼠标改变大小
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}
```

