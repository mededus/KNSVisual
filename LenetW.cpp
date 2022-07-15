// LenetW.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LenetW.h"
#include "LenetWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLenetWApp

BEGIN_MESSAGE_MAP(CLenetWApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLenetWApp construction

CLenetWApp::CLenetWApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CLenetWApp object

CLenetWApp theApp;


// CLenetWApp initialization

BOOL CLenetWApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();


	CLenetWDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
