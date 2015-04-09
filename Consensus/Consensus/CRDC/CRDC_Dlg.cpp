// Relation.cpp : implementation file
//

#include "stdafx.h"
#include "..\\Consensus.h"
#include "CRDC_Dlg.h"
#include "ConvertUTF.h"

// CCRDC_Dlg dialog

IMPLEMENT_DYNAMIC(CCRDC_Dlg, CDialog)

CCRDC_Dlg::CCRDC_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCRDC_Dlg::IDD, pParent)
	, m_IterTime(30)
	, m_ICTCLAS_Flag(TRUE)
	, m_LCWCC_Flag(TRUE)
	, m_Internet_Flag(FALSE)
	, m_Segmentation_Flag(FALSE)
	, m_CaseCheck_Flag(FALSE)
	, m_AllFeature_Flag(FALSE)
	, m_AllLexicon_Flag(FALSE)
	, m_ICTLASLexion_Flag(FALSE)
	, m_TYPE_Flag(TRUE)
	, m_SUBTYPE_Flag(FALSE)
	, m_Enitity_TYPE_Flag(TRUE)
	, m_POS_Tag_Flag(TRUE)
	, m_EntitiesStructure_Flag(TRUE)
	, m_EntityCLASS_Flag(FALSE)
	, m_HeadNoun_Flag(TRUE)
	, m_OmniWords_Flag(TRUE)
	, m_nGross(5)
	//, m_CParsere(m_CSegmter)
{
	For_English_Relation_Flag = FALSE;
	CRDC_Busy_Flag = false;
	Generate_Relation_Flag = false;
	Generate_Training_Flag = false;
	Start_Training_Flag = false;
	Training_Model_Flag = false;
	Loading_Model_Flag = false;
	Relation_Recognition_Flag = false;
	Collecting_Model_Parameter_Flag = false;
	Output_Feature_For_SVM_Flag = false;

	CRDC_Data_Floder = DATA_FOLDER;
	CRDC_Data_Floder += "CRDC\\";
	if(!NLPOP::FolderExist(NLPOP::string2CString(CRDC_Data_Floder))){
		_mkdir(CRDC_Data_Floder.c_str());
	}
	RelationCasePath = CRDC_Data_Floder + "Relation_Case.txt";
	TrainingCasePath = CRDC_Data_Floder + "Training_Case.txt";
	InfoPath = CRDC_Data_Floder + "Info.txt";

	m_Menu.LoadMenu(IDR_CRDC_INPUT_MENU);
	ARG1_Loaded_Flag = FALSE;
	ARG2_Loaded_Flag = TRUE;

	//ACECorpusFolder = p_CParentDlg->ACECorpusFolder;
	//Enable_Usable_Button_CRDC();

	m_CRDC.For_English_Relation_Flag = For_English_Relation_Flag;
}

CCRDC_Dlg::~CCRDC_Dlg()
{
}

void CCRDC_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_TYPE, m_TYPE_Flag);
	DDX_Check(pDX, IDC_SUBTYPE, m_SUBTYPE_Flag);
	DDX_Check(pDX, IDC_ENTITY_SUBTYPE, m_Enitity_TYPE_Flag);
	DDX_Check(pDX, IDC_POS_TAG, m_POS_Tag_Flag);
	DDX_Check(pDX, IDC_POSITIONAL_STRUCTURE, m_EntitiesStructure_Flag);
	DDX_Check(pDX, IDC_ENTITY_HEAD, m_HeadNoun_Flag);
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

	DDX_Control(pDX, IDC_REDIT_ARG1, m_REdit_ARG1);
	DDX_Control(pDX, IDC_REDIT_ARG2, m_REdit_ARG2);
	DDX_Control(pDX, IDC_REDIT_CRDC_OUTPUT, m_REdit_Output);
}


BEGIN_MESSAGE_MAP(CCRDC_Dlg, CDialog)
	ON_BN_CLICKED(IDC_XML_PARSE, &CCRDC_Dlg::OnBnClickedXmlParse)
	ON_BN_CLICKED(IDC_RELATION_CASE, &CCRDC_Dlg::OnBnClickedRelationCase)
	ON_BN_CLICKED(IDC_TRAINING_CASE, &CCRDC_Dlg::OnBnClickedTrainingCase)
	ON_BN_CLICKED(IDC_START_TRAINING, &CCRDC_Dlg::OnBnClickedStartTraining)
	ON_BN_CLICKED(IDC_ALL_FEATURE, &CCRDC_Dlg::OnBnClickedAllFeature)
	ON_BN_CLICKED(IDC_ALL_LEXICON, &CCRDC_Dlg::OnBnClickedAllLexicon)
	ON_BN_CLICKED(IDC_OMNI_WORD, &CCRDC_Dlg::OnBnClickedOmniWord)
	ON_BN_CLICKED(IDC_CHECK2, &CCRDC_Dlg::OnBnClicked_BY_Segmentation)
	ON_BN_CLICKED(IDC_CRDC_LOAD_MODEL, &CCRDC_Dlg::OnBnClickedCrdcLoadModel)
	ON_BN_CLICKED(IDC_CRDC_RECOGNITION, &CCRDC_Dlg::OnBnClickedCrdcRecognition)
	ON_BN_CLICKED(IDC_CRDC_TRAININT, &CCRDC_Dlg::OnBnClickedCrdcTrainint)
	ON_BN_CLICKED(IDC_CRDC_DEL, &CCRDC_Dlg::OnBnClickedCrdcDel)
	ON_BN_CLICKED(IDC_CRDC_RUN, &CCRDC_Dlg::OnBnClickedCrdcRun)
	ON_BN_CLICKED(IDC_CRDC_COLLECT_PAR, &CCRDC_Dlg::OnBnClickedCrdcCollectPar)
	ON_BN_CLICKED(IDC_CRDC_OUTPUT_SVM, &CCRDC_Dlg::OnBnClickedCrdcOutputSvm)
END_MESSAGE_MAP()



BOOL CCRDC_Dlg::PreTranslateMessage(MSG* pMsg)
{

	if(WM_RBUTTONDOWN == pMsg->message )//(WM_MOUSEMOVE == pMsg->message)|| 
	{
		CWnd* pWnd = (CWnd*)   GetDlgItem(IDC_CRDC_INPUT);
		if(true){//pWnd == GetFocus())
			CWnd* pWnd = (CWnd*)   GetDlgItem(IDC_CRDC_INPUT);   
			if(true){//pWnd == GetFocus()){//richedit上发生了right   button   down
				//-----------------------copied codes
				LPPOINT lpoint=new tagPOINT; //标示menu显示位置
				::GetCursorPos(lpoint);//得到鼠标位置

				SetForegroundWindow();//本行代码作用：如果用户未选择子菜单则自动隐藏该menu
				
				if(ARG1_Loaded_Flag){
					m_Menu.EnableMenuItem(ID_MANU_ARG_1, MF_DISABLED);
				}
				else{
					m_Menu.EnableMenuItem(ID_MANU_ARG_1, MF_ENABLED);
				}
				if(ARG2_Loaded_Flag){
					m_Menu.EnableMenuItem(ID_MANU_ARG_2, MF_DISABLED);
				}
				else{
					m_Menu.EnableMenuItem(ID_MANU_ARG_2, MF_ENABLED);
				}
				//ID_MANU_ARG_1 
				m_Menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTALIGN,lpoint->x,lpoint->y,this); //显示menu
				UpdateData(TRUE);
				if(m_CRDCInputEdit.GetTextLength() != 0){
					if(!ARG1_Loaded_Flag){
						m_CRDCInputEdit.Copy();
						m_REdit_ARG1.Paste();
						ARG1_Loaded_Flag = TRUE;
						ARG2_Loaded_Flag = FALSE;
					}
					else if(!ARG2_Loaded_Flag){
						m_CRDCInputEdit.Copy();
						m_REdit_ARG2.Paste();
						ARG2_Loaded_Flag = TRUE;
					}
				}

				delete lpoint; //回收资源
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
// CCRDC_Dlg message handlers
void CCRDC_Dlg::Output_ACE_for_YQWL(string savepath, ACE_Corpus& m_ACE_Corpus)
{
	//char intchar[64];
	//_itoa_s(FrequentPatternLength - 1,intchar,64,10);

	wchar_t UnicodeChar[65535];
	char UTF8Char[65535];

	CTime loc_Time = CTime::GetCurrentTime();

	map<string, ACE_sgm>& ACE_sgm_mmap = m_ACE_Corpus.ACE_sgm_mmap;
	
	for(map<string, ACE_sgm>::iterator mite = ACE_sgm_mmap.begin(); mite != ACE_sgm_mmap.end(); mite++){
		string DocIDs = mite->first;
		DocIDs = DocIDs.substr(0, DocIDs.rfind('.'));
		DocIDs = DocIDs.substr(0, DocIDs.rfind('.'));
		DocIDs = DocIDs.substr(0, DocIDs.rfind('.'));
		if(DocIDs.length() < 8){
			AppCall::Secretary_Message_Box("Data Error: CCRDC_Dlg::Output_ACE_for_YQWL");
		}
		string year = DocIDs.substr(DocIDs.length()-8, 4);
		string month = DocIDs.substr(DocIDs.length()-4, 2);
		string day = DocIDs.substr(DocIDs.length()-2, 2);
		istringstream Yinstream(year);
		int Int_Year;
		Yinstream >> Int_Year;
		Int_Year += 13;
		if(Int_Year > 2014){
			Int_Year = 2014;
		}
		istringstream Minstream(month);
		int Int_Month;
		Minstream >> Int_Month;
		istringstream Dinstream(day);
		int Int_Day;
		Dinstream >> Int_Day;

		string oTexts;
		string& TEXT = mite->second.TEXT;
		for(size_t i = 0; i < TEXT.length(); i++){
			if('\n' == TEXT.c_str()[i]){
				continue;
			}
			if('\r' == TEXT.c_str()[i]){
				continue;
			}
			if(32 == TEXT.c_str()[i]){
				continue;
			}
			oTexts += TEXT.c_str()[i];
		}
		string filepath = savepath;
		filepath += mite->first.c_str();
		filepath += ".txt";
		ofstream out(filepath.c_str());
		if(out.bad())
			return;
		out.clear();
		out.seekp(0, ios::beg);
		out << 1 << endl;
		out << 5 << endl;
		out << "" << endl;//title
		out << Int_Year << ":" << Int_Month << ":" << Int_Day << endl;
		out << "" << endl; //url
		out << loc_Time.GetYear() << ":" << loc_Time.GetMonth() << ":" << loc_Time.GetDay() << endl;//createdtime
		
		SCONVERT::AnsiToUnicode(oTexts.c_str(), UnicodeChar);
		SCONVERT::UnicodeToUTF8(UnicodeChar, UTF8Char);

		out << UTF8Char << endl;//content;
		out << "" << endl;//author
		out << "" << endl;//views
		out << "" << endl;//comments
		out << "" << endl;//forwards
		out << "" << endl;//sourcename
		out << "" << endl;//sourceurl
		out << "Chinese" << endl;//Chinese
		out << "" << endl;//sourceplace
		out.close();
	}

}

DWORD WINAPI CRDC_Implement_Threaad(LPVOID pParam)
{
	CCRDC_Dlg& dlgRef = *(CCRDC_Dlg*)pParam;
	if(dlgRef.p_CParentDlg->XML_Parsing_Flag){
		dlgRef.p_CParentDlg->m_SXMLer.Xercesc_ACE_Folder_Files_Filter(dlgRef.ACECorpusFolder, dlgRef.p_CParentDlg->m_ACE_Corpus);
		dlgRef.p_CParentDlg->m_SXMLer.Xercesc_ACE_Folder_For_sgm_Files_Filter(dlgRef.ACECorpusFolder, dlgRef.p_CParentDlg->m_ACE_Corpus);
		//YQWL
		//dlgRef.Output_ACE_for_YQWL("F:\\YQ_Data\\", dlgRef.p_CParentDlg->m_ACE_Corpus);
		dlgRef.p_CParentDlg->XML_Parsing_Flag = false;
		dlgRef.p_CParentDlg->ACE_Coupus_Loaded_Flag = true;
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
		dlgRef.m_CRDC.Start_Relation_Training_Port(dlgRef.RelationCasePath, dlgRef.TrainingCasePath, dlgRef.InfoPath);
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
		//dlgRef.m_CRDCInputEdit.SetWindowTextW(NLPOP::string2CString(dlgRef.m_CRDC.Relation_Recognition_Port()));
	}
	if(dlgRef.Collecting_Model_Parameter_Flag){
		dlgRef.m_CRDC.Collecting_Model_Parameter_Port(dlgRef.RelationCasePath, dlgRef.TrainingCasePath, dlgRef.InfoPath);
	}
	if(dlgRef.Output_Feature_For_SVM_Flag){
		dlgRef.m_CRDC.Output_Feature_For_SVM_Port();
	}

	dlgRef.Generate_Relation_Flag = false;
	dlgRef.Generate_Training_Flag = false;
	dlgRef.Start_Training_Flag = false;
	dlgRef.Training_Model_Flag = false;
	dlgRef.Loading_Model_Flag = false;
	dlgRef.Relation_Recognition_Flag = false;
	dlgRef.Collecting_Model_Parameter_Flag = false;
	dlgRef.Output_Feature_For_SVM_Flag = false;

	
	dlgRef.CRDC_Busy_Flag = false;
	dlgRef.Enable_Usable_Button_CRDC();
	ExitThread(0);
}

void CCRDC_Dlg::OnBnClickedXmlParse()
{
	if(p_CParentDlg->XML_Parsing_Flag){
		p_CParentDlg->Output_MSG("\"XML Parse\" is not usable, please wait...", 1);
		return;
	}
	if(IDYES != AppCall::Secretary_Message_Box("Warning: This command may delete related file.\n Are you sure?", MB_YESNOCANCEL)){
			return;
	}
	CRDC_Busy_Flag = true;
	p_CParentDlg->XML_Parsing_Flag = true;
	ACECorpusFolder = p_CParentDlg->ACECorpusFolder;
	DeleteFile(NLPOP::string2CString(RelationCasePath));
	DeleteFile(NLPOP::string2CString(TrainingCasePath));
	DeleteFile(NLPOP::string2CString(InfoPath));
	Enable_Usable_Button_CRDC();
	p_CParentDlg->Output_MSG("\"XML Parse\" is started ...", 1);

	if(!p_CParentDlg->m_ACE_Corpus.ACE_Entity_Info_map.empty() || !p_CParentDlg->m_ACE_Corpus.ACE_Relation_Info_v.empty() || !p_CParentDlg->m_ACE_Corpus.ACE_sgm_mmap.empty()){
		p_CParentDlg->m_ACE_Corpus.ACE_Entity_Info_map.clear();
		p_CParentDlg->m_ACE_Corpus.ACE_Relation_Info_v.clear();
		p_CParentDlg->m_ACE_Corpus.ACE_sgm_mmap.clear();
		Enable_Usable_Button_CRDC();
	}
	if(!NLPOP::FolderExist(NLPOP::string2CString(ACECorpusFolder))){
		string outstr = "Please put ACE corpus into fold: " + ACECorpusFolder;
		p_CParentDlg->Output_MSG(outstr.c_str(), 1);
		return;
	}

	p_CParentDlg->XML_Parsing_Flag = true;
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
	CRDC_Busy_Flag = true;
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
	if(!p_CParentDlg->m_ACE_Corpus.ACE_Entity_Info_map.empty() || !p_CParentDlg->m_ACE_Corpus.ACE_Relation_Info_v.empty() || !p_CParentDlg->m_ACE_Corpus.ACE_sgm_mmap.empty()){
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
	if(NLPOP::FileExist(NLPOP::string2CString(TrainingCasePath))){
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
		m_Enitity_TYPE_Flag = TRUE;
		m_POS_Tag_Flag = TRUE;
		m_EntitiesStructure_Flag = TRUE;
		m_HeadNoun_Flag = TRUE;
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



void CCRDC_Dlg::Updata_CRDC_Configurations()
{
	UpdateData(TRUE);
	m_CRDC.TYPE_Flag = m_TYPE_Flag;
	m_CRDC.SUBTYPE_Flag = m_SUBTYPE_Flag;
	m_CRDC.Enitity_TYPE_Flag = m_Enitity_TYPE_Flag;
	m_CRDC.POS_Tag_Flag = m_POS_Tag_Flag;
	m_CRDC.EntitiesStructure_Flag = m_EntitiesStructure_Flag;
	m_CRDC.HeadNoun_Flag = m_HeadNoun_Flag;
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

	ACECorpusFolder = p_CParentDlg->ACECorpusFolder;
	m_CRDC.ACECorpusFolder = ACECorpusFolder;

	/*m_CERDC.TYPE_Flag = m_TYPE_Flag;
	m_CERDC.SUBTYPE_Flag = m_SUBTYPE_Flag;
	m_CERDC.POS_Tag_Flag = m_POS_Tag_Flag;
	m_CERDC.CaseCheck_Flag = m_CaseCheck_Flag;
	m_CERDC.Enitity_TYPE_Flag = m_Enitity_TYPE_Flag;*/
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
	CRDC_Busy_Flag = true;
	UpdateData(TRUE);
	Updata_CRDC_Configurations();

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
	CRDC_Busy_Flag = true;
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
	CRDC_Busy_Flag = true;
	UpdateData(TRUE);
	Updata_CRDC_Configurations();
	if((m_IterTime < 0) || (m_IterTime > 500)){
		p_CParentDlg->Output_MSG("Iterative time is unreasonable", 1);
		return;
	}
	p_CParentDlg->Output_MSG("Start Training, loading data..", 1);

	Training_Model_Flag  = true;
	ImpThread = CreateThread(NULL, 0, CRDC_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);
}

void CCRDC_Dlg::OnBnClickedCrdcDel()
{
	m_REdit_ARG1.SetWindowTextW(_T(""));
	m_REdit_ARG2.SetWindowTextW(_T(""));
	ARG1_Loaded_Flag = FALSE;
	ARG2_Loaded_Flag = TRUE;
}

void CCRDC_Dlg::OnBnClickedCrdcRun()
{
	if((m_REdit_ARG1.GetTextLength() != 0) || (m_REdit_ARG2.GetTextLength() != 0)){
		m_REdit_Output.SetWindowTextW(_T("Arg-1 or Arg-2 can not be empty!"));
	}
	CString Input_CS, Arg1_CS, Arg2_CS;
	m_REdit_Output.GetWindowText(Input_CS);
	m_REdit_ARG1.GetWindowText(Arg1_CS);
	m_REdit_ARG2.GetWindowText(Arg2_CS);
	string Input_s = NLPOP::CString2string(Input_CS);
	string Arg1_s = NLPOP::CString2string(Arg1_CS);
	string Arg2_s = NLPOP::CString2string(Arg2_CS);
	CString Result_CS = NLPOP::string2CString(m_CRDC.Relation_Recognition_Port(Input_s.c_str(), Arg1_s.c_str(), Arg2_s.c_str()));

	m_REdit_Output.SetWindowTextW(Result_CS);
}

void CCRDC_Dlg::OnBnClickedCrdcCollectPar()
{
	if(CRDC_Busy_Flag){
		p_CParentDlg->Output_MSG("\"Start Training\" is not usable, please wait...", 1);
		return;
	}
	CRDC_Busy_Flag = true;
	UpdateData(TRUE);
	Updata_CRDC_Configurations();
	if((m_IterTime < 0) || (m_IterTime > 500)){
		p_CParentDlg->Output_MSG("Iterative time is unreasonable", 1);
		return;
	}
	p_CParentDlg->Output_MSG("Start Collecting Parameter, loading data..", 1);

	Collecting_Model_Parameter_Flag  = true;
	ImpThread = CreateThread(NULL, 0, CRDC_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);
}

void CCRDC_Dlg::OnBnClickedCrdcOutputSvm()
{
	if(CRDC_Busy_Flag){
		p_CParentDlg->Output_MSG("\"Start Training\" is not usable, please wait...", 1);
		return;
	}
	CRDC_Busy_Flag = true;
	UpdateData(TRUE);
	Updata_CRDC_Configurations();
	if((m_IterTime < 0) || (m_IterTime > 500)){
		p_CParentDlg->Output_MSG("Iterative time is unreasonable", 1);
		return;
	}
	p_CParentDlg->Output_MSG("Start Output Feature For SVM, loading data..", 1);

	Output_Feature_For_SVM_Flag  = true;
	ImpThread = CreateThread(NULL, 0, CRDC_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);
}
