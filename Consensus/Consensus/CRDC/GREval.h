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
class _PsDocInfo;
class CGREval
{
public:
	CRDC* pCRDC;
	CSegmter& m_CSegmter;
	bool m_Output_Training_Data_Dynamically;

	map<string,string> Synonym_map;
	map<string, size_t> BinLeftcnt_m;
	map<string, size_t> BinMiddlecnt_m;
	map<string, size_t> BinRightcnt_m;

	CGREval(CSegmter& ref_Segmter);
	~CGREval();
	void Extracting_English_radical();
	void POS_of_Entity_Mention(ACE_entity_mention& Ref_Ei, map<string, _PsDocInfo*> PsDocsInfo_map, map<string, float>& WordsCnt_map, string index);
	void POS_and_Dependency_of_Entity_Mention(Relation_Case& pm_RelCase, map<string, _PsDocInfo*> PsDocsInfo_map, map<string, float>& WordsCnt_map, string index);
	//-------------------------------Generate Training and Testing Relation Cases
	void Generate_YPench_Evaluation_Port(const char * savepath, vector<Relation_Case*>& Relation_Case_v, DulTYPE_Vctor& pmTraining_v);
	void Generate_CRDC_Feature_Vector_Port(Relation_Case& pmRelation_Case, vector<pair<string, float>>& Features_v);

	void Generate_Che_Evaluation_Port(vector<Relation_Case*>& Relation_Case_v, DulTYPE_Vctor& pmTraining_v);
	void Generate_Zhang_Evaluation_Port(vector<Relation_Case*>& Relation_Case_v, DulTYPE_Vctor& pmTraining_v);

	void Generate_YPench_English_Evaluation_Port(const char * savepath, vector<Relation_Case*>& Relation_Case_v, DulTYPE_Vctor& pmTraining_v);
	//----------------------------------------------------------------------------------------------
	void ACE_Entity_Mention_in_Sentence(map<string, map<size_t, map<size_t, ACE_entity_mention>>*>& EntityMention_mm);
	void Delet_0AH_and_20H_in_Relation_Case(Relation_Envi& pmRelEnvi);
	string Get_Relation_Envi_and_Return_9_Types_Position_Structure(Relation_Case& pmRelCase, Relation_Envi& pmRelEnvi, map<string, map<size_t, map<size_t, ACE_entity_mention>>*>& pmEntityMention_mm);
	void N_Gram_of_Sent_Sequence_with_Range_Limination(const char* sentchar, char CharTYPE, string FeatureID, size_t n, size_t range, map<string, float>& nGramFeature_map);
	void N_Gram_of_Sent_Sequence(const char* sentchar, size_t range, map<string, float>& WordsCnt_map, string prix, string prox);

	void Generating_Synonyms_Framework(vector<Relation_Case*>& Relation_Case_v);
	void Generating_Synonyms_Framework_Cases_Features(RelationContext& loc_Context, map<string, float>& WordsCnt_map);
	void Generate_Nanda_English_Evaluation_Port(const char * savepath, vector<Relation_Case*>& Relation_Case_v, DulTYPE_Vctor& pmTraining_v);
};
