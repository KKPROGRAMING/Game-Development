#pragma once
#include "stdafx.h"
#include "resource.h"       // ������

class CCloud {
public:
	CCloud();
	~CCloud();

public:
	double c_x, c_y;//λ��
	double c_vx;//�ٶ�
	double c_ax;

	void createCloud();
	void drawCloud(CDC* pDC, int img);
	void moveCloud(CRect win);

	CImage CLOUD;//ѩ����ͼ
}; 
