// Consensus_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Consensus.h"
#include "Consensus_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI Consensus_Responce_Threaad(LPVOID pParam);
// CAboutDlg dialog used for App About

STeller_Dlg* G_pSTeller;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CConsensus_Dlg dialog
CConsensus_Dlg::CConsensus_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConsensus_Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	XML_Parsing_Flag = false;
	MaxentRan_Flag = false;
	MSG_Thread_Flag = false;
	UserInputRequireFlag = false;
	ACE_Coupus_Loaded_Flag = false;
	XML_Cigaword_Parsing_Flag = false;
	System_Busy_Flag = false;
}

void CConsensus_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_sheet);
	DDX_Control(pDX, IDC_RICHEDIT23, m_ResponceEdit);
	DDX_Control(pDX, IDC_RICHEDIT21, m_CorpusEdit);
	DDX_Control(pDX, IDC_RICHEDIT22, m_GigawordEdit);
}

BEGIN_MESSAGE_MAP(CConsensus_Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DISPLAY, OnResponceDisplay)
	
	ON_BN_CLICKED(IDC_BUTTON2, &CConsensus_Dlg::OnBnClickedCorpusPathButton)
	ON_BN_CLICKED(IDC_BUTTON1, &CConsensus_Dlg::OnBnClickedDataPathButton)
	ON_BN_CLICKED(IDC_PAD_BUTTON, &CConsensus_Dlg::OnBnClickedPadButton)

END_MESSAGE_MAP()


// CConsensus_Dlg message handlers

BOOL CConsensus_Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_S_Producer = CreateSemaphore(NULL, MAX_PRODUCT_SEMAPHORE, MAX_PRODUCT_SEMAPHORE, NULL);	//初始计数为M
    m_S_Consumer = CreateSemaphore(NULL, 0, MAX_PRODUCT_SEMAPHORE, NULL);						//初始计数为0
    m_E_MessageListEvent = CreateEvent(NULL, FALSE, TRUE, NULL);

	ACECorpusFolder = CHINESE_CORPUS_FOLDER;
	DataFolder = DATA_FOLDER;
	MaxentRunPath = MAXENT_FOLDER;
	GigawordFolder = Gigawrod_CORPUS_FOLDER;

	m_pSTeller = new STeller_Dlg;
	m_pSTeller->Create(IDD_SELLER_DIALOG, NULL);
	m_pSTeller->ShowWindow(SW_HIDE);
	G_pSTeller = m_pSTeller;

#ifdef CONSENSUS_ENET
	m_sheet.AddPage(_T("Analyzer"), &m_CENET_Dlg, IDD_ANALYZER);
	m_CENET_Dlg.p_CParentDlg = this;
	m_CENET_Dlg.GigawordFolder = EVENT_ORGANIZATION_FOLDER;
	m_CENET_Dlg.ACECorpusFolder = CHINESE_CORPUS_FOLDER;
	//m_CENET_Dlg.OnBnClickedAnalExample();
#endif

#ifdef CONSENSUS_CDOC
	m_CDOC_Dlg.p_CParentDlg = this;
	m_sheet.AddPage(_T("Document Event"), &m_CDOC_Dlg, IDD_CDOC);
	ACECorpusFolder = Gigawrod_CORPUS_FOLDER;
	m_CDOC_Dlg.ACECorpusFolder = EVENT_ORGANIZATION_FOLDER;

#endif

#ifdef CONSENSUS_CEDT
	m_CCEDT_Dlg.p_CParentDlg = this;
	m_sheet.AddPage(_T("Named Entity"), &m_CCEDT_Dlg, IDD_CEDT);
	ACECorpusFolder = CHINESE_CORPUS_FOLDER;
#endif

#ifdef CONSENSUS_CRDC
	m_CCRDC_Dlg.p_CParentDlg = this;
	m_sheet.AddPage(_T("Relation"), &m_CCRDC_Dlg, IDD_CRDC);
	ACECorpusFolder = CHINESE_CORPUS_FOLDER;
	if(m_CCRDC_Dlg.For_English_Relation_Flag){
		ACECorpusFolder = ENGLISH_CORPUS_FOLDER;
	}
		
#endif

#ifdef CONSENSUS_ECOR
	m_ECOR_Dlg.p_CParentDlg = this;
	m_sheet.AddPage(_T("指代消解"), &m_ECOR_Dlg, IDD_ECOR);
	ACECorpusFolder = ENGLISH_CORPUS_FOLDER;
#endif

	m_sheet.Show();
	Enable_Usable_Button();

#ifdef CONSENSUS_ENET
	m_CENET_Dlg.Dispaly_Event_Network_Pictures();
#endif

	m_CorpusEdit.SetWindowTextW(NLPOP::string2CString(ACECorpusFolder));
	m_GigawordEdit.SetWindowTextW(_T(Gigawrod_CORPUS_FOLDER));

	msgThread = CreateThread(NULL, 0, Consensus_Responce_Threaad, (LPVOID)this, 0, &msgphreadId);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CConsensus_Dlg::OnCancel()
{
	if(System_Busy_Flag){
		int rtn = AfxMessageBox(_T("有任务正在运行，确定要退出吗？"), MB_YESNOCANCEL);
		if(rtn != IDYES){
			return;
		}
	}
	#ifdef CONSENSUS_ENET
	if(m_CENET_Dlg.Opened_CDENer_Dlg_Flag){
		m_CENET_Dlg.p_m_CDENer_Dlg->Init_Reviewer();
		delete m_CENET_Dlg.p_m_CDENer_Dlg;
	}
	#endif
	CConsensusApp *app = (CConsensusApp *)AfxGetApp();
	delete m_pSTeller;

	for(size_t i = 0; i < m_GigaDOC_v.size(); i++){
		delete m_GigaDOC_v[i];
	}
	CDialog::OnCancel();
}

void CConsensus_Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
void CConsensus_Dlg::Notification_Input(const char* inputchar)
{
	//CConsensusApp *app = (CConsensusApp *)AfxGetApp();
#ifdef CONSENSUS_ENET
	if(m_CENET_Dlg.Command_From_Notification(inputchar)){
		return;
	}
#endif

	if(!strcmp("", inputchar) || !strcmp("show", inputchar)){
		m_pSTeller->ShowWindow(SW_SHOW);
	}
	if(!strcmp("social", inputchar)){
		string FileName = ENET_FOLDER;
		FileName += "Social_Analysis.paj";
		AppCall::Consensus_Open_Process(FileName.c_str(), Pajet_Path);
	}
	if(!strcmp("000", inputchar)){
		string FileName = ENET_FOLDER;
		FileName += "PLT_Analysis_relation.paj";
		AppCall::Consensus_Open_Process(FileName.c_str(), Pajet_Path);
	}
}

BOOL CConsensus_Dlg::PreTranslateMessage(MSG* pMsg)
{
	if(WM_KEYDOWN == pMsg->message )  //keyboard buttondown
	{  
		UINT nKey = (int) pMsg->wParam;
		if( VK_RETURN == nKey || VK_ESCAPE == nKey )
		{
			//-----------------------------------------------
			m_ResponceEdit.GetWindowText(m_Notification);
			Notification_Input(NLPOP::CString2string(m_Notification).c_str());
			m_ResponceEdit.SetWindowTextW(_T(""));
			return TRUE ; 
			//----------------------------------------------------
		}
	}
	else if(WM_LBUTTONDOWN == pMsg->message )//(WM_MOUSEMOVE == pMsg->message)|| 
	{
		CWnd* pWnd = (CWnd*)   GetDlgItem(IDC_RICHEDIT23);
		if((pWnd == GetFocus()) && !UserInputRequireFlag)
		{
			LPRECT lpRect = new RECT;
			pWnd->GetClientRect(lpRect);
			pWnd->ClientToScreen(lpRect);

			LPPOINT lpoint=new tagPOINT; //标示menu显示位置
			::GetCursorPos(lpoint);

			if((lpRect->left < lpoint->x) && (lpRect->right > lpoint->x) && (lpRect->bottom > lpoint->y) && (lpRect->top < lpoint->y))
			{
				UserInputRequireFlag = true;
				m_ResponceEdit.SetWindowTextW(_T(""));
			}
			delete lpRect;
			delete lpoint; //回收资源
		}
	}
	else if(160 ==  pMsg->message)
	{
		UserInputRequireFlag = false;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CConsensus_Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CConsensus_Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI Consensus_Responce_Threaad(LPVOID pParam)
{
	CConsensusApp *app = (CConsensusApp *)AfxGetApp();
	CConsensus_Dlg& dlgRef = *(CConsensus_Dlg*)pParam;
	char OutputChar[MAX_SENTENCE];
	char CurputChar[MAX_SENTENCE];
	char sChar[3];
	sChar[2] = 0;
	MSG msg;
	BOOL bRet;
	dlgRef.MSG_Thread_Flag = true;
	while(true){
		if(dlgRef.XML_Parsing_Flag){
			strcpy_s(OutputChar, MAX_SENTENCE, "\"XML Parse\" is running ...");
		}
#ifdef CONSENSUS_CEDT
		else if(dlgRef.m_CCEDT_Dlg.CEDT_Busy_Flag){
			strcpy_s(OutputChar, MAX_SENTENCE, "CEDT is running...");
		}
#endif

#ifdef CONSENSUS_CRDC
		else if(dlgRef.m_CCRDC_Dlg.CRDC_Busy_Flag){
			strcpy_s(OutputChar, MAX_SENTENCE, "CRDC is running...");
		}
#endif

#ifdef CONSENSUS_ECOR
		else if(dlgRef.m_ECOR_Dlg.ECOR_Busy_Flag){
			strcpy_s(OutputChar, MAX_SENTENCE, "ECOR is running...");
		}
#endif

#ifdef CONSENSUS_CDOC
		else if(dlgRef.m_CDOC_Dlg.CDOC_Busy_Flag){
			strcpy_s(OutputChar, MAX_SENTENCE, "CDOC is running...");
		}
#endif
#ifdef CONSENSUS_ENET
		else if(dlgRef.m_CENET_Dlg.CENET_Busy_Flag){
			strcpy_s(OutputChar, MAX_SENTENCE, "ENET is running...");
		}
#endif
		else{
			strcpy_s(OutputChar, MAX_SENTENCE, "Consensus is ready...");
		}
		strcpy_s(CurputChar, MAX_SENTENCE, "");
		::SendMessage(dlgRef.m_hWnd, WM_DISPLAY,(WPARAM)CurputChar, 1);
		Sleep(500);
		for(size_t i = 0; i < strlen(OutputChar); ){
			sChar[0] = OutputChar[i++];
			sChar[1] = 0;
			if(sChar[0] < 0){
				sChar[1] = OutputChar[i++];
			}
			strcat_s(CurputChar, MAX_SENTENCE, sChar);
			if((bRet = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) != 0)//PM_NOREMOVE, 
			{
				if (bRet == -1){
					// handle the error and possibly exit
				}
				else{
					if(WaitForSingleObject(dlgRef.m_S_Consumer, INFINITE) == WAIT_OBJECT_0){//consumer
						if(WaitForSingleObject(dlgRef.m_E_MessageListEvent, RESPONCE_WAIT_TIME) == WAIT_OBJECT_0){	// OK now, Get product then
							strcpy_s(CurputChar, MAX_SENTENCE, dlgRef.outpusmsg_l.front().c_str());
							dlgRef.outpusmsg_l.pop_front();
							ReleaseSemaphore(dlgRef.m_S_Producer, 1, NULL);// releasee productor’s semaphore
							SetEvent(dlgRef.m_E_MessageListEvent);
						}
						else{
							ReleaseSemaphore(dlgRef.m_S_Consumer, 1, NULL);
						}
					}
					::SendMessage(dlgRef.m_hWnd, WM_DISPLAY,(WPARAM)CurputChar, 1);
					Sleep(3500);
					break;
				}
			}
			::SendMessage(dlgRef.m_hWnd, WM_DISPLAY,(WPARAM)CurputChar, 1);
			if(strcmp(sChar, " ")){
				Sleep(300);
			}
			else{
				Sleep(100);
			}
		}
	}
	ExitThread(0);
}


LRESULT CConsensus_Dlg::OnResponceDisplay(WPARAM wParam, LPARAM lParam)
{
	if(UserInputRequireFlag){
		return 1;
	}
	CString loc_Responce((char*)wParam);
	m_ResponceEdit.SetWindowTextW(loc_Responce);
	return 1;
}

void CConsensus_Dlg::OnBnClickedCorpusPathButton()
{
	CString Fname;
	CString Fpath;
	CFileDialog OpenDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST, _T(""), NULL);
	int nModal = OpenDialog.DoModal();
	if (nModal == IDOK){
		Fpath = OpenDialog.GetPathName();
		Fpath = Fpath.Left(Fpath.ReverseFind('\\') + 1);
		Fname = OpenDialog. GetFileName();
		m_CorpusEdit.SetWindowTextW(Fpath);
		ACECorpusFolder = NLPOP::CString2string(Fpath);
	}
	Enable_Usable_Button();
}
void CConsensus_Dlg::Enable_Usable_Button()
{

#ifdef CONSENSUS_CEDT
	m_CCEDT_Dlg.Enable_Usable_Button_CEDT();
#endif

#ifdef CONSENSUS_CRDC
	m_CCRDC_Dlg.Enable_Usable_Button_CRDC();
#endif

#ifdef CONSENSUS_CDOC
	m_CDOC_Dlg.Enable_Usable_Button_CDOC();
#endif

#ifdef CONSENSUS_ENET
	m_CENET_Dlg.Enable_Usable_Button_ENET();
#endif
}


void CConsensus_Dlg::OnBnClickedDataPathButton()
{
	CString Fname;
	CString Fpath;
	CFileDialog OpenDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST, _T(""), NULL);
	int nModal = OpenDialog.DoModal();
	if (nModal == IDOK){
		Fpath = OpenDialog.GetPathName();
		Fpath = Fpath.Left(Fpath.ReverseFind('\\') + 1);
		Fname = OpenDialog. GetFileName();
		m_GigawordEdit.SetWindowTextW(Fpath);
		GigawordFolder = NLPOP::CString2string(Fpath);
	}
	Enable_Usable_Button();
}

void CConsensus_Dlg::Output_MSG(const char* outchar, WPARAM wParam = 0)
{
	if(!MSG_Thread_Flag){
		return;
	}
	if(WaitForSingleObject(m_S_Producer, RESPONCE_WAIT_TIME) == WAIT_OBJECT_0){
		if(WaitForSingleObject(m_E_MessageListEvent, RESPONCE_WAIT_TIME) == WAIT_OBJECT_0){	
			outpusmsg_l.push_back(outchar);// put product	
		}
		ReleaseSemaphore(m_S_Consumer, 1, NULL);//
		SetEvent(m_E_MessageListEvent);// set event to signal
	}
	PostThreadMessage(msgphreadId, WM_MESSAGE, (WPARAM)wParam, (LPARAM)0);
}


void STeller_Responce_Message(const char* poutstr)
{
	G_pSTeller->STeller_Output_Port(poutstr);
		
}

void CConsensus_Dlg::OnBnClickedPadButton()
{
	//CConsensusApp *app = (CConsensusApp *)AfxGetApp();
	if(!m_pSTeller->IsWindowVisible()){
		m_pSTeller->ShowWindow(SW_SHOW);
		STeller_Responce_Message("");
	}
	else{
		m_pSTeller->ShowWindow(SW_HIDE);
	}
}

void STeller_Responce_Message_with_Save(const char* poutstr)
{
	G_pSTeller->STeller_Output_Port_with_Save(poutstr);
}


