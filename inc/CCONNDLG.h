#pragma once


// CCONNDLG 对话框

class CCONNDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CCONNDLG)

public:
	CCONNDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCONNDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg void OnClose();
private:
	DWORD m_ipaddr;
	CString m_user;
	CString m_pwd;
public:
	CString str_ipaddr;
	CString str_delay;
	CString str_user;
public:
	afx_msg void OnBnClickedDologin();
	afx_msg void OnBnClickedDoexit();
	virtual BOOL OnInitDialog();
};
