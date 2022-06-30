#include "StdAfx.h"
#include "Board.h"
#include "resource.h"


int m_data[15][15];

//////////////////////////////////////////////////////////////////////////
// ���캯������ʼ�����������Լ�ͼ������
//////////////////////////////////////////////////////////////////////////
CBoard::CBoard()
{
    // ��ʼ��ͼ���б�
    m_iml.Create(BOARD_CELL, BOARD_CELL, ILC_COLOR24 | ILC_MASK, 0, 2 );
    // ����ڡ�����������λͼ
    CBitmap bmpBlack, bmpWhite;
    bmpBlack.LoadBitmap( IDB_BMP_BLACK );
    m_iml.Add( &bmpBlack, 0xff00ff );
    bmpWhite.LoadBitmap( IDB_BMP_WHITE );
    m_iml.Add( &bmpWhite, 0xff00ff );
}
//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////
CBoard::~CBoard()
{

}

//////////////////////////////////////////////////////////////////////////
// ����WM_PAINT��Ϣ
//////////////////////////////////////////////////////////////////////////
void CBoard::Draw(CDC* pDC)
{
    CDC MemDC;
    MemDC.CreateCompatibleDC( pDC );
    // װ������
    CBitmap bmp;
    bmp.LoadBitmap( IDB_BMP_BACKGROUND );

    MemDC.SelectObject( &bmp );

    // �����������ݻ�������
    int x, y;
    POINT pt;
    for ( y = 0; y < 15; y++ )
    {
        for ( x = 0; x < 15; x++ )
        {
            if ( -1 != m_data[x][y] )
            {
                pt.x = BOARD_START_X + BOARD_GRID_W * x - BOARD_CELL / 2;
                pt.y = BOARD_START_Y + BOARD_GRID_W * y - BOARD_CELL / 2;
                m_iml.Draw( &MemDC, m_data[x][y], pt, ILD_TRANSPARENT );
            }
        }
    }
	// ��ɻ���
    pDC->BitBlt( 0, 0, WIN_WIDTH, WIN_HEIGHT, &MemDC,0, 0, SRCCOPY );

	bmp.DeleteObject();
	MemDC.DeleteDC();
}

//////////////////////////////////////////////////////////////////////////
// �������
//////////////////////////////////////////////////////////////////////////
void CBoard::Clear()
{
    int x, y;
    for ( y = 0; y < 15; y++ )
    {
        for ( x = 0; x < 15; x++ )
        {
            m_data[x][y] = -1;
        }
    }

}


//////////////////////////////////////////////////////////////////////////
// �����������ݣ�����������
//////////////////////////////////////////////////////////////////////////
void CBoard::SetData( int x, int y, int color )
{
    m_data[x][y] = color;
}
//////////////////////////////////////////////////////////////////////////
// ��ָ���������괦����ָ����ɫ������
//////////////////////////////////////////////////////////////////////////
void CBoard::DrawXY(CDC* pDC, int x, int y, int color)
{
    POINT pt;
    pt.x = BOARD_START_X + BOARD_GRID_W * x;
    pt.y = BOARD_START_Y + BOARD_GRID_W * y;

    m_iml.Draw( pDC, color, pt, ILD_TRANSPARENT );

}




void CBoard::Retract(int x, int y)
{
    // TODO: �ڴ˴����ʵ�ִ���.
    m_data[x][y] = -1 ;
}
