#include "stdafx.h"
#include "CRDC.h"
#include "RCCom.h"
#include "ConvertUTF.h"


CRDC::CRDC()
: m_CGRIns(m_CSegmter)
, m_CGREval(m_CSegmter)
{	
	/*string inputpath = DATA_FOLDER;
	inputpath += "CRDC\\";
	inputpath += "Training_Case.txt";
	string outputfolder = "F:\\YPench\\LIBLINEAR\\Data\\";
	RCCOM::Output_For_LIBSVM(inputpath.c_str(), outputfolder.c_str());*/
	//ace_op::Computing_P_R_F("F:\\P_R_F.txt");
	CRDC_Model_Loaded_Flag = false;
	m_Modelspace = DATA_FOLDER;
	m_Modelspace += "CRDC\\Model\\";
	if(NLPOP::FolderExist(NLPOP::string2CString(m_Modelspace))){
		//NLPOP::DeleteDir(NLPOP::string2CString(m_Modelspace));
		//_mkdir(m_Modelspace.c_str());
	}
	else{
		_mkdir(m_Modelspace.c_str());
	}
	m_CGRIns.pCRDC = this;
	m_CGREval.pCRDC = this;
}
CRDC::~CRDC()
{
	for(size_t i = 0; i < Training_v.size(); i++){
		if(!(Training_v[i] != NULL)){
			delete Training_v[i];
		}
	}
	Training_v.clear();
}
void CRDC::Relation_Cases_Training_Port(string RelationCaseFilePath, string TrainingCaseFilePath, string infopath)
{
	if(m_nGross == 0){
		CRDC_Training_Model_Port(RelationCaseFilePath, TrainingCaseFilePath, infopath);
		return;
	}
	if(Training_v.empty()){
		Training_v.reserve(300000);
		MAXEN::Read_CSmaxent_Training_Data(TrainingCaseFilePath.c_str(), Training_v);
	}
	MAXEN::cross_validation(Training_v, m_nGross, m_IterTime, "lbfgs", 0, true);
}
void CRDC::CRDC_Training_Model_Port(string RelationCaseFilePath, string TrainingCaseFilePath, string infopath)
{
	if(CRDC_Model_Loaded_Flag){
		AppCall::Secretary_Message_Box("Model already loaded...", MB_OK);
		return;
	}
	if(Training_v.empty()){
		Training_v.reserve(300000);
		MAXEN::Read_CSmaxent_Training_Data(TrainingCaseFilePath.c_str(), Training_v);
	}
	MAXEN::Maxen_Training_with_Erasing(Training_v, m_Maxen, m_IterTime);
	
	string modelpath = m_Modelspace;
	modelpath += "model";
	m_Maxen.save(modelpath.c_str(), true);
	CRDC_Model_Loaded_Flag = true;
}

void CRDC::Loading_CRDC_Model()
{
	if(CRDC_Model_Loaded_Flag){
		AppCall::Secretary_Message_Box("Model already loaded...", MB_OK);
		return;
	}
	string modelfile = m_Modelspace;
	modelfile+= "model";
	if(!NLPOP::Exist_of_This_File(modelfile.c_str())){
		return;
	}
	m_Maxen.load(modelfile.c_str());
	CRDC_Model_Loaded_Flag = true;
}
string CRDC::Relation_Recognition_Port()
{
	return "not usable now...";
}

//-------------------------------<Generate Training Cases>
string CRDC::Generate_Training_Cases_Port(string RelationCaseFilePath, string TrainingCaseFilePath, string infopath)
{
	if(!NLPOP::Exist_of_This_File(RelationCaseFilePath)){
		AppCall::Secretary_Message_Box("Relation Case text isn't exist!", MB_OK);
	}
	if(!Training_v.empty()){
		for(size_t i = 0; i < Training_v.size(); i++){
			delete Training_v[i];
		}
		Training_v.clear();
	}
	
	vector<Relation_Case> Relation_Case_v;
	Relation_Case_v.reserve(300000);
	ace_op::Load_Relation_Case(RelationCaseFilePath.c_str(), Relation_Case_v);

	Initiate_Relation_Det_Words_set(Relation_Case_v);

	m_CGREval.Generate_YPench_Evaluation_Port(Relation_Case_v, Training_v);
	//m_CGREval.Generate_Che_Evaluation_Port(Relation_Case_v, Training_v);
	//m_CGREval.Generate_Zhang_Evaluation_Port(Relation_Case_v, Training_v);


	RCCOM::Output_Extract_ACE_Relation_Words_Context_For_Maxent(TrainingCaseFilePath.c_str(), Training_v);
	m_WordsSet.clear();
	Relation_Case_v.clear();
	return "";
}


//-------------------------------<Generate Realtion Cases> 
void CRDC::Generate_Relation_Case_Port(string savepath, ACE_Corpus& m_ACE_Corpus)
{
	m_CGRIns.Generate_Positive_And_Negetive_ACE_Relation_Cases(savepath, m_ACE_Corpus);
}

void CRDC::Initiate_Relation_Det_Words_set(vector<Relation_Case>& Relation_Case_v)
{
	CSegmter m_CSegmter;
	string lexiconpath;

	if(LCWCC_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "CVMC.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("Lexicon of CVMC is not existing...", MB_OK);
		}
		NLPOP::LoadWordsStringSet(lexiconpath, m_WordsSet);
	}

	if(Internet_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "Worddb_set.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("Lexicon from Internet is not existing...", MB_OK);
		}
		NLPOP::LoadWordsStringSet(lexiconpath, m_WordsSet);
	}

	if(ICTCLAS_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "CRDC\\ICTCLAS_Lexicon.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("ICTCLAS_Lexicon.dat is not exist, A new one will be created...", MB_OK);
			for(vector<Relation_Case>::iterator rvite = Relation_Case_v.begin(); rvite != Relation_Case_v.end(); rvite++){
				string relation_mention_str = ace_op::Delet_0AH_and_20H_in_string(rvite->relatin_mention.extent.charseq.c_str());
				CLAUSEPOS loc_Seg;
				m_CSegmter.ICTCLAS_Segmention_Port(relation_mention_str.c_str(), loc_Seg);
				for(vector<string>::iterator vite = loc_Seg.Clause.begin(); vite != loc_Seg.Clause.end(); vite++){
					m_WordsSet.insert(vite->data());
				}
			}
		}
		else{
			NLPOP::LoadWordsStringSet(lexiconpath, m_WordsSet);
			//for(vector<Relation_Case>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
			//	m_WordsSet.insert(ace_op::Delet_0AH_and_20H_in_string(Ref_E1.head.charseq.c_str()));
			//	m_WordsSet.insert(ace_op::Delet_0AH_and_20H_in_string(Ref_E2.head.charseq.c_str()));
			//}
		}
	}
	/*
	lexiconpath = DATA_FOLDER;
	lexiconpath += "stopword.dat";
	vector<string> StopWords_v;
	Read_Line_InFile_To_Vector_Deque_List<vector<string>>(lexiconpath, StopWords_v, 0, '\n', false);
	for(vector<string>::iterator vite = StopWords_v.begin(); vite != StopWords_v.end(); vite++){
		if(m_WordsSet.find(vite->data()) != m_WordsSet.end()){
			m_WordsSet.erase(vite->data());
		}
	}
	*/
}