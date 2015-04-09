// STeller_Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "stdafx.h"
#include "Consensus.h"
#include "STeller_Dlg.h"
#include <sstream>


CRichEditCtrl* pRich;

IMPLEMENT_DYNAMIC(STeller_Dlg, CDialog)

DWORD	CurrentID = GetCurrentThreadId();

STeller_Dlg::STeller_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(STeller_Dlg::IDD, pParent)
{
	pRich = &m_richSeller;
	NLPOP::LoadWordsStringSet(PATH_STOPTIMETYPE_SET, STopTimeType_SET);
	Diaplay_Thread_Running = false;
	Pause_FLag = false;
	strcpy_s(m_DisplayBuf, MAX_DISPLAY_BUF, "");
}

STeller_Dlg::~STeller_Dlg()
{
}

void STeller_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STELLER_RICHEDIT2, m_richSeller);
	DDX_Control(pDX, IDC_STELLER_PAUSE, m_PauseButton);
}
BEGIN_MESSAGE_MAP(STeller_Dlg, CDialog)

ON_MESSAGE(WM_STELLER_DISPLAY_DIALOG, STeller_Initialization)

ON_BN_CLICKED(IDC_BUTTON_SAVE, &STeller_Dlg::OnBnClickedButtonSave)
ON_BN_CLICKED(IDC_BUTTON_CLEAR, &STeller_Dlg::OnBnClickedButtonClear)
ON_BN_CLICKED(IDC_BUTTON_RETURN, &STeller_Dlg::OnBnClickedButtonReturn)
ON_BN_CLICKED(IDC_BUTTON_SAVE_AS, &STeller_Dlg::OnBnClickedButtonSaveAs)
ON_BN_CLICKED(IDC_BUTTON_OPEN_HISTORY, &STeller_Dlg::OnBnClickedButtonOpenHistory)
ON_BN_CLICKED(IDC_STELLER_PAUSE, &STeller_Dlg::OnBnClickedStellerPause)
END_MESSAGE_MAP()

/*
void STeller_Dlg::OnCancel()
{
	if(Diaplay_Thread_Running){
		Diaplay_Thread_Running = false;
		GetExitCodeThread(hThread, &ThreadId);
		TerminateThread(hThread, ThreadId);
		Display_Content_v.clear();
	}
	ShowWindow(SW_HIDE);
	return;
}*/

BOOL  STeller_Dlg::PreTranslateMessage(MSG* pMsg)  
{  
/*	if(WM_KEYDOWN == pMsg->message ){   //keyboard buttondown
  		UINT nKey = (int) pMsg->wParam;   
		if( VK_RETURN == nKey ){
			OnCancel();
			return TRUE ; 
		}
 		if(32 == nKey){
			if(!Display_Content_v.empty()){
				Diaplay_Thread_Running = false;
				GetExitCodeThread(hThread, &ThreadId);
  				TerminateThread(hThread, ThreadId);
				STeller_Initialization((WPARAM)NULL, 0);
			}
		}
	}*/
	return CDialog::PreTranslateMessage(pMsg);  
}


DWORD WINAPI Display_Message_Thread(LPVOID pParam) 
{
	STeller_Dlg* pstller = (STeller_Dlg*)pParam;
	stringstream sstream;
	if(pstller->Display_Content_v.empty()){
		pRich->SetWindowTextW(_T("无显示内容！"));
		ExitThread(0);
	}
	pRich->SetWindowTextW(_T("空格键重来，回车退出！"));
	Sleep(1500);
	for(int RowNumber = 2; RowNumber >= 0; RowNumber--){//Display time elapse for warning;
		sstream.str("");
		sstream << RowNumber;
		pRich->SetWindowTextW(NLPOP::string2CString(sstream.str()));
		Sleep(1000);
	}
	strcpy_s(pstller->m_DisplayBuf, MAX_DISPLAY_BUF, "");
	for(vector<pair<string, int>>::iterator vite = pstller->Display_Content_v.begin(); vite != pstller->Display_Content_v.end(); vite++){	
		if(!pstller->Diaplay_Thread_Running){
			ExitThread(0);
		}
		pstller->Reset_Display_Buf(vite->first.c_str());
		pRich->SetWindowTextW(NLPOP::string2CString(pstller->m_DisplayBuf));
		pRich->PostMessage(WM_VSCROLL, SB_BOTTOM,0);
		Sleep(vite->second);
	}
	pstller->Reset_Display_Buf("\n-------------------------\n");
	pstller->Reset_Display_Buf("空格键重复，回车退出！");
	pRich->SetWindowTextW(NLPOP::string2CString(pstller->m_DisplayBuf));

	pstller->Diaplay_Thread_Running = false;
	ExitThread(0);
}

LRESULT STeller_Dlg::STeller_Initialization(WPARAM wParam, LPARAM lParam )
{
	deque<string>* pdstrory = (deque<string>*)wParam;

	if(Diaplay_Thread_Running){
		GetExitCodeThread(hThread, &ThreadId);
		TerminateThread(hThread, ThreadId);
		Display_Content_v.clear();
	}

	if(!Display_Content_v.empty()){// Check wether a space key is pushing down or a new display message is coming;
		if(pdstrory == NULL){		//NULL means a space key is pushed; Repeat display the former message;
			Diaplay_Thread_Running = true;
			hThread = CreateThread(NULL, 0, Display_Message_Thread, (LPVOID)this, 0, &ThreadId);
			return 1;
		}
	}
	if(1 == lParam){// common message;
		for(deque<string>::iterator dite = pdstrory->begin(); dite != pdstrory->end(); dite++){
			if(strlen(dite->data()) == 0){
				continue;
			}
			Display_Content_v.push_back(make_pair(dite->data(), 100));
		}
		pdstrory->clear();
		delete pdstrory;
		this->ShowWindow(SW_SHOW);
		Diaplay_Thread_Running = true;
		hThread = CreateThread(NULL, 0, Display_Message_Thread, (LPVOID)this, 0, &ThreadId);
	}
	else if(0 == lParam){//Display sentnece content;
		for(deque<string>::iterator dite = pdstrory->begin(); dite != pdstrory->end(); dite++){
			if(STopTimeType_SET.find(dite->data()) != STopTimeType_SET.end()){
				Display_Content_v.push_back(make_pair(dite->data(), 300));
			}
			else{
				Display_Content_v.push_back(make_pair(dite->data(), 100));
			}
		}
		pdstrory->clear();
		delete pdstrory;
		this->ShowWindow(SW_SHOW);
		Diaplay_Thread_Running = true;
		hThread = CreateThread(NULL, 0, Display_Message_Thread, (LPVOID)this, 0, &ThreadId);
	}
	return 1;
}
void STeller_Dlg::STeller_Output_Port(const char* outchar)
{
	/*if(!G_pSTeller->IsWindowVisible()){
		G_Buf_Count += strlen(poutstr);
		if(G_Buf_Count > MAX_SENTENCE){
			strcpy_s(G_STellerBuf, MAX_SENTENCE, "");
			G_Buf_Count = strlen(poutstr);
		}
		strcat_s(G_STellerBuf, MAX_SENTENCE, poutstr);
		return;
	}
	
	if(0 != strlen(G_STellerBuf)){
		G_pSTeller->STeller_Output_Port(G_STellerBuf);
		strcpy_s(G_STellerBuf, MAX_SENTENCE, "");
		G_Buf_Count = 0;
	}*/
	Reset_Display_Buf(outchar);
	if(!IsWindowVisible()){
		return;
	}
	if(!Pause_FLag){
		m_richSeller.SetWindowTextW(NLPOP::string2CString(m_DisplayBuf));
		m_richSeller.PostMessage(WM_VSCROLL, SB_BOTTOM,0);
	}
}

//MAX_DISPLAY_BUF;
void STeller_Dlg::Reset_Display_Buf(const char* inputchar)
{

	int CurSize = strlen(m_DisplayBuf);
	int InpSize = strlen(inputchar);
	int i = CurSize+InpSize;
	int j = 0;
	if(!(i < MAX_DISPLAY_BUF)){
		i = CurSize/6*5;
		if(m_DisplayBuf[i] < 0){
			i++;
		}
		while(i < MAX_DISPLAY_BUF){
			if(!m_DisplayBuf[i]){
				break;
			}
			m_DisplayBuf[j++] = m_DisplayBuf[i++];
		}
		m_DisplayBuf[j] = 0;
	}
	j = strlen(m_DisplayBuf);
	i = 0;
	while(j < MAX_DISPLAY_BUF && i < InpSize){
		m_DisplayBuf[j++] = inputchar[i++];
	}
	m_DisplayBuf[j] = 0;
	/*{
		i = MAX_DISPLAY_BUF-1-InpSize;
		int j = 0;
		int k = CurSize - i;
		for(; j < i; j++){
			m_DisplayBuf[j] = m_DisplayBuf[j+k];
		}
		m_DisplayBuf[j] = 0;
	}*/
	/*
	int i = CurSize+InpSize;

	if(i > MAX_DISPLAY_BUF-1){
		i = MAX_DISPLAY_BUF-1;
	}
	m_DisplayBuf[i] = 0;
	
	for( i--; i >= InpSize; i--){
		m_DisplayBuf[i] = m_DisplayBuf[i-InpSize];
	}
	for(i = 0; i < InpSize; i++){
		m_DisplayBuf[i] = inputchar[i];
	}*/
}

void STeller_Dlg::STeller_Output_Port_with_Save(const char* inputchar)
{
	//m_STeller.ShowWindow(SW_SHOW);
	
	STeller_Output_Port(inputchar);
	ResultHistorystr += inputchar;
	ResultHistorystr += "\n";
	if(ResultHistorystr.length() > MAX_SENTENCE){
		OnBnClickedButtonSave();
	}
}


void STeller_Dlg::OnBnClickedButtonClear()
{
	strcpy_s(m_DisplayBuf, MAX_DISPLAY_BUF, "");
	m_richSeller.SetWindowTextW(_T(""));

	// TODO: Add your control notification handler code here
}

void STeller_Dlg::OnBnClickedButtonReturn()
{
	OnCancel();
//	CConsensusApp *app = (CConsensusApp *)AfxGetApp();
//	app->pdlg->ShowWindow(SW_SHOW);
	// TODO: Add your control notification handler code here
}
//---------Msg Histoty----
void STeller_Dlg::OnBnClickedButtonSave()
{
	vector<string> History_v;
	if(NLPOP::FileExist(_T(CEDT_LOG_PATH))){
		Read_Line_InFile_To_Vector_Deque_List<vector<string>>(CEDT_LOG_PATH, History_v, 0, '\n', false);
	}
	//------------------------------------------------
	LPSYSTEMTIME lpSystemTime = new  SYSTEMTIME;
	ostringstream ostrsteam;
	GetLocalTime(lpSystemTime);
	ostrsteam << lpSystemTime->wYear << '_' << lpSystemTime->wMonth << '_' << lpSystemTime->wDay << ' ' << lpSystemTime->wHour << ':' << lpSystemTime->wMinute << ':' << lpSystemTime->wSecond;

	ofstream out(CEDT_LOG_PATH);
	if(out.bad())
		return;
	out << "------------------------------------------------------" << endl;
	out << "Write Time: " << ostrsteam.str() << "                        |" << endl;
	out << "------------------------------------------------------" << endl;
	out << ResultHistorystr << endl;
	out << endl;
	for(size_t i = 0; (i < 2000)&&(i < History_v.size()); i++){
		out << History_v[i] << endl;
	}
	out.close();
	ResultHistorystr = "";
	delete lpSystemTime;
	STeller_Output_Port("Result is saved...");
}
void STeller_Dlg::OnBnClickedButtonSaveAs()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, L"Text files (*.txt)\0*.txt\0\0", GetTopLevelParent());
	if (dlg.DoModal() != IDOK)
		return;
	CString csfilename = dlg.GetFileName();
	csfilename += _T(".txt");
	string sfilename = NLPOP::CString2string(csfilename);

	//string sfilename = "F:\\result.txt";
	ofstream out(sfilename.c_str());
	if(out.bad())
		return;
	out.clear();
	out.seekp(0, ios::beg);
	out << m_DisplayBuf;
	out.close();

	STeller_Output_Port("Result is saved...");
	return;
}

void STeller_Dlg::OnBnClickedButtonOpenHistory()
{
	char* buffer;
	unsigned long FileLength;
	CFile m_File;
	try{
		m_File.Open(_T(CEDT_LOG_PATH), CFile::modeReadWrite);
		m_File.SeekToBegin();
	} 
	catch(...)
	{
		AppCall::Secretary_Message_Box("打开文件出错in STeller_Dlg()", MB_OK);
	}
	FileLength = (unsigned long)m_File.GetLength();
	buffer = new char[FileLength];
	m_File.Read((void*)buffer, FileLength);
	m_File.Close();

	STeller_Output_Port(buffer);
}

void STeller_Dlg::OnBnClickedStellerPause()
{
	if(!Pause_FLag){
		Pause_FLag = true;
		//::SetDlgItemText(AfxGetApp()->m_pMainWnd->m_hWnd, IDC_STELLER_PAUSE, _T("Pause")); //((CButton*)(IDC_STELLER_PAUSE))
		//((CButton*)(IDC_STELLER_PAUSE))->SetWindowText(_T("Pause"));
		CWnd* pWnd = GetDlgItem(IDC_STELLER_PAUSE);  
		pWnd->SetWindowText(_T("Continue"));
	}
	else{
		Pause_FLag = false;
		//CButton.SetWindowText("Continue");
		//((CButton*)(IDC_STELLER_PAUSE))->SetWindowText(_T("Continue"));
		//::SetDlgItemText(AfxGetApp()->m_pMainWnd->m_hWnd, IDC_STELLER_PAUSE, _T("Continue"));
		CWnd* pWnd = GetDlgItem(IDC_STELLER_PAUSE);  
		pWnd->SetWindowText(_T("Pause"));
		m_richSeller.SetWindowTextW(NLPOP::string2CString(m_DisplayBuf));
		m_richSeller.PostMessage(WM_VSCROLL, SB_BOTTOM,0);
	}
		
}
