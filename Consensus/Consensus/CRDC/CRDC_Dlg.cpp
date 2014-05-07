// Relation.cpp : implementation file
//

#include "stdafx.h"
#include "..\\Consensus.h"
#include "CRDC_Dlg.h"


// CCRDC_Dlg dialog

IMPLEMENT_DYNAMIC(CCRDC_Dlg, CDialog)

CCRDC_Dlg::CCRDC_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCRDC_Dlg::IDD, pParent)
	, m_IterTime(30)
	, m_ICTCLAS_Flag(TRUE)
	, m_LCWCC_Flag(FALSE)
	, m_Internet_Flag(FALSE)
	, m_Segmentation_Flag(FALSE)
	, m_CaseCheck_Flag(FALSE)
	, m_AllFeature_Flag(FALSE)
	, m_AllLexicon_Flag(FALSE)
	, m_ICTLASLexion_Flag(FALSE)
	, m_TYPE_Flag(FALSE)
	, m_SUBTYPE_Flag(TRUE)
	, m_Enitity_Subtype_Flag(TRUE)
	, m_SoftConstraint(TRUE)
	, m_PositionalStructure_Flag(TRUE)
	, m_EntityCLASS_Flag(FALSE)
	, m_EntityHead_Flag(TRUE)
	, m_OmniWords_Flag(TRUE)
	, m_nGross(5)
	//, m_CParse(m_CSegmter)
{
	CRDC_Busy_Flag = false;
	Generate_Relation_Flag = false;
	Generate_Training_Flag = false;
	Start_Training_Flag = false;
	Training_Model_Flag = false;
	Loading_Model_Flag = false;
	Relation_Recognition_Flag = false;

	CRDC_Data_Floder = DATA_FOLDER;
	CRDC_Data_Floder += "CRDC\\";
	if(!NLPOP::FolderExist(NLPOP::string2CString(CRDC_Data_Floder))){
		_mkdir(CRDC_Data_Floder.c_str());
	}
	RelationCasePath = CRDC_Data_Floder + "Relation_Case.txt";
	TrainingCasePath = CRDC_Data_Floder + "Training_Case.txt";
	InfoPath = CRDC_Data_Floder + "Info.txt";

	//CorpusFolder = p_CParentDlg->CorpusFolder;
	//Enable_Usable_Button_CRDC();
}

CCRDC_Dlg::~CCRDC_Dlg()
{
}

void CCRDC_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_TYPE, m_TYPE_Flag);
	DDX_Check(pDX, IDC_SUBTYPE, m_SUBTYPE_Flag);
	DDX_Check(pDX, IDC_ENTITY_SUBTYPE, m_Enitity_Subtype_Flag);
	DDX_Check(pDX, IDC_SOFT_CONSTRAINT, m_SoftConstraint);
	DDX_Check(pDX, IDC_POSITIONAL_STRUCTURE, m_PositionalStructure_Flag);
	DDX_Check(pDX, IDC_ENTITY_HEAD, m_EntityHead_Flag);
	DDX_Check(pDX, IDC_OMNI_WORD, m_OmniWords_Flag);
	DDX_Control(pDX, IDC_XML_PARSE, m_ParseButton);
	DDX_Control(pDX, IDC_RELATION_CASE, m_RelationButton);
	DDX_Control(pDX, IDC_TRAINING_CASE, m_TrainingButton);
	DDX_Control(pDX, IDC_START_TRAINING, m_StartTrainingButton);
	DDX_Text(pDX, IDC_ITERATOR_TIME, m_IterTime);
	DDX_Text(pDX, IDC_CRDC_nCROSS, m_nGross);
	DDX_Check(pDX, IDC_ICTCLAS, m_ICTCLAS_Flag);
	DDX_Check(pDX, IDC_LCWCC, m_LCWCC_Flag);
	DDX_Check(pDX, IDC_INTERNET, m_Internet_Flag);
	DDX_Check(pDX, IDC_SEGMENTATION, m_Segmentation_Flag);
	DDX_Check(pDX, IDC_CASE_CHECK, m_CaseCheck_Flag);
	DDX_Check(pDX, IDC_ENTITY_CLASS, m_EntityCLASS_Flag);
	DDX_Check(pDX, IDC_ALL_FEATURE, m_AllFeature_Flag);
	DDX_Check(pDX, IDC_ALL_LEXICON, m_AllLexicon_Flag);
	DDX_Check(pDX, IDC_GEN_ICTCLAS_LEXICON, m_ICTLASLexion_Flag);
	DDX_Control(pDX, IDC_CRDC_LOAD_MODEL, m_LoadModelButton);
	DDX_Control(pDX, IDC_CRDC_RECOGNITION, m_RegnitionButton);
	DDX_Control(pDX, IDC_CRDC_TRAININT, m_TrainModelButton);
	DDX_Control(pDX, IDC_CRDC_INPUT, m_CRDCInputEdit);
}


BEGIN_MESSAGE_MAP(CCRDC_Dlg, CDialog)
	ON_BN_CLICKED(IDC_XML_PARSE, &CCRDC_Dlg::OnBnClickedXmlParse)
	ON_BN_CLICKED(IDC_RELATION_CASE, &CCRDC_Dlg::OnBnClickedRelationCase)
	ON_BN_CLICKED(IDC_TRAINING_CASE, &CCRDC_Dlg::OnBnClickedTrainingCase)
	ON_BN_CLICKED(IDC_START_TRAINING, &CCRDC_Dlg::OnBnClickedStartTraining)
	ON_BN_CLICKED(IDC_ALL_FEATURE, &CCRDC_Dlg::OnBnClickedAllFeature)
	ON_BN_CLICKED(IDC_ALL_LEXICON, &CCRDC_Dlg::OnBnClickedAllLexicon)
	ON_BN_CLICKED(IDC_OMNI_WORD, &CCRDC_Dlg::OnBnClickedOmniWord)
	ON_BN_CLICKED(IDC_CRDC_STOP, &CCRDC_Dlg::OnBnClicked_STOP_Button)
	ON_BN_CLICKED(IDC_CHECK2, &CCRDC_Dlg::OnBnClicked_BY_Segmentation)
	ON_BN_CLICKED(IDC_RDC_FOR_ENGLISH, &CCRDC_Dlg::OnBnClickedRdcForEnglish)
	ON_BN_CLICKED(IDC_CRDC_LOAD_MODEL, &CCRDC_Dlg::OnBnClickedCrdcLoadModel)
	ON_BN_CLICKED(IDC_CRDC_RECOGNITION, &CCRDC_Dlg::OnBnClickedCrdcRecognition)
	ON_BN_CLICKED(IDC_CRDC_TRAININT, &CCRDC_Dlg::OnBnClickedCrdcTrainint)
END_MESSAGE_MAP()


// CCRDC_Dlg message handlers

DWORD WINAPI CRDC_Implement_Threaad(LPVOID pParam)
{
	CCRDC_Dlg& dlgRef = *(CCRDC_Dlg*)pParam;
	if(dlgRef.p_CParentDlg->XML_Parse_Flag){
		dlgRef.p_CParentDlg->m_SXMLer.Xercesc_ACE_Folder_Files_Filter(dlgRef.CorpusFolder, dlgRef.p_CParentDlg->m_ACE_Corpus);
		dlgRef.p_CParentDlg->m_SXMLer.Xercesc_ACE_Folder_For_sgm_Files_Filter(dlgRef.CorpusFolder, dlgRef.p_CParentDlg->m_ACE_Corpus);
		dlgRef.p_CParentDlg->XML_Parse_Flag = false;
		dlgRef.p_CParentDlg->Output_MSG("\"XML Parse\" is complete...", 1);
	}
	if(dlgRef.Generate_Relation_Flag){
		dlgRef.m_CRDC.Generate_Relation_Case_Port(dlgRef.RelationCasePath, dlgRef.p_CParentDlg->m_ACE_Corpus);
		dlgRef.p_CParentDlg->Output_MSG("\"Generating Relation Case\" is complete...", 1);
	}
	if(dlgRef.Generate_Training_Flag){
		dlgRef.m_CRDC.Generate_Training_Cases_Port(dlgRef.RelationCasePath, dlgRef.TrainingCasePath, dlgRef.InfoPath);
		dlgRef.p_CParentDlg->Output_MSG("\"Generating Training Case\" is complete...", 1);
	}
	if(dlgRef.Start_Training_Flag){
		dlgRef.m_CRDC.Relation_Cases_Training_Port(dlgRef.RelationCasePath, dlgRef.TrainingCasePath, dlgRef.InfoPath);
		dlgRef.p_CParentDlg->Output_MSG("\"CRDC Training\" is complete...", 1);
	}
	if(dlgRef.Training_Model_Flag){
		dlgRef.m_CRDC.CRDC_Training_Model_Port(dlgRef.RelationCasePath, dlgRef.TrainingCasePath, dlgRef.InfoPath);
		dlgRef.p_CParentDlg->Output_MSG("\"CRDC Training Model\" is complete...", 1);
	}
	if(dlgRef.Loading_Model_Flag){
		dlgRef.m_CRDC.Loading_CRDC_Model();
		dlgRef.m_CRDC.CRDC_Model_Loaded_Flag = true;
	}
	if(dlgRef.Relation_Recognition_Flag){
		dlgRef.m_CRDCInputEdit.SetWindowTextW(NLPOP::string2CString(dlgRef.m_CRDC.Relation_Recognition_Port()));
	}
	dlgRef.Generate_Relation_Flag = false;
	dlgRef.Generate_Training_Flag = false;
	dlgRef.Start_Training_Flag = false;
	dlgRef.Training_Model_Flag = false;
	dlgRef.Loading_Model_Flag = false;
	dlgRef.Relation_Recognition_Flag = false;
	
	dlgRef.CRDC_Busy_Flag = false;
	dlgRef.Enable_Usable_Button_CRDC();
	ExitThread(0);
}
void CCRDC_Dlg::OnBnClickedXmlParse()
{
	if(p_CParentDlg->XML_Parse_Flag){
		p_CParentDlg->Output_MSG("\"XML Parse\" is not usable, please wait...", 1);
		return;
	}
	if(IDYES != AppCall::Secretary_Message_Box("Warning: This command may delete related file.\n Are you sure?", MB_YESNOCANCEL)){
			return;
	}
	CRDC_Busy_Flag = true;
	p_CParentDlg->XML_Parse_Flag = true;
	CorpusFolder = p_CParentDlg->CorpusFolder;
	DeleteFile(NLPOP::string2CString(RelationCasePath));
	DeleteFile(NLPOP::string2CString(TrainingCasePath));
	DeleteFile(NLPOP::string2CString(InfoPath));
	Enable_Usable_Button_CRDC();
	p_CParentDlg->Output_MSG("\"XML Parse\" is started ...", 1);

	if(!p_CParentDlg->m_ACE_Corpus.ACE_Entity_Info_map.empty() || !p_CParentDlg->m_ACE_Corpus.ACE_Relation_Info_d.empty() || !p_CParentDlg->m_ACE_Corpus.ACE_sgm_mmap.empty()){
		p_CParentDlg->m_ACE_Corpus.ACE_Entity_Info_map.clear();
		p_CParentDlg->m_ACE_Corpus.ACE_Relation_Info_d.clear();
		p_CParentDlg->m_ACE_Corpus.ACE_sgm_mmap.clear();
		Enable_Usable_Button_CRDC();
	}

	if(!NLPOP::FolderExist(NLPOP::string2CString(CorpusFolder))){
		string outstr = "Please put ACE corpus into fold: " + CorpusFolder;
		p_CParentDlg->Output_MSG(outstr.c_str(), 1);
		return;
	}

	p_CParentDlg->XML_Parse_Flag = true;
	ImpThread = CreateThread(NULL, 0, CRDC_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);
	OnPaint();
}

void CCRDC_Dlg::OnBnClickedRelationCase()
{
	if(CRDC_Busy_Flag){
		p_CParentDlg->Output_MSG("\"Generate Relation Case\" is not usable, please wait...", 1);
		return;
	}
	CRDC_Busy_Flag = true;
	p_CParentDlg->Output_MSG("\"Generate Relation Case\" is started ...", 1);

	Generate_Relation_Flag = true;
	Updata_CRDC_Configurations();
	ImpThread = CreateThread(NULL, 0, CRDC_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);
	OnPaint();
}

void CCRDC_Dlg::OnBnClickedTrainingCase()
{
	if(CRDC_Busy_Flag){
		p_CParentDlg->Output_MSG("\"Generate Training Case\" is not usable, please wait...", 1);
		return;
	}
	CRDC_Busy_Flag = true;
	p_CParentDlg->Output_MSG("\"Generate Training Case\" is started ...", 1);

	if(!NLPOP::FileExist(NLPOP::string2CString(RelationCasePath))){
		string outstr = "Relation Case is not existed in " + RelationCasePath;
		p_CParentDlg->Output_MSG(outstr.c_str(), 1);
		return;
	}
	Updata_CRDC_Configurations();

	Generate_Training_Flag = true;
	ImpThread = CreateThread(NULL, 0, CRDC_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);
}

void CCRDC_Dlg::OnBnClickedStartTraining()
{
	if(CRDC_Busy_Flag){
		p_CParentDlg->Output_MSG("\"Start Training\" is not usable, please wait...", 1);
		return;
	}
	UpdateData(TRUE);
	if((m_IterTime < 1) || (m_IterTime > 500)){
		p_CParentDlg->Output_MSG("Iterative time is unreasonable", 1);
		return;
	}
	p_CParentDlg->Output_MSG("Start Training, loading data..", 1);

	Updata_CRDC_Configurations();

	Start_Training_Flag  = true;
	ImpThread = CreateThread(NULL, 0, CRDC_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);
}
void CCRDC_Dlg::Disable_Button_CRDC()
{
	m_ParseButton.EnableWindow(FALSE);
	m_RelationButton.EnableWindow(FALSE);
	m_TrainingButton.EnableWindow(FALSE);
	m_StartTrainingButton.EnableWindow(FALSE);
	m_LoadModelButton.EnableWindow(FALSE);
	m_RegnitionButton.EnableWindow(FALSE);
	m_TrainModelButton.EnableWindow(FALSE);
}
void CCRDC_Dlg::Enable_Usable_Button_CRDC()
{
	Disable_Button_CRDC();

	m_ParseButton.EnableWindow(TRUE);
	if(!p_CParentDlg->m_ACE_Corpus.ACE_Entity_Info_map.empty() || !p_CParentDlg->m_ACE_Corpus.ACE_Relation_Info_d.empty() || !p_CParentDlg->m_ACE_Corpus.ACE_sgm_mmap.empty()){
		m_RelationButton.EnableWindow(TRUE);
	}
	if(!NLPOP::FileExist(NLPOP::string2CString(RelationCasePath))){
		m_TrainingButton.EnableWindow(FALSE);
	}
	else{
		m_TrainingButton.EnableWindow(TRUE);
	}
	if(!NLPOP::FileExist(NLPOP::string2CString(TrainingCasePath))){
		m_StartTrainingButton.EnableWindow(FALSE);
	}
	else{
		m_StartTrainingButton.EnableWindow(TRUE);
	}
	if(m_CRDC.CRDC_Model_Loaded_Flag){
		m_RegnitionButton.EnableWindow(TRUE);
	}
	else{
		m_RegnitionButton.EnableWindow(FALSE);
	}
	if(NLPOP::Exist_of_This_File(m_CRDC.m_Modelspace+"model")){
		m_LoadModelButton.EnableWindow(TRUE);
	}
	else{
		m_LoadModelButton.EnableWindow(FALSE);
	}
	if(NLPOP::FileExist(NLPOP::string2CString(p_CParentDlg->CorpusFolder+"apf.v5.1.1.dtd"))){
		m_TrainModelButton.EnableWindow(TRUE);
	}
	else{
		m_TrainModelButton.EnableWindow(FALSE);
	}
}

void CCRDC_Dlg::OnBnClickedAllFeature()
{
	UpdateData(TRUE);
	if(m_AllFeature_Flag){
		m_Enitity_Subtype_Flag = TRUE;
		m_SoftConstraint = TRUE;
		m_PositionalStructure_Flag = TRUE;
		m_EntityHead_Flag = TRUE;
		m_OmniWords_Flag = TRUE;
		UpdateData(FALSE);
		OnBnClickedOmniWord();
	}
}

void CCRDC_Dlg::OnBnClickedAllLexicon()
{
	UpdateData(TRUE);
	if(m_AllLexicon_Flag){
		m_ICTCLAS_Flag = TRUE;
		m_LCWCC_Flag = TRUE;
		m_Internet_Flag = TRUE;
		m_OmniWords_Flag = TRUE;
		m_Segmentation_Flag =FALSE;
		UpdateData(FALSE);
	}
}

void CCRDC_Dlg::OnBnClickedOmniWord()
{
	UpdateData(TRUE);
	if(m_OmniWords_Flag){
		if(!m_ICTCLAS_Flag && !m_LCWCC_Flag && !m_Internet_Flag && !m_Segmentation_Flag){
			m_ICTCLAS_Flag = TRUE;
			UpdateData(FALSE);
		}
	}
}
void CCRDC_Dlg::OnBnClicked_BY_Segmentation()
{
	UpdateData(TRUE);
	if(m_Segmentation_Flag){
		m_ICTCLAS_Flag = FALSE;
		m_LCWCC_Flag = FALSE;
		m_Internet_Flag = FALSE;
		m_OmniWords_Flag = TRUE;
		m_AllLexicon_Flag = FALSE;
		UpdateData(FALSE);
	}
}

void CCRDC_Dlg::OnBnClicked_STOP_Button()
{
	GetExitCodeThread(ImpThread, &ImpphreadId);
	TerminateThread(ImpThread, ImpphreadId);

	p_CParentDlg->Output_MSG("Current Command is Cancel...", 1);
	//p_CParentDlg->XML_Parse_Flag = false;
	Generate_Relation_Flag = false;
	Generate_Training_Flag = false;
	
	CRDC_Busy_Flag = false;
	Enable_Usable_Button_CRDC();
}
void CCRDC_Dlg::Updata_CRDC_Configurations()
{
	UpdateData(TRUE);
	m_CRDC.TYPE_Flag = m_TYPE_Flag;
	m_CRDC.SUBTYPE_Flag = m_SUBTYPE_Flag;
	m_CRDC.Enitity_Subtype_Flag = m_Enitity_Subtype_Flag;
	m_CRDC.SoftConstraint_Flag = m_SoftConstraint;
	m_CRDC.PositionalStructure_Flag = m_PositionalStructure_Flag;
	m_CRDC.EntityHead_Flag = m_EntityHead_Flag;
	m_CRDC.OmniWords_Flag = m_OmniWords_Flag;
	m_CRDC.EntityCLASS_Flag = m_EntityCLASS_Flag;
	m_CRDC.ICTCLAS_Flag = m_ICTCLAS_Flag;
	m_CRDC.LCWCC_Flag = m_LCWCC_Flag;
	m_CRDC.Internet_Flag = m_Internet_Flag;
	m_CRDC.Segmentation_Flag = m_Segmentation_Flag;
	m_CRDC.CaseCheck_Flag = m_CaseCheck_Flag;
	m_CRDC.m_nGross = m_nGross;
	m_CRDC.m_IterTime = m_IterTime;
	m_CRDC.Generate_ICTCLAS_LEXICAL_FLAG = m_ICTLASLexion_Flag;

	CorpusFolder = p_CParentDlg->CorpusFolder;
	m_CRDC.CorpusFolder = CorpusFolder;

	/*m_CERDC.TYPE_Flag = m_TYPE_Flag;
	m_CERDC.SUBTYPE_Flag = m_SUBTYPE_Flag;
	m_CERDC.SoftConstraint_Flag = m_SoftConstraint;
	m_CERDC.CaseCheck_Flag = m_CaseCheck_Flag;
	m_CERDC.Enitity_Subtype_Flag = m_Enitity_Subtype_Flag;*/
}

void CCRDC_Dlg::OnBnClickedRdcForEnglish()
{
	vector<Relation_Case> Relation_Case_v;
	Updata_CRDC_Configurations();
//	m_CERDC.Generate_Positive_And_Negetive_ACE_Relation_Cases(p_CParentDlg->m_ACE_Corpus, Relation_Case_v);
//	m_CERDC.Generate_English_Training_Cases(InfoPath.c_str(), Relation_Case_v, Maxentraining_v);
	AppCall::Secretary_Message_Box("Before training", MB_OK);
//	MAXEN::cross_validation(Maxentraining_v, 5, m_IterTime, "lbfgs", 0, true);
//	MAXEN::Get_Training_Cases_Model_Parameter(Maxentraining_v);
//	Maxentraining_v.clear();
}

void CCRDC_Dlg::OnBnClickedCrdcLoadModel()
{
	if(m_CRDC.CRDC_Model_Loaded_Flag){
		return;
	}
	if(CRDC_Busy_Flag){
		p_CParentDlg->Output_MSG("\"Load Model\" is not usable, please wait...", 1);
		return;
	}
	UpdateData(TRUE);

	p_CParentDlg->Output_MSG("Load Model, loading data..", 1);

	Loading_Model_Flag  = true;
	ImpThread = CreateThread(NULL, 0, CRDC_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);


}

void CCRDC_Dlg::OnBnClickedCrdcRecognition()
{
	// TODO: Add your control notification handler code here
	if(CRDC_Busy_Flag){
		p_CParentDlg->Output_MSG("\"Relation_Recognition\" is not usable, please wait...", 1);
		return;
	}
	UpdateData(TRUE);
	p_CParentDlg->Output_MSG("Relation_Recognition...", 1);

	Relation_Recognition_Flag  = true;
	ImpThread = CreateThread(NULL, 0, CRDC_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);
}

void CCRDC_Dlg::OnBnClickedCrdcTrainint()
{
	if(CRDC_Busy_Flag){
		p_CParentDlg->Output_MSG("\"Start Training\" is not usable, please wait...", 1);
		return;
	}
	UpdateData(TRUE);
	if((m_IterTime < 0) || (m_IterTime > 500)){
		p_CParentDlg->Output_MSG("Iterative time is unreasonable", 1);
		return;
	}
	p_CParentDlg->Output_MSG("Start Training, loading data..", 1);

	Training_Model_Flag  = true;
	ImpThread = CreateThread(NULL, 0, CRDC_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);
}
