#include "stdafx.h"
#include "RCCom.h"
#include "maxen.h"
#include "CSegmter.h"


void RCCOM::Get_Relation_Case_Info(const char* infopath, list<Relation_Case>& Relation_Case_l)
{
	map<string, size_t> Docinfo_m;
	size_t Doc_size = 0;
	size_t NegetiveCnt = 0;
	size_t TotallCnt = 0;
	map<string, size_t> Relation_TYPE_info_m;
	map<string, size_t> Relation_SUBTYPE_info_m;
	map<string, size_t> Entity_TYPE_info_m;
	map<string, size_t> Entity_SUBTYPE_info_m;
	for(list<Relation_Case>::iterator lite = Relation_Case_l.begin(); lite != Relation_Case_l.end(); lite++){
		//doc id
		TotallCnt++;
		if(!strcmp(lite->TYPE.c_str(), NEGETIVE )){
			NegetiveCnt++;
		}
		if(Docinfo_m.find(lite->DOCID) == Docinfo_m.end()){
			Docinfo_m.insert(make_pair(lite->DOCID, 1));
			Doc_size++;
		}
		else{
			Docinfo_m[lite->DOCID]++;
		}
		//relation type
		if(Relation_TYPE_info_m.find(lite->TYPE) == Relation_TYPE_info_m.end()){
			Relation_TYPE_info_m.insert(make_pair(lite->TYPE, 1));
		}
		else{
			Relation_TYPE_info_m[lite->TYPE]++;
		}
		//relation subtype
		if(Relation_SUBTYPE_info_m.find(lite->SUBTYPE) == Relation_SUBTYPE_info_m.end()){
			Relation_SUBTYPE_info_m.insert(make_pair(lite->SUBTYPE, 1));
		}
		else{
			Relation_SUBTYPE_info_m[lite->SUBTYPE]++;
		}
		// first entity type
		if(Entity_TYPE_info_m.find(lite->first_entity.Entity_TYPE) == Entity_TYPE_info_m.end()){
			Entity_TYPE_info_m.insert(make_pair(lite->first_entity.Entity_TYPE, 1));
		}
		else{
			Entity_TYPE_info_m[lite->first_entity.Entity_TYPE]++;
		}
		// second entity type
		if(Entity_TYPE_info_m.find(lite->sencond_entity.Entity_TYPE) == Entity_TYPE_info_m.end()){
			Entity_TYPE_info_m.insert(make_pair(lite->sencond_entity.Entity_TYPE, 1));
		}
		else{
			Entity_TYPE_info_m[lite->sencond_entity.Entity_TYPE]++;
		}
		// first entity subtype
		if(Entity_SUBTYPE_info_m.find(lite->first_entity.Entity_SUBSTYPE) == Entity_SUBTYPE_info_m.end()){
			Entity_SUBTYPE_info_m.insert(make_pair(lite->first_entity.Entity_SUBSTYPE, 1));
		}
		else{
			Entity_SUBTYPE_info_m[lite->first_entity.Entity_SUBSTYPE]++;
		}
		// second entity subtype
		if(Entity_SUBTYPE_info_m.find(lite->sencond_entity.Entity_SUBSTYPE) == Entity_SUBTYPE_info_m.end()){
			Entity_SUBTYPE_info_m.insert(make_pair(lite->sencond_entity.Entity_SUBSTYPE, 1));
		}
		else{
			Entity_SUBTYPE_info_m[lite->sencond_entity.Entity_SUBSTYPE]++;
		}
	}
/*
	map<string, size_t> Docinfo_m;
	map<string, size_t> Relation_TYPE_info_m;
	map<string, size_t> Relation_SUBTYPE_info_m;
	map<string, size_t> Entity_TYPE_info_m;
	map<string, size_t> Entity_SUBTYPE_info_m;
*/
	vector<pair<string, map<string, size_t>>> info_v;
	info_v.push_back(make_pair("关系类型: ", Relation_TYPE_info_m));
	info_v.push_back(make_pair("关系子类: ", Relation_SUBTYPE_info_m));
	info_v.push_back(make_pair("实体类型: ", Entity_TYPE_info_m));
	info_v.push_back(make_pair("实体子类: ", Entity_SUBTYPE_info_m));
	ofstream out(infopath);
	if(out.bad())
		return;
	out.clear();
	out.seekp(0, ios::beg);
	
	out << "文档数" << ": " << Doc_size << ";" << endl;
	out << "Relation Mention Amount: " << TotallCnt <<  ";" << endl;
	out << NEGETIVE << ": " << NegetiveCnt <<  ";" << endl;
	out << POSITIVE << ": " << TotallCnt-NegetiveCnt <<  ";" << endl;

	size_t totall;
	for(vector<pair<string, map<string, size_t>>>::iterator vmite = info_v.begin(); vmite != info_v.end(); vmite++){
		out << vmite->first << endl;
		totall = 0;
		for(map<string, size_t>::iterator mite = vmite->second.begin(); mite != vmite->second.end(); mite++){
			out << mite->first << ',';
		}
		out << "总数" << endl;
		for(map<string, size_t>::iterator mite = vmite->second.begin(); mite != vmite->second.end(); mite++){
			out << mite->second << ',';
			totall += mite->second;
		}
		out << totall << endl;
	}
	out << endl;
	size_t RelationID = 0;
	//out << "---------------------------------------------------------------------------------------" << endl;
	//for(vector<Relation_Case>::iterator vite = Relation_Case_v.begin(); vite != Relation_Case_v.end(); vite++){
	//	out << RelationID++ << ": " << vite->TYPE << '\t' << vite->SUBTYPE << endl;
	//	out << Delet_0AH_and_20H_in_string(vite->relatin_mention.extent.charseq.c_str()) << endl;
	//	out << "(1) " << Delet_0AH_and_20H_in_string(vite->first_entity.extent.charseq.c_str()) << endl;
	//	out << "(2) " << Delet_0AH_and_20H_in_string(vite->sencond_entity .extent.charseq.c_str()) << endl;
	//}
	out.close();
}
void RCCOM::Generate_ICTCLAS_Lexicon(CSegmter& m_CSegmter, list<Relation_Case>& Relation_Case_l)
{
	set<string> Lexicon_set;
	for(list<Relation_Case>::iterator lite = Relation_Case_l.begin(); lite != Relation_Case_l.end(); lite++){
		string relation_mention_str = ace_op::Delet_0AH_and_20H_in_string(lite->relatin_mention.extent.charseq.c_str());
		CLAUSEPOS loc_Seg;
		m_CSegmter.ICTCLAS_Segmention_Port(relation_mention_str.c_str(), loc_Seg);

		for(vector<string>::iterator vite = loc_Seg.Clause.begin(); vite != loc_Seg.Clause.end(); vite++){
			if((*vite).find(',') != -1){
				//AppCall::Secretary_Message_Box("Lexicon entry is wrong in CRDC::Generate_ICTCLAS_Lexicon()...", MB_OK);
				continue;
			}
			Lexicon_set.insert(vite->data());
		}
	}
	string savepath = DATA_FOLDER;
	savepath += "CRDC\\ICTCLAS_Lexicon.dat";
	NLPOP::Save_Set_String_With_Comma_Divide(savepath, Lexicon_set);
}

void RCCOM::Extracting_Entity_Mention_Context_in_Relation_Mention(ACE_relation_mention& pmRMention, ACE_entity_mention& pmEMention, string& o_prix, string& o_prox)
{
	size_t Relative_position = pmRMention.extent.START;
	size_t Entity_START = pmEMention.extent.START - Relative_position;
	size_t Entity_END = pmEMention.extent.END - Relative_position;

	o_prix = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmRMention.extent.charseq.c_str(), 0, Entity_START);
	o_prox = Sentop::Get_Substr_by_Chinese_Character_Cnt(pmRMention.extent.charseq.c_str(), Entity_END + 1, -1);
}


void RCCOM::Entity_Adjacent_Words_and_POS_Feature_Extracting(CSegmter& m_CSegmter, string EntityID, string prix, string prox, map<string, float>& WordsCnt_map, size_t Range)
{
	ostringstream ostrsteam;
	CLAUSEPOS loc_Seg;
	vector<string>::reverse_iterator rWORDite;
	vector<string>::reverse_iterator rPOSite;

	//---------------Left
	m_CSegmter.ICTCLAS_Segmention_Port(prix.c_str(), loc_Seg);
	rWORDite = loc_Seg.Clause.rbegin();
	rPOSite = loc_Seg.POS.rbegin();
	if(loc_Seg.Clause.size() != loc_Seg.POS.size()){
		AppCall::Secretary_Message_Box("Unequal data in: RCCOM::Entity_Adjacent_Words_and_POS_Feature_Extracting()", MB_OK);
	}
	for(size_t posit = 1 ; (rWORDite != loc_Seg.Clause.rend()) && (posit <= Range); rWORDite++, rPOSite++, posit++){
		ostrsteam.str("");
		ostrsteam << rWORDite->data() << "_" << EntityID << "_Word_L" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));

		ostrsteam.str("");
		ostrsteam << rPOSite->data() << "_" << EntityID << "_POS_L" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
	}
	loc_Seg.Clause.clear();
	loc_Seg.POS.clear();

	m_CSegmter.ICTCLAS_Segmention_Port(prox.c_str(), loc_Seg);
	if(loc_Seg.Clause.size() != loc_Seg.POS.size()){
		AppCall::Secretary_Message_Box("Unequal data in: RCCOM::Entity_Adjacent_Words_and_POS_Feature_Extracting()", MB_OK);
	}
	for(size_t posit = 1, i = 0 ; (i < loc_Seg.Clause.size()) && (posit <= Range); i++, posit++){
		ostrsteam.str("");
		ostrsteam << loc_Seg.Clause[i] <<  "_" << EntityID << "_Word_R" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));

		ostrsteam.str("");
		ostrsteam << loc_Seg.POS[i] <<  "_" << EntityID << "_POS_R" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
	}
	loc_Seg.Clause.clear();
	loc_Seg.POS.clear();
}


void RCCOM::RelationContext_Adjacent_Words_and_POS_Feature_Extracting(CSegmter& m_CSegmter, RelationContext& pmContext, map<string, float>& WordsCnt_map, size_t Range)
{
	ostringstream ostrsteam;
	CLAUSEPOS loc_Seg;
	vector<string>::reverse_iterator rWORDite;
	vector<string>::reverse_iterator rPOSite;

	//---------------Left
	m_CSegmter.ICTCLAS_Segmention_Port(pmContext.L_str.c_str(), loc_Seg);
	rWORDite = loc_Seg.Clause.rbegin();
	rPOSite = loc_Seg.POS.rbegin();
	if(loc_Seg.Clause.size() != loc_Seg.POS.size()){
		AppCall::Secretary_Message_Box("Unequal data in: RCCOM::Entity_Adjacent_Words_and_POS_Feature_Extracting()", MB_OK);
	}
	for(size_t posit = 1 ; (rWORDite != loc_Seg.Clause.rend()) && (posit <= Range); rWORDite++, rPOSite++, posit++){
		ostrsteam.str("");
		ostrsteam << rWORDite->data() << "_E1_Word_L" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));

		ostrsteam.str("");
		ostrsteam << rPOSite->data() << "_E1_POS_L" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
	}
	loc_Seg.Clause.clear();
	loc_Seg.POS.clear();

	//---------------Middle
	m_CSegmter.ICTCLAS_Segmention_Port(pmContext.L_str.c_str(), loc_Seg);
	if(loc_Seg.Clause.size() != loc_Seg.POS.size()){
		AppCall::Secretary_Message_Box("Unequal data in: RCCOM::Entity_Adjacent_Words_and_POS_Feature_Extracting()", MB_OK);
	}
	for(size_t posit = 1, i = 0 ; (i < loc_Seg.Clause.size()) && (posit <= Range); i++, posit++){
		ostrsteam.str("");
		ostrsteam << loc_Seg.Clause[i] << "_E1_Word_R" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));

		ostrsteam.str("");
		ostrsteam << loc_Seg.POS[i] << "_E1_POS_R" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
	}
	loc_Seg.Clause.clear();
	loc_Seg.POS.clear();
	
	rWORDite = loc_Seg.Clause.rbegin();
	rPOSite = loc_Seg.POS.rbegin();
	for(size_t posit = 1 ; (rWORDite != loc_Seg.Clause.rend()) && (posit <= Range); rWORDite++, rPOSite++, posit++){
		ostrsteam.str("");
		ostrsteam << rWORDite->data() << "_E2_Word_L" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));

		ostrsteam.str("");
		ostrsteam << rPOSite->data() << "_E2_POS_L" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
	}
	loc_Seg.Clause.clear();
	loc_Seg.POS.clear();
	
	//---------------Right
	m_CSegmter.ICTCLAS_Segmention_Port(pmContext.R_str.c_str(), loc_Seg);
	if(loc_Seg.Clause.size() != loc_Seg.POS.size()){
		AppCall::Secretary_Message_Box("Unequal data in: RCCOM::Entity_Adjacent_Words_and_POS_Feature_Extracting()", MB_OK);
	}
	for(size_t posit = 1, i = 0 ; (i < loc_Seg.Clause.size()) && (posit <= Range); i++, posit++){
		ostrsteam.str("");
		ostrsteam << loc_Seg.Clause[i] << "_E2_Word_R" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));

		ostrsteam.str("");
		ostrsteam << loc_Seg.POS[i] << "_E2_POS_R" << posit;
		WordsCnt_map.insert(make_pair(ostrsteam.str(), (float)1));
	}
	loc_Seg.Clause.clear();
	loc_Seg.POS.clear();
}

void RCCOM::Delet_0AH_and_20H_in_RelationContext(RelationContext& pm_context)
{
	ace_op::Delet_0AH_and_20H_in_string(pm_context.L_str);
	ace_op::Delet_0AH_and_20H_in_string(pm_context.M_str);
	ace_op::Delet_0AH_and_20H_in_string(pm_context.R_str);
}
void RCCOM::Delet_0AH_and_20H_in_Relation_Case(Relation_Case& pmRelCase)
{
	ace_op::Delet_0AH_and_20H_in_string(pmRelCase.first_entity.extent.charseq);
	ace_op::Delet_0AH_and_20H_in_string(pmRelCase.sencond_entity.extent.charseq);
	ace_op::Delet_0AH_and_20H_in_string(pmRelCase.relatin_mention.extent.charseq);
			
	ace_op::Delet_0AH_and_20H_in_string(pmRelCase.first_entity.head.charseq);
	ace_op::Delet_0AH_and_20H_in_string(pmRelCase.sencond_entity.head.charseq);
}

void RCCOM::Output_Extract_ACE_Relation_Words_Context_For_Maxent(const char* FilePath, FeatureVctor& RelationWrods_v)
{
	ofstream out(FilePath);
	if(out.bad())
		return;
	out.clear();
	out.seekp(0, ios::beg);

	FeatureVctor::iterator mmite = RelationWrods_v.begin();
	for(; mmite != RelationWrods_v.end(); mmite++){
		for(vector<pair<string, float>>::iterator  vite = (*mmite)->second.begin(); vite != (*mmite)->second.end(); vite++){
			out << vite->first << ';' << vite->second << ';';
		}
		out << endl << (*mmite)->first << endl;
	}
	out.close();
}

void RCCOM::Wrods_Length_Detect(const char* lexiconpath, map<size_t, size_t>& LengthCnt_map)
{
	set<string> Loc_WordsSet;
	if(!NLPOP::Exist_of_This_File(lexiconpath)){
		AppCall::Secretary_Message_Box("Lexicon of CVMC is not existing...", MB_OK);
	}
	NLPOP::LoadWordsStringSet(lexiconpath, Loc_WordsSet);

	size_t length;
	for(set<string>::iterator site = Loc_WordsSet.begin(); site != Loc_WordsSet.end(); site++){
		length = NLPOP::Get_Chinese_Sentence_Length_Counter(site->data());
		if(LengthCnt_map.find(length) == LengthCnt_map.end()){
			LengthCnt_map.insert(make_pair(length, 0));
		}
		LengthCnt_map[length]++;
	}
}

string RCCOM::Get_Six_Types_of_Entity_Positional_Structures(Relation_Case& pm_RCase){

	//ostringstream ostream;
	if(pm_RCase.first_entity.extent.END <  pm_RCase.sencond_entity.extent.START){
		//_1_separate_2++;
		return "N1_separate_N2";
	}
	if(pm_RCase.sencond_entity.extent.END < pm_RCase.first_entity.extent.START){
		return "N2_separate_N1";
	}
	if(pm_RCase.first_entity.extent.START == pm_RCase.sencond_entity.extent.START){
		//left_equal++;
		return "N1_N2_left_equal";
	}
	if(pm_RCase.first_entity.extent.END == pm_RCase.sencond_entity.extent.END){
		//right_equal++;
		return "N1_N2_right_equal";
	}
	if((pm_RCase.first_entity.extent.START >=  pm_RCase.sencond_entity.extent.START) && (pm_RCase.first_entity.extent.END <=  pm_RCase.sencond_entity.extent.END)){
		//_N1_nestedin_N2;
		return "N1_nestedin_N2";
	}
	if((pm_RCase.sencond_entity.extent.START >=  pm_RCase.first_entity.extent.START) && (pm_RCase.sencond_entity.extent.END <= pm_RCase.first_entity.extent.END)){
		//_2_nestedin_1++;
		return "N2_nestedin_N1";
	}
	AppCall::Secretary_Message_Box("Error in ace_op::Relation_Case_Structural_Position()", MB_OK);
	return "Other_Position";
}

string RCCOM::Get_Entity_Positional_Structures(Relation_Case& pm_RCase)
{
	//if((pm_RCase.relatin_mention.extent.START == pm_RCase.first_entity.extent.START) && (pm_RCase.relatin_mention.extent.END == pm_RCase.first_entity.extent.END)){
		//AppCall::Secretary_Message_Box("数据出错：1_Adjacent_2 is hit", MB_OK);
	//}
	//Nested, Adjacent and Separated
	if((pm_RCase.first_entity.extent.START >= pm_RCase.sencond_entity.extent.START) && (pm_RCase.first_entity.extent.END <= pm_RCase.sencond_entity.extent.END)){
		return "1_Nested_in_2";
	}
	if((pm_RCase.sencond_entity.extent.START >= pm_RCase.first_entity.extent.START) && (pm_RCase.sencond_entity.extent.END <= pm_RCase.first_entity.extent.END)){
		return "2_Nested_in_1";
	}
	if((pm_RCase.sencond_entity.extent.START <= pm_RCase.first_entity.extent.END) && (pm_RCase.sencond_entity.extent.END > pm_RCase.first_entity.extent.END)){
		AppCall::Secretary_Message_Box("数据出错：1_Adjacent_2 is hit", MB_OK);
		return "1_Adjacent_2";
	}
	if((pm_RCase.first_entity.extent.START <= pm_RCase.sencond_entity.extent.END) && (pm_RCase.first_entity.extent.END > pm_RCase.sencond_entity.extent.END)){
		AppCall::Secretary_Message_Box("数据出错：2_Adjacent_1 is hit", MB_OK);
		return "2_Adjacent_1";
	}
	if(pm_RCase.sencond_entity.extent.START > pm_RCase.first_entity.extent.END){
		return "1_Separated_2";
	}
	if(pm_RCase.first_entity.extent.START > pm_RCase.sencond_entity.extent.END){
		return "2_Separated_1";
	}
	AppCall::Secretary_Message_Box("数据出错：ace_op::Get_Entity_Positional_Structures()", MB_OK);
	return "";
}

string RCCOM::Get_Four_Types_of_Entity_Positional_Structures(Relation_Case& pm_RCase)
{
	//Nested, Adjacent and Separated
	if((pm_RCase.first_entity.extent.START >= pm_RCase.sencond_entity.extent.START) && (pm_RCase.first_entity.extent.END <= pm_RCase.sencond_entity.extent.END)){
		return "Order_3";
	}
	if((pm_RCase.sencond_entity.extent.START >= pm_RCase.first_entity.extent.START) && (pm_RCase.sencond_entity.extent.END <= pm_RCase.first_entity.extent.END)){
		return "Order_2";
	}
	if(pm_RCase.first_entity.extent.START > pm_RCase.sencond_entity.extent.START){
		return "Order_0";
	}
	if(pm_RCase.sencond_entity.extent.START > pm_RCase.first_entity.extent.START){
		return "Order_1";
	}
	AppCall::Secretary_Message_Box("数据出错：ace_op::Get_Entity_Positional_Structures()", MB_OK);
	return "";
}

void RCCOM::Output_For_LIBSVM(const char* inputpath, const char* outputfolder)
{
	ostringstream ostream;
	uFeatureVctor Training_v;
	uFeatureVctor::iterator  iter_v;
	//MAXEN::Read_CSmaxent_Training_Data(inputpath, Training_v);

	map<string, long> Feature_Position_map;
	long FeatureID = 1;
	for(iter_v = Training_v.begin(); iter_v != Training_v.end(); iter_v++){
		for(vector<pair<string, float>>::iterator vite = iter_v->second.begin(); vite != iter_v->second.end();vite++){
			if(Feature_Position_map.find(vite->first) == Feature_Position_map.end()){
				Feature_Position_map.insert(make_pair(vite->first, FeatureID++));
			}
		}
	}
	map<string, vector<map<size_t, size_t>>> Output_map;
	for(iter_v = Training_v.begin(); iter_v != Training_v.end(); iter_v++){
		if(Output_map.find(iter_v->first) == Output_map.end()){
			Output_map[iter_v->first];
		}
		map<size_t, size_t> loc_Case_map;
		for(vector<pair<string, float>>::iterator vite = iter_v->second.begin(); vite != iter_v->second.end();vite++){
			loc_Case_map.insert(make_pair(Feature_Position_map[vite->first], 1));
		}
		Output_map[iter_v->first].push_back(loc_Case_map);
	}
	Training_v.clear();
	double SVM_class;
	for(map<string, vector<map<size_t, size_t>>>::iterator mvite = Output_map.begin(); mvite != Output_map.end();mvite++){
		vector<pair<double, map<size_t, size_t>*>> prob_v;
		for(map<string, vector<map<size_t, size_t>>>::iterator omvite = Output_map.begin(); omvite != Output_map.end();omvite++){
			for(vector<map<size_t, size_t>>::iterator vite = omvite->second.begin(); vite != omvite->second.end(); vite++){
				if(omvite == mvite){
					SVM_class = 1;
				}
				else{
					SVM_class = -1;
				}
				prob_v.push_back(make_pair(SVM_class, &(*vite)));
			}
		}
		random_shuffle(prob_v.begin(), prob_v.end());
		ostream.str("");
		ostream << outputfolder << mvite->first;
		ofstream out(ostream.str().c_str());
		if(out.bad()){
			return;
		}
		out.clear();
		out.seekp(0, ios::beg);
		for(vector<pair<double, map<size_t, size_t>*>>::iterator vite = prob_v.begin(); vite != prob_v.end(); vite++){
			out << vite->first << ' ';
			for(map<size_t, size_t>::iterator mite = vite->second->begin(); mite != vite->second->end(); mite++){
				out << mite->first << ':' << mite->second << ' ';
			}
			out << endl;
		}
		out.close();
	}
	return;
}


void RCCOM::Adjacent_Words_POS_Feature_Extracting(CSegmter& m_CSegmter, const char* sentchar, map<string, float>& WordsCnt_map, string prix = "", string prox = "")
{
	CLAUSEPOS loc_Seg;
	m_CSegmter.ICTCLAS_Segmention_Port(sentchar, loc_Seg);
	if(prix.length() > 0){
		if(!loc_Seg.POS.empty()){
			WordsCnt_map.insert(make_pair(prix + loc_Seg.POS[0], (float)1));
		}
	}
	if(prox.length() > 0){
		if(!loc_Seg.POS.empty()){
			WordsCnt_map.insert(make_pair(loc_Seg.POS[loc_Seg.POS.size() - 1] + prox, (float)1));
		}
	}
}
void RCCOM::Local_Adjacent_Words_Singleton_POS_Feature_Extracting(CSegmter& m_CSegmter, const char* sentchar, map<string, float>& WordsCnt_map, string prix = "", string prox = "")
{
	CLAUSEPOS loc_Seg;
	m_CSegmter.ICTCLAS_Segmention_Port(sentchar, loc_Seg);
	if(prix.length() > 0){
		if(!loc_Seg.POS.empty()){
			WordsCnt_map.insert(make_pair(loc_Seg.POS[0], (float)1));
		}
	}
	if(prox.length() > 0){
		if(!loc_Seg.POS.empty()){
			WordsCnt_map.insert(make_pair(loc_Seg.POS[loc_Seg.POS.size() - 1], (float)1));
		}
	}
}
//</Generate Training Cases>
void RCCOM::Get_Entity_Mention_extent_Map(map<string, ACE_entity>& ACE_Entity_Info_map, map<string, ACE_entity_mention>& EntityMention_map)
{
	for(map<string, ACE_entity>::iterator mite = ACE_Entity_Info_map.begin(); mite != ACE_Entity_Info_map.end(); mite++){
		for(vector<ACE_entity_mention>::iterator vite = mite->second.entity_mention_v.begin(); vite != mite->second.entity_mention_v.end(); vite++){
			if(EntityMention_map.find(vite->ID) == EntityMention_map.end()){
				EntityMention_map.insert(make_pair(vite->ID, *vite));
			}
		}
	}
}