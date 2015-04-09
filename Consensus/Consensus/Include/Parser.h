#pragma once

#include "..\\Include\\ace.h"
#include "maxen.h"
//#include "ECCom.h"

typedef struct _DTriple{
	string dependstr;
	size_t arg1;
	size_t arg2;
}DTriple, *pDTriple;

typedef struct _Range{
	size_t posit;
	size_t START;
	size_t END;
}CRange, *pCRange;

typedef pair<multimap<size_t, DTriple*>::iterator,multimap<size_t, DTriple*>::iterator> DTripleMultimapite;


class _PsWordInfo{
public:
	string Wordstr;
	string POS;
	size_t START;
	size_t END;
	//size_t posit;
	size_t SentenceID;
	size_t WordID;
	multimap<size_t, DTriple*> HeadedTriple_mm;
	multimap<size_t, DTriple*> TailedTriple_mm;

	bool Is_Appositional_modifier(size_t opposite){
		DTripleMultimapite mmiterpair = HeadedTriple_mm.equal_range(opposite);
		while(mmiterpair.first != mmiterpair.second){
			if(!strcmp(mmiterpair.first->second->dependstr.c_str(), "appos")){
				return true;
			}
			mmiterpair.first++;
		}
		mmiterpair = TailedTriple_mm.equal_range(opposite);
		while(mmiterpair.first != mmiterpair.second){
			if(!strcmp(mmiterpair.first->second->dependstr.c_str(), "appos")){
				return true;
			}
			mmiterpair.first++;
		}
		return false;
	};
};
class _PsSentInfo{

public:
	//string Sentstr;
	size_t START;
	size_t END;
	map<size_t, map<size_t, _PsWordInfo*>> SentencePsWord_mm;
	map<size_t, _PsWordInfo*> PositPsWord_m;

	string Get_Word_By_Posit(size_t posit){
		if(0 == posit){
			return "ROOT";
		}
		if(PositPsWord_m.find(posit) != PositPsWord_m.end()){
			return PositPsWord_m[posit]->Wordstr;
		}
		else{
			//AppCall::Secretary_Message_Box("Missing Data in: Get_Word_By_Posit...");
		}
		return "";
	};
};

class _PsDocInfo{
public:
	bool Parsed_Info_Loaded_Flag;
	bool Conversed_Flag;
	size_t START;
	size_t END;
	string DOCID;
	const char* DOCBuf;
	const char* TEXTBuf;
	char* ReplaceTEXTBuf;

	map<size_t, size_t> SentBoundary_m;
	vector<vector<pair<size_t, size_t>>*> WordsIndex_vv;
	vector<vector<string>*> Sentences_vv;
	vector<vector<string>*> POS_vv;
	vector<vector<DTriple*>*> Dependency_vv;
	map<string, map<size_t, pair<size_t, string>>*> MentionSegmentedTEXT_mm;
	map<size_t, map<size_t, _PsWordInfo*>> PsWordInfo_mmap;
	map<size_t, map<size_t, _PsSentInfo*>> PsSentInfo_mmap;

public:
	void Slimming(){
		if(!Conversed_Flag){
			return;
		}
		for(map<size_t, map<size_t, _PsWordInfo*>>::iterator mmite = PsWordInfo_mmap.begin();  mmite != PsWordInfo_mmap.end(); mmite++){
			for(map<size_t, _PsWordInfo*>::iterator mite = mmite->second.begin(); mite != mmite->second.end(); mite++){
				delete mite->second;	}	}
		PsWordInfo_mmap.clear();
		for(map<size_t, map<size_t, _PsSentInfo*>>::iterator mmite = PsSentInfo_mmap.begin();  mmite != PsSentInfo_mmap.end(); mmite++){
			for(map<size_t, _PsSentInfo*>::iterator mite = mmite->second.begin(); mite != mmite->second.end(); mite++){
				delete mite->second;	}	}
		PsSentInfo_mmap.clear();
		Conversed_Flag = false;
	};
	void Release(){
		Slimming();
		SentBoundary_m.clear();
		if(ReplaceTEXTBuf){
			delete ReplaceTEXTBuf;
			ReplaceTEXTBuf = NULL;
		}
		for(size_t i = 0; i < WordsIndex_vv.size(); i++){  delete WordsIndex_vv[i]; } WordsIndex_vv.clear();
		for(size_t i = 0; i < Sentences_vv.size(); i++){ delete Sentences_vv[i]; } Sentences_vv.clear();
		for(size_t i = 0; i < POS_vv.size(); i++){ delete POS_vv[i]; } POS_vv.clear();
		for(size_t i = 0; i < Dependency_vv.size(); i++){
			vector<DTriple*>& Dependency_v = *Dependency_vv[i];
			for(size_t j = 0; j < Dependency_v.size(); j++){	delete Dependency_v[j];	}
			delete Dependency_vv[i];	}
		Dependency_vv.clear();
		for(map<string, map<size_t, pair<size_t, string>>*>::iterator mmite = MentionSegmentedTEXT_mm.begin();
			mmite != MentionSegmentedTEXT_mm.end(); mmite++){
			delete mmite->second;	}
		MentionSegmentedTEXT_mm.clear();

		for(map<size_t, map<size_t, _PsWordInfo*>>::iterator mmite = PsWordInfo_mmap.begin();  mmite != PsWordInfo_mmap.end(); mmite++){
			for(map<size_t, _PsWordInfo*>::iterator mite = mmite->second.begin(); mite != mmite->second.end(); mite++){
				delete mite->second;	}	}
		PsWordInfo_mmap.clear();
		for(map<size_t, map<size_t, _PsSentInfo*>>::iterator mmite = PsSentInfo_mmap.begin();  mmite != PsSentInfo_mmap.end(); mmite++){
			for(map<size_t, _PsSentInfo*>::iterator mite = mmite->second.begin(); mite != mmite->second.end(); mite++){
				delete mite->second;	}	}
		PsSentInfo_mmap.clear();
		Parsed_Info_Loaded_Flag = false;
	};
	bool Is_the_Same_Sentence(ACE_entity_mention& pmF_mention, ACE_entity_mention& pmS_mention){
		size_t Min_START = pmF_mention.extent.START<pmS_mention.extent.START?pmF_mention.extent.START:pmS_mention.extent.START;
		size_t Max_END = pmF_mention.extent.END>pmS_mention.extent.END?pmF_mention.extent.END:pmS_mention.extent.END;
		for(map<size_t, size_t>::iterator mite = SentBoundary_m.begin(); mite != SentBoundary_m.end(); mite++){
			if((mite->first <= Min_START) && (mite->second >= Max_END)){
				return true;	}
			if(mite->first > Min_START){
				return false;	}	}
		return false;
	};
	_PsSentInfo* Get_Sentence_PsInfo(_PsWordInfo* pPsWord){
		size_t START = pPsWord->START;
		size_t END = pPsWord->END;
		for(map<size_t, size_t>::iterator mite = SentBoundary_m.begin(); mite != SentBoundary_m.end(); mite++){
			if(START >= mite->first && END <= mite->second){
				START = mite->first;
				END = mite->second;
				break;
			}
		}
		if(PsSentInfo_mmap.find(START) != PsSentInfo_mmap.end()){
			if(PsSentInfo_mmap[START].find(END) != PsSentInfo_mmap[START].end()){
				return PsSentInfo_mmap[START][END];
			}
		}
		return NULL;
	};
	_PsWordInfo* Get_Word_PsInfo(ACE_entity_mention& pmMention){
		size_t START = pmMention.extent.START;
		size_t END = pmMention.extent.END;
		if(PsWordInfo_mmap.find(START) != PsWordInfo_mmap.end()){
			if(PsWordInfo_mmap[START].find(END) != PsWordInfo_mmap[START].end()){
				return PsWordInfo_mmap[START][END];
			}
		}
		for(map<size_t, map<size_t, _PsWordInfo*>>::iterator mmite = PsWordInfo_mmap.begin(); mmite != PsWordInfo_mmap.end(); mmite++){
			for(map<size_t, _PsWordInfo*>::iterator mite = mmite->second.begin(); mite != mmite->second.end(); mite++){
				if(mmite->first <= START && mite->first >= END){
					return mite->second;
				}
			}
		}
		return NULL;
	};
	void Init_PsWordInfo_and_PsSentInfo(){
	if(Conversed_Flag){
		return;
	}
	for(size_t SentenceID = 0; SentenceID < Sentences_vv.size(); SentenceID++){
		vector<string>& Words_v = *Sentences_vv[SentenceID];
		vector<string>& POS_v = *POS_vv[SentenceID];
		vector<pair<size_t, size_t>>& WordIndex_v = *WordsIndex_vv[SentenceID];
		vector<DTriple*>& Dependency_v = *Dependency_vv[SentenceID];

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
			if(PsWordInfo_mmap.find(coordpair.first) == PsWordInfo_mmap.end()){
				PsWordInfo_mmap[coordpair.first];
			}
			if(PsWordInfo_mmap[coordpair.first].find(coordpair.second) == PsWordInfo_mmap[coordpair.first].end()){
				PsWordInfo_mmap[coordpair.first][coordpair.second];
				PsWordInfo_mmap[coordpair.first][coordpair.second] = new _PsWordInfo;
			}
			else{
				AppCall::Secretary_Message_Box("Duplicated Data in: Init_PsWordInfo_and_PsSentInfo()");
			}
			_PsWordInfo& loc_PsWordInfo = *PsWordInfo_mmap[coordpair.first][coordpair.second];
			loc_PsWordInfo.START = coordpair.first;
			loc_PsWordInfo.END = coordpair.second;
			loc_PsWordInfo.Wordstr = Words_v[WordID];
			
			#ifdef ECOR_DATA_CHECKING
			string teststr = Sentop::Get_Substr_by_Chinese_Character_Cnt(DOCBuf, coordpair.first, coordpair.second-coordpair.first+1);
			if(strcmp(loc_PsWordInfo.Wordstr.c_str(), Sentop::Get_Substr_by_Chinese_Character_Cnt(DOCBuf, coordpair.first, coordpair.second-coordpair.first+1).c_str())){
				ostringstream ostream;
				ostream << "Segmented string: " << Sentop::Get_Substr_by_Chinese_Character_Cnt(DOCBuf, coordpair.first, coordpair.second-coordpair.first+1);
				ostream << loc_PsWordInfo.Wordstr << endl;
				AppCall::Secretary_Message_Box(ostream.str().c_str());
				continue;
			}
			#endif

			loc_PsWordInfo.POS = POS_v[WordID];
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
			//loc_PsWordInfo.posit = WordID+1; //0 is used for ROOT in Stanford Paser;
				
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
	for(size_t SentenceID = 0; SentenceID < Sentences_vv.size(); SentenceID++){
		size_t SentSTART = WordsIndex_vv[SentenceID]->begin()->first;
		size_t SentEND = WordsIndex_vv[SentenceID]->rbegin()->second;

		if(PsSentInfo_mmap[SentSTART].find(SentEND) == PsSentInfo_mmap[SentSTART].end()){
			PsSentInfo_mmap[SentSTART].insert(make_pair(SentEND, new _PsSentInfo));
		}
		else{
			AppCall::Secretary_Message_Box("Data Error in: Init_PsWordInfo_and_PsSentInfo()");
		}
		_PsSentInfo& loc_PsSentInfo = *PsSentInfo_mmap[SentSTART][SentEND];
		loc_PsSentInfo.START = SentSTART;
		loc_PsSentInfo.END = SentEND;
	}
	map<size_t, map<size_t, _PsSentInfo*>>::iterator senmmite = PsSentInfo_mmap.begin();
	map<size_t, _PsSentInfo*>::iterator senite = senmmite->second.begin();
	for(map<size_t, map<size_t, _PsWordInfo*>>::iterator mmite = PsWordInfo_mmap.begin(); mmite != PsWordInfo_mmap.end(); mmite++){
		for(map<size_t, _PsWordInfo*>::iterator mite = mmite->second.begin(); mite != mmite->second.end(); mite++){
			while(mmite->first > senite->first){
				senmmite++;
				senite = senmmite->second.begin();
				#ifdef ECOR_DATA_CHECKING
				if(senmmite == PsSentInfo_mmap.end()){
					AppCall::Secretary_Message_Box("Data Error in: Init_PsWordInfo_and_PsSentInfo(1-1)");
					continue;
				}
				#endif
			}
			#ifdef ECOR_DATA_CHECKING
			if(!(mmite->first >= senmmite->first && mite->first <= senite->first)){
				ostringstream ostream;
				if(!(mmite->first >= senmmite->first)){
					ostream << "mmite->first >= senmmite->first: = 0" <<endl; 
				}
				if(!(mite->first <= senite->first)){
					ostream << "mite->first <= senite->first: = 0" <<endl;
					ostream << "mmite->first = " << mmite->first << endl;
					ostream << "mite->first = " << mite->first << endl;
					ostream << "senmmite->first = " << senmmite->first << endl;
					ostream << "senite->first = " << senite->first << endl;
					ostream << "Document: " << DOCID << endl;
					ostream << endl;
					ostream << "Word: " << Sentop::Get_Substr_by_Chinese_Character_Cnt(DOCBuf, mmite->first, mite->first-mmite->first+1) << endl;
					ostream << endl;
					ostream << "Sent: " << Sentop::Get_Substr_by_Chinese_Character_Cnt(DOCBuf, senmmite->first, senite->first-senmmite->first+1) << endl;
				}
				AppCall::Secretary_Message_Box(ostream.str().c_str());
				continue;
			}
			#endif
			_PsSentInfo& loc_PsSentInfo = *PsSentInfo_mmap[senmmite->first][senite->first];
			_PsWordInfo& locWordInfo = *mite->second;
			#ifdef ECOR_DATA_CHECKING
			if(loc_PsSentInfo.SentencePsWord_mm.find(locWordInfo.START) != loc_PsSentInfo.SentencePsWord_mm.end()){
				AppCall::Secretary_Message_Box("Data Error in: Init_PsWordInfo_and_PsSentInfo(1)");
			}
			#endif
			loc_PsSentInfo.SentencePsWord_mm[locWordInfo.START];
			#ifdef ECOR_DATA_CHECKING
			if(loc_PsSentInfo.SentencePsWord_mm[locWordInfo.START].find(locWordInfo.END) != loc_PsSentInfo.SentencePsWord_mm[locWordInfo.START].end()){
				AppCall::Secretary_Message_Box("Data Error in: Init_PsWordInfo_and_PsSentInfo(2)");
			}
			#endif
			loc_PsSentInfo.SentencePsWord_mm[locWordInfo.START].insert(make_pair(locWordInfo.END, mite->second));
			#ifdef ECOR_DATA_CHECKING
			if(loc_PsSentInfo.PositPsWord_m.find(locWordInfo.posit) != loc_PsSentInfo.PositPsWord_m.end()){
				AppCall::Secretary_Message_Box("Data Error in: Init_PsWordInfo_and_PsSentInfo(3)");
			}
			#endif
			//loc_PsSentInfo.PositPsWord_m.insert(make_pair(locWordInfo.posit, mite->second));
		}
	}
	Conversed_Flag = true;
	};
};


namespace PARS{
	void PsDocsInfo_map_to_PsWordInfo_Mmap(map<string, _PsDocInfo*>& pmPsDocsInfo_map);
	void PsDocsInfo_map_to_PsSentInfo_Mmap_by_PsWordInfo_Mmap(map<string, _PsDocInfo*>& pmPsDocsInfo_map);
	
	void PsDocsInfo_map_to_PsSentInfo_Mmap(map<string, _PsDocInfo*>& pmPsDocsInfo_map);
	void PsDocs_Information_Checking(map<string, ACE_sgm>& pm_sgmmap, map<string, _PsDocInfo*>& PsDocsInfo_map);
};


class CParser
{
public:
	map<string, string> m_OddCharMap;

public:
	CParser();
	~CParser();

	//===========================Parsing Port
	void Stanford_Parsing_Port(ACE_Corpus& pmACE_Corpus, const char* cCoupusPath);
	void Replacing_TEXT_with_Entity_Mention_and_Save(const char* savefolder, ACE_Corpus& pmACE_Corpus);

	//===========================Generating Parsed Info Port
	void Generating_Parsed_DOC_Info_Port(ACE_Corpus& pmACE_Corpus, const char* cParsInfoPath, map<string, _PsDocInfo*>& PsDocsInfo_map);
	void Get_ACE_TEXT_From_DOC(map<string, ACE_sgm>& pm_sgmmap, map<string, _PsDocInfo*>& PsDocsInfo_map);
	void Load_Stanford_Pased_TEXT_Information(const char* cpath, const char* DOCID, map<string, _PsDocInfo*>& PsDocsInfo_map);
	void Segmented_Replace_TEXT_Sentence_Words_Index_Labeling(_PsDocInfo& pmPsDocInfo);	
	void Reversing_Replaced_TEXT_Sentence_Words_Index_Labeling(ACE_Corpus& pmACE_Corpus, map<string, _PsDocInfo*>& PsDocsInfo_map);
};
