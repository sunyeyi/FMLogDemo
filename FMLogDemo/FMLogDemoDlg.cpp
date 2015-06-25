
// FMLogDemoDlg.cpp : implementation file
//

#include "StdAfxFMDLL.h"
#include "FMLogDemo.h"
#include "FMLogDemoDlg.h"
#include "afxdialogex.h"
#include "FMLog\FMLoggerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define ID_UPDATE_TEST_LOG_PROGRESS WM_USER + 1

#define  MAX_THREAD_NUMBER              20
#define  SLEEP_TIME_EACH_LOG            1
#define  LOG_COUNT_EACH_THREAD          1000

volatile bool g_bExitSystem = false;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CFMLogDemoDlg dialog

CFMLogDemoDlg::CFMLogDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFMLogDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_vecMsg.resize(MAX_THREAD_NUMBER);
    m_vecStatic.resize(MAX_THREAD_NUMBER);
    m_vecThread.resize(MAX_THREAD_NUMBER);
    m_vecThreadParam.resize(MAX_THREAD_NUMBER);
    s_nThreadIndex = 0;
}

void CFMLogDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFMLogDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CFMLogDemoDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CFMLogDemoDlg::OnBnClickedCancel)
    ON_BN_CLICKED(ID_BTN_START_NEW_THREAD, &CFMLogDemoDlg::OnBnClickedBtnStartNewThread)
    ON_MESSAGE(ID_UPDATE_TEST_LOG_PROGRESS, &CFMLogDemoDlg::OnLogProgressReport)
    ON_BN_CLICKED(ID_BTN_START_ALL_THREAD, &CFMLogDemoDlg::OnBnClickedBtnStartAllThread)
END_MESSAGE_MAP()


// CFMLogDemoDlg message handlers

BOOL CFMLogDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFMLogDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFMLogDemoDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFMLogDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

FMLOG_MODULE_DECLARE(DemoTest);
FMLOG_MODULE_IMPLEMENT(DemoTest);

void CFMLogDemoDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    
    FMLOG_FATAL(DemoTest, _T("OK button is pressed."));

    FMLOG_FATAL_N(_T("end of CFMLogDemoDlg::OnBnClickedOk"));

    this->SetWindowText(_T("OK button is pressed. Waiting to exit..."));
    
    if (WaitForAllAddLogThread())
    {
        CDialogEx::OnOK();
    }
}

void CFMLogDemoDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here

    FMLOG_FATAL(DemoTest, _T("Cancel button is pressed."));
    
    g_bExitSystem = true;

    if (WaitForAllAddLogThread())
    {
        CDialogEx::OnCancel();
    }
}

FMLOG_MODULE_DECLARE(ThreadTest);
FMLOG_MODULE_IMPLEMENT(ThreadTest);

void MoreAndMoreTest();

FMLOG_MODULE_DECLARE(TestModule);
FMLOG_MODULE_IMPLEMENT(TestModule);
void MoreAndMoreTest()
{
    FMLOG_INFO_SPE(TestModule, _T("log message "));
    FMLOG_INFO_SPE_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_INFO_SPE_N(_T("log message "));
    FMLOG_INFO_SPE_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_ERROR_UNK(TestModule, _T("log message "));
        FMLOG_ERROR_UNK_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_ERROR_UNK_N(_T("log message "));
    FMLOG_ERROR_UNK_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_FATAL(TestModule, _T("log message "));
    FMLOG_FATAL_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_FATAL_N(_T("log message "));
    FMLOG_FATAL_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_ERROR(TestModule, _T("log message "));
    FMLOG_ERROR_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_ERROR_N(_T("log message "));
    FMLOG_ERROR_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_WARN(TestModule, _T("log message "));
    FMLOG_WARN_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_WARN_N(_T("log message "));
    FMLOG_WARN_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_INFO(TestModule, _T("log message "));
    FMLOG_INFO_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_INFO_N(_T("log message "));
    FMLOG_INFO_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_DEBUG(TestModule, _T("log message "));
    FMLOG_DEBUG_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_DEBUG_N(_T("log message "));
    FMLOG_DEBUG_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_TRACE(TestModule, _T("log message "));
    FMLOG_TRACE_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_TRACE_N(_T("log message "));
    FMLOG_TRACE_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_TEMP(TestModule, _T("log message "));
    FMLOG_TEMP_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_TEMP_N(_T("log message "));
    FMLOG_TEMP_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_ALWAYS(TestModule, _T("log message "));
    FMLOG_ALWAYS_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_ALWAYS_N(_T("log message "));
    FMLOG_ALWAYS_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    int x = 3;
    FMLOG_ASSERT((x == 2), _T("wrong expression"));
    FMLOG_ASSERT_V((x == 2), _T("wrong expression. x = %d"), x);
    x = 2;
    FMLOG_ASSERT((x == 2), _T("wrong expression"));
    FMLOG_ASSERT_V((x == 2), _T("wrong expression. x = %d"), x);
}

UINT AddLogThreadProc(LPVOID pParam)
{
    DWORD start = GetTickCount();

    AddLogThreadParam* pLogParam = (AddLogThreadParam*)pParam;
    pLogParam->m_nThreadId = ::GetCurrentThreadId();

    int nPercent = 1;
    for (int i = 0; i < pLogParam->m_nTotalLogCount; ++i)
    {
        pLogParam->m_nWroteLogCount++;

        CString strMsg;
        strMsg.Format(_T("Log message %09d from thread %02d"), pLogParam->m_nWroteLogCount, pLogParam->m_nThreadIndex);
        FMLOG_FATAL(ThreadTest, strMsg);

        MoreAndMoreTest();

        double nTotal = pLogParam->m_nTotalLogCount;
        int nNumber = i + 1;
        if (100*nNumber/nTotal > nPercent-(1E-10))
        {
            //::SendMessage(pLogParam->m_hMain, ID_UPDATE_TEST_LOG_PROGRESS, (WPARAM)pLogParam->m_nThreadIndex, (LPARAM)pLogParam->m_nWroteLogCount);
            ::PostMessage(pLogParam->m_hMain, ID_UPDATE_TEST_LOG_PROGRESS, (WPARAM)pLogParam->m_nThreadIndex, (LPARAM)pLogParam->m_nWroteLogCount);

            ++nPercent;
        }

        if (SLEEP_TIME_EACH_LOG > 10)
        {
            Sleep(SLEEP_TIME_EACH_LOG);
        }

        if (g_bExitSystem)
        {
            DWORD end = GetTickCount();
            CString strMsg;
            DWORD nDuration = end - start;
            strMsg.Format(_T("Thread:%d, TotalDuration=%d Canceled."), pLogParam->m_nThreadIndex, nDuration);
            FMLOG_FATAL(ThreadTest, strMsg);

            delete pLogParam;
            return 99;
        }
    }

    DWORD end = GetTickCount();
    CString strMsg;
    DWORD nDuration = end - start;
    strMsg.Format(_T("Thread:%d, TotalDuration=%d Finished."), pLogParam->m_nThreadIndex, nDuration);
    FMLOG_FATAL(ThreadTest, strMsg);

    ::PostMessage(pLogParam->m_hMain, ID_UPDATE_TEST_LOG_PROGRESS, (WPARAM)pLogParam->m_nThreadIndex, (LPARAM)pLogParam->m_nWroteLogCount);

    delete pLogParam;
    return 0;
}

LRESULT  CFMLogDemoDlg::OnLogProgressReport(WPARAM wParam, LPARAM lParam)
{
    int nThreadIndex = (int)wParam;
    int nWroteLogCount = (int)lParam;

    ASSERT(nThreadIndex >= 0 && nThreadIndex < MAX_THREAD_NUMBER);

    m_vecThreadParam[nThreadIndex].m_nWroteLogCount = nWroteLogCount;
    
    CString strMsg;
    strMsg.Format(_T("Thread %02d   Log Count %5d   Logged %5d  %3.1f%%")
        , m_vecThreadParam[nThreadIndex].m_nThreadIndex
        , m_vecThreadParam[nThreadIndex].m_nTotalLogCount
        , m_vecThreadParam[nThreadIndex].m_nWroteLogCount
        , m_vecThreadParam[nThreadIndex].m_nWroteLogCount * 100.0 / m_vecThreadParam[nThreadIndex].m_nTotalLogCount
        );

    if (m_vecThreadParam[nThreadIndex].m_nWroteLogCount == m_vecThreadParam[nThreadIndex].m_nTotalLogCount)
    {
        strMsg += _T(" Finished ✔");
    }

    m_vecMsg[nThreadIndex] = strMsg;

    //GetDlgItem(IDC_STATIC)->SetWindowText(strMsg);
    m_vecStatic[nThreadIndex]->SetWindowText(m_vecMsg[nThreadIndex]);


    return 0;
}

void CFMLogDemoDlg::OnBnClickedBtnStartNewThread()
{
    // TODO: Add your control notification handler code here

    if (s_nThreadIndex < MAX_THREAD_NUMBER)
    {
        StartNewThread(s_nThreadIndex);

        ++s_nThreadIndex;
    }

    // disable button if thread count is enough.
    if (s_nThreadIndex == MAX_THREAD_NUMBER)
    {
        GetDlgItem(ID_BTN_START_NEW_THREAD)->EnableWindow(FALSE);
        GetDlgItem(ID_BTN_START_ALL_THREAD)->EnableWindow(FALSE);
        s_nThreadIndex = 0;
    }

    //this->OnBnClickedOk();
    //this->OnBnClickedCancel();
}

void CFMLogDemoDlg::OnBnClickedBtnStartAllThread()
{
    // TODO: Add your control notification handler code here
    
    while (s_nThreadIndex < MAX_THREAD_NUMBER)
    {
        StartNewThread(s_nThreadIndex);

        ++s_nThreadIndex;
    }

    // disable button if thread count is enough.
    if (s_nThreadIndex == MAX_THREAD_NUMBER)
    {
        GetDlgItem(ID_BTN_START_NEW_THREAD)->EnableWindow(FALSE);
        GetDlgItem(ID_BTN_START_ALL_THREAD)->EnableWindow(FALSE);
        s_nThreadIndex = 0;
    }
}

void CFMLogDemoDlg::ReleaseStaicControl()
{
    for (size_t i = 0; i < m_vecStatic.size(); ++i)
    {
        if (m_vecStatic[i] != nullptr)
        {
            m_vecStatic[i]->DestroyWindow();
            delete m_vecStatic[i];
            m_vecStatic[i] = nullptr;
        }
    }
}

CFMLogDemoDlg::~CFMLogDemoDlg()
{
    ReleaseStaicControl();

    for (size_t i = 0; i < m_vecThread.size(); ++i)
    {
        if (m_vecThread[i] != nullptr)
        {
            delete m_vecThread[i];
            m_vecThread[i] = nullptr;
        }
    }
}

bool CFMLogDemoDlg::WaitForAllAddLogThread()
{
    int nThreadCount = 0;
    HANDLE pThread[MAX_THREAD_NUMBER];
    for (size_t i = 0; i < m_vecThread.size(); ++i)
    {
        if (m_vecThread[i] != nullptr)
        {
            m_vecThread[i]->AssertValid();

            pThread[nThreadCount] = m_vecThread[i]->m_hThread;
            nThreadCount++;
        }
    }

    if (nThreadCount > 0)
    {
        DWORD dw = WaitForMultipleObjects(nThreadCount, pThread, TRUE, INFINITE);
        if (dw == WAIT_OBJECT_0)
        {
            return true;
        }
        else if (dw == WAIT_FAILED)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return true;
}

void CFMLogDemoDlg::StartNewThread(int nThreadIndex)
{
    // create a new static control
    if (m_vecStatic[nThreadIndex] == nullptr)
    {
        CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC);
        CStatic* pNewStatic = new CStatic();
        CRect newRect;
        pStatic->GetClientRect(&newRect);
        newRect.MoveToX(20);
        newRect.MoveToY(newRect.bottom + 40 + nThreadIndex * 20);
        pNewStatic->Create(_T("gogogo..."), WS_CHILD | WS_VISIBLE | SS_LEFT, newRect, this);

        m_vecStatic[nThreadIndex] = pNewStatic;
    }

    // create a new thread
    if (m_vecThread[nThreadIndex] == nullptr)
    {
        AddLogThreadParam* pLogParam = new AddLogThreadParam();
        pLogParam->m_nTotalLogCount = LOG_COUNT_EACH_THREAD;
        pLogParam->m_hMain = GetSafeHwnd();
        pLogParam->m_nThreadIndex = nThreadIndex;
        m_vecThreadParam[nThreadIndex] = *pLogParam;

        m_vecThread[nThreadIndex] = AfxBeginThread(AddLogThreadProc, pLogParam, 0, CREATE_SUSPENDED);

        m_vecThread[nThreadIndex]->m_bAutoDelete = FALSE;
    }
}
