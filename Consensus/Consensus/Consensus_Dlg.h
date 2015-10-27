/*
 * Copyright (C) 2015 by Yanping Chen <ypench@gmail.com>
 * Begin       : 01-Oct-2015
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser GPL (LGPL) as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.
 */

// Consensus_Dlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "Config.h"
//#include "..\\Include\\CGCom.h"
#include "SXMLer.h"

#include "TabSheet.h"
#include "STeller_Dlg.h"

#ifdef CONSENSUS_CEDT
#include "CEDT\\CEDT_Dlg.h"
#endif

#ifdef CONSENSUS_CRDC
#include "CRDC\\CRDC_Dlg.h"
#endif

#ifdef CONSENSUS_ECOR
#include "ECOR\\ECOR_Dlg.h"
#endif

#ifdef CONSENSUS_CDOC
#include "CDOC\\DOC_Dlg.h"
#endif

#ifdef CONSENSUS_ENET
#include "ENET\\ENET_Dlg.h"
#endif
// CConsensus_Dlg dialog
class CConsensus_Dlg : public CDialog
{
public:

#ifdef CONSENSUS_CEDT
	CCEDT_Dlg m_CCEDT_Dlg;
#endif

#ifdef CONSENSUS_CRDC	
	CCRDC_Dlg m_CCRDC_Dlg;
#endif

#ifdef CONSENSUS_ECOR
	ECOR_Dlg m_ECOR_Dlg;
#endif

#ifdef CONSENSUS_CDOC
	CDOC_Dlg m_CDOC_Dlg;
#endif

#ifdef CONSENSUS_ENET
	CENET_Dlg m_CENET_Dlg;
#endif

	//CFPrint_Dlg m_CFPrint_Dlg;
	//CHot_Dlg m_CHot_Dlg;
	STeller_Dlg* m_pSTeller;
public:
	CConsensus_Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CONSENSUS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnCancel();
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCorpusPathButton();
	afx_msg void OnBnClickedDataPathButton();
	afx_msg LRESULT OnResponceDisplay( WPARAM wParam, LPARAM lParam );
	afx_msg void OnBnClickedPadButton();

	DECLARE_MESSAGE_MAP()
public:
	void Output_MSG(const char* outchar, WPARAM wParam);
	void Notification_Input(const char* inputchar);
	void Enable_Usable_Button();

	HANDLE m_S_Producer;
    HANDLE m_S_Consumer;
    HANDLE m_E_MessageListEvent;

public:
	CTabSheet m_sheet;
	SXMLer m_SXMLer;
	ACE_Corpus m_ACE_Corpus;
	vector<pCGigaDOC> m_GigaDOC_v;

	HANDLE msgThread;
	DWORD msgphreadId;

	bool ACE_Coupus_Loaded_Flag; 
	bool MSG_Thread_Flag;
	bool XML_Parsing_Flag;
	bool XML_Cigaword_Parsing_Flag;
	bool MaxentRan_Flag;
	bool UserInputRequireFlag;
	bool System_Busy_Flag;

	list<string> outpusmsg_l;

	CString m_Notification;
	string ACECorpusFolder;
	string GigawordFolder;

	string DataFolder;
	string MaxentRunPath;
	
	CRichEditCtrl m_ResponceEdit;
	CRichEditCtrl m_CorpusEdit;
	CRichEditCtrl m_GigawordEdit;

	//void STeller_Responce_Message(const char* poutstr);
	//void STeller_Responce_Message_with_Save(const char* poutstr);

};
