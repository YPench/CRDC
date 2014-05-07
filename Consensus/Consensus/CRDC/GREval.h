#pragma once
#include "..\\Include\\CGCom.h"
#include "..\\Include\\ace.h"
#include "CSegmter.h"

typedef struct _Relation_Envi{
	string E1_Extend;
	string E1_Head;
	string E2_Extend;
	string E2_Head;
	string E1_inBetw_E2;
	string CBM1;
	string CAM1;
	string CBM2;
	string CAM2;
}Relation_Envi, *pRelation_Envi;


class CRDC;
class CGREval
{
public:
	CRDC* pCRDC;
	CSegmter& m_CSegmter;

	CGREval(CSegmter& ref_Segmter);
	~CGREval();

	//-------------------------------Generate Training and Testing Relation Cases
	void Generate_YPench_Evaluation_Port(vector<Relation_Case>& Relation_Case_v, FeatureVctor& pmTraining_v);
	void Generate_Che_Evaluation_Port(vector<Relation_Case>& Relation_Case_v, FeatureVctor& pmTraining_v);
	void Generate_Zhang_Evaluation_Port(vector<Relation_Case>& Relation_Case_v, FeatureVctor& pmTraining_v);

	void ACE_Entity_Mention_in_Sentence(map<string, map<size_t, map<size_t, ACE_entity_mention>>*>& EntityMention_mm);
	void Delet_0AH_and_20H_in_Relation_Case(Relation_Envi& pmRelEnvi);
	string Get_Relation_Envi_and_Return_9_Types_Position_Structure(Relation_Case& pmRelCase, Relation_Envi& pmRelEnvi, map<string, map<size_t, map<size_t, ACE_entity_mention>>*>& pmEntityMention_mm);
	void N_Gram_of_Sent_Sequence_with_Range_Limination(const char* sentchar, char CharTYPE, string FeatureID, size_t n, size_t range, map<string, float>& nGramFeature_map);



};
