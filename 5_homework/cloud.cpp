#include "stdafx.h"
#include "Cloud.h"

CCloud::CCloud() {

}

CCloud::~CCloud() {

}

void CCloud::createCloud() {
	c_y = rand()%20+10;
	c_vx = rand()%5+2;
	c_x = rand()%1200-400;
	c_ax = rand() % 1 + 0.01;
}

void CCloud::drawCloud(CDC* pDC, int img) {
	CLOUD.Destroy();
	CLOUD.LoadFromResource(AfxGetInstanceHandle(), img);
	CLOUD.SetTransparentColor(RGB(255, 0, 0));

	CLOUD.TransparentBlt(pDC->GetSafeHdc(), c_x, c_y, 560, 298,
		0, 0, 560, 298);
}

void CCloud::moveCloud(CRect win) {
	c_x += c_vx;
	c_vx += c_ax;

		if (c_x+100 >= win.Width()) {
			c_x = -500;
			c_vx = rand() % 5 + 2;
			c_ax = rand() % 1 + 0.01;
		}

}