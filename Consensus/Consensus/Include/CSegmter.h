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
