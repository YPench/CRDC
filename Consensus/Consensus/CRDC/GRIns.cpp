#include "stdafx.h"
#include "GRIns.h"
#include "ConvertUTF.h"
#include "RCCom.h"
#include "CRDC.h"

CGRIns::CGRIns(CSegmter& ref_Segmter): m_CSegmter(ref_Segmter)
{	
	
}
CGRIns::~CGRIns()
{
}


//-----------------------------------Extract Positive and Negative Relation Instances
void CGRIns::Generate_Positive_And_Negetive_ACE_Relation_Cases(string savepath, ACE_Corpus& m_ACE_Corpus)
{
	SENID = 0;
	string RelationCaseFilePath = savepath;
	map<string, ACE_entity>& ACE_Entity_Info_map = m_ACE_Corpus.ACE_Entity_Info_map;
	deque<ACE_relation>& ACE_Relation_Info_d = m_ACE_Corpus.ACE_Relation_Info_d;
	map<string, ACE_sgm>& ACE_sgm_mmap = m_ACE_Corpus.ACE_sgm_mmap;

	list<Relation_Case> totall_Case_l;

	Extract_Sentence_with_Two_Named_Entities(m_ACE_Corpus, totall_Case_l);

	Delete_Sentence_Contianing_Positive_Relation_Case(totall_Case_l, ACE_Relation_Info_d);

	Adding_Positive_Training_Case_in_Relation_Mention(m_ACE_Corpus, totall_Case_l);

	//For negetive case typ: 3
	//Adding_Positive_Training_Case_in_Relation_Mention(m_ACE_Corpus, totall_Case_l);

	if(pCRDC->Generate_ICTCLAS_LEXICAL_FLAG){
		RCCOM::Generate_ICTCLAS_Lexicon(m_CSegmter, totall_Case_l);
	}
	if(pCRDC->CaseCheck_Flag){
		for(list<Relation_Case>::iterator lite = totall_Case_l.begin(); lite != totall_Case_l.end(); lite++){
			ace_op::Check_Relation_Cases(*lite);
		}
	}
	ostringstream ostream;
	ostream.str("");
	ostream << "The number of generated Relation Cases: " << totall_Case_l.size() << '\n';
	ostream << "是否需要保存结果？" << '\n';
	if(IDYES != AppCall::Secretary_Message_Box(ostream.str(), MB_YESNOCANCEL)){
		return;
	}
	ace_op::Output_Relation_Case(RelationCaseFilePath.c_str(), totall_Case_l);

	string infopath = DATA_FOLDER;
	infopath += "CRDC\\Info.dat";
	RCCOM::Get_Relation_Case_Info(infopath.c_str(), totall_Case_l);
}

void CGRIns::Adding_Positive_Training_Case_in_Relation_Mention(ACE_Corpus& m_ACE_Corpus, list<Relation_Case>& Relation_Case_l)
{
	deque<ACE_relation>& ACE_Relation_Info_d = m_ACE_Corpus.ACE_Relation_Info_d;
	map<string, ACE_entity>& ACE_Entity_Info_map = m_ACE_Corpus.ACE_Entity_Info_map;

	map<string, ACE_entity_mention> EntityMention_map;
	RCCOM::Get_Entity_Mention_extent_Map(ACE_Entity_Info_map, EntityMention_map);
	
	for(size_t i = 0; i < ACE_Relation_Info_d.size(); i++){
		for(vector<ACE_relation_mention>::iterator rmvite = ACE_Relation_Info_d[i].relation_mention_v.begin();  rmvite != ACE_Relation_Info_d[i].relation_mention_v.end(); rmvite++){
			Relation_Case loc_case;
			loc_case.SENID = SENID++;
			loc_case.DOCID = rmvite->DOCID;
			loc_case.TYPE = ACE_Relation_Info_d[i].TYPE;
			loc_case.SUBTYPE = ACE_Relation_Info_d[i].SUBTYPE;
			loc_case.relatin_mention = *rmvite;
			for(multimap<string, ACE_relation_mention_arg>::iterator emvite = rmvite->arg_mention_mmap.begin(); emvite != rmvite->arg_mention_mmap.end(); emvite++){
				if(!strcmp("Arg-1", emvite->first.c_str())){
					if(EntityMention_map.find(emvite->second.REFID) != EntityMention_map.end()){
						loc_case.first_entity = EntityMention_map[emvite->second.REFID];
					}
				}
				if(!strcmp("Arg-2", emvite->first.c_str())){
					if(EntityMention_map.find(emvite->second.REFID) != EntityMention_map.end()){
						loc_case.sencond_entity = EntityMention_map[emvite->second.REFID];
					}
				}
			}
			Relation_Case_l.push_back(loc_case);
		}
	}
}



//-----------------------------------Extract Sentence with Two Named Entities
void CGRIns::Delete_Sentence_Contianing_Positive_Relation_Case(list<Relation_Case>& Relation_Case_l, deque<ACE_relation>& ACE_Relation_Info_d)
{
	bool Erase_Flag;
	for(list<Relation_Case>::iterator lite = Relation_Case_l.begin(); lite != Relation_Case_l.end(); ){
		Erase_Flag = false;
		for(deque<ACE_relation>::iterator dite = ACE_Relation_Info_d.begin(); dite != ACE_Relation_Info_d.end(); dite++){
			for(vector<ACE_relation_mention>::iterator vite =  dite->relation_mention_v.begin(); vite !=  dite->relation_mention_v.end(); vite++){
				if(vite->ID.find(lite->DOCID) == -1){
					continue;
				}
				ACE_extent Arg_1, Arg_2;
				for(multimap<string, ACE_relation_mention_arg>::iterator mmite = vite->arg_mention_mmap.begin(); mmite != vite->arg_mention_mmap.end(); mmite++){
					if(!strcmp(mmite->second.ROLE.c_str(), "Arg-1")){
						Arg_1 = mmite->second.extent;
					}
					if(!strcmp(mmite->second.ROLE.c_str(), "Arg-2")){
						Arg_2 = mmite->second.extent;
					}
				}
				if((Arg_1.START == lite->first_entity.extent.START) && (Arg_1.END == lite->first_entity.extent.END) && 
					(Arg_2.START == lite->sencond_entity.extent.START) && (Arg_2.END == lite->sencond_entity.extent.END)){
					lite = Relation_Case_l.erase(lite);
					//------------------------------------------------------------------------------------
					//The following code, used for negetive case influence analyse
					//For negetive case typ: 3
					/*lite->TYPE = dite->TYPE;
					lite->SUBTYPE = dite->SUBTYPE;
					lite++;*/
					//------------------------------------------------------------------------------------
					Erase_Flag = true;
					break;
				}
			}
			if(Erase_Flag){
				break;
			}
		}
		if(!Erase_Flag){
			lite++;
		}
	}
}

void CGRIns::Filter_sgm_sentence_with_Relation_Mention(ACE_Corpus& ACE_Corpus, map<string, list<pair<string, pair<size_t,size_t>>>>& pm_ACE_DocSentence_map)
{
	map<string, list<pair<string, pair<size_t,size_t>>>> RelationMention_map;
	deque<ACE_relation>& ACE_Relation_Info_d = ACE_Corpus.ACE_Relation_Info_d;

	for(size_t i = 0; i < ACE_Relation_Info_d.size(); i++){
		for(vector<ACE_relation_mention>::iterator rmvite = ACE_Relation_Info_d[i].relation_mention_v.begin();  rmvite != ACE_Relation_Info_d[i].relation_mention_v.end(); rmvite++){
			
			if(RelationMention_map.find(rmvite->DOCID) == RelationMention_map.end()){
				RelationMention_map[rmvite->DOCID];
			}
			RelationMention_map[rmvite->DOCID].push_back(make_pair(rmvite->extent.charseq, make_pair(rmvite->extent.START, rmvite->extent.END)));
		}
	}
	/*bool Erase_Flag;
	for(map<string, list<pair<string, pair<size_t,size_t>>>>::iterator mlite = pm_ACE_DocSentence_map.begin(); mlite != pm_ACE_DocSentence_map.end(); mlite++){
		if(RelationMention_map.find(mlite->first) == RelationMention_map.end()){
			ostringstream ostream;
			ostream << "Doc_ID isn't found..." << endl;
			ostream << "File Name: " << mlite->first;
			AppCall::Secretary_Message_Box(ostream.str(), MB_OK);
			//AppCall::Secretary_Message_Box("Doc_ID isn't found...", MB_OK);
		}
		list<pair<string, pair<size_t,size_t>>>::iterator lite;
		for(lite = mlite->second.begin(); lite != mlite->second.end(); ){
			Erase_Flag = false;
			for(list<pair<string, pair<size_t,size_t>>>::iterator rlite = RelationMention_map[mlite->first].begin(); rlite != RelationMention_map[mlite->first].end(); rlite++){
				if(lite->second.first >= rlite->second.first && lite->second.second <= rlite->second.second){//************** ||
					lite = mlite->second.erase(lite);
					Erase_Flag = true;
					break;
				}
			}
			if(!Erase_Flag){
				lite++;
			}
		}
	}*/
	pm_ACE_DocSentence_map.clear();
	for(map<string, list<pair<string, pair<size_t,size_t>>>>::iterator mlite = RelationMention_map.begin(); mlite != RelationMention_map.end(); mlite++){
		if(pm_ACE_DocSentence_map.find(mlite->first) == pm_ACE_DocSentence_map.end()){
				pm_ACE_DocSentence_map[mlite->first];
		}
		for(list<pair<string, pair<size_t,size_t>>>::iterator rlite = RelationMention_map[mlite->first].begin(); rlite != RelationMention_map[mlite->first].end(); rlite++){	
			pm_ACE_DocSentence_map[mlite->first].push_back(*rlite);
		}
	}
}


//-----------------------------------Extract Sentence with Two Named Entities
void CGRIns::Extract_Sentence_with_Two_Named_Entities(ACE_Corpus& ACE_Corpus, list<Relation_Case>& Relation_Case_l)
{
	map<string, ACE_entity>& ACE_Entity_Info_map = ACE_Corpus.ACE_Entity_Info_map;
	map<string, ACE_sgm>& ACE_sgm_mmap = ACE_Corpus.ACE_sgm_mmap;
	map<string, list<pair<string, pair<size_t,size_t>>>> ACE_DocSentence_map;//<DOCID, list_pair: <Sentence, (START, ENd);>
	map<string, vector<pair<ACE_extent, ACE_entity_mention*>>> EntityMentionInfo_map;//Save each entity mention info in doc.
	//------------------------------------------------------------------------
	ace_op::Segment_ACE_sgm_to_Sentence(ACE_sgm_mmap, ACE_DocSentence_map);

	//------------------------------------------------------------------------
	//Only for negetive case influence analyse
	//Filter_sgm_sentence_with_Relation_Mention(ACE_Corpus, ACE_DocSentence_map);

	//------------------------------------------------------------------------
	//Extract each entity mention info in doc.
	for(map<string, ACE_entity>::iterator mite = ACE_Entity_Info_map.begin(); mite != ACE_Entity_Info_map.end(); mite++){
		for(vector<ACE_entity_mention>::iterator vite = mite->second.entity_mention_v.begin(); vite != mite->second.entity_mention_v.end(); vite++){
			if(EntityMentionInfo_map.find(vite->DOCID) == EntityMentionInfo_map.end()){
				EntityMentionInfo_map[vite->DOCID];
			}
			EntityMentionInfo_map[vite->DOCID].push_back(make_pair(vite->extent, &(*vite)));
		}
	}

	//------------------------------------------------------------------------
	//<DOCID, list_pair: <Sentence, <START, ENd> >
	ACE_entity_mention* First_Enitity;
	pair<size_t, size_t> FirstMentionExtent;
	
	for(map<string, list<pair<string, pair<size_t,size_t>>>>::iterator mite = ACE_DocSentence_map.begin(); mite != ACE_DocSentence_map.end(); mite++){

		for(list<pair<string, pair<size_t,size_t>>>::iterator sgmlite = mite->second.begin(); sgmlite != mite->second.end(); sgmlite++, SENID++){
			vector<pair<ACE_extent, ACE_entity_mention*>>& entityref_v = EntityMentionInfo_map[mite->first];
			for(vector<pair<ACE_extent, ACE_entity_mention*>>::iterator fvist = entityref_v.begin(); fvist != entityref_v.end(); fvist++){
				//Check wether the entity set is accordance with original doc;
				if(strcmp(fvist->first.charseq.c_str(), Sentop::Get_Substr_by_Chinese_Character_Cnt(ACE_sgm_mmap[mite->first].DOC.c_str(), fvist->first.START, fvist->first.END + 1 - fvist->first.START).c_str())){
					ostringstream ostream;
					ostream << "文档：" << mite->first << endl << "位置：" << fvist->first.START<< endl<< "命名实体：" << fvist->first.charseq << endl;
					ostream << "文档字符：" << Sentop::Get_Substr_by_Chinese_Character_Cnt(ACE_sgm_mmap[mite->first].DOC.c_str(), fvist->first.START, fvist->first.END + 1 - fvist->first.START) << endl;
					if(IDYES != AppCall::Secretary_Message_Box(ostream.str(), MB_YESNOCANCEL)){
						return;
					}
					continue;
				}
				if((sgmlite->second.first  <= fvist->first.START) && (sgmlite->second.second >= fvist->first.END)){
					First_Enitity = fvist->second;
					FirstMentionExtent.first = fvist->first.START;
					FirstMentionExtent.second = fvist->first.END;
					vector<pair<ACE_extent, ACE_entity_mention*>>::iterator svist = entityref_v.begin();
					
					for(; svist != entityref_v.end(); svist++){
						if((sgmlite->second.first <= svist->first.START) && (sgmlite->second.second >= svist->first.END)){ 
							if((FirstMentionExtent.first != svist->first.START) || (FirstMentionExtent.second != svist->first.END)){

								Relation_Case loc_Relation_Case;
								loc_Relation_Case.SENID = SENID;
								loc_Relation_Case.DOCID = mite->first;
								loc_Relation_Case.TYPE = NEGETIVE;
								loc_Relation_Case.SUBTYPE = NEGETIVE;
								loc_Relation_Case.relatin_mention.DOCID = mite->first;
								loc_Relation_Case.relatin_mention.ID = "";
								loc_Relation_Case.relatin_mention.LEXICALCONDITION = "";
								loc_Relation_Case.relatin_mention.extent.charseq = sgmlite->first;
								loc_Relation_Case.relatin_mention.extent.START = sgmlite->second.first;
								loc_Relation_Case.relatin_mention.extent.END = sgmlite->second.second;
								
								loc_Relation_Case.first_entity = *First_Enitity;
								loc_Relation_Case.sencond_entity = *(svist->second);
								
								Relation_Case_l.push_back(loc_Relation_Case);
							}
						}
					}//svist
				}
			}//fvist
		}//for(list<pair<
	}//for(map<string,
}