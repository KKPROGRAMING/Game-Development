### Readme _ 第六次作业说明

#### 1.“TANK”地图效果实现

​	由于地图编辑画出来的图像没有办法保存出txt文件，所以采用了手打坐标的方式，定位出“TANK”砖块的位置。

​	判断该位置是放入草地图像还是砖块图像的方式是**哈希表**。

```c++
	for (int row = 0; row < MAX_BLOCK_NUM; row++)
		for (int col = 0; col < MAX_BLOCK_NUM; col++)
		{
			if (m_map[row][col] != -1)
				m_map[row][col] = IDB_BITMAP1;
			else
				m_map[row][col] = IDB_BITMAP4;

			bmp.LoadBitmap(m_map[row][col]);
			memDC.SelectObject(&bmp);

			int y = col * BLOCK_SIZE;
			int x = row * BLOCK_SIZE;
			pDC->BitBlt(x, y, BLOCK_SIZE, BLOCK_SIZE,
				&memDC, 0, 0, SRCCOPY);

			bmp.DeleteObject();

		}
```

#### 2. AlphaBlend 实现

​	我对源代码做出了以下的改动：

**1）OnDraw绘图**

​	由于 AlphaBlend() 函数中需要输入 hdcDest 参数，所以将原来采用的 OnPaint() 绘图方式改变为OnDraw().

**2)透明度自动改变**

​	在 OnTime() 函数中让对应调节图像透明度的 blendFunction.SourceConstantAlpha 参数自动递增.

