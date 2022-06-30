//#include "stdafx.h"
//#include "Bplus.h"

// Missile.cpp: implementation of the CBomb class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bplus.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageList Bplus::m_Images;

Bplus::Bplus(int nVert, int nSpeed)
{
	m_ptPos.y = 50;
	m_ptPos.x = nVert;
	m_nSpeed = nSpeed;
}

Bplus::~Bplus()
{

}

bool Bplus::Draw(CDC* pDC, bool bPause)
{  // return false -  in screen
	//		 true  -  move out of screen
	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	if (bPause)
		return false;

	m_ptPos.y += m_nSpeed;	// move up
	if (m_ptPos.y > GAME_HEIGHT)
		return true;

	return false;
}

BOOL Bplus::LoadImage()
{

	CBitmap bmp_bombplus;
	bmp_bombplus.LoadBitmap(IDB_BOMBPLUS);

	// initialize CImageList
	if (!m_Images.Create(BOMB_WIDTH, BOMB_HEIGHT, ILC_COLOR24 | ILC_MASK, 1, 0))
		return FALSE;

	m_Images.Add(&bmp_bombplus, RGB(255, 0, 255));




	return TRUE;
}

void Bplus::DeleteImage()
{
	m_Images.DeleteImageList();
}
