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

#include "CRDC.h"
//#include "ERDC.h"
//#include "Parse.h"
#include "maxen.h"
//#include "CSegmter.h"
#include "afxwin.h"
#include "afxcmn.h"
// CCRDC_Dlg dialog

class CConsensus_Dlg;
class CCRDC_Dlg : public CDialog
{
	DECLARE_DYNAMIC(CCRDC_Dlg)

public:
	CCRDC_Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCRDC_Dlg();

// Dialog Data
	enum { IDD = IDD_CRDC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedXmlParse();
	afx_msg void OnBnClickedRelationCase();
	afx_msg void OnBnClickedTrainingCase();
	afx_msg void OnBnClickedStartTraining();
	afx_msg void OnBnClickedOmniWord();
	afx_msg void OnBnClicked_BY_Segmentation();
	afx_msg void OnBnClickedAllFeature();
	afx_msg void OnBnClickedAllLexicon();
	afx_msg void OnBnClickedRdcForEnglish();
	afx_msg void OnBnClickedCrdcLoadModel();
	afx_msg void OnBnClickedCrdcRecognition();
	afx_msg void OnBnClickedCrdcTrainint();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	
	void Disable_Button_CRDC();
	void Enable_Usable_Button_CRDC();
	void Updata_CRDC_Configurations();

public:
	CConsensus_Dlg* p_CParentDlg;
	CRDC m_CRDC;
	//CERDC m_CERDC;

	afx_msg void OnBnClickedCrdcDel();
	afx_msg void OnBnClickedCrdcRun();
	afx_msg void OnBnClickedCrdcCollectPar();
	afx_msg void OnBnClickedCrdcOutputSvm();

	CButton m_ParseButton;
	CButton m_RelationButton;
	CButton m_TrainingButton;
	CButton m_StartTrainingButton;
	CButton m_LoadModelButton;
	CButton m_RegnitionButton;
	CButton m_TrainModelButton;
	
	//string ACE_Corpus_Folder;
	string CRDC_Data_Floder;
	string RelationCasePath;
	string TrainingCasePath;
	string InfoPath;

	BOOL For_English_Relation_Flag;

	string ACECorpusFolder;

	int m_IterTime;
	size_t m_nGross;
	
	BOOL m_ICTCLAS_Flag;
	BOOL m_LCWCC_Flag;
	BOOL m_Internet_Flag;
	BOOL m_Segmentation_Flag;
	BOOL m_ICTLASLexion_Flag;
	BOOL m_CaseCheck_Flag;
	BOOL m_AllFeature_Flag;
	BOOL m_AllLexicon_Flag;
	BOOL m_TYPE_Flag;
	BOOL m_SUBTYPE_Flag;
	BOOL m_Enitity_TYPE_Flag;
	BOOL m_POS_Tag_Flag;
	BOOL m_EntitiesStructure_Flag;
	BOOL m_HeadNoun_Flag;
	BOOL m_OmniWords_Flag;
	BOOL m_EntityCLASS_Flag;

	HANDLE ImpThread;
	DWORD ImpphreadId;
	bool CRDC_Busy_Flag;

	bool Generate_Relation_Flag;
	bool Generate_Training_Flag;
	bool Start_Training_Flag;
	bool Training_Model_Flag;
	bool Loading_Model_Flag;
	bool Relation_Recognition_Flag;
	bool Collecting_Model_Parameter_Flag;
	bool Output_Feature_For_SVM_Flag;

	void Output_ACE_for_YQWL(string savepath, ACE_Corpus& m_ACE_Corpus);
	
	CMenu m_Menu;
	BOOL ARG1_Loaded_Flag;
	BOOL ARG2_Loaded_Flag;
	CRichEditCtrl m_REdit_ARG1;
	CRichEditCtrl m_REdit_ARG2;
	CRichEditCtrl m_REdit_Output;
	CRichEditCtrl m_CRDCInputEdit;

};
