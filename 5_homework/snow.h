#pragma once
#include "stdafx.h"
#include "resource.h"       // 主符号

class CSnow {
public :
	CSnow();
	~CSnow();

public:
	double s_x, s_y;//位置
	double s_vx, s_vy;//速度
	double s_ax, s_ay;//加速度
	double s_life;//生命
	double s_radius;//半径
	double s_decay;//生命消耗值

	void createSnow(double x, double y, double vx, double vy, 
		double ax, double ay, double life, double decay, double radius);
	void drawSnow(CDC* pDC, int img);
	int moveSnow(CRect win);



	CImage SNOW;//雪花贴图
};