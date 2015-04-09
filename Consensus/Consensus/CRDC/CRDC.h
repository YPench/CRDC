#pragma once
#include "CGCom.h"
#include "CSegmter.h"
#include "ace.h"
#include "maxen.h"
#include "GRIns.h"
#include "GREval.h"

class CRDC
{
public:
	CSegmter m_CSegmter;
	CGRIns m_CGRIns;
	CGREval m_CGREval;

	FeatureVctor Training_v;
	MaxentModel m_Maxen;
	set<string> m_WordsSet;
	size_t Max_Word_Legnth;
	bool CRDC_Model_Loaded_Flag;
	string m_Modelspace;
	string ACECorpusFolder;
	size_t m_nGross;
	size_t m_IterTime;

	BOOL For_English_Relation_Flag;
	BOOL TYPE_Flag;
	BOOL SUBTYPE_Flag;
	BOOL Enitity_TYPE_Flag;
	BOOL POS_Tag_Flag;
	BOOL EntitiesStructure_Flag;
	BOOL HeadNoun_Flag;
	BOOL OmniWords_Flag;
	BOOL EntityCLASS_Flag;
	BOOL ICTCLAS_Flag;
	BOOL LCWCC_Flag;
	BOOL Internet_Flag;
	BOOL Segmentation_Flag;
	BOOL CaseCheck_Flag;
	BOOL Generate_ICTCLAS_LEXICAL_FLAG;


public:
	CRDC();
	~CRDC();

	string Relation_Recognition_Port(const char* charInstance, const char* Arg_1, const char* Arg_2);
	string Relation_Case_Recognition_Port(Relation_Case& pmRelation_Case);
	string Generate_Training_Cases_Port(string RelationCaseFilePath, string TrainingCaseFilePath, string infopath);
	void Generate_Relation_Case_Port(string savepath, ACE_Corpus& m_ACE_Corpus);
	void Start_Relation_Training_Port(string RelationCaseFilePath, string TrainingCaseFilePath, string infopath);
	void CRDC_Training_Model_Port(string RelationCaseFilePath, string TrainingCaseFilePath, string infopath);
	void Collecting_Model_Parameter_Port(string RelationCaseFilePath, string TrainingCaseFilePath, string infopath);
	void Output_Feature_For_SVM_Port();

	//-------------------------------
	void Loading_CRDC_Model();
	void Initiate_Relation_Det_Words_set(vector<Relation_Case*>& Relation_Case_v);
};
