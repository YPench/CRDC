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

#pragma once
#include "afxcmn.h"
#include "..\\Include\\CGCom.h"
#include "afxwin.h"

#define MAX_DISPLAY_BUF 12768
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
	//virtual void OnCancel();
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

	string ResultHistorystr;
	bool Diaplay_Thread_Running;

	void STeller_Output_Port(const char* inputchar);
	void STeller_Output_Port_with_Save(const char* inputchar);
	bool Pause_FLag;
	char m_DisplayBuf[MAX_DISPLAY_BUF];

	
	afx_msg void OnBnClickedButtonOpenHistory();
	afx_msg void OnBnClickedStellerPause();
	CButton m_PauseButton;

	void Reset_Display_Buf(const char* outchar);

};
