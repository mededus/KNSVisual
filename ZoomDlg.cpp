// ZoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LenetW.h"
#include "ZoomDlg.h"


// CZoomDlg dialog

IMPLEMENT_DYNAMIC(CZoomDlg, CDialog)
CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDlg::IDD, pParent)
{
	
	
}

CZoomDlg::~CZoomDlg()
{
}

void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
END_MESSAGE_MAP()


// CZoomDlg message handlers

void CZoomDlg::set_position(int x, int y)
{
	SetWindowPos(NULL, x + 67, y + 220, 30*5 + 5, 30*5 + 5, NULL);
}

void CZoomDlg::zoom_print(COLORREF zoom_field[30][30])
{
	CClientDC dc(this);
	//5x zoom
	for (int i = 0 ; i < 30*5 ; i++) {
		for (int j = 0 ; j < 30*5 ; j++) {
			dc.SetPixelV(i,j,zoom_field[i/5][j/5]);
		}
	}
}
