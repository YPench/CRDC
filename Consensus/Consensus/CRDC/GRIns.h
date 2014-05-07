#pragma once
#include "..\\Include\\CGCom.h"
#include "..\\Include\\ace.h"
#include "CSegmter.h"

class CRDC;

class CGRIns
{
public:
	CRDC* pCRDC;
	CSegmter& m_CSegmter;
	CGRIns(CSegmter& ref_Segmter);
	~CGRIns();
	size_t SENID;

	//-----------------------------------Extract Positive and Negative Relation Instances
	void Generate_Positive_And_Negetive_ACE_Relation_Cases(string savepath, ACE_Corpus& m_ACE_Corpus);
	void Delete_Sentence_Contianing_Positive_Relation_Case(list<Relation_Case>& Relation_Case_l, deque<ACE_relation>& ACE_Relation_Info_d);
	void Adding_Positive_Training_Case_in_Relation_Mention(ACE_Corpus& m_ACE_Corpus, list<Relation_Case>& Relation_Case_l);

	//-----------------------------------Extract Sentence with Two Named Entities
	void Extract_Sentence_with_Two_Named_Entities(ACE_Corpus& ACE_Corpus, list<Relation_Case>& Relation_Case_l);
	void Filter_sgm_sentence_with_Relation_Mention(ACE_Corpus& ACE_Corpus, map<string, list<pair<string, pair<size_t,size_t>>>>& pm_ACE_DocSentence_map);

};
