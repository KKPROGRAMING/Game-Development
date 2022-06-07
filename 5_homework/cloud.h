#pragma once
#include "stdafx.h"
#include "resource.h"       // 主符号

class CCloud {
public:
	CCloud();
	~CCloud();

public:
	double c_x, c_y;//位置
	double c_vx;//速度
	double c_ax;

	void createCloud();
	void drawCloud(CDC* pDC, int img);
	void moveCloud(CRect win);

	CImage CLOUD;//雪花贴图
}; 
