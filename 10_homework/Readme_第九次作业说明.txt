## Readme_第九次作业说明

相较于原程序，我主要添加了以下功能，并做了相关的改动：

##### 1.添加了悔棋功能

​	在对局过程中，若一方想要悔棋，则会给对方发送悔棋消息MSG_RETRACT。若对方同意，则会向悔棋发起方传回同意悔棋消息MSG_AGREE_RETRACT，同意方首先出现对方悔棋的效果（即撤销悔棋发起方上一次的落子），悔棋发起方在确认同意方传回的同意悔棋消息后才会出现本方悔棋的效果；若不同意，则传回拒绝悔棋消息MSG_REFUSE_RETRACT。

​	同时，我定义了一个MSGSTRUCT类型的变量old_msg来存放落子方上一次的落子坐标。当任意一方需要用到悔棋功能时，便可根据old_msg变量中的x值、y值来选择“撤回”对应的棋子。

```c++
//来自于文件GoBangView.h
//保存上一次的落子信息
	MSGSTRUCT old_msg;
```

```c++
//来自于文件GoBangView.cpp
//该函数用于处理落子
void CGobangView::OnLButtonUp(UINT nFlags, CPoint point)
{
    ......
			// 如果位置合法，则落子
			m_board.SetData(x, y, m_color);
			msg.color = m_color;
			msg.x = x;
			msg.y = y;

			//保存这一次的落子信息
			old_msg.x = x;
			old_msg.y = y;
    ......
}
```

悔棋发起方向对方传送悔棋消息MSG_RETRACT时，也需要将old_msg中保存的x值、y值放入向对方发送的MSGSTRUCT类型参数内。如果对方同意悔棋，则可根据这一坐标信息进行棋盘上棋子的“撤回”。

​	另外，我对Board类进行了改动，添加一个悔棋方法Retract(int x, int y)，根据其中传入的横纵坐标参数对对应的位置进行棋子的“撤回”。

```c++
void CBoard::Retract(int x, int y)
{
    // TODO: 在此处添加实现代码.
    m_data[x][y] = -1 ;
}
```

​	需要注意的是，要避免因为悔棋而打乱了原有的落子顺序。这里通过更改变量m_bWait的值来实现，详情见文件GoBangView.cpp。

​	

##### 2.添加了认输功能

​	认输功能与原有的和棋功能类似，只需对弹出消息框中的文字进行相应改动即可。



​	具体效果与演示见文件 五子棋动画.mp4 。
