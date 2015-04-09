#include "stdafx.h"
#include "..\\Include\\CGCom.h"
#include <shlwapi.h>
#include "..\\Consensus.h"
#include "ConvertUTF.h"


string Detect_Single_NE_TYPE;
bool Collect_nGross_Boundary_Info;
bool Collect_nGross_Candidate_Info;
map<string, pair<size_t, size_t>> G_P_nCrossRtn_m;
map<string, pair<size_t, size_t>> G_R_nCrossRtn_m;

extern void STeller_Responce_Message(const char* poutstr);
extern void STeller_Responce_Message_with_Save(const char* poutstr);

void AppCall::Secretary_Hide()
{
}
void AppCall::Secretary_Show()
{
}

void AppCall::Response_Diplay(string sout, DWORD MessageType = 0)
{
	AppCall::Secretary_Message_Box(sout, MessageType);
}

void AppCall::Maxen_Responce_Message(const char* poutstr)
{
	STeller_Responce_Message(poutstr);
}
void AppCall::Maxen_Responce_Message_with_Save(const char* poutstr)
{
	STeller_Responce_Message_with_Save(poutstr);
}

int AppCall::Secretary_Message_Box(const char* cmsg)
{
	return AppCall::Secretary_Message_Box(cmsg, MB_OK);
}

int AppCall::Secretary_Message_Box(string msgstr, UINT nType = MB_OK)
{
	CConsensusApp *app = (CConsensusApp *)AfxGetApp();
	int rtn = AfxMessageBox(NLPOP::string2CString(msgstr), nType);
	return rtn;
}

void AppCall::Consensus_Responce_Message(string msgstr, UINT nType)
{
//	static string outpusmsg = "";
//	CConsensusApp *app = (CConsensusApp *)AfxGetApp();
//	app->pdlg->Output_MSG(msgstr, nType);
	//outpusmsg = msgstr;
//	DWORD msgphreadId =app->pdlg->msgphreadId;
	//PostThreadMessage(app->pdlg->msgphreadId, WM_MESSAGE, (WPARAM)nType, (LPARAM)outpusmsg.c_str());
}

void AppCall::Load_Document_Events_Info(const char* loadpath, map<string, size_t>& DocID2Event_m)
{
	char getlineBuf[MAX_DOC_BUFFER];
	size_t doc_cnt;
	size_t EventID;

	ifstream in(loadpath);
	if(in.bad())
		return;
	in.clear();
	in.seekg(0, ios::beg);
	if(in.peek() == EOF){
		in.close();
		return;
	}
	in >> doc_cnt;
	in.getline(getlineBuf, MAX_DOC_BUFFER,'\n');
	for(size_t i = 0; i < doc_cnt; i++){
		in.getline(getlineBuf, MAX_DOC_BUFFER,'\t');
		in >> EventID;
		DocID2Event_m.insert(make_pair(getlineBuf, EventID));
		in.getline(getlineBuf, MAX_DOC_BUFFER,'\n');
	}

	in.close();
	return;
}
void AppCall::Load_Document_Events_Info(const char* loadpath, map<string, size_t>& DocID2Event_m, vector<vector<string>*>& EventTopTerm_v)
{
	char getlineBuf[MAX_DOC_BUFFER];
	size_t doc_cnt;
	size_t EventID;

	ifstream in(loadpath);
	if(in.bad())
		return;
	in.clear();
	in.seekg(0, ios::beg);
	if(in.peek() == EOF){
		in.close();
		return;
	}
	in >> doc_cnt;
	in.getline(getlineBuf, MAX_DOC_BUFFER,'\n');
	for(size_t i = 0; i < doc_cnt; i++){
		in.getline(getlineBuf, MAX_DOC_BUFFER,'\t');
		in >> EventID;
		DocID2Event_m.insert(make_pair(getlineBuf, EventID));
		in.getline(getlineBuf, MAX_DOC_BUFFER,'\n');
	}
	in >> doc_cnt;
	in.getline(getlineBuf, MAX_DOC_BUFFER,'\n');
	while(in.peek() != EOF){
		in.getline(getlineBuf, MAX_DOC_BUFFER,'\n');
		istringstream instream(getlineBuf);
		vector<string>* pTerm_v = new vector<string>;
		EventTopTerm_v.push_back(pTerm_v);
		while(instream.peek() != EOF){
			instream.getline(getlineBuf, MAX_DOC_BUFFER,';');
			pTerm_v->push_back(getlineBuf);
		}
	}
	in.close();
}
string AppCall::Subsection_Responce_Message_Memo(const char* memochar)
{
	ostringstream ostream;
	
	ostream << "\n\n\n\
****************************************************************************\n\
****************************************************************************\n\
***********                                                                                               ***********\n\
***********                                   ";
ostream << memochar;
ostream << "                                ***********\n\
***********                                                                                               ***********\n\
****************************************************************************\n\
****************************************************************************\n\n";

	return ostream.str();
}

void AppCall::Console_Running_with_Output_Redirection(const char* cImplementFolder, const char* parameters,  const char* outputFile)
{
	if(strlen(parameters) == 0){
		AppCall::Secretary_Message_Box("File or Parameters not exist in: CCRF::CRF_Training_Port()", MB_OK);
		return;
	}

	wchar_t awParameters[256];
	wchar_t awOutputFileName[256];

	SCONVERT::AnsiToUnicode(outputFile, awOutputFileName);
	SCONVERT::AnsiToUnicode(parameters, awParameters);

	TCHAR cmdline[1024];
	_tcscpy_s(cmdline, 1024, awParameters);

	DeleteFile(awOutputFileName);

	SECURITY_ATTRIBUTES sa={sizeof ( sa ),NULL,TRUE};
    SECURITY_ATTRIBUTES *psa=NULL; 
    DWORD dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;  
    OSVERSIONINFO osVersion={0};  
    osVersion.dwOSVersionInfoSize =sizeof ( osVersion );  
    if(GetVersionEx(&osVersion)){  
        if(osVersion.dwPlatformId ==VER_PLATFORM_WIN32_NT)  {  
            psa=&sa;
            dwShareMode|=FILE_SHARE_DELETE;  
        }  
    }
	HANDLE hConsoleRedirect = CreateFile(  
								awOutputFileName,  
								GENERIC_WRITE,  
								dwShareMode,  
								psa,  
								OPEN_ALWAYS,  
								FILE_ATTRIBUTE_NORMAL,  
								NULL );  
	ASSERT(hConsoleRedirect!=INVALID_HANDLE_VALUE );


    STARTUPINFO si={sizeof(si)};  
    si.dwFlags =STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;//使用标准柄和显示窗口  
    si.hStdOutput =hConsoleRedirect;//将文件作为标准输出句柄  
    si.wShowWindow =SW_HIDE;//隐藏控制台窗口  
    PROCESS_INFORMATION pi={0};
	memset(&pi, 0, sizeof(pi));

//	if(CreateProcess(NULL, cmdline,NULL,NULL,TRUE,NULL,NULL, _T(cImplementFolder),&si,&pi))  { 
	if(CreateProcess(NULL, cmdline,NULL,NULL,TRUE,NULL,NULL,NLPOP::string2CString(cImplementFolder),&si,&pi))  {
        WaitForSingleObject(pi.hProcess, INFINITE);  
        CloseHandle(pi.hProcess);  
        CloseHandle(pi.hThread);
    }  
    CloseHandle(hConsoleRedirect);
}

BOOL AppCall::Consensus_Open_Process(string beoperpath, string exepath)
{
	STARTUPINFO si;
	PROCESS_INFORMATION processInformation;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	memset(&processInformation, 0, sizeof(processInformation));

	CString SFile_path = NLPOP::string2CString(beoperpath);
	SFile_path = _T("WORDPAD ") + SFile_path;

	TCHAR tempExeRoot[MAX_PATH * 2];
	TCHAR tempCmdLine[MAX_PATH * 2];
	_tcscpy_s(tempExeRoot, MAX_PATH*2, NLPOP::string2CString(exepath));
	_tcscpy_s(tempCmdLine, MAX_PATH*2, SFile_path);

	return CreateProcess(tempExeRoot, tempCmdLine, NULL, NULL, TRUE, ABOVE_NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &processInformation);
}

void NLPOP::LoadNotesMapMerge(string notespath, map<string, string>& locmap)
{
	ifstream in(notespath.c_str());
	if(in.bad())
		return;
	in.clear();
	in.seekg(0, ios::beg);
	if(in.peek() == EOF)
		return;
	
	string vocable;
	string notes;
	char buffer[MAX_SENTENCE];

	while(true)
	{
		in.getline(buffer, MAX_SENTENCE, ',');
		vocable= buffer;
		in.getline(buffer, MAX_SENTENCE, '#');
		notes= buffer;

		if(in.peek() == EOF)
		 break;
		locmap.insert(make_pair(vocable, notes));	
	}
	in.close();
	return;
}

void NLPOP::Load_String_to_String_Mapping(const char* cFilePath, map<string, string>& pmString2String_m)
{
	char FirstBuf[MAX_SENTENCE];
	char SecondBuf[MAX_SENTENCE];
	ifstream in(cFilePath);
	if(in.bad()){
		in.close();
		return ;
	}
	in.clear();
	in.seekg(0, ios::beg);
	while(in.peek() != EOF){
		in.getline(FirstBuf, MAX_SENTENCE,'\t');
		in.getline(SecondBuf, MAX_SENTENCE,'\n');
		pmString2String_m.insert(make_pair(FirstBuf, SecondBuf));
	} 
	in.close();
}

void NLPOP::Save_String_to_String_Mapping(const char* cFilePath, map<string, string>& pmString2String_m)
{
	FILE * fout;
	if(fopen_s(&fout, cFilePath, "w")){
		AppCall::Secretary_Message_Box("Error to Open File in: NLPOP::Save_String_to_String_Mapping()");
	}
	for(map<string, string>::iterator mite = pmString2String_m.begin(); mite != pmString2String_m.end(); mite++){
		fprintf(fout, mite->first.c_str());
		fprintf(fout, "\t");
		fprintf(fout, mite->second.c_str());
		fprintf(fout, "\n");
	}
	fclose(fout);

}



void NLPOP::charseq_to_vector(const char* pcharseq, vector<string>& pvector)
{
	char sChar[3];
	sChar[2]=0;
	size_t seqlength = strlen(pcharseq);

	for(size_t i = 0; i < seqlength; ){
		sChar[0] = pcharseq[i++];
		sChar[1] = 0;
		if(sChar[0] < 0 ){
			sChar[1] = pcharseq[i++];
		}
		pvector.push_back(sChar);
	}
}
void NLPOP::Converting_strng_set_into_2Gram_set(set<string>& pmSet)
{
	set<string> locSet_s;
	for(set<string>::iterator site = pmSet.begin(); site != pmSet.end(); site++){
		locSet_s.insert(site->data());
	}
	pmSet.clear();
	for(set<string>::iterator site = locSet_s.begin(); site != locSet_s.end(); site++){
		vector<string> char_v;
		NLPOP::charseq_to_vector(site->data(), char_v);
		//if(!char_v.empty()){
		//	pmSet.insert(char_v[0]);
		//}
		for(size_t i = 1; i < char_v.size(); i++){
			pmSet.insert(char_v[i-1]+char_v[i]);
		}
	}
}

bool NLPOP::prix_substring_matching(const char* allchar, const char* subchar)
{
	size_t sublenght = strlen(subchar);
	if(strlen(allchar) < sublenght){
		return false;
	}
	for(size_t i = 0; i < sublenght; i++){
		if(allchar[i] != subchar[i]){
			return false;
		}
	}
	return true;
}
bool NLPOP::prox_substring_matching(const char* allchar, const char* subchar)
{



	return false;
}


size_t NLPOP::Get_Chinese_Sentence_Length_Counter(const char* contentchar){

	size_t cnt = 0;
	size_t length = strlen(contentchar);
	for(size_t i = 0; i < length; ){	
		if(contentchar[i++] < 0 ){
			i++;
		}
		cnt++;
	}
	return cnt;
}
void NLPOP::Segmenting_TEXT_into_Sentences_By_Character(const char* cSent, set<string>& PBreakpoint_s, vector<string>& pmSent_v)
{
	char SentenceBuf[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;
	size_t SentLength = strlen(cSent);

	strcpy_s(SentenceBuf, MAX_SENTENCE, "");
	for(size_t i = 0; i < SentLength; ){
		sChar[0] = cSent[i++];
		sChar[1] = 0;	
		if(sChar[0] < 0 ){
			sChar[1] = cSent[i++];
		}
		strcat_s(SentenceBuf, MAX_SENTENCE, sChar);
		if(PBreakpoint_s.find(sChar) != PBreakpoint_s.end()){
			pmSent_v.push_back(SentenceBuf);
			strcpy_s(SentenceBuf, MAX_SENTENCE, "");
		}
	}
}
void NLPOP::merging_numbers(string& stringref, set<string>& Number_Set)
{
	stringref = NLPOP::merging_numbers(stringref.c_str(), Number_Set);
}

string NLPOP::merging_numbers(const char* pcharseq, set<string>& Number_Set)
{
	char sChar[3];
	sChar[2]=0;
	size_t seqlength = strlen(pcharseq);
	string rtnstr = "";
	bool Pre_Number_Flag = false;

	for(size_t i = 0; i < seqlength; ){
		sChar[0] = pcharseq[i++];
		sChar[1] = 0;
		if(sChar[0] < 0 ){
			sChar[1] = pcharseq[i++];
		}
		if(Number_Set.find(sChar) != Number_Set.end()){
			if(Pre_Number_Flag){
				//
			}
			else{
				rtnstr += "1";
				Pre_Number_Flag = true;
			}
		}
		else if(!strcmp(sChar, ".") || !strcmp(sChar, ":") || !strcmp(sChar, "/")){
			if(Pre_Number_Flag){
				//
			}
			else{
				rtnstr += sChar;
			}
		}
		else{
			rtnstr += sChar;
			Pre_Number_Flag = false;
		}
	}
	return rtnstr;
}
void NLPOP::merging_English(string& stringref, set<string>& English_Set)
{
	stringref = NLPOP::merging_English(stringref.c_str(), English_Set);
}
string NLPOP::merging_English(const char* pcharseq, set<string>& English_Set)
{
	char sChar[3];
	sChar[2]=0;
	size_t seqlength = strlen(pcharseq);
	string rtnstr = "";
	bool Pre_English_Flag = false;

	for(size_t i = 0; i < seqlength; ){
		sChar[0] = pcharseq[i++];
		sChar[1] = 0;
		if(sChar[0] < 0 ){
			sChar[1] = pcharseq[i++];
		}
		if(English_Set.find(sChar) != English_Set.end()){
			if(Pre_English_Flag){
				//
			}
			else{
				rtnstr += "E";
				Pre_English_Flag = true;
			}
		}
		else if(!strcmp(sChar, ".") || !strcmp(sChar, " ")){
			if(Pre_English_Flag){
				//
			}
			else{
				rtnstr += sChar;
			}
		}
		else{
			rtnstr += sChar;
			Pre_English_Flag = false;
		}
	}
	return rtnstr;

}


bool NLPOP::Find_Substr_In_Chinese_Sent(const char* TargetBuf, const char* CandidateBuf)
{
	unsigned int Tlength = strlen(TargetBuf);
	unsigned int Clength = strlen(CandidateBuf);
	if(0 == Clength)
		return true;
	if(Tlength < Clength)
		return false;
	unsigned int Dlength = Tlength - Clength;
	unsigned int Tposit = 0;
	unsigned int Cposit = 0;
	while(Tposit <= Dlength)
	{
		for(Cposit =0;  Cposit< Clength; )
		{
			if(TargetBuf[Tposit + Cposit] != CandidateBuf[Cposit])
			{
				break;
			}
			if(CandidateBuf[Cposit++] < 0)
			{
				if(TargetBuf[Tposit + Cposit] != CandidateBuf[Cposit++])
				{
					break;
				}
			}
			if(Cposit == Clength)
			{
				return true;
			}
		}
		if(TargetBuf[Tposit++] < 0)
		{
			Tposit++;
		}
	}

	return false;
}


void  NLPOP::Save_Set_String_With_Comma_Divide(string pmSaveName, set<string>& pmSaveSet)
{
	//CSecretaryApp *app = (CSecretaryApp *)AfxGetApp();
	ostringstream ostrsteam;
	string sOutStr;

	if(!NLPOP::Exist_of_This_File(pmSaveName))
	{
		ostrsteam << "提示：新建文件：" << pmSaveName << endl;
		ostrsteam << "In Save_Set_String_With_Comma_Divide()";
		sOutStr = ostrsteam.str();
		AppCall::Secretary_Message_Box(sOutStr, MB_OK);
	}
	
	ofstream out(pmSaveName.c_str());
	if(out.bad())
	{
		ostrsteam.str("");
		ostrsteam << "保存文件出错：" << pmSaveName << endl;
		ostrsteam << "函数：NLPOP::Save_Set_String_With_Comma_Divide(string pmSaveName, set<string>& pmSaveSet)";
		sOutStr = ostrsteam.str();
		AppCall::Secretary_Message_Box(sOutStr, MB_OK);
		return;
	}
	out.clear();
	out.seekp(0, ios::beg);
	
	for(set<string>::iterator site = pmSaveSet.begin(); site != pmSaveSet.end(); site++)
	{
		out << *site << ',';
	}

	out.close();



}
void  NLPOP::Output_Processing_Message(bool Total_Flag, size_t TotalCnt, size_t CurCnt)
{
	ostringstream ostrsteam;
	ostrsteam.precision(2);
	if(Total_Flag){
		ostrsteam << endl;
		ostrsteam << "===================Processing Message=====================" << endl;
		ostrsteam << "Total Processing Cases: " << TotalCnt << endl;
	}
	if(TotalCnt == 0){
		return;
	}
	ostrsteam << "Processed: " << 1.0*CurCnt/TotalCnt << "; " << "Remained: " << TotalCnt-CurCnt << endl;
	
}

void NLPOP::LoadWordsStringSet(string pathstr, set<string>& strset)
{
	ifstream in(pathstr.c_str());
	if(in.bad())
	{
		in.close();
		ostringstream ostrsteam;
		string sOutStr;
		ostrsteam.str("");
		ostrsteam << "打开文件出错：" << pathstr << endl;
		ostrsteam << "函数：NLPOP::LoadWordsStringSet(string pathstr, set<string>& strset)";
		sOutStr = ostrsteam.str();
		AppCall::Secretary_Message_Box(sOutStr, MB_OK);
		return ;
	}
	char getlineBuf[MAX_SENTENCE];
	in.clear();
	in.seekg(0, ios::beg);
	if(in.peek() == EOF)
	{
		in.close();
		return;
	}
	while(true)
	{
		if(in.peek() == EOF)
		{
			break;
		}
		in.getline(getlineBuf, MAX_SENTENCE,',');
		if(strlen(getlineBuf) == 0)
			continue;
		strset.insert(getlineBuf);
	} 
	in.clear();
	in.close();
}


void NLPOP::LoadWordData_AS_Map_string_MorphstrSet(string pmWDpath, map<string, set<string>>& pmWordMor_ms)
{
	ifstream in(pmWDpath.c_str());
	if(in.bad())
		return;
	in.clear();
	in.seekg(0, ios::beg);

	char getlineBuf[MAX_SENTENCE];
	string wordstr;
	string morphstr;

	if(in.peek() == EOF)
	{
		in.close();
		return;
	}
	while(!in.eof())
	{
		in.getline(getlineBuf, MAX_SENTENCE,'\t');
		in.getline(getlineBuf, MAX_SENTENCE,'\t');
		wordstr = getlineBuf;
		in.getline(getlineBuf, MAX_SENTENCE,' ');
		in.getline(getlineBuf, MAX_SENTENCE,' ');
		in.getline(getlineBuf, MAX_SENTENCE,'\n');
		morphstr = getlineBuf;

		if(wordstr == "")
			continue;
		if(pmWordMor_ms.find(wordstr) == pmWordMor_ms.end())
		{
			pmWordMor_ms[wordstr];
			pmWordMor_ms[wordstr].insert(morphstr);
		}
		else
		{
			pmWordMor_ms[wordstr].insert(morphstr);
		}
	} //while end;
	in.close();
	return;
}

void NLPOP::Get_Child_Folders(string pmFolderPath, vector<string>& FilesRoot_v)
{
	CString csFilePath = NLPOP::string2CString(pmFolderPath);
	if(!NLPOP::FolderExist(csFilePath)){
		return;
	}
	CFileFind foldpath;
	BOOL bFind = foldpath.FindFile(csFilePath + _T("*.*")); 
	if(!bFind){
		return;
	}
	while(bFind = foldpath.FindNextFile())   {  
		csFilePath = foldpath.GetFilePath();
		if(foldpath.IsDots())   
			continue;
		if(foldpath.IsDirectory()){
			csFilePath += _T("\\");
			FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
		}	
    }
	csFilePath = foldpath.GetFilePath();
	if(foldpath.IsDots()){
		return;
	}
	if(foldpath.IsDirectory()){
		csFilePath += _T("\\");
		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
	}
}
void NLPOP::Get_Specified_Files(string pmFolderPath, vector<string>& FilesRoot_v, string DotAddSuffix = ".txt")
{
	string proxstr;
	CString csFilePath = NLPOP::string2CString(pmFolderPath);
	CFileFind foldpath;
	BOOL bFind = foldpath.FindFile(csFilePath + _T("*.*")); 
	if(!bFind){
		return;
	}
	while(bFind = foldpath.FindNextFile())   {  
		csFilePath = foldpath.GetFilePath();
		if(foldpath.IsDots())   
			continue;
		if(foldpath.IsDirectory()){
			csFilePath += _T("\\");
			Get_Specified_Files(NLPOP::CString2string(csFilePath) , FilesRoot_v, DotAddSuffix);
		}
		proxstr = NLPOP::CString2string(csFilePath);
		proxstr = proxstr.substr(proxstr.rfind('\\') + 1, proxstr.length());
		if(proxstr.size() == 0)
			continue;
		if(DotAddSuffix.length()  == 0){
			FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
			continue;
		}
		if(string::npos == proxstr.rfind('.')){
			continue;
		}
		if(!strcmp(DotAddSuffix.c_str(), proxstr.substr(proxstr.rfind('.'), proxstr.length()-proxstr.rfind('.')).c_str())){
			FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
		}	
    }
	csFilePath = foldpath.GetFilePath();
	if(foldpath.IsDots()){
		return;
	}
	if(foldpath.IsDirectory()){
		csFilePath += _T("\\");
		Get_Specified_Files(NLPOP::CString2string(csFilePath) , FilesRoot_v, DotAddSuffix);
	}
	proxstr = NLPOP::CString2string(csFilePath);
	proxstr = proxstr.substr(proxstr.rfind('\\') + 1, proxstr.length());
	if(proxstr.size() == 0)
		return;
	if(DotAddSuffix.length()  == 0){
		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
		return;
	}
	if(string::npos == proxstr.rfind('.')){
		return;
	}
	if(!strcmp(DotAddSuffix.c_str(), proxstr.substr(proxstr.rfind('.'), proxstr.length()-proxstr.rfind('.')).c_str())){
		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
	}	
}
string NLPOP::CString2string(CString str)
{

//  CStringW   str(L"test");   
  CStringA   stra(str.GetBuffer(0));   
  str.ReleaseBuffer();       
  std::string   strs   (stra.GetBuffer(0));   
  stra.ReleaseBuffer();
  return strs;
}

CString NLPOP::string2CString(string str)
{
	CString cstr(str.c_str());
	return cstr;//Format("%s", str.c_str());
}

bool NLPOP::Exist_of_This_File(string filename)
{
	CString FPFile = NLPOP::string2CString(filename);
	CFileFind finder;
	BOOL bFind = finder.FindFile(FPFile);
	finder.Close();
	if(bFind)
		return true;
	return false;
}



BOOL NLPOP::FolderExist(CString strPath)
{
	if((strPath.GetLength() >= 2) && (strPath.ReverseFind('\\') == (strPath.GetLength() - 1)))
	{
		strPath = strPath.Left(strPath.GetLength() - 1);
	}
     WIN32_FIND_DATA   wfd;
     BOOL rValue = FALSE;
     HANDLE hFind = FindFirstFile(strPath, &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
              rValue = TRUE;   
     }
     FindClose(hFind);
    return rValue;
}
BOOL NLPOP::FileExist(CString strFileName)
{
      CFileFind fFind;
     return fFind.FindFile(strFileName); 
}
void NLPOP::Checking_Dir_if_none_then_Greate(string filename)
{
	if(NLPOP::FolderExist(NLPOP::string2CString(filename))){
		return;
	}
	_mkdir(filename.c_str());
}

void NLPOP::Delete_Dir_and_Grate(string filename)
{
	if(NLPOP::FolderExist(NLPOP::string2CString(filename))){
		NLPOP::DeleteDir(NLPOP::string2CString(filename));
	}
	_mkdir(filename.c_str());
}

BOOL NLPOP::CreateFolder(CString strPath)
{
     SECURITY_ATTRIBUTES attrib;
     attrib.bInheritHandle = FALSE;
     attrib.lpSecurityDescriptor = NULL;
     attrib.nLength =sizeof(SECURITY_ATTRIBUTES);
    //上面定义的属性可以省略。 直接return ::CreateDirectory( path, NULL); 即可
     return ::CreateDirectory( strPath, &attrib);
} 

void NLPOP::DeleteDir(CString strDelDir)
{
    CFileFind ff;
    CString strDir, strFile;
    strDir = strDelDir;

	if(strDir.Right(1) != "\\")
		strDir += "\\";
    strDir += "*.*";
    BOOL bFind = ff.FindFile(strDir);
    while ( bFind )
    {
        bFind = ff.FindNextFile();
        if ( ff.IsDots() )
            continue;
        CString strFileName = ff.GetFileName();

        strFile = strDelDir;
        if ( strFile.Right(1) != "\\" )
			strFile += "\\";
		strFile += strFileName;
        if ( ff.IsDirectory() )
            DeleteDir(strFile);
        else
        {
            if ( ff.IsReadOnly() )
                SetFileAttributes(strFile, GetFileAttributes(strFile) & (~FILE_ATTRIBUTE_READONLY) );
            DeleteFile(strFile);
        }
    }
    ff.Close();
    RemoveDirectory(strDelDir);
 
} 
void NLPOP::Delete_Files_In_Dir(CString strDelDir)
{

    CFileFind ff;
    CString strDir, strFile;
    strDir = strDelDir;

	if(strDir.Right(1) != "\\")
		strDir += "\\";
    strDir += "*.*";
    BOOL bFind = ff.FindFile(strDir);
    while ( bFind )
    {
        bFind = ff.FindNextFile();
        if ( ff.IsDots() )
            continue;
        CString strFileName = ff.GetFileName();

        strFile = strDelDir;
        if ( strFile.Right(1) != "\\" )
			strFile += "\\";
		strFile += strFileName;
        if ( ff.IsDirectory() )
            DeleteDir(strFile);
        else
        {
            if ( ff.IsReadOnly() )
                SetFileAttributes(strFile, GetFileAttributes(strFile) & (~FILE_ATTRIBUTE_READONLY) );
            DeleteFile(strFile);
        }
    }
    ff.Close();

}
