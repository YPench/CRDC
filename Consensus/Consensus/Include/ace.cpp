#include "stdafx.h"
#include "..\\Include\\ace.h"

string Detect_Single_NE_TYPE;//extern string Detect_Single_NE_TYPE;

bool Greedy_Matching_Method_FLag;//extern bool Greedy_Matching_Method_FLag;

bool Collect_nGross_Boundary_Info;//extern bool Collect_nGross_Boundary_Info;
bool Collect_nGross_Candidate_Info;//extern bool Collect_nGross_Candidate_Info;

Maxen_Rtn_map G_P_nCrossRtn_m;//extern Maxen_Rtn_map G_P_nCrossRtn_m;
Maxen_Rtn_map G_R_nCrossRtn_m;//extern Maxen_Rtn_map G_P_nCrossRtn_m;

void ace_op::Get_Entity_Mention_extent_Map(vector<ACE_entity_mention>& EntityMention_v, set<size_t>& START_s, set<size_t>& END_s)
{
	for(vector<ACE_entity_mention>::iterator pvite = EntityMention_v.begin(); pvite != EntityMention_v.end(); pvite++){	
		START_s.insert(pvite->extent.START);
		END_s.insert(pvite->extent.END);
	}
}

void ace_op::Get_Entity_Mention_extent_Map(vector<ACE_entity_mention>& EntityMention_v, map<size_t, map<size_t, ACE_entity_mention*>>& EntityMention_mm, set<size_t>& START_s, set<size_t>& END_s)
{
	for(vector<ACE_entity_mention>::iterator pvite = EntityMention_v.begin(); pvite != EntityMention_v.end(); pvite++){	
		
		if(EntityMention_mm.find(pvite->extent.START) == EntityMention_mm.end()){
			EntityMention_mm[pvite->extent.START];
			START_s.insert(pvite->extent.START);
		}
		if(EntityMention_mm[pvite->extent.START].find(pvite->extent.END) == EntityMention_mm[pvite->extent.START].end()){
			EntityMention_mm[pvite->extent.START][pvite->extent.END];
			EntityMention_mm[pvite->extent.START][pvite->extent.END] = &*pvite;
			END_s.insert(pvite->extent.END);
		}
		//else if(EntityMention_mm[pvite->extent.START][pvite->extent.END]->ID != pvite->ID){
		//	ostringstream ostream;
		//	ostream << EntityMention_mm[pvite->extent.START][pvite->extent.END]->extent.START << '\t' << EntityMention_mm[pvite->extent.START][pvite->extent.END]->extent.END << '\t' << EntityMention_mm[pvite->extent.START][pvite->extent.END]->extent.charseq << endl;
		//	ostream << pvite->extent.START << '\t' << pvite->extent.END << '\t' << pvite->extent.charseq << endl;
		//	AppCall::Secretary_Message_Box(ostream.str().c_str(), MB_OK);
		//}
	}
}

void ace_op::Get_Entity_Mention_head_Map(vector<ACE_entity_mention>& EntityMention_v, set<size_t>& START_s, set<size_t>& END_s)
{
	for(vector<ACE_entity_mention>::iterator pvite = EntityMention_v.begin(); pvite != EntityMention_v.end(); pvite++){
		if(Detect_Single_NE_TYPE.length() == 0){
			START_s.insert(pvite->head.START);
			END_s.insert(pvite->head.END);
		}
		else if(!strcmp(pvite->Entity_TYPE.c_str(), Detect_Single_NE_TYPE.c_str())){
			START_s.insert(pvite->head.START);
			END_s.insert(pvite->head.END);
		}
	}
}
void ace_op::Get_Entity_Mention_head_Map(vector<ACE_entity_mention>& EntityMention_v, map<size_t, map<size_t, ACE_entity_mention*>>& EntityMention_mm, set<size_t>& START_s, set<size_t>& END_s)
{
	for(vector<ACE_entity_mention>::iterator pvite = EntityMention_v.begin(); pvite != EntityMention_v.end(); pvite++){	
		if((Detect_Single_NE_TYPE.length() != 0) && strcmp(pvite->Entity_TYPE.c_str(), Detect_Single_NE_TYPE.c_str())){
			continue;
		}

		if(EntityMention_mm.find(pvite->head.START) == EntityMention_mm.end()){
			EntityMention_mm[pvite->head.START];
			START_s.insert(pvite->head.START);
		}
		if(EntityMention_mm[pvite->head.START].find(pvite->head.END) == EntityMention_mm[pvite->head.START].end()){
			EntityMention_mm[pvite->head.START][pvite->head.END];
			EntityMention_mm[pvite->head.START][pvite->head.END] = &*pvite;
			END_s.insert(pvite->head.END);
		}
	}
}



bool ace_op::Check_Relation_Cases(Relation_Case& pmCase)
{
	size_t Relative_position;

	REntityPairPosit loc_RENposit;
	Relative_position = pmCase.relatin_mention.extent.START;

	loc_RENposit.First_L = pmCase.first_entity.extent.START - Relative_position;
	loc_RENposit.First_R = pmCase.first_entity.extent.END - Relative_position;
	loc_RENposit.Second_L = pmCase.sencond_entity.extent.START - Relative_position;
	loc_RENposit.Second_R = pmCase.sencond_entity.extent.END - Relative_position;

	string entitystr = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCase.relatin_mention.extent.charseq.c_str(), loc_RENposit.First_L, loc_RENposit.First_R + 1 - loc_RENposit.First_L);
	if(strcmp(entitystr.c_str(), pmCase.first_entity.extent.charseq.c_str())){
		AppCall::Secretary_Message_Box("数据出错：ACE_EDT::Check_Relation_Cases()\n", MB_OK);
		return false;
	}
	entitystr = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCase.relatin_mention.extent.charseq.c_str(), loc_RENposit.Second_L, loc_RENposit.Second_R + 1 - loc_RENposit.Second_L);
	if(strcmp(entitystr.c_str(), pmCase.sencond_entity.extent.charseq.c_str())){
		AppCall::Secretary_Message_Box("数据出错：ACE_EDT::Check_Relation_Cases()\n", MB_OK);
		return false;
	}
	return true;
}

void ace_op::Segment_ACE_sgm_to_Sentence_Among_TEXT(map<string, ACE_sgm>& pm_sgmmap, map<string, list<pair<string, pair<size_t,size_t>>>>& pm_ACE_DocSentence_map)
{
	set<string> PBreakpoint_s;
	PBreakpoint_s.insert("。");
	PBreakpoint_s.insert("！");
	PBreakpoint_s.insert("？");
	PBreakpoint_s.insert("；");
	PBreakpoint_s.insert("，");
	//PBreakpoint_s.insert(" ");
	//PBreakpoint_s.insert("\n");
	//PBreakpoint_s.insert("\r");

	list<pair<string, pair<size_t,size_t>>>::iterator lite, formerlite;
	char sChar[3];
	sChar[2]=0;
	size_t PositCnt;
	size_t charseq_length;

	for(map<string, ACE_sgm>::iterator mite = pm_sgmmap.begin(); mite != pm_sgmmap.end(); mite++){
		if(pm_ACE_DocSentence_map.find(mite->second.DOCID) == pm_ACE_DocSentence_map.end()){
			pm_ACE_DocSentence_map[mite->second.DOCID];
		}
		int TEXTposit = mite->second.DOC.find(mite->second.TEXT.c_str());
		if(TEXTposit == string::npos){
			AppCall::Secretary_Message_Box("TEXT substring position is wrong...", MB_OK);
		}
		size_t CharCnt = 0;
		for(int i = 0; i < TEXTposit; ){
			if(mite->second.DOC.c_str()[i++] < 0){
				i++;
			}
			CharCnt++;
		}
		
		pm_ACE_DocSentence_map[mite->second.DOCID].push_back(make_pair(mite->second.TEXT, make_pair(CharCnt, CharCnt+NLPOP::Get_Chinese_Sentence_Length_Counter(mite->second.TEXT.c_str()))));
		//:text
		//string teststr = Sentop::Get_Substr_by_Chinese_Character_Cnt(mite->second.DOC.c_str(), CharCnt, NLPOP::Get_Chinese_Sentence_Length_Counter(mite->second.TEXT.c_str()));
		//mite->second.DOC = mite->second.TEXT;
		list<pair<string, pair<size_t,size_t>>>& listref = pm_ACE_DocSentence_map[mite->second.DOCID];
		lite = listref.begin();
		
		int Relative_Posit = lite->second.first;
		charseq_length = lite->second.second;
		PositCnt = lite->second.first;
		lite->second.second = lite->second.first;
		for(size_t Cnt = 0; PositCnt < charseq_length; ){
			sChar[0] = lite->first.c_str()[Cnt++];
			sChar[1] = 0;	
			if(sChar[0] < 0 ){
				sChar[1] = lite->first.c_str()[Cnt++];
			}
			PositCnt++;
			if(PBreakpoint_s.find(sChar) != PBreakpoint_s.end()){
				string str;
				formerlite = lite;
				if(listref.size() == 1){
					str = Sentop::Get_Substr_by_Chinese_Character_Cnt(lite->first.c_str(), 0, PositCnt-Relative_Posit);
					listref.insert(lite, make_pair(str, make_pair(Relative_Posit, PositCnt - 1)));
				}
				else{
					formerlite--;
					str = Sentop::Get_Substr_by_Chinese_Character_Cnt(lite->first.c_str(), formerlite->second.second + 1 - Relative_Posit, PositCnt - formerlite->second.second - 1);
					listref.insert(lite, make_pair(str, make_pair(formerlite->second.second + 1, PositCnt - 1)));
				}
			}
		}
		if(listref.size() == 1){
			continue;
		}
		else{
			formerlite = lite;
			formerlite--;
			lite->first = (Sentop::Get_Substr_by_Chinese_Character_Cnt(lite->first.c_str(), formerlite->second.second + 1 - Relative_Posit, PositCnt - formerlite->second.second - 1));
			lite->second.first = formerlite->second.second + 1;
			lite->second.second = PositCnt - 1;
		}
	}
	for(map<string, list<pair<string, pair<size_t,size_t>>>>::iterator mlite = pm_ACE_DocSentence_map.begin(); mlite != pm_ACE_DocSentence_map.end(); mlite++){
		for(list<pair<string, pair<size_t,size_t>>>::iterator lite = mlite->second.begin(); lite != mlite->second.end(); lite++){
			if(lite->first.length() < 2){
				continue;
			}
			sChar[0] = lite->first.c_str()[lite->first.length()-2];
			sChar[1] = lite->first.c_str()[lite->first.length()-1];
			if(PBreakpoint_s.find(sChar) != PBreakpoint_s.end()){
				lite->first = Sentop::Get_Substr_by_Chinese_Character_Cnt(lite->first.c_str(), 0, lite->second.second-lite->second.first);
				lite->second.second = lite->second.second-1;
			}
		}
	}
}

void ace_op::Segment_ACE_sgm_to_Sentence(map<string, ACE_sgm>& pm_sgmmap, map<string, list<pair<string, pair<size_t,size_t>>>>& pm_ACE_DocSentence_map)
{
	set<string> PBreakpoint_s;
	PBreakpoint_s.insert("。");
	PBreakpoint_s.insert("！");
	PBreakpoint_s.insert("？");
	PBreakpoint_s.insert("；");
	PBreakpoint_s.insert("，");
	//PBreakpoint_s.insert(" ");
	//PBreakpoint_s.insert("\n");
	//PBreakpoint_s.insert("\r");

	list<pair<string, pair<size_t,size_t>>>::iterator lite, formerlite;
	char sChar[3];
	sChar[2]=0;
	size_t PositCnt;
	size_t charseq_length;

	for(map<string, ACE_sgm>::iterator mite = pm_sgmmap.begin(); mite != pm_sgmmap.end(); mite++){
		if(pm_ACE_DocSentence_map.find(mite->second.DOCID) == pm_ACE_DocSentence_map.end()){
			pm_ACE_DocSentence_map[mite->second.DOCID];
		}
		pm_ACE_DocSentence_map[mite->second.DOCID].push_back(make_pair(mite->second.DOC, make_pair(0, NLPOP::Get_Chinese_Sentence_Length_Counter(mite->second.DOC.c_str()))));
		list<pair<string, pair<size_t,size_t>>>& listref = pm_ACE_DocSentence_map[mite->second.DOCID];
		lite = listref.begin();
		if((lite->second.first != 0) || (listref.size() != 1)){
			AppCall::Secretary_Message_Box("数据出错：ACE_EDT::Segment_ACE_sgm_to_Sentence()", MB_OK);
		}
		charseq_length = lite->second.second;
		PositCnt = 0;
		lite->second.second = 0;
		for(size_t Cnt = 0; PositCnt < charseq_length; ){
			sChar[0] = lite->first.c_str()[Cnt++];
			sChar[1] = 0;	
			if(sChar[0] < 0 ){
				sChar[1] = lite->first.c_str()[Cnt++];
			}
			PositCnt++;
			if(PBreakpoint_s.find(sChar) != PBreakpoint_s.end()){
				string str;
				formerlite = lite;
				if(listref.size() == 1){
					str = Sentop::Get_Substr_by_Chinese_Character_Cnt(lite->first.c_str(), 0, PositCnt);
					listref.insert(lite, make_pair(str, make_pair(0, PositCnt - 1)));
				}
				else{
					formerlite--;
					str = Sentop::Get_Substr_by_Chinese_Character_Cnt(lite->first.c_str(), formerlite->second.second + 1, PositCnt - formerlite->second.second - 1);
					listref.insert(lite, make_pair(str, make_pair(formerlite->second.second + 1, PositCnt - 1)));
				}
			}
		}
		if(listref.size() == 1){
			continue;
		}
		else{
			formerlite = lite;
			formerlite--;
			lite->first = (Sentop::Get_Substr_by_Chinese_Character_Cnt(lite->first.c_str(), formerlite->second.second + 1, PositCnt - formerlite->second.second - 1));
			lite->second.first = formerlite->second.second + 1;
			lite->second.second = PositCnt - 1;
		}
	}
}
void ace_op::Segment_ACE_English_sgm_to_Sentence(map<string, ACE_sgm>& pm_sgmmap, map<string, list<pair<string, pair<size_t,size_t>>>>& pm_ACE_DocSentence_map)
{
	set<string> PBreakpoint_s;
	PBreakpoint_s.insert(";");
	PBreakpoint_s.insert("!");
	PBreakpoint_s.insert("?");
	PBreakpoint_s.insert(".");

	list<pair<string, pair<size_t,size_t>>>::iterator lite, formerlite;
	char sChar[3];
	sChar[2]=0;
	size_t PositCnt;
	size_t charseq_length;

	for(map<string, ACE_sgm>::iterator mite = pm_sgmmap.begin(); mite != pm_sgmmap.end(); mite++){
		if(pm_ACE_DocSentence_map.find(mite->second.DOCID) == pm_ACE_DocSentence_map.end()){
			pm_ACE_DocSentence_map[mite->second.DOCID];
		}
		pm_ACE_DocSentence_map[mite->second.DOCID].push_back(make_pair(mite->second.DOC, make_pair(0, NLPOP::Get_Chinese_Sentence_Length_Counter(mite->second.DOC.c_str()))));
		list<pair<string, pair<size_t,size_t>>>& listref = pm_ACE_DocSentence_map[mite->second.DOCID];
		lite = listref.begin();
		if((lite->second.first != 0) || (listref.size() != 1)){
			AppCall::Secretary_Message_Box("数据出错：ACE_EDT::Segment_ACE_sgm_to_Sentence()", MB_OK);
		}
		charseq_length = lite->second.second;
		PositCnt = 0;
		lite->second.second = 0;
		for(size_t Cnt = 0; PositCnt < charseq_length; ){
			sChar[0] = lite->first.c_str()[Cnt++];
			sChar[1] = 0;	
			if(sChar[0] < 0 ){
				sChar[1] = lite->first.c_str()[Cnt++];
			}
			PositCnt++;
			if(PBreakpoint_s.find(sChar) != PBreakpoint_s.end()){
				string str;
				formerlite = lite;
				if(listref.size() == 1){
					str = Sentop::Get_Substr_by_Chinese_Character_Cnt(lite->first.c_str(), 0, PositCnt);
					listref.insert(lite, make_pair(str, make_pair(0, PositCnt - 1)));
				}
				else{
					formerlite--;
					str = Sentop::Get_Substr_by_Chinese_Character_Cnt(lite->first.c_str(), formerlite->second.second + 1, PositCnt - formerlite->second.second - 1);
					listref.insert(lite, make_pair(str, make_pair(formerlite->second.second + 1, PositCnt - 1)));
				}
			}
		}
		if(listref.size() == 1){
			continue;
		}
		else{
			formerlite = lite;
			formerlite--;
			lite->first = (Sentop::Get_Substr_by_Chinese_Character_Cnt(lite->first.c_str(), formerlite->second.second + 1, PositCnt - formerlite->second.second - 1));
			lite->second.first = formerlite->second.second + 1;
			lite->second.second = PositCnt - 1;
		}
	}
}

void ace_op::Extract_ACE_Relation_Mention_Info(string savefile, deque<ACE_relation>& ACE_Relation_Info_d)
{
	string RealtionType;
	string relation_charseqstr;
	map<string, set<string>> relation_mention_map;
	deque<string> Relation_mention_sentence_d;

	for(size_t i = 0; i < ACE_Relation_Info_d.size(); i++){
		RealtionType = ACE_Relation_Info_d[i].TYPE;

		for(vector<ACE_relation_mention>::iterator vite = ACE_Relation_Info_d[i].relation_mention_v.begin(); vite != ACE_Relation_Info_d[i].relation_mention_v.end(); vite++){
			relation_charseqstr = vite->extent.charseq;
			if(relation_mention_map.find(RealtionType) == relation_mention_map.end()){
				relation_mention_map[RealtionType];
			}
			Delet_0AH_and_20H_in_string(relation_charseqstr);

			relation_mention_map[RealtionType].insert(relation_charseqstr);

			Relation_mention_sentence_d.push_back(relation_charseqstr);
		}
	}

	//Write_Vector_Deque_List_To_File<deque<string>>(savefile, Relation_mention_sentence_d, '\n');

	return;
}

string ace_op::Delet_0AH_and_20H_in_string(const char* strchar)
{
	char sentencechar[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;
	size_t strlength = strlen(strchar);

	strcpy_s(sentencechar, MAX_SENTENCE, "");
	for(size_t Cnt = 0; Cnt < strlength; ){
		sChar[0] = strchar[Cnt++];
		sChar[1] = 0;	
		if(sChar[0] < 0 ){
			sChar[1] = strchar[Cnt++];
		}
		if(!strcmp(sChar, "\n")){
			continue;
		}
		else if(!strcmp(sChar, " ")){
			continue;
		}
		else{
			strcat_s(sentencechar, MAX_SENTENCE, sChar);
		}
	}
	return sentencechar;

}
void ace_op::Delet_0AH_and_20H_in_string(string &pmstr)
{
	char sentencechar[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;

	strcpy_s(sentencechar, MAX_SENTENCE, "");
	for(size_t Cnt = 0; Cnt < pmstr.length(); ){
		sChar[0] = pmstr.c_str()[Cnt++];
		sChar[1] = 0;	
		if(sChar[0] < 0 ){
			sChar[1]=pmstr.c_str()[Cnt++];
		}
		if(!strcmp(sChar, "\n")){
			continue;
		}
		else if(!strcmp(sChar, " ")){
			continue;
		}
		else{
			strcat_s(sentencechar, MAX_SENTENCE, sChar);
		}
	}
	pmstr = sentencechar;

}








void ace_op::output_training_case_for_maxent(const char* FilePath, multimap<string, vector<pair<string, unsigned long>>>& RelationWrods_map)
{
	ofstream out(FilePath);
	if(out.bad())
		return;
	out.clear();
	out.seekp(0, ios::beg);

	multimap<string, vector<pair<string, unsigned long>>>::iterator mmite = RelationWrods_map.begin();
	for(; mmite != RelationWrods_map.end(); mmite++){

		out << mmite->first << ' ';
		for(vector<pair<string, unsigned long>>::iterator vite = mmite->second.begin(); vite != mmite->second.end(); vite++){
			//out << vite->first << ';' << vite->second << ' ';
			out << vite->first << ' ';
		}
		out << endl;
	}
	out.close();
}









void ace_op::Output_Relation_Case(const char* FilePath, list<Relation_Case>& Relation_Case_l)
{
	ofstream out(FilePath);
	if(out.bad())
		return;
	out.clear();
	out.seekp(0, ios::beg);

	for(list<Relation_Case>::iterator list = Relation_Case_l.begin(); list != Relation_Case_l.end(); list++){
		out << list->SENID << '\t' << list->DOCID << '\t' << list->TYPE << '\t' << list->SUBTYPE << '\t' << '\r';
		
		ACE_relation_mention& relation_mention = list->relatin_mention;
		out << relation_mention.DOCID << '\t' << relation_mention.ID << '\t' << relation_mention.LEXICALCONDITION << '\t';
		out << relation_mention.extent.charseq << '\t' << relation_mention.extent.START << '\t' << relation_mention.extent.END << '\t' << '\r';
		
		ACE_entity_mention& first_entity = list->first_entity;
		out << first_entity.DOCID << '\t' << first_entity.Entity_TYPE << '\t' << first_entity.Entity_SUBSTYPE << '\t' << first_entity.Entity_CLASS << '\t' << first_entity.ID << '\t' << first_entity.TYPE << '\t' << first_entity.LDCTYPE << '\t';
		out << first_entity.head.charseq << '\t' << first_entity.head.START << '\t' << first_entity.head.END << '\t';
		out << first_entity.extent.charseq << '\t' << first_entity.extent.START << '\t' << first_entity.extent.END << '\t' << '\r';

		ACE_entity_mention& second_entity = list->sencond_entity;
		out << second_entity.DOCID << '\t' << second_entity.Entity_TYPE << '\t' << second_entity.Entity_SUBSTYPE << '\t' << second_entity.Entity_CLASS << '\t' << second_entity.ID << '\t' << second_entity.TYPE << '\t' << second_entity.LDCTYPE << '\t'; 
		out << second_entity.head.charseq << '\t' << second_entity.head.START << '\t' << second_entity.head.END << '\t';
		out << second_entity.extent.charseq << '\t' << second_entity.extent.START << '\t' << second_entity.extent.END << '\t' << '\r';

	}

	out.close();

}

void ace_op::Relation_Case_Structural_Position(vector<Relation_Case>& Relation_Case_v){

	size_t _1_separate_2 = 0, _2_separate_1 = 0;
	size_t _1_nestedin_2 = 0, _2_nestedin_1 = 0;
	size_t _1_overlap__2 = 0, _2_overlap__1 = 0;
	size_t left_equal = 0, right_equal = 0;
	size_t left_unequal = 0, right_unequal = 0;
	size_t nestedin_cnt = 0;
	size_t both_unequal = 0;

	ostringstream ostream;
	
	for(vector<Relation_Case>::iterator rcvite = Relation_Case_v.begin(); rcvite != Relation_Case_v.end(); rcvite++){
		if(rcvite->first_entity.extent.END <  rcvite->sencond_entity.extent.START){
			_1_separate_2++;
			continue;
		}
		if(rcvite->sencond_entity.extent.END < rcvite->first_entity.extent.START){
			_2_separate_1++;
			continue;
		}
		nestedin_cnt++;
		if((rcvite->first_entity.extent.START != rcvite->sencond_entity.extent.START) && (rcvite->first_entity.extent.END != rcvite->sencond_entity.extent.END)){
			both_unequal++;
			//ostream.str("");
			//ostream << rcvite->first_entity.extent.charseq << endl;
			//ostream << rcvite->sencond_entity.extent.charseq << endl;
			//AppCall::Secretary_Message_Box(ostream.str(), MB_OK);
		}
		if(rcvite->first_entity.extent.START == rcvite->sencond_entity.extent.START){
			left_equal++;
		}
		if(rcvite->first_entity.extent.END == rcvite->sencond_entity.extent.END){
			right_equal++;
		}
		if(rcvite->first_entity.extent.START != rcvite->sencond_entity.extent.START){
			left_unequal++;
		}
		if(rcvite->first_entity.extent.END != rcvite->sencond_entity.extent.END){
			right_unequal++;
		}
		if((rcvite->first_entity.extent.START >=  rcvite->sencond_entity.extent.START) && (rcvite->first_entity.extent.END <=  rcvite->sencond_entity.extent.END)){
			_1_nestedin_2++;
			continue;
		}
		if((rcvite->sencond_entity.extent.START >=  rcvite->first_entity.extent.START) && (rcvite->sencond_entity.extent.END <= rcvite->first_entity.extent.END)){
			_2_nestedin_1++;
			continue;
		}
		if(rcvite->first_entity.extent.END >= rcvite->sencond_entity.extent.START){
			_1_overlap__2++;
			continue;
		}
		if(rcvite->sencond_entity.extent.END >= rcvite->first_entity.extent.START){
			_2_overlap__1++;
			continue;
		}
		AppCall::Secretary_Message_Box("Error in ace_op::Relation_Case_Structural_Position()", MB_OK);
	}
	ostream.str("");
	ostream << "_1_separate_2: " << _1_separate_2 << endl;
	ostream << "_2_separate_1: " << _2_separate_1 << endl;
	ostream << "nestedin_cnt: " << nestedin_cnt << endl;
	ostream << "both_unequal: " << both_unequal << endl;
	ostream << "_1_nestedin_2: " << _1_nestedin_2 << endl;
	ostream << "_2_nestedin_1: " << _2_nestedin_1 << endl;
	ostream << "left_equal: " << left_equal << endl;
	ostream << "right_equal: " << right_equal << endl;
	ostream << "left_unequal: " << left_unequal << endl;
	ostream << "right_unequal: " << right_unequal << endl;
	ostream << "_1_overlap__2: " << _1_overlap__2 << endl;
	ostream << "_2_overlap__1: " << _2_overlap__1 << endl;

	AppCall::Secretary_Message_Box(ostream.str(), MB_OK);
}

void ace_op::Load_Relation_Case(const char* FilePath, vector<Relation_Case>& Relation_Case_v)
{

	char SentenceBuf[MAX_SENTENCE];
	ifstream in(FilePath);

	if(in.bad()){
		AppCall::Secretary_Message_Box("打开文件出错：ace_op::Load_Relation_Case()", MB_OK);
	}
	in.clear();
	in.seekg(0, ios::beg);

	while(true){
		if(in.peek() == EOF){	
			break;
		}
		Relation_Case loc_RCase;
		in.getline(SentenceBuf, MAX_SENTENCE, '\r');
		istringstream sstream(SentenceBuf);
		
		sstream >> loc_RCase.SENID;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');

		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		loc_RCase.DOCID = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		loc_RCase.TYPE = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		loc_RCase.SUBTYPE = SentenceBuf;

		//Relation mention
		sstream.str("");
		in.getline(SentenceBuf, MAX_SENTENCE, '\r');
		sstream.str(SentenceBuf);
		ACE_relation_mention& relation_mention = loc_RCase.relatin_mention;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		relation_mention.DOCID = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		relation_mention.ID = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		relation_mention.LEXICALCONDITION = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		relation_mention.extent.charseq = SentenceBuf;
		sstream >> relation_mention.extent.START;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		sstream >> relation_mention.extent.END;
	
		//first entity mention
		sstream.str("");
		in.getline(SentenceBuf, MAX_SENTENCE, '\r');
		sstream.str(SentenceBuf);
		ACE_entity_mention& first_entity = loc_RCase.first_entity;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		first_entity.DOCID = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		first_entity.Entity_TYPE = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		first_entity.Entity_SUBSTYPE = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		first_entity.Entity_CLASS = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		first_entity.ID = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		first_entity.TYPE = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		first_entity.LDCTYPE = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		first_entity.head.charseq = SentenceBuf;
		sstream >> first_entity.head.START;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		sstream >> first_entity.head.END;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		first_entity.extent.charseq = SentenceBuf;
		sstream >> first_entity.extent.START;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		sstream >> first_entity.extent.END;

		//second entity mention
		sstream.str("");
		in.getline(SentenceBuf, MAX_SENTENCE, '\r');
		sstream.str(SentenceBuf);
		ACE_entity_mention& second_entity = loc_RCase.sencond_entity;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		second_entity.DOCID = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		second_entity.Entity_TYPE = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		second_entity.Entity_SUBSTYPE = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		second_entity.Entity_CLASS = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		second_entity.ID = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		second_entity.TYPE = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		second_entity.LDCTYPE = SentenceBuf;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		second_entity.head.charseq = SentenceBuf;
		sstream >> second_entity.head.START;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		sstream >> second_entity.head.END;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		second_entity.extent.charseq = SentenceBuf;
		sstream >> second_entity.extent.START;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');
		sstream >> second_entity.extent.END;
		sstream.getline(SentenceBuf, MAX_SENTENCE, '\t');

		Relation_Case_v.push_back(loc_RCase);
	}
	in.close();
}

void ace_op::Computing_P_R_F(const char* FilePath)
{
	double SUM = 0;
	double Average = 0;
	double value = 0;
	size_t Count = 0;
	char SentenceBuf[MAX_SENTENCE];
	ifstream in(FilePath);

	if(in.bad()){
		AppCall::Secretary_Message_Box("打开文件出错：ace_op::Load_Relation_Case()", MB_OK);
	}
	in.clear();
	in.seekg(0, ios::beg);

	while(true){
		if(in.peek() == EOF){	
			break;
		}
		in >> value;
		Count++;
		SUM += value;
		in.getline(SentenceBuf, MAX_SENTENCE, '\n');
	}
	in.close();
	if(Count == 0){
		return;
	}
	Average = SUM/Count;

	ostringstream ostream;
	ostream << "Average: " << Average << endl;
	ostream << "Count  : " << Count;
	AppCall::Secretary_Message_Box(ostream.str(), MB_OK);

	
}

string Sentop::Get_Substr_by_Chinese_Character_Cnt(const char* charstr, int position, int Cnt)
{
	char sentencechar[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;
	int intercnt;
	size_t strlength = strlen(charstr);
	if(Cnt < 0){
		Cnt = strlength;
	}
	intercnt = 0;
	strcpy_s(sentencechar, MAX_SENTENCE, "");
	for(unsigned int cntNum = 0; cntNum < strlength; ){
		sChar[0] = charstr[cntNum++];
		sChar[1] = 0;	
		if(sChar[0] < 0 ){
			sChar[1]=charstr[cntNum++];
		}
		intercnt++;
		if(intercnt > (position + Cnt)){
			break;
		}
		if(position < intercnt){
			strcat_s(sentencechar, MAX_SENTENCE, sChar);
		}
	}
	return sentencechar;
}

void Sentop::REntityPairPosit_Change_on_order(REntityPairPosit& pm_machPosi)
{
	int Tempint;
	if(pm_machPosi.First_L != pm_machPosi.Second_L){
		if(pm_machPosi.First_L >  pm_machPosi.Second_L){
			Tempint = pm_machPosi.First_L; pm_machPosi.First_L = pm_machPosi.Second_L; pm_machPosi.Second_L = Tempint;
			Tempint = pm_machPosi.First_R; pm_machPosi.First_R = pm_machPosi.Second_R; pm_machPosi.Second_R = Tempint;
		}
	}
	else{
		if(	pm_machPosi.First_R >  pm_machPosi.Second_R){
			Tempint = pm_machPosi.First_L; pm_machPosi.First_L = pm_machPosi.Second_L; pm_machPosi.Second_L = Tempint;
			Tempint = pm_machPosi.First_R; pm_machPosi.First_R = pm_machPosi.Second_R; pm_machPosi.Second_R = Tempint;
		}
	}
}

void Sentop::Chinese_Count_Extract_Sentence_Three_context_Devided_by_Entity_Pair(const char* pmCharsent, REntityPairPosit& pm_machPosit, RelationContext& pm_context, int Mutual_Position)
{
	
	if(1 !=  Mutual_Position){//1:	With Overlap;
		if(pm_machPosit.First_L < pm_machPosit.Second_L){
			pm_context.L_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, 0, pm_machPosit.First_L);
			pm_context.M_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, pm_machPosit.First_R + 1, pm_machPosit.Second_L - pm_machPosit.First_R);
			pm_context.R_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, pm_machPosit.Second_R + 1, -1);
		}
		else{
			pm_context.L_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, 0, pm_machPosit.Second_L);
			pm_context.M_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, pm_machPosit.Second_R + 1, pm_machPosit.First_L - pm_machPosit.Second_R);
			pm_context.R_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, pm_machPosit.First_R + 1, -1);
		}
	}
	else{
		if(((pm_machPosit.First_L <= pm_machPosit.Second_L) && pm_machPosit.Second_L <= pm_machPosit.First_R)
			|| ((pm_machPosit.Second_L <= pm_machPosit.First_L) && (pm_machPosit.First_L <= pm_machPosit.Second_R))){
			
			if(pm_machPosit.First_L < pm_machPosit.Second_L){
				pm_context.L_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, 0, pm_machPosit.First_L);
			}
			else{
				pm_context.L_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, 0, pm_machPosit.Second_L);
			}
			pm_context.M_str = "";
			if(pm_machPosit.First_R < pm_machPosit.Second_R){
				pm_context.R_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, pm_machPosit.Second_R + 1, -1);
			}
			else{
				pm_context.R_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, pm_machPosit.First_R + 1, -1);
			}
		}
		else{
			if(pm_machPosit.First_L < pm_machPosit.Second_L){
				pm_context.L_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, 0, pm_machPosit.First_L);
				pm_context.M_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, pm_machPosit.First_R+1, pm_machPosit.Second_L - pm_machPosit.First_R - 1);
				pm_context.R_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, pm_machPosit.Second_R + 1, -1);
			}
			else{
				pm_context.L_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, 0, pm_machPosit.Second_L);
				pm_context.M_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, pm_machPosit.Second_R+1, pm_machPosit.First_L - pm_machPosit.Second_R - 1);
				pm_context.R_str = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmCharsent, pm_machPosit.First_R + 1, -1);
			}
		}
	}		
}

void Sentop::Extract_ACE_entities_Pair_Context(Relation_Case& pmCase, RelationContext& pmContext)
{
	REntityPairPosit m_REntityPairPosit;
	int Relative_position;

	Relative_position = pmCase.relatin_mention.extent.START;
	m_REntityPairPosit.First_L = pmCase.first_entity.extent.START - Relative_position;
	m_REntityPairPosit.First_R = pmCase.first_entity.extent.END - Relative_position;
	m_REntityPairPosit.Second_L = pmCase.sencond_entity.extent.START - Relative_position;
	m_REntityPairPosit.Second_R = pmCase.sencond_entity.extent.END - Relative_position;

	Sentop::Chinese_Count_Extract_Sentence_Three_context_Devided_by_Entity_Pair(pmCase.relatin_mention.extent.charseq.c_str(), m_REntityPairPosit, pmContext, 1);
}

string Sentop::Get_Substr_by_Chinese_Character_Cnt_With_RelPosit(const char* charstr, int start, int relposit, int Cnt)
{
	if(relposit < start){
		return "";
	}
	return Sentop::Get_Substr_by_Chinese_Character_Cnt(charstr, relposit-start, Cnt);
}
