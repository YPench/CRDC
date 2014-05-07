#include "stdafx.h"
#include "..\\Include\\CGCom.h"
#include <shlwapi.h>
#include "..\\Consensus.h"
#include "ConvertUTF.h"


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

	CConsensusApp *app = (CConsensusApp *)AfxGetApp();
	app->m_STeller.STeller_Output_Port(poutstr);
}
void AppCall::Maxen_Responce_Message_with_Save(const char* poutstr)
{
	CConsensusApp *app = (CConsensusApp *)AfxGetApp();
	app->m_STeller.STeller_Output_Port_with_Save(poutstr);
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


size_t NLPOP::Get_Chinese_Sentence_Length_Counter(const char* contentchar){

	size_t cnt;
	size_t length = strlen(contentchar);
	cnt = 0;
	for(size_t i = 0; i < length; ){	
		if(contentchar[i++] < 0 ){
			i++;
		}
		cnt++;
	}
	return cnt;
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


void NLPOP::Get_Specified_Files(string pmFolderPath, vector<string>& FilesRoot_v, string DotAddSuffix = ".txt")
{
	string proxstr;
	CString csFilePath = NLPOP::string2CString(pmFolderPath);
	CFileFind foldpath;
	BOOL bFind = foldpath.FindFile(csFilePath + _T("*.*")); 
	if(!bFind)
	{
		return;
	}
	while(bFind = foldpath.FindNextFile())  
    {  
		csFilePath = foldpath.GetFilePath();
		if(foldpath.IsDots())   
			continue;
		if(foldpath.IsDirectory())
		{
			csFilePath += _T("\\");
			Get_Specified_Files(NLPOP::CString2string(csFilePath) , FilesRoot_v, DotAddSuffix);
		}
		

		proxstr = NLPOP::CString2string(csFilePath);
		proxstr = proxstr.substr(proxstr.rfind('\\') + 1, proxstr.length());
		if(proxstr.size() == 0)
			continue;
		if(proxstr.rfind('.') == -1)
		{
			FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
			continue;
		}
		if(0 == DotAddSuffix.length()){
			FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
			continue;
		}
		if(csFilePath.Right(csFilePath.GetLength() - csFilePath.ReverseFind('.')) != DotAddSuffix.c_str())
			continue;

		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
    }
	csFilePath = foldpath.GetFilePath();
	if(foldpath.IsDots())   
			return;
	if(foldpath.IsDirectory())
	{
		csFilePath += _T("\\");
		Get_Specified_Files(NLPOP::CString2string(csFilePath) , FilesRoot_v, DotAddSuffix);
	}
	proxstr = NLPOP::CString2string(csFilePath);
	proxstr = proxstr.substr(proxstr.rfind('\\') + 1, proxstr.length());
	if(proxstr.size() == 0)
		return;
	if(proxstr.rfind('.') == -1)
	{
		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
		return;
	}
	proxstr = proxstr.substr(proxstr.rfind('.'), proxstr.length());
	
	if(0 == DotAddSuffix.length()){
		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
		return;
	}

	if((strcmp(proxstr.c_str(), DotAddSuffix.c_str()) == 0 ) && (proxstr.size() != 0))
		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
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
