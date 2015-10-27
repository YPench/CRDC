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

class SXMLer
{

public:
	bool Xercesc_Initialize_Flag;
	size_t Pased_File_Cnt;

public:
	SXMLer();
	~SXMLer();

	string Xercesc_Initialize();
	string Xercesc_ACE_Folder_For_sgm_Files_Filter(string path, ACE_Corpus& m_ACE_Corpus);
	string Xercesc_ACE_Folder_Files_Filter(string path, ACE_Corpus& m_ACE_Corpus);

	bool Xercesc_ACE_Relation_Corpus_Extractor(const char* xmlFile, ACE_Corpus& m_ACE_Corpus);
	bool Xercesc_ACE_sgm_Content_Extractor(const char* xmlFile, ACE_Corpus& m_ACE_Corpus);
	bool Xercesc_Gigaword_Content_Extractor(const char* xmlFile, vector<pCGigaDOC>& m_GigaDOC_v, const char* TYPE);
	bool Xercesc_ACE_Action_Mention_Extractor(const char* xmlFile, ACE_Corpus& m_ACE_Corpus);
	
};
