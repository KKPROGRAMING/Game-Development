#pragma once
class CBoard
{
private:
	CImageList m_iml;  //用来管理绘制棋子图像

public:
	CBoard(void);
	~CBoard(void);

	void Clear();   //初始化棋盘
	void SetData(int x, int y , int color);  //color: 0 , 1 , -1
	void DrawXY(CDC* pDC, int x, int y, int color); 
	void Draw(CDC* pDC);

	void Retract(int x, int y);
};

