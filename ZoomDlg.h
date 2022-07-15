#pragma once


// CZoomDlg dialog

class CZoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CZoomDlg)

public:
	CZoomDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CZoomDlg();

// Dialog Data
	enum { IDD = IDD_ZOOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void zoom_print(COLORREF [30][30]);
	void set_position(int, int);
};
