
#include "StdAfx.h"
#include "CMaxmatch.h"


CMaxmatch::CMaxmatch()
{
	/*vector<string> segment_v;
	set<string> pmWordSet;
	string locstr = "我曾多次看《法门寺》这一出戏，我非常欣赏演员们的表演艺术";
	Maximum_Match_Segmentation(locstr.c_str(), segment_v, pmWordSet, 0);*/
	maxtoken_size = 0;
	Init_CMaxmatch_Flag = false;
}
CMaxmatch::~CMaxmatch()
{
}

void CMaxmatch::Reset_Maximum_Matcher_Lexicon(const char* lexiconpath)
{
	if(Init_CMaxmatch_Flag){
		return;
	}
	if(!NLPOP::FileExist(NLPOP::string2CString(lexiconpath))){
		AppCall::Secretary_Message_Box(lexiconpath, MB_OK);
	}

	lexicon_set.clear();
	NLPOP::LoadWordsStringSet(lexiconpath, lexicon_set);
	maxtoken_size = 0;
	for(set<string>::iterator site = lexicon_set.begin(); site != lexicon_set.end(); site++){
		if(site->size() > (size_t)maxtoken_size){
			maxtoken_size = site->size();
		}
	}
	Init_CMaxmatch_Flag = true;
}

void CMaxmatch::Reset_Maximum_Matcher_Lexicon(set<string>& pmOuterLexicion_s)
{
	lexicon_set.clear();
	maxtoken_size = 0;
	for(set<string>::iterator site = pmOuterLexicion_s.begin(); site != pmOuterLexicion_s.end(); site++){
		if(site->size() > (size_t)maxtoken_size){
			maxtoken_size = site->size();
		}
		lexicon_set.insert(site->data());
	}
	
}



//0: Forward Maximum;
//1: Backward Maximum;
void CMaxmatch::Maximum_Match_Segmentation_Traditional(const char* Sentbuf, vector<string>& segment_v, const char* cSegmentType)
{
	if(!Init_CMaxmatch_Flag || (0 == maxtoken_size)){
		AppCall::Secretary_Message_Box("CMaxmatch should be initialzed...", MB_OK);
	}
	vector<string> chars_v;
	NLPOP::charseq_to_vector(Sentbuf, chars_v);
	string wordstr;
	string matchstr;
	bool Lexicon_Matched_Flag;
	if(!strcmp(cSegmentType, "FMM")){
		for(size_t i = 0; i < chars_v.size(); i++){
			wordstr = "";
			Lexicon_Matched_Flag = false;
			size_t match_posit;
			for(size_t j = i; ((j < chars_v.size()) && (!((j-i) > (size_t)maxtoken_size))); j++){
				wordstr += chars_v[j];
				if(lexicon_set.find(wordstr) != lexicon_set.end()){
					matchstr = wordstr;
					match_posit = j;
					Lexicon_Matched_Flag = true;
				}
			}
			if(Lexicon_Matched_Flag){
				segment_v.push_back(matchstr);
				i = match_posit;
			}
			else{
				segment_v.push_back(chars_v[i]);
			}
		}
	}
	else if(!strcmp(cSegmentType, "BMM")){
		vector<string> reverse_segment_v;
		vector<string>::reverse_iterator matchite;
		for(vector<string>::reverse_iterator i = chars_v.rbegin(); i != chars_v.rend(); i++){
			wordstr = "";
			Lexicon_Matched_Flag = false;
			for(vector<string>::reverse_iterator j = i; ((j != chars_v.rend()) && (!((j-i) > maxtoken_size))); j++){
				wordstr = j->data()+wordstr;
				if(lexicon_set.find(wordstr) != lexicon_set.end()){
					matchstr = wordstr;
					matchite = j;
					Lexicon_Matched_Flag = true;
				}
			}
			if(Lexicon_Matched_Flag){
				reverse_segment_v.push_back(matchstr);
				i = matchite;
			}
			else{
				reverse_segment_v.push_back(i->data());
			}
		}
		for(vector<string>::reverse_iterator rvite = reverse_segment_v.rbegin(); rvite != reverse_segment_v.rend(); rvite++){
			segment_v.push_back(rvite->data());
		}
	}
}

void CMaxmatch::Maximum_Match_Segmentation_Traditional(const char* Sentbuf, vector<string>& segment_v, const char* cSegmentType, set<string>& pmWordSet)
{
	if(pmWordSet.empty()){
		AppCall::Secretary_Message_Box("CMaxmatch should be initialzed...", MB_OK);
	}
	vector<string> chars_v;
	NLPOP::charseq_to_vector(Sentbuf, chars_v);
	string wordstr;
	string matchstr;
	bool Lexicon_Matched_Flag;
	if(!strcmp(cSegmentType, "FMM")){
		for(size_t i = 0; i < chars_v.size(); i++){
			wordstr = "";
			Lexicon_Matched_Flag = false;
			size_t match_posit;
			for(size_t j = i; j < chars_v.size(); j++){
				wordstr += chars_v[j];
				if(pmWordSet.find(wordstr) != pmWordSet.end()){
					matchstr = wordstr;
					match_posit = j;
					Lexicon_Matched_Flag = true;
				}
			}
			if(Lexicon_Matched_Flag){
				segment_v.push_back(matchstr);
				i = match_posit;
			}
			else{
				segment_v.push_back(chars_v[i]);
			}
		}
	}
	else if(!strcmp(cSegmentType, "BMM")){
		vector<string> reverse_segment_v;
		vector<string>::reverse_iterator matchite;
		for(vector<string>::reverse_iterator i = chars_v.rbegin(); i != chars_v.rend(); i++){
			wordstr = "";
			Lexicon_Matched_Flag = false;
			for(vector<string>::reverse_iterator j = i; j != chars_v.rend(); j++){
				wordstr = j->data()+wordstr;
				if(pmWordSet.find(wordstr) != pmWordSet.end()){
					matchstr = wordstr;
					matchite = j;
					Lexicon_Matched_Flag = true;
				}
			}
			if(Lexicon_Matched_Flag){
				reverse_segment_v.push_back(matchstr);
				i = matchite;
			}
			else{
				reverse_segment_v.push_back(i->data());
			}
		}
		for(vector<string>::reverse_iterator rvite = reverse_segment_v.rbegin(); rvite != reverse_segment_v.rend(); rvite++){
			segment_v.push_back(rvite->data());
		}
	}
}


void CMaxmatch::Maximum_Match_Segmentation(const char* Sentbuf, vector<string>& segment_v, set<string>& pmWordSet, const char* cSegmentType)
{
	ZeroMemory(SegMatrix, MAX_CLAUSE_LENGTH*MAX_CLAUSE_LENGTH*sizeof(bool));
	unsigned int mLength = 0;
	segment_v.clear();

	vector<int> segInt_v;
	vector<string> TokenCS_v;

	if(NLPOP::Get_Chinese_Sentence_Length_Counter(Sentbuf) > MAX_CLAUSE_LENGTH){
		//ostringstream ostream;
		//ostream << "Sentence is outnumber in: CMaxmatch::Maximum_Match_Segmentation()" << endl;
		//ostream <<  Sentbuf << endl;
		//ostream << "Length by Chinese: " << NLPOP::Get_Chinese_Sentence_Length_Counter(Sentbuf) << endl;
		//AppCall::Secretary_Message_Box(ostream.str(), MB_OK);

		size_t length = strlen(Sentbuf);
		size_t cnt = 0;
		size_t i = 0;
		if(!strcmp(cSegmentType, "FMM")){
			char* locsenchar = new char[length+1];
			strcpy_s(locsenchar, length+1, Sentbuf);
			for( ; i < length; ){
				if(!(cnt++ < MAX_CLAUSE_LENGTH)){
					break;
				}
				if(locsenchar[i++] < 0 ){
					i++;
				}	
			}
			locsenchar[i] = 0;
			Generate_Segmentation_Matrix(locsenchar, pmWordSet, TokenCS_v, MAX_CLAUSE_LENGTH, SegMatrix, mLength);
			delete locsenchar;
		}
		else if(!strcmp(cSegmentType, "BMM")){
			size_t locBeg = NLPOP::Get_Chinese_Sentence_Length_Counter(Sentbuf)-MAX_CLAUSE_LENGTH;
			for( ; i < length; ){
				if(!(cnt++ < locBeg)){
					break;
				}
				if(Sentbuf[i++] < 0 ){
					i++;
				}				
			}
			Generate_Segmentation_Matrix(Sentbuf+i*sizeof(char), pmWordSet, TokenCS_v, MAX_CLAUSE_LENGTH, SegMatrix, mLength);
		}
	}
	else{
		Generate_Segmentation_Matrix(Sentbuf, pmWordSet, TokenCS_v, MAX_CLAUSE_LENGTH, SegMatrix, mLength);
	}
	if(!strcmp(cSegmentType, "FMM")){
		Forword_Max_Maximum_Matching_Segmentation(SegMatrix, mLength, segInt_v);
	}
	else if(!strcmp(cSegmentType, "BMM")){
		Backword_Max_Maximum_Matching_Segmentation(SegMatrix, mLength, segInt_v);
	}

	Generate_Segmentation_Pathn(segInt_v, TokenCS_v, segment_v);
}


void CMaxmatch::Generate_Segmentation_Matrix(const char* SentBuf, set<string>& locCSWords_Set, vector<string>& TokenCS_v, unsigned int Max_Word_Length, bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int& mLength)
{
	char sChar[3];
	int strLen = strlen(SentBuf);
	int rowtokenlength = 0;
	int coltokenlength = 0;
	string wordstr;
	sChar[2]=0;
	for(int forposit = 0; forposit < strLen; ){
		wordstr = "";
		for(int bacposit = 0; ((forposit + bacposit) < strLen) && (bacposit < (int)Max_Word_Length); ){
			sChar[0]=SentBuf[forposit + bacposit++];
			sChar[1]=0;	
			if(sChar[0] < 0 ){
				sChar[1]=SentBuf[forposit + bacposit++];
			}
			wordstr += sChar;
			if(locCSWords_Set.find(wordstr) != locCSWords_Set.end()){
				SegMatrix[rowtokenlength][coltokenlength] = true;;
			}
			coltokenlength++;
		}
		sChar[0]=SentBuf[forposit++];
		sChar[1]=0;	
		if(sChar[0] < 0 ){
			sChar[1]=SentBuf[forposit++];
		}
		TokenCS_v.push_back(sChar);
		rowtokenlength++;
		coltokenlength = rowtokenlength;
	}
	mLength = rowtokenlength;
}

void CMaxmatch::Forword_Max_Maximum_Matching_Segmentation(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<int>& Seg_v)
{
	for(int row = 0; row < (int)mLength; ){
		int col = mLength - 1;
		for( ; col >= row; col--){
			if(SegMatrix[row][col]){
				Seg_v.push_back(col);
				break;
			}
			else if(col == row){
				Seg_v.push_back(col);
				break;
			}
		}
		row = col + 1;
	}
}

void CMaxmatch::Backword_Max_Maximum_Matching_Segmentation(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<int>& Seg_v)
{
	vector<int> rSeg_v;
	for(int col = mLength - 1 ; col >= 0; ){
		int row;
		for(row = 0; row < (int)mLength; row++){
			if(SegMatrix[row][col]){
				rSeg_v.push_back(col);
				break;
			}
			else if(col == row){
				rSeg_v.push_back(col);
				break;
			}
		}
		col = row - 1;
	}
	for(unsigned int i = 0; i < rSeg_v.size(); i++){
		Seg_v.push_back(rSeg_v[rSeg_v.size()-1-i]);
	}
}

void CMaxmatch::Generate_Segmentation_Pathn(vector<int>& pmSeg_v, vector<string>& pmTokenCS, vector<string>& pmSegment_v)
{
	unsigned int startpoint = 0;
	unsigned int backpoint = 0;
	char sentencechar[MAX_CLAUSE_LENGTH];
	
	for(unsigned int i = 0; i < pmSeg_v.size(); i++){
		strcpy_s(sentencechar, MAX_CLAUSE_LENGTH, "");
		backpoint = pmSeg_v[i];
		for(unsigned int j = startpoint; j <= backpoint; j++){
			strcat_s(sentencechar, MAX_CLAUSE_LENGTH, pmTokenCS[j].c_str());
		}
		pmSegment_v.push_back(sentencechar);
		startpoint = backpoint;
		startpoint++;
	}
}



