// Consensus_Dlg.h : header file
//

#pragma once
#include "afxcmn.h"

#include "SXMLer.h"

#include "TabSheet.h"
#include "CRDC\\CRDC_Dlg.h"



// CConsensus_Dlg dialog
class CConsensus_Dlg : public CDialog
{
// Construction
public:
	CConsensus_Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CONSENSUS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

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
	
	

	DECLARE_MESSAGE_MAP()
public:
	void Output_MSG(const char* outchar, WPARAM wParam);
	void Notification_Input(const char* inputchar);
	bool Notification_Pad_Flag;
	HANDLE m_S_Producer;
    HANDLE m_S_Consumer;
    HANDLE m_E_MessageListEvent;

public:
	CTabSheet m_sheet;
	CCRDC_Dlg m_CCRDC_Dlg;


	SXMLer m_SXMLer;
	ACE_Corpus m_ACE_Corpus;

	bool MSG_Thread_Flag;
	HANDLE msgThread;
	DWORD msgphreadId;

	bool XML_Parse_Flag;
	bool MaxentRan_Flag;
	bool UserInputRequireFlag;

	list<string> outpusmsg_l;

	CString m_Notification;
	string CorpusFolder;
	string DataFolder;
	string MaxentRunPath;
	
	CRichEditCtrl m_ResponceEdit;
	CRichEditCtrl m_CorpusEdit;
	CRichEditCtrl m_DataEdit;

	afx_msg void OnBnClickedPadButton();
};
