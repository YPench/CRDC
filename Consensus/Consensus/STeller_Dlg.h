#pragma once
#include "afxcmn.h"
#include "..\\Include\\CGCom.h"
#include "afxwin.h"


class STeller_Dlg : public CDialog
{
	DECLARE_DYNAMIC(STeller_Dlg)

public:
	STeller_Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~STeller_Dlg();

// Dialog Data
	enum { IDD = IDD_SELLER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonReturn();
	afx_msg void OnBnClickedButtonSaveAs();

	afx_msg LRESULT STeller_Initialization( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl m_richSeller;
	HANDLE hThread;
	DWORD ThreadId;

	set<string> STopTimeType_SET;
	vector<pair<string, int>> Display_Content_v;

	string displaystr;
	string ResultHistorystr;
	bool Diaplay_Thread_Running;

	void STeller_Output_Port(const char* outchar);
	void STeller_Output_Port_with_Save(const char* outchar);
	bool Pause_FLag;

	
	afx_msg void OnBnClickedButtonOpenHistory();
	afx_msg void OnBnClickedStellerPause();
	CButton m_PauseButton;
};
