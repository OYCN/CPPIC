// CCONNDLG.cpp: 实现文件
//

#include "pch.h"
#include "IC.h"
#include "CCONNDLG.h"
#include "afxdialogex.h"


// CCONNDLG 对话框

IMPLEMENT_DYNAMIC(CCONNDLG, CDialogEx)

CCONNDLG::CCONNDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN_DLG, pParent)
	, m_ipaddr(202<<24 | 206<<16 | 243<<8 | 34)
	, m_user(_T("TEST_USER"))
	, m_pwd(_T("12345678"))

	, str_ipaddr(_T(""))
	, str_delay(_T(""))
	, str_user(_T(""))
{
	
}

CCONNDLG::~CCONNDLG()
{
}

void CCONNDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_ipaddr);
	DDX_Text(pDX, IDC_USER, m_user);
	DDX_Text(pDX, IDC_PWD, m_pwd);
}


BEGIN_MESSAGE_MAP(CCONNDLG, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DOLOGIN, &CCONNDLG::OnBnClickedDologin)
	ON_BN_CLICKED(IDC_DOEXIT, &CCONNDLG::OnBnClickedDoexit)
END_MESSAGE_MAP()


// CCONNDLG 消息处理程序


void CCONNDLG::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
	CCONNDLG::OnBnClickedDologin();
}


void CCONNDLG::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

void CCONNDLG::OnBnClickedDologin()
{
	// TODO: 在此添加控件通知处理程序代码
	//禁用按钮
	GetDlgItem(IDC_DOLOGIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DOEXIT)->EnableWindow(FALSE);
	//更新文本框数据
	UpdateData(TRUE);
	//判断是否数据位空
	if (m_user.IsEmpty() || m_pwd.IsEmpty())
	{
		MessageBox(TEXT("用户名密码不能为空"));
		GetDlgItem(IDC_DOLOGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DOEXIT)->EnableWindow(TRUE);
		return;
	}
	//判断ip是否特殊，弹出提醒
	if (((m_ipaddr >> 24) & 0x0FF) == 0 || ((m_ipaddr >> 24) & 0x0FF) == 127)
	{
		if (MessageBox(TEXT("注意：当前输入的非外网服务器ip，是否仍然继续？"), TEXT("警告"), MB_YESNO | MB_ICONASTERISK | MB_SYSTEMMODAL) == IDNO)
		{
			//恢复按钮
			GetDlgItem(IDC_DOLOGIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_DOEXIT)->EnableWindow(TRUE);
			return;
		}
	}
	//ping服务器操作，检测是否存活
	FUN_RESULT pres;
	char ipaddr[16] = { '\0' };
	sprintf_s(ipaddr, "%d.%d.%d.%d\0", (int)((m_ipaddr >> 24) & 0x0FF), (int)((m_ipaddr >> 16) & 0x0FF),
												(int)((m_ipaddr >> 8) & 0x0FF), (int)((m_ipaddr) & 0x0FF));
	pres = Ping(ipaddr);
	CSocket aSocket;
	CString msg;
	CString str_send;
	char szRecValue[1024] = { 0 };
	//如果在线
	if (pres.state == PSUCC)
	{
		str_ipaddr = CString(inet_ntoa(pres.dwIPaddr));
		str_delay.Format(TEXT("%ld"), pres.dwRoundTripTime);
		msg.Format(TEXT("Reply from %s: bytes=%ld time=%ldms TTL=%ld\n"), str_ipaddr, pres.size, pres.dwRoundTripTime, pres.iTTL);
		//进行登陆验证
		if (!aSocket.Create())
		{
			msg.Format(TEXT("socket创建失败。"));
			MessageBox(msg);
			//恢复按钮
			GetDlgItem(IDC_DOLOGIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_DOEXIT)->EnableWindow(TRUE);
			return;
		}
		//是否能连接服务器
		if (aSocket.Connect(str_ipaddr, 9999))
		{
			str_send.Format(TEXT("L\n%s\n%s"), m_user, m_pwd);
			aSocket.Send(str_send, str_send.GetLength()*2); //发送内容给服务器
			aSocket.Receive((void*)szRecValue, 1024); //接收服务器发送回来的内容(该方法会阻塞, 在此等待有内容接收到才继续向下执行)
		}
		else
		{
			msg.Format(TEXT("服务端不在线。"));
			MessageBox(msg);

			//恢复按钮
			GetDlgItem(IDC_DOLOGIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_DOEXIT)->EnableWindow(TRUE);
			return;
		}
		aSocket.Close();
		//用户名密码是否错误
		if (szRecValue[0] != 1)
		//if(FALSE)
		{
			MessageBox(TEXT("账号或密码错误！"), TEXT("错误"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
			GetDlgItem(IDC_DOLOGIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_DOEXIT)->EnableWindow(TRUE);
			return;
		}
		str_user.Format(TEXT("%s"), m_user);
		GetDlgItem(IDC_DOLOGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DOEXIT)->EnableWindow(TRUE);
		CDialogEx::OnOK();
		return;
	}
	//服务器不在线
	else if (pres.state == PERR_TIMEOUT)
	{
		msg.Format(TEXT("无法访问服务器，请确认连接是否稳定。"));
	}
	else
		msg.Format(TEXT("发生未知错误。"));
	MessageBox(msg);
	
	//恢复按钮
	GetDlgItem(IDC_DOLOGIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_DOEXIT)->EnableWindow(TRUE);
	return;
}


void CCONNDLG::OnBnClickedDoexit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CCONNDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	AfxSocketInit();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
