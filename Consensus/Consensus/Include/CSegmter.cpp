#include "StdAfx.h"

#include "CSegmter.h"
#include "F:\\YPench\\Toolkit\\ICTCLAS50_Windows_32_C\\API\\ICTCLAS50.h"


CSegmter::CSegmter()
{
	ICTCLAS50_INIT_FLAG = false;
	Max_Lexicon_Size = 0;
	//CLAUSEPOS pm_PaseCS;
	//ICTCLAS_Segmention_Port("被王菲赶出家门的人", pm_PaseCS);
}

CSegmter::~CSegmter()
{
	if(ICTCLAS50_INIT_FLAG)
		ICTCLAS_Exit();

}

void CSegmter::English_Radical_Extractor(const char* sentchar, set<string>& pmRadicalSet, size_t Max_Word_Legnth, map<string, float>& WordsCnt_map, string prix = "", string prox = "")
{
	if(strlen(sentchar) == 0){
		stringstream ostream;
		ostream << prix << "#" << prox;
		if(WordsCnt_map.find(ostream.str()) == WordsCnt_map.end()){
			WordsCnt_map.insert(make_pair(ostream.str(), (float)1));
		}
		return;
	}
	char sentencechar[MAX_CLAUSE_LENGTH];
	char sChar[3];
	sChar[2]=0;
	size_t char_size = strlen(sentchar);
	if(0 == char_size){
		return;
	}
	size_t max_j;
	for(unsigned int i = 0; i < char_size; ){
		strcpy_s(sentencechar, MAX_CLAUSE_LENGTH, "");
		max_j = i + Max_Word_Legnth;
		if(max_j > char_size){
			max_j = char_size;
		}
		for(unsigned int j = i; j < max_j; ){
			sChar[0] = sentchar[j++];
			sChar[1] = 0;
			if(sChar[0] < 0 ){
				sChar[1]=sentchar[j++];
			}
			strcat_s(sentencechar, MAX_CLAUSE_LENGTH, sChar);
			if(pmRadicalSet.find(sentencechar) != pmRadicalSet.end()){
				stringstream ostream;
				ostream << prix << sentencechar << prox;
				if(WordsCnt_map.find(ostream.str()) == WordsCnt_map.end()){
					WordsCnt_map.insert(make_pair(ostream.str(), (float)1));
				}
				//else {
				//	WordsCnt_map[ostream.str()]++;
				//}
			}
		}
		if(sentchar[i++] < 0){
			i++;
		}
	}
}

void CSegmter::English_Words_Extractor(const char* sentchar, set<string>& pmWordSet, size_t Max_Word_Legnth, map<string, float>& WordsCnt_map, string prix = "", string prox = "")
{
	char getlineBuf[MAX_SENTENCE];
	istringstream streamstr(sentchar);
	vector<string> words_v;
	while(EOF != streamstr.peek()){
		streamstr.getline(getlineBuf, MAX_SENTENCE,' ');
		if(strlen(getlineBuf) == 0){
			continue;
		}
		words_v.push_back(getlineBuf);
		stringstream ostream;
		ostream << prix << getlineBuf << prox;
		if(WordsCnt_map.find(ostream.str()) == WordsCnt_map.end()){
			WordsCnt_map.insert(make_pair(ostream.str(), (float)1));
		}
	}
	/*for(size_t i = 0; i < words_v.size(); i++){
		if(i+1 < words_v.size()){
			stringstream ostream;
			ostream << "n_" << prix << "_" << words_v[i] << "_" << words_v[i+1] << "_" << prox;
			WordsCnt_map.insert(make_pair(ostream.str(), (float)1));
		}
	}*/
}

void CSegmter::ICTCLAS_Segmentation_words_feature_Extracting(const char* sentchar, map<string, float>& WordsCnt_map, string prix = "", string prox = "")
{
	string feature;
	CLAUSEPOS loc_Seg;

	if(strlen(sentchar) == 0){
		feature = prix + "#" + prox;
		WordsCnt_map.insert(make_pair(feature, (float)1));
		return;
	}

	ICTCLAS_Segmention_Port(sentchar, loc_Seg);
	for(vector<string>::iterator vite = loc_Seg.Clause.begin(); vite != loc_Seg.Clause.end(); vite++){
		feature = prix + vite->data() + prox;
		if(WordsCnt_map.find(feature) == WordsCnt_map.end()){
			WordsCnt_map.insert(make_pair(feature, (float)1));
		}
	}
}


void CSegmter::ICTCLAS_Segmention_Port(const char* sentstr, CLAUSEPOS& pm_PaseCS)
{
	if(!ICTCLAS50_INIT_FLAG)
	{
		if(ICTCLAS_Init("F:\\YPench\\Toolkit\\ICTCLAS50_Windows_32_C\\API\\")) 	
			ICTCLAS50_INIT_FLAG = true;
		else
		{
			ICTCLAS50_INIT_FLAG = false;
			AppCall::Secretary_Message_Box("ICTCLAS30没有正确初始化……", MB_OK);
			return;
		}
	}

	int nPaLen = strlen(sentstr);
	if(nPaLen > MAX_SENTENCE)
		return;
	char sSentence[MAX_SENTENCE];

	strncpy_s(sSentence, sentstr, nPaLen); ;
	char* sRst = 0;//用户自行分配空间，用于保存结果；
	sRst = new char[nPaLen*8];//建议长度为字符串长度的6倍。(ICTCLAS建议出错)
	//*****sRst = 0x0897bfe0 "9/vyou " 返回长度为7,溢出报错。
	int nRstLen = 0;
	nRstLen = ICTCLAS_ParagraphProcess(sSentence, nPaLen, sRst, CODE_TYPE_UNKNOWN, 1);
	istringstream streamstr(sRst);
	if(EOF == streamstr.peek())
		return;
	char getlineBuf[MAX_SENTENCE];
	string wordstr;
	string morphstr;
	while(true)
	{
		streamstr.getline(getlineBuf, MAX_SENTENCE,'/');
		wordstr = getlineBuf;
		streamstr.getline(getlineBuf, MAX_SENTENCE,' ');
		morphstr  = getlineBuf;
		if((wordstr.length() != 0) && (morphstr.length() != 0))
		{
			pm_PaseCS.Clause.push_back(wordstr);
			pm_PaseCS.POS.push_back(morphstr);
		}
		else
		{
			if(EOF == streamstr.peek())
				break;
			streamstr.getline(getlineBuf, MAX_SENTENCE,' ');
		}
	}
	delete sRst;
	return ; 



}


void CSegmter::Omni_words_feature_Extracting(const char* sentchar, set<string>& pmWordSet, size_t Max_Word_Legnth, map<string, float>& WordsCnt_map, string prix = "", string prox = "")
{
	if(strlen(sentchar) == 0){
		stringstream ostream;
		ostream << prix << "#" << prox;
		if(WordsCnt_map.find(ostream.str()) == WordsCnt_map.end()){
			WordsCnt_map.insert(make_pair(ostream.str(), (float)1));
		}
		return;
	}
	char sentencechar[MAX_CLAUSE_LENGTH];
	char sChar[3];
	sChar[2]=0;
	size_t char_size = strlen(sentchar);
	if(0 == char_size){
		return;
	}
	size_t max_j;
	for(unsigned int i = 0; i < char_size; ){
		strcpy_s(sentencechar, MAX_CLAUSE_LENGTH, "");
		max_j = i + Max_Word_Legnth;
		if(max_j > char_size){
			max_j = char_size;
		}
		for(unsigned int j = i; j < max_j; ){
			sChar[0] = sentchar[j++];
			sChar[1] = 0;
			if(sChar[0] < 0 ){
				sChar[1]=sentchar[j++];
			}
			strcat_s(sentencechar, MAX_CLAUSE_LENGTH, sChar);
			if(pmWordSet.find(sentencechar) != pmWordSet.end()){
				stringstream ostream;
				ostream << prix << sentencechar << prox;
				if(WordsCnt_map.find(ostream.str()) == WordsCnt_map.end()){
					WordsCnt_map.insert(make_pair(ostream.str(), (float)1));
				}
				//else {
				//	WordsCnt_map[ostream.str()]++;
				//}
			}
		}
		if(sentchar[i++] < 0){
			i++;
		}
	}
	/*string feature;
	map<string, unsigned long> loc_featuremap;

	Get_Sentence_Omni_Words_Cnt(sentchar, pmWordSet, loc_featuremap);

	for(map<string, unsigned long>::iterator mite = loc_featuremap.begin(); mite != loc_featuremap.end(); mite++){
		feature = prix + mite->first + prox;
		if(WordsCnt_map.find(feature) == WordsCnt_map.end()){
			WordsCnt_map.insert(make_pair(feature, (float)1));//Warning
		}
	}*/
}
//older function
void CSegmter::Get_Sentence_Omni_Words_Cnt(const char* charstr, set<string>& pmWordsset, map<string, unsigned long>& WordsCnt_map)
{
	char sentencechar[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;
	if(strlen(charstr) == 0){
		if(WordsCnt_map.find("#") == WordsCnt_map.end()){
			WordsCnt_map.insert(make_pair("#", 1));
		}
		else {
			WordsCnt_map["#"]++;
		}
		return;
	}
	for(unsigned int i = 0; i < strlen(charstr); ){
		strcpy_s(sentencechar, MAX_SENTENCE, "");
		for(unsigned int j = i; j < strlen(charstr); ){
			sChar[0] = charstr[j++];
			sChar[1] = 0;	
			if(sChar[0] < 0 ){
				sChar[1]=charstr[j++];
			}
			strcat_s(sentencechar, MAX_SENTENCE, sChar);
			if(pmWordsset.find(sentencechar) != pmWordsset.end()){
				if(WordsCnt_map.find(sentencechar) == WordsCnt_map.end()){
					WordsCnt_map.insert(make_pair(sentencechar, 1));
				}
				else {
					WordsCnt_map[sentencechar]++;
				}
			}
		}
		if(charstr[i++] < 0){
			i++;
		}
	}
}

void CSegmter::Omni_words_feature_Extracting(const char* sentchar, set<string>& pmWordSet, map<string, size_t>& WordsCnt_map)
{
	char sentencechar[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;
	size_t char_size = strlen(sentchar);

	size_t max_j;
	for(unsigned int i = 0; i < char_size; ){
		strcpy_s(sentencechar, MAX_SENTENCE, "");
		max_j = i + Max_Lexicon_Size;
		if(max_j > char_size){
			max_j = char_size;
		}
		for(unsigned int j = i; j < max_j; ){
			sChar[0] = sentchar[j++];
			sChar[1] = 0;
			if(sChar[0] < 0 ){
				sChar[1]=sentchar[j++];
			}
			strcat_s(sentencechar, MAX_SENTENCE, sChar);
			if(pmWordSet.find(sentencechar) != pmWordSet.end()){
				if(WordsCnt_map.find(sentencechar) == WordsCnt_map.end()){
					WordsCnt_map.insert(make_pair(sentencechar, 1));
				}
				else {
					WordsCnt_map[sentencechar]++;
				}
			}
		}
		if(sentchar[i++] < 0){
			i++;
		}
	}
}

void CSegmter::Get_Sentence_Omni_Words_Cnt(const char* charstr, set<string>& pmWordsset, size_t Max_Word_Legnth, map<string, unsigned long>& WordsCnt_map)
{
	char sentencechar[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;
	size_t charstrlen = strlen(charstr);

	for(unsigned int i = 0; i < charstrlen; ){
		strcpy_s(sentencechar, MAX_SENTENCE, "");
		size_t max_j = i + Max_Word_Legnth;
		if(max_j > charstrlen){
			max_j = charstrlen;
		}
		for(unsigned int j = i; j < max_j; ){
			sChar[0] = charstr[j++];
			sChar[1] = 0;	
			if(sChar[0] < 0 ){
				sChar[1]=charstr[j++];
			}
			strcat_s(sentencechar, MAX_SENTENCE, sChar);
			if(pmWordsset.find(sentencechar) != pmWordsset.end()){
				if(WordsCnt_map.find(sentencechar) == WordsCnt_map.end()){
					WordsCnt_map.insert(make_pair(sentencechar, 1));
				}
				else {
					WordsCnt_map[sentencechar]++;
				}
			}
		}
		if(charstr[i++] < 0){
			i++;
		}
	}
}



void CSegmter::Initiate_Words_set(vector<DismCase>& pDismCase_v, set<string>& pmWordSet)
{
	string lexiconpath;
	bool LCWCC_Flag = true;
	bool Internet_Flag = false;
	bool ICTCLAS_Flag = false;

	if(LCWCC_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "LCWCC.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("Lexicon of CVMC is not existing...", MB_OK);
		}
		NLPOP::LoadWordsStringSet(lexiconpath, pmWordSet);
	}

	if(Internet_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "Worddb_set.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("Lexicon from Internet is not existing...", MB_OK);
		}
		NLPOP::LoadWordsStringSet(lexiconpath, pmWordSet);
	}

	if(ICTCLAS_Flag){
		lexiconpath = DATA_FOLDER;
		lexiconpath += "ICTCLAS_Lexicon.dat";
		if(!NLPOP::Exist_of_This_File(lexiconpath)){
			AppCall::Secretary_Message_Box("ICTCLAS_Lexicon.dat is not exist, A new one will be created...", MB_OK);
			for(vector<DismCase>::iterator rvite = pDismCase_v.begin(); rvite != pDismCase_v.end(); rvite++){
//				string relation_mention_str = ace_op::Delet_0AH_and_20H_in_string(rvite->sentence.c_str());
				CLAUSEPOS loc_Seg;
//				ICTCLAS_Segmention_Port(relation_mention_str.c_str(), loc_Seg);
				for(vector<string>::iterator vite = loc_Seg.Clause.begin(); vite != loc_Seg.Clause.end(); vite++){
					pmWordSet.insert(vite->data());
				}
			}
		}
		else{
			NLPOP::LoadWordsStringSet(lexiconpath, pmWordSet);
		}
	}

}
