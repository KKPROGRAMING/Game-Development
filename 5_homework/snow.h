#pragma once
#include "stdafx.h"
#include "resource.h"       // ������

class CSnow {
public :
	CSnow();
	~CSnow();

public:
	double s_x, s_y;//λ��
	double s_vx, s_vy;//�ٶ�
	double s_ax, s_ay;//���ٶ�
	double s_life;//����
	double s_radius;//�뾶
	double s_decay;//��������ֵ

	void createSnow(double x, double y, double vx, double vy, 
		double ax, double ay, double life, double decay, double radius);
	void drawSnow(CDC* pDC, int img);
	int moveSnow(CRect win);



	CImage SNOW;//ѩ����ͼ
};