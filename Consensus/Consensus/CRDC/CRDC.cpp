#include "stdafx.h"
#include "CRDC.h"
#include "RCCom.h"
#include "ConvertUTF.h"

CRDC::CRDC()
: m_CGRIns(m_CSegmter)
, m_CGREval(m_CSegmter)
{

	CRDC_Model_Loaded_Flag = false;
	Max_Word_Legnth = 0;
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

void CRDC::Collecting_Model_Parameter_Port(string RelationCaseFilePath, string TrainingCaseFilePath, string infopath)
{
	if(Training_v.empty()){
		Training_v.reserve(300000);
		MAXEN::Read_CSmaxent_Training_Data(TrainingCaseFilePath.c_str(), TYPE_Flag?true:false, Training_v);
	}

	AppCall::Maxen_Responce_Message("\n\nCollect Model Parameter...\n\n");
	AppCall::Maxen_Responce_Message("==========================================\n\n");

	MAXEN::Get_Training_Cases_Model_Parameter(Training_v);

}

void CRDC::Output_Feature_For_SVM_Port()
{
	// Output for SVM Evaluation;

	string inputpath = DATA_FOLDER;
	inputpath += "CRDC\\";
	inputpath += "Training_Case.txt";
	string outputfolder = "F:\\YPench\\LIBLINEAR\\Data\\";
	RCCOM::Output_For_LIBSVM(inputpath.c_str(), TYPE_Flag?true:false, outputfolder.c_str());
	//ace_op::Computing_P_R_F("F:\\P_R_F.txt");
}

void CRDC::Start_Relation_Training_Port(string RelationCaseFilePath, string TrainingCaseFilePath, string infopath)
{
	if(m_nGross == 0){
		CRDC_Training_Model_Port(RelationCaseFilePath, TrainingCaseFilePath, infopath);
		return;
	}
	if(!Training_v.empty()){
		for(size_t i = 0; i < Training_v.size(); i++){
			delete Training_v[i];
		}
		Training_v.clear();
	}
	if(Training_v.empty()){
		Training_v.reserve(300000);
		MAXEN::Read_CSmaxent_Training_Data(TrainingCaseFilePath.c_str(), TYPE_Flag?true:false, Training_v);
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
		MAXEN::Read_CSmaxent_Training_Data(TrainingCaseFilePath.c_str(), TYPE_Flag?true:false, Training_v);
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
		//AppCall::Secretary_Message_Box("Model already loaded...", MB_OK);
		return;
	}
	string modelfile = m_Modelspace;
	modelfile+= "model";
	if(!NLPOP::Exist_of_This_File(modelfile.c_str())){
		return;
	}
	m_Maxen.load(modelfile.c_str());

	string lexiconpath;
	if(LCWCC_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "LCWCC.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("Lexicon of CVMC is not existing...", MB_OK);
		}
		NLPOP::LoadWordsStringSet(lexiconpath, m_WordsSet);
	}
	if(ICTCLAS_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "CRDC\\ICTCLAS_Lexicon.dat";
		NLPOP::LoadWordsStringSet(lexiconpath, m_WordsSet);
	}
	Max_Word_Legnth = 0;
	for(set<string>::iterator site = m_WordsSet.begin();  site != m_WordsSet.end(); site++){
		if( site->size() > Max_Word_Legnth){
			Max_Word_Legnth = site->size();
		}
	}
	if(Max_Word_Legnth >= MAX_CLAUSE_LENGTH){
		AppCall::Secretary_Message_Box("Max size word is out of buffer size in:CRDC::Loading_CRDC_Model() ");
	}
	
	CRDC_Model_Loaded_Flag = true;
}



string CRDC::Relation_Case_Recognition_Port(Relation_Case& pmRelation_Case)
{
	vector<pair<string, float>> Features_v;

	m_CGREval.Generate_CRDC_Feature_Vector_Port(pmRelation_Case, Features_v);

	pmRelation_Case.TYPE = m_Maxen.predict(Features_v);

	pmRelation_Case.TYPE_V = m_Maxen.eval(Features_v, pmRelation_Case.TYPE);

	return pmRelation_Case.TYPE;
}


string CRDC::Relation_Recognition_Port(const char* charInstance, const char* Arg_1, const char* Arg_2)
{
	vector<Relation_Case> Relation_Case_v;
	
	RCCOM::Generating_Relation_Cases(charInstance, Arg_1, Arg_2, Relation_Case_v);
	
	FeatureVctor Recogizing_v;
	//m_CGREval.Generate_YPench_Evaluation_Port(Relation_Case_v, Recogizing_v);

	vector<pair<string,double>> rtnValue_v;

	MAXEN::Maxen_Recognizing_with_eval_Erasing(m_Maxen, Recogizing_v, rtnValue_v);

	ostringstream ostream;
	if(Relation_Case_v.size() != rtnValue_v.size()){
		AppCall::Secretary_Message_Box("Unmatched data in: CRDC::Relation_Recognition_Port()...", MB_OK);
	}
	for(size_t i = 0; i < Relation_Case_v.size(); i++){
		ostream << Relation_Case_v[i].first_entity.extent.charseq << "\t" << Relation_Case_v[i].sencond_entity.extent.charseq << ": ";
		ostream << rtnValue_v[i].first << "\t" << rtnValue_v[i].second << endl;
	}
	return ostream.str();
}

//-------------------------------<Generate Training Cases>
string CRDC::Generate_Training_Cases_Port(string RelationCaseFilePath, string TrainingCaseFilePath, string infopath)
{
	DulTYPE_Vctor DulTraining_v;
	if(!NLPOP::Exist_of_This_File(RelationCaseFilePath)){
		AppCall::Secretary_Message_Box("Relation Case text isn't exist!", MB_OK);
	}
	if(!DulTraining_v.empty()){
		for(size_t i = 0; i < DulTraining_v.size(); i++){
			delete DulTraining_v[i];
		}
		DulTraining_v.clear();
	}
	
	vector<Relation_Case*> Relation_Case_v;
	Relation_Case_v.reserve(300000);
	DulTraining_v.reserve(300000);
	ace_op::Load_Relation_Case(RelationCaseFilePath.c_str(), Relation_Case_v);

	if(!For_English_Relation_Flag){
		Initiate_Relation_Det_Words_set(Relation_Case_v);
	}
	
	if(!For_English_Relation_Flag){
		m_CGREval.Generate_YPench_Evaluation_Port(TrainingCaseFilePath.c_str(), Relation_Case_v, DulTraining_v);
		//m_CGREval.Generate_Che_Evaluation_Port(Relation_Case_v, DulTraining_v);
		//m_CGREval.Generate_Zhang_Evaluation_Port(Relation_Case_v, DulTraining_v);
	}
	else{
		//m_CGREval.Generate_YPench_English_Evaluation_Port(TrainingCaseFilePath.c_str(), Relation_Case_v, DulTraining_v);
		m_CGREval.Generate_Nanda_English_Evaluation_Port(TrainingCaseFilePath.c_str(), Relation_Case_v, DulTraining_v);
	}

	if(!DulTraining_v.empty()){
		MAXEN::Output_Extract_Feature_Context_For_Maxent(TrainingCaseFilePath.c_str(), DulTraining_v);
	}
	m_WordsSet.clear();
	for(size_t i = 0; i < Relation_Case_v.size(); i++){
		delete Relation_Case_v[i];
	}
	Relation_Case_v.clear();
	for(size_t i = 0; i < DulTraining_v.size(); i++){
		delete DulTraining_v[i];
	}
	DulTraining_v.clear();
	return "";
}


//-------------------------------<Generate Realtion Cases> 
void CRDC::Generate_Relation_Case_Port(string savepath, ACE_Corpus& m_ACE_Corpus)
{
	m_CGRIns.Generate_Positive_And_Negetive_ACE_Relation_Cases(savepath, m_ACE_Corpus);
}

void CRDC::Initiate_Relation_Det_Words_set(vector<Relation_Case*>& Relation_Case_v)
{
	CSegmter m_CSegmter;
	string lexiconpath;


	if(false){//(LCWCC_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "default.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("Lexicon of default is not existing...", MB_OK);
		}
		NLPOP::LoadWordsStringSet(lexiconpath, m_WordsSet);//62,036
	}

	if(true){//(LCWCC_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "LCWCC.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("Lexicon of CVMC is not existing...", MB_OK);
		}
		NLPOP::LoadWordsStringSet(lexiconpath, m_WordsSet);
	}
	
	if(true){//(ICTCLAS_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "CRDC\\ICTCLAS_Lexicon.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("ICTCLAS_Lexicon.dat is not exist, A new one will be created...", MB_OK);
			for(vector<Relation_Case*>::iterator rvite = Relation_Case_v.begin(); rvite != Relation_Case_v.end(); rvite++){
				string relation_mention_str = ace_op::Delet_0AH_and_20H_in_string((*rvite)->relatin_mention.extent.charseq.c_str());
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
			//NLPOP::LoadWordsStringSet(lexiconpath, loc_set);
		}
	}
	/*char sChar[3];
	sChar[2]=0;
	for(set<string>::iterator site = loc_set.begin(); site != loc_set.end(); site++){
		for(size_t i = 0; i < site->length(); i++){
			sChar[0] = site->c_str()[i++];
			sChar[1] = 0;
			if(sChar[0] < 0 ){
				site->c_str()[i++];
			}
			m_WordsSet.insert(sChar);
		}
	}
	lexiconpath = DATA_FOLDER;
	lexiconpath += "default.data";
	NLPOP::Save_Set_String_With_Comma_Divide(lexiconpath.c_str(), m_WordsSet);*/

	if(false){//(Internet_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "Worddb_set.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("Lexicon from Internet is not existing...", MB_OK);
		}
		NLPOP::LoadWordsStringSet(lexiconpath, m_WordsSet);
	}
	if(false){//(Internet_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "CSWordsSet.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("Lexicon from Internet is not existing...", MB_OK);
		}
		NLPOP::LoadWordsStringSet(lexiconpath, m_WordsSet);
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
	/*string notespathstr = "F:\\YPench\\ConsensusGraph\\Data\\CRDC\\MergedNotes.dat";
	map<string, string> locmap;
	NLPOP::LoadNotesMapMerge(notespathstr, locmap);
	for(map<string, string>::iterator mite = locmap.begin(); mite != locmap.end(); mite++){
		m_WordsSet.insert(mite->first.c_str());
	}*/

	Max_Word_Legnth = 0;
	for(set<string>::iterator site = m_WordsSet.begin();  site != m_WordsSet.end(); site++){
		if( site->size() > Max_Word_Legnth){
			Max_Word_Legnth = site->size();
		}
	}
}