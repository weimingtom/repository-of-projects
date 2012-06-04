// YonMultiwindowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YonMultiwindow.h"
#include "YonMultiwindowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CYonMultiwindowDlg �Ի���




CYonMultiwindowDlg::CYonMultiwindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYonMultiwindowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYonMultiwindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYonMultiwindowDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CYonMultiwindowDlg ��Ϣ��������

BOOL CYonMultiwindowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������
	CRect renderRect1;
	GetDlgItem(IDC_STATIC1)->GetWindowRect(&renderRect1);
	TRACE("%d,%d,%d,%d\r\n",renderRect1.top,renderRect1.bottom,renderRect1.left,renderRect1.right);
	ScreenToClient(&renderRect1);
	m_dlg1.Create(IDD_DIALOG1,this);
	m_dlg1.MoveWindow(renderRect1);
	m_dlg1.ShowWindow(SW_SHOW);

	CRect renderRect2;
	GetDlgItem(IDC_STATIC2)->GetWindowRect(&renderRect2);
	TRACE("%d,%d,%d,%d\r\n",renderRect2.top,renderRect2.bottom,renderRect2.left,renderRect2.right);
	ScreenToClient(&renderRect2);
	m_dlg2.Create(IDD_DIALOG2,this);
	m_dlg2.MoveWindow(renderRect2);
	m_dlg2.ShowWindow(SW_SHOW);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CYonMultiwindowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CYonMultiwindowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CYonMultiwindowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CYonMultiwindowDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	if(nIDEvent==WM_RENDER_FRAME)
	{
		//���´��������ͻ�����ͬʱ�ػ�ʱ������������
		m_dlg1.InvalidateRect(NULL,FALSE);
		m_dlg2.InvalidateRect(NULL,FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

int CYonMultiwindowDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ�������ר�õĴ�������
	SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n �滻Ϊ����Ҫ��ʱ�����ڣ���λ���롣

	return 0;
}

void CYonMultiwindowDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴�������Ϣ�����������
	KillTimer(WM_RENDER_FRAME);
}