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

//-------------------CSegmter-------------------------------------------
class CSegmter
{
public:
	bool ICTCLAS50_INIT_FLAG;
	size_t Max_Lexicon_Size;

public:
	CSegmter();
	~CSegmter();

	void ICTCLAS_Segmentation_words_feature_Extracting(const char* sentchar, map<string, float>& WordsCnt_map, string prix, string prox);
	void ICTCLAS_Segmention_Port(const char* sentstr, CLAUSEPOS& pm_PaseCS);
	void Omni_words_feature_Extracting(const char* sentchar, set<string>& pmWordSet, size_t Max_Word_Legnth, map<string, float>& WordsCnt_map, string prix, string prox);
	void Omni_words_feature_Extracting(const char* sentchar, set<string>& pmWordSet, map<string, size_t>& WordsCnt_map);
	void Get_Sentence_Omni_Words_Cnt(const char* charstr, set<string>& pmWordsset, size_t Max_Word_Legnth,  map<string, unsigned long>& WordsCnt_map);
	void Initiate_Words_set(vector<DismCase>& pDismCase_v, set<string>& pmWordSet);
	//older function
	void Get_Sentence_Omni_Words_Cnt(const char* charstr, set<string>& pmWordsset, map<string, unsigned long>& WordsCnt_map);
	void English_Words_Extractor(const char* sentchar, set<string>& pmWordSet, size_t Max_Word_Legnth, map<string, float>& WordsCnt_map, string prix, string prox);
	void English_Radical_Extractor(const char* sentchar, set<string>& pmRadicalSet, size_t Max_Word_Legnth, map<string, float>& WordsCnt_map, string prix, string prox);

};
