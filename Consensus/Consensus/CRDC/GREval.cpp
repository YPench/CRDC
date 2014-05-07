#include "stdafx.h"
#include "GREval.h"
#include "ConvertUTF.h"
#include "RCCom.h"
#include "CRDC.h"
#include "SXMLer.h"
CGREval::CGREval(CSegmter& ref_Segmter): m_CSegmter(ref_Segmter)
{	
	
}
CGREval::~CGREval()
{

}

//-------------------------------Generate Training and Testing Relation Cases

void CGREval::Generate_YPench_Evaluation_Port(vector<Relation_Case>& Relation_Case_v, FeatureVctor& pmTraining_v)
{
	map<string, size_t> PositionStructure_m;

	for(vector<Relation_Case>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
		ostringstream ostrsteam;
		Relation_Case& loc_Case = *vite;
		RelationContext loc_Context;
		ACE_entity_mention& Ref_E1 = loc_Case.first_entity;
		ACE_entity_mention& Ref_E2 = loc_Case.sencond_entity;
		ACE_relation_mention& Ref_relationmention = loc_Case.relatin_mention;
		map<string, float> WordsCnt_map;
		
		Sentop::Extract_ACE_entities_Pair_Context(loc_Case, loc_Context);
		RCCOM::Delet_0AH_and_20H_in_Relation_Case(loc_Case);
		RCCOM::Delet_0AH_and_20H_in_RelationContext(loc_Context);

		//----Subtype and TYPE of entity pair;
		if(pCRDC->Enitity_Subtype_Flag){
			ostrsteam.str("");
			ostrsteam << Ref_E1.Entity_SUBSTYPE << "_" << Ref_E2.Entity_SUBSTYPE;
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));

			//WordsCnt_map.insert(make_pair(Ref_E1.Entity_SUBSTYPE, (float)1));
			//WordsCnt_map.insert(make_pair(Ref_E2.Entity_SUBSTYPE, (float)1));
			
			ostrsteam.str("");
			ostrsteam << Ref_E1.Entity_TYPE << "_" << Ref_E2.Entity_TYPE;
			WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));

			//WordsCnt_map.insert(make_pair(Ref_E1.Entity_TYPE, (float)1)); //***????
			//WordsCnt_map.insert(make_pair(Ref_E2.Entity_TYPE, (float)1));
		}
		//-----positional structure;
		if(pCRDC->PositionalStructure_Flag){
			WordsCnt_map.insert(make_pair(RCCOM::Get_Entity_Positional_Structures(*vite), (float)1));
			//WordsCnt_map.insert(make_pair(RCCOM::New_Get_Entity_Positional_Structures(*vite), 1));
			/*string locstr = RCCOM::Get_Entity_Positional_Structures(*vite);
			if(PositionStructure_m.find(locstr) == PositionStructure_m.end()){
				PositionStructure_m.insert(make_pair(locstr, 0));
			}
			PositionStructure_m[locstr]++;*/
		}
		//-----head noun;
		if(pCRDC->EntityHead_Flag){
			//ostrsteam.str("");
			//ostrsteam << "H1_" << first_head_str << "_" << second_head_str << "_H2";
			//WordsCnt_map.insert(make_pair(ostrsteam.str(), 1));
			if(Ref_E1.head.charseq.length() > 0){
				WordsCnt_map.insert(make_pair(Ref_E1.head.charseq, (float)1));
				//WordsCnt_map.insert(make_pair(Ref_E1.head.charseq+"_E1", (float)1));
			}
			if(Ref_E2.head.charseq.length() > 0){
				WordsCnt_map.insert(make_pair(Ref_E2.head.charseq, (float)1));
				//WordsCnt_map.insert(make_pair(Ref_E2.head.charseq+"_E2", (float)1));
			}
		}
		if(pCRDC->EntityCLASS_Flag){
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
		//-----dual types constriant;
		if(pCRDC->SoftConstraint_Flag){
			RCCOM::Local_Adjacent_Words_Singleton_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_");
			RCCOM::Local_Adjacent_Words_Singleton_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, "_", "_");
			RCCOM::Local_Adjacent_Words_Singleton_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, "_", "");

			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "_l");
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, "m_", "_m");
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, "r_", "");

			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.L_str.c_str(), WordsCnt_map, "", "__"+Ref_E1.Entity_TYPE);
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.M_str.c_str(), WordsCnt_map, Ref_E1.Entity_TYPE+"__", "__"+Ref_E2.Entity_TYPE);
			//RCCOM::Adjacent_Words_POS_Feature_Extracting(m_CSegmter, loc_Context.R_str.c_str(), WordsCnt_map, Ref_E2.Entity_TYPE+"__", "");
		}
		//-----relation mention;	
		//WordsCnt_map.insert(make_pair(first_entity_mention_str, 1));
		//WordsCnt_map.insert(make_pair(second_entity_mention_str, 1));

		//-----Omni-words feature;
		if(pCRDC->OmniWords_Flag){
			//m_CSegmter.Omni_words_feature_Extracting(loc_Case.relatin_mention.extent.charseq.c_str(), pCRDC->m_WordsSet, WordsCnt_map, "", "");

			m_CSegmter.Omni_words_feature_Extracting(Ref_E1.extent.charseq.c_str(), pCRDC->m_WordsSet, WordsCnt_map, "F_", "");
			m_CSegmter.Omni_words_feature_Extracting(Ref_E2.extent.charseq.c_str(), pCRDC->m_WordsSet, WordsCnt_map, "S_", "");
			m_CSegmter.Omni_words_feature_Extracting(loc_Context.L_str.c_str(), pCRDC->m_WordsSet, WordsCnt_map, "L_", "");
			m_CSegmter.Omni_words_feature_Extracting(loc_Context.M_str.c_str(), pCRDC->m_WordsSet, WordsCnt_map, "M_", "");
			m_CSegmter.Omni_words_feature_Extracting(loc_Context.R_str.c_str(), pCRDC->m_WordsSet, WordsCnt_map, "R_", "");
		}
		if(pCRDC->Segmentation_Flag){
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(Ref_E1.extent.charseq.c_str(), WordsCnt_map, "FS_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(Ref_E2.extent.charseq.c_str(), WordsCnt_map, "SS_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(loc_Context.L_str.c_str(), WordsCnt_map, "LS_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(loc_Context.M_str.c_str(), WordsCnt_map, "MS_", "");
			m_CSegmter.ICTCLAS_Segmentation_words_feature_Extracting(loc_Context.R_str.c_str(), WordsCnt_map, "RS_", "");
		}

		if(WordsCnt_map.empty()){
			AppCall::Secretary_Message_Box("Empty feature case in CRDC::Generate_ACE_Training_Cases()", MB_OK);
		}

		pair<string, vector<pair<string, float>>>* pCRDC_Case_v = new pair<string, vector<pair<string, float>>>;
		pCRDC_Case_v->first = pCRDC->TYPE_Flag?vite->TYPE:vite->SUBTYPE;
		for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
			pCRDC_Case_v->second.push_back(make_pair(mite->first, mite->second));
		}
		pmTraining_v.push_back(pCRDC_Case_v);
	}
}

void CGREval::Generate_Che_Evaluation_Port(vector<Relation_Case>& Relation_Case_v, FeatureVctor& pmTraining_v)
{
	for(vector<Relation_Case>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
		Relation_Case& loc_Case = *vite;
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
		pair<string, vector<pair<string, float>>>* pCRDC_Case_v = new pair<string, vector<pair<string, float>>>;
		pCRDC_Case_v->first = pCRDC->TYPE_Flag?vite->TYPE:vite->SUBTYPE;
		for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
			pCRDC_Case_v->second.push_back(make_pair(mite->first, mite->second));
		}
		pmTraining_v.push_back(pCRDC_Case_v);
	}
}

void CGREval::Generate_Zhang_Evaluation_Port(vector<Relation_Case>& Relation_Case_v, FeatureVctor& pmTraining_v)
{
	map<string, map<size_t, map<size_t, ACE_entity_mention>>*> loc_EntityMention_mm;
	ACE_Entity_Mention_in_Sentence(loc_EntityMention_mm);

	for(vector<Relation_Case>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
		Relation_Case& loc_Case = *vite;
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
		pair<string, vector<pair<string, float>>>* pCRDC_Case_v = new pair<string, vector<pair<string, float>>>;
		pCRDC_Case_v->first = pCRDC->TYPE_Flag?vite->TYPE:vite->SUBTYPE;
		for(map<string, float>::iterator mite = WordsCnt_map.begin(); mite != WordsCnt_map.end(); mite++){
			pCRDC_Case_v->second.push_back(make_pair(mite->first, mite->second));
		}
		pmTraining_v.push_back(pCRDC_Case_v);
	}

	if(!loc_EntityMention_mm.empty()){
		for(map<string, map<size_t, map<size_t, ACE_entity_mention>>*>::iterator mmite = loc_EntityMention_mm.begin(); mmite != loc_EntityMention_mm.end(); mmite++){
			delete mmite->second;
		}
		loc_EntityMention_mm.clear();
	}
}

void CGREval::ACE_Entity_Mention_in_Sentence(map<string, map<size_t, map<size_t, ACE_entity_mention>>*>& EntityMention_mm)
{
	AppCall::Secretary_Message_Box("Implementing this method, the corpus folder must be in the righ place, the same as the generating relation cases!", MB_OK);

	ACE_Corpus m_ACE_Corpus;
	SXMLer m_SXMLer;
	string corpusfolder = pCRDC->CorpusFolder;
	m_SXMLer.Xercesc_ACE_Folder_Files_Filter(pCRDC->CorpusFolder, m_ACE_Corpus);
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
		ostrsteam << "Please insure that the CorpusFolder is on the right place..." << endl;
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



