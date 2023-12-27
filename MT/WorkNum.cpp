// WorkNum.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "WorkNum.h"


// CWorkNum dialog

IMPLEMENT_DYNAMIC(CWorkNum, CDialog)

CWorkNum::CWorkNum(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkNum::IDD, pParent)
	, m_StrWorkNum(_T(""))
	, m_strlLine(_T(""))
{

}

CWorkNum::~CWorkNum()
{
}

void CWorkNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StrWorkNum);
	DDX_Text(pDX, IDC_EDIT_LINE, m_strlLine);
	DDX_Control(pDX, IDC_EDIT_LINE, m_ctrlLine);
}


BEGIN_MESSAGE_MAP(CWorkNum, CDialog)
	ON_BN_CLICKED(IDOK, &CWorkNum::OnBnClickedOk)
END_MESSAGE_MAP()


// CWorkNum message handlers

void CWorkNum::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	this->m_cla_iniFile_Ext.StoreStr(_T("Main"), _T("WorkNum"), m_StrWorkNum);
	this->m_cla_iniFile_Ext.StoreStr(_T("Main"), _T("LineNum"), m_strlLine);

	

	OnOK();
}
BOOL CWorkNum::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg-> wParam == VK_RETURN)   // VK_RETURN������ǻس�����  
	{     
		//�ڴ�д������Ҫ�Ĳ�������OnBnClickedButton1();
		return true;//��ʾ���»س��޲���
	} 
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CWorkNum::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//��ǰ����·��
	wchar_t wcrCurr_Exe_Path[MAX_PATH];
	wmemset(wcrCurr_Exe_Path, 0, MAX_PATH);
	::GetCurrentDirectory(MAX_PATH, wcrCurr_Exe_Path);

	int iLong = wcslen(wcrCurr_Exe_Path);

	if(wcrCurr_Exe_Path[iLong - 1] == '\\')
	{
		wcrCurr_Exe_Path[iLong - 1] = '\0';
	}
	this->m_cla_iniFile_Ext.m_csIniFileName.Format(_T("%s\\Multi_Config.ini"), wcrCurr_Exe_Path);

	m_StrWorkNum = this->m_cla_iniFile_Ext.ReadStr(_T("Main"), _T("WorkNum"), _T(""));
	m_strlLine = this->m_cla_iniFile_Ext.ReadStr(_T("Main"), _T("LineNum"), _T(""));

	UpdateData(FALSE);


	CString strExePath = wcrCurr_Exe_Path;
	int len = WideCharToMultiByte(CP_ACP, 0, strExePath, -1, NULL, 0, NULL, NULL);
	char *ptxtTemp = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, strExePath, -1, ptxtTemp, len, NULL, NULL);

	//����0�������ģ�����1��������
	if (IncludeChinese(ptxtTemp) == 1)
	{
		AfxMessageBox(_T("���߰����·���������ģ���ʹ��ȫӢ��·��;\r\nThe tool package is stored in Chinese. Use the English directory\r\n"));
		delete[] ptxtTemp;
		exit(0);
	}
	delete[] ptxtTemp;



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


bool CWorkNum::IncludeChinese(char * str)
{
	char c;
	while (1)
	{
		c = *str++;
		if (c == 0) break;  //������ַ���β��˵�����ַ���û�������ַ�
		if (c & 0x80)        //����ַ���λΪ1����һ�ַ���λҲ��1���������ַ�
			if (*str & 0x80) return 1;
	}
	return 0;
}
