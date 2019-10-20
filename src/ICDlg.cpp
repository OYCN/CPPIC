
// ICDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "IC.h"
#include "ICDlg.h"
#include "afxdialogex.h"
#include "CCONNDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CICDlg 对话框



CICDlg::CICDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IC_DIALOG, pParent)
	, str_ipaddr(_T(""))
	, str_delay(_T(""))
	, str_user(_T(""))
	, m_log(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CICDlg::CICDlg(CString ipaddr, CString delay, CString user, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IC_DIALOG, pParent)
{
	str_ipaddr = ipaddr;
	str_delay = delay;
	str_user = user;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CICDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IPADDR, str_ipaddr);
	DDX_Text(pDX, IDC_DELAY, str_delay);
	DDX_Text(pDX, IDC_USER, str_user);
	DDX_Text(pDX, IDC_LOG, m_log);
	DDX_Control(pDX, IDC_LOG, ctl_log);
}

BEGIN_MESSAGE_MAP(CICDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON2, &CICDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CICDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CICDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CICDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CICDlg 消息处理程序

BOOL CICDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_log.Format(TEXT(">>> 当前服务器为 %s | 延迟为 %s | 用户名为 %s"), str_ipaddr, str_delay, str_user);
	sync_log();
	log_server_msg();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CICDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CICDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CICDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	// 设置图标
	SetClassLong(m_hWnd, GCL_HICON, (LONG)AfxGetApp()->LoadIconW(IDI_ICON_BM));
	//设置窗口大小
	//MoveWindow(0, 0, 800, 500);
	//设置居中显示
	CenterWindow();
	return 0;
}

//退出
void CICDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}


//清空日志
void CICDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_log = TEXT("");
	UpdateData(FALSE);
}

//切换服务器
void CICDlg::OnBnClickedButton3()
{
	//实例化登陆窗
	CCONNDLG dlg;
	if (dlg.DoModal() != IDOK) return;
	//从登陆窗取变量
	str_ipaddr = dlg.str_ipaddr;
	str_delay = dlg.str_delay;
	str_user = dlg.str_user;

	//UpdateData(TRUE);
	//操作写入日志
	CString tmp;
	tmp.Format(TEXT("%s\r\n>>> 切换服务器为 %s | 延迟为 %s | 用户名为 %s"), m_log, str_ipaddr, str_delay, str_user);
	m_log = tmp;
	sync_log();
	log_server_msg();
}

//刷新信息
void CICDlg::OnBnClickedButton1()
{
	FUN_RESULT pres;
	CStringA tmp;
	tmp = str_ipaddr;
	char *ipaddr = tmp.GetBuffer();
	//写日志
	m_log += TEXT("\r\n>>> 正在ping ");
	m_log += str_ipaddr;
	sync_log();
	//ping服务器
	pres = Ping(ipaddr);
	if (pres.state == PSUCC)
	{
		str_ipaddr = CString(inet_ntoa(pres.dwIPaddr));
		str_delay.Format(TEXT("%ld"), pres.dwRoundTripTime);
		m_log += TEXT(" ---> 延迟为 ");
		m_log += str_delay;
	}
	else if (pres.state == PERR_TIMEOUT)
	{
		str_delay = TEXT("超时");
		m_log += TEXT(" ---> 超时 ");
	}
	else
	{
		MessageBox(TEXT("发生未知错误。"));
		exit(0);
	}
	sync_log();
	//拉取信息
	log_server_msg();
}

void CICDlg::log_server_msg()
{
	CSocket aSocket;
	CString msg;
	CString str_send = TEXT("G");
	char szRecValue[1024] = { 0 };
	//禁用页面的四个按钮
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	if (!aSocket.Create())
	{
		msg.Format(TEXT("socket创建失败。"));
		MessageBox(msg);
		//恢复按钮
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		return;
	}
	//连接服务器
	if (aSocket.Connect(str_ipaddr, 9999))
	{
		aSocket.Send(str_send, str_send.GetLength()); //发送内容给服务器
		aSocket.Receive((void*)szRecValue, 1024); //接收服务器发送回来的内容(该方法会阻塞, 在此等待有内容接收到才继续向下执行)
	}
	else
	{
		msg.Format(TEXT("无法进行登陆。"));
		MessageBox(msg);

		//恢复按钮
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		return;
	}
	aSocket.Close();
	m_log += TEXT("\r\n=====================");
	m_log += CString(szRecValue);
	m_log += TEXT("\r\n=====================");
	sync_log();
	//恢复按钮
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
}
