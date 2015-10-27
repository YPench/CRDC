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
#include "afxmt.h"
#include "..\\Include\\CGCom.h"

#define STANFORD_PARSER_FOLDER "F:\\YPench\\ConsensusGraph\\EXE\\Stanford_Parser\\"
#define JAVA_SPACE "F:\\YPench\\JavaSpace\\" //Standard Parser Data

typedef struct _RelationContext{
	string L_str;
	string M_str;
	string R_str;
}RelationContext, *pRelationContext;

typedef struct _REntityPairPosit{
	size_t First_L;
	size_t First_R;
	size_t Second_L;
	size_t Second_R;
}REntityPairPosit, *pREntityPairPosit;

//-----------------------------------------------------------------------
typedef struct _ACE_entity_head{
	size_t START;
	size_t END;
	string charseq;
} ACE_entity_head, *pACE_entity_head;

typedef struct _ACE_extent{
	size_t START;
	size_t END;
	string charseq;
} ACE_extent, *pACE_extent;
typedef struct _ACE_name{
	size_t START;
	size_t END;
	string charseq;
} ACE_name, *pACE_name;
typedef struct _ACE_relation_mention_arg{
	string REFID;
	string ROLE;
	ACE_extent extent;
}ACE_relation_mention_arg, *pACE_relation_mention_arg;

typedef struct _ACE_entity_attributes{
	vector<ACE_name> name_v;
}ACE_entity_attributes, *pACE_entity_attributes;

typedef struct _ACE_relation_mention{
	string DOCID;
	string ID;
	string LEXICALCONDITION;
	ACE_extent extent;
	multimap<string, ACE_relation_mention_arg> arg_mention_mmap;
} ACE_relation_mention, *pACE_relation_mention;

typedef struct _ACE_relation_argument{
	string REFID;
	string ROLE;
}ACE_relation_argument, *pACE_relation_argument;

typedef struct _ACE_relation{
	string DOCID;
	string ID;
	string TYPE;
	string SUBTYPE;
	string TENSE;
	string MODALITY;
	vector<ACE_relation_argument> relation_argument_v;
	vector<ACE_relation_mention> relation_mention_v;
}ACE_relation, *pACE_relation;

typedef struct _event_mention_argument{
string REFID;
string ROLE;
ACE_extent extent;
}event_mention_argument, *pevent_mention_argument;

typedef struct _event_mention{
string ID;
ACE_extent extent;
ACE_extent ldc_scope;
ACE_extent anchor;
vector<event_mention_argument> event_mention_argument_v;

}event_mention, *pevent_mention;

typedef struct _ACE_Event{
string DOCID;
string ID;
string TYPE;
string SUBTYPE;
string MODALITY;
string POLARITY;
string GENERICITY;
string TENSE;
vector<pair<string, string>> event_argument_v;
vector<event_mention> event_mention_v;
}ACE_Event, *pACE_Event;

typedef struct _ACE_entity_mention{
	//-------------------------additional info;
	string DOCID;
	size_t CASID;
	size_t SENID;
	string Entity_ID;
	string Entity_TYPE;
	string Entity_SUBSTYPE;
	string Entity_CLASS;
	//--------------
	string ID;
	string TYPE;
	string LDCTYPE;
	string LDCATR;
	ACE_extent extent;
	ACE_entity_head head;
} ACE_entity_mention, *pACE_entity_mention;

typedef struct _ACE_entity{
	string DOCID;	
	string ID;
	string TYPE;
	string SUBTYPE;
	string CLASS;
	vector<ACE_entity_mention> entity_mention_v;
	ACE_entity_attributes entity_attributes;
}ACE_entity, *pACE_entity;

typedef struct _ACE_sgm{
	string DOCID;
	string DOCTYPE;
	string DATETIME;
	string DOC;
	string TEXT;
}ACE_sgm, *pACE_sgm;

typedef struct _ACE_DocInfo{
	string DOCID;
	string URI;
	string SOURCE;
	string TYPE;
	string AUTHOR;
	string ENCODING;
}ACE_DocInfo, *pACE_DocInfo;

typedef struct _ACE_Corpus{
	vector<ACE_relation> ACE_Relation_Info_v;
	map<string, ACE_entity> ACE_Entity_Info_map;
	map<string, ACE_sgm> ACE_sgm_mmap;
	map<string, ACE_DocInfo> ACE_DocInfo_m;
	map<string, ACE_Event> ACE_EventInfo_m;
}ACE_Corpus, *pACE_Corpus;



typedef struct _Relation_Entities{
	string relation_mention;
	string arg_1;
	string arg_2;
}Relation_Entities,*pRelation_Entities;

typedef struct _Relation_Case{
	size_t SENID;
	string DOCID;
	string TYPE;
	string SUBTYPE;
	ACE_relation_mention relatin_mention;
	ACE_entity_mention first_entity;
	ACE_entity_mention sencond_entity;
	double TYPE_V;
	bool Faulted_Flag;
}Relation_Case, *pRelation_Case;

typedef struct _CGigaDOC{
	char* p_DOCID;
	char* p_TYPE;
	char* p_Headline;
	char* p_Dataline;
	char* p_TEXT;
}CGigaDOC, *pCGigaDOC;

typedef struct _NE_Surround{
	string DOCID;
	size_t SENID;
	string insur;
	string prixsur;
	string proxsur;
	size_t START;
	size_t END;
	vector<ACE_entity_mention> entity_mention_v;
	//vector<ACE_extent> Negetive_NE_v;
}NE_Surround, *pNE_Surround;

typedef struct _BoundCase{
	size_t CASID;
	size_t position;
	size_t SENID;
	string prixstr;
	string proxstr;
	bool Bound_Falg;
	ACE_entity_mention* pEntity_Mention;
}BoundCase, *pBoundCase;

typedef struct _CanditCase{
	size_t SENID;
	size_t CASID;
	bool CEDT_Head_Flag;
	bool Detection_Flag;
	bool Cand_Flag;
	string org_TYPE;
	string predit_TYPE;
	string prix;
	string prox;
	string mention;
	int START;
	int END;
	double value;
	string l_outmstr;
	string r_outmstr;
	ACE_entity_mention* pNE_mention;
}CanditCase, *pCanditCase;

typedef struct _DismCase{
	size_t SENID;
	size_t CASID;
	bool Head_Held_Flag;
	bool Extend_Held_Flag;
	bool Dism_Flag;
	bool Matched_Extend_Flag;

	string org_TYPE;
	string predit_TYPE;
	double TYPE_value;
	pair<string, pair<string, double>> Entity_CLASS;
	pair<string, pair<string, double>> Entity_TYPE;
	pair<string, pair<string, double>> Entity_SUBTYPE;
	pair<string, pair<string, double>> TYPE;
	pair<string, pair<string, double>> ROLE;

	pCanditCase pExtend;
	pCanditCase pHead;
	ACE_entity_mention* pNE_mention;
}DismCase, *pDismCase;

typedef struct entity_mention{
	string DOCID;
	size_t CASID;
	size_t SENID;
	string TYPE;
	string SUBTYPE;
	double TYPE_V;
	double SUBTYPE_V;
	ACE_extent extent;
	ACE_entity_head head;
} entity_mention, *p_entity_mention;

typedef vector<pair<pair<string,string>, vector<pair<string, float>>>*> DulTYPE_Vctor;
typedef map<string, pair<size_t, size_t>> Maxen_Rtn_map;
typedef vector<pair<string, vector<pair<string, float>>>*> FeatureVctor;
typedef vector<pair<string, vector<pair<string, float>>>> uFeatureVctor;

namespace ace_op{
	void Computing_P_R_F(const char* FilePath);
	void Relation_Case_Structural_Position(vector<Relation_Case>& Relation_Case_v);
	
	
	bool Check_Relation_Cases(Relation_Case& pmCase);
	void Segment_ACE_sgm_to_Sentence(map<string, ACE_sgm>& pm_sgmmap, map<string, list<pair<string, pair<size_t,size_t>>>>& pm_ACE_DocSentence_map);
	void Extract_ACE_Relation_Mention_Info(string savefile, vector<ACE_relation>& ACE_Relation_Info_v);

	string Delet_0AH_and_20H_in_string(RelationContext& pmContext);
	string Delet_0AH_and_20H_in_string(const char* strchar);
	void Delet_0AH_and_20H_in_string(string &pmstr);
	void Delet_0AH_and_20H_in_string(const char* inputchar, char* outputchar, size_t BUF_SIZE);
	void Delet_0AH_in_string(string &pmstr);

	void Segment_ACE_English_sgm_to_Sentence(map<string, ACE_sgm>& pm_sgmmap, map<string, list<pair<string, pair<size_t,size_t>>>>& pm_ACE_DocSentence_map);
	void Segment_ACE_sgm_to_Sentence_Among_TEXT(map<string, ACE_sgm>& pm_sgmmap, map<string, list<pair<string, pair<size_t,size_t>>>>& pm_ACE_DocSentence_map);
	
	void Get_Entity_Mention_extent_Map(vector<ACE_entity_mention*>& EntityMentionp_v, map<size_t, map<size_t, ACE_entity_mention*>>& EntityMention_mm);

	void Get_Entity_Mention_extent_Map(vector<ACE_entity_mention>& EntityMention_v, map<size_t, map<size_t, ACE_entity_mention*>>& EntityMention_mm, set<size_t>& START_Info, set<size_t>& END_s);
	void Get_Entity_Mention_extent_Map(vector<ACE_entity_mention>& EntityMention_v, set<size_t>& START_s, set<size_t>& END_s);
	void Get_Entity_Mention_head_Map(vector<ACE_entity_mention>& EntityMention_v, set<size_t>& START_s, set<size_t>& END_s);
	void Get_Entity_Mention_head_Map(vector<ACE_entity_mention>& EntityMention_v, map<size_t, map<size_t, ACE_entity_mention*>>& EntityMention_mm, set<size_t>& START_s, set<size_t>& END_s);

	void output_training_case_for_maxent(const char* FilePath, multimap<string, vector<pair<string, unsigned long>>>& RelationWrods_map);
	void Output_Relation_Case(const char* FilePath, list<Relation_Case>& Relation_Case_l);
	void Load_Relation_Case(const char* FilePath, vector<Relation_Case*>& Relation_Case_v);

};

namespace Sentop{

	string Get_Substr_by_Chinese_Character_Cnt(const char* charstr, int position, int Cnt);
	string Get_Substr_by_Chinese_Character_Cnt_With_RelPosit(const char* charstr, int start, int relposit, int Cnt);
	void REntityPairPosit_Change_on_order(REntityPairPosit& pm_machPosi);
	void Chinese_Count_Extract_Sentence_Three_context_Devided_by_Entity_Pair(const char* pmCharsent, REntityPairPosit& pm_machPosit, RelationContext& pm_context, int Mutual_Position);
	void Extract_ACE_entities_Pair_Context(Relation_Case& pmCase, RelationContext& pmContext);
};