
// MTView.cpp : implementation of the CMTView class
//

#include "stdafx.h"
#include "MT.h"

#include "MTDoc.h"
#include "MTView.h"

extern CString g_csMain_Title_Name;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE g_hdlWrite=NULL; //20181123 �����
HANDLE g_hdlRead= NULL; 

// CMTView

IMPLEMENT_DYNCREATE(CMTView, CFormView)

BEGIN_MESSAGE_MAP(CMTView, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(WM_INFO_MSG, OnGetMsg_Show_Info)
	ON_MESSAGE(WM_LOG_IN_MSG, OnGetMsg_Log_In)
	ON_MESSAGE(WM_LOG_OUT_MSG, OnGetMsg_Log_Out)
	ON_MESSAGE(WM_LOG_CHANGE_MSG, OnGetMsg_Log_Change)
	ON_MESSAGE(WM_CHANGE_PASSWORD_MSG, OnGetMsg_Change_Password)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CMTView::OnTcnSelchangeTabMain)
	ON_COMMAND(ID_MENU_RESET_STAT_TSCH_1, &CMTView::OnMenu_Reset_Stat_Tsch_1)
	ON_COMMAND(ID_MENU_RESET_STAT_TSCH_2, &CMTView::OnMenu_Reset_Stat_Tsch_2)
	ON_COMMAND(ID_MENU_RESET_STAT_TSCH_3, &CMTView::OnMenu_Reset_Stat_Tsch_3)
	ON_COMMAND(ID_MENU_RESET_STAT_TSCH_4, &CMTView::OnMenu_Reset_Stat_Tsch_4)
	ON_COMMAND(ID_MENU_RESET_STAT_TSCH_5, &CMTView::OnMenu_Reset_Stat_Tsch_5)
	ON_COMMAND(ID_MENU_RESET_STAT_TSCH_6, &CMTView::OnMenu_Reset_Stat_Tsch_6)
	ON_COMMAND(ID_MENU_RESET_STAT_TSCH_7, &CMTView::OnMenu_Reset_Stat_Tsch_7)
	ON_COMMAND(ID_MENU_RESET_STAT_TSCH_8, &CMTView::OnMenu_Reset_Stat_Tsch_8)
	ON_COMMAND(ID_MENU_RESET_STAT_TSCH_9, &CMTView::OnMenu_Reset_Stat_Tsch_9)
	ON_COMMAND(ID_MENU_RESET_STAT_TSCH_10, &CMTView::OnMenu_Reset_Stat_Tsch_10)

	ON_COMMAND(ID_EXP_CONF, &CMTView::OnMenu_EXP_CONF)
	ON_COMMAND(ID_IMP_CONF, &CMTView::OnMenu_IMP_CONF)
	
	ON_COMMAND(ID_M_WR_FLASH, &CMTView::OnMenu_Write_Flash)
	ON_COMMAND(ID_M_DL_UNLOCK, &CMTView::OnMenu_DL_Unlock)
	ON_COMMAND(ID_CON_M_LOAD_FW, &CMTView::OnMenu_M_LOAD_FW)
	
	ON_COMMAND(ID_CONF_ODM_SPC_NOT_DL, &CMTView::OnMenu_ODM_SPC_NOT_DL)

	ON_COMMAND(ID_UPDATE_DLL,&CMTView::OnUpdate_Dll/*&CMTApp::OnUpdateDll*/)
	ON_COMMAND(ID_UPDATE_TOOL,&CMTView::OnUpdate_Tool)

	ON_WM_PAINT()
	ON_COMMAND(ID_SAHARAONLY, &CMTView::OnSaharaonly)
	ON_UPDATE_COMMAND_UI(ID_SAHARAONLY, &CMTView::OnUpdateSaharaonly)
END_MESSAGE_MAP()

// CMTView construction/destruction

CMTView::CMTView()
	: CFormView(CMTView::IDD)
	, m_clap_ShowInfoDia(NULL)
{
	// TODO: add construction code here

	m_csLocal_Tool_Version.Format(_T(""));
	m_bSHOWMESH = FALSE;
}

CMTView::~CMTView()
{
}

void CMTView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_ctr_tabMain);
}

BOOL CMTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CMTView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//������ҳ����
	this->Create_Dlg_View();


	//�趨����ͷ�ҳ����ģʽ
	this->Ctrl_Dlg_View();


	//��ʼ�������ļ�·��
	this->Init_Config_File_Path();

	if (1== this->m_dlg_Upgrade_FW_Single.m_iQFLASH_CMD)
	{
		CString strData;
		strData.Format(_T("DL INIT...\r\n"));
		WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);
	}

	//��������Ĳ���
	this->Read_Params_First_Basic();

	//qcn�ӿڵ�һ�γ�ʼ��
	::QLIB_QCN_INIT_FIRST();
#ifdef FW_MULTI
	//SQL�ӿڵ�һ�γ�ʼ��
	::SQL_DB_INIT_FIRST();

	//TS_LAN����ڽ������Ϣ����
	::TS_LAN_INIT_FIRST( this->m_dlg_Upgrade_FW_Multi.m_hWnd );

	//FLASH_TEST
	::FLASH_TEST_INIT_FIRST();

	//WRITE_IMEI
	::WRITE_IMEI_INIT_FIRST();
#endif
	//
	this->SetTimer(1, 200, 0);
}

void CMTView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMTView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMTView diagnostics

#ifdef _DEBUG
void CMTView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMTView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMTDoc* CMTView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMTDoc)));
	return (CMTDoc*)m_pDocument;
}
#endif //_DEBUG


// ��ȡ��ǰ·��������ʼ��Config�ļ���·��

void CMTView::Init_Config_File_Path(void)
{
	/*
	ע�⣬��sizeof()������wchar_t����ĳߴ磬����������Ԫ����Ŀ��2������wmemset������飬��Խ��
	*/
	//��ǰ����·��
	wmemset(this->m_wcrCurr_Exe_Path, 0, MAX_PATH);
	::GetCurrentDirectory(MAX_PATH, this->m_wcrCurr_Exe_Path);

	int iLong = wcslen(this->m_wcrCurr_Exe_Path);

	if(this->m_wcrCurr_Exe_Path[iLong - 1] == '\\')
	{
		this->m_wcrCurr_Exe_Path[iLong - 1] = '\0';
	}
	//LOG�ļ�·��
	wmemset(this->m_wcrLog_File_Path, 0, MAX_PATH);
	swprintf_s(this->m_wcrLog_File_Path, _T("%s\\Log_File.txt"), this->m_wcrCurr_Exe_Path);
	//������������
	this->m_dlg_Upgrade_FW_Single.m_cla_iniFile.m_csIniFileName.Format(_T("%s\\MainConfig.ini"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Single.m_cla_iniParams.m_csIniFileName.Format(_T("%s\\Params_Main.ini"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Single.m_csCurr_Exe_Path.Format(_T("%s"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Single.m_cla_Flash_OP_Main.m_csCurr_Exe_Path.Format(_T("%s"), this->m_wcrCurr_Exe_Path);
	//������ء���������
	this->m_dlg_Upgrade_FW_Multi.m_cla_iniFile.m_csIniFileName.Format(_T("%s\\Multi_Config.ini"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams.m_csIniFileName.Format(_T("%s\\Params_Main.ini"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_csCurr_Exe_Path.Format(_T("%s"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_1.m_csCurr_Exe_Path.Format(_T("%s"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_dlg_Config_Multi.m_cla_iniFile.m_csIniFileName.Format(_T("%s"), this->m_dlg_Upgrade_FW_Multi.m_cla_iniFile.m_csIniFileName);
	this->m_dlg_Upgrade_FW_Multi.m_cla_IniStatistics.m_csIniFileName.Format(_T("%s\\Tongji.ini"), this->m_wcrCurr_Exe_Path);

	//Debug_Log�ļ�·��
	this->m_dlg_Upgrade_FW_Single.m_csDebug_Log_File_Path.Format(_T("%s\\Debug_Log_File.log"), this->m_wcrCurr_Exe_Path);
	//�������ò����ļ�
	this->m_cla_IniFile_Basic.m_csIniFileName.Format(_T("%s\\Params\\eco.ini"), this->m_wcrCurr_Exe_Path);
	
	this->m_dlg_Upgrade_FW_Multi.m_csDebug_Log_File_Path_1.Format(_T("%s\\log\\Debug_Log_File_1.log"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_csDebug_Log_File_Path_2.Format(_T("%s\\log\\Debug_Log_File_2.log"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_csDebug_Log_File_Path_3.Format(_T("%s\\log\\Debug_Log_File_3.log"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_csDebug_Log_File_Path_4.Format(_T("%s\\log\\Debug_Log_File_4.log"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_csDebug_Log_File_Path_5.Format(_T("%s\\log\\Debug_Log_File_5.log"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_csDebug_Log_File_Path_6.Format(_T("%s\\log\\Debug_Log_File_6.log"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_csDebug_Log_File_Path_7.Format(_T("%s\\log\\Debug_Log_File_7.log"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_csDebug_Log_File_Path_8.Format(_T("%s\\log\\Debug_Log_File_8.log"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_csDebug_Log_File_Path_9.Format(_T("%s\\log\\Debug_Log_File_9.log"), this->m_wcrCurr_Exe_Path);
	this->m_dlg_Upgrade_FW_Multi.m_csDebug_Log_File_Path_10.Format(_T("%s\\log\\Debug_Log_File_10.log"), this->m_wcrCurr_Exe_Path);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	CTime aTime = CTime::GetCurrentTime();
	CString csData;

	int y = aTime.GetYear();
	int m = aTime.GetMonth();
	int d = aTime.GetDay();
	int h = aTime.GetHour();
	int min = aTime.GetMinute();
	int s = aTime.GetSecond();
	csData.Format(_T("%d_%d_%d_%d_%d_%d"), y, m, d, h, min, s);
	this->m_dlg_Upgrade_FW_Multi.m_csOrg_Log_File_Path.Format(_T("%s\\log\\Org_Log_File_%s.log"), this->m_wcrCurr_Exe_Path,csData);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_2.m_csCurr_Exe_Path = this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_1.m_csCurr_Exe_Path;
	this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_3.m_csCurr_Exe_Path = this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_1.m_csCurr_Exe_Path;
	this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_4.m_csCurr_Exe_Path = this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_1.m_csCurr_Exe_Path;
	this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_5.m_csCurr_Exe_Path = this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_1.m_csCurr_Exe_Path;
	this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_6.m_csCurr_Exe_Path = this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_1.m_csCurr_Exe_Path;
	this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_7.m_csCurr_Exe_Path = this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_1.m_csCurr_Exe_Path;
	this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_8.m_csCurr_Exe_Path = this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_1.m_csCurr_Exe_Path;
	this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_9.m_csCurr_Exe_Path = this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_1.m_csCurr_Exe_Path;
	this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_10.m_csCurr_Exe_Path = this->m_dlg_Upgrade_FW_Multi.m_cla_Flash_OP_1.m_csCurr_Exe_Path;
   
	
	//20180511
	this->m_dlg_Upgrade_FW_Multi.m_cla_ARM_OP_QCM_1.m_cla_iniQD_Params = this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams;
	this->m_dlg_Upgrade_FW_Multi.m_cla_ARM_OP_QCM_2.m_cla_iniQD_Params = this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams;
	this->m_dlg_Upgrade_FW_Multi.m_cla_ARM_OP_QCM_3.m_cla_iniQD_Params = this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams;
	this->m_dlg_Upgrade_FW_Multi.m_cla_ARM_OP_QCM_4.m_cla_iniQD_Params = this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams;
	this->m_dlg_Upgrade_FW_Multi.m_cla_ARM_OP_QCM_5.m_cla_iniQD_Params = this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams;
	this->m_dlg_Upgrade_FW_Multi.m_cla_ARM_OP_QCM_6.m_cla_iniQD_Params = this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams;
	this->m_dlg_Upgrade_FW_Multi.m_cla_ARM_OP_QCM_7.m_cla_iniQD_Params = this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams;
	this->m_dlg_Upgrade_FW_Multi.m_cla_ARM_OP_QCM_8.m_cla_iniQD_Params = this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams;
	this->m_dlg_Upgrade_FW_Multi.m_cla_ARM_OP_QCM_9.m_cla_iniQD_Params = this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams;
	this->m_dlg_Upgrade_FW_Multi.m_cla_ARM_OP_QCM_10.m_cla_iniQD_Params = this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams;
	//��Ӧ������
	this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams.StoreInt(_T("FIREHOSE"), _T("QD_1"), 0);

	this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams.StoreInt(_T("FIREHOSE"), _T("QD_2"), 0);

	this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams.StoreInt(_T("FIREHOSE"), _T("QD_3"), 0);

	this->m_dlg_Upgrade_FW_Multi.m_cla_iniParams.StoreInt(_T("FIREHOSE"), _T("QD_4"), 0);

	
	
	//�������з�ʽ
	this->m_dlg_Upgrade_FW_Single.m_iQFLASH_CMD = this->m_dlg_Upgrade_FW_Single.m_cla_iniFile.ReadInt(_T("Main"), _T("QFLASH_CMD"), 0);
	if (1 == this->m_dlg_Upgrade_FW_Single.m_iQFLASH_CMD)
	{
		TCHAR Buffer[512]; //������
		memset(Buffer, 0, sizeof(TCHAR)*512);
		DWORD dwCount = 0;//��������

		CString strInfo;

		::AllocConsole();
		g_hdlWrite = GetStdHandle(STD_OUTPUT_HANDLE); //����Ҳ����ʹ��STD_ERROR_HANDLE
		g_hdlRead = GetStdHandle(STD_INPUT_HANDLE);

		CString strData;
		strData.Format(_T("Input Port_Baudrate_FW_Path:"));

		WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);
	
		ReadConsole(g_hdlRead, Buffer, 512, &dwCount, NULL);

		if (dwCount>0)
		{
			strInfo.Format(_T("%s"),Buffer);

			//------------------------------------------------------
			CString csrList[4];
			int num =0;
			for (int i =0;i<4;i++)
			{
				csrList[i].Format(_T(""));
			}
			
			this->m_dlg_Upgrade_FW_Single.m_cla_Flash_OP_Main.m_cla_GFC.WCSTOK_STRING_T_C(strInfo,_T(" "),csrList,&num);

			if (num>=4)
			{
				//QFlash_V4.11.exe 12 921600 D:\4_Firmware\MC20CBR01A04_TTS\QUECTEL03D_M2M_11C_BB.cfg
				this->m_dlg_Upgrade_FW_Single.iCmdComPort = _wtoi(csrList[1]);
				this->m_dlg_Upgrade_FW_Single.iCmdBd = _wtoi(csrList[2]);
				csrList[3].TrimLeft();
				csrList[3].TrimRight();
				this->m_dlg_Upgrade_FW_Single.strCmdPath.Format(_T("%s"),csrList[3]);
				return;
			}
			else if (num==1) //����Ŀ����Ƕ˿ں�
			{
				this->m_dlg_Upgrade_FW_Single.iCmdComPort = _wtoi(csrList[0]);
			}
			else
			{
				//�����ʽ����
				AfxMessageBox(_T("Input String Format Not Support"));
				return;

			}

			//------------------------------------------------------



			
		}
		else
		{
			WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);

			ReadConsole(g_hdlRead, Buffer, 512, &dwCount, NULL);

			if (dwCount>0)
			{
				strInfo.Format(_T("%s"),Buffer);

				this->m_dlg_Upgrade_FW_Single.iCmdComPort = _wtoi(strInfo);
			}
			else
			{
				WriteConsole(g_hdlWrite, _T("Port input error"), strData.GetLength(), NULL, NULL);
				return;
			}
		}
	
		//Baudrate**********************************************************

		strData.Format(_T("Input Baudrate:"));

		WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);

		ReadConsole(g_hdlRead, Buffer, 512, &dwCount, NULL);

		if (dwCount>0)
		{
			strInfo.Format(_T("%s"),Buffer);

			this->m_dlg_Upgrade_FW_Single.iCmdBd = _wtoi(strInfo);
		}
		else
		{
			WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);

			ReadConsole(g_hdlRead, Buffer, 512, &dwCount, NULL);

			if (dwCount>0)
			{
				strInfo.Format(_T("%s"),Buffer);

				this->m_dlg_Upgrade_FW_Single.iCmdBd = _wtoi(strInfo);
			}
			else
			{
				WriteConsole(g_hdlWrite, _T("Baudrate input error"), strData.GetLength(), NULL, NULL);
				return;
			}
		}

		//Firmware Path**********************************************************

		strData.Format(_T("Input Firmware Path:"));

		WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);
		
		ReadConsole(g_hdlRead, Buffer, 512, &dwCount, NULL);
		if (dwCount>0)
		{
			strInfo.Format(_T("%s"),Buffer);
			strInfo.TrimLeft();
			strInfo.TrimRight();
			this->m_dlg_Upgrade_FW_Single.strCmdPath.Format(_T("%s"),strInfo);
		}
		else
		{
			WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);

			ReadConsole(g_hdlRead, Buffer, 512, &dwCount, NULL);

			if (dwCount>0)
			{
				strInfo.Format(_T("%s"),Buffer);
				strInfo.TrimLeft();
				strInfo.TrimRight();
				this->m_dlg_Upgrade_FW_Single.strCmdPath.Format(_T("%s"),strInfo);
			}
			else
			{
				WriteConsole(g_hdlWrite, _T("Firmware Path input error"), strData.GetLength(), NULL, NULL);
				return;
			}
		}




		//**********************************************************
#if 0
		CString sCmdline = AfxGetApp()->m_lpCmdLine;
	
		//iCmdComPort
		//��ȡ�˿�
		if(this->m_dlg_Upgrade_FW_Single.m_cla_GFC.WCSTOK_STRING_C( sCmdline, _T(" "), 1, &strInfo ) == false)
		{
			AfxMessageBox(_T("Port Get Fail"));
			return;
		}
		else
		{
			this->m_dlg_Upgrade_FW_Single.iCmdComPort = _wtoi(strInfo);
		}
		//��ȡ������
		if(this->m_dlg_Upgrade_FW_Single.m_cla_GFC.WCSTOK_STRING_C( sCmdline, _T(" "), 2, &strInfo ) == false)
		{
			AfxMessageBox(_T("BaudRate Get Fail"));
			return;
		}
		else
		{
			this->m_dlg_Upgrade_FW_Single.iCmdBd = _wtoi(strInfo);
		}	

		//��ȡFirmware·��
		if(this->m_dlg_Upgrade_FW_Single.m_cla_GFC.WCSTOK_STRING_C( sCmdline, _T(" "), 3, &strInfo ) == false)
		{
			AfxMessageBox(_T("Frimware Path Get Fail"));
			return;
		}
		else
		{
			this->m_dlg_Upgrade_FW_Single.strCmdPath = strInfo;
		}

		strInfo.Format(_T("%d,%d,%s"),this->m_dlg_Upgrade_FW_Single.iCmdComPort,this->m_dlg_Upgrade_FW_Single.iCmdBd,this->m_dlg_Upgrade_FW_Single.strCmdPath);
		AfxMessageBox(strInfo);
#endif
	}

}

void CMTView::OnTimer(UINT_PTR nIDEvent)
{
	this->KillTimer((int)nIDEvent);

	//��ʼ���桢������״̬
#ifdef FW_MULTI
	this->m_dlg_Upgrade_FW_Multi.Load_View_Static_Value();
	this->m_dlg_Upgrade_FW_Multi.INIT();
#else//#elif ��������
	this->m_dlg_Upgrade_FW_Single.Load_View_Static_Value();
	this->m_dlg_Upgrade_FW_Single.INIT();

	if(1== this->m_dlg_Upgrade_FW_Single.m_iQFLASH_CMD )
	{
		Sleep(4000);
		if (1== this->m_dlg_Upgrade_FW_Single.m_iQFLASH_CMD)
		{
			CString strData;
			strData.Format(_T("DL INIT Pass\r\n"));
			WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);
		}
		this->m_dlg_Upgrade_FW_Single.ShowWindow(SW_MINIMIZE);
		this->m_dlg_Upgrade_FW_Single.OnBnClickedButtonStart();
		
	}
#endif
	
	//
	this->UpdateData(FALSE);

	CFormView::OnTimer(nIDEvent);
}

void CMTView::Show_MsgBox(const wchar_t * wcpTitle, const wchar_t * wcpMsg, int iFlag, bool bClose, int iTime)
{
	CMsgBox *clap_MsgBox;

	clap_MsgBox = new CMsgBox();
	clap_MsgBox->Create(IDD_DIALOG_MSGBOX);
	clap_MsgBox->ShowWindow(SW_SHOW);

	if(iFlag == 0)
	{
		clap_MsgBox->m_ctr_staPass.ShowWindow(SW_SHOW);
		clap_MsgBox->m_ctr_staFail.ShowWindow(SW_HIDE);
	}
	else if(iFlag == 1)
	{
		clap_MsgBox->m_ctr_staPass.ShowWindow(SW_HIDE);
		clap_MsgBox->m_ctr_staFail.ShowWindow(SW_SHOW);
	}
	else
	{
		clap_MsgBox->m_ctr_staPass.ShowWindow(SW_HIDE);
		clap_MsgBox->m_ctr_staFail.ShowWindow(SW_HIDE);
	}

	clap_MsgBox->SetWindowTextW(wcpTitle);
	clap_MsgBox->m_ctr_edtShow.SetWindowTextW(wcpMsg);

	RECT reWindow;
	GetWindowRect(&reWindow);

	const CWnd * pWndInsertAfter;
	pWndInsertAfter = &wndNoTopMost;
	clap_MsgBox->SetWindowPos(pWndInsertAfter, (reWindow.left + 90), (reWindow.top + 200), 0, 0, SWP_NOSIZE);

	if(bClose)
	{
		clap_MsgBox->SetTimer(2, iTime, 0);
	}
}

LRESULT CMTView::OnGetMsg_Show_Info(WPARAM wParam, LPARAM lParam)
{
	this->m_clap_ShowInfoDia->Show_RichEdit_Info((wchar_t *)wParam, (int)lParam);

	return 0;
}

void CMTView::Show_Info(const wchar_t * wcpMsg)
{
	::SendMessage(this->m_hWnd, WM_INFO_MSG, (WPARAM)wcpMsg, (LPARAM)true); 
}

HBRUSH CMTView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CMTView::OnDestroy()
{
	CFormView::OnDestroy();
#ifdef FW_MULTI
	//�ر�ȫ��SQL�����ӿڣ��ͷ���Դ
	::SQL_DB_FREE_ALL();
#endif


	if (g_hdlRead != NULL)
	{
		::CloseHandle(g_hdlRead);
	}
	if (g_hdlWrite != NULL)
	{
		::CloseHandle(g_hdlWrite);
	}	

}

void CMTView::Create_Dlg_View(void)
{
	//��������ҳ��ҳ���˴���������ҳ��OnInitDialog()����
#ifdef FW_MULTI
	this->m_dlg_Upgrade_FW_Multi.Create(IDD_DIALOG_FW_MULTI, GetDlgItem(IDC_TAB_MAIN));

	this->m_logviewDlg.Create(IDD_DIA_LOG, GetDlgItem(IDC_TAB_MAIN));

	this->m_dlg_Upgrade_FW_Multi.m_hWnd_log_View = this->m_logviewDlg.m_hWnd;

#else//#elif ��������
	this->m_dlg_Upgrade_FW_Single.Create(IDD_DIALOG_EXAMPLE, GetDlgItem(IDC_TAB_MAIN));
#endif

	//����ҳ���λ����m_tab�ؼ���Χ��
	this->m_ctr_tabMain.GetClientRect(&this->m_rect_tab);
	this->m_ctr_tabMain.GetClientRect(&this->m_rect_main);
	//��ҳ����ĳߴ�С��Main��������ɳߴ�
	this->m_rect_tab.top += 20;

}

void CMTView::Ctrl_Dlg_View()
{
	//iMode�趨��ͬȨ�ޡ�״̬�£��������������ڵľ��塰���ܷ�ҳ��
	//ע�⣺�л���ҳ����ʱ���롰����ʧ���������ʾ�����£���һ�·�ҳ��Ӧ�ġ��Ի��򡱣���ˢ��Ч���������ҳ�Ի����޷�������ʾ

	//ɾ��ȫ����ҳ
	while(this->m_ctr_tabMain.DeleteItem(0) != 0)
	{
	}

#ifdef FW_MULTI
	this->m_dlg_Upgrade_FW_Multi.MoveWindow(&this->m_rect_tab);//��ʼ���棬����ߴ�Ϊtab�ߴ�
	this->m_dlg_Upgrade_FW_Multi.ShowWindow(SW_SHOW);

	this->m_logviewDlg.MoveWindow(&this->m_rect_tab);
	this->m_logviewDlg.ShowWindow(SW_HIDE);	

	//��ʼ��m_tab�ؼ�����ҳ��
	this->m_ctr_tabMain.InsertItem(0, _T("Multi-Port"));

	TCITEM tcItem2;
	tcItem2.mask = TCIF_TEXT;
	tcItem2.pszText = _T("Log-View");
	this->m_ctr_tabMain.InsertItem(1, &tcItem2);

#else//#elif ��������	
	if (1 == this->m_dlg_Upgrade_FW_Single.m_iQFLASH_CMD)
	{
		this->m_dlg_Upgrade_FW_Single.MoveWindow(&this->m_rect_tab);//��ʼ���棬����ߴ�Ϊtab�ߴ�
		this->m_dlg_Upgrade_FW_Single.ShowWindow(SW_MINIMIZE);
	}
	else
	{
		this->m_dlg_Upgrade_FW_Single.MoveWindow(&this->m_rect_tab);//��ʼ���棬����ߴ�Ϊtab�ߴ�
		this->m_dlg_Upgrade_FW_Single.ShowWindow(SW_SHOW);
	}
	//��ʼ��m_tab�ؼ�����ҳ��
	this->m_ctr_tabMain.InsertItem(0, _T("Upgrade FW"));
#endif

	//Tab�ؼ��趨����һҳ��Ϊ��ǰѡ��ҳ
	this->m_ctr_tabMain.SetCurSel(0);
}

LRESULT CMTView::OnGetMsg_Log_In(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

LRESULT CMTView::OnGetMsg_Log_Out(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

LRESULT CMTView::OnGetMsg_Log_Change(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

void CMTView::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	//����ķ�ҳIndex��������˽����ҳ������
	int iPage = this->m_ctr_tabMain.GetCurSel();
	switch(iPage)
	{
		case 0:
			this->m_dlg_Upgrade_FW_Multi.ShowWindow(SW_SHOW);
			this->m_logviewDlg.ShowWindow(SW_HIDE);	
			break;
		case 1:
			this->m_dlg_Upgrade_FW_Multi.ShowWindow(SW_HIDE);
			this->m_logviewDlg.ShowWindow(SW_SHOW);	

			break;
		default: break;
	}



	*pResult = 0;
}

LRESULT CMTView::OnGetMsg_Change_Password(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

void CMTView::Read_Params_First_Basic(void)
{
	::g_csMain_Title_Name = this->m_cla_IniFile_Basic.ReadStr(_T("MAIN_WINDOWS"), _T("NAME"), _T(""));
}

void CMTView::OnMenu_Reset_Stat_Tsch_1()
{
	int iTS_CH = 1;
	int iTS_Numb;
	//
	if(this->m_dlg_Upgrade_FW_Multi.Get_TS_Number( iTS_CH, &iTS_Numb ) == true)
	{
		//
		this->m_dlg_Upgrade_FW_Multi.Reset_Statistics_Data( iTS_CH, iTS_Numb );
	}
}

void CMTView::OnMenu_Reset_Stat_Tsch_2()
{
	int iTS_CH = 2;
	int iTS_Numb;
	//
	if(this->m_dlg_Upgrade_FW_Multi.Get_TS_Number( iTS_CH, &iTS_Numb ) == true)
	{
		//
		this->m_dlg_Upgrade_FW_Multi.Reset_Statistics_Data( iTS_CH, iTS_Numb );
	}
}

void CMTView::OnMenu_Reset_Stat_Tsch_3()
{
	int iTS_CH = 3;
	int iTS_Numb;
	//
	if(this->m_dlg_Upgrade_FW_Multi.Get_TS_Number( iTS_CH, &iTS_Numb ) == true)
	{
		//
		this->m_dlg_Upgrade_FW_Multi.Reset_Statistics_Data( iTS_CH, iTS_Numb );
	}
}

void CMTView::OnMenu_Reset_Stat_Tsch_4()
{
	int iTS_CH = 4;
	int iTS_Numb;
	//
	if(this->m_dlg_Upgrade_FW_Multi.Get_TS_Number( iTS_CH, &iTS_Numb ) == true)
	{
		//
		this->m_dlg_Upgrade_FW_Multi.Reset_Statistics_Data( iTS_CH, iTS_Numb );
	}
}

void CMTView::OnMenu_Reset_Stat_Tsch_5()
{
	int iTS_CH = 5;
	int iTS_Numb;
	//
	if(this->m_dlg_Upgrade_FW_Multi.Get_TS_Number( iTS_CH, &iTS_Numb ) == true)
	{
		//
		this->m_dlg_Upgrade_FW_Multi.Reset_Statistics_Data( iTS_CH, iTS_Numb );
	}
}

void CMTView::OnMenu_Reset_Stat_Tsch_6()
{
	int iTS_CH = 6;
	int iTS_Numb;
	//
	if(this->m_dlg_Upgrade_FW_Multi.Get_TS_Number( iTS_CH, &iTS_Numb ) == true)
	{
		//
		this->m_dlg_Upgrade_FW_Multi.Reset_Statistics_Data( iTS_CH, iTS_Numb );
	}
}

void CMTView::OnMenu_Reset_Stat_Tsch_7()
{
	int iTS_CH = 7;
	int iTS_Numb;
	//
	if(this->m_dlg_Upgrade_FW_Multi.Get_TS_Number( iTS_CH, &iTS_Numb ) == true)
	{
		//
		this->m_dlg_Upgrade_FW_Multi.Reset_Statistics_Data( iTS_CH, iTS_Numb );
	}
}

void CMTView::OnMenu_Reset_Stat_Tsch_8()
{
	int iTS_CH = 8;
	int iTS_Numb;
	//
	if(this->m_dlg_Upgrade_FW_Multi.Get_TS_Number( iTS_CH, &iTS_Numb ) == true)
	{
		//
		this->m_dlg_Upgrade_FW_Multi.Reset_Statistics_Data( iTS_CH, iTS_Numb );
	}
}

void CMTView::OnMenu_Reset_Stat_Tsch_9()
{
	int iTS_CH = 9;
	int iTS_Numb;
	//
	if(this->m_dlg_Upgrade_FW_Multi.Get_TS_Number( iTS_CH, &iTS_Numb ) == true)
	{
		//
		this->m_dlg_Upgrade_FW_Multi.Reset_Statistics_Data( iTS_CH, iTS_Numb );
	}
}

void CMTView::OnMenu_Reset_Stat_Tsch_10()
{
	int iTS_CH = 10;
	int iTS_Numb;
	//
	if(this->m_dlg_Upgrade_FW_Multi.Get_TS_Number( iTS_CH, &iTS_Numb ) == true)
	{
		//
		this->m_dlg_Upgrade_FW_Multi.Reset_Statistics_Data( iTS_CH, iTS_Numb );
	}
}

void CMTView::OnMenu_EXP_CONF()
{

	this->m_dlg_Upgrade_FW_Multi.OnEXP_CONF_File();


}
void CMTView::OnMenu_IMP_CONF()
{
	this->m_dlg_Upgrade_FW_Multi.OnIMP_CONF_File();
}

void CMTView::OnMenu_Write_Flash()//MTK д�Ϸ�ID��Ȩ
{
	this->m_dlg_Upgrade_FW_Multi.OnWrite_Flash();
}
void CMTView::OnMenu_DL_Unlock()//����������Ȩ
{
	this->m_dlg_Upgrade_FW_Multi.OnDL_Unlock();
}
void CMTView::OnMenu_M_LOAD_FW()//�ֶ����ع̼���Ȩ
{
	this->m_dlg_Upgrade_FW_Multi.OnM_LOAD_FW();
}

void CMTView::OnMenu_ODM_SPC_NOT_DL()//����QDC506���ܲ��ֹ̼���������Ȩ
{
	this->m_dlg_Upgrade_FW_Multi.OnMenu_ODM_SPC_NOT_DL();
}



void CMTView::OnUpdate_Dll(void)
{
	//���FTP������
// 	if (false == this->m_dlg_Config.m_bNeedUpload_FTP_Log)
// 	{
// 		this->Show_RichEdit_Info_PF(_T("ftp��������ʼ�����������޷�����DLL����"), SHOW_FAIL);
// 	}
#ifdef FW_MULTI
	//��ʾ

	if (IDYES != MessageBox(_T("Need Upload DLL?"), _T("NOTE"), MB_ICONASTERISK | MB_YESNO))
	{
		return;
	}
	//Read_Factory_Info_Name(this->m_csFactory_Info_Name);

	//����DLL_Update.ini����
	m_UpdateConfig.InitConfigPath(_T("\\Update\\DLL_Update.ini"));
	m_UpdateConfig.m_csServerName_Fac.Format(_T("%s"), this->m_dlg_Upgrade_FW_Multi.m_dlg_Config_Multi.m_csServerName_Main);//����������IP
	m_UpdateConfig.m_csFactory_Info_Name.Format(_T("%s"), this->m_dlg_Upgrade_FW_Multi.m_csFactory_Info_Name);//�������ƣ�HF/XT/JSD/WCL)
	m_UpdateConfig.m_bOCP = true;
	m_UpdateConfig.m_bWrite_Check = false;

	m_UpdateConfig.m_csSave_Path.Format(_T("%s\\"), this->m_dlg_Upgrade_FW_Single.m_csCurr_Exe_Path);
	m_UpdateConfig.SaveConfig();

	//����DLL_Update.exe
	
	m_csUpdateDLL_EXE_File_Path.Format(_T("%s\\Update\\DLL_Update.exe"), this->m_dlg_Upgrade_FW_Single.m_csCurr_Exe_Path);
	ShellExecute(NULL, _T("open"), m_csUpdateDLL_EXE_File_Path, NULL, NULL, SW_SHOWNORMAL);

	//killself
	Sleep(500);
	this->m_dlg_Upgrade_FW_Multi.Quit();
	//::PostQuitMessage(0);
	GetParent()->SendMessage(WM_CLOSE);
	
#endif


}

void CMTView::OnUpdate_Tool(void)
{
     this->Auto_Update_Tool();
}

void CMTView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
}
bool CMTView::Auto_Update_Tool () 
{ 
	CString csTool_Path_In_FTP;  
	CString csTool_Type;  
	CString csTemp; 
	int iFindIndex;
	bool b_Need_Update = false; 


	//FW_Download_And_Format_V5.0.30
	//m_csLocal_Tool_Version = V5.0.30

	m_csLocal_Tool_Version = ::g_csMain_Title_Name;

	int pos = m_csLocal_Tool_Version.Find(_T('V'));

	m_csLocal_Tool_Version = m_csLocal_Tool_Version.Right(m_csLocal_Tool_Version.GetLength()-pos);

	try  
	{ 
		//1�����ݲ�ͬ����ȷ����������FTP·��   
		//���ݹ���ȷ��FTP·��   

		csTool_Path_In_FTP.Format ( _T ( "%s/Tools/%s/" ) , this->m_dlg_Upgrade_FW_Multi.m_strFTPdir,_T("MCU_FW_DL_ODM_COB") );   


// 		if ( this->m_dlg_Upgrade_FW_Multi.m_csFactory_Info_Name == _T ( "HF" ) )   
// 		{    
// 			//�Ϸ����������    
// 			csTool_Path_In_FTP.Format ( _T ( "//quectel/DCC/quectel-hf/Tools/%s/" ) , _T("FW_Download_And_Format") );   
// 		}
// 		else if ( this->m_dlg_Upgrade_FW_Multi.m_csFactory_Info_Name == _T ( "XT" ) )
// 		{    //��̫����    
// 			csTool_Path_In_FTP.Format ( _T ( "//quectel/DCC/quectel-sintave/Tools/%s/" ) , _T("FW_Download_And_Format"));  
// 		}   
// 		else if ( this->m_dlg_Upgrade_FW_Multi.m_csFactory_Info_Name == _T ( "JSD" ) )   
// 		{ 
// 			//�����﹤��    
// 			csTool_Path_In_FTP.Format ( _T ( "//quectel/DCC/quectel-qisda/Tools/%s/" ) , _T("FW_Download_And_Format") );
// 		}   
// 		else if ( this->m_dlg_Upgrade_FW_Multi.m_csFactory_Info_Name == _T ( "WCL" ) )  
// 		{    
// 			//ΰ��������    
// 			csTool_Path_In_FTP.Format ( _T ( "//quectel/DCC/quectel-flex/Tools/%s/" ) , _T("FW_Download_And_Format") ); 
// 		}   
// 		else if ( this->m_dlg_Upgrade_FW_Multi.m_csFactory_Info_Name == _T ( "KD" ) )  
// 		{    
// 			//���﹤��    
// 			csTool_Path_In_FTP.Format ( _T ( "//quectel/DCC/quectel-mitac/Tools/%s/" ) , _T("FW_Download_And_Format") ); 
// 		}  
// 		else if ( this->m_dlg_Upgrade_FW_Multi.m_csFactory_Info_Name == _T ( "TT" ) )  
// 		{    
// 			//��ͨ����    
// 			csTool_Path_In_FTP.Format ( _T ( "//quectel/DCC/quectel-tdg/Tools/%s/" ) , _T("FW_Download_And_Format") );  
// 		}  
// 		else   
// 		{    
// 			//δ֪����   
// 			throw( 0 ); 
// 		} 


		//2������FTP����Ŀ¼�汾   
		//2.1 ����FTP�˺�����   
		this->m_cls_Ftp_Interface.m_FtpInfo.sServerName = this->m_dlg_Upgrade_FW_Multi.m_dlg_Config_Multi.m_csFTP_LOG_Server;
		//�����߶�̬��ѯ����FTP Server ��ֵ��FTP����   
		this->m_cls_Ftp_Interface.m_FtpInfo.sPassword = this->m_dlg_Upgrade_FW_Multi.m_dlg_Config_Multi.m_csFTP_LOG_Password;
		//�����߶�̬��ѯ����FTP Password ��ֵ��FTP����   
		this->m_cls_Ftp_Interface.m_FtpInfo.sUserName = this->m_dlg_Upgrade_FW_Multi.m_dlg_Config_Multi.m_csFTP_LOG_User_Name;
		//�����߶�̬��ѯ����FTP User_Name ��ֵ��FTP����   
		this->m_cls_Ftp_Interface.m_FtpInfo.iPort = this->m_dlg_Upgrade_FW_Multi.m_dlg_Config_Multi.m_iFTP_LOG_Port;
		//�����߶�̬�� ѯ����FTP Port ��ֵ��FTP���� 



		//2.2 ����FTP   
		if ( false == this->m_cls_Ftp_Interface.Connect_FTP_Server () ) 
		{    
			throw( 0 ); 
		} 




		//2.3 ���ù�������FTP·��   
		if ( false == this->m_cls_Ftp_Interface.Set_FTP_Path ( csTool_Path_In_FTP ) ) 
		{    
			throw( 0 );  
		} 




		//2.4 ��ȡָ��Ŀ¼���ļ�list   
		if ( false == this->m_cls_Ftp_Interface.Get_Files_From_Current_Path () ) 
		{    

			throw( 0 ); 
		} 



		//2.5 ���������Ƿ��и��ߵİ汾����  
		if ( this->m_cls_Ftp_Interface.m_sFileList[0].GetSize () > 0 )  
		{    
			for ( int i = 0; i < this->m_cls_Ftp_Interface.m_sFileList[0].GetSize (); i++ )
			{     
				csTemp = this->m_cls_Ftp_Interface.m_sFileList[0][i]; 

				csTemp.MakeUpper ();    
				iFindIndex = csTemp.Find ( _T ( "_V" ) ); 

				if ( iFindIndex == -1 )     
				{      
					//���Ǻ��ʵ��ļ�      
					continue;     
				}     
				else     
				{ 
					//ASR_RF_CAL_V1.10.1.ZIP      
					csTemp = csTemp.Right ( csTemp.GetLength () - iFindIndex - 1 );  
					csTemp.Replace ( _T ( ".ZIP" ) , _T ( "" ) ); 
					//V1.10.1 

					//�Ƚϰ汾           
					if ( true == Compare_Tool_Local_and_FTP_Version ( this->m_csLocal_Tool_Version , csTemp ) ) 
					{       
						//���ڵ�ǰ�汾      
						b_Need_Update = true;  
						break;    
					}      
					else   
					{      
						//С�ڵ�ǰ�汾  
						continue;    
					}   
				} 
			}   
		} 



		//3.�汾������ϣ��Ͽ�FTP   
		this->m_cls_Ftp_Interface.DisConnect_FTP_Server (); 

		if ( true == b_Need_Update ) 
		{   

			//4.��Ҫ���� 
			this->DoExcute_Update_Tool (); 
		}   
		else  
		{   
			AfxMessageBox(_T("No new version found,so no update is required"));
			//����Ҫ����   
			throw( 0 ); 
		} 

	}  
	catch ( ... ) 
	{   

		//�Ͽ�FTP 
		this->m_cls_Ftp_Interface.DisConnect_FTP_Server ();
		return false;  
	} 


	return true; 
} 

void CMTView::DoExcute_Update_Tool ()
{  
	CString csUpdateDLL_EXE_File_Path; 


	//��ʾ�û��Ƿ���Ҫ���� 
	if ( IDYES != MessageBox ( _T ( "There is an new version, do you want to updated?" ) , _T ( "Tip" ) , MB_ICONASTERISK | MB_YESNO ) ) 
	{   
		return; 
	} 

	//����Tool_Update.ini 

	//�ļ�·������  
	m_cls_Tool_Update_Config.InitConfigPath ( _T ( "\\Update\\Tool_Update.ini" ) ); 

	//����������IP ����������IP�� 
	m_cls_Tool_Update_Config.m_csServerName_Fac.Format ( _T ( "%s" ) , this->m_dlg_Upgrade_FW_Multi.m_dlg_Config_Multi.m_csSQL_Server_Name); 

	//�������ƣ�HF/XT/JSD/WCL/KD) 
	m_cls_Tool_Update_Config.m_csFactory_Info_Name.Format ( _T ( "%s" ) , this->m_dlg_Upgrade_FW_Multi.m_csFactory_Info_Name ); 

	//�ϰ汾exe����·�� /* D:\Run\FW_Download_And_Format_V5.0.30 */  
	m_cls_Tool_Update_Config.m_csExe_Path = this->m_dlg_Upgrade_FW_Multi.m_csCurr_Exe_Path;

	//��������(FW_Download_And_Format_V5.0.30) 
	m_cls_Tool_Update_Config.m_csExe_Name.Format ( _T ( "%s" ) , ::g_csMain_Title_Name ); 

	//��Ҫ���Ϲ���copy���¹��ߵ��ļ�  
	m_cls_Tool_Update_Config.m_csSetting = _T ( "Multi_Config.ini" ); 

	// �������� FW_Download_And_Format
	m_cls_Tool_Update_Config.m_csTool_Name.Format ( _T ( "%s" ) , _T("MCU_FW_DL_ODM_COB") ); 

	//���� 
	m_cls_Tool_Update_Config.SaveConfig (); 

	Sleep ( 1000 ); 

	//����Tool_Update.exe  
	csUpdateDLL_EXE_File_Path.Format ( _T ( "%s\\Update\\Tool_Update.exe" ) , this->m_dlg_Upgrade_FW_Multi.m_csCurr_Exe_Path ); 
	ShellExecute ( NULL , _T ( "open" ) , csUpdateDLL_EXE_File_Path , NULL , NULL , SW_SHOWNORMAL ); 


	//killself  Sleep ( 500 ); 
	this->GetParent ()->SendMessage ( WM_CLOSE ); 

	Sleep ( 1000 ); 
} 

bool CMTView::Compare_Tool_Local_and_FTP_Version ( const CString csLocal_Version , const CString csFTP_Version )
{  
	//�Ƚϱ��ذ汾��FTP�汾�Ŵ�С�� 
	//FTP���ڱ��ط���true����Ҫ������ 
	//FTPС�ڵ��ڱ��ط���false,����Ҫ���� 
	//V1.19.1  �� 
	//V1.9.1   С  
	CString csLocal_Tool_Version , csFTP_Tool_Version; 
	CString csr_FTP_Code_List[5]; 
	CString csr_Local_Code_List[5]; 
	int i_FTP_Quan , i_Local_Quan; 


	csLocal_Tool_Version = csLocal_Version; 
	csFTP_Tool_Version = csFTP_Version; 


	//V1.15.1  
	//ͳһ��д�����V  
	csFTP_Tool_Version.MakeUpper ();
	csFTP_Tool_Version.Replace ( _T ( "V" ) , _T ( "" ) ); 
	csLocal_Tool_Version.MakeUpper (); 
	csLocal_Tool_Version.Replace ( _T ( "V" ) , _T ( "" ) ); 

	//����.���ָ�  
	csr_FTP_Code_List[0].Format ( _T ( "" ) ); 
	csr_FTP_Code_List[1].Format ( _T ( "" ) );  
	csr_FTP_Code_List[2].Format ( _T ( "" ) ); 
	csr_FTP_Code_List[3].Format ( _T ( "" ) ); 
	csr_FTP_Code_List[4].Format ( _T ( "" ) ); 
	csr_Local_Code_List[0].Format ( _T ( "" ) ); 
	csr_Local_Code_List[1].Format ( _T ( "" ) );  
	csr_Local_Code_List[2].Format ( _T ( "" ) ); 
	csr_Local_Code_List[3].Format ( _T ( "" ) );  
	csr_Local_Code_List[4].Format ( _T ( "" ) ); 

	this->m_dlg_Upgrade_FW_Multi.m_cla_GFC.WCSTOK_STRING_T_C ( csFTP_Tool_Version , _T ( "." ) , csr_FTP_Code_List , &i_FTP_Quan ); 
	this->m_dlg_Upgrade_FW_Multi.m_cla_GFC.WCSTOK_STRING_T_C ( csLocal_Tool_Version , _T ( "." ) , csr_Local_Code_List , &i_Local_Quan ); 

	//�ȶ������Ƿ�Ϸ� 
	if ( ( 3 == i_FTP_Quan ) && ( 3 == i_Local_Quan ) ) 
	{ 

	//��ʼ�ȶ� 
		if ( ( _ttoi ( csr_FTP_Code_List[0] ) ) < ( _ttoi ( csr_Local_Code_List[0] ) ) ) 
		{    
			//FTP�汾��һλС�ڱ��ذ汾��һλ   
			//����Ҫ����   
			return false;  
		}   
		else if ( ( _ttoi ( csr_FTP_Code_List[0] ) ) > ( _ttoi ( csr_Local_Code_List[0] ) ) ) 
		{    
			//FTP�汾��һλ���ڱ��ذ汾��һλ    
			//��Ҫ����  
			return true; 
		} 
		else if ( ( _ttoi ( csr_FTP_Code_List[0] ) ) == ( _ttoi ( csr_Local_Code_List[0] ) ) )  
		{    
			//FTP�汾��һλ���ڱ��ذ汾��һλ�����еڶ�λ�ȶ�  
			if ( ( _ttoi ( csr_FTP_Code_List[1] ) ) < ( _ttoi ( csr_Local_Code_List[1] ) ) )   
			{    
				//FTP�汾�ڶ�λС�ڱ��ذ汾�ڶ�λ   
				//����Ҫ����    
				return false;  
			}    
			else if ( ( _ttoi ( csr_FTP_Code_List[1] ) ) > ( _ttoi ( csr_Local_Code_List[1] ) ) ) 
			{     
				//FTP�汾�ڶ�λ���ڱ��ذ汾�ڶ�λ   
				//��Ҫ����    
				return true;  
			}    
			else if ( ( _ttoi ( csr_FTP_Code_List[1] ) ) == ( _ttoi ( csr_Local_Code_List[1] ) ) )  
			{    
				//FTP�汾�ڶ�λ���ڱ��ذ汾�ڶ�λ�����е���λ�ȶ�    
				if ( ( _ttoi ( csr_FTP_Code_List[2] ) ) < ( _ttoi ( csr_Local_Code_List[2] ) ) )   
				{     
					//FTP�汾����λС�ڱ��ذ汾����λ   
					//����Ҫ����     
					return false;    
				}     
				else if ( ( _ttoi ( csr_FTP_Code_List[2] ) ) > ( _ttoi ( csr_Local_Code_List[2] ) ) )  
				{      
					//FTP�汾����λ���ڱ��ذ汾����λ    
					//��Ҫ����     
					return true;   
				}     
				else if ( ( _ttoi ( csr_FTP_Code_List[2] ) ) == ( _ttoi ( csr_Local_Code_List[2] ) ) )    
				{      
					//FTP�汾����λ���ڱ��ذ汾����λ 
					//����Ҫ����    
					return false;    
				}   
			}   
		} 
	}  
	else 
	{   //��ʽ����ȷ�������бȽϣ��������� 
		return false; 
	}

	return false;
}

void CMTView::OnSaharaonly()
{
	// TODO: Add your command handler code here

	m_bSHOWMESH=!m_bSHOWMESH;


#ifdef FW_MULTI
	this->m_dlg_Upgrade_FW_Multi.m_bSaharaOnly = m_bSHOWMESH;
	this->m_dlg_Upgrade_FW_Multi.Load_View_Static_Value();
	this->m_dlg_Upgrade_FW_Multi.INIT();
#else//#elif ��������
	this->m_dlg_Upgrade_FW_Single.m_bSaharaOnly = m_bSHOWMESH;
	this->m_dlg_Upgrade_FW_Single.Load_View_Static_Value();
	this->m_dlg_Upgrade_FW_Single.INIT();
#endif


}

void CMTView::OnUpdateSaharaonly(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->SetCheck(m_bSHOWMESH);
}
