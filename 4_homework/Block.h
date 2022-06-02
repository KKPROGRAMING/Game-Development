#pragma once
class Block
{
public:
	Block(void);
	~Block(void);

	void create(int img, int x , int y , int width , int height , int v);
	CRect getrect();

	void move(CRect win, char ch);
	void draw(CDC* pDC);

	int b_x;
	int b_y;
	int b_width;
	int b_height;
	int b_v;
	//int b_img;
	CImage b_img;
};

