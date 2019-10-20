
// ICDlg.h: 头文件
//

#pragma once


// CICDlg 对话框
class CICDlg : public CDialogEx
{
// 构造
public:
	CICDlg(CWnd* pParent = nullptr);	// 标准构造函数
	CICDlg(CString str_ipaddr, CString str_delay, CString str_user, CWnd* pParent = nullptr);	// 自定义构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButton2();

private:
	CString str_ipaddr;
	CString str_delay;
	CString str_user;
	CString m_log;
public:
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
private:
	CEdit ctl_log;
private:
	void sync_log()
	{
		//变量值更新到控件
		UpdateData(FALSE);
		//使日志进度条最底端
		ctl_log.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	}
	void log_server_msg();
};
