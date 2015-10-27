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

#include "CGCom.h"
#include "ace.h"

#define	 MAX_CLAUSE_LENGTH 128

class CMaxmatch
{
public:
	bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH];
	set<string> lexicon_set;
	int maxtoken_size;
	bool Init_CMaxmatch_Flag;

public:
	CMaxmatch();
	~CMaxmatch();

	void Reset_Maximum_Matcher_Lexicon(set<string>& pmOuterLexicion_s);
	void Reset_Maximum_Matcher_Lexicon(const char* lexiconpath);

	void Maximum_Match_Segmentation(const char* Sentbuf, vector<string>& segment_v, set<string>& pmWordSet, const char* cSegmentType);
	void Maximum_Match_Segmentation_Traditional(const char* Sentbuf, vector<string>& segment_v, const char* cSegmentType);
	void Maximum_Match_Segmentation_Traditional(const char* Sentbuf, vector<string>& segment_v, const char* cSegmentType, set<string>& pmWordSet);
	
		void Generate_Segmentation_Matrix(const char* SentBuf, set<string>& locCSWords_Set, vector<string>& TokenCS_v, unsigned int Max_Word_Length, bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int& mLength);
	void Forword_Max_Maximum_Matching_Segmentation(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<int>& Seg_v);
	void Backword_Max_Maximum_Matching_Segmentation(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<int>& Seg_v);
	void Generate_Segmentation_Pathn(vector<int>& pmSeg_v, vector<string>& pmTokenCS, vector<string>& pmSegment_v);


};

