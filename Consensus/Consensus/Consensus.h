// Consensus.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "Consensus_Dlg.h"

// CConsensusApp:
// See Consensus.cpp for the implementation of this class
//

class CConsensusApp : public CWinApp
{
public:
	CConsensusApp();
// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

public:
	CConsensus_Dlg* pdlg;
	//STeller_Dlg* m_pSTeller;
	

	string teststr;

};

extern CConsensusApp theApp;