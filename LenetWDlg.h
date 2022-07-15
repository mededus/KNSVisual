// LenetWDlg.h : header file
//

#pragma once

#include "convol_layer.h"	//vrstvy siete
#include "sampl_layer.h"
#include "classic_layer.h"
#include "input_layer.h"
#include "read_file.h"		//komunikacia zo suborom
#include "zoomdlg.h"
#include "afxwin.h"
#include "afxcmn.h"

// CLenetWDlg dialog
class CLenetWDlg : public CDialog
{
// Construction
public:
	CLenetWDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LENETW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	input_layer		*p_INP;						//smerniky na vstup a vystup
	classic_layer	*p_OUT;				
	convol_layer	*p_C[_MAX_CONVOLUTIONS];	//smerniky na vnutorne vrstvy
	sampl_layer		*p_S[_MAX_SAMPLINGS];		//obmedzene poctom
	classic_layer	*p_F[_MAX_CLASSICS];

	read_file	*p_vstup;
	char *info;									//nesie info o type vrstiev
	
	int _CONVOLUTIONS;			//uchovavaju skutocny pocet vrstiev v sieti
	int _SAMPLINGS;				//pocet konvolucii a vzorkovani sa musi rovnat !!!
	int _CLASSICS;

private:
	int m_iPrevX;
	int m_iPrevY;
	CZoomDlg zoom;
public:
	CSliderCtrl m_slider;
	CButton m_epat_prev;
	CButton m_epat_next;
	CButton m_brun;
	
	long m_epat_max;
	long m_epat_no;
	int m_eresult;
	int position_X;
	int position_Y;
	int brush_width;
	COLORREF brush_color;
	
	void PrintInput(void);
	void PrintStructure(void);
	float squash_f(float value);
	int RbfCount(void);
	void FeedForward(void);
	void initialize_net(CString, bool);
	
	afx_msg void OnOpenNETButton();
	afx_msg void OnOpenPATButton();
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPatternBackButton();
	afx_msg void OnPatternForwardButton();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRUNButton();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaintBlack();
	afx_msg void OnPaintWhite();
	afx_msg void OnPaint2pixels();
	afx_msg void OnPaint4pixels();
	afx_msg void OnPaint6pixels();
	afx_msg void OnPaint8pixels();
	afx_msg void OnPaint10pixels();
	afx_msg void OnPaintClear();
	afx_msg void OnGoToButton();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CTreeCtrl m_treelist;
	float m_eerror;
	int GetOutput(void);
	float get_output_activation(void);
	void PrintFields(void);
	int m_edit_class;
	float m_eradius;
};
