#pragma once
#include "..\\Include\\CGCom.h"

#include <cstdlib>
#include <cassert>
#include <stdexcept> //for std::runtime_error
#include <memory>    //for std::bad_alloc
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "maxen//line_stream_iterator.hpp"
#include "maxen//maxentmodel.hpp"
#include "maxen//display.hpp" 


 

#include "ace.h"

using namespace std;
using namespace maxent;

typedef MaxentModel::context_type me_context_type;
typedef MaxentModel::outcome_type me_outcome_type;

//add_event helper function objects
struct AddEventToModel{
	AddEventToModel(MaxentModel& m):model(m){}
	void operator()(const me_context_type& context, const me_outcome_type& outcome) {
        model.add_event(context, outcome, 1);
    }
    private:
    MaxentModel& model;
};
struct AddHeldoutEventToModel{
    AddHeldoutEventToModel(MaxentModel& m):model(m){}
    void operator()(const me_context_type& context, const me_outcome_type& outcome) {
        model.add_heldout_event(context, outcome, 1);
    }
    private:
    MaxentModel& model;
};
struct AddEventToVector{
    typedef vector<pair<me_context_type, me_outcome_type> >  EventVector_;
    AddEventToVector(EventVector_& v):vec(v){}
    void operator()(const me_context_type& context, const me_outcome_type& outcome) {
        vec.push_back(make_pair(context, outcome));
    }
    private:
    EventVector_& vec;
};


namespace MAXEN{
	//----------------For ECOR
	void Coreference_Discriminating_with_Erasing(vector<pair<string, double>>& CoValue_v, FeatureVctor& Testing_v, MaxentModel& pmMaxen);

	void CExtra_Maxen_Testing_with_FeatureVector_Erasing(vector<pair<string, pair<string, double>>*>& pmResult_v, FeatureVctor& Testing_v, MaxentModel& pmMaxen);
	//-----------------For CEDT
	void Discrimer_Maxen_Recognization_with_Erasing(vector<DismCase*>& pmDismCase_v, FeatureVctor& Testing_v, MaxentModel& pmMaxen);
	void Discrimer_Maxen_Testing_with_Erasing(vector<DismCase*>& pmDismCase_v, FeatureVctor& Testing_v, MaxentModel& pmMaxen);
	
	void Candit_Maxen_Testing_with_Erasing(FeatureVctor& Testing_v, MaxentModel& pmMaxen, map<string, size_t>& pmPositive_Cases, vector<CanditCase*>& pmCandit_v);
	void Candit_Maxen_Recognization_with_Erasing(FeatureVctor& Testing_v, MaxentModel& pmMaxen, vector<CanditCase*>& pmCandit_v);
	
	void Boundary_Training_without_Erasing(FeatureVctor& Training_v, MaxentModel& pmMaxen, size_t ite);
	void Boundary_Detection_with_eval_Erasing(FeatureVctor& Training_v, MaxentModel& pmMaxen, map<size_t, map<size_t, double>*>& rtn_mm);
	void Boundary_Testing_with_eval_Erasing(FeatureVctor& Training_v, MaxentModel& pmMaxen, map<size_t, map<size_t, double>*>& rtn_mm, bool Save_Info_Flag);
	
	//-------------------------------------For normal
	void Maxen_Recognizing_with_eval_Erasing(MaxentModel& pmMaxen, FeatureVctor& Recongizing_v, vector<pair<string,double>>& rtnValue_v);
	void Init_Rtn_map_by_Designated_String(Maxen_Rtn_map& P_Rtn_m, Maxen_Rtn_map& R_Rtn_m, string& pmTYPE);
	void Collect_nGross_Performances(Maxen_Rtn_map& P_Rtn_m, Maxen_Rtn_map& R_Rtn_m, Maxen_Rtn_map& G_P_nCrossRtn_m, Maxen_Rtn_map& G_R_nCrossRtn_m);
	void Training_Cases_Check(FeatureVctor& Training_v);
	void Maxen_Training_with_Erasing(FeatureVctor& Training_v, MaxentModel& pmMaxen, size_t ite);
	void Display_Performance_for_MAXEN(bool Out_Positve_Flag, bool Save_Info_Flag, Maxen_Rtn_map&  Pricision_map, Maxen_Rtn_map& Recall_map, string memo);
	void Maxen_Training_With_Heldout(size_t heldout, vector<pair<me_context_type, me_outcome_type> >& Training_v);
	void cross_validation(FeatureVctor& v, size_t n, int iter, const string& method, double gaussian, bool random);
	void cross_validation_with_Weight(FeatureVctor& v, size_t n, int iter, const string& method, double gaussian, bool random, double weight, const char* c_TYPE) ;	
	
	//---For translation
	void Get_Training_Cases_Model_Parameter(FeatureVctor& v);
	void Training_v_To_training_case(uFeatureVctor& Training_v, vector<pair<me_context_type, me_outcome_type> >& training_case_v);
	void Read_CSmaxent_Training_Data(const char* FilePath, bool TYPE_Flag, FeatureVctor& Training_v);
	void Generate_Training_Matrix(const char* openpath, const char* savepath);
	void Generate_Training_Matrix_3_Gram(const char* openpath, const char* savepath, const char* classpath);
	void Push_Back_FeatureCase_v_from_Feature_v_with_Check(FeatureVctor& Training_v, vector<string>& pmfeature_v, string label);

	void Output_Extract_Feature_Context_For_Maxent(const char* FilePath, DulTYPE_Vctor& RelationWrods_v);
	void Output_Extract_Feature_Context_For_Maxent(const char* FilePath, FeatureVctor& RelationWrods_v);
};

bool get_sample(const string& line, me_context_type& context, me_outcome_type& outcome, bool binary_feature);
bool is_binary_feature(const string& file);
template <typename Func>
void load_events(const string& file, Func add_event);
void predict(const MaxentModel& m, const string& in_file, const string& out_file, bool output_prob);

