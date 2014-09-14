
// mfc_timerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc_timer.h"
#include "mfc_timerDlg.h"
#include "afxdialogex.h"
//mine
#include "core_code.h"

 core_code ccode;
//mine

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


// Cmfc_timerDlg 对话框




Cmfc_timerDlg::Cmfc_timerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cmfc_timerDlg::IDD, pParent)
	, m_slider1(0)
	, system_volume(0)
	, m_slider2(0)
	, m_slider3(0)
	, current_amplitude(0)
	, critical_amplitude(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);//我自己的 icon
}

void Cmfc_timerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER1, m_slider1);
	DDV_MinMaxInt(pDX, m_slider1, 0, 100);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1_ctr);
	DDX_Text(pDX, IDC_EDIT1, system_volume);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2_ctr);
	DDX_Slider(pDX, IDC_SLIDER2, m_slider2);
	DDX_Control(pDX, IDC_SLIDER3, m_slider3_ctr);
	DDX_Slider(pDX, IDC_SLIDER3, m_slider3);
	DDX_Text(pDX, IDC_EDIT2, current_amplitude);
	DDX_Text(pDX, IDC_EDIT3, critical_amplitude);
	DDX_Control(pDX, IDC_BUTTON1, m_button1_ctr);
	DDX_Control(pDX, IDC_BUTTON2, m_button2_ctr);
}

BEGIN_MESSAGE_MAP(Cmfc_timerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &Cmfc_timerDlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &Cmfc_timerDlg::OnNMCustomdrawSlider3)
//	ON_EN_CHANGE(IDC_EDIT1, &Cmfc_timerDlg::OnEnChangeEdit1)
ON_BN_CLICKED(IDC_BUTTON1, &Cmfc_timerDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &Cmfc_timerDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Cmfc_timerDlg 消息处理程序

BOOL Cmfc_timerDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetTimer(IDTIMER1,1000,NULL);
	m_slider1_ctr.SetRange(1,100,50);
	m_slider2_ctr.SetRange(1,100,50);
	m_slider3_ctr.SetRange(1,100,50);
	m_slider1_ctr.SetTicFreq(10);//
	//mine initialization
   ccode.capture_initialize();
   ccode.volume_control_intialize();
   ccode.capture_start();
   m_slider1=ccode.fVolume;
   m_slider3=85;
   UpdateData(FALSE);
	//
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cmfc_timerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cmfc_timerDlg::OnPaint()
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
		//CDialogEx::OnPaint();
		CPaintDC dc(this); 
		 CRect   rect;  
  GetClientRect(&rect);  
  CDC   dcMem;  
  dcMem.CreateCompatibleDC(&dc);  
  CBitmap   bmpBackground;  
  bmpBackground.LoadBitmap(IDB_BITMAP1);
  BITMAP   bitmap;
  bmpBackground.GetBitmap(&bitmap);
  CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);
  dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cmfc_timerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void  Cmfc_timerDlg::OnTimer(UINT nIDEvent){
	switch (nIDEvent)
    {

	case IDTIMER1:  // 计时器1
		{
			if (timer_enable_flag==false)
				break;

			ccode.wavedata_collect();
			ccode.latest_amplitude=ccode.amplitude_analyse( );
			m_slider2=current_amplitude=ccode.latest_amplitude;
			UpdateData(FALSE);
			if(ccode.latest_amplitude>critical_amplitude && ccode.time_adjusted<2)
			  {system_volume/=2;
			   m_slider1=system_volume;
	           ccode.volume_control((float)system_volume);
	           UpdateData(FALSE);
			   ccode.time_adjusted++;
			  }
			break;
		}
	}

}//timer 响应函数

void Cmfc_timerDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);     
	// TODO: 在此添加控件通知处理程序代码
	system_volume=m_slider1=m_slider1_ctr.GetPos();
	ccode.volume_control((float)system_volume);
	UpdateData(FALSE);          
	ccode.time_adjusted=0;
	*pResult = 0;
}


void Cmfc_timerDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	critical_amplitude=m_slider3=m_slider3_ctr.GetPos();
	UpdateData(FALSE); 
	*pResult = 0;
}




void Cmfc_timerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_button1_ctr.EnableWindow(FALSE);
	m_button2_ctr.EnableWindow(TRUE);
	timer_enable_flag=true;
}


void Cmfc_timerDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_button1_ctr.EnableWindow(TRUE);
	m_button2_ctr.EnableWindow(FALSE);
	timer_enable_flag=false;
}
