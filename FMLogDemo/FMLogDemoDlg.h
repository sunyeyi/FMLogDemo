
// FMLogDemoDlg.h : header file
//

#pragma once
#include <vector>

class AddLogThreadParam
{
public:
    AddLogThreadParam()
        : m_nTotalLogCount(0)
        , m_nWroteLogCount(0)
        , m_nThreadIndex(0)
        , m_nThreadId(-1)
    {
    }

public: //thread input parameters
    int m_nTotalLogCount;
    int m_nThreadIndex;
    HWND m_hMain;

public: //thread output parameters
    int m_nWroteLogCount;
    DWORD m_nThreadId;
};

// CFMLogDemoDlg dialog
class CFMLogDemoDlg : public CDialogEx
{
// Construction
public:
	CFMLogDemoDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CFMLogDemoDlg();

// Dialog Data
	enum { IDD = IDD_FMLOGDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedBtnStartNewThread();

    afx_msg LRESULT OnLogProgressReport(WPARAM wParam, LPARAM lParam);


private:
    void StartNewThread(int nThreadIndex);
    bool WaitForAllAddLogThread();
    void ReleaseStaicControl();

private:
    std::vector<CString> m_vecMsg;
    std::vector<CStatic*> m_vecStatic;
    std::vector<CWinThread*> m_vecThread;
    std::vector<AddLogThreadParam> m_vecThreadParam;
    int s_nThreadIndex;

public:
    afx_msg void OnBnClickedBtnStartAllThread();
};
