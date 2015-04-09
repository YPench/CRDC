// Consensus.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <vld.h>

#include "Consensus.h"
#include "Consensus_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CConsensusApp

BEGIN_MESSAGE_MAP(CConsensusApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CConsensusApp construction

CConsensusApp::CConsensusApp()
{

}


// The one and only CConsensusApp object

CConsensusApp theApp;


// CConsensusApp initialization

BOOL CConsensusApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	AfxInitRichEdit();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	//m_pSTeller = new STeller_Dlg;
	//m_pSTeller->Create(IDD_SELLER_DIALOG, NULL);
	//m_pSTeller->ShowWindow(SW_HIDE);

	CConsensus_Dlg dlg;

	//dlg.Create(IDD_CONSENSUS_DIALOG, NULL);
	//dlg.ShowWindow(SW_SHOW);

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

//#define WM_DISPLAY	WM_USER+100
//#define WM_MESSAGE	WM_USER+101
