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

#include <string>
#include <ostream>
#include <iterator>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>

using namespace std;

#define	 MAX_PRODUCT_SEMAPHORE 128
#define	 RESPONCE_WAIT_TIME 100
#define  MAX_SENTENCE		65535
#define  EVERY_FILE_NUMBER 10000
#define  RESERVED_NUM 300000
#define	 MAX_CLAUSE_LENGTH 128
#define MAX_DOC_BUFFER 300000

#define WM_ENET_MESSAGE WM_USER+100

#define max_event_NUM 25

#define POSITIVE "Positive"
#define NEGETIVE "Negetive"

#define PATH_STOPTIMETYPE_SET		".\\..\\..\\Data\\STopTimeType_Set.dat"
#define PATH_WORDDB					".\\..\\..\\Worddb.dat"

#define DATA_FOLDER					".\\..\\..\\Data\\"
#define ENGLISH_CORPUS_FOLDER				".\\..\\..\\Corpus\\ACE_E\\"
#define CHINESE_CORPUS_FOLDER				".\\..\\..\\Corpus\\ACE_T\\"
#define EVENT_ORGANIZATION_FOLDER		    ".\\..\\..\\Data\\Events Organizing\\"
#define Gigawrod_CORPUS_FOLDER				".\\..\\..\\Corpus\\_pla_cmn\\"
#define PICTURE_PATH		".\\..\\..\\Data\\PICTURE\\"

#define CEDT_LOG_PATH				".\\..\\..\\Data\\CEDT\\CEDT_Log.txt"
#define CEDT_BOUNDARY_RETURN		".\\..\\..\\Data\\CEDT\\BoundaryRet_v.txt"
#define CDOC_FOLDER					".\\..\\..\\Data\\CDOC\\"
#define ENET_FOLDER					".\\..\\..\\Data\\ENET\\"

#define MAXENT_FOLDER				".\\..\\..\\EXE\\RFM\\Release\\RFM.exe"
#define Notepad_Path				".\\..\\..\\EXE\\notepad\\notepad++.exe"
#define Pajet_Path					".\\..\\..\\EXE\\pajek\\Pajek.exe"
#define ACE_Perl_Eval_Path			".\\..\\..\\EXE\\Perl_Eval\\ace08-eval-v17.pl"
#define ACE_Eval_FOLDER				".\\..\\..\\EXE\\Perl_Eval\\"

typedef struct _CLAUSEPOS
{
	vector<string> Clause;
	vector<string> POS;
}CLAUSEPOS, Sentencepos, *pCLAUSEPOS;



namespace NLPOP{
	bool prix_substring_matching(const char* allchar, const char* subchar);
	bool prox_substring_matching(const char* allchar, const char* subchar);

	size_t  Get_Chinese_Sentence_Length_Counter(const char* contentchar);
	string CString2string(CString str);
	CString string2CString(string str);
	void charseq_to_vector(const char* pcharseq, vector<string>& pvector);
	void merging_numbers(string& stringref, set<string>& Number_Set);
	string merging_numbers(const char* pcharseq, set<string>& Number_Set);

	void merging_English(string& stringref, set<string>& English_Set);
	string merging_English(const char* pcharseq, set<string>& English_Set);


	void Output_Processing_Message(bool Total_Flag, size_t TotalCnt, size_t CurCnt);
	void LoadWordsStringSet(string pathstr, set<string>& strset);
	void Get_Specified_Files(string pmFolderPath, vector<string>& FilesRoot_v, string DotAddSuffix);
	void LoadWordData_AS_Map_string_MorphstrSet(string pmWDpath, map<string, set<string>>& pmWordMor_ms);

	bool Exist_of_This_File(string filename);
	BOOL FolderExist(CString strPath);
	BOOL FileExist(CString strFileName);
	BOOL CreateFolder(CString strPath);
	void Delete_Dir_and_Grate(string filename);
	void DeleteDir(CString strDelDir);
	void Delete_Files_In_Dir(CString strDelDir);
	void Checking_Dir_if_none_then_Greate(string filename);
	void Get_Child_Folders(string pmFolderPath, vector<string>& FilesRoot_v);


	bool Find_Substr_In_Chinese_Sent(const char* TargetBuf, const char* CandidateBuf);
	void Save_Set_String_With_Comma_Divide(string pmSaveName, set<string>& pmSaveSet);
	void Converting_strng_set_into_2Gram_set(set<string>& pmSet);
	void Segmenting_TEXT_into_Sentences_By_Character(const char* cSent, set<string>& PBreakpoint_s, vector<string>& pmSent_v);

	void Load_String_to_String_Mapping(const char* cFilePath, map<string, string>& pmString2String_m);
	void Save_String_to_String_Mapping(const char* cFilePath, map<string, string>& pmString2String_m);
	void LoadNotesMapMerge(string notespath, map<string, string>& locmap);
}


namespace AppCall{

	void Maxen_Responce_Message_with_Save(const char* poutstr);
	int Secretary_Message_Box(string msgstr, UINT nType);
	int Secretary_Message_Box(const char* cmsg);
	void Consensus_Responce_Message(string msgstr, UINT nType);
	BOOL Consensus_Open_Process(string beoperpath, string exepath);
	string Subsection_Responce_Message_Memo(const char* memochar);

	void Maxen_Responce_Message(const char* poutstr);
	void Response_Diplay(std::string sout, DWORD MessageType);

	void Secretary_Hide();
	void Secretary_Show();

	void Console_Running_with_Output_Redirection(const char* cImplementFolder, const char* parameters, const char* outputFile);

	void Load_Document_Events_Info(const char* loadpath, map<string, size_t>& DocID2Event_m, vector<vector<string>*>& EventTopTerm_v);
	void Load_Document_Events_Info(const char* loadpath, map<string, size_t>& DocID2Event_m);
};

template<typename TCLASS> bool Read_Line_InFile_To_Vector_Deque_List(string ResultFolder, TCLASS& pm_T_Class, unsigned int ThreadID, char Gchar, bool IgnoreEnterCharFlag)
{
	char getlineBuf[MAX_SENTENCE];
	ifstream in(ResultFolder.c_str());
	if(in.bad())
		return false;
	in.clear();
	in.seekg(0, ios::beg);
	if(in.peek() == EOF){
		in.close();
		return false;
	}
	while(true){
		if(in.peek() == EOF){	
			break;
		}
		in.getline(getlineBuf, MAX_SENTENCE, Gchar);
		pm_T_Class.push_back(getlineBuf);
		if(IgnoreEnterCharFlag){
			in.getline(getlineBuf, MAX_SENTENCE, '\n');
		}
	} 
	in.close();
	return true;
}
template<typename TCLASS> bool Write_Vector_Deque_List_To_File(string pmSaveFileName, TCLASS& pm_T_Class, unsigned int ThreadID, char Gchar, string pmmemo)
{
	ofstream out(pmSaveFileName.c_str());
	if(out.bad())
		return 0;
	out.clear();
	out.seekp(0, ios::beg);
	//--------------------------save result file;
	//out << 0xEF << 0xBB << 0xBF;
	for(TCLASS::iterator dite = pm_T_Class.begin(); dite != pm_T_Class.end();  dite++){
		out << *dite << Gchar;
	}
	out.close();
	return true;
}



