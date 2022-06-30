#include "StdAfx.h"
#include "Board.h"
#include "resource.h"


int m_data[15][15];

//////////////////////////////////////////////////////////////////////////
// 构造函数，初始化棋盘数据以及图像数据
//////////////////////////////////////////////////////////////////////////
CBoard::CBoard()
{
    // 初始化图像列表
    m_iml.Create(BOARD_CELL, BOARD_CELL, ILC_COLOR24 | ILC_MASK, 0, 2 );
    // 载入黑、白棋子掩码位图
    CBitmap bmpBlack, bmpWhite;
    bmpBlack.LoadBitmap( IDB_BMP_BLACK );
    m_iml.Add( &bmpBlack, 0xff00ff );
    bmpWhite.LoadBitmap( IDB_BMP_WHITE );
    m_iml.Add( &bmpWhite, 0xff00ff );
}
//////////////////////////////////////////////////////////////////////////
// 析构函数
//////////////////////////////////////////////////////////////////////////
CBoard::~CBoard()
{

}

//////////////////////////////////////////////////////////////////////////
// 处理WM_PAINT消息
//////////////////////////////////////////////////////////////////////////
void CBoard::Draw(CDC* pDC)
{
    CDC MemDC;
    MemDC.CreateCompatibleDC( pDC );
    // 装载棋盘
    CBitmap bmp;
    bmp.LoadBitmap( IDB_BMP_BACKGROUND );

    MemDC.SelectObject( &bmp );

    // 根据棋盘数据绘制棋子
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
	// 完成绘制
    pDC->BitBlt( 0, 0, WIN_WIDTH, WIN_HEIGHT, &MemDC,0, 0, SRCCOPY );

	bmp.DeleteObject();
	MemDC.DeleteDC();
}

//////////////////////////////////////////////////////////////////////////
// 清空棋盘
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
// 设置棋盘数据，并绘制棋子
//////////////////////////////////////////////////////////////////////////
void CBoard::SetData( int x, int y, int color )
{
    m_data[x][y] = color;
}
//////////////////////////////////////////////////////////////////////////
// 在指定棋盘坐标处绘制指定颜色的棋子
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
    // TODO: 在此处添加实现代码.
    m_data[x][y] = -1 ;
}
