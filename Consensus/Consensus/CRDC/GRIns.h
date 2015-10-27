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
	void Delete_Sentence_Contianing_Positive_Relation_Case(list<Relation_Case>& Relation_Case_l, vector<ACE_relation>& ACE_Relation_Info_v);
	void Adding_Positive_Training_Case_in_Relation_Mention(ACE_Corpus& m_ACE_Corpus, list<Relation_Case>& Relation_Case_l);

	//-----------------------------------Extract Sentence with Two Named Entities
	void Extract_Sentence_with_Two_Named_Entities(ACE_Corpus& ACE_Corpus, list<Relation_Case>& Relation_Case_l);
	void Filter_sgm_sentence_with_Relation_Mention(ACE_Corpus& ACE_Corpus, map<string, list<pair<string, pair<size_t,size_t>>>>& pm_ACE_DocSentence_map);

};
