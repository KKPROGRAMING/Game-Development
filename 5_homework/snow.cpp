#include "stdafx.h"
#include "Snow.h"

CSnow::CSnow() {

}

CSnow::~CSnow() {

}

void CSnow::createSnow(double x, double y, double vx, double vy, 
	double ax, double ay, double life, double decay, double radius) {
	s_x = x;
	s_y = y;
	s_vx = vx;
	s_vy = vy;
	s_ax = ax;
	s_ay = ay;
	s_life = life;
	s_decay = decay;
	s_radius = radius;
}

void CSnow::drawSnow(CDC* pDC, int img) {
	SNOW.Destroy();
	SNOW.LoadFromResource(AfxGetInstanceHandle(), img);
	SNOW.SetTransparentColor(RGB(255, 255, 255));

	SNOW.TransparentBlt(pDC->GetSafeHdc(), s_x, s_y, 50, 50,
		0, 0, 50, 50);
}

int CSnow::moveSnow(CRect win) {
	if (s_life > 0) {
		s_x += s_vx;
		s_y += s_vy;
		s_vx += s_ax;
		s_vy += s_ay;
		s_life -= s_decay;

		if (s_x >= win.Width() || s_y >= win.Height()) {
			s_x = rand() % 1500;
			s_y = 200 ;
			s_vx = rand() % 5 - 2.5;
			s_vy = rand() % 5 + 2;
			s_ax = rand() % 1;
			s_ay = rand() % 1 + 0.1;
			s_life = rand() % 10 + 20;
			s_radius = rand();
			s_decay = 0.01;
		}
	}
	return 1;
}