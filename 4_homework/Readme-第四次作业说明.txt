#### 小球游戏 - 第四次作业改进说明

1.增设了**木板类（Block）**,详情见文件block.h以及block.cpp。

2.对已创建的**小球类（CBall）**和**砖块类（Brick）**进行了改进。

​	1）为小球类增设了检测是否碰撞到砖块的方法**check（）**，将其原来的检测是否碰撞到边缘或木板的方法**update（）**进行了区分。

```c++
void CBall::check(CRect brick) {
	m_x = m_x + m_vx;
	m_y = m_y + m_vy;

	POINT ball_left;
	ball_left.x = m_x;
	ball_left.y = m_y;

	POINT ball_right;
	ball_right.x = m_x + m_size.cx;
	ball_right.y = m_y;

	if (brick.PtInRect(ball_left)|| brick.PtInRect(ball_right)) {
	m_y += 5;
	m_vy = -m_vy;
	mark += 30;
}
```

​	2）为砖块类增设了一个改变其贴图的方法**change（）**，将小球的贴图换成透明。每当小球撞击到砖块时触发，以此达到视觉上“砖块消失”的效果。

```c++
void CBrick::change(int tmp) {
	img.Destroy();
	img.LoadFromResource(AfxGetInstanceHandle(), tmp);
}
```

​	其实我本来打算采用析构函数**~ CBrick（）**来达到让砖块消失的效果，但发现每次都会报错，猜测可能是受到了**OnDraw（）**的影响，所以采用了更换贴图的方式。

2.如何找到当前小球与砖块数组中的哪一个砖块发生了碰撞？我在这里采用了**哈希表**的思想，设计了一个简单的**“不用查找就能找到”**的找砖块算法。

```c++
	POINT brick;
	brick.x = int(floor(ball.getrect().top / BRICK_HEIGHT));
	brick.y = int(floor(ball.getrect().left / BRICK_WIDTH));//哈希表，节约遍历找出被撞击的砖块的时间
```

采用 **floor（）**向下取整 和 **ceil（）**向上取整 会获得不同的效果。前者是正确的，后者会让小球在初始位置不停“转圈”，无法移动。（关于这一现象我还没有做更多的分析...）

3.完善了加分机制和相关信息的显示（生命值与当前分数）；完善了游戏结束的界面。

4.改进了字体显示，使其为白色且背景透明。

```c++
	SetTextColor(*pDC,RGB(255,255,255));
	SetBkMode(*pDC, TRANSPARENT);
```

（其他：不创建木板类的写法以注释形式保存在文件中。）



最终效果如下图所示：

**（游戏中）**

![image-20220322201550859](C:\Users\bonjour\AppData\Roaming\Typora\typora-user-images\image-20220322201550859.png)

**（游戏结束）**

![image-20220322201619389](C:\Users\bonjour\AppData\Roaming\Typora\typora-user-images\image-20220322201619389.png)
