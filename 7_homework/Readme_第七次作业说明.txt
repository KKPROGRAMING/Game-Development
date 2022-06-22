### Readme_第七次作业说明

##### 1.得分超过100分，切换背景

​	在函数OnDraw()中对当前得分m_score的大小进行判断，若小于100分，则保持原背景；否则切换新背景。方法是对背景bmp赋予不同的贴图。

```c++
if (m_score < 100)
		bmp.LoadBitmapA(IDB_BKGROUND);
	else
		bmp.LoadBitmapA(IDB_BG2);
```



##### 2.随着分数变高，柱子运动加速

​		添加了一个weight参数，作为分数对柱子运动速度产生影响的一个权重。为了让这个参数起作用，对pipe类的move()方法做了改变，需要传入一个weight参数。

```c++
void CPipe::move(double weight)
{
	double speed_now = speedx * ( 1 + weight);
	pos.x = pos.x - speedx * 1.0;
	if (pos.x < -size.cx)
	{
		pos.x = WIN_WIDTH;
		pos.y = rand() % 200 - 300;
		SPACE = rand() % 40 + 80;
	}
		
}
```

```c++
	m_score++;
	weight *= m_score * 0.05;
```

