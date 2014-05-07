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
	XML_Parse_Flag = false;
	MaxentRan_Flag = false;
	MSG_Thread_Flag = false;
	UserInputRequireFlag = false;
	Notification_Pad_Flag = false;
	
}

void CConsensus_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_sheet);
	DDX_Control(pDX, IDC_RICHEDIT23, m_ResponceEdit);
	DDX_Control(pDX, IDC_RICHEDIT21, m_CorpusEdit);
	DDX_Control(pDX, IDC_RICHEDIT22, m_DataEdit);
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

	CorpusFolder = CORPUS_FOLDER;
	DataFolder = DATA_FOLDER;
	MaxentRunPath = MAXENT_FOLDER;

	m_CCRDC_Dlg.p_CParentDlg = this;
	// TODO: Add extra initialization here
	
	m_sheet.AddPage(_T("关系识别"), &m_CCRDC_Dlg, IDD_CRDC);

	m_sheet.Show();

	m_CCRDC_Dlg.Enable_Usable_Button_CRDC();
	m_CorpusEdit.SetWindowTextW(_T(CORPUS_FOLDER));
	m_DataEdit.SetWindowTextW(_T(DATA_FOLDER));

	msgThread = CreateThread(NULL, 0, Consensus_Responce_Threaad, (LPVOID)this, 0, &msgphreadId);

	return TRUE;  // return TRUE  unless you set the focus to a control
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
	if(!strcmp("", inputchar) || !strcmp("show", inputchar)){
		CConsensusApp *app = (CConsensusApp *)AfxGetApp();
		app->m_STeller.ShowWindow(SW_SHOW);
		Notification_Pad_Flag = true;
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
		if(dlgRef.XML_Parse_Flag){
			strcpy_s(OutputChar, MAX_SENTENCE, "\"XML Parse\" is running ...");
		}
		else if(dlgRef.m_CCRDC_Dlg.CRDC_Busy_Flag){
			strcpy_s(OutputChar, MAX_SENTENCE, "CRDC is running...");
		}
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
		CorpusFolder = NLPOP::CString2string(Fpath);
	}
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
		m_DataEdit.SetWindowTextW(Fpath);
		DataFolder = NLPOP::CString2string(Fpath);
	}
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
		ReleaseSemaphore(m_S_Consumer, 1, NULL);// relase comsumer’s semaphore
		SetEvent(m_E_MessageListEvent);// set event to signal
	}
	PostThreadMessage(msgphreadId, WM_MESSAGE, (WPARAM)wParam, (LPARAM)0);
}



void CConsensus_Dlg::OnBnClickedPadButton()
{
	CConsensusApp *app = (CConsensusApp *)AfxGetApp();
	if(!Notification_Pad_Flag){
		app->m_STeller.ShowWindow(SW_SHOW);
		Notification_Pad_Flag = true;
	}
	else{
		app->m_STeller.ShowWindow(SW_HIDE);
		Notification_Pad_Flag = false;
	}
}
