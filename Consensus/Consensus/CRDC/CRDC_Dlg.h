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
	afx_msg void OnBnClicked_STOP_Button();
	afx_msg void OnBnClicked_BY_Segmentation();
	afx_msg void OnBnClickedAllFeature();
	afx_msg void OnBnClickedAllLexicon();
	afx_msg void OnBnClickedRdcForEnglish();
	afx_msg void OnBnClickedCrdcLoadModel();
	afx_msg void OnBnClickedCrdcRecognition();
	afx_msg void OnBnClickedCrdcTrainint();
	
	void Disable_Button_CRDC();
	void Enable_Usable_Button_CRDC();
	void Updata_CRDC_Configurations();

public:
	CConsensus_Dlg* p_CParentDlg;
	CRDC m_CRDC;
	//CERDC m_CERDC;

	CButton m_ParseButton;
	CButton m_RelationButton;
	CButton m_TrainingButton;
	CButton m_StartTrainingButton;
	CButton m_LoadModelButton;
	CButton m_RegnitionButton;
	CButton m_TrainModelButton;
	CRichEditCtrl m_CRDCInputEdit;

	//string ACE_Corpus_Folder;
	string CRDC_Data_Floder;
	string RelationCasePath;
	string TrainingCasePath;
	string InfoPath;

	string CorpusFolder;

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
	BOOL m_Enitity_Subtype_Flag;
	BOOL m_SoftConstraint;
	BOOL m_PositionalStructure_Flag;
	BOOL m_EntityHead_Flag;
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
	
};
