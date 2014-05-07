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
	
};
