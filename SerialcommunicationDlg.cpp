
// SerialcommunicationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Serialcommunication.h"
#include "SerialcommunicationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSerialcommunicationDlg 对话框



CSerialcommunicationDlg::CSerialcommunicationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerialcommunicationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialcommunicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSerialcommunicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_COMMCONTROL, &CSerialcommunicationDlg::OnBnClickedBtnCommcontrol)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSerialcommunicationDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CSerialcommunicationDlg 消息处理程序

BOOL CSerialcommunicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	InitComboBox();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSerialcommunicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSerialcommunicationDlg::OnPaint()
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
HCURSOR CSerialcommunicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSerialcommunicationDlg::InitComboBox()
{
	CComboBox *pComboComm = (CComboBox*)GetDlgItem(IDC_COMBO_COMM);
	ASSERT(pComboComm);

	for (int i = 1; i <= 8; i++)
	{
		CString strComm;
		strComm.Format(_T("COM%d"), i);
		pComboComm->AddString(strComm);
	}
	pComboComm->SetCurSel(0); //选中第0个列表

	//添加波特率
	CComboBox *pComboBaudrate = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
	ASSERT(pComboBaudrate);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("300")), 300);  //字符300与数字300关联
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("600")), 600);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("1200")), 1200);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("2400")), 2400);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("4800")), 4800);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("9600")), 9600);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("19200")), 19200);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("38400")), 38400);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("43000")), 43000);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("56000")), 56000);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("17600")), 17600);
	pComboBaudrate->SetItemData(pComboBaudrate->AddString(_T("115200")), 115200);
	pComboBaudrate->SetCurSel(5); //选中第5个列表

	//添加校验位
	CComboBox *pComboCheckBit = (CComboBox*)GetDlgItem(IDC_COMBO_CHECKBIT);
	ASSERT(pComboCheckBit);
	pComboCheckBit->SetItemData(pComboCheckBit->AddString(_T("无None")), NOPARITY);
	pComboCheckBit->SetItemData(pComboCheckBit->AddString(_T("奇0DD")), ODDPARITY);
	pComboCheckBit->SetItemData(pComboCheckBit->AddString(_T("偶EVEN")), EVENPARITY);
	pComboCheckBit->SetCurSel(0);

	//添加数据位
	CComboBox *pComboDataBit = (CComboBox*)GetDlgItem(IDC_COMBO_DATABIT);
	ASSERT(pComboDataBit);
	pComboDataBit->SetItemData(pComboDataBit->AddString(_T("6")), 6);
	pComboDataBit->SetItemData(pComboDataBit->AddString(_T("7")), 7);
	pComboDataBit->SetItemData(pComboDataBit->AddString(_T("8")), 8);
	pComboDataBit->SetCurSel(2);

	//添加停止位
	CComboBox *pComboStopBit = (CComboBox*)GetDlgItem(IDC_COMBO_STOPBIT);
	ASSERT(pComboStopBit);
	pComboStopBit->SetItemData(pComboStopBit->AddString(_T("1")), ONE5STOPBITS);
	pComboStopBit->SetItemData(pComboStopBit->AddString(_T("2")), TWOSTOPBITS);
	pComboStopBit->SetCurSel(0);
}

void CSerialcommunicationDlg::OnBnClickedBtnCommcontrol()
{
	// TODO:  在此添加控件通知处理程序代码
	static BOOL bIsOpen = FALSE;
	CButton* pBtnCommControl = (CButton*)GetDlgItem(IDC_BTN_COMMCONTROL); //获取按钮的对象
	ASSERT(pBtnCommControl);

	CComboBox *pComboBoxComm = (CComboBox*)GetDlgItem(IDC_COMBO_COMM);
	ASSERT(pComboBoxComm);

	int nSel = pComboBoxComm->GetCurSel();
	CString strComm;
	pComboBoxComm->GetLBText(nSel, strComm);//获取选中的内容

	CComboBox *pComboBoxBaudrate = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
	ASSERT(pComboBoxBaudrate);
	nSel = pComboBoxBaudrate->GetCurSel();
	DWORD dwBaudrate = (DWORD)pComboBoxBaudrate->GetItemData(nSel);

	CComboBox *pComboBoxCheckbit = (CComboBox*)GetDlgItem(IDC_COMBO_CHECKBIT);
	ASSERT(pComboBoxCheckbit);
	nSel = pComboBoxCheckbit->GetCurSel();
	BYTE byParity = (BYTE)pComboBoxCheckbit->GetItemData(nSel);

	CComboBox *pComboBoxDatabit = (CComboBox*)GetDlgItem(IDC_COMBO_DATABIT);
	ASSERT(pComboBoxDatabit);
	nSel = pComboBoxDatabit->GetCurSel();
	BYTE byDataSize = (BYTE)pComboBoxDatabit->GetItemData(nSel);

	CComboBox *pComboBoxStopbit = (CComboBox*)GetDlgItem(IDC_COMBO_STOPBIT);
	ASSERT(pComboBoxStopbit);
	nSel = pComboBoxStopbit->GetCurSel();
	BYTE byStopBits = (BYTE)pComboBoxStopbit->GetItemData(nSel);

	if (!bIsOpen)
	{
		
		bIsOpen = gSerialPort.OpenCommm(strComm); //打开串口
		if (bIsOpen)
		{
			
			BOOL bRet = gSerialPort.SetCommStata(dwBaudrate, byParity, byDataSize, byStopBits); //设置串口属性
			if (!bRet)
			{
				gSerialPort.CloseComm();
				AfxMessageBox(_T("设置串口属性失败！"));
				return;
			}

			bRet = gSerialPort.SetupComm(1024, 1024);  //设置输入输出缓冲区的大小
			if (!bRet)
			{
				gSerialPort.CloseComm();
				AfxMessageBox(_T("设置串口输入输出缓冲区失败！"));
				return;
			}
			bRet = gSerialPort.PurgeComm(PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);  //
			if (!bRet)
			{
				gSerialPort.CloseComm();
				AfxMessageBox(_T("无法清除串口的错误状态！"));
				return;
			}

			bRet=gSerialPort.SetCommMask(EV_RXCHAR);
			if (!bRet)
			{
				gSerialPort.CloseComm();
				AfxMessageBox(_T("设置串口事件出错！"));
				return;
			}
			gSerialPort.StartComm();  //开始串口通信，打开线程，

			bIsOpen = TRUE;
			pBtnCommControl->SetWindowText(_T("关闭串口"));
		}
		else
		{
			pBtnCommControl->SetWindowText(_T("打开串口"));
		}

	}
	else   //原先状态已经打开后，在点击按钮，就关闭串口
	{
		gSerialPort.CloseComm();
		bIsOpen = FALSE;
		pBtnCommControl->SetWindowText(_T("打开串口"));
	}
}


void CSerialcommunicationDlg::OnBnClickedBtnSend()
{
	// TODO:  在此添加控件通知处理程序代码

}
