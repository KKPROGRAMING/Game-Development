## Read me 第十次作业说明

#### 作业目标：实现坦克寻路AI

##### 1.依据Tracking算法

​	我的实现方法是依据Tracking算法的思想对敌机坦克的寻路方向进行选择，从而达到自动追踪玩家位置的目的。但是由于地图只允许上下左右四个方向移动、而不允许斜对角线与其他方向移动的限制，所以在寻路效率上仍有一些缺陷。

```c++
//寻找目标玩家
	void FindUser(Direction Forbidden_d);//有参数
	void FindUser_n();//无参数
```

```c++
//寻找玩家位置
void CEnemyTank::FindUser(Direction Forbidden_d) {

	if (f_user_p.y == m_ptPos.y && f_user_p.x != m_ptPos.x) {
		if (f_user_p.x > m_ptPos.x)
			m_Dir = DIR_LEFT;
		else
			m_Dir = DIR_RIGHT;
		return;
	}

	int random = rand()%2;
	double k = (1.0*(f_user_p.x-m_ptPos.x)) / (1.0*(f_user_p.y - m_ptPos.y));
	if (k < 0) {
		if(f_user_p.x > m_ptPos.x)
			if(random)
				m_Dir = DIR_RIGHT;
			else
				m_Dir = DIR_UP;
		else
			if(random)
				m_Dir = DIR_LEFT;
			else
				m_Dir = DIR_DOWN;
	}
	else if (k > 0) {
		if (f_user_p.x > m_ptPos.x)
			if(random)
				m_Dir = DIR_RIGHT;
			else
				m_Dir = DIR_DOWN;
		else
			if(random)
				m_Dir = DIR_LEFT;
			else
				m_Dir = DIR_UP;
	}
	else {
		if (f_user_p.y > m_ptPos.y)
			m_Dir = DIR_DOWN;
		else
			m_Dir = DIR_UP;
		return;
	}

	switch (m_Dir)
	{
	case DIR_UP: {
		if (Forbidden_d == DIR_UP)
			break;
		else {
			m_Speed.x = 0;
			m_Speed.y = -1 * (rand() % 6 + 3);
			break;
		}
	}
	case DIR_DOWN: {
		if (Forbidden_d == DIR_DOWN)
			break;
		else {
			m_Speed.x = 0;
			m_Speed.y = 1 * (rand() % 6 + 3);
			break;
		}
	}
	case DIR_LEFT: {
		if (Forbidden_d == DIR_LEFT)
			break;
		else {
			m_Speed.x = -1 * (rand() % 6 + 3);
			m_Speed.y = 0;
			break;
		}
	}
	case DIR_RIGHT: {
		if (Forbidden_d == DIR_UP)
			break;
		else {
			m_Speed.x = 1 * (rand() % 6 + 3);
			m_Speed.y = 0;
			break;
		}
	}
	}
}
```

##### 2.避免过低的寻路效率

​	实现过程多次采用了产生随机数的rand()函数，用来达到在合适频率内更新寻路方向的目的。由于玩家坦克可能随时改变自身位置，如果敌机坦克每次移动前都要重新选择一次方向，就会降低接近玩家坦克位置的效率。所以我在多次尝试后，将敌机坦克重新选择方向的概率设置为较为合适的50%。

```c++
if ((rand() % 2) == 1)
		FindUser_n();
	//让敌机以1/2的概率重新定位玩家位置
```

##### 3.“此路不通”后的做法

​	由于地图中存在路障，所以会产生“此路不通”的情况，即寻路后所选择的方向无法让坦克通行。我设置了一个一维数组，用于保存无法通行方向的数量情况.

```c++
int cnt[4];//用于收集无法通行的方向数量
```

```c++
//对这一标志数组再次初始化
			cnt[0] = -1;
			cnt[1] = -1;
			cnt[2] = -1;
			cnt[3] = -1;
```

​	当无法通行的方向数量大于等于2时，坦克便会通过 SetRandomDirections() 方法产生一个随机方向，而不再根据寻路算法提供的方向行动。

​	如果无法通行的方向数量等于1，坦克会保存这个方向，并在下一次行动时避免这一方向，但是依旧根据寻路算法提供的方向移动。

```c++
int many = 0 ;//用于统计无法通行的方向数量，若大于等于2，则采用随机方向方法；否则继续另一个方向寻路
		for (int i = 0; i < 4; i++)
			if (cnt[i] != -1)
				many += 1;

		if (many < 2) {
			//排除无法通行的方向
			Direction forbidden = m_Dir;

			//不再随机寻找方向，而是以玩家坦克为目标
			FindUser(forbidden);
			cnt[forbidden] = 1;
		}
		else {
			//随机方向
			SetRandomDirections();
			//对这一标志数组再次初始化
			cnt[0] = -1;
			cnt[1] = -1;
			cnt[2] = -1;
			cnt[3] = -1;
		}
```

​	以上便是我对原代码的关键改动内容，具体效果见附件“坦克寻路动画.mp4”，改动后代码见各附件。
