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

#include "stdafx.h"
#include "GREval.h"
#include "ConvertUTF.h"
#include "RCCom.h"
#include "CRDC.h"
#include "SXMLer.h"
#include "Parser.h"

#define Synonyms_Pattern_Frequence 5

CGREval::CGREval(CSegmter& ref_Segmter): m_CSegmter(ref_Segmter)
{	
	m_Output_Training_Data_Dynamically = false;
	//Extracting_English_radical();
}
CGREval::~CGREval()
{

}
//-------------------------------Generate Training and Testing Relation Cases
void CGREval::Extracting_English_radical()
{
	set<string> Radical_s;
	char getlineBuf[MAX_SENTENCE];
	string Rootstr;
	
	ifstream in("F:\\1.txt");
	if(in.bad())
		return;
	in.clear();
	in.seekg(0, ios::beg);
	while(in.peek() != EOF){
		in.getline(getlineBuf, MAX_SENTENCE, '=');
		istringstream instream(getlineBuf);
		instream.getline(getlineBuf, MAX_SENTENCE, ',');
		while(instream.peek() != EOF){
			instream.getline(getlineBuf, MAX_SENTENCE, ',');
			if(strlen(getlineBuf) != 0){
				Radical_s.insert(getlineBuf);
			}
			instream.getline(getlineBuf, MAX_SENTENCE, ' ');
		}
		in.getline(getlineBuf, MAX_SENTENCE, '\n');
	}
	in.close();

	in.open("F:\\2.txt");
	if(in.bad())
		return;
	in.clear();
	in.seekg(0, ios::beg);
	while(in.peek() != EOF){
		in.getline(getlineBuf, MAX_SENTENCE, '-');
		in.getline(getlineBuf, MAX_SENTENCE, '-');
		if(strlen(getlineBuf) != 0){
			Radical_s.insert(getlineBuf);
		}
	}
	in.close();
	NLPOP::Save_Set_String_With_Comma_Divide("F:\\Radical.dat", Radical_s);
}


void CGREval::Generating_Synonyms_Framework(vector<Relation_Case*>& Relation_Case_v)
{
	char getlineBuf[MAX_SENTENCE];
	string Rootstr;
	
	ifstream in("F:\\YPench\\ConsensusGraph\\Data\\CRDC\\Synonym.dat");
	if(in.bad())
		return;
	in.clear();
	in.seekg(0, ios::beg);
	if(in.peek() == EOF){
		in.close();
		return;
	}
	while(true){
		if(in.peek() == EOF){	
			break;
		}
		in.getline(getlineBuf, MAX_SENTENCE, '\n');
		in.getline(getlineBuf, MAX_SENTENCE, '\n');
		istringstream instream(getlineBuf);
		if(instream.peek() != EOF){
			instream.getline(getlineBuf, MAX_SENTENCE, ' ');
			Rootstr = getlineBuf;
		}
		if(Synonym_map.find(Rootstr) != Synonym_map.end()){
			continue;
		}
		while(instream.peek() != EOF){
			instream.getline(getlineBuf, MAX_SENTENCE, ' ');
			if(Synonym_map.find(getlineBuf) != Synonym_map.end()){
				continue;
			}
			else{
				if(!strcmp(getlineBuf, Rootstr.c_str())){
					continue;
				}
				Synonym_map.insert(make_pair(getlineBuf, Rootstr));
			}
		}
	}
	in.close();

	map<string, float> WordsCnt_map;
	for(vector<Relation_Case*>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
		ostringstream ostrsteam;
		Relation_Case& loc_Case = **vite;
		RelationContext loc_Context;
		ACE_entity_mention& Ref_E1 = loc_Case.first_entity;
		ACE_entity_mention& Ref_E2 = loc_Case.sencond_entity;
		ACE_relation_mention& Ref_relationmention = loc_Case.relatin_mention;
		Sentop::Extract_ACE_entities_Pair_Context(loc_Case, loc_Context);
		RCCOM::Delet_0AH_and_20H_in_Relation_Case(loc_Case);
		RCCOM::Delet_0AH_and_20H_in_RelationContext(loc_Context);
		m_CSegmter.Omni_words_feature_Extracting(loc_Context.L_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
		for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
			if(Synonym_map.find(mite->first) != Synonym_map.end()){
				if(WordsCnt_map.find(Synonym_map[mite->first]) == WordsCnt_map.end()){
					if(BinLeftcnt_m.find(Synonym_map[mite->first]) == BinLeftcnt_m.end()){
						BinLeftcnt_m.insert(make_pair(Synonym_map[mite->first], 1));
					}
					else{
						BinLeftcnt_m[Synonym_map[mite->first]]++;
					}
				}
			}
			else{
				if(BinLeftcnt_m.find(mite->first) == BinLeftcnt_m.end()){
					BinLeftcnt_m.insert(make_pair(mite->first, 1));
				}
				else{
					BinLeftcnt_m[mite->first]++;
				}
			}
		}
		WordsCnt_map.clear();
		m_CSegmter.Omni_words_feature_Extracting(loc_Context.M_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
		for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
			if(Synonym_map.find(mite->first) != Synonym_map.end()){
				if(WordsCnt_map.find(Synonym_map[mite->first]) == WordsCnt_map.end()){
					if(BinMiddlecnt_m.find(Synonym_map[mite->first]) == BinMiddlecnt_m.end()){
						BinMiddlecnt_m.insert(make_pair(Synonym_map[mite->first], 1));
					}
					else{
						BinMiddlecnt_m[Synonym_map[mite->first]]++;
					}
				}
			}
			else{
				if(BinMiddlecnt_m.find(mite->first) == BinMiddlecnt_m.end()){
					BinMiddlecnt_m.insert(make_pair(mite->first, 1));
				}
				else{
					BinMiddlecnt_m[mite->first]++;
				}
			}
		}
		WordsCnt_map.clear();
		m_CSegmter.Omni_words_feature_Extracting(loc_Context.R_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
		for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
			if(Synonym_map.find(mite->first) != Synonym_map.end()){
				if(WordsCnt_map.find(Synonym_map[mite->first]) == WordsCnt_map.end()){
					if(BinRightcnt_m.find(Synonym_map[mite->first]) == BinRightcnt_m.end()){
						BinRightcnt_m.insert(make_pair(Synonym_map[mite->first], 1));
					}
					else{
						BinRightcnt_m[Synonym_map[mite->first]]++;
					}
				}
			}
			else{
				if(BinRightcnt_m.find(mite->first) == BinRightcnt_m.end()){
					BinRightcnt_m.insert(make_pair(mite->first, 1));
				}
				else{
					BinRightcnt_m[mite->first]++;
				}
			}
		}
		WordsCnt_map.clear();
	}

	for(map<string, size_t>::iterator mite = BinLeftcnt_m.begin(); mite != BinLeftcnt_m.end(); ){
		if(mite->second < Synonyms_Pattern_Frequence){
			mite = BinLeftcnt_m.erase(mite);
		}
		else{
			mite++;
		}
	}
	for(map<string, size_t>::iterator mite = BinMiddlecnt_m.begin(); mite != BinMiddlecnt_m.end(); ){
		if(mite->second < Synonyms_Pattern_Frequence){
			mite = BinMiddlecnt_m.erase(mite);
		}
		else{
			mite++;
		}
	}
	for(map<string, size_t>::iterator mite = BinRightcnt_m.begin(); mite != BinRightcnt_m.end(); ){
		if(mite->second < Synonyms_Pattern_Frequence){
			mite = BinRightcnt_m.erase(mite);
		}
		else{
			mite++;
		}
	}
}

void CGREval::Generating_Synonyms_Framework_Cases_Features(RelationContext& loc_Context, map<string, float>& WordsCnt_map)
{
	ostringstream ostrsteam;
	map<string, float> Words_m;
	set<string> LeftWord_s, MiddleWords_s, RightWords_s;
	m_CSegmter.Omni_words_feature_Extracting(loc_Context.L_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, Words_m, "", "");
	for(map<string, float>::iterator mite = Words_m.begin(); mite != Words_m.end(); mite++){
		if(Synonym_map.find(Synonym_map[mite->first]) != Synonym_map.end()){
			LeftWord_s.insert(Synonym_map[mite->first]);
		}
		else{
			LeftWord_s.insert(mite->first);
		}
	}
	Words_m.clear();
	m_CSegmter.Omni_words_feature_Extracting(loc_Context.M_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, Words_m, "", "");
	for(map<string, float>::iterator mite = Words_m.begin(); mite != Words_m.end(); mite++){
		if(Synonym_map.find(Synonym_map[mite->first]) != Synonym_map.end()){
			MiddleWords_s.insert(Synonym_map[mite->first]);
		}
		else{
			MiddleWords_s.insert(mite->first);
		}
	}
	Words_m.clear();
	m_CSegmter.Omni_words_feature_Extracting(loc_Context.R_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, Words_m, "", "");
	for(map<string, float>::iterator mite = Words_m.begin(); mite != Words_m.end(); mite++){
		if(Synonym_map.find(Synonym_map[mite->first]) != Synonym_map.end()){
			RightWords_s.insert(Synonym_map[mite->first]);
		}
		else{
			RightWords_s.insert(mite->first);
		}
	}
	Words_m.clear();
	for(set<string>::iterator lsite = LeftWord_s.begin(); lsite != LeftWord_s.end(); lsite++){
		if(BinLeftcnt_m.find(lsite->data()) == BinLeftcnt_m.end()){
			break;
		}
		for(set<string>::iterator msite = MiddleWords_s.begin(); msite != MiddleWords_s.end(); msite++){
			if(BinMiddlecnt_m.find(msite->data()) == BinMiddlecnt_m.end()){
				break;
			}
			for(set<string>::iterator rsite = RightWords_s.begin(); rsite != RightWords_s.end(); rsite++){
				if(BinRightcnt_m.find(rsite->data()) == BinRightcnt_m.end()){
					continue;
				}
				else{
					ostrsteam.str("");
					ostrsteam << lsite->data() << "_" << msite->data() << "_" << rsite->data();
					//ostrsteam << "_" << RCCOM::Get_Entity_Positional_Structures(loc_Case);
					//string teststr = ostrsteam.str();
					WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
				}
			}
		}
	}
}

void CGREval::Generate_CRDC_Feature_Vector_Port(Relation_Case& pmRelation_Case, vector<pair<string, float>>& Features_v)
{
	ostringstream ostrsteam;
	Relation_Case& loc_Case = pmRelation_Case;
	RelationContext loc_Context;
	ACE_entity_mention& Ref_E1 = loc_Case.first_entity;
	ACE_entity_mention& Ref_E2 = loc_Case.sencond_entity;
	ACE_relation_mention& Ref_relationmention = loc_Case.relatin_mention;
	map<string, float> WordsCnt_map;
		
	Sentop::Extract_ACE_entities_Pair_Context(loc_Case, loc_Context);
	RCCOM::Delet_0AH_and_20H_in_Relation_Case(loc_Case);
	RCCOM::Delet_0AH_and_20H_in_RelationContext(loc_Context);

	//----Subtype and TYPE of entity pair;
	if(true){//(pCRDC->Enitity_TYPE_Flag){
		//------Multiplication Constraint   \mathcal{F}_{(E\_M)}
		ostrsteam.str("");
		ostrsteam << Ref_E1.Entity_TYPE << "_" << Ref_E2.Entity_TYPE;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
		if(false){
			ostrsteam.str("");
			ostrsteam << Ref_E1.Entity_SUBSTYPE << "_" << Ref_E2.Entity_SUBSTYPE;
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
		}
	}
	//-----Entities Structure;
	if(true){//(pCRDC->EntitiesStructure_Flag){
		//------Singleton-------------------------\mathcal{F}_{(S\_S)}
		WordsCnt_map.insert(make_pair(RCCOM::Get_Entity_Positional_Structures(loc_Case), (float)1));
	}
	//-----head noun;
	if(true){//(pCRDC->HeadNoun_Flag){	
		//------Position	\mathcal{F}_{(H\_P)}
		WordsCnt_map.insert(make_pair(Ref_E1.head.charseq+"_E1", (float)1));	
		WordsCnt_map.insert(make_pair(Ref_E2.head.charseq+"_E2", (float)1));

	}
	//-----dual types constriant;
	if(true){//(pCRDC->POS_Tag_Flag){
		//------Multiply Entity TYPE and Position
		RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_"+Ref_E1.Entity_TYPE+"_E1");
		RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, "E1_"+Ref_E1.Entity_TYPE+"_", "_"+Ref_E2.Entity_TYPE+"_E2");
		RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, "E2_"+Ref_E2.Entity_TYPE+"_", "");
	}

	//-----Omni-words feature;
	if(true){//(pCRDC->OmniWords_Flag){
		//------Bin
		m_CSegmter.Omni_words_feature_Extracting(Ref_E1.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "F_", "");
		m_CSegmter.Omni_words_feature_Extracting(Ref_E2.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "S_", "");
		m_CSegmter.Omni_words_feature_Extracting(loc_Context.L_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "L_", "");
		m_CSegmter.Omni_words_feature_Extracting(loc_Context.M_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "M_", "");
		m_CSegmter.Omni_words_feature_Extracting(loc_Context.R_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "R_", "");
	}
	for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
		Features_v.push_back(make_pair(mite->first, mite->second));
	}
}

void CGREval::POS_of_Entity_Mention(ACE_entity_mention& Ref_Ei, map<string, _PsDocInfo*> PsDocsInfo_map, map<string, float>& WordsCnt_map, string index = "")
{
	ostringstream osstrem;
	if(PsDocsInfo_map.find(Ref_Ei.DOCID) == PsDocsInfo_map.end()){
		osstrem << "Data Error In: CGREval::POS_of_Entity_Mention(1)" << endl;
		osstrem << "DOCID: " << Ref_Ei.DOCID << endl;
		osstrem << "metion: " << Ref_Ei.extent.charseq;
		AppCall::Secretary_Message_Box(osstrem.str().c_str());
	}
	_PsDocInfo& loc_PsDocInfo = *PsDocsInfo_map[Ref_Ei.DOCID];

	map<size_t, size_t>& SentBoundary_m = loc_PsDocInfo.SentBoundary_m;
	vector<vector<pair<size_t, size_t>>*>& WordsIndex_vv = loc_PsDocInfo.WordsIndex_vv;
	vector<vector<string>*>& Sentences_vv = loc_PsDocInfo.Sentences_vv;
	vector<vector<string>*>& POS_vv = loc_PsDocInfo.POS_vv;
	vector<vector<DTriple*>*>& Dependency_vv = loc_PsDocInfo.Dependency_vv;
	map<string, map<size_t, pair<size_t, string>>*>& MentionSegmentedTEXT_mm = loc_PsDocInfo.MentionSegmentedTEXT_mm;
	map<size_t, map<size_t, _PsWordInfo*>>& PsWordInfo_mmap = loc_PsDocInfo.PsWordInfo_mmap;
	map<size_t, map<size_t, _PsSentInfo*>>& PsSentInfo_mmap = loc_PsDocInfo.PsSentInfo_mmap;
	
	size_t Ei_START = Ref_Ei.extent.START;
	size_t Ei_END = Ref_Ei.extent.END;

	//----Because some entity extend mention may be nested;
	for(map<size_t, map<size_t, _PsWordInfo*>>::iterator mmite = PsWordInfo_mmap.begin(); mmite != PsWordInfo_mmap.end(); mmite++){
		for(map<size_t, _PsWordInfo*>::iterator mite = mmite->second.begin(); mite != mmite->second.end(); mite++){
			if(mmite->first <= Ei_START && mite->first >= Ei_END){
				Ei_START = mmite->first;
				Ei_END = mite->first;
			}
		}
	}
	if(PsWordInfo_mmap.find(Ei_START) == PsWordInfo_mmap.end()){
		AppCall::Secretary_Message_Box("Data Error In: CGREval::POS_of_Entity_Mention(2)");
	}
	if(PsWordInfo_mmap[Ei_START].find(Ei_END) == PsWordInfo_mmap[Ei_START].end()){
		AppCall::Secretary_Message_Box("Data Error In: CGREval::POS_of_Entity_Mention(3)");
	}
	_PsWordInfo& loc__PsWordInfo = *PsWordInfo_mmap[Ei_START][Ei_END];
	if(Sentences_vv.size() != POS_vv.size() || loc__PsWordInfo.SentenceID >= Sentences_vv.size()){
		AppCall::Secretary_Message_Box("Data Error In: CGREval::POS_of_Entity_Mention(4)");
	}
	vector<string>& Words_v = *Sentences_vv[loc__PsWordInfo.SentenceID];
	vector<string>& POS_v = *POS_vv[loc__PsWordInfo.SentenceID];
	if(Words_v.size() != POS_v.size() || loc__PsWordInfo.WordID >= Words_v.size()){
		AppCall::Secretary_Message_Box("Data Error In: CGREval::POS_of_Entity_Mention(5)");
	}
	//if(strcmp(Words_v[loc__PsWordInfo.WordID].c_str(), Ref_Ei.extent.charseq.c_str())){
		//AppCall::Secretary_Message_Box("Data Error In: CGREval::POS_of_Entity_Mention(6)");
	//}
	
	//---------POS L;
	osstrem.str("");
	osstrem << "pos_L_";
	if(loc__PsWordInfo.WordID != 0){
		osstrem << POS_v[loc__PsWordInfo.WordID-1];
	}
	osstrem << "" << index;
	WordsCnt_map.insert(make_pair(osstrem.str(), (float)1));

	//---------POS M;
	osstrem.str("");
	osstrem << "pos_M_" << POS_v[loc__PsWordInfo.WordID] << "" << index;
	WordsCnt_map.insert(make_pair(osstrem.str(), (float)1));
	
	//---------POS R;
	osstrem.str("");
	osstrem << "pos_R_";
	if(loc__PsWordInfo.WordID != POS_v.size()-1){
		osstrem << POS_v[loc__PsWordInfo.WordID+1];
	}
	osstrem << "" << index;
	WordsCnt_map.insert(make_pair(osstrem.str(), (float)1));

	//------------------
	multimap<size_t, DTriple*>& rHead_mm = loc__PsWordInfo.HeadedTriple_mm;
	multimap<size_t, DTriple*>& rTail_mm = loc__PsWordInfo.TailedTriple_mm;

	for(multimap<size_t, DTriple*>::iterator mite = rHead_mm.begin(); mite != rHead_mm.end(); mite++){
		size_t DepID = mite->second->arg2;
		if(!(DepID < Words_v.size())){
			osstrem.str("");
			osstrem << "Data Error In: CGREval::POS_of_Entity_Mention(7)" << endl;
			osstrem << "DOCID: " << Ref_Ei.DOCID << endl;
			osstrem << "metion: " << Ref_Ei.extent.charseq;
			AppCall::Secretary_Message_Box(osstrem.str().c_str());
		}
		osstrem.str("");
		osstrem  << "dep_" << Words_v[DepID] << "_" << index;
		WordsCnt_map.insert(make_pair(osstrem.str(), (float)1));

		osstrem.str("");
		osstrem  << "dep_" << POS_v[DepID] << "_" << index;
		WordsCnt_map.insert(make_pair(osstrem.str(), (float)1));
	}

	for(multimap<size_t, DTriple*>::iterator mite = rTail_mm.begin(); mite != rTail_mm.end(); mite++){
		size_t DepID = mite->second->arg1;
		if(!(DepID < Words_v.size())){
			continue;
			AppCall::Secretary_Message_Box("Data Error In: CGREval::POS_of_Entity_Mention(8)");
		}
		osstrem.str("");
		osstrem << "dep_" << Words_v[DepID] << "_" << index;
		WordsCnt_map.insert(make_pair(osstrem.str(), (float)1));

		osstrem.str("");
		osstrem << "dep_" << POS_v[DepID] << "_" << index;
		WordsCnt_map.insert(make_pair(osstrem.str(), (float)1));
	}

}

void CGREval::Generate_YPench_English_Evaluation_Port(const char * savepath, vector<Relation_Case*>& Relation_Case_v, DulTYPE_Vctor& pmTraining_v)
{
	//---For English
	CParser m_CParser;
	map<string, _PsDocInfo*> PsDocsInfo_map;

	SXMLer m_SXMLer;
	ACE_Corpus loc_ACE_Corpus;
	//string ACECorpusFolder = "F:\\YPench\\ConsensusGraph\\Corpus\\ACE_E\\";
	//string PasingFolder =    "F:\\YPench\\ConsensusGraph\\Data\\ECOR\\Parser_E\\";

	string ACECorpusFolder = "F:\\YPench\\ConsensusGraph\\Corpus\\ACE\\ACE\\";
	string PasingFolder =    "F:\\YPench\\ConsensusGraph\\Data\\ECOR\\Parser\\";

	m_SXMLer.Xercesc_ACE_Folder_Files_Filter(ACECorpusFolder, loc_ACE_Corpus);
	m_SXMLer.Xercesc_ACE_Folder_For_sgm_Files_Filter(ACECorpusFolder, loc_ACE_Corpus);

	m_CParser.Generating_Parsed_DOC_Info_Port(loc_ACE_Corpus, PasingFolder.c_str(), PsDocsInfo_map);
	 
	//AppCall::Secretary_Message_Box("Done");
	//----Words---------------------
	AppCall::Maxen_Responce_Message("4: Begining PARS::PsDocsInfo_map_to_PsSentInfo_Mmap()\n\n");
	PARS::PsDocsInfo_map_to_PsWordInfo_Mmap(PsDocsInfo_map);
	//----Sentences---------------------
	//AppCall::Maxen_Responce_Message("5: Begining PARS::PsDocsInfo_map_to_PsSentInfo_Mmap()\n\n");
	//PARS::PsDocsInfo_map_to_PsSentInfo_Mmap(PsDocsInfo_map);
	//----Slimming---------------------
	//AppCall::Maxen_Responce_Message("6: Begining PARS::Slimming()\n\n");
	//for(map<string, _PsDocInfo*>::iterator mite = PsDocsInfo_map.begin(); mite != PsDocsInfo_map.end(); mite++){
	//	mite->second->Slimming();
	//}

	for(vector<Relation_Case*>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
		ostringstream ostrsteam;
		Relation_Case& loc_Case = **vite;

		if(PsDocsInfo_map.find(loc_Case.DOCID) == PsDocsInfo_map.end()){
			delete *vite;
			continue;
		}

		RelationContext loc_Context;
		ACE_entity_mention& Ref_E1 = loc_Case.first_entity;
		ACE_entity_mention& Ref_E2 = loc_Case.sencond_entity;
		ACE_relation_mention& Ref_relationmention = loc_Case.relatin_mention;
		map<string, float> WordsCnt_map;
		
		Sentop::Extract_ACE_entities_Pair_Context(loc_Case, loc_Context);

		ace_op::Delet_0AH_in_string(loc_Case.first_entity.extent.charseq);
		ace_op::Delet_0AH_in_string(loc_Case.sencond_entity.extent.charseq);
		ace_op::Delet_0AH_in_string(loc_Case.relatin_mention.extent.charseq);
		ace_op::Delet_0AH_in_string(loc_Case.first_entity.head.charseq);
		ace_op::Delet_0AH_in_string(loc_Case.sencond_entity.head.charseq);

		ace_op::Delet_0AH_in_string(loc_Context.L_str);
		ace_op::Delet_0AH_in_string(loc_Context.M_str);
		ace_op::Delet_0AH_in_string(loc_Context.R_str);

		//----Subtype and TYPE of entity pair;
		if(true){//(pCRDC->Enitity_TYPE_Flag){
			//------Multiplication Constraint   \mathcal{F}_{(E\_M)}
			ostrsteam.str("");
			ostrsteam << Ref_E1.Entity_TYPE << "_" << Ref_E2.Entity_TYPE;
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
			
			if(true){//for generating event network
				ostrsteam.str("");
				ostrsteam << Ref_E1.Entity_SUBSTYPE << "_" << Ref_E2.Entity_SUBSTYPE;
				WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
			}
				
			//------Collection Constraint   \mathcal{F}_{(E\_C)}
			/*set<string> Collective_s;
			Collective_s.insert(Ref_E1.Entity_SUBSTYPE);
			Collective_s.insert(Ref_E2.Entity_SUBSTYPE);
			for(set<string>::iterator site = Collective_s.begin(); site != Collective_s.end(); site++){
				ostrsteam << site->data();
				ostrsteam << "_";
			}
			ostrsteam << "EC";
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
			ostrsteam.str("");
			Collective_s.clear();
			Collective_s.insert(Ref_E1.Entity_TYPE);
			Collective_s.insert(Ref_E2.Entity_TYPE);
			for(set<string>::iterator site = Collective_s.begin(); site != Collective_s.end(); site++){
				ostrsteam << site->data();
				ostrsteam << "_";
			}
			ostrsteam << "EC";
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));*/

			//------Position Constraint   \mathcal{F}_{(E\_P)}
			//WordsCnt_map.insert(make_pair("E1_"+Ref_E1.Entity_SUBSTYPE, (float)1));
			//WordsCnt_map.insert(make_pair("E2_"+Ref_E2.Entity_SUBSTYPE, (float)1));
			//WordsCnt_map.insert(make_pair("E1_"+Ref_E1.Entity_TYPE, (float)1));
			//WordsCnt_map.insert(make_pair("E2_"+Ref_E2.Entity_TYPE, (float)1));

			//------Singleton   \mathcal{F}_{(E\_S)}
			//WordsCnt_map.insert(make_pair(Ref_E1.Entity_TYPE, (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E2.Entity_TYPE, (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E1.Entity_SUBSTYPE, (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E2.Entity_SUBSTYPE, (float)1));
		}
		//-----head noun;
		if(true){//(pCRDC->HeadNoun_Flag){
			//------Multiplication---------------\mathcal{F}_{(H\_M)}
			//ostrsteam.str("");
			//ostrsteam << Ref_E1.head.charseq << "_" << Ref_E2.head.charseq;
			//WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
			
			//------Singleton---------------\mathcal{F}_{(H\_S)}
			//WordsCnt_map.insert(make_pair(Ref_E1.head.charseq, (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E2.head.charseq, (float)1));
			
			//------Position	\mathcal{F}_{(H\_P)}
			WordsCnt_map.insert(make_pair(Ref_E1.head.charseq+"_E1", (float)1));	
			WordsCnt_map.insert(make_pair(Ref_E2.head.charseq+"_E2", (float)1));

			//------Collection---------------\mathcal{F}_{(H\_C)}
			/*ostrsteam.str("");
			set<string> Collective_s;
			Collective_s.insert(Ref_E1.head.charseq);
			Collective_s.insert(Ref_E2.head.charseq);
			for(set<string>::iterator site = Collective_s.begin(); site != Collective_s.end(); site++){
				ostrsteam << site->data();
				ostrsteam << "_";
			}
			ostrsteam << "HC";
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));*/

			//WordsCnt_map.insert(make_pair(Ref_E1.extent.charseq, (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E2.extent.charseq, (float)1));

			//WordsCnt_map.insert(make_pair(Ref_E1.extent.charseq+"_E1", (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E2.extent.charseq+"_E2", (float)1));
		}
		//-----Entities Structure;
		if(true){//(pCRDC->EntitiesStructure_Flag){
			//------Singleton-------------------------\mathcal{F}_{(S\_S)}
			WordsCnt_map.insert(make_pair(RCCOM::Get_Entity_Positional_Structures(loc_Case), (float)1));
		}
		//-----dual types constriant;
		if(false){//(pCRDC->POS_Tag_Flag){
			//------Singleton   \mathcal{F}_{(P\_S)}
			//RCCOM::Local_Adjacent_Words_Singleton_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_");
			//RCCOM::Local_Adjacent_Words_Singleton_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, "_", "_");
			//RCCOM::Local_Adjacent_Words_Singleton_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, "_", "");
			//------Position	\mathcal{F}_{P\_P)}
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_E1");
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, "E1_", "_E2");
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, "E2_", "");

			//------Multiply Entity TYPE and Position
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_"+Ref_E1.Entity_TYPE+"_E1");
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, "E1_"+Ref_E1.Entity_TYPE+"_", "_"+Ref_E2.Entity_TYPE+"_E2");
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, "E2_"+Ref_E2.Entity_TYPE+"_", "");

			//------Multiply Entity TYPE-------$\mathcal{F}_{(PE\_M)}$
			POS_of_Entity_Mention(Ref_E1, PsDocsInfo_map, WordsCnt_map, Ref_E1.Entity_TYPE+"_E1");
			POS_of_Entity_Mention(Ref_E2, PsDocsInfo_map, WordsCnt_map, Ref_E2.Entity_TYPE+"_E2");

			//------Multiply Entity TYPE-------$\mathcal{F}_{(PH\_M)}$
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_"+Ref_E1.head.charseq);
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, Ref_E1.head.charseq+"_", "_"+Ref_E2.head.charseq);
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, Ref_E2.head.charseq+"_", "");
		}
		//-----Omni-words feature;
		if(true){//(pCRDC->OmniWords_Flag){
			//------Singleton
			//m_CSegmter.Omni_words_feature_Extracting(ace_op::Delet_0AH_and_20H_in_string(loc_Case.relatin_mention.extent.charseq.c_str()).c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			//------Bin
			m_CSegmter.English_Words_Extractor(Ref_E1.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "F_", "");
			m_CSegmter.English_Words_Extractor(Ref_E2.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "S_", "");
			m_CSegmter.English_Words_Extractor(loc_Context.L_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "L_", "");
			m_CSegmter.English_Words_Extractor(loc_Context.M_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "M_", "");
			m_CSegmter.English_Words_Extractor(loc_Context.R_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "R_", "");
			/*m_CSegmter.English_Words_Extractor(Ref_E1.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			m_CSegmter.English_Words_Extractor(Ref_E2.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			m_CSegmter.English_Words_Extractor(loc_Context.L_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			m_CSegmter.English_Words_Extractor(loc_Context.M_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			m_CSegmter.English_Words_Extractor(loc_Context.R_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");*/
		}
		if(false){//(pCRDC->Segmentation_Flag){
			//m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(ace_op::Delet_0AH_and_20H_in_string(loc_Case.relatin_mention.extent.charseq.c_str()).c_str(), WordsCnt_map, "", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(Ref_E1.extent.charseq.c_str(), WordsCnt_map, "Fs_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(Ref_E2.extent.charseq.c_str(), WordsCnt_map, "Ss_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(loc_Context.L_str.c_str(), WordsCnt_map, "Ls_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(loc_Context.M_str.c_str(), WordsCnt_map, "Ms_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(loc_Context.R_str.c_str(), WordsCnt_map, "Rs_", "");
		}
		if(false){//Generating_Synonyms_Framework
			Generating_Synonyms_Framework_Cases_Features(loc_Context, WordsCnt_map);
		}
		if(false){
			size_t nGram = 1;
			//------Singleton
			//N_Gram_of_Sent_Sequence(ace_op::Delet_0AH_and_20H_in_string(loc_Case.relatin_mention.extent.charseq.c_str()).c_str(), nGram, WordsCnt_map, "", "");
			//------S
			/*N_Gram_of_Sent_Sequence(Ref_E1.extent.charseq.c_str(), nGram, WordsCnt_map, "", "");
			N_Gram_of_Sent_Sequence(Ref_E2.extent.charseq.c_str(), nGram, WordsCnt_map, "", "");
			N_Gram_of_Sent_Sequence(loc_Context.L_str.c_str(), nGram, WordsCnt_map, "", "");
			N_Gram_of_Sent_Sequence(loc_Context.M_str.c_str(), nGram, WordsCnt_map, "", "");
			N_Gram_of_Sent_Sequence(loc_Context.R_str.c_str(), nGram, WordsCnt_map, "", "");*/
			//------Bin
			N_Gram_of_Sent_Sequence(Ref_E1.extent.charseq.c_str(), nGram, WordsCnt_map, "F_", "");
			N_Gram_of_Sent_Sequence(Ref_E2.extent.charseq.c_str(), nGram, WordsCnt_map, "S_", "");
			N_Gram_of_Sent_Sequence(loc_Context.L_str.c_str(), nGram, WordsCnt_map, "L_", "");
			N_Gram_of_Sent_Sequence(loc_Context.M_str.c_str(), nGram, WordsCnt_map, "M_", "");
			N_Gram_of_Sent_Sequence(loc_Context.R_str.c_str(), nGram, WordsCnt_map, "R_", "");
		}
		if(false){//(pCRDC->EntityCLASS_Flag){
			WordsCnt_map.insert(make_pair(Ref_E1.Entity_CLASS, (float)1));
			WordsCnt_map.insert(make_pair(Ref_E2.Entity_CLASS, (float)1));
			WordsCnt_map.insert(make_pair(Ref_E1.LDCTYPE, (float)1));
			WordsCnt_map.insert(make_pair(Ref_E2.LDCTYPE, (float)1));
			//ostrsteam.str("");
			//ostrsteam << "_" << Ref_E1.Entity_CLASS << "_" << Ref_E2.Entity_CLASS << "_";
			//WordsCnt_map.insert(make_pair(ostrsteam.str(), 1));
			//ostrsteam.str("");
			//ostrsteam << "_" << Ref_E1.LDCTYPE << "_" << Ref_E2.LDCTYPE << "_";
			//WordsCnt_map.insert(make_pair(ostrsteam.str(), 1));
		}
		if(WordsCnt_map.empty()){
			//AppCall::Secretary_Message_Box("Empty feature case in CRDC::Generate_ACE_Training_Cases()", MB_OK);
			//The POS Tag with position constraint may result to empty feature;
			continue;
		}

		pair<pair<string, string>, vector<pair<string, float>>>* pCRDC_Case_v = new pair<pair<string, string>, vector<pair<string, float>>>;
		pCRDC_Case_v->first.first  = loc_Case.TYPE;
		pCRDC_Case_v->first.second = loc_Case.SUBTYPE;
		for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
			pCRDC_Case_v->second.push_back(make_pair(mite->first, mite->second));
		}
		pmTraining_v.push_back(pCRDC_Case_v);
		
		delete *vite;
	}

	Relation_Case_v.clear();
	for(map<string, _PsDocInfo*>::iterator mite = PsDocsInfo_map.begin(); mite != PsDocsInfo_map.end(); mite++){
		mite->second->Release();
		delete mite->second;
	}
}

void CGREval::POS_and_Dependency_of_Entity_Mention(Relation_Case& pm_RelCase, map<string, _PsDocInfo*> PsDocsInfo_map, map<string, float>& WordsCnt_map, string index = "")
{
	if(PsDocsInfo_map.find(pm_RelCase.DOCID) == PsDocsInfo_map.end()){
		ostringstream osstrem;
		osstrem << "Data Error In: CGREval::POS_of_Entity_Mention(1)" << endl;
		osstrem << "DOCID: " << pm_RelCase.DOCID << endl;
		osstrem << "metion: " << pm_RelCase.relatin_mention.extent.charseq;
		AppCall::Secretary_Message_Box(osstrem.str().c_str());
	}
	_PsDocInfo& loc_PsDocInfo = *PsDocsInfo_map[pm_RelCase.DOCID];

	map<size_t, size_t>& SentBoundary_m = loc_PsDocInfo.SentBoundary_m;
	vector<vector<pair<size_t, size_t>>*>& WordsIndex_vv = loc_PsDocInfo.WordsIndex_vv;
	vector<vector<string>*>& Sentences_vv = loc_PsDocInfo.Sentences_vv;
	vector<vector<string>*>& POS_vv = loc_PsDocInfo.POS_vv;
	vector<vector<DTriple*>*>& Dependency_vv = loc_PsDocInfo.Dependency_vv;
	map<string, map<size_t, pair<size_t, string>>*>& MentionSegmentedTEXT_mm = loc_PsDocInfo.MentionSegmentedTEXT_mm;
	map<size_t, map<size_t, _PsWordInfo*>>& PsWordInfo_mmap = loc_PsDocInfo.PsWordInfo_mmap;
	map<size_t, map<size_t, _PsSentInfo*>>& PsSentInfo_mmap = loc_PsDocInfo.PsSentInfo_mmap;
	
}
void CGREval::Generate_Nanda_English_Evaluation_Port(const char * savepath, vector<Relation_Case*>& Relation_Case_v, DulTYPE_Vctor& pmTraining_v)
{
	//---For English
	CParser m_CParser;
	map<string, _PsDocInfo*> PsDocsInfo_map;

	SXMLer m_SXMLer;
	ACE_Corpus loc_ACE_Corpus;
	//string ACECorpusFolder = "F:\\YPench\\ConsensusGraph\\Corpus\\ACE_E\\";
	//string PasingFolder =    "F:\\YPench\\ConsensusGraph\\Data\\ECOR\\Parser_E\\";

	string ACECorpusFolder = "F:\\YPench\\ConsensusGraph\\Corpus\\ACE\\ACE\\";
	string PasingFolder =    "F:\\YPench\\ConsensusGraph\\Data\\ECOR\\Parser\\";

	m_SXMLer.Xercesc_ACE_Folder_Files_Filter(ACECorpusFolder, loc_ACE_Corpus);
	m_SXMLer.Xercesc_ACE_Folder_For_sgm_Files_Filter(ACECorpusFolder, loc_ACE_Corpus);

	m_CParser.Generating_Parsed_DOC_Info_Port(loc_ACE_Corpus, PasingFolder.c_str(), PsDocsInfo_map);
	
	//AppCall::Secretary_Message_Box("Done");
	//----Words---------------------
	AppCall::Maxen_Responce_Message("4: Begining PARS::PsDocsInfo_map_to_PsSentInfo_Mmap()\n\n");
	PARS::PsDocsInfo_map_to_PsWordInfo_Mmap(PsDocsInfo_map);

	set<string> Radical_s;
	NLPOP::LoadWordsStringSet("F:\\YPench\\ConsensusGraph\\Data\\Radical.dat", Radical_s);
	size_t Max_Radical_Length = 0;
	for(set<string>::iterator site = Radical_s.begin(); site != Radical_s.end(); site++){
		if(site->length() > Max_Radical_Length){
			Max_Radical_Length = site->length();
		}
	}
 
	for(vector<Relation_Case*>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
		ostringstream ostrsteam;
		Relation_Case& loc_Case = **vite;
		if(PsDocsInfo_map.find(loc_Case.DOCID) == PsDocsInfo_map.end()){
			ostringstream osstrem;
			osstrem << "Data Error In: CGREval::POS_of_Entity_Mention(1)" << endl;
			osstrem << "DOCID: " << loc_Case.DOCID << endl;
			osstrem << "metion: " << loc_Case.relatin_mention.extent.charseq;
			AppCall::Secretary_Message_Box(osstrem.str().c_str());
			delete *vite;
			continue;
		}
		//if(!strcmp(loc_Case.TYPE.c_str(), NEGETIVE)){
		//	delete *vite;
		//	continue;
		//}
		RelationContext loc_Context;
		ACE_entity_mention& Ref_E1 = loc_Case.first_entity;
		ACE_entity_mention& Ref_E2 = loc_Case.sencond_entity;
		ACE_relation_mention& Ref_relationmention = loc_Case.relatin_mention;
		map<string, float> WordsCnt_map;
		
		Sentop::Extract_ACE_entities_Pair_Context(loc_Case, loc_Context);

		ace_op::Delet_0AH_in_string(loc_Case.first_entity.extent.charseq);
		ace_op::Delet_0AH_in_string(loc_Case.sencond_entity.extent.charseq);
		ace_op::Delet_0AH_in_string(loc_Case.relatin_mention.extent.charseq);
		ace_op::Delet_0AH_in_string(loc_Case.first_entity.head.charseq);
		ace_op::Delet_0AH_in_string(loc_Case.sencond_entity.head.charseq);

		ace_op::Delet_0AH_in_string(loc_Context.L_str);
		ace_op::Delet_0AH_in_string(loc_Context.M_str);
		ace_op::Delet_0AH_in_string(loc_Context.R_str);

		//----Subtype and TYPE of entity pair;
		if(true){//(pCRDC->Enitity_TYPE_Flag){
			//------Position Constraint   \mathcal{F}_{(E\_P)}
			/*WordsCnt_map.insert(make_pair("E1_"+Ref_E1.Entity_SUBSTYPE, (float)1));
			WordsCnt_map.insert(make_pair("E2_"+Ref_E2.Entity_SUBSTYPE, (float)1));
			WordsCnt_map.insert(make_pair("E1_"+Ref_E1.Entity_TYPE, (float)1));
			WordsCnt_map.insert(make_pair("E2_"+Ref_E2.Entity_TYPE, (float)1));*/

			WordsCnt_map.insert(make_pair("E1_"+Ref_E1.TYPE, (float)1));
			WordsCnt_map.insert(make_pair("E2_"+Ref_E2.TYPE, (float)1));
			
			ostrsteam.str("");
			ostrsteam << Ref_E1.Entity_TYPE << "_" << Ref_E2.Entity_TYPE;
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
			
			ostrsteam.str("");
			ostrsteam << Ref_E1.Entity_SUBSTYPE << "_" << Ref_E2.Entity_SUBSTYPE;
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));

			//ostrsteam.str("");
			//ostrsteam << Ref_E1.TYPE << "_" << Ref_E2.TYPE;
			//WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));

		}
		//-----head noun;
		if(true){//(pCRDC->HeadNoun_Flag){
			//------Position	\mathcal{F}_{(H\_P)}
			WordsCnt_map.insert(make_pair(Ref_E1.head.charseq+"_E1", (float)1));	
			WordsCnt_map.insert(make_pair(Ref_E2.head.charseq+"_E2", (float)1));

			//ostrsteam.str("");
			//ostrsteam << Ref_E1.head.charseq << "_" << Ref_E2.head.charseq;
			//WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
		}
		//-----Entities Structure;
		if(true){//(pCRDC->EntitiesStructure_Flag){
			//------Singleton-------------------------\mathcal{F}_{(S\_S)}
			WordsCnt_map.insert(make_pair(RCCOM::Get_Entity_Positional_Structures(loc_Case), (float)1));
		}
		//-----dual types constriant;
		if(false){//(pCRDC->POS_Tag_Flag){
			POS_of_Entity_Mention(Ref_E1, PsDocsInfo_map, WordsCnt_map, "_E1");
			POS_of_Entity_Mention(Ref_E2, PsDocsInfo_map, WordsCnt_map, "_E2");
		}
		//-----Omni-words feature;
		if(true){//(pCRDC->OmniWords_Flag){
			//------Singleton
			//m_CSegmter.Omni_words_feature_Extracting(ace_op::Delet_0AH_and_20H_in_string(loc_Case.relatin_mention.extent.charseq.c_str()).c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			//------Bin
			m_CSegmter.English_Words_Extractor(Ref_E1.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "F_", "");
			m_CSegmter.English_Words_Extractor(Ref_E2.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "S_", "");
			m_CSegmter.English_Words_Extractor(loc_Context.L_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "L_", "");
			m_CSegmter.English_Words_Extractor(loc_Context.M_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "M_", "");
			m_CSegmter.English_Words_Extractor(loc_Context.R_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "R_", "");

			/*m_CSegmter.English_Radical_Extractor(Ref_E1.extent.charseq.c_str(), Radical_s, Max_Radical_Length, WordsCnt_map, "F_", "");
			m_CSegmter.English_Radical_Extractor(Ref_E2.extent.charseq.c_str(), Radical_s, Max_Radical_Length, WordsCnt_map, "S_", "");
			m_CSegmter.English_Radical_Extractor(loc_Context.L_str.c_str(), Radical_s, Max_Radical_Length, WordsCnt_map, "L_", "");
			m_CSegmter.English_Radical_Extractor(loc_Context.M_str.c_str(), Radical_s, Max_Radical_Length, WordsCnt_map, "M_", "");
			m_CSegmter.English_Radical_Extractor(loc_Context.R_str.c_str(), Radical_s, Max_Radical_Length, WordsCnt_map, "R_", "");*/

		}
		if(WordsCnt_map.empty()){
			//AppCall::Secretary_Message_Box("Empty feature case in CRDC::Generate_ACE_Training_Cases()", MB_OK);
			//The POS Tag with position constraint may result to empty feature;
			continue;
		}
		pair<pair<string, string>, vector<pair<string, float>>>* pCRDC_Case_v = new pair<pair<string, string>, vector<pair<string, float>>>;
		pCRDC_Case_v->first.first  = loc_Case.TYPE;
		pCRDC_Case_v->first.second = loc_Case.SUBTYPE;
		for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
			if(mite->first.find(';') != -1){
				continue;
			}
			if(mite->first.find('\n') == -1){
				pCRDC_Case_v->second.push_back(make_pair(mite->first, mite->second));
			}
			else{
				string test = mite->first;
				ace_op::Delet_0AH_in_string(test);
				pCRDC_Case_v->second.push_back(make_pair(test, mite->second));
			}

		}
		pmTraining_v.push_back(pCRDC_Case_v);
		delete *vite;
	}
	Relation_Case_v.clear();
	for(map<string, _PsDocInfo*>::iterator mite = PsDocsInfo_map.begin(); mite != PsDocsInfo_map.end(); mite++){
		mite->second->Release();
		delete mite->second;
	}

}
void CGREval::Generate_YPench_Evaluation_Port(const char * savepath, vector<Relation_Case*>& Relation_Case_v, DulTYPE_Vctor& pmTraining_v)
{
	map<string, size_t> PositionStructure_m;

	if(false){
		Generating_Synonyms_Framework(Relation_Case_v);
	}
	//Add head mention and extned mention to lexicon
//	RCCOM::Adding_Entity_Extent_and_Head_to_Lexicon(Relation_Case_v, pCRDC->m_WordsSet, 'H');//"E,H,A"

	//-------Write Generated Data Directly;
	m_Output_Training_Data_Dynamically = false;
	ofstream out;
	if(m_Output_Training_Data_Dynamically){
		out.open(savepath);
		if(out.bad())
			return;
		out.clear();
		out.seekp(0, ios::beg);
	}
	//map<string, long> FeatureMap_m;
	//unsigned long FeaturID = 0;
	
	for(vector<Relation_Case*>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
		ostringstream ostrsteam;
		Relation_Case& loc_Case = **vite;
		RelationContext loc_Context;
		ACE_entity_mention& Ref_E1 = loc_Case.first_entity;
		ACE_entity_mention& Ref_E2 = loc_Case.sencond_entity;
		ACE_relation_mention& Ref_relationmention = loc_Case.relatin_mention;
		map<string, float> WordsCnt_map;
		
		Sentop::Extract_ACE_entities_Pair_Context(loc_Case, loc_Context);
		RCCOM::Delet_0AH_and_20H_in_Relation_Case(loc_Case);
		RCCOM::Delet_0AH_and_20H_in_RelationContext(loc_Context);

		//----Subtype and TYPE of entity pair;
		if(false){//(pCRDC->Enitity_TYPE_Flag){
			//------Multiplication Constraint   \mathcal{F}_{(E\_M)}
			/*ostrsteam.str("");
			ostrsteam << Ref_E1.Entity_TYPE << "_" << Ref_E2.Entity_TYPE;
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
			
			if(true){//for generating event network
				ostrsteam.str("");
				ostrsteam << Ref_E1.Entity_SUBSTYPE << "_" << Ref_E2.Entity_SUBSTYPE;
				WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
			}*/
				
			//------Collection Constraint   \mathcal{F}_{(E\_C)}
			/*set<string> Collective_s;
			Collective_s.insert(Ref_E1.Entity_SUBSTYPE);
			Collective_s.insert(Ref_E2.Entity_SUBSTYPE);
			for(set<string>::iterator site = Collective_s.begin(); site != Collective_s.end(); site++){
				ostrsteam << site->data();
				ostrsteam << "_";
			}
			ostrsteam << "EC";
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
			ostrsteam.str("");
			Collective_s.clear();
			Collective_s.insert(Ref_E1.Entity_TYPE);
			Collective_s.insert(Ref_E2.Entity_TYPE);
			for(set<string>::iterator site = Collective_s.begin(); site != Collective_s.end(); site++){
				ostrsteam << site->data();
				ostrsteam << "_";
			}
			ostrsteam << "EC";
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));*/

			//------Position Constraint   \mathcal{F}_{(E\_P)}
			WordsCnt_map.insert(make_pair("E1_"+Ref_E1.Entity_SUBSTYPE, (float)1));
			WordsCnt_map.insert(make_pair("E2_"+Ref_E2.Entity_SUBSTYPE, (float)1));
			WordsCnt_map.insert(make_pair("E1_"+Ref_E1.Entity_TYPE, (float)1));
			WordsCnt_map.insert(make_pair("E2_"+Ref_E2.Entity_TYPE, (float)1));

			//------Singleton   \mathcal{F}_{(E\_S)}
			//WordsCnt_map.insert(make_pair(Ref_E1.Entity_TYPE, (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E2.Entity_TYPE, (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E1.Entity_SUBSTYPE, (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E2.Entity_SUBSTYPE, (float)1));
		}
		//-----head noun;
		if(false){//(pCRDC->HeadNoun_Flag){
			//------Multiplication---------------\mathcal{F}_{(H\_M)}
			//ostrsteam.str("");
			//ostrsteam << Ref_E1.head.charseq << "_" << Ref_E2.head.charseq;
			//WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
			
			//------Singleton---------------\mathcal{F}_{(H\_S)}
			WordsCnt_map.insert(make_pair(Ref_E1.head.charseq, (float)1));
			WordsCnt_map.insert(make_pair(Ref_E2.head.charseq, (float)1));
			
			//------Position	\mathcal{F}_{(H\_P)}
			//WordsCnt_map.insert(make_pair(Ref_E1.head.charseq+"_E1", (float)1));	
			//WordsCnt_map.insert(make_pair(Ref_E2.head.charseq+"_E2", (float)1));

			//------Collection---------------\mathcal{F}_{(H\_C)}
			/*ostrsteam.str("");
			set<string> Collective_s;
			Collective_s.insert(Ref_E1.head.charseq);
			Collective_s.insert(Ref_E2.head.charseq);
			for(set<string>::iterator site = Collective_s.begin(); site != Collective_s.end(); site++){
				ostrsteam << site->data();
				ostrsteam << "_";
			}
			ostrsteam << "HC";
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));*/

			//WordsCnt_map.insert(make_pair(Ref_E1.extent.charseq, (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E2.extent.charseq, (float)1));

			//WordsCnt_map.insert(make_pair(Ref_E1.extent.charseq+"_E1", (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E2.extent.charseq+"_E2", (float)1));
		}
		//-----Entities Structure;
		if(false){//(pCRDC->EntitiesStructure_Flag){
			//------Singleton-------------------------\mathcal{F}_{(S\_S)}
			WordsCnt_map.insert(make_pair(RCCOM::Get_Entity_Positional_Structures(loc_Case), (float)1));

			string locstr = RCCOM::Get_Entity_Positional_Structures(**vite);
			if(PositionStructure_m.find(locstr) == PositionStructure_m.end()){
				PositionStructure_m.insert(make_pair(locstr, 0));
			}
			PositionStructure_m[locstr]++;
		}
		//-----dual types constriant;
		if(false){//(pCRDC->POS_Tag_Flag){
			//------Singleton   \mathcal{F}_{(P\_S)}
			//RCCOM::Local_Adjacent_Words_Singleton_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_");
			//RCCOM::Local_Adjacent_Words_Singleton_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, "_", "_");
			//RCCOM::Local_Adjacent_Words_Singleton_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, "_", "");
			//------Position	\mathcal{F}_{P\_P)}
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_E1");
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, "E1_", "_E2");
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, "E2_", "");

			//------Multiply Entity TYPE and Position
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_"+Ref_E1.Entity_TYPE+"_E1");
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, "E1_"+Ref_E1.Entity_TYPE+"_", "_"+Ref_E2.Entity_TYPE+"_E2");
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, "E2_"+Ref_E2.Entity_TYPE+"_", "");

			//------Multiply Entity TYPE-------$\mathcal{F}_{(PE\_M)}$
			RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_"+Ref_E1.Entity_TYPE);
			RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, Ref_E1.Entity_TYPE+"_", "_"+Ref_E2.Entity_TYPE);
			RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, Ref_E2.Entity_TYPE+"_", "");

			//------Multiply Entity TYPE-------$\mathcal{F}_{(PH\_M)}$
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_"+Ref_E1.head.charseq);
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, Ref_E1.head.charseq+"_", "_"+Ref_E2.head.charseq);
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, Ref_E2.head.charseq+"_", "");
		}
		//-----Omni-words feature;
		if(true){//(pCRDC->OmniWords_Flag){
			//------Singleton
			//m_CSegmter.Omni_words_feature_Extracting(ace_op::Delet_0AH_and_20H_in_string(loc_Case.relatin_mention.extent.charseq.c_str()).c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			//------Bin
			m_CSegmter.Omni_words_feature_Extracting(Ref_E1.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "F_", "");
			m_CSegmter.Omni_words_feature_Extracting(Ref_E2.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "S_", "");
			m_CSegmter.Omni_words_feature_Extracting(loc_Context.L_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "L_", "");
			m_CSegmter.Omni_words_feature_Extracting(loc_Context.M_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "M_", "");
			m_CSegmter.Omni_words_feature_Extracting(loc_Context.R_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "R_", "");
			/*m_CSegmter.Omni_words_feature_Extracting(Ref_E1.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			m_CSegmter.Omni_words_feature_Extracting(Ref_E2.extent.charseq.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			m_CSegmter.Omni_words_feature_Extracting(loc_Context.L_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			m_CSegmter.Omni_words_feature_Extracting(loc_Context.M_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");
			m_CSegmter.Omni_words_feature_Extracting(loc_Context.R_str.c_str(), pCRDC->m_WordsSet, pCRDC->Max_Word_Legnth, WordsCnt_map, "", "");*/
		}
		if(false){//(pCRDC->Segmentation_Flag){
			//m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(ace_op::Delet_0AH_and_20H_in_string(loc_Case.relatin_mention.extent.charseq.c_str()).c_str(), WordsCnt_map, "", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(Ref_E1.extent.charseq.c_str(), WordsCnt_map, "Fs_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(Ref_E2.extent.charseq.c_str(), WordsCnt_map, "Ss_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(loc_Context.L_str.c_str(), WordsCnt_map, "Ls_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(loc_Context.M_str.c_str(), WordsCnt_map, "Ms_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(loc_Context.R_str.c_str(), WordsCnt_map, "Rs_", "");
		}
		if(false){//Generating_Synonyms_Framework
			Generating_Synonyms_Framework_Cases_Features(loc_Context, WordsCnt_map);
		}
		if(true){
			size_t nGram = 1;
			//------Singleton
			//N_Gram_of_Sent_Sequence(ace_op::Delet_0AH_and_20H_in_string(loc_Case.relatin_mention.extent.charseq.c_str()).c_str(), nGram, WordsCnt_map, "", "");
			//------S
			/*N_Gram_of_Sent_Sequence(Ref_E1.extent.charseq.c_str(), nGram, WordsCnt_map, "", "");
			N_Gram_of_Sent_Sequence(Ref_E2.extent.charseq.c_str(), nGram, WordsCnt_map, "", "");
			N_Gram_of_Sent_Sequence(loc_Context.L_str.c_str(), nGram, WordsCnt_map, "", "");
			N_Gram_of_Sent_Sequence(loc_Context.M_str.c_str(), nGram, WordsCnt_map, "", "");
			N_Gram_of_Sent_Sequence(loc_Context.R_str.c_str(), nGram, WordsCnt_map, "", "");*/
			//------Bin
			N_Gram_of_Sent_Sequence(Ref_E1.extent.charseq.c_str(), nGram, WordsCnt_map, "F_", "");
			N_Gram_of_Sent_Sequence(Ref_E2.extent.charseq.c_str(), nGram, WordsCnt_map, "S_", "");
			N_Gram_of_Sent_Sequence(loc_Context.L_str.c_str(), nGram, WordsCnt_map, "L_", "");
			N_Gram_of_Sent_Sequence(loc_Context.M_str.c_str(), nGram, WordsCnt_map, "M_", "");
			N_Gram_of_Sent_Sequence(loc_Context.R_str.c_str(), nGram, WordsCnt_map, "R_", "");
		}
		if(false){//(pCRDC->EntityCLASS_Flag){
			WordsCnt_map.insert(make_pair(Ref_E1.Entity_CLASS, (float)1));
			WordsCnt_map.insert(make_pair(Ref_E2.Entity_CLASS, (float)1));
			WordsCnt_map.insert(make_pair(Ref_E1.LDCTYPE, (float)1));
			WordsCnt_map.insert(make_pair(Ref_E2.LDCTYPE, (float)1));
			//ostrsteam.str("");
			//ostrsteam << "_" << Ref_E1.Entity_CLASS << "_" << Ref_E2.Entity_CLASS << "_";
			//WordsCnt_map.insert(make_pair(ostrsteam.str(), 1));
			//ostrsteam.str("");
			//ostrsteam << "_" << Ref_E1.LDCTYPE << "_" << Ref_E2.LDCTYPE << "_";
			//WordsCnt_map.insert(make_pair(ostrsteam.str(), 1));
		}
		if(WordsCnt_map.empty()){
			//AppCall::Secretary_Message_Box("Empty feature case in CRDC::Generate_ACE_Training_Cases()", MB_OK);
			//The POS Tag with position constraint may result to empty feature;
			continue;
		}
		if(m_Output_Training_Data_Dynamically){
			for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
				out << mite->first << ';' << mite->second << ';';
			}
			out << endl;
			out << loc_Case.TYPE << ";" << loc_Case.SUBTYPE << endl;
		}
		else{
			pair<pair<string, string>, vector<pair<string, float>>>* pCRDC_Case_v = new pair<pair<string, string>, vector<pair<string, float>>>;
			pCRDC_Case_v->first.first  = loc_Case.TYPE;
			pCRDC_Case_v->first.second = loc_Case.SUBTYPE;
			for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
				pCRDC_Case_v->second.push_back(make_pair(mite->first, mite->second));
			}
			pmTraining_v.push_back(pCRDC_Case_v);
			/*for(vector<pair<string, float>>::iterator  outvite = pCRDC_Case_v->second.begin(); outvite != pCRDC_Case_v->second.end(); outvite++){
				//if(FeatureMap_m.find(outvite->first) == FeatureMap_m.end()){
				//	FeatureMap_m.insert(make_pair(outvite->first, FeaturID++));
				//}
				//out << FeatureMap_m[outvite->first] << ';' << outvite->second << ';';
				out << outvite->first << ';' << outvite->second << ';';
			}
			out << endl << pCRDC_Case_v->first << endl;
			//delete pCRDC_Case_v;*/
		}
		delete *vite;
	}
	if(m_Output_Training_Data_Dynamically){
		out.close();
	}
	Relation_Case_v.clear();
}


void CGREval::Generate_Che_Evaluation_Port(vector<Relation_Case*>& Relation_Case_v, DulTYPE_Vctor& pmTraining_v)
{
	for(vector<Relation_Case*>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
		Relation_Case& loc_Case = **vite;
		ACE_entity_mention& Ref_E1 = loc_Case.first_entity;
		ACE_entity_mention& Ref_E2 = loc_Case.sencond_entity;
		ACE_relation_mention& Ref_relationmention = loc_Case.relatin_mention;
		map<string, float> WordsCnt_map;

		//Entity Types
		WordsCnt_map.insert(make_pair(Ref_E1.Entity_TYPE+"_E1", (float)1));
		WordsCnt_map.insert(make_pair(Ref_E1.Entity_SUBSTYPE+"_E1", (float)1));
		WordsCnt_map.insert(make_pair(Ref_E2.Entity_TYPE+"_E2", (float)1));
		WordsCnt_map.insert(make_pair(Ref_E2.Entity_SUBSTYPE+"_E2", (float)1));
		//Order
		WordsCnt_map.insert(make_pair(RCCOM::Get_Four_Types_of_Entity_Positional_Structures(loc_Case), (float)1));

		//map<string, float> testmap1, testmap2;
		//Words and POS Features
		string prix, prox;
		RCCOM::Extracting_Entity_Mention_Context_in_Relation_Mention(Ref_relationmention, Ref_E1, prix, prox);
		ace_op::Delet_0AH_and_20H_in_string(prix);
		ace_op::Delet_0AH_and_20H_in_string(prox);
		RCCOM::Entity_Adjacent_Words_and_POS_Feature_Extracting(m_CSegmter, "E1", prix, prox, WordsCnt_map, 2);

		RCCOM::Extracting_Entity_Mention_Context_in_Relation_Mention(Ref_relationmention, Ref_E2, prix, prox);
		ace_op::Delet_0AH_and_20H_in_string(prix);
		ace_op::Delet_0AH_and_20H_in_string(prox);
		RCCOM::Entity_Adjacent_Words_and_POS_Feature_Extracting(m_CSegmter, "E2", prix, prox, WordsCnt_map, 2);

		if(WordsCnt_map.empty()){
			AppCall::Secretary_Message_Box("Empty feature case in CRDC::Generate_ACE_Training_Cases()", MB_OK);
		}
		/*pair<string, vector<pair<string, float>>>* pCRDC_Case_v = new pair<string, vector<pair<string, float>>>;
		pCRDC_Case_v->first = pCRDC->TYPE_Flag?loc_Case.TYPE:loc_Case.SUBTYPE;
		for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
			pCRDC_Case_v->second.push_back(make_pair(mite->first, mite->second));
		}
		pmTraining_v.push_back(pCRDC_Case_v);*/
		if(m_Output_Training_Data_Dynamically){
			AppCall::Secretary_Message_Box("Empty feature case in CRDC::Generate_ACE_Training_Cases()", MB_OK);
		}
		else{
			pair<pair<string, string>, vector<pair<string, float>>>* pCRDC_Case_v = new pair<pair<string, string>, vector<pair<string, float>>>;
			pCRDC_Case_v->first.first  = loc_Case.TYPE;
			pCRDC_Case_v->first.second = loc_Case.SUBTYPE;
			for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
				pCRDC_Case_v->second.push_back(make_pair(mite->first, mite->second));
			}
			pmTraining_v.push_back(pCRDC_Case_v);
			/*for(vector<pair<string, float>>::iterator  outvite = pCRDC_Case_v->second.begin(); outvite != pCRDC_Case_v->second.end(); outvite++){
				//if(FeatureMap_m.find(outvite->first) == FeatureMap_m.end()){
				//	FeatureMap_m.insert(make_pair(outvite->first, FeaturID++));
				//}
				//out << FeatureMap_m[outvite->first] << ';' << outvite->second << ';';
				out << outvite->first << ';' << outvite->second << ';';
			}
			out << endl << pCRDC_Case_v->first << endl;
			//delete pCRDC_Case_v;*/
		}
		delete *vite;
	}
	Relation_Case_v.clear();
}

void CGREval::Generate_Zhang_Evaluation_Port(vector<Relation_Case*>& Relation_Case_v, DulTYPE_Vctor& pmTraining_v)
{
	map<string, map<size_t, map<size_t, ACE_entity_mention>>*> loc_EntityMention_mm;
	ACE_Entity_Mention_in_Sentence(loc_EntityMention_mm);

	for(vector<Relation_Case*>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
		Relation_Case& loc_Case = **vite;
		ACE_entity_mention& Ref_E1 = loc_Case.first_entity;
		ACE_entity_mention& Ref_E2 = loc_Case.sencond_entity;
		ACE_relation_mention& Ref_relationmention = loc_Case.relatin_mention;
		map<string, float> WordsCnt_map;

		//Entity Types
		WordsCnt_map.insert(make_pair(Ref_E1.Entity_TYPE+"_E1", (float)1));
		WordsCnt_map.insert(make_pair(Ref_E1.Entity_SUBSTYPE+"_E1", (float)1));
		WordsCnt_map.insert(make_pair(Ref_E2.Entity_TYPE+"_E2", (float)1));
		WordsCnt_map.insert(make_pair(Ref_E2.Entity_SUBSTYPE+"_E2", (float)1));
		//Order
		Relation_Envi locRelEnvi;
		string PositiveTYPE;
		PositiveTYPE = Get_Relation_Envi_and_Return_9_Types_Position_Structure(loc_Case, locRelEnvi, loc_EntityMention_mm);
		
		WordsCnt_map.insert(make_pair(PositiveTYPE, (float)1));

		Delet_0AH_and_20H_in_Relation_Case(locRelEnvi);

		//map<string, float> Test_map;
		N_Gram_of_Sent_Sequence_with_Range_Limination(locRelEnvi.E1_Extend.c_str(), 'M', "CME1", 2, 0, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(locRelEnvi.E2_Extend.c_str(), 'M', "CME2", 2, 0, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(  locRelEnvi.E1_Head.c_str(), 'M', "CMH1", 2, 0, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(  locRelEnvi.E2_Head.c_str(), 'M', "CMH2", 2, 0, WordsCnt_map);

		N_Gram_of_Sent_Sequence_with_Range_Limination(     locRelEnvi.CBM1.c_str(), 'B', "CBM1", 2, 4, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(     locRelEnvi.CAM1.c_str(), 'A', "CAM1", 2, 4, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(     locRelEnvi.CBM2.c_str(), 'B', "CBM2", 2, 4, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(     locRelEnvi.CAM2.c_str(), 'A', "CAM2", 2, 4, WordsCnt_map);

	 N_Gram_of_Sent_Sequence_with_Range_Limination(locRelEnvi.E1_inBetw_E2.c_str(), 'M', "IBB", 2, 0, WordsCnt_map);
	 N_Gram_of_Sent_Sequence_with_Range_Limination(locRelEnvi.E1_inBetw_E2.c_str(), 'M', "IBU", 1, 0, WordsCnt_map);

		N_Gram_of_Sent_Sequence_with_Range_Limination(locRelEnvi.E1_Extend.c_str(), 'M', "CME1", 1, 0, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(locRelEnvi.E2_Extend.c_str(), 'M', "CME2", 1, 0, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(  locRelEnvi.E1_Head.c_str(), 'M', "CMH1", 1, 0, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(  locRelEnvi.E2_Head.c_str(), 'M', "CMH2", 1, 0, WordsCnt_map);

		N_Gram_of_Sent_Sequence_with_Range_Limination(     locRelEnvi.CBM1.c_str(), 'B', "CBM1", 1, 4, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(     locRelEnvi.CAM1.c_str(), 'A', "CAM1", 1, 4, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(     locRelEnvi.CBM2.c_str(), 'B', "CBM2", 1, 4, WordsCnt_map);
		N_Gram_of_Sent_Sequence_with_Range_Limination(     locRelEnvi.CAM2.c_str(), 'A', "CAM2", 1, 4, WordsCnt_map);


		if(WordsCnt_map.empty()){
			AppCall::Secretary_Message_Box("Empty feature case in CRDC::Generate_ACE_Training_Cases()", MB_OK);
		}
		/*pair<string, vector<pair<string, float>>>* pCRDC_Case_v = new pair<string, vector<pair<string, float>>>;
		pCRDC_Case_v->first = pCRDC->TYPE_Flag?loc_Case.TYPE:loc_Case.SUBTYPE;
		for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
			pCRDC_Case_v->second.push_back(make_pair(mite->first, mite->second));
		}
		pmTraining_v.push_back(pCRDC_Case_v);*/
		if(m_Output_Training_Data_Dynamically){
			AppCall::Secretary_Message_Box("Empty feature case in CRDC::Generate_ACE_Training_Cases()", MB_OK);
		}
		else{
			pair<pair<string, string>, vector<pair<string, float>>>* pCRDC_Case_v = new pair<pair<string, string>, vector<pair<string, float>>>;
			pCRDC_Case_v->first.first  = loc_Case.TYPE;
			pCRDC_Case_v->first.second = loc_Case.SUBTYPE;
			for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
				pCRDC_Case_v->second.push_back(make_pair(mite->first, mite->second));
			}
			pmTraining_v.push_back(pCRDC_Case_v);
			/*for(vector<pair<string, float>>::iterator  outvite = pCRDC_Case_v->second.begin(); outvite != pCRDC_Case_v->second.end(); outvite++){
				//if(FeatureMap_m.find(outvite->first) == FeatureMap_m.end()){
				//	FeatureMap_m.insert(make_pair(outvite->first, FeaturID++));
				//}
				//out << FeatureMap_m[outvite->first] << ';' << outvite->second << ';';
				out << outvite->first << ';' << outvite->second << ';';
			}
			out << endl << pCRDC_Case_v->first << endl;
			//delete pCRDC_Case_v;*/
		}
		delete *vite;
	}

	if(!loc_EntityMention_mm.empty()){
		for(map<string, map<size_t, map<size_t, ACE_entity_mention>>*>::iterator mmite = loc_EntityMention_mm.begin(); mmite != loc_EntityMention_mm.end(); mmite++){
			delete mmite->second;
		}
		loc_EntityMention_mm.clear();
	}
	Relation_Case_v.clear();
}

void CGREval::ACE_Entity_Mention_in_Sentence(map<string, map<size_t, map<size_t, ACE_entity_mention>>*>& EntityMention_mm)
{
	AppCall::Secretary_Message_Box("Implementing this method, the corpus folder must be in the righ place, the same as the generating relation cases!", MB_OK);

	ACE_Corpus m_ACE_Corpus;
	SXMLer m_SXMLer;
	string ACECorpusFolder = pCRDC->ACECorpusFolder;
	m_SXMLer.Xercesc_ACE_Folder_Files_Filter(pCRDC->ACECorpusFolder, m_ACE_Corpus);
	map<string, ACE_entity>& ACE_Entity_Info_map = m_ACE_Corpus.ACE_Entity_Info_map;
	
	for(map<string, ACE_entity>::iterator mite = ACE_Entity_Info_map.begin(); mite != ACE_Entity_Info_map.end(); mite++){
		for(vector<ACE_entity_mention>::iterator vite = mite->second.entity_mention_v.begin(); vite != mite->second.entity_mention_v.end(); vite++){
			if(EntityMention_mm.find(vite->DOCID) == EntityMention_mm.end()){
				EntityMention_mm[vite->DOCID];
				EntityMention_mm[vite->DOCID] = new  map<size_t, map<size_t, ACE_entity_mention>>;
			}
			map<size_t, map<size_t, ACE_entity_mention>>& locMen_m = *EntityMention_mm[vite->DOCID];
			if(locMen_m.find(vite->extent.START) == locMen_m.end()){
				locMen_m[vite->extent.START];
			}
			if(locMen_m[vite->extent.START].find(vite->extent.END) == locMen_m[vite->extent.START].end()){
				locMen_m[vite->extent.START].insert(make_pair(vite->extent.END, *vite));
			}
			else{
				//AppCall::Secretary_Message_Box("Duplicated Data in: CGREval::ACE_Entity_Mention_in_Sentence()", MB_OK);
			}
		}
	}
}

string CGREval::Get_Relation_Envi_and_Return_9_Types_Position_Structure(Relation_Case& pmRelCase, Relation_Envi& pmRelEnvi, map<string, map<size_t, map<size_t, ACE_entity_mention>>*>& pmEntityMention_mm)
{
	if(pmEntityMention_mm.find(pmRelCase.DOCID) == pmEntityMention_mm.end()){
		ostringstream ostrsteam;
		ostrsteam << "Missing of Data info in: CGREval::Get_Relation_Envi()" <<endl;
		ostrsteam << pmRelCase.DOCID << endl;
		ostrsteam << "size of DOC: " << pmEntityMention_mm.size();
		ostrsteam << "Please insure that the ACECorpusFolder is on the right place..." << endl;
		AppCall::Secretary_Message_Box(ostrsteam.str(), MB_OK);
		return "";
	}

	ACE_relation_mention& relatin_mention = pmRelCase.relatin_mention;
	map<size_t, map<size_t, ACE_entity_mention>>& docEMention_mm = *pmEntityMention_mm[pmRelCase.DOCID];
	vector<ACE_entity_mention> inEMention_v;
	
	for(map<size_t, map<size_t, ACE_entity_mention>>::iterator mmite = docEMention_mm.begin(); mmite != docEMention_mm.end(); mmite++){
		for(map<size_t, ACE_entity_mention>::iterator mite = mmite->second.begin(); mite != mmite->second.end(); mite++){
			if((mmite->first >= relatin_mention.extent.START) && (mite->first <= relatin_mention.extent.END)){
				inEMention_v.push_back(mite->second);
			}
		}
	}
	size_t Sentlength = NLPOP::Get_Chinese_Sentence_Length_Counter(relatin_mention.extent.charseq.c_str());
	size_t offset = relatin_mention.extent.START;
	relatin_mention.extent.START = 0;
	relatin_mention.extent.END -= offset;
	vector<bool> MutexInfo_v;
	for(size_t i = 0; i < Sentlength; i++){
		MutexInfo_v.push_back(false);
	}
	for(size_t i = 0; i < inEMention_v.size(); i++){
		inEMention_v[i].extent.START -= offset;
		inEMention_v[i].extent.END -= offset;
		inEMention_v[i].head.START -= offset;
		inEMention_v[i].head.END -= offset;
		for(size_t j = inEMention_v[i].extent.START; j <= inEMention_v[i].extent.END; j++){
			MutexInfo_v[j] = true;
		}
	}
	pmRelCase.first_entity.extent.START -= offset;
	pmRelCase.first_entity.extent.END -= offset;
	pmRelCase.first_entity.head.START -= offset;
	pmRelCase.first_entity.head.END -= offset;

	pmRelCase.sencond_entity.extent.START -= offset;
	pmRelCase.sencond_entity.extent.END -= offset;
	pmRelCase.sencond_entity.head.START -= offset;
	pmRelCase.sencond_entity.head.END -= offset;
	//=============================================================================
	string PositiveTYPE;
	ACE_extent em1, em2;
	if(pmRelCase.first_entity.extent.START <= pmRelCase.sencond_entity.extent.START){
		em1 = pmRelCase.first_entity.extent;
		em2 = pmRelCase.sencond_entity.extent;
	}
	else{
		em1 = pmRelCase.sencond_entity.extent;
		em2 = pmRelCase.first_entity.extent;
	}

	if((em1.START == em2.START) && (em1.END == em2.END)){
		PositiveTYPE = "Superposition";
	}
	else if((em1.START <= em2.START) && (em1.END >= em2.START)){
		bool Nested_Nested_Flag = false;
		for(size_t i = 0; i < inEMention_v.size(); i++){
			if((inEMention_v[i].extent.START <= em2.START) && (inEMention_v[i].extent.END >= em2.END)){
				if((inEMention_v[i].extent.START != em1.START) && (inEMention_v[i].extent.END != em1.END)){
					Nested_Nested_Flag = true;
					break;
				}
			}
		}
		if(Nested_Nested_Flag){
			PositiveTYPE = "Nested_Nested";
		}
		else{
			PositiveTYPE = "Nested";
		}
	}
	else if(em1.END < em2.START){
		bool Interval_Flag = false;
		for(size_t j = em1.END+1; j < em2.START; j++){
			if(MutexInfo_v[j]){
				Interval_Flag = true;
				break;
			}
		}
		bool First_Nested_Flag = false;
		bool Second_Nested_Flag = false;
		for(size_t i = 0; i < inEMention_v.size(); i++){
			if((inEMention_v[i].extent.START == em1.START) && (inEMention_v[i].extent.END == em1.END)){
				continue;
			}
			if((inEMention_v[i].extent.START == em2.START) && (inEMention_v[i].extent.END == em2.END)){
				continue;
			}
			if((inEMention_v[i].extent.START <= em1.START) && (inEMention_v[i].extent.END >= em1.END)){
				First_Nested_Flag = true;
			}
			if((inEMention_v[i].extent.START <= em2.START) && (inEMention_v[i].extent.END >= em2.END)){
				Second_Nested_Flag = true;
			}
		}
		if(!Interval_Flag){
			if(First_Nested_Flag && Second_Nested_Flag){
				PositiveTYPE = "Nested_Nested_Adjacent";
			}
			else if(!First_Nested_Flag && !Second_Nested_Flag){
				PositiveTYPE = "Adjacent";
			}
			else{
				PositiveTYPE = "Nested_Adjacent";
			}
		}
		else{
			if(First_Nested_Flag && Second_Nested_Flag){
				PositiveTYPE = "Nested_Nested_Separated";
			}
			else if(!First_Nested_Flag && !Second_Nested_Flag){
				PositiveTYPE = "Separated";
			}
			else{
				PositiveTYPE = "Nested_Separated";
			}
		}
	}
	else{
		AppCall::Secretary_Message_Box("Unknown Position Structure...", MB_OK);
	}
	//=============================================================================
	size_t CAM, CBM, count;
	const char* SentChar = pmRelCase.relatin_mention.extent.charseq.c_str();
	//-----------------------CBM1-------------------------
	CBM = 0;
	count = 0;
	for(int i = pmRelCase.first_entity.extent.START-1; i >= 0; i--){
		if(MutexInfo_v[i]){
			break;	
		}
		CBM = i;
		count++;
	}
	pmRelEnvi.CBM1 = Sentop::Get_Substr_by_Chinese_Character_Cnt(SentChar, CBM, count);
	//-----------------------CAM1-------------------------
	CAM = NLPOP::Get_Chinese_Sentence_Length_Counter(SentChar);
	count = 0;
	for(int i = pmRelCase.first_entity.extent.END+1; i < (int)MutexInfo_v.size(); i++){
		if(MutexInfo_v[i]){
			break;	
		}
		CAM = i;
		count++;
	}
	pmRelEnvi.CAM1 = Sentop::Get_Substr_by_Chinese_Character_Cnt(SentChar, pmRelCase.first_entity.extent.END+1, count);
	//-----------------------CBM2-------------------------
	CBM = 0;
	count = 0;
	for(int i = pmRelCase.sencond_entity.extent.START-1; i >= 0; i--){
		if(MutexInfo_v[i]){
			break;	
		}
		CBM = i;
		count++;
	}
	pmRelEnvi.CBM2 = Sentop::Get_Substr_by_Chinese_Character_Cnt(SentChar, CBM, count);
	//-----------------------CAM2-------------------------
	CAM = NLPOP::Get_Chinese_Sentence_Length_Counter(SentChar);
	count = 0;
	for(int i = pmRelCase.sencond_entity.extent.END+1; i < (int)MutexInfo_v.size(); i++){
		if(MutexInfo_v[i]){
			break;	
		}
		CAM = i;
		count++;
	}
	pmRelEnvi.CAM2 = Sentop::Get_Substr_by_Chinese_Character_Cnt(SentChar, pmRelCase.sencond_entity.extent.END+1, count);
//-----------------------E1_inBetw_E2-------------------------	
	if(pmRelCase.sencond_entity.extent.START > pmRelCase.first_entity.extent.END){
		count = pmRelCase.sencond_entity.extent.START-pmRelCase.first_entity.extent.END-1;
		pmRelEnvi.E1_inBetw_E2 = Sentop::Get_Substr_by_Chinese_Character_Cnt(SentChar, pmRelCase.first_entity.extent.END+1, count);
	}
	else if(pmRelCase.first_entity.extent.START > pmRelCase.sencond_entity.extent.END){
		count = pmRelCase.first_entity.extent.START - pmRelCase.sencond_entity.extent.END-1;
		pmRelEnvi.E1_inBetw_E2 = Sentop::Get_Substr_by_Chinese_Character_Cnt(SentChar, pmRelCase.sencond_entity.extent.END+1, count);
	}
	else{
		pmRelEnvi.E1_inBetw_E2 = "";
	}
	//SentChar;
	pmRelEnvi.E1_Extend = pmRelCase.first_entity.head.charseq;
	pmRelEnvi.E1_Head = pmRelCase.first_entity.head.charseq;
	pmRelEnvi.E2_Extend = pmRelCase.sencond_entity.extent.charseq;
	pmRelEnvi.E2_Head = pmRelCase.sencond_entity.head.charseq;

	return PositiveTYPE;
}



void CGREval::Delet_0AH_and_20H_in_Relation_Case(Relation_Envi& pmRelEnvi)
{
	ace_op::Delet_0AH_and_20H_in_string(pmRelEnvi.CAM1);
	ace_op::Delet_0AH_and_20H_in_string(pmRelEnvi.CAM2);
	ace_op::Delet_0AH_and_20H_in_string(pmRelEnvi.CBM1);
	ace_op::Delet_0AH_and_20H_in_string(pmRelEnvi.CBM2);
	ace_op::Delet_0AH_and_20H_in_string(pmRelEnvi.E1_Extend);
	ace_op::Delet_0AH_and_20H_in_string(pmRelEnvi.E1_Head);
	ace_op::Delet_0AH_and_20H_in_string(pmRelEnvi.E1_inBetw_E2);
	ace_op::Delet_0AH_and_20H_in_string(pmRelEnvi.E2_Extend);
	ace_op::Delet_0AH_and_20H_in_string(pmRelEnvi.E2_Head);
}

void CGREval::N_Gram_of_Sent_Sequence(const char* sentchar, size_t range, map<string, float>& WordsCnt_map, string prix = "", string prox = "")
{
	//map<string, float> loc_WordsCnt_map;
	ostringstream ostrsteam;
	vector<string> locchar_v;
	NLPOP::charseq_to_vector(sentchar, locchar_v);
	size_t Counter = 0;
	for(size_t i = 0; i < locchar_v.size(); i++){
		size_t limit = i + range;
		if(limit > locchar_v.size()){
			limit = locchar_v.size();
		}
		string str;
		for(size_t j = i; j < limit; j++){
			str += locchar_v[j];
			ostrsteam.str("");
			ostrsteam << prix << str << prox;
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
			//loc_WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
		}
	}

}


void CGREval::N_Gram_of_Sent_Sequence_with_Range_Limination(const char* sentchar, char CharTYPE, string FeatureID, size_t n, size_t range, map<string, float>& nGramFeature_map)
{
	char sChar[3];
	sChar[2]=0;
	string locstr;
	if(CharTYPE == 'A'){
		size_t length = strlen(sentchar);
		size_t charcnt = 0;
		for(size_t i = 0; i < length; ){
			sChar[0] = sentchar[i++];
			sChar[1] = 0;
			if(sChar[0] < 0 ){
				sChar[1] = sentchar[i++];
			}
			locstr += sChar;
			charcnt++;
			if(!(charcnt < range)){
				break;
			}
		}
	}
	else if(CharTYPE == 'B'){
		vector<string> locchar_v;
		NLPOP::charseq_to_vector(sentchar, locchar_v);
		size_t Counter = 0;
		for(vector<string>::reverse_iterator rvite = locchar_v.rbegin(); rvite != locchar_v.rend(); rvite++){
			locstr = rvite->data()+locstr;
			Counter++;
			if(!(Counter < range)){
				break;
			}
		}
	}
	else{
		locstr = sentchar;
	}
	vector<string> char_v;
	NLPOP::charseq_to_vector(locstr.c_str(), char_v);
	int limit = char_v.size()-n+1;
	for(int i = 0; i < limit; i++){
		ostringstream ostrsteam;
		for(size_t j = i, k = 0; ((j < char_v.size()) && (k < n)); j++, k++){
			ostrsteam << char_v[j];
		}
		ostrsteam << "_" << FeatureID;
		nGramFeature_map.insert(make_pair(ostrsteam.str(), (float)1));
	}
}



