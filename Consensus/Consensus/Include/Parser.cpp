
#include "stdafx.h"
#include <algorithm>
#include "Parser.h"
#include "ConvertUTF.h"
//#include "ECOR.h"
#include "..\\Consensus.h"

CParser::CParser()
{
	m_OddCharMap.insert(make_pair("-LRB-", "("));
	m_OddCharMap.insert(make_pair("-RRB-", ")"));
	m_OddCharMap.insert(make_pair("-LCB-", "{"));
	m_OddCharMap.insert(make_pair("-RCB-", "}"));
	m_OddCharMap.insert(make_pair("-LSB-", "["));
	m_OddCharMap.insert(make_pair("-RSB-", "]"));
	m_OddCharMap.insert(make_pair("``", "\""));
	m_OddCharMap.insert(make_pair("''", "\""));
	m_OddCharMap.insert(make_pair("`", ""));

}
CParser::~CParser()
{
}

//---------------Name Space PARS
void PARS::PsDocs_Information_Checking(map<string, ACE_sgm>& pm_sgmmap, map<string, _PsDocInfo*>& PsDocsInfo_map)
{

	for(map<string, _PsDocInfo*>::iterator testinfoite = PsDocsInfo_map.begin();  testinfoite != PsDocsInfo_map.end(); testinfoite++){
		_PsDocInfo& loc_PsDocInfo = *testinfoite->second;
		if(pm_sgmmap.find(testinfoite->first) == pm_sgmmap.end()){
			AppCall::Secretary_Message_Box("Data Error in: PARS::PsDocs_Information_Checking(1)");
			continue;
		}
		ACE_sgm& loc_ACE_sgm = pm_sgmmap[testinfoite->first];
		const char* DOCBuf = loc_ACE_sgm.DOC.c_str();

		vector<vector<pair<size_t, size_t>>*>& WordsIndex_vv = loc_PsDocInfo.WordsIndex_vv;
		vector<vector<string>*>& Sentences_vv = loc_PsDocInfo.Sentences_vv;
		
		for(size_t i = 0; i < WordsIndex_vv.size(); i++){
			size_t RevisIndex = 0;
			for(size_t j = 0; j+RevisIndex < WordsIndex_vv[i]->size(); j++){
				pair<size_t, size_t>& loc_pair = (*WordsIndex_vv[i])[j];
				while((*Sentences_vv[i])[j+RevisIndex].length() == 0){
					RevisIndex++;
				}
				string teststr1 = Sentop::Get_Substr_by_Chinese_Character_Cnt(DOCBuf, loc_pair.first, loc_pair.second-loc_pair.first+1);
				string teststr2 = (*Sentences_vv[i])[j+RevisIndex].c_str();
				if(strcmp(Sentop::Get_Substr_by_Chinese_Character_Cnt(DOCBuf, loc_pair.first, loc_pair.second-loc_pair.first+1).c_str(), (*Sentences_vv[i])[j+RevisIndex].c_str())){
					ostringstream ostream;
					ostream << "DOC string: " << teststr1 << endl;
					ostream << "Check string: " << teststr2 << endl;
					AppCall::Secretary_Message_Box(ostream.str().c_str());
				}
			}
		}
	}
}

void PARS::PsDocsInfo_map_to_PsWordInfo_Mmap(map<string, _PsDocInfo*>& pmPsDocsInfo_map)
{
	for(map<string, _PsDocInfo*>::iterator psdocite = pmPsDocsInfo_map.begin(); psdocite != pmPsDocsInfo_map.end(); psdocite++){
		
		_PsDocInfo& loc_PsDocInfo = *psdocite->second;
		vector<vector<pair<size_t, size_t>>*>& WordsIndex_vv = loc_PsDocInfo.WordsIndex_vv;
		vector<vector<string>*>& Sentences_vv = loc_PsDocInfo.Sentences_vv;
		vector<vector<string>*>& POS_vv = loc_PsDocInfo.POS_vv;
		vector<vector<DTriple*>*>& Dependency_vv = loc_PsDocInfo.Dependency_vv;
		
		for(size_t SentenceID = 0; SentenceID < Sentences_vv.size(); SentenceID++){
			vector<string>& Words_v = *Sentences_vv[SentenceID];
			vector<string>& POS_v = *POS_vv[SentenceID];
			vector<pair<size_t, size_t>>& WordIndex_v = *WordsIndex_vv[SentenceID];
			vector<DTriple*>& Dependency_v = *Dependency_vv[SentenceID];
			
			map<size_t, map<size_t, _PsWordInfo*>>& PsWordInfo_m = psdocite->second->PsWordInfo_mmap;
			map<size_t, multimap<size_t, DTriple*>> LocHeadedDep_mm;
			map<size_t, multimap<size_t, DTriple*>> LocTailedDep_mm;
			
			for(vector<DTriple*>::iterator vite = Dependency_v.begin(); vite != Dependency_v.end(); vite++){
				DTriple& loc_DTriple = **vite;
				if(LocHeadedDep_mm.find(loc_DTriple.arg1) == LocHeadedDep_mm.end()){
					LocHeadedDep_mm[loc_DTriple.arg1];
				}
				if(LocTailedDep_mm.find(loc_DTriple.arg2) == LocTailedDep_mm.end()){
					LocTailedDep_mm[loc_DTriple.arg2];
				}
				LocHeadedDep_mm[loc_DTriple.arg1].insert(make_pair(loc_DTriple.arg2, *vite));
				LocTailedDep_mm[loc_DTriple.arg2].insert(make_pair(loc_DTriple.arg1, *vite));
				
			}

			size_t ReviIndex = 0;
			for(size_t WordID = 0; WordID < Words_v.size(); WordID++){
				if(Words_v[WordID].length() == 0){
					ReviIndex++;
					continue;
				}
				pair<size_t, size_t>& coordpair = WordIndex_v[WordID-ReviIndex];
				if(PsWordInfo_m.find(coordpair.first) == PsWordInfo_m.end()){
					PsWordInfo_m[coordpair.first];
				}
				if(PsWordInfo_m[coordpair.first].find(coordpair.second) == PsWordInfo_m[coordpair.first].end()){
					PsWordInfo_m[coordpair.first][coordpair.second];
					PsWordInfo_m[coordpair.first][coordpair.second] = new _PsWordInfo;
				}
				else{
					AppCall::Secretary_Message_Box("Duplicated Data in: ECCOM::PsDocsInfo_map_to_PsWordInfo_Mmap()");
				}
				_PsWordInfo& loc_PsWordInfo = *PsWordInfo_m[coordpair.first][coordpair.second];
				loc_PsWordInfo.START = coordpair.first;
				loc_PsWordInfo.END = coordpair.second;
				loc_PsWordInfo.Wordstr = Words_v[WordID];
				loc_PsWordInfo.POS = POS_v[WordID];
				//loc_PsWordInfo.posit = WordID+1; //0 is used for ROOT in Stanford Paser;
				loc_PsWordInfo.SentenceID = SentenceID;
				loc_PsWordInfo.WordID = WordID;

				if(LocHeadedDep_mm.find(loc_PsWordInfo.WordID) !=  LocHeadedDep_mm.end()){
					multimap<size_t, DTriple*>& refHeaded_m = LocHeadedDep_mm[loc_PsWordInfo.WordID];
					for(multimap<size_t, DTriple*>::iterator mite = refHeaded_m.begin(); mite != refHeaded_m.end(); mite++){
						loc_PsWordInfo.HeadedTriple_mm.insert(make_pair(mite->first, mite->second));
					}
				}

				if(LocTailedDep_mm.find(loc_PsWordInfo.WordID) != LocTailedDep_mm.end()){
					multimap<size_t, DTriple*>& refTailed_m = LocTailedDep_mm[loc_PsWordInfo.WordID];
					for(multimap<size_t, DTriple*>::iterator mite = refTailed_m.begin(); mite != refTailed_m.end(); mite++){
						loc_PsWordInfo.TailedTriple_mm.insert(make_pair(mite->first, mite->second));
					}
				}

				
				/*if(LocHeadedDep_mm.find(loc_PsWordInfo.posit) !=  LocHeadedDep_mm.end()){
					multimap<size_t, DTriple*>& refHeaded_m = LocHeadedDep_mm[loc_PsWordInfo.posit];
					for(multimap<size_t, DTriple*>::iterator mite = refHeaded_m.begin(); mite != refHeaded_m.end(); mite++){
						loc_PsWordInfo.HeadedTriple_mm.insert(make_pair(mite->first, mite->second));
					}
				}
				if(LocTailedDep_mm.find(loc_PsWordInfo.posit) != LocTailedDep_mm.end()){
					multimap<size_t, DTriple*>& refTailed_m = LocTailedDep_mm[loc_PsWordInfo.posit];
					for(multimap<size_t, DTriple*>::iterator mite = refTailed_m.begin(); mite != refTailed_m.end(); mite++){
						loc_PsWordInfo.TailedTriple_mm.insert(make_pair(mite->first, mite->second));
					}
				}*/
			}
		}
	}
}

void PARS::PsDocsInfo_map_to_PsSentInfo_Mmap_by_PsWordInfo_Mmap(map<string, _PsDocInfo*>& pmPsDocsInfo_map)
{
	for(map<string, _PsDocInfo*>::iterator psdocite = pmPsDocsInfo_map.begin(); psdocite != pmPsDocsInfo_map.end(); psdocite++){
		_PsDocInfo& loc_PsDocInfo = *psdocite->second;
		map<size_t, map<size_t, _PsSentInfo*>>& PsSentInfo_m = loc_PsDocInfo.PsSentInfo_mmap;
		map<size_t, map<size_t, _PsWordInfo*>>& PsWordInfo_mmap = loc_PsDocInfo.PsWordInfo_mmap;
		
		vector<vector<pair<size_t, size_t>>*>& WordsIndex_vv = loc_PsDocInfo.WordsIndex_vv;
		vector<vector<string>*>& Sentences_vv = loc_PsDocInfo.Sentences_vv;
		
		for(size_t SentenceID = 0; SentenceID < Sentences_vv.size(); SentenceID++){
			size_t SentSTART = WordsIndex_vv[SentenceID]->begin()->first;
			size_t SentEND = WordsIndex_vv[SentenceID]->rbegin()->second;

			if(PsSentInfo_m[SentSTART].find(SentEND) == PsSentInfo_m[SentSTART].end()){
				PsSentInfo_m[SentSTART].insert(make_pair(SentEND, new _PsSentInfo));
			}
			else{
				AppCall::Secretary_Message_Box("Data Error in: ECCOM::PsDocsInfo_map_to_PsSentInfo_Mmap_by_PsWordInfo_Mmap()");
			}
			_PsSentInfo& loc_PsSentInfo = *PsSentInfo_m[SentSTART][SentEND];
			loc_PsSentInfo.START = SentSTART;
			loc_PsSentInfo.END = SentEND;
			
			for(map<size_t, map<size_t, _PsWordInfo*>>::iterator mmite = PsWordInfo_mmap.begin(); mmite != PsWordInfo_mmap.end(); mmite++){
				for(map<size_t, _PsWordInfo*>::iterator mite = mmite->second.begin(); mite != mmite->second.end(); mite++){
					
					_PsWordInfo& locWordInfo = *mite->second;
					if(locWordInfo.START >= SentSTART && locWordInfo.END <=	SentEND){
						if(loc_PsSentInfo.SentencePsWord_mm.find(locWordInfo.START) != loc_PsSentInfo.SentencePsWord_mm.end()){
							AppCall::Secretary_Message_Box("Data Error in: ECCOM::PsDocsInfo_map_to_PsSentInfo_Mmap_by_PsWordInfo_Mmap(1)");
						}
						loc_PsSentInfo.SentencePsWord_mm[locWordInfo.START];
						if(loc_PsSentInfo.SentencePsWord_mm[locWordInfo.START].find(locWordInfo.END) != loc_PsSentInfo.SentencePsWord_mm[locWordInfo.START].end()){
							AppCall::Secretary_Message_Box("Data Error in: ECCOM::PsDocsInfo_map_to_PsSentInfo_Mmap_by_PsWordInfo_Mmap(2)");
						}
						loc_PsSentInfo.SentencePsWord_mm[locWordInfo.START].insert(make_pair(locWordInfo.END, mite->second));
					}
					//if(loc_PsSentInfo.PositPsWord_m.find(locWordInfo.posit) != loc_PsSentInfo.PositPsWord_m.end()){
					//	AppCall::Secretary_Message_Box("Data Error in: ECCOM::PsDocsInfo_map_to_PsSentInfo_Mmap_by_PsWordInfo_Mmap(3)");
					//}
					//loc_PsSentInfo.PositPsWord_m.insert(make_pair(locWordInfo.posit, mite->second));
				}
			}
		}
	}
}

void PARS::PsDocsInfo_map_to_PsSentInfo_Mmap(map<string, _PsDocInfo*>& pmPsDocsInfo_map)
{
	/*for(map<string, _PsDocInfo*>::iterator psdocite = pmPsDocsInfo_map.begin(); psdocite != pmPsDocsInfo_map.end(); psdocite++){
		map<size_t, map<size_t, _PsSentInfo*>>& PsSentInfo_m = psdocite->second->PsSentInfo_mmap;
		_PsDocInfo& loc_PsDocInfo = *psdocite->second;
		vector<vector<pair<size_t, size_t>>*>& WordsIndex_vv = loc_PsDocInfo.WordsIndex_vv;
		vector<vector<string>*>& Sentences_vv = loc_PsDocInfo.Sentences_vv;
		vector<vector<string>*>& POS_vv = loc_PsDocInfo.POS_vv;
		vector<vector<DTriple*>*>& Dependency_vv = loc_PsDocInfo.Dependency_vv;
		
		for(size_t SentenceID = 0; SentenceID < Sentences_vv.size(); SentenceID++){
			size_t SentSTART = WordsIndex_vv[SentenceID]->begin()->first;
			size_t SentEND = WordsIndex_vv[SentenceID]->rbegin()->second;
			if(PsSentInfo_m.find(SentSTART) == PsSentInfo_m.end()){
				PsSentInfo_m[SentSTART];
			}
			if(PsSentInfo_m[SentSTART].find(SentEND) == PsSentInfo_m[SentSTART].end()){
				PsSentInfo_m[SentSTART].insert(make_pair(SentEND, new _PsSentInfo));
			}
			else{
				AppCall::Secretary_Message_Box("Data Error in: ECCOM::PsDocsInfo_map_to_PsSentInfo_Mmap()");
			}
			_PsSentInfo& loc_PsSentInfo = *PsSentInfo_m[SentSTART][SentEND];
			loc_PsSentInfo.START = SentSTART;
			loc_PsSentInfo.END = SentEND;
			loc_PsSentInfo.pWordsIndex_v = WordsIndex_vv[SentenceID];
			loc_PsSentInfo.pSentence_v = Sentences_vv[SentenceID];
			loc_PsSentInfo.pDependency_v = Dependency_vv[SentenceID];
			vector<string>& Sentence_v = *Sentences_vv[SentenceID];
			//for(size_t i = 0; i < Sentence_v.size(); i++){
			//	loc_PsSentInfo.Sentstr += Sentence_v[i];
			//}
		}
	}*/
}


//===========================Parsing Port
void CParser::Stanford_Parsing_Port(ACE_Corpus& pmACE_Corpus, const char* cCoupusPath)
{

	string ParserFolder = STANFORD_PARSER_FOLDER;
	string ACE_E = cCoupusPath;
	ACE_E += "Parsing\\"; 
	string Parsed = cCoupusPath;
	Parsed += "Parsed\\";
	NLPOP::Delete_Dir_and_Grate(ACE_E);
	NLPOP::Delete_Dir_and_Grate(Parsed);
	string JavaSpace = JAVA_SPACE;

	map<string, _PsDocInfo*> trainsgm_map;
	Replacing_TEXT_with_Entity_Mention_and_Save(ACE_E.c_str(), pmACE_Corpus);

	ostringstream outputFilestream;
	ostringstream inputFilestream;
	ostringstream parameterstream;

	parameterstream << "java -jar " << "Stanford_Parser.jar" << " " << ACE_E << " " << Parsed << " " << JavaSpace;
	outputFilestream << cCoupusPath << "parsed_output.txt";
	AppCall::Console_Running_with_Output_Redirection(ParserFolder.c_str(), parameterstream.str().c_str(), outputFilestream.str().c_str());

	AppCall::Consensus_Open_Process(outputFilestream.str().c_str(), Notepad_Path);
}

void CParser::Replacing_TEXT_with_Entity_Mention_and_Save(const char* savefolder, ACE_Corpus& pmACE_Corpus)
{
	map<string, vector<ACE_entity_mention*>> EntityMentionp_mv;
	for(map<string, ACE_entity>::iterator mmite = pmACE_Corpus.ACE_Entity_Info_map.begin(); mmite != pmACE_Corpus.ACE_Entity_Info_map.end(); mmite++){
		vector<ACE_entity_mention>& loc_mention_v = mmite->second.entity_mention_v;
		for(size_t i = 0; i < loc_mention_v.size(); i++){
			if(EntityMentionp_mv.find(loc_mention_v[i].DOCID) == EntityMentionp_mv.end()){
				EntityMentionp_mv[loc_mention_v[i].DOCID];
			}
			EntityMentionp_mv[loc_mention_v[i].DOCID].push_back(&loc_mention_v[i]);
		}
	}
	for(map<string, ACE_sgm>::iterator mite = pmACE_Corpus.ACE_sgm_mmap.begin(); mite != pmACE_Corpus.ACE_sgm_mmap.end(); mite++){

		int TEXTposit = mite->second.DOC.find(mite->second.TEXT.c_str());
		if(TEXTposit == string::npos){
			AppCall::Secretary_Message_Box("TEXT substring position is wrong...", MB_OK);
		}
		size_t oldcharcnt = 0;
		size_t newcharcnt = 0;
		for(int i = 0; i < TEXTposit; ){
			if(mite->second.DOC.c_str()[i++] < 0){
				i++;
			}
			oldcharcnt++;
		}
		if(EntityMentionp_mv.find(mite->first) == EntityMentionp_mv.end()){
			AppCall::Secretary_Message_Box("Data Error...", MB_OK);
		}
		map<size_t, map<size_t, ACE_entity_mention*>> EntityMention_mm;
		ace_op::Get_Entity_Mention_extent_Map(EntityMentionp_mv[mite->first], EntityMention_mm);

		vector<string> Replace_v;
		//vector<pair<size_t, size_t>>* pCoupledIndex = new vector<pair<size_t, size_t>>;
		//ExtnedReplacedTEXT_map.insert(make_pair(mite->first, pReplace_v));
		//ExtnedReplacedIndex_map.insert(make_pair(mite->first, pCoupledIndex));
		int DBUF_LIMIT = mite->second.DOC.size();
		const char* DOCBuf = mite->second.DOC.c_str();
		string sReplaced;
		
		//size_t testpoint = 0;
		for(int i = TEXTposit; i < DBUF_LIMIT; ){
			if(EntityMention_mm.find(i) != EntityMention_mm.end()){
				if(sReplaced.length() != 0){
					Replace_v.push_back(sReplaced);
					sReplaced = "";
				}
				ACE_extent& loc_extend = EntityMention_mm[i].rbegin()->second->extent;
				if(loc_extend.START != oldcharcnt){
					ostringstream ostrsteam;
					ostrsteam << "Document ID: " << mite->first << endl;
					AppCall::Secretary_Message_Box(ostrsteam.str().c_str());
				}
				const char* testchar = DOCBuf+i;
				if(!NLPOP::prix_substring_matching(testchar, loc_extend.charseq.c_str())){
					AppCall::Secretary_Message_Box("Unequral extend START in: CParser::Replacing_TEXT_with_Entity_Mention...2", MB_OK);
				}

				sReplaced = EntityMention_mm[i].rbegin()->second->Entity_TYPE;
				oldcharcnt += NLPOP::Get_Chinese_Sentence_Length_Counter(loc_extend.charseq.c_str());
				
				newcharcnt += NLPOP::Get_Chinese_Sentence_Length_Counter(sReplaced.c_str());
				//pCoupledIndex->push_back(make_pair(oldcharcnt, newcharcnt));
				i += loc_extend.charseq.length();
			}
			else{
				sReplaced += DOCBuf[i];
				if(DOCBuf[i++] < 0){
					i++;
					sReplaced += DOCBuf[i];
				}
				oldcharcnt++;
				newcharcnt++;
			}
		}
		if(sReplaced.length() != 0){
			Replace_v.push_back(sReplaced);
			sReplaced = "";
		}
		string FilePath = savefolder + mite->second.DOCID;
		FilePath += ".sgm";
		ofstream out(FilePath.c_str());
		if(out.bad())
			return;
		out.clear();
		out.seekp(0, ios::beg);
		for(vector<string>::iterator vite = Replace_v.begin(); vite != Replace_v.end(); vite++){
			out << vite->data();
		}
		out.close();
	}
}

//===========================Generating Parsed Info Port
void CParser::Generating_Parsed_DOC_Info_Port(ACE_Corpus& pmACE_Corpus, const char* cParsInfoPath, map<string, _PsDocInfo*>& PsDocsInfo_map)
{
	AppCall::Maxen_Responce_Message("1: Begining CParser::Get_ACE_TEXT_From_DOC()\n\n");

	Get_ACE_TEXT_From_DOC(pmACE_Corpus.ACE_sgm_mmap, PsDocsInfo_map);

	string coupusname = cParsInfoPath;
	vector<string> FilesRoot_v;
	NLPOP::Get_Specified_Files(coupusname + "Parsing\\", FilesRoot_v, ".sgm");

	for(size_t i = 0; i < FilesRoot_v.size(); i++){
		string DOCID = FilesRoot_v[i].substr(FilesRoot_v[i].rfind('\\')+1, FilesRoot_v[i].rfind('.')-FilesRoot_v[i].rfind('\\')-1);

#ifdef ECOR_DATA_CHECKING
		if(PsDocsInfo_map.find(DOCID) == PsDocsInfo_map.end()){
			AppCall::Secretary_Message_Box("File is not exist in: CParser::Generating_Parsed_DOC_Info_Port...", MB_OK);
		}
#endif
		if(!NLPOP::Exist_of_This_File(FilesRoot_v[i].c_str())){
			AppCall::Secretary_Message_Box("Original File is not exist in: CParser::Generating_Parsed_DOC_Info_Port...", MB_OK);
		}
		CFile m_File;
		m_File.Open(NLPOP::string2CString(FilesRoot_v[i].c_str()), CFile::modeRead);
		m_File.SeekToBegin();
		PsDocsInfo_map[DOCID]->ReplaceTEXTBuf = new char[(unsigned long)m_File.GetLength()];
		m_File.Read((void*)(PsDocsInfo_map[DOCID]->ReplaceTEXTBuf), (unsigned long)m_File.GetLength()); 
		m_File.Close();
	}

	AppCall::Maxen_Responce_Message("2: Begining CParser::Load_Stanford_Pased_TEXT_Information()\n\n");

	FilesRoot_v.clear();
	NLPOP::Get_Specified_Files(coupusname + "Parsed\\", FilesRoot_v, ".sgm");
	for(size_t i = 0; i < FilesRoot_v.size(); i++){
		string DOCID = FilesRoot_v[i].substr(FilesRoot_v[i].rfind('\\')+1, FilesRoot_v[i].rfind('.')-FilesRoot_v[i].rfind('\\')-1);

		Load_Stanford_Pased_TEXT_Information(FilesRoot_v[i].c_str(), DOCID.c_str(), PsDocsInfo_map);
		
		if(PsDocsInfo_map.find(DOCID) != PsDocsInfo_map.end()){
			Segmented_Replace_TEXT_Sentence_Words_Index_Labeling(*PsDocsInfo_map[DOCID]);
		}
	}

	AppCall::Maxen_Responce_Message("3: Begining CParser::Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling()\n\n");

	Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling(pmACE_Corpus, PsDocsInfo_map);
}


void CParser::Get_ACE_TEXT_From_DOC(map<string, ACE_sgm>& pm_sgmmap, map<string, _PsDocInfo*>& PsDocsInfo_map)
{
	char sChar[3];
	sChar[2]=0;
	for(map<string, ACE_sgm>::iterator mite = pm_sgmmap.begin(); mite != pm_sgmmap.end(); mite++){
		int TEXTposit = mite->second.DOC.find(mite->second.TEXT.c_str());
		if(TEXTposit == string::npos){
			AppCall::Secretary_Message_Box("TEXT substring position is wrong...", MB_OK);
		}
		if(PsDocsInfo_map.find(mite->second.DOCID) == PsDocsInfo_map.end()){
			PsDocsInfo_map[mite->second.DOCID];
		}
		PsDocsInfo_map[mite->second.DOCID] =  new _PsDocInfo;
		PsDocsInfo_map[mite->second.DOCID]->Parsed_Info_Loaded_Flag = false;
		PsDocsInfo_map[mite->second.DOCID]->Conversed_Flag = false;
		_PsDocInfo* ploc_PsDocInfo = PsDocsInfo_map[mite->second.DOCID];
		ploc_PsDocInfo->DOCID = mite->second.DOCID;
		ploc_PsDocInfo->DOCBuf = mite->second.DOC.c_str();
		ploc_PsDocInfo->TEXTBuf = mite->second.TEXT.c_str();
		size_t CharCnt = 0;
		for(int i = 0; i < TEXTposit; ){
			if(mite->second.DOC.c_str()[i++] < 0){
				i++;
			}
			CharCnt++;
		}
		ploc_PsDocInfo->START = CharCnt;

		int limit = mite->second.DOC.size();
		for(int i = TEXTposit; i < limit; i++){
			if(mite->second.DOC.c_str()[i++] < 0){
				i++;
			}
			CharCnt++;
		}
		ploc_PsDocInfo->END = CharCnt;
	}
}

void CParser::Load_Stanford_Pased_TEXT_Information(const char* cpath, const char*  DOCID, map<string, _PsDocInfo*>& PsDocsInfo_map)
{
	if(!NLPOP::Exist_of_This_File(cpath)){
		AppCall::Secretary_Message_Box("打开文件出错：CExtrue::Load_Stanford_Pased_TEXT_Information(1)", MB_OK);
	}
	if(PsDocsInfo_map.find(DOCID) == PsDocsInfo_map.end()){
		AppCall::Secretary_Message_Box("TEXT Info is absence in: CExtrue::Load_Stanford_Pased_TEXT_Information...", MB_OK);
		return;
	}
	_PsDocInfo& Ref_locPsDocInfo = *PsDocsInfo_map[DOCID];

	char SentenceBuf[MAX_SENTENCE];
	char WordBuf[256];
	char POSBuf[256];
	char cDepend[256];
	char cArg1[256];
	char cArg2[256];
	int ID_Arg1;
	int ID_Arg2;
	char* prChar;
	ifstream in(cpath);
	if(in.bad()){
		AppCall::Secretary_Message_Box("打开文件出错: CExtrue::Load_Stanford_Pased_TEXT_Information(2)", MB_OK);
	}
	in.clear();
	in.seekg(0, ios::beg);

	while(true){
		if(in.peek() == EOF){	
			break;
		}
		in.getline(SentenceBuf, MAX_SENTENCE, '\n');
#ifdef ECOR_DATA_CHECKING		
		if(strlen(SentenceBuf) == 0){
			AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(3)", MB_OK);
		}
#endif
		istringstream sstream(SentenceBuf);

		vector<string>* pSentence_v = new vector<string>;
		vector<string>* pPOS_v = new vector<string>;
		vector<DTriple*>* pDependency_v = new vector<DTriple*>;

		Ref_locPsDocInfo.Sentences_vv.push_back(pSentence_v);
		Ref_locPsDocInfo.POS_vv.push_back(pPOS_v);
		Ref_locPsDocInfo.Dependency_vv.push_back(pDependency_v);

		vector<DTriple*>& Dependency_v = *pDependency_v;

		while(sstream.peek() != EOF){
			
			sstream.getline(SentenceBuf, 256, ' ');
			prChar = strrchr(SentenceBuf, '/');
			*prChar = 0; 
			strcpy_s(WordBuf, 256, SentenceBuf);
			prChar++;
			strcpy_s(POSBuf, 256, prChar);
			
#ifdef ECOR_DATA_CHECKING
			if(strlen(WordBuf) == 0 || strlen(POSBuf) == 0){
				ostringstream ostream;
				ostream << "Error Function: CParser::Load_Stanford_Pased_TEXT_Information..." << endl;
				ostream << "Error Document ID: " << DOCID << endl;
				ostream << SentenceBuf << endl;
				AppCall::Secretary_Message_Box(ostream.str().c_str());
			}
#endif
			pSentence_v->push_back(WordBuf);
			pPOS_v->push_back(POSBuf);
		}
		
		in.getline(SentenceBuf, MAX_SENTENCE, '\n');
		in.getline(SentenceBuf, MAX_SENTENCE, '\n');
#ifdef ECOR_DATA_CHECKING
		if(strlen(SentenceBuf) == 0){
			AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(5)", MB_OK);
		}
#endif
		istringstream dstream(SentenceBuf);
#ifdef ECOR_DATA_CHECKING
		if(dstream.peek() != '['){
			AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(6)", MB_OK);
		}
#endif
		dstream.getline(SentenceBuf, 256, '[');

		map<size_t, string> rMapDenpencyIndex_m; 

		while(dstream.peek() != ']'){

			dstream.getline(cDepend, 256, '(');

			dstream.getline(cArg1, 256, ' ');
			if(strlen(cArg1) < 2){
				AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(6-1)", MB_OK);
			}
			if(cArg1[strlen(cArg1)-1] != ','){
				AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(6-2)", MB_OK);
			}
			cArg1[strlen(cArg1)-1] = 0;
			ID_Arg1 = atoi(strrchr(cArg1, '-')+1);
			
			for(size_t i = strlen(cArg1)-1; i > 0; i--){
				if(cArg1[i] == '-'){
					cArg1[i] = 0;
					if(rMapDenpencyIndex_m.find(ID_Arg1) == rMapDenpencyIndex_m.end()){
						rMapDenpencyIndex_m.insert(make_pair(ID_Arg1, cArg1));
					}
					else if(strcmp(rMapDenpencyIndex_m[ID_Arg1].c_str(), cArg1)){
						AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(6-2-2)", MB_OK);
					}
					break;
				}
			}

			dstream.getline(cArg2, 256, ')');
			ID_Arg2 = atoi(strrchr(cArg2, '-')+1);

			for(size_t i = strlen(cArg2)-1; i > 0; i--){
				if(cArg2[i] == '-'){
					cArg2[i] = 0;
					if(rMapDenpencyIndex_m.find(ID_Arg2) == rMapDenpencyIndex_m.end()){
						rMapDenpencyIndex_m.insert(make_pair(ID_Arg2, cArg2));
					}
					else if(strcmp(rMapDenpencyIndex_m[ID_Arg2].c_str(), cArg2)){
						AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(6-2-2)", MB_OK);
					}
					break;
				}
			}
			if(strlen(cDepend) == 0){
				AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(7-0)", MB_OK);
			}
			if(0 == ID_Arg1 || 0 == ID_Arg2){
				continue;
			}
			DTriple* ploc_DTriple = new DTriple;
			ploc_DTriple->dependstr = cDepend;
			ploc_DTriple->arg1 = ID_Arg1;
			ploc_DTriple->arg2 = ID_Arg2;
			Dependency_v.push_back(ploc_DTriple);

			if(dstream.peek() == ']'){
				break;
			}
#ifdef ECOR_DATA_CHECKING
			if(dstream.peek() == EOF){
				AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(7)", MB_OK);
			}
#endif
			dstream.getline(SentenceBuf, 256, ' ');
		}
		dstream.getline(SentenceBuf, 256, ']');
#ifdef ECOR_DATA_CHECKING
		if(dstream.peek() != EOF){
			AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(8)", MB_OK);
		}
#endif
		in.getline(SentenceBuf, MAX_SENTENCE, '\n');

		 rMapDenpencyIndex_m; pSentence_v;Dependency_v;
		 size_t WordID = 0;
		 map<size_t, size_t> locmap_m;
		 for(map<size_t, string>::iterator mite = rMapDenpencyIndex_m.begin(); mite != rMapDenpencyIndex_m.end(); mite++){
			 if(mite->first == 0){
				continue;
			 }
			 bool Matched_Flag = false;
			 for(; WordID < pSentence_v->size(); WordID++){
				 if(!strcmp((*pSentence_v)[WordID].c_str(), mite->second.c_str())){
					 locmap_m.insert(make_pair(mite->first, WordID));
					 Matched_Flag = true;
					 WordID++;
					 break;
				 }
			 }
			 if(!Matched_Flag){
				 AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(7-0-1)", MB_OK);
			 }
		 }
			
		 for(size_t i = 0; i < Dependency_v.size(); i++){
			 DTriple& locDTriple = *Dependency_v[i];
			 if(locmap_m.find(locDTriple.arg1) == locmap_m.end()){
				 AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(7-0-2)", MB_OK);
			 }
			 locDTriple.arg1 = locmap_m[locDTriple.arg1];
			 if(locmap_m.find(locDTriple.arg2) == locmap_m.end()){
				 AppCall::Secretary_Message_Box("Data Error: CExtrue::Load_Stanford_Pased_TEXT_Information(7-0-3)", MB_OK);
			 }
			 locDTriple.arg2 = locmap_m[locDTriple.arg2];
		 }
	}
	in.close();
	Ref_locPsDocInfo.Parsed_Info_Loaded_Flag = true;
}

void CParser::Segmented_Replace_TEXT_Sentence_Words_Index_Labeling(_PsDocInfo& pmPsDocInfo)
{
	if(!pmPsDocInfo.Parsed_Info_Loaded_Flag){
		AppCall::Secretary_Message_Box("Syntax info should be loaded before...", MB_OK);
	}
	vector<vector<pair<size_t, size_t>>*>& WordsIndex_vv = pmPsDocInfo.WordsIndex_vv;
	vector<vector<string>*>& Sentences_vv = pmPsDocInfo.Sentences_vv;

	size_t BUF_LIMIT = strlen(pmPsDocInfo.ReplaceTEXTBuf);
	const char* TEXTBuf = pmPsDocInfo.ReplaceTEXTBuf;
	size_t DBuf_Index = 0;
	size_t TEXT_CharCnt = pmPsDocInfo.START;
	for(size_t SentenceID = 0; SentenceID < Sentences_vv.size(); SentenceID++){
		vector<pair<size_t, size_t>>* pLoc_WordsIndex_v = new vector<pair<size_t, size_t>>;
		vector<string>& Words_v = *Sentences_vv[SentenceID];
		WordsIndex_vv.push_back(pLoc_WordsIndex_v);

		//:YPench
		//if(24 == SentenceID){
		//	int viewpoint = 0;
		//}
		for(size_t WordID = 0; WordID < Words_v.size(); WordID++){
			const char* WrodBuf = Words_v[WordID].c_str();
			size_t WBuf_Index = 0;
			size_t Word_START;
			size_t Word_END;
			size_t WORD_LIMIT = Words_v[WordID].size();
			bool Word_Matching_Flag = false;
			for(; ((DBuf_Index < BUF_LIMIT) && (WBuf_Index < WORD_LIMIT)); ){
				
				//:YPench
				//string tests = &TEXTBuf[DBuf_Index];
				//if(24 == WordID){
				//	int viewpoint = 0;
				//}
				if(TEXTBuf[DBuf_Index] == '\r'){
					DBuf_Index++;
					continue;
				}
				if(-62 == WrodBuf[WBuf_Index]){//0xC2A0 is used in standford instead '_';
					if((TEXTBuf[DBuf_Index] == ' ') && (-96 == WrodBuf[WBuf_Index+1])){
						DBuf_Index++;
						TEXT_CharCnt++;
						WBuf_Index += 2;
						continue;
					}
				}
				if((TEXTBuf[DBuf_Index] == ' ') || (TEXTBuf[DBuf_Index] == '\n') || (TEXTBuf[DBuf_Index] == '\t')){
					DBuf_Index++;
					TEXT_CharCnt++;
					continue;
				}
				if(!strcmp(WrodBuf, "...") || !strcmp(WrodBuf, "--")){//stanford paser use "..." instead "...."
					Word_START = TEXT_CharCnt;
					Words_v[WordID] = "";
					while((TEXTBuf[DBuf_Index] == '.') || (TEXTBuf[DBuf_Index] == ' ') || (TEXTBuf[DBuf_Index] == '-')){
						Word_Matching_Flag = true;
						Words_v[WordID] += TEXTBuf[DBuf_Index];
						DBuf_Index++;
						TEXT_CharCnt++;
					}
					WBuf_Index = WORD_LIMIT;
					break;
				}
				
				if(TEXTBuf[DBuf_Index] == WrodBuf[WBuf_Index]){
					if(!Word_Matching_Flag){
						Word_Matching_Flag = true;
						Word_START = TEXT_CharCnt;
					}
#ifdef ECOR_DATA_CHECKING
					if(TEXTBuf[DBuf_Index] < 0){
						DBuf_Index++;
						WBuf_Index++;
						AppCall::Secretary_Message_Box("Worring: non English character is hit...", MB_OK);
					}
#endif
					DBuf_Index++;
					WBuf_Index++;
					TEXT_CharCnt++;
				}
				else{
					if(!strcmp(&WrodBuf[WBuf_Index], "-LRB-") || !strcmp(&WrodBuf[WBuf_Index], "-RRB-") || !strcmp(&WrodBuf[WBuf_Index], "``") || !strcmp(&WrodBuf[WBuf_Index], "''")
						|| !strcmp(&WrodBuf[WBuf_Index], "`") || !strcmp(&WrodBuf[WBuf_Index], "-LCB-") || !strcmp(&WrodBuf[WBuf_Index], "-LSB-") || !strcmp(&WrodBuf[WBuf_Index], "-RSB-")
						){
						Word_Matching_Flag = true;
						WBuf_Index = WORD_LIMIT;
						//if(TEXTBuf[DBuf_Index++] < 0){
							DBuf_Index++;
							//AppCall::Secretary_Message_Box("Worring: non English brackets is hit...", MB_OK);
						//}
						TEXT_CharCnt++;
						Word_START = TEXT_CharCnt;
						break;
					}
					else if(!strcmp(WrodBuf, ".")){// a point is added by stanford such as pa.
						size_t i = 1;
						while((TEXTBuf[DBuf_Index-i] == ' ') || (TEXTBuf[DBuf_Index-i] == '\n') || (TEXTBuf[DBuf_Index-i] == '\r') 
							|| (TEXTBuf[DBuf_Index-i] == '\t')){
							i++;
						}
						//:YPench
						//string teststr = &TEXTBuf[DBuf_Index-i];
						if(TEXTBuf[DBuf_Index-i] == '.'){
							Words_v[WordID] = "";
							WBuf_Index = WORD_LIMIT;
							break;
						}
						AppCall::Secretary_Message_Box("if(!strcmp(WrodBuf, \".\"): Segmented_Replace_TEXT_Sentence_Words_Index_Labeling(2-1)", MB_OK);
					}
					else if(!strcmp(WrodBuf, "-")){
						if(TEXTBuf[DBuf_Index-1] == '-'){
							Words_v[WordID] = "";
							Word_START = TEXT_CharCnt;
							WBuf_Index = WORD_LIMIT;
							TEXT_CharCnt++;
							Word_Matching_Flag = true;
							break;
						}
						AppCall::Secretary_Message_Box("!strcmp(WrodBuf, \"-\"): Segmented_Replace_TEXT_Sentence_Words_Index_Labeling(2-1)", MB_OK);
					}
#ifdef ECOR_DATA_CHECKING
					else{
						ostringstream ostream;
						ostream << "Error Function: CParser::Segmented_Replace_TEXT_Sentence_Words_Index_Labeling..." << endl;
						ostream << "Document ID: " << pmPsDocInfo.DOCID << endl;
						ostream << "TEXT characters: " << TEXTBuf[DBuf_Index] << endl;
						ostream << "Unexpected characters: " << WrodBuf << endl;
						ostream << "Sentence: ";
						for(vector<string>::iterator wordsvite = Words_v.begin(); wordsvite != Words_v.end(); wordsvite++){
							ostream << wordsvite->data() << " ";
						}
						ostream << endl;
						AppCall::Secretary_Message_Box(ostream.str().c_str(), MB_OK);
						WBuf_Index += WORD_LIMIT;
						if(TEXTBuf[DBuf_Index++] < 0){
							DBuf_Index++;
						}
						break;
					}
					if(Word_Matching_Flag){
						AppCall::Secretary_Message_Box("Data Error: Segmented_Replace_TEXT_Sentence_Words_Index_Labeling(2)", MB_OK);
					}
#endif
				}
			}
			if(Word_Matching_Flag && (WBuf_Index == WORD_LIMIT)){
				Word_END = TEXT_CharCnt-1;
				pLoc_WordsIndex_v->push_back(make_pair(Word_START, Word_END));
			}
#ifdef ECOR_DATA_CHECKING
			else if(WBuf_Index != WORD_LIMIT){
				AppCall::Secretary_Message_Box("Data Error: Segmented_Replace_TEXT_Sentence_Words_Index_Labeling(3)", MB_OK);
			}
#endif
		}
	}
}


void CParser::Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling(ACE_Corpus& pmACE_Corpus, map<string, _PsDocInfo*>& PsDocsInfo_map)
{
	map<string, vector<ACE_entity_mention*>> EntityMentionp_mv;
	for(map<string, ACE_entity>::iterator mmite = pmACE_Corpus.ACE_Entity_Info_map.begin(); mmite != pmACE_Corpus.ACE_Entity_Info_map.end(); mmite++){
		vector<ACE_entity_mention>& loc_mention_v = mmite->second.entity_mention_v;
		for(size_t i = 0; i < loc_mention_v.size(); i++){
			if(EntityMentionp_mv.find(loc_mention_v[i].DOCID) == EntityMentionp_mv.end()){
				EntityMentionp_mv[loc_mention_v[i].DOCID];
			}
			EntityMentionp_mv[loc_mention_v[i].DOCID].push_back(&loc_mention_v[i]);
		}
	}
	for(map<string, _PsDocInfo*>::iterator mite = PsDocsInfo_map.begin(); mite != PsDocsInfo_map.end(); mite++){
		//if(!strcmp(mite->first.c_str(), "rec.music.phish_20041215.1554")){
		//	AppCall::Secretary_Message_Box("");
		//	continue;
		//}
		_PsDocInfo& loc_PsDocInfo = *mite->second;
		size_t doccnt = 0;
		const char* DocBuf = mite->second->DOCBuf;
		const char* matchposit = strstr(DocBuf, mite->second->TEXTBuf);
		 //= mite->second->DOC.find(mite->second->TEXT.c_str());
#ifdef ECOR_DATA_CHECKING
		if(matchposit == NULL){
			AppCall::Secretary_Message_Box("TEXT substring position is wrong...", MB_OK);
		}
#endif
		size_t TEXTposit = matchposit-DocBuf;
		size_t DOC_LIMIT = strlen(DocBuf);
		size_t DBUF_Index = 0;
		for(; DBUF_Index < TEXTposit; ){
			if(DocBuf[DBUF_Index++] < 0){
				DBUF_Index++;
			}
			doccnt++;
		}
		map<size_t, map<size_t, ACE_entity_mention*>> EntityMention_mm;
		ace_op::Get_Entity_Mention_extent_Map(EntityMentionp_mv[mite->first], EntityMention_mm);
		vector<vector<pair<size_t, size_t>>*>& WordsIndex_vv = mite->second->WordsIndex_vv;
		vector<vector<string>*>& Sentences_vv = mite->second->Sentences_vv;
		map<string, map<size_t, pair<size_t, string>>*>& MentionSegmentedTEXT_mm = mite->second->MentionSegmentedTEXT_mm;
		map<size_t, pair<size_t, string>>* pLocSegmentedTEXT = new map<size_t, pair<size_t, string>>;
		MentionSegmentedTEXT_mm.insert(make_pair(mite->first, pLocSegmentedTEXT));

		size_t DOC_START = doccnt;
		size_t DOC_END = doccnt;
		char SentBuf[MAX_SENTENCE];
		char CharBuf[2];
		char ReplaceWordBuf[MAX_SENTENCE];
		CharBuf[1] = 0;
		strcpy_s(SentBuf, MAX_SENTENCE, "");
		
		for(size_t SentenceID = 0; SentenceID < Sentences_vv.size(); SentenceID++){
			vector<string>& Words_v = *Sentences_vv[SentenceID];
			vector<pair<size_t, size_t>>& WordsIndex_v = *WordsIndex_vv[SentenceID];
			int RevisedID = 0;
			//:YPench
			//if(9 == SentenceID){
			//	int viewpoint = 0;
			//}
			for(size_t WordID = 0; WordID < Words_v.size(); WordID++){
				if(Words_v[WordID].length() == 0){
					RevisedID++;
					continue;
				}
				//if(62 == WordID){
				//	int viewpoint = 0;
				//}
				const char* WordBuf = Words_v[WordID].c_str();
				size_t WORD_LIMIT = Words_v[WordID].size();
				size_t wordcnt = 0;
				size_t WBUF_Index = 0;
				strcpy_s(ReplaceWordBuf, MAX_SENTENCE, "");
				while((DocBuf[DBUF_Index] == ' ') || (DocBuf[DBUF_Index] == '\n') || (DocBuf[DBUF_Index] == '\t')){
					CharBuf[0] = DocBuf[DBUF_Index];
					strcat_s(SentBuf, MAX_SENTENCE, CharBuf);
					DBUF_Index++;
					doccnt++;
				}
				if(!(WordID-RevisedID < WordsIndex_v.size())){
					ostringstream ostream;
					ostream << "Data Out of Range: CParser::Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling(a)" << endl;
					ostream << mite->first;
					AppCall::Secretary_Message_Box(ostream.str().c_str(), MB_OK);
				}
				WordsIndex_v[WordID-RevisedID].first = doccnt;
				for( ; ((DBUF_Index < DOC_LIMIT) && (WBUF_Index < WORD_LIMIT)); ){
					if(EntityMention_mm.find(doccnt) != EntityMention_mm.end()){
						DOC_END = doccnt - 1;
						(*pLocSegmentedTEXT)[DOC_START];
						(*pLocSegmentedTEXT)[DOC_START].first = DOC_END;
						(*pLocSegmentedTEXT)[DOC_START].second = SentBuf;
#ifdef ECOR_DATA_CHECKING						
						//Test
						string teststr = Sentop::Get_Substr_by_Chinese_Character_Cnt(DocBuf, DOC_START, DOC_END-DOC_START+1).c_str();
						if(strcmp(SentBuf, Sentop::Get_Substr_by_Chinese_Character_Cnt(DocBuf, DOC_START, DOC_END-DOC_START+1).c_str())){
							AppCall::Secretary_Message_Box("Data Error in: CParser::Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling(1)", MB_OK);
						}
#endif	
						strcpy_s(SentBuf, MAX_SENTENCE, "");
						DOC_START = doccnt;

						ACE_entity_mention& loc_Menton = *EntityMention_mm[doccnt].rbegin()->second;
#ifdef ECOR_DATA_CHECKING
						if(!NLPOP::prix_substring_matching(&WordBuf[WBUF_Index], loc_Menton.Entity_TYPE.c_str())){
							string teststr = Words_v[WordID].c_str();
							AppCall::Secretary_Message_Box("Data Error in: CParser::Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling(2)", MB_OK);
						}
#endif
						ACE_extent& loc_extend =  loc_Menton.extent;
						DOC_END = loc_extend.END;
						(*pLocSegmentedTEXT)[DOC_START];
						(*pLocSegmentedTEXT)[DOC_START].first = DOC_END;
						(*pLocSegmentedTEXT)[DOC_START].second = loc_extend.charseq;
#ifdef ECOR_DATA_CHECKING
						teststr = Sentop::Get_Substr_by_Chinese_Character_Cnt(DocBuf, DOC_START, DOC_END-DOC_START+1).c_str();
						if(strcmp(loc_extend.charseq.c_str(), Sentop::Get_Substr_by_Chinese_Character_Cnt(DocBuf, DOC_START, DOC_END-DOC_START+1).c_str())){
							AppCall::Secretary_Message_Box("Data Error in: CParser::Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling(3)", MB_OK);
						}
#endif
						DOC_START = DOC_END + 1;

						DBUF_Index += loc_extend.charseq.length();
						WBUF_Index += loc_Menton.Entity_TYPE.length();
						doccnt += NLPOP::Get_Chinese_Sentence_Length_Counter(loc_extend.charseq.c_str());
						wordcnt += NLPOP::Get_Chinese_Sentence_Length_Counter(loc_Menton.Entity_TYPE.c_str());
						//------Sometime word may have forms like "GPE-based" "*GPE*"
						strcat_s(ReplaceWordBuf, MAX_SENTENCE, loc_extend.charseq.c_str());
						if((Words_v[WordID].length() == loc_Menton.Entity_TYPE.length()) || !(WBUF_Index < WORD_LIMIT)){
							break;
						}
					}
					if(DocBuf[DBUF_Index] != WordBuf[WBUF_Index]){
						if(m_OddCharMap.find(&(Words_v[WordID].c_str()[WBUF_Index])) != m_OddCharMap.end()){
							WBUF_Index = Words_v[WordID].length();
#ifdef ECOR_DATA_CHECKING
							if(DocBuf[DBUF_Index] < 0){
								AppCall::Secretary_Message_Box("Data Abnormal..");
							}
#endif
							Words_v[WordID] = ReplaceWordBuf;
							Words_v[WordID] += DocBuf[DBUF_Index];
						}
						else if(-62 == WordBuf[WBUF_Index]){
							if((DocBuf[DBUF_Index] == ' ') && (-96 == WordBuf[WBUF_Index+1])){
								strcat_s(SentBuf, MAX_SENTENCE, " ");
								strcat_s(ReplaceWordBuf, MAX_SENTENCE, " ");
								WBUF_Index += 2;
								DBUF_Index++;
								wordcnt++;
								doccnt++;
								continue;
							}
							AppCall::Secretary_Message_Box("Data Error in: CParser::Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling(3-1)", MB_OK);

						}
#ifdef ECOR_DATA_CHECKING
						else{
							ostringstream ostream;
							ostream << "Error Function: CParser::Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling..." << endl;
							ostream << "DOC characters: " << DocBuf[DBUF_Index] << endl;
							ostream << "Unexpected characters: " << WordBuf[wordcnt] << endl;
							ostream << "Sentence: ";
							for(vector<string>::iterator wordsvite = Words_v.begin(); wordsvite != Words_v.end(); wordsvite++){
								ostream << wordsvite->data() << " ";
							}
							ostream << endl;
							AppCall::Secretary_Message_Box(ostream.str().c_str(), MB_OK);
						}
#endif
					}
					CharBuf[0] = DocBuf[DBUF_Index];
					if(DocBuf[DBUF_Index] < 0){
						DBUF_Index++;
						WBUF_Index++;
						CharBuf[1] = DocBuf[DBUF_Index];
					}
					DBUF_Index++;
					WBUF_Index++;
					wordcnt++;
					doccnt++;
					strcat_s(SentBuf, MAX_SENTENCE, CharBuf);
					strcat_s(ReplaceWordBuf, MAX_SENTENCE, CharBuf);
				}
				WordsIndex_v[WordID-RevisedID].second = doccnt - 1;
				Words_v[WordID] = ReplaceWordBuf;
			}//End of words
			if(Words_v.size() - RevisedID < WordsIndex_v.size()){
				for(size_t i  = Words_v.size() - RevisedID; i < WordsIndex_v.size(); i++){
					WordsIndex_v.pop_back();
				}
			}
		}//End of Sentences
		if(strlen(SentBuf) != 0){
			DOC_END = doccnt - 1;
			(*pLocSegmentedTEXT)[DOC_START];
			(*pLocSegmentedTEXT)[DOC_START].first = DOC_END;
			(*pLocSegmentedTEXT)[DOC_START].second = SentBuf;
#ifdef ECOR_DATA_CHECKING
			string teststr = Sentop::Get_Substr_by_Chinese_Character_Cnt(DocBuf, DOC_START, DOC_END-DOC_START+1).c_str();
			if(strcmp(SentBuf, Sentop::Get_Substr_by_Chinese_Character_Cnt(DocBuf, DOC_START, DOC_END-DOC_START+1).c_str())){
				AppCall::Secretary_Message_Box("Data Error in: CParser::Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling(4)", MB_OK);
			}
#endif
			strcpy_s(SentBuf, MAX_SENTENCE, "");
			DOC_START = DOC_END + 1;
		}
		//loc_PsDocInfo.Init_PsWordInfo_and_PsSentInfo();
	}//End of Doc

	//Inite Sentence Boundary
	for(map<string, _PsDocInfo*>::iterator sgmite = PsDocsInfo_map.begin(); sgmite != PsDocsInfo_map.end(); sgmite++){
		vector<vector<pair<size_t, size_t>>*>& WordsIndex_vv = sgmite->second->WordsIndex_vv;
		map<size_t, size_t>& SentBoundary_m = sgmite->second->SentBoundary_m;
		for(size_t SentenceID = 0; SentenceID < WordsIndex_vv.size(); SentenceID++){
			vector<pair<size_t, size_t>>& Sentence_v = *WordsIndex_vv[SentenceID];
			size_t Sent_START = Sentence_v.begin()->first;
			size_t Sent_END = Sentence_v.rbegin()->second;
#ifdef ECOR_DATA_CHECKING
			if(SentBoundary_m.find(Sent_START) != SentBoundary_m.end()){
				AppCall::Secretary_Message_Box("Data Error in: Inite Sentence Boundary...");
			}
#endif
			SentBoundary_m.insert(make_pair(Sent_START, Sent_END));
		}
	}
}




