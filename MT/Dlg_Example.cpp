// Dlg_Example.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "Dlg_Example.h"

#include "aboot.h"
#include "ptable.h"
#include "io.h"

#include "USBDeviceDetection.h"


// CDlg_Example dialog

extern HANDLE g_hdlWrite;

IMPLEMENT_DYNAMIC(CDlg_Example, CDialog)

CDlg_Example::CDlg_Example(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Example::IDD, pParent)
	, m_iQFLASH_CMD(0)
	, m_iCom_Main_Idx(0)
	, m_iBR_Main_Idx(0)
	, m_csDA_File_Path(_T(""))
	, m_csScat_Cfg_File_Path(_T(""))
	, m_dwThread_ID(0)
	, m_dwThread_Erase_ID(0)
	, m_csCurr_Exe_Path(_T(""))
	, m_iNv_Type(0)
	, m_iPgr_Last(0)
	, m_bInside_Flash(0)
	, m_bLoad_Bin_List(false)
	, m_iPlatform_Curr(-1)
	, m_iBBchip_Curr(-1)
	, m_iCtrl_Dl_Type_S(-1)
	, m_iCtrl_OP_Option_S(-1)
	, m_bGet_Debug_Log(FALSE)
	, m_bFirehsoeFlag(FALSE)
	, m_bNB_BC95_Flag(FALSE)
	, m_csQCN_Temp_File_Path(_T(""))
	, m_bEnable_Advanced_Option(false)
	, m_bEnable_Advanced_Option_1(false)
	, m_bEnable_Com_Method(false)
	, m_bEnable_Emerg_DLoad(false)
	, m_bEnable_Get_Debug_Log(false)
	, m_iCtrl_DLoad_Port_S(0)
	, m_bEnable_DLoad_Port(false)
	, m_iDebug_Log_Handle(0)
	, m_csDebug_Log_File_Path(_T(""))
	, m_iOCPU_Name_S(0)
	, m_iComMethod(0)
	, m_iResetFlag(0)
	, m_iClearNVFlag(0)
	, m_bFirehoseReset(FALSE)
	, m_bSaharaOnly(FALSE)
	, m_bNoBaudSwitch(true)
	, m_bSSB_Update(FALSE)
	, m_blWrCon(FALSE)
	, m_bWriteKV(FALSE)
	, m_strImei(_T(""))
	, m_bWriteFEC(FALSE)
	, m_bClearNv(FALSE)
	, m_b_aboot_engine_init_flag(false)
	, m_str_ASR3601_FW(_T(""))
	, m_iHS_DL_MODE(0)
	, m_iAT_Port(0)
{
	//Edit������ɫ
	m_colorEditText = RGB(0, 0, 0);
	this->m_cla_ARM_OP_QCM_Main.m_bFirehose_Reset_after_DL=false;
	this->m_cla_ARM_OP_QCM_Main.m_bDL_Provision=false;
	this->m_cla_ARM_OP_QCM_Main.m_bPCIE_DL = false;


	m_IndexProgressMap.RemoveAll();
	m_bIsInit = FALSE;
	m_bIsAdminAuthorized = false;

	m_lPInstanceParams = NULL;
	m_gSWDhModule = NULL;
	m_eSWDMode = kFormalRelease;

	m_bIsStart = false;
	m_bIsPackageReady = false;
	m_bInitializeBL = false;
	m_gWtptphModule = NULL;

	m_hLenovoHandle  = NULL;

	this->m_bHoneywell_A4_flag = false;
	this->m_bQDM002_flag = false;
	

	m_PortMap.RemoveAll();
	m_pDlCctrl = NULL;

	
}
//-------------------------------UNISOC----------------------------
HWND CDlg_Example::m_hwndUI = NULL;
TCHAR CDlg_Example::m_szSATES[MAX_PATH] = {0};

void __stdcall CDlg_Example::Progress_Update(UINT nProgress, UINT nPosID, LPVOID pUserData)
{

	UNUSED_ALWAYS( nProgress );
	UNUSED_ALWAYS( pUserData );
	::PostMessage(m_hwndUI,WM_PROGRESS_MSG,nProgress, nPosID);
}
void __stdcall CDlg_Example::Text_Output(LPCTSTR lpszStr, COLORREF clr, UINT nPosID, EUI_TEXT_ID eUITextID, LPVOID pUserData)
{
	//AfxMessageBox(lpszStr);
	UNUSED_ALWAYS( lpszStr );
	UNUSED_ALWAYS( clr );
	UNUSED_ALWAYS( nPosID );
	UNUSED_ALWAYS( eUITextID );
	UNUSED_ALWAYS( pUserData );

	_tcscpy(m_szSATES,lpszStr);
	_tcscat(m_szSATES,_T("\r\n"));

	::SendMessage(m_hwndUI,WM_UNISOC_STATUS_MSG,(WPARAM)m_szSATES,(LPARAM)nPosID);
}
//-------------------------------END----------------------------
//-------------------------------ASR----------------------------
typedef bool (* FREEINSTANCEPARAMS)(PInstanceParams pInstParam);
typedef PInstanceParams (*MALLOCINSTANCEPARAMS)();

typedef bool (* READBLF)(PInstanceParams pInstParam,const TCHAR * pszBLFfileName);
typedef BOOL (* INITIALIZEBL)(PInstanceParams pInstParam);
typedef BOOL (* TERMINATEBL)();
typedef bool (* DOWNLOADWPACKAGE)(const TCHAR* pszDownloadPackagePath,PInstanceParams pInstParam);
typedef bool (* GETERASEFLASHSTATUS)(PInstanceParams pInstParam);
typedef bool (* SETERASEFLASHSTATUS)(PInstanceParams pInstParam,  bool bEraseAllFlash);

static void CALLBACK CallbackProc(LPVOID pParam, WPARAM wParam, LPARAM lParam)
{
	CWnd* pWnd = (CWnd*)pParam;
	NotifyStatus* pNtStat = new NotifyStatus; 
	*pNtStat = *(NotifyStatus*)lParam;
	pWnd->PostMessage(WM_CALLBACK, wParam, (LPARAM)pNtStat);
}

CDlg_Example * g_pDlg_Example =NULL;


static BOOL fnSearchProgressInfo(const CString & csKey, int & nProgress)
{
	int nIndex = csKey.Find(_T("\"progress\" :"));
	if (nIndex < 0)
	{
		return FALSE;
	}
	CString csInfo, csTmp;
	csTmp.Format(_T("%s"), csKey.Mid(nIndex + 12));

	nIndex = csTmp.Find(_T(","));
	if (nIndex < 0)
	{
		return FALSE;
	}
	csInfo.Format(_T("%s"), csTmp.Mid(1, nIndex - 1));

	nProgress = _tstoi(csInfo);
	return TRUE;
}

static BOOL fnSearchPathInfoExt(const CString & csKey,int & nPort)
{
	/*	 "starting to fire device <COM70>...\n"
	}*/

	//
	int nIndex = csKey.Find(_T("COM"));
	if (nIndex < 0)
	{
		return FALSE;
	}
	CString csTmp;
	csTmp.Format(_T("%s"),csKey);


   CString csPortInfo;
   int index=0;

   while (index<4)
   {
	   csPortInfo =  g_pDlg_Example->m_strUSBPortPath[index];
	   if (FALSE == csPortInfo.IsEmpty())
	   {
		   if ( 0 <= csTmp.Find(csPortInfo))
		   {
			   nPort = index+1;
			   //�ҵ�
			   return TRUE;
		   }
	   }
	   index++;
   }
	return FALSE;

}
static  BOOL fnSearchPathInfo(const CString & csKey, CString & csPathInfo)
{
	int nIndex = csKey.Find(_T("\"path\" :"));
	if (nIndex < 0)
	{
		return FALSE;
	}
	CString csInfo, csTmp;
	csTmp.Format(_T("%s"), csKey.Mid(nIndex));

	nIndex = csTmp.Find(_T(","));
	if (nIndex < 0)
	{
		return FALSE;
	}
	csInfo.Format(_T("%s"), csTmp.Mid(0, nIndex));

	//"path" : "COM70"
	nIndex = csInfo.Find(_T("COM"));
	if (nIndex < 0)
	{
		return FALSE;
	}
	csPathInfo.Format(_T("%s"), csInfo.Mid(nIndex, csInfo.GetLength()-nIndex-1));

	return TRUE;
}

static BOOL fnSearchPortInfo_Ext(const CString & csKey,CString & strPath)
{
	/*	{
		"displayName" : "ASR Modem Device (COM78)",
		"enabled" : true,
		"event" : 5,
		"locationInfo" : "Port_#0002.Hub_#0001",
		"order" : 1,
		"path" : "COM78",
		"productId" : 12311,
		"progress" : 0,
		"status" : "ONLINE",
		"vendorId" : 11980
		}*/

	//
	int nIndex = csKey.Find(_T("\"locationInfo\" :"));
	if (nIndex < 0)
	{
		return FALSE;
	}
	CString csInfo,csTmp;
	csTmp.Format(_T("%s"),csKey.Mid(nIndex+ 16));

	nIndex = csTmp.Find(_T(","));
	if (nIndex < 0)
	{
		return FALSE;
	}
	csInfo.Format(_T("%s"),csTmp.Mid(2,nIndex-3));
	if(TRUE ==  csInfo.IsEmpty())
	{
		return FALSE;
	}

	strPath = csInfo;

	return TRUE;
}

static void ConvertTCHARToChar(TCHAR* tcIn, char* cOut, int nOutLength)
{
	LPSTR pszOut = NULL;
	if (tcIn != NULL)
	{
		int nInputStrLen = wcslen(tcIn);

		// Double NULL Termination
		int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, tcIn, nInputStrLen, NULL, 0, 0, 0) + 2;
		pszOut = new char[nOutputStrLen + 1];

		if (pszOut)
		{
			memset(pszOut, 0x00, nOutputStrLen + 1);
			WideCharToMultiByte(CP_ACP, 0, tcIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
		}
	}
	strcpy_s(cOut, nOutLength, pszOut);
	delete pszOut;
	pszOut = NULL;
}
static void judgeDownloadStatus(const CString& csJson)
{
	/*		{
	"displayName" : "ASR Modem Device (COM77)",
	"enabled" : true,
	"event" : 5,
	"locationInfo" : "Port_#0001.Hub_#0001",
	"order" : 2,
	"path" : "COM77",
	"productId" : 12311,
	"progress" : 100,
	"status" : "SUCCEEDED",
	"vendorId" : 11980
	}*/

	/*	2020-3-20_10:20:0 <--> {
		"displayName" : "Quectel Download Port (COM70)",
			"enabled" : false,
			"event" : 6,
			"locationInfo" : "Port_#0001.Hub_#0001",
			"order" : 0,
			"path" : "COM70",
			"productId" : 12311,
			"progress" : 0,
			"status" : "ONLINE",
			"vendorId" : 11980
	}*/



	int nIndex = csJson.Find(_T("\"status\" :"));
	if (nIndex < 0)
	{
		return;
	}
	if (nIndex + 10 > csJson.GetLength())
	{
		g_pDlg_Example->Show_Status(_T("nIndex Error\n"));
		return;
	}

	CString csStatue;
	csStatue.Format(_T("%s"), csJson.Mid(nIndex + 10));
	csStatue.MakeUpper();
	int nPort = 0;

	if (csStatue.Find(_T("ONLINE")) >= 0)
	{
		//��ȡ�����ַ
		fnSearchPathInfo(csJson,g_pDlg_Example->m_strUSBPortPath);
// 		//��ʼ����
// 		g_pDlg_Example->m_ctr_btnStart.EnableWindow(TRUE);
// 		g_pDlg_Example->OnBnClickedButtonStart();

	}
	else if(csStatue.Find(_T("FAILED")) >= 0)
	{
		g_pDlg_Example->Show_Status(_T("DOWNLOAD FAILED\n"));

		g_pDlg_Example->m_cla_ARM_OP_ASR_Main.m_bRuning = false;
		g_pDlg_Example->m_cla_ARM_OP_ASR_Main.m_csError_Code =_T("DOWNLOAD FAILED");
		g_pDlg_Example->m_cla_ARM_OP_ASR_Main.m_iError_Code = 0x03000001;

		g_pDlg_Example->Save_Trace_Log_File(csJson);
		g_pDlg_Example->Save_Trace_Log_File(_T("DOWNLOAD FAILED"));
		::SetEvent(g_pDlg_Example->m_hWaitTestFin_Main);	
	}
	else if (csStatue.Find(_T("SUCCEEDED")) >= 0)
	{

		g_pDlg_Example->Show_Status(_T("DOWNLOAD SUCCEEDED\n"));

		g_pDlg_Example->m_cla_ARM_OP_ASR_Main.m_bRuning = false;
		g_pDlg_Example->m_cla_ARM_OP_ASR_Main.m_csError_Code =_T("");
		g_pDlg_Example->m_cla_ARM_OP_ASR_Main.m_iError_Code = 0xffffffff;
		g_pDlg_Example->Save_Trace_Log_File(_T("DOWNLOAD SUCCEEDED"));
		::SetEvent(g_pDlg_Example->m_hWaitTestFin_Main);
	}
	else
	{

	}
}
static void aboot_download_callback(const char *json,void *ctx )
{
	CString csJson;

	CDlg_Example  * pSv = g_pDlg_Example;


	int iLen =0;
	wchar_t * wcJson = NULL;
	//�����ڴ�
	pSv->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcJson, strlen(json) + 256);
	//��������
	pSv->m_cla_GFC.MultiByte_To_WideChar(json, wcJson, &iLen);
	csJson.Format(_T("%s"), wcJson);
	//�ͷ��ڴ�
	pSv->m_cla_GFC.BUFF_WCHAR_FREE(&wcJson);

	pSv->Show_Status(csJson);

	pSv->Save_Trace_Log_File(csJson);

	int nProgress = 0;
	BOOL  bSearFlag = FALSE;
	CString  strPercent;
	int nPort = 0;

	BOOL bEraseFlag = FALSE;

	if ((fnSearchProgressInfo(csJson, nProgress))&&(nProgress>0))
	{
		pSv->Show_Progress(nProgress);	
	}
	
	bool bIsOffline = false;
	if (csJson.Find(_T("\"status\" : \"OFFLINE\",")) >= 0)
	{
		bIsOffline = true;
		CString strInfo;
		strInfo.Format(_T("Port:%d-DL_State:%d-IsOffline:%d"),nPort,pSv->m_irDL_OBJ_State,bIsOffline);
		pSv->Show_Status(strInfo);
		pSv->Save_Trace_Log_File(strInfo);
		pSv->Show_Status(_T("Module Offline,Can\'t Download"));
		//pSv->m_ctr_btnStart.EnableWindow(FALSE);
	}
	else
	{
		int nIndex =-1;
		nIndex = csJson.Find(_T("ready to be enabled manually"));//���������߳���enable��������
		if (nIndex >= 0)
		{
		//	pSv->m_ctr_btnStart.EnableWindow(TRUE);
		}

		if (csJson.Find(_T("\"status\" : \"ONLINE\",")) >= 0)
		{
// 			bIsOffline = false;
// 			pSv->Show_Status(_T("Please Click Start Button"));
// 			pSv->m_ctr_btnStart.EnableWindow(TRUE);		
			
		}
		else
		{
			//...
// 			if (true == pSv->m_cla_ARM_OP_ASR_Main.m_bRuning)
// 			{
// 				pSv->m_ctr_btnStart.EnableWindow(FALSE);
// 			}

		}

	}


	//�������״̬
	judgeDownloadStatus(csJson);

	int nIndex = csJson.Find(_T("\"event\" :"));
	if (nIndex < 0)
	{
		return ;
	}


	int nValue =0;

	if (csJson.GetLength()> (nIndex+9))
	{
		nValue = _tstoi(csJson.Mid(nIndex+9));
	} 
	else
	{
		pSv->Show_Status(_T("csJson len error"));
		pSv->Save_Trace_Log_File(_T("csJson len error"));
	}
	
	

	
	switch(nValue)
	{
	case ABOOT_EVENT_SYSTEM_LOG:

		{
			nIndex = csJson.Find(_T("\"message\" :"));
			if (nIndex < 0)
			{
				return ;
			}
			CString strShowInfo;

			strShowInfo = csJson.Mid(nIndex + 11);

			
			pSv->Show_Status(strShowInfo);

			//ֻ��������Ϣ��������Ϊ���ؽ���
			//2019-12-5_10:27:16 <--> {
			//	"event" : 0,
			//	"locationInfo" : "Port_#0001.Hub_#0001",
			//	"message" : "<COM70> all finished. total time: 29.408s\n"
			//}
			nIndex = strShowInfo.Find(_T("all finished"));
			if (nIndex >= 0)
			{
				 pSv->m_ASR_DL_OK = true;
				 pSv->Save_Trace_Log_File(csJson);
			}
			
			//20200319
			nIndex = strShowInfo.Find(_T("aboot download engine started failed: boot engine startup failed"));
			if (nIndex >= 0)
			{
//				pSv->m_ctr_btnStart.EnableWindow(FALSE);
			}

			nIndex = strShowInfo.Find(_T("new device arrived"));
			if (nIndex >= 0)
			{
//				pSv->m_ctr_btnStart.EnableWindow(TRUE);
			}
			nIndex = strShowInfo.Find(_T("Aboot server started on port"));
			if (nIndex >= 0)
			{
				//��ʼ����
// 				pSv->m_ctr_btnStart.EnableWindow(TRUE);
// 				pSv->OnBnClickedButtonStart();

//				pSv->Show_Status(_T("Please Click Start Button"));



				//	
			}

			 
			


			//���һ��log

			//"Reading from COM port (ReadIOCompletion): Operation aborted\n"
			//"disconnected from serial device <COM??> successfully!\n" ��;�γ�

			strShowInfo.MakeLower();
			if ((strShowInfo.Find(_T("operation aborted")) >=0)
				||(strShowInfo.Find(_T("operation aborted")) >=0))
			{
				pSv->Show_Status(csJson);
				pSv->Save_Trace_Log_File(csJson);
				
			}
					 
		}
		break;
	case ABOOT_EVENT_DOWNLOAD_INIT:

		pSv->Save_Trace_Log_File(_T("### ABOOT_EVENT_DOWNLOAD_INIT ###\n"));
		pSv->Show_Status(_T("### ABOOT_EVENT_DOWNLOAD_INIT ###\n"));
		{

//			pSv->m_ctr_btnStart.EnableWindow(FALSE);

			pSv->Show_Status(csJson);
			pSv->Show_Status(_T("Aboot_Download_Start"));
			pSv->Save_Trace_Log_File(csJson);


			bool usbAutoEnable =true;
			bool usbOnly = true;
			bool dumpEnable = false;
			bool enableOnce = true;

			CStringA	csInfo;
			CStringA	csTmp;

			csInfo.Format("");
			csInfo+="{\r\n";
			csTmp.Format("\"baudRate\" : %d,\r\n",115200);
			csInfo+=csTmp;
			csTmp.Format("\"usbOnly\" : %s,\r\n",usbOnly ? "true" : "false");
			csInfo+=csTmp;

			csTmp.Format("\"usbAutoEnable\" : %s,\r\n",usbAutoEnable ? "true" : "false");
			csInfo+=csTmp;

			csTmp.Format("\"enableOnce\" : %s,\r\n", enableOnce ? "true" : "false");
			csInfo += csTmp;

			//20200608
// 			csTmp.Format("\"atFallback\" : true,\r\n");
// 			csInfo += csTmp;

			//20191205 reboot ���غ��Ƿ���Ҫ�Զ�������Ŀǰ���Զ�����
			csTmp.Format("\"reboot\" : false,\r\n");
			csInfo += csTmp;


             //20191204 ����
			 //"productionMode": true/false ��������������д���Ǹ�����д
			bool bProductionMode = false;//������ʽ

#ifdef FW_MULTI
#ifndef FW_UPGRADE
			bEraseFlag = TRUE;
			bProductionMode = true; //����ȫFormat���ط�ʽ
#endif
#endif
			csTmp.Format("\"productionMode\" : %s,\r\n", bProductionMode ? "true" : "false");
			csInfo += csTmp;
#if 0
			//����ʱ��Ҫ
			if ((FALSE == bEraseFlag) && (pSv->m_bDisableGroupInit))
			{
				csTmp.Format("\"dumpEnable\" : %s,\r\n", dumpEnable ? "true" : "false");
				csInfo+=csTmp;

				CStringA csGroup(""),csGroupTmp;
				for (int i = 0; i < pSv->m_iDisableGroupNum; i++)
				{
					if (i == 0)
					{
						csGroupTmp.Format("\"%d\"", pSv->m_iDisableGroupName[i]);
					}
					else
					{
						csGroupTmp.Format(", \"%d\"", pSv->m_iDisableGroupName[i]);
					}
					csGroup = csGroup + csGroupTmp;
				}
				csTmp.Format("\"disabledGroups\" : [ %s ]\r\n", csGroup);
				csInfo += csTmp;
			}
			else
#endif
			{
				csTmp.Format("\"dumpEnable\" : %s\r\n", dumpEnable ? "true" : "false");
				csInfo+=csTmp;
			}



			csTmp.Format("}\r\n");
			csInfo+=csTmp;



			wchar_t * wcInfo = NULL;
			CString  strInfo,strInfo1;
			//�����ڴ�
			pSv->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcInfo, strlen(csInfo) + 256);
			//��������
			pSv->m_cla_GFC.MultiByte_To_WideChar(csInfo, wcInfo, &iLen);
			strInfo.Format(_T("%s"), wcInfo);
			//�ͷ��ڴ�
			pSv->m_cla_GFC.BUFF_WCHAR_FREE(&wcInfo);

			strInfo1.Format(_T("++++aboot_download_start - %s\r\n"),strInfo);
			g_pDlg_Example->Show_Status(strInfo1);
			g_pDlg_Example->Save_Trace_Log_File(strInfo1);



			if (aboot_download_start(csInfo.GetBuffer(0)) < 0)
			{
				//rc = -1;
				//result_promise.set_value();
			}
			else
			{

			}
			
			csInfo.ReleaseBuffer();
			
		}
		break;


	case ABOOT_EVENT_DOWNLOAD_START:

		pSv->Save_Trace_Log_File(_T("### ABOOT_EVENT_DOWNLOAD_START ###\n"));
		pSv->Show_Status(_T("### ABOOT_EVENT_DOWNLOAD_START ###\n"));

		break;

	case ABOOT_EVENT_DOWNLOAD_STOP:


		pSv->Save_Trace_Log_File(_T("### ABOOT_EVENT_DOWNLOAD_STOP ###\n"));
		pSv->Show_Status(_T("### ABOOT_EVENT_DOWNLOAD_STOP ###\n"));

		//20191204
		pSv->aboot_download_init_Ext();

		break;

	case ABOOT_EVENT_DEVICE_NEW:

		//pSv->Show_Status(_T("### ABOOT_EVENT_DEVICE_NEW ###\n"));
		pSv->Show_Status(_T("ABOOT_EVENT_DEVICE_NEW"));
		pSv->Save_Trace_Log_File(_T("### ABOOT_EVENT_DEVICE_NEW ###\n"));

		break;

	case ABOOT_EVENT_DEVICE_CHANGE:

		//pSv->Show_Status(_T("### ABOOT_EVENT_DEVICE_CHANGE ###\n"));
		pSv->Save_Trace_Log_File(_T("### ABOOT_EVENT_DEVICE_CHANGE ###\n"));

		break;
	default:
		break;
	}

}











//-------------------------------END----------------------------
CDlg_Example::~CDlg_Example()
{
	//-------------------------------ASR----------------------------
	if(m_lPInstanceParams)
	{
		if (m_gSWDhModule)
		{
			FREEINSTANCEPARAMS FreeInstanceParams = (FREEINSTANCEPARAMS)GetProcAddress(m_gSWDhModule, "FreeInstanceParams");
			FreeInstanceParams(m_lPInstanceParams);
			m_lPInstanceParams = NULL;
		}

	}
	if (m_gSWDhModule != NULL)
	{
		FreeLibrary(m_gSWDhModule);
		m_gSWDhModule = NULL;
	}

	if(m_bInitializeBL)
	{
		StopDownload();
		m_bInitializeBL = false;
	}

	//ASR 3601
	if (ASR3601 == this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip)
	{

		aboot_download_stop();
		aboot_engine_unregister_cb();
		aboot_engine_exit();


		//20191203
// 		if(PathFileExists(this->m_str_ASR3601_FW))
// 		{
// 			::SetFileAttributes(this->m_str_ASR3601_FW, FILE_ATTRIBUTE_NORMAL);  
// 			::DeleteFile(this->m_str_ASR3601_FW);
// 		}  

		m_sExtractPackageFile.Format(_T("%s\\%s"),m_csCurr_Exe_Path,TEMP_PACKAGE);
		
		if(PathFileExists(this->m_sExtractPackageFile))
		{
			this->DeleteFolder(this->m_sExtractPackageFile);
		}
	




		Sleep(3000);	
	}

	//...
	if (true == m_bWaitTestEvent_Main)
	{
		::SetEvent(m_hWaitTestFin_Main);
	}

	//-------------------------------END----------------------------
	//-------------------------------UNISOC----------------------------
	if (m_pDlCctrl)
	{
		ReleaseFlashToolObject(m_pDlCctrl);
		m_pDlCctrl = NULL;
	}
	m_PortMap.RemoveAll();
	//-------------------------------END----------------------------
}

void CDlg_Example::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_COM_MAIN, m_iCom_Main_Idx);
	DDX_Control(pDX, IDC_COMBO_COM_MAIN, m_ctr_cbxCom_Main);
	DDX_CBIndex(pDX, IDC_COMBO_BR_MAIN, m_iBR_Main_Idx);
	DDX_Control(pDX, IDC_COMBO_BR_MAIN, m_ctr_cbxBR_Main);
	DDX_Control(pDX, IDC_BUTTON_SET_SCAT_FILE, m_ctr_btnSet_Scat_Cfg_File);
	DDX_Control(pDX, IDC_BUTTON_START, m_ctr_btnStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_ctr_btnStop);
	DDX_Control(pDX, IDC_LIST_BIN_LIST, m_ctr_lscBin_List);
	DDX_Control(pDX, IDC_EDIT_STATUS_MAIN, m_ctr_edtStatus_Main);
	DDX_Control(pDX, IDC_EDIT_STATUS_MAIN2, m_ctr_edtStatus_Main2);

	DDX_Control(pDX, IDC_PROGRESS_MAIN_PORT, m_ctr_pgrProgress_Main);
	DDX_Control(pDX, IDC_EDIT_TIME, m_ctr_edtTime_Main);
	DDX_Control(pDX, IDC_STATIC_SETTING_ADV, m_ctr_staConfig_Adv);
	DDX_CBIndex(pDX, IDC_COMBO_CTRL_DL_TYPE_S, m_iCtrl_Dl_Type_S);
	DDX_Control(pDX, IDC_COMBO_CTRL_DL_TYPE_S, m_ctr_cbxCtrl_Dl_Type_S);
	DDX_CBIndex(pDX, IDC_COMBO_CTRL_OP_OPTION_S, m_iCtrl_OP_Option_S);
	DDX_Control(pDX, IDC_COMBO_CTRL_OP_OPTION_S, m_ctr_cbxCtrl_OP_Option_S);
	DDX_Check(pDX, IDC_CHECK_GET_DEBUG_LOG, m_bGet_Debug_Log);
	DDX_Control(pDX, IDC_CHECK_GET_DEBUG_LOG, m_ctr_cekGet_Debug_Log);
	DDX_Control(pDX, IDC_STATIC_CTRL_DL_TYPE_S, m_ctr_staCtrl_Dl_Type_S);
	DDX_Control(pDX, IDC_STATIC_CTRL_OP_OPTION_S, m_ctr_staCtrl_OP_Option_S);
	DDX_Control(pDX, IDC_COMBO_CTRL_DL_PORT_S, m_ctr_cbxCtrl_DLoad_Port_S);
	DDX_CBIndex(pDX, IDC_COMBO_CTRL_DL_PORT_S, m_iCtrl_DLoad_Port_S);
	DDX_Control(pDX, IDC_STATIC_CTRL_DL_PORT_S, m_ctr_staCtrl_DLoad_Port_S);
	DDX_CBIndex(pDX, IDC_COMBO_OCPU_MODEL_S, m_iOCPU_Name_S);
	DDX_Control(pDX, IDC_COMBO_OCPU_MODEL_S, m_ctr_cbxOCPU_Name_S);
	DDX_Control(pDX, IDC_STATIC_OCPU_MODEL_S, m_ctr_staOCPU_Name_S);
	DDX_Control(pDX, IDC_FIREHOSE_RESET, m_ctrlFirehoseReset);
	DDX_Check(pDX, IDC_FIREHOSE_RESET, m_bFirehoseReset);
	//DDX_Control(pDX, IDC_SAHARA_DL, m_ctrlSaharaDL);
	//DDX_Check(pDX, IDC_SAHARA_DL, m_bSaharaDL);
	DDX_Control(pDX, IDC_noBaudSwitch, m_ctrlNoBaudSwitch);
	DDX_Check(pDX, IDC_noBaudSwitch, (BOOL)m_bNoBaudSwitch);
	DDX_Control(pDX, IDC_SSB_UP, m_ctrlSSB_Update);
	DDX_Check(pDX, IDC_SSB_UP, m_bSSB_Update);
	DDX_Control(pDX, IDC_Wr_Con, m_ctrlWrCon);
	DDX_Check(pDX, IDC_Wr_Con, m_blWrCon);
	DDX_Check(pDX, IDC_WriteKV, m_bWriteKV);
	DDX_Control(pDX, IDC_Erase, m_ctrlErase);
	DDX_Control(pDX, IDC_E_IMEI, m_ctrlEditImei);
	DDX_Text(pDX, IDC_E_IMEI, m_strImei);
	DDX_Control(pDX, IDC_WriteKV, m_ctrlWKV);
	DDX_Control(pDX, IDC_COMBO_COMM_METHOD, m_ctr_ComMethod);
	DDX_CBIndex(pDX, IDC_COMBO_COMM_METHOD, m_iComMethod);

	DDX_Control(pDX, IDC_STATIC_DL_TPPE, m_ctrl_Dl_Type);
	DDX_Control(pDX, IDC_FEC, m_ctrlFEC);
	DDX_Check(pDX, IDC_FEC, m_bWriteFEC);
	DDX_Control(pDX, IDC_ClearNv, m_CtrlClearNv);
	DDX_Check(pDX, IDC_ClearNv, m_bClearNv);
	DDX_Text(pDX, IDC_AT_PORT, m_iAT_Port);
}


BEGIN_MESSAGE_MAP(CDlg_Example, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_Example::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_COM_MAIN, &CDlg_Example::OnCbnSelchangeComboComMain)
	ON_CBN_SELCHANGE(IDC_COMBO_BR_MAIN, &CDlg_Example::OnCbnSelchangeComboBrMain)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BIN_LIST, &CDlg_Example::OnLvnItemchangedListBinList)
	ON_BN_CLICKED(IDC_BUTTON_SET_SCAT_FILE, &CDlg_Example::OnBnClickedButtonSetScatFile)
	ON_MESSAGE(WM_STATUS_DOS, OnGetMsg_Show_Dos)
	ON_MESSAGE(WM_STATUS_MSG, OnGetMsg_Show_Status)
	ON_MESSAGE(WM_STATUS_MSG_2, OnGetMsg_Show_Status2)
	ON_MESSAGE(WM_PROGRESS_MSG, OnGetMsg_Show_Progress)
	ON_MESSAGE(WM_PROGRESS_ENTIRE_MSG, OnGetMsg_Show_Progress_Entire)
	ON_MESSAGE(WM_COLOUR_MSG, OnGetMsg_Ctrl_Colour)
	ON_MESSAGE(WM_DL_INFO_MSG, OnGetMsg_DL_Info)
	ON_MESSAGE(WM_HOTKEY, OnGetMsg_Hot_Key)
	ON_MESSAGE(WM_HOTKEY_UPDATE_GEN_MSG, OnGetMsg_Hot_Key_Update_Gen)
	ON_MESSAGE(WM_HOTKEY_DLOAD_MSG, OnGetMsg_Hot_Key_DLoad)
	ON_MESSAGE(WM_HOTKEY_OPTION_MSG, OnGetMsg_Hot_Key_Option)
	ON_MESSAGE(WM_HOTKEY_OPTION_MSG_1, OnGetMsg_Hot_Key_Option_1)
	ON_MESSAGE(WM_HOTKEY_OPTION_MSG_METHOD, OnGetMsg_Hot_Key_Option_Method)
	ON_MESSAGE(WM_HOTKEY_DEBUG_MSG, OnGetMsg_Hot_Key_Get_Debug_Log)
	ON_MESSAGE(WM_HOTKEY_DLOAD_PORT_MSG, OnGetMsg_Hot_Key_DLoad_Port)
	ON_MESSAGE(WM_TRACE_LOG_MSG, OnGetMsg_Trace_Log_Msg)
	ON_MESSAGE(WM_HOTKEY_FIREHOSE_MSG, OnGetMsg_Hot_Key_Firehose)
	ON_MESSAGE(WM_HOTKEY_NB_BC95_PARA_MSG, OnGetMsg_Hot_Key_NB_BC95_PARA)
	ON_MESSAGE(WM_OUT_STATUS_MSG, OnGetMsg_Trace_Log_Msg)


	ON_BN_CLICKED(IDC_BUTTON_START, &CDlg_Example::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlg_Example::OnBnClickedButtonStop)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_CBN_DROPDOWN(IDC_COMBO_COM_MAIN, &CDlg_Example::OnCbnDropdownComboComMain)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_CTRL_DL_TYPE_S, &CDlg_Example::OnCbnSelchangeComboCtrlDlTypeS)
	ON_CBN_SELCHANGE(IDC_COMBO_CTRL_OP_OPTION_S, &CDlg_Example::OnCbnSelchangeComboCtrlOpOptionS)
	ON_BN_CLICKED(IDC_CHECK_GET_DEBUG_LOG, &CDlg_Example::OnBnClickedCheckGetDebugLog)
	ON_CBN_SELCHANGE(IDC_COMBO_CTRL_DL_PORT_S, &CDlg_Example::OnCbnSelchangeComboCtrlDlPortS)
	ON_CBN_SELCHANGE(IDC_COMBO_OCPU_MODEL_S, &CDlg_Example::OnCbnSelchangeComboOcpuModelS)
	ON_BN_CLICKED(IDC_FIREHOSE_RESET, &CDlg_Example::OnBnClickedFirehoseReset)
	ON_BN_CLICKED(IDC_SAHARA_DL, &CDlg_Example::OnBnClickedSaharaDl)
	ON_BN_CLICKED(IDC_noBaudSwitch, &CDlg_Example::OnBnClickednobaudswitch)
	ON_BN_CLICKED(IDC_Wr_Con, &CDlg_Example::OnBnClickedWrCon)
	ON_BN_CLICKED(IDC_WriteKV, &CDlg_Example::OnBnClickedWriteKV)

	ON_BN_CLICKED(IDC_SSB_UP, &CDlg_Example::OnBnClickedSsbUp)
	ON_BN_CLICKED(IDC_Erase, &CDlg_Example::OnBnClickedErase)
	ON_BN_CLICKED(IDC_R_Version, &CDlg_Example::OnBnClickedRVersion)
	ON_BN_CLICKED(IDC_R_IMEI, &CDlg_Example::OnBnClickedRImei)
	ON_BN_CLICKED(IDC_W_IMEI, &CDlg_Example::OnBnClickedWImei)

	ON_MESSAGE(WM_CALLBACK, OnCallbackFunc_RSA)

	ON_CBN_SELCHANGE(IDC_COMBO_COMM_METHOD, &CDlg_Example::OnCbnSelchangeComboCommMethod)
	ON_BN_CLICKED(IDC_FEC, &CDlg_Example::OnBnClickedFec)
	ON_BN_CLICKED(IDC_ClearNv, &CDlg_Example::OnBnClickedClearnv)
	ON_BN_CLICKED(IDC_SET_DL_MODE, &CDlg_Example::OnBnClickedSetDlMode)
	ON_MESSAGE(WM_UNISOC_STATUS_MSG, OnGetMsg_Unisoc_Show_Status)
END_MESSAGE_MAP()


// CDlg_Example message handlers

void Operate_1(LPVOID);

void CDlg_Example::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	OnOK();
}

//cb Show_State
static void __stdcall cb_show_state_m(const wchar_t * wcp_State, void * p_usr_Arg)
{
	CDlg_Example * pSv = (CDlg_Example *)p_usr_Arg;

	pSv->Show_Status(wcp_State);

}

void CDlg_Example::Read_Config(void)
{
	bool bRetFlag = false;
	this->m_iCom_Main_Idx = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Com_Main"), 0);
	this->m_iBR_Main_Idx = this->m_cla_iniFile.ReadInt(_T("Main"), _T("BR_Main"), 6);
	this->m_csScat_Cfg_File_Path = this->m_cla_iniFile.ReadStr(_T("Main"), _T("Scat_Cfg_File_Path"), _T(""));
	this->m_csQCN_Temp_File_Path = this->m_cla_iniFile.ReadStr(_T("Main"), _T("QCN_Temp_File_Path"), _T(""));
	this->m_iOCPU_Name_S = this->m_cla_iniFile.ReadInt(_T("Main"), _T("OCPU_Name"), 0);
	this->m_iComMethod = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Com_Method"), IN_UART);
	this->m_iResetFlag = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Reset_after_DL"), 0);
	this->m_iClearNVFlag = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Clear_User_Data"), 0);

	if (0 == this->m_iClearNVFlag)
	{
		this->m_bClearNv =FALSE;
	} 
	else
	{
		this->m_bClearNv =TRUE;
	}
	 


	
	

	bRetFlag =  this->m_cla_iniFile.ReadB(_T("Main"), _T("BC95_Baud_Switch"), false);

	if (true == bRetFlag)
	{
		this->m_cla_ARM_OP_HUAWEI_Main.m_iBC95BaudSwitch =1;
	} 
	else
	{
		this->m_cla_ARM_OP_HUAWEI_Main.m_iBC95BaudSwitch =0;
	}
	
	this->m_cla_ARM_OP_HUAWEI_Main.m_iBcg_ssb_COM_update_B200 =  this->m_cla_iniFile.ReadInt(_T("Main"), _T("Bcg_ssb_com_update_B200"), 0);
	this->m_cla_ARM_OP_HUAWEI_Main.m_iBcg_ssb_COM_update_B200_B180 =  this->m_cla_iniFile.ReadInt(_T("Main"), _T("Bcg_ssb_com_update_B200_B180"), 0);
	this->m_cla_ARM_OP_HUAWEI_Main.m_iBcg_ssb_com= this->m_cla_iniFile.ReadInt(_T("Main"), _T("Bcg_ssb_com"), 0);//1:������Ƭ��Ҫ��������SSB.BIN
	this->m_cla_ARM_OP_HUAWEI_Main.m_bCheckSaveLog =  this->m_cla_iniFile.ReadB(_T("Main"), _T("BC95_Save_Log"), false);

	this->m_cla_ARM_OP_HUAWEI_Main.m_iBC95_EraseSwitch =  this->m_cla_iniFile.ReadInt(_T("Main"), _T("BC95_Erase_Switch"), 0);

	this->m_cla_ARM_OP_QCM_Main.m_bFirehose_Reset_after_DL =  this->m_cla_iniFile.ReadB(_T("Main"), _T("Firehose_Reset_after_DL"), false);

	this->m_cla_ARM_OP_QCM_Main.m_bDL_Provision =  this->m_cla_iniFile.ReadB(_T("Main"), _T("DL_Provision"), false);

	this->m_cla_ARM_OP_QCM_Main.m_bPCIE_DL = this->m_cla_iniFile.ReadB(_T("Main"), _T("PCIE_DL"), false);
	
	this->m_bFirehoseReset = this->m_cla_ARM_OP_QCM_Main.m_bFirehose_Reset_after_DL;

	if (this->m_cla_ARM_OP_HUAWEI_Main.m_iBC95BaudSwitch ==0)
	{
		this->m_bNoBaudSwitch = false;
	} 
	else
	{
		this->m_bNoBaudSwitch  = true;
	}

	this->m_blWrCon =  this->m_cla_iniFile.ReadB(_T("Main"), _T("Wirte_RF_Configure"), false);

	this->m_iHS_DL_MODE=  this->m_cla_iniFile.ReadInt(_T("Main"), _T("HS_DL_MODE"), 0);

	this->m_cla_ARM_OP_HUAWEI_Main.m_blWrCon =this->m_blWrCon;

	this->m_cla_ARM_OP_HUAWEI_Main.m_iHS_DL_MODE_M =this->m_iHS_DL_MODE;

	//this->m_bWriteKV =  this->m_cla_iniFile.ReadB(_T("Main"), _T("WriteKV"), false);
	//this->m_cla_ARM_OP_HUAWEI_Main.m_bWriteKV =this->m_bWriteKV;

	//this->m_bWriteFEC =  this->m_cla_iniFile.ReadB(_T("Main"), _T("WriteFEC"), false);
	//this->m_cla_ARM_OP_HUAWEI_Main.m_bWriteFEC =this->m_bWriteFEC;


	//20200619 ��Ҫ��BG96��MBN�ӻ����Ӫ�̵�����
	this->m_cla_ARM_OP_QCM_Main.m_strOpName = this->m_cla_iniFile.ReadStr(_T("Main"), _T("MBN_USER_NAME"), _T(""));


	//20180503
	if(1== this->m_iQFLASH_CMD )
	{
		this->m_csScat_Cfg_File_Path = this->strCmdPath;
	}

	this->m_bHoneywell_A4_flag = false;
	this->m_bQDM002_flag = false;


}

void CDlg_Example::Save_Config(void)
{
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Com_Main"), this->m_iCom_Main_Idx);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("BR_Main"), this->m_iBR_Main_Idx);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("Scat_Cfg_File_Path"), this->m_csScat_Cfg_File_Path);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("QCN_Temp_File_Path"), this->m_csQCN_Temp_File_Path);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("OCPU_Name"), this->m_iOCPU_Name_S);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Com_Method"), this->m_iComMethod);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Clear_User_Data"), this->m_iClearNVFlag);
	
	

	//this->m_cla_iniFile.StoreB(_T("Main"), _T("SAHARA_DL_Switch"), this->m_bSaharaDL);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("Firehose_Reset_after_DL"), this->m_bFirehoseReset);
	bool bRetflag = false;
	if (1== this->m_cla_ARM_OP_HUAWEI_Main.m_iBC95BaudSwitch)
	{
		bRetflag = true;
	} 
	this->m_cla_iniFile.StoreB(_T("Main"), _T("BC95_Baud_Switch"), bRetflag);
}

void CDlg_Example::OnCbnSelchangeComboComMain()
{
	this->UpdateData(TRUE);
	this->Save_Config();
	//�������õı������ز���
	this->Load_Flash_Option_Params();
}

void CDlg_Example::OnCbnSelchangeComboBrMain()
{
	this->UpdateData(TRUE);
	this->Save_Config();
	//�������õı������ز���
	this->Load_Flash_Option_Params();
}

void CDlg_Example::Update_Com_Ctrl_Option()
{
	//�����COM�����˵��ؼ����ٰ��������ĵ���COM�ں�����COM�����˵��ؼ��У�
	//�������䵱ǰѡ��趨�����˵�ʹ��״̬

	//����������ʽ���趨COM�б�
	//������б�
	while(this->m_ctr_cbxCom_Main.GetCount())
	{
		this->m_ctr_cbxCom_Main.DeleteString(0);
	}
	//
	if((this->m_iPlatform_Curr == PF_MTK) && (this->m_cla_Flash_OP_Main.m_iComm_Method == IN_USB))
	{
		if (BB_MT2625 != m_iBBchip_Curr)
		{
			//ֻ��MTKƽ̨��USB���زŲ���Ҫ����ѡ��COM�ں�
			this->m_ctr_cbxCom_Main.AddString(_T("USB"));
			this->m_ctr_cbxCom_Main.EnableWindow(FALSE);
			this->m_iCom_Main_Idx = 0;
		} 
		else
		{
			//...
		}
	}
	else if((this->m_iPlatform_Curr == PF_INTEL)
		     ||((this->m_iPlatform_Curr == PF_ASR) && (ASR3601 != this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip)))
	{
		//Intelƽ̨����USB Device���أ�����û��USB�ں�
		this->m_ctr_cbxCom_Main.AddString(_T("USB"));
		this->m_ctr_cbxCom_Main.EnableWindow(FALSE);
		this->m_iCom_Main_Idx = 0;
	}
	else
	{
		CString csData;
		int i = 0;
		int irCom_Port_List[64];
		int iCom_Port_Quan = 0;

		//����PC������COM��
		memset(irCom_Port_List, 0, sizeof(irCom_Port_List));
		this->m_cla_Flash_OP_Main.m_cla_GFC.Get_Reg_Com_Port_List(irCom_Port_List, &iCom_Port_Quan);

		//����COM���б�
		while(i < iCom_Port_Quan)
		{
			csData.Format(_T("%d"), irCom_Port_List[i]);
			this->m_ctr_cbxCom_Main.AddString(csData);
			i++;
		}

		if(this->m_iCom_Main_Idx >= iCom_Port_Quan)
		{
			this->m_iCom_Main_Idx = iCom_Port_Quan - 1;
		}

		this->m_ctr_cbxCom_Main.EnableWindow(TRUE);
	}

	this->m_ctr_cbxCom_Main.SetCurSel(this->m_iCom_Main_Idx);
}

BOOL CDlg_Example::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//CString sCmdline = ::GetCommandLine();  
	//AfxMessageBox(sCmdline); 

	//��Class��ʶ��Index
	this->m_cla_Flash_OP_Main.m_iClass_Idx = 1;
	this->m_cla_Flash_OP_Main.m_hWnd_View = this->m_hWnd;

	this->m_cla_ARM_OP_NB_Main.m_iClass_Idx = 1;
	this->m_cla_ARM_OP_NB_Main.m_hWnd_View = this->m_hWnd;
	
	this->m_cla_ARM_OP_5G_Main.m_iClass_Idx = 1;
	this->m_cla_ARM_OP_5G_Main.m_hWnd_View = this->m_hWnd;

	this->m_cla_ARM_OP_QCM_Main.m_iClass_Idx = 1;
	this->m_cla_ARM_OP_QCM_Main.m_hWnd_View = this->m_hWnd;

	this->m_cla_ARM_OP_RDA_Main.m_iClass_Idx = 1;
	this->m_cla_ARM_OP_RDA_Main.m_hWnd_View = this->m_hWnd;

	this->m_cla_ARM_OP_INTEL_Main.m_iClass_Idx = 1;
	this->m_cla_ARM_OP_INTEL_Main.m_hWnd_View = this->m_hWnd;

	this->m_cla_ARM_OP_HUAWEI_Main.m_iClass_Idx = 1;
	this->m_cla_ARM_OP_HUAWEI_Main.m_hWnd_View = this->m_hWnd;

	this->m_cla_ARM_OP_EIGEN_Main.m_iClass_Idx = 1;
	this->m_cla_ARM_OP_EIGEN_Main.m_hWnd_View = this->m_hWnd;

	this->m_cla_ARM_OP_BEKEN_Main.m_iClass_Idx = 1;
	this->m_cla_ARM_OP_BEKEN_Main.m_hWnd_View = this->m_hWnd;

	//����CListCtrl��ʽ������
//	ListView_SetExtendedListViewStyle(this->m_ctr_lscBin_List.GetSafeHwnd(), this->m_ctr_lscBin_List.GetExStyle() | LVS_EX_CHECKBOXES);
	this->m_ctr_lscBin_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	this->m_ctr_lscBin_List.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 147, -1);
	this->m_ctr_lscBin_List.InsertColumn(1, _T("Location"), LVCFMT_LEFT, 540, -1);

	//��������
	this->Set_View_Ctrl_Font();

	g_pDlg_Example = this;


	//-----------------------------------------------------ASR---------------------
	m_gSWDhModule = LoadLibrary(IFLSWDDLL_NAME);
	if (m_gSWDhModule)
	{
		MALLOCINSTANCEPARAMS MallocInstanceParams = (MALLOCINSTANCEPARAMS)GetProcAddress(m_gSWDhModule, "MallocInstanceParams");
		if (NULL == MallocInstanceParams)
		{
			AfxMessageBox(_T("LoadLibrary function failed for IFL_SWD.dll"));
			return FALSE;
		}
		if(NULL==m_lPInstanceParams)
		{
			m_lPInstanceParams = MallocInstanceParams();
		}
	}
	else
	{
		AfxMessageBox(_T("IFL_SWD.dll not find"));
		return FALSE;
	}
	//-----------------------------------------------------END---------------------

	//ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlg_Example::OnLvnItemchangedListBinList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: Add your control notification handler code here
	/* 
	typedef struct tagNMLISTVIEW 
	{ 
	NMHDR   hdr; 
	int     iItem; 
	int     iSubItem; 
	UINT    uNewState; 
	UINT    uOldState; 
	UINT    uChanged; 
	POINT   ptAction; 
	LPARAM  lParam; 
	} NMLISTVIEW, *LPNMLISTVIEW; 
	*/

	if(this->m_bLoad_Bin_List == false)
	{
		CString csData;

		if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1))		// old state : unchecked
			&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2))	// new state : checked
			)
		{
			this->m_bLoad_Bin_List = true;
			this->m_ctr_lscBin_List.SetCheck(pNMLV->iItem, FALSE);//Ϊ�ý��汣�֡���ѡ�С�
			//this->m_ctr_lscBin_List.SetCheck(pNMLV->iItem, TRUE);//Ϊ�ý��汣�֡���ѡ�С�
			this->m_bLoad_Bin_List = false;
			//		csData.Format(_T("Item %d is checked"), pNMLV->iItem);
			//		::AfxMessageBox(csData);
		}
		else if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2))	// old state : checked
			&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1))	// new state : unchecked
			)
		{
			this->m_bLoad_Bin_List = true;
			this->m_ctr_lscBin_List.SetCheck(pNMLV->iItem, TRUE);//Ϊ�ý��汣�֡���ѡ�С�
			//this->m_ctr_lscBin_List.SetCheck(pNMLV->iItem, FALSE);//Ϊ�ý��汣�֡���ѡ�С�
			this->m_bLoad_Bin_List = false;
			//		csData.Format(_T("Item %d is unchecked"), pNMLV->iItem);
			//		::AfxMessageBox(csData);
		}
		else
		{
			//		csData.Format(_T("Item %d does't change the check-status"), pNMLV->iItem);
			//		::AfxMessageBox(csData);
		}
	}

	*pResult = 0;
}

void CDlg_Example::OnBnClickedButtonSetScatFile()
{
	FILE * fpOpen_File = NULL;
	CString csFilter, csLast_Path, csDef_Ext;
	BOOL bIn_Out;

	try
	{
		//���߲�ͬ�Ĳ�����������ļ����ͻ��в�ͬ
		if((this->m_iPlatform_Curr == PF_QUALCOMM) && (this->m_iCtrl_OP_Option_S == QCMM_OPTION_BACKUP_ONLY))
		{
			csDef_Ext.Format(_T(""));
			csFilter.Format(_T("QCN Files (*.qcn;*.qqb)|*.qcn;*.qqb|All Files (*.*)|*.*||"));
			bIn_Out = FALSE;
			csLast_Path = this->m_csQCN_Temp_File_Path;
		}
		else if((this->m_iPlatform_Curr == PF_QUALCOMM) && (this->m_iCtrl_OP_Option_S == QCMM_OPTION_RESTORE_ONLY))
		{
			csDef_Ext.Format(_T(""));
			csFilter.Format(_T("QCN Files (*.qcn;*.qqb)|*.qcn;*.qqb|All Files (*.*)|*.*||"));
			bIn_Out = TRUE;
			csLast_Path = this->m_csQCN_Temp_File_Path;
		}
		else
		{
			//ע�⡰*.txt;*.cfg|���еġ�|���롰cfg���䲻���пո�
			csDef_Ext.Format(_T(""));
			csFilter.Format(_T("FW Files (*.txt;*.cfg;*.mbn;*.lod;*.fls;*.fwpkg;*.elf;*.zip;*.bin;*.bat;*.hex;*.pac;*.xml)|*.txt;*.cfg;*.mbn;*.elf;*.lod;*.fls;*.fwpkg;*.zip;*.bin;*.bat;*.hex;*.pac;*.xml|All Files (*.*)|*.*||"));
			bIn_Out = TRUE;
			csLast_Path = this->m_csScat_Cfg_File_Path;
		}
		
		//�Ի���
		CFileDialog dlg(bIn_Out, csDef_Ext, csLast_Path, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, csFilter);

		if(dlg.DoModal() != IDOK)
		{
			throw(0);
		}

		//������ļ�·��
		if((this->m_iPlatform_Curr == PF_QUALCOMM) &&
			((this->m_iCtrl_OP_Option_S == QCMM_OPTION_BACKUP_ONLY) || (this->m_iCtrl_OP_Option_S == QCMM_OPTION_RESTORE_ONLY))
			)
		{
			//����QCN·��
			this->m_csQCN_Temp_File_Path.Format(_T("%s"), dlg.GetPathName());

			//����ǵ���QCN
			if(this->m_iCtrl_OP_Option_S == QCMM_OPTION_BACKUP_ONLY)
			{
				//����QCN�޷�Ԥ��ȷ���ļ���ʽ��������ȥ���ļ���ʽ��׺
				CString csTemp;
				//��ȡ�ļ�����չ��
				csTemp.Format(_T(".%s"), dlg.GetFileExt());
				//ȥ���ļ�����չ��
				this->m_csQCN_Temp_File_Path.Replace(csTemp, _T("\0"));
			}
		}
		else
		{
			this->m_csScat_Cfg_File_Path.Format(_T("%s"), dlg.GetPathName());
		}

		//
		this->Save_Config();

		//��ʼ������
		this->INIT();
	}
	catch(...)
	{
		return;
	}

	return;
}

void CDlg_Example::Add_ListCtrl_Bin_List_MTK(const ROM_INFO_EX str_Rom_Info_Ex)
{
	wchar_t * wcpData = NULL;
	int iTemp, i, j;
	CString csData, csDir;

	try
	{
		switch(this->m_cla_Flash_OP_Main.m_iFw_Type)
		{
		case FW_STD://��׼��MTK FW��
			break;
		case FW_APP://ֻ����app�ļ���ֻ����ѡ�е�app�ļ�
			{
				if(str_Rom_Info_Ex.enable != _TRUE)
				{
					return;
				}
			}
			break;
		default:
			return;
		}

		//
		i = strlen(str_Rom_Info_Ex.name);
		j = strlen(str_Rom_Info_Ex.filepath);

		//�����ڴ�
		this->m_cla_Flash_OP_Main.m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, (i + j));

		//BIN�ļ���
		this->m_cla_Flash_OP_Main.m_cla_GFC.MultiByte_To_WideChar(str_Rom_Info_Ex.name, wcpData, &iTemp);

		//��ǰlist�е�index��
		i = this->m_ctr_lscBin_List.GetItemCount();

		//����µ�һ�У�BIN�ļ�����
		j = this->m_ctr_lscBin_List.InsertItem(LVIF_TEXT|LVIF_STATE, i, wcpData,
			LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
		
		if((j < 0) || (j != i))
		{
			::AfxMessageBox(_T("FAIL, Show Bin Path!"));
			throw(0);
		}
		
		//BIN�ļ����ġ�CheckBox��
		this->m_ctr_lscBin_List.SetCheck(j, str_Rom_Info_Ex.enable);

		//BIN�ļ�·��
		switch(this->m_cla_Flash_OP_Main.m_iFw_Type)
		{
		case FW_STD://��׼��MTK FW��
			{
				this->m_cla_Flash_OP_Main.m_cla_GFC.MultiByte_To_WideChar(str_Rom_Info_Ex.filepath, wcpData, &iTemp);
				this->m_ctr_lscBin_List.SetItemText(j, 1, wcpData);
			}
			break;
		case FW_APP://ֻ����app�ļ���ֻ����ѡ�е�app�ļ�
			{
				this->m_cla_Flash_OP_Main.m_cla_GFC.Get_File_Directory(this->m_csScat_Cfg_File_Path, &csDir);
				csData.Format(_T("%s\\%s"), csDir, wcpData);
				//
				this->m_ctr_lscBin_List.SetItemText(j, 1, csData);
			}
			break;
		default:
			return;
		}


		//
		CSize ss(0, (j + 1));
		this->m_ctr_lscBin_List.Scroll(ss);
	}
	catch(...)
	{
		//�ͷ��ڴ�
		this->m_cla_Flash_OP_Main.m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		return;
	}

	//�ͷ��ڴ�
	this->m_cla_Flash_OP_Main.m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	return;
}

void CDlg_Example::Load_View_Bin_List_MTK(void)
{
	this->m_bLoad_Bin_List = true;

	//��ʾscat�ļ�
	this->Add_ListCtrl_Bin_List_Data(_T("SCAT"), this->m_csScat_Cfg_File_Path, true);

	//��ʾDA�ļ�
	this->Add_ListCtrl_Bin_List_Data(_T("DA"), this->m_csDA_File_Path, true);

	//��ʾMTK������е���������BIN�ļ�
	for(int i=0; i<this->m_cla_Flash_OP_Main.m_usROMcount; i++)
	{
		this->Add_ListCtrl_Bin_List_MTK(this->m_cla_Flash_OP_Main.m_strr_Rom_Info_Ex[i]);
	}

	this->m_bLoad_Bin_List = false;
}

bool CDlg_Example::Load_All_DL_Files_MTK(void)
{
	CString csNew_App_Scat_Cfg_File_Path, csData;

	try
	{	
		this->m_cla_Flash_OP_Main.m_iQFLASH_CMD_MTK = m_iQFLASH_CMD; 

		//����DA�ļ������뵽DA_HANDLE
		//DA�ļ�·��
		this->m_csDA_File_Path.Format(_T("%s\\MTK_AllInOne_DA.bin"), this->m_csCurr_Exe_Path);
		//
		if(this->m_cla_Flash_OP_Main.LOAD_DA_FILE(this->m_csDA_File_Path) == false)
		{
			this->Show_Status(this->m_cla_Flash_OP_Main.m_csError_Code);
			throw(0);
		}


		//�ж�SCAT�ļ���STD�������APP���
		if(this->m_cla_Flash_OP_Main.JUDGE_SCAT_FILE(this->m_csScat_Cfg_File_Path, &this->m_cla_Flash_OP_Main.m_iFw_Type) == false)
		{
			this->Show_Status(this->m_cla_Flash_OP_Main.m_csError_Code);
			throw(0);
		}


		//�����APP�������Ҫ�齨������APP������ذ�
		if(this->m_cla_Flash_OP_Main.m_iFw_Type == FW_APP)
		{
			//����OCPU����ѡ�������
			this->m_ctr_staOCPU_Name_S.ShowWindow(SW_SHOW);
			this->m_ctr_cbxOCPU_Name_S.ShowWindow(SW_SHOW);
			//��ȡOCPU����ѡ���������
			this->m_ctr_cbxOCPU_Name_S.SetCurSel(this->m_iOCPU_Name_S);//�����ʱOCPU����û��ˢ�µ����
			this->m_ctr_cbxOCPU_Name_S.GetWindowTextW(csData);

			//�齨app������FW��
			if(this->m_cla_Flash_OP_Main.SETUP_APP_FULL_FW_FILE(this->m_csScat_Cfg_File_Path, csData, &csNew_App_Scat_Cfg_File_Path) == false)
			{
				this->Show_Status(this->m_cla_Flash_OP_Main.m_csError_Code);
				throw(0);
			}

			//����app����Scat�ļ������뵽DL_HANDLE
			if(this->m_cla_Flash_OP_Main.LOAD_SCAT_FILE(csNew_App_Scat_Cfg_File_Path) == false)
			{
				this->Show_Status(this->m_cla_Flash_OP_Main.m_csError_Code);
				throw(0);
			}
		}
		else
		{
			//����OCPU����ѡ���ҡ�
			this->m_ctr_staOCPU_Name_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxOCPU_Name_S.ShowWindow(SW_HIDE);

			//����Scat�ļ������뵽DL_HANDLE
			if(this->m_cla_Flash_OP_Main.LOAD_SCAT_FILE(this->m_csScat_Cfg_File_Path) == false)
			{
				this->Show_Status(this->m_cla_Flash_OP_Main.m_csError_Code);
				throw(0);
			}
		}


		//�������͡�Flash��Ϣ��ȷ��Flash�Ĳ�������
		if(this->m_cla_Flash_OP_Main.Load_Bin_File_Product_Flash_Name() == false)
		{
			this->Show_Status(this->m_cla_Flash_OP_Main.m_csError_Code);
			throw(0);
		}

		//���ݻ��ͣ�����������صĻ��Ͳ���
		this->Load_Flash_Product_Params(this->m_cla_Flash_OP_Main.m_csBin_Product);

		/*
		//�ⲿFlash��Ҫ���Flash�ͺţ�6252D��6250D��6250M����Flash��6252��6250A����Flash��
		if(this->m_bInside_Flash == false)
		{
			//�ж�Flash��Ϣ
			if(this->m_cla_Flash_OP_Main.m_iBin_Flash == NDF_FLASH)
			{
				this->Show_Status(_T("Error, Bin file does not have flash name!"));
				throw(0);
			}
		}
		*/

		//����һ����������BIN�ļ���Ϣ����顢�趨״̬
		if(!this->m_cla_Flash_OP_Main.LOAD_BIN_FILE())
		{
			this->Show_Status(this->m_cla_Flash_OP_Main.m_csError_Code);
			throw(0);
		}

		//���������Ҫ���ص�BIN�ļ���������ʾ����BIN�嵥��
		this->Load_View_Bin_List_MTK();		
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CDlg_Example::Load_All_DL_Files_MTK_NB(void)
{
	CString csNew_App_Scat_Cfg_File_Path, csData;

	try
	{	
		//����DA�ļ������뵽DA_HANDLE
		//20190910
		this->Init_Progress_Ctrl(PF_MTK, -1);

		//DA�ļ�·��
		m_cla_ARM_OP_NB_Main.LoadFirst();

		this->m_csDA_File_Path.Format(_T("%s\\MTK_AllInOne_DA_NB.bin"), this->m_csCurr_Exe_Path);

		CString csLog_File_Path;
		csLog_File_Path.Format(_T("%s\\log\\MTK\\debug.txt"), this->m_csCurr_Exe_Path);

		m_cla_ARM_OP_NB_Main.LoadInit(1,&m_ctr_pgrProgress_Main,m_csDA_File_Path,this->m_csScat_Cfg_File_Path,csLog_File_Path);
		
		this->m_bLoad_Bin_List = true;


		//��ʾscat�ļ�
		//this->Add_ListCtrl_Bin_List_Data(_T("SCAT"), this->m_csScat_Cfg_File_Path, true);

		//��ʾDA�ļ�
		this->Add_ListCtrl_Bin_List_Data(_T("DA"), this->m_csDA_File_Path, true);

		CString strFilePath;
		int find =0;
		find=this->m_csScat_Cfg_File_Path.ReverseFind('\\');
		strFilePath = this->m_csScat_Cfg_File_Path.Left(find+1);

		FindBinFile(strFilePath);

		this->m_bLoad_Bin_List = false;
	}
	catch(...)
	{
		return false;
	}

	return true;
}

//
bool CDlg_Example::Load_All_DL_Files_MTK_5G(void)
{
	CString strDir_Path;
	CString strFilePath;
	int find = 0;
	try
	{
		//20190910
		this->Init_Progress_Ctrl(PF_MTK, -1);

		//..\\download_agent\\flash.xml
		//MT6880_openwrt_scatter.xml
		m_cla_ARM_OP_5G_Main.m_csCurr_Exe_Path = m_csCurr_Exe_Path;
		m_cla_ARM_OP_5G_Main.m_csFlash_Path = this->m_csScat_Cfg_File_Path;
		
		find = this->m_csScat_Cfg_File_Path.Find(_T("download_agent"));
		if (NULL != find)
		{
			strFilePath = this->m_csScat_Cfg_File_Path.Left(find);

			strDir_Path = strFilePath;

			strFilePath += _T("MT6880_openwrt_scatter.xml");

			m_cla_ARM_OP_5G_Main.Load_Scatter_Xml_Data(strFilePath, strDir_Path);
		}
		else
		{
			return false;
		}
		
		

	}
	catch (...)
	{
		return false;
	}

	return true;
}




bool CDlg_Example::FindBinFile(CString lpPath)
{
	CString strSubPath,strNewPath;

	//WIN32_FIND_DATA�ṹ������һ����FindFirstFile, 
	//FindFirstFileEx, ��FindNextFile�������ҵ����ļ���Ϣ
	WIN32_FIND_DATA FindFileData;

	//ͨ��FindFirstFile()�������ݵ�ǰ���ļ����·��
	//���Ҹ��ļ����Ѵ������ļ���������Զ�ȡ��WIN32_FIND_DATA�ṹ��ȥ
	strNewPath.Format(_T("%s*.*"),lpPath);
	HANDLE hFind=::FindFirstFile(strNewPath,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)    
	{
		return false;  //���hFind�ľ��ֵ��Ч������0
	}

	while(TRUE)
	{
		//��dwFileAttributes��FILE_ATTRIBUTE_DIRECTORY��λ"��"�������ж����ҵ�����Ŀ�ǲ����ļ��У�
		//��γ����Ŀ���ǲ����ļ��������ļ����е�����
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!='.')
			{	
				strSubPath.Format(_T("%s%s\\"),lpPath,FindFileData.cFileName);				
			}
		}
		else
		{
			strSubPath.Format(_T("%s%s"),lpPath,FindFileData.cFileName);	
			this->Add_ListCtrl_Bin_List_Data(_T("Firmware File"), strSubPath, true);
		}
		if(!FindNextFile(hFind,&FindFileData))   
			break;//���û���ҵ���һ���ļ�����������ѭ��
	}
	FindClose(hFind);

	return true;
}

//*****************************************************************
LRESULT CDlg_Example::OnGetMsg_Show_Dos(WPARAM wParam, LPARAM lParam)
{
	CString strData;
	strData.Format(_T("%s\r\n"),(wchar_t *)wParam);
	::WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);
	return LRESULT();
}
void CDlg_Example::Show_Dos(const wchar_t * wcpStatus)
{
	::SendMessage(this->m_hWnd, WM_STATUS_DOS, (WPARAM)wcpStatus, NULL);
}
//*****************************************************************


LRESULT CDlg_Example::OnGetMsg_Show_Status(WPARAM wParam, LPARAM lParam)
{
	this->m_ctr_edtStatus_Main.SetWindowTextW((wchar_t *)wParam);

	return LRESULT();
}

void CDlg_Example::Show_Status(const wchar_t * wcpStatus)
{
	::SendMessage(this->m_hWnd, WM_STATUS_MSG, (WPARAM)wcpStatus, NULL);
}

LRESULT CDlg_Example::OnGetMsg_Show_Status2(WPARAM wParam, LPARAM lParam)
{
	this->m_ctr_edtStatus_Main2.SetWindowTextW((wchar_t *)wParam);

	return LRESULT();
}

void CDlg_Example::Show_Status2(const wchar_t * wcpStatus)
{
	::SendMessage(this->m_hWnd, WM_STATUS_MSG_2, (WPARAM)wcpStatus, NULL);
}



void CDlg_Example::Load_Flash_Option_Params(void)
{
	//�����á�����������
	//COM�ں�
	if((this->m_iPlatform_Curr == PF_MTK) && (this->m_cla_Flash_OP_Main.m_iComm_Method == IN_USB))
	{
		if (BB_MT2625 != m_iBBchip_Curr)
		{
			this->m_cla_Flash_OP_Main.m_iCom_Port = 0;
		}		
		else
		{

		}
	}
	else
	{
		//20180503
		if(1== this->m_iQFLASH_CMD )
		{
			this->m_cla_Flash_OP_Main.m_iCom_Port = this->iCmdComPort;
		}
		else
		{
			CString csCOM;
			this->m_ctr_cbxCom_Main.GetWindowTextW(csCOM);
			this->m_cla_Flash_OP_Main.m_iCom_Port = _wtoi(csCOM);
		}
		this->m_cla_ARM_OP_QCM_Main.m_iCom_Port = this->m_cla_Flash_OP_Main.m_iCom_Port;
		this->m_cla_ARM_OP_RDA_Main.m_iCom_Port = this->m_cla_Flash_OP_Main.m_iCom_Port;
		this->m_cla_ARM_OP_HUAWEI_Main.m_iCom_Port = this->m_cla_Flash_OP_Main.m_iCom_Port;
		this->m_cla_ARM_OP_EIGEN_Main.m_iCom_Port = this->m_cla_Flash_OP_Main.m_iCom_Port;
		this->m_cla_ARM_OP_ASR_Main.m_iCom_Port = this->m_cla_Flash_OP_Main.m_iCom_Port;
		this->m_cla_ARM_OP_BEKEN_Main.m_iCom_Port = this->m_cla_Flash_OP_Main.m_iCom_Port;
		//20191119
		this->m_cla_ARM_OP_HUAWEI_Main.m_cla_Com_Port.m_iPort = this->m_cla_Flash_OP_Main.m_iCom_Port;
		this->m_cla_ARM_OP_HUAWEI_Main.m_cla_Com_Port.m_iBaudRate = 9600;

		this->m_cla_ARM_OP_NB_Main.m_iCom_Port = this->m_cla_Flash_OP_Main.m_iCom_Port;
		this->m_cla_ARM_OP_5G_Main.m_iCom_Port = this->m_cla_Flash_OP_Main.m_iCom_Port;

		this->m_cla_ARM_OP_SPREADTRUM_Main.m_iCom_Port = this->m_cla_Flash_OP_Main.m_iCom_Port;

	}

	//20180503
	if(1== this->m_iQFLASH_CMD )
	{
		this->m_cla_ARM_OP_QCM_Main.m_iBaud_Rate = this->iCmdBd;
		this->m_cla_ARM_OP_RDA_Main.m_iBaud_Rate = this->iCmdBd;
		switch (this->iCmdBd)
		{
			case 9600:this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_9600;break;
			case 19200:this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_19200;break;
			case 38400:this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_38400;break;
			case 57600:this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_57600;break;
			case 115200:this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_115200;break;
			case 230400:this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_230400;break;
			case 460800:this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_460800;break;
			case 921600:this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_921600;break;
			default:this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_460800;break;

		}

	}
	else
	{
		//������
		switch(this->m_iBR_Main_Idx)
		{
		case 0:
			this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_9600;
			this->m_cla_ARM_OP_QCM_Main.m_iBaud_Rate = 9600;
			this->m_cla_ARM_OP_RDA_Main.m_iBaud_Rate = 9600;
			this->m_cla_ARM_OP_EIGEN_Main.m_iBaud_Rate = 9600;
			this->m_cla_ARM_OP_NB_Main.m_iBaud_Rate = 115200;
			this->m_cla_ARM_OP_5G_Main.m_iBaud_Rate = 115200;
			break;
		case 1:
			this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_19200;
			this->m_cla_ARM_OP_QCM_Main.m_iBaud_Rate = 19200;
			this->m_cla_ARM_OP_RDA_Main.m_iBaud_Rate = 19200;
			this->m_cla_ARM_OP_EIGEN_Main.m_iBaud_Rate = 19200;
			this->m_cla_ARM_OP_NB_Main.m_iBaud_Rate = 115200;
			this->m_cla_ARM_OP_5G_Main.m_iBaud_Rate = 115200;

			break;
		case 2:
			this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_38400;
			this->m_cla_ARM_OP_QCM_Main.m_iBaud_Rate = 38400;
			this->m_cla_ARM_OP_RDA_Main.m_iBaud_Rate = 38400;
			this->m_cla_ARM_OP_EIGEN_Main.m_iBaud_Rate = 38400;
			this->m_cla_ARM_OP_NB_Main.m_iBaud_Rate = 115200;
			this->m_cla_ARM_OP_5G_Main.m_iBaud_Rate = 115200;
			break;
		case 3:
			this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_57600;
			this->m_cla_ARM_OP_QCM_Main.m_iBaud_Rate = 57600;
			this->m_cla_ARM_OP_RDA_Main.m_iBaud_Rate = 57600;
			this->m_cla_ARM_OP_EIGEN_Main.m_iBaud_Rate = 57600;
			this->m_cla_ARM_OP_NB_Main.m_iBaud_Rate = 115200;
			this->m_cla_ARM_OP_5G_Main.m_iBaud_Rate = 115200;
			break;
		case 4:
			this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_115200;
			this->m_cla_ARM_OP_QCM_Main.m_iBaud_Rate = 115200;
			this->m_cla_ARM_OP_RDA_Main.m_iBaud_Rate = 115200;
			this->m_cla_ARM_OP_EIGEN_Main.m_iBaud_Rate = 115200;
			this->m_cla_ARM_OP_NB_Main.m_iBaud_Rate = 115200;
			this->m_cla_ARM_OP_5G_Main.m_iBaud_Rate = 115200;
			break;
		case 5:
			this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_230400;
			this->m_cla_ARM_OP_QCM_Main.m_iBaud_Rate = 230400;
			this->m_cla_ARM_OP_RDA_Main.m_iBaud_Rate = 230400;
			this->m_cla_ARM_OP_EIGEN_Main.m_iBaud_Rate = 230400;
			this->m_cla_ARM_OP_NB_Main.m_iBaud_Rate = 921600;
			this->m_cla_ARM_OP_5G_Main.m_iBaud_Rate = 921600;
			break;
		case 6:
			this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_460800;
			this->m_cla_ARM_OP_QCM_Main.m_iBaud_Rate = 460800;
			this->m_cla_ARM_OP_RDA_Main.m_iBaud_Rate = 460800;
			this->m_cla_ARM_OP_EIGEN_Main.m_iBaud_Rate = 460800;
			this->m_cla_ARM_OP_NB_Main.m_iBaud_Rate = 921600;
			this->m_cla_ARM_OP_5G_Main.m_iBaud_Rate = 921600;
			break;
		case 7:
			this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_921600;
			this->m_cla_ARM_OP_QCM_Main.m_iBaud_Rate = 921600;
			this->m_cla_ARM_OP_RDA_Main.m_iBaud_Rate = 921600;
			this->m_cla_ARM_OP_EIGEN_Main.m_iBaud_Rate = 921600;
			this->m_cla_ARM_OP_NB_Main.m_iBaud_Rate = 921600;
			this->m_cla_ARM_OP_5G_Main.m_iBaud_Rate = 921600;
			break;
		default:
			this->m_cla_Flash_OP_Main.m_eu_BR_UART = UART_BAUD_460800;
			this->m_cla_ARM_OP_QCM_Main.m_iBaud_Rate = 460800;
			this->m_cla_ARM_OP_RDA_Main.m_iBaud_Rate = 460800;
			this->m_cla_ARM_OP_NB_Main.m_iBaud_Rate = 921600;
			this->m_cla_ARM_OP_5G_Main.m_iBaud_Rate = 921600;
			break;
		}
	}



	//�����ͨ���ڲ���
	this->m_cla_ARM_OP_QCM_Main.Load_Com_Port_Params();

	//RDA 8955 MC25��������
	//�ĵ����ذ�ť��Ӧ֮��������
	//this->m_cla_ARM_OP_RDA_Main.Load_Com_Conn();

}

void CDlg_Example::Load_Flash_Product_Params(const wchar_t * wcp_Product)
{
	//����NV�ļ��ı����б�
	this->m_iNv_Type = this->m_cla_iniParams.ReadInt(wcp_Product, _T("NV_TYPE"), 1);
	//�ڡ���Flash
	this->m_bInside_Flash = this->m_cla_iniParams.ReadB(wcp_Product, _T("INSIDE_FLASH"), false);

	//ʹ�����ֿ����أ�UART�ڡ�USB��
	//�����ļ������ã����ڴ˴����� 20191130

	//this->m_cla_Flash_OP_Main.m_iComm_Method = this->m_cla_iniParams.ReadInt(wcp_Product, _T("COMM_METHOD"), IN_UART);	
	
	
	//BB_CHIP
	this->m_cla_Flash_OP_Main.m_eu_BB_Chip = (BBCHIP_TYPE)this->m_cla_iniParams.ReadInt(wcp_Product, _T("BB_CHIP"), AUTO_DETECT_BBCHIP);
	//EXT_CLOCK
	this->m_cla_Flash_OP_Main.m_eu_Ext_Clock = (EXT_CLOCK)this->m_cla_iniParams.ReadInt(wcp_Product, _T("EXT_CLOCK"), AUTO_DETECT_EXT_CLOCK);
}

void CDlg_Example::OnBnClickedButtonStart()
{
	CString strData;

	if(!this->m_ctr_btnStart.IsWindowEnabled())
	{
		return;
	}

	//20200623
	if(PF_QUALCOMM == this->m_iPlatform_Curr)
	{	
	// 	Ŀǰϵͳ������Lenovo��Ŀʱ��Ҫʵ�������̼��Ĺ��ܡ� �������ں�QFlash�����߿��ܻ��г�ͻ�� �������޸�QFlash���������������ͻ��
	// 	Ŀǰ���ǵ��뷨�ǽ���Global name event��ʵ�ֵġ�
	// 	������ʼǰ�� ʹ��CreateEvent������Global��event��
	// 	��������ɹ��� ���Ƿ���event�������ں����Ѵ��ڣ� ��ô˵���������������� ��ʱ�� ������Ҫ�������������
	// 	��������ɹ��� �ҷ���event�������ں��в����ڣ� ��ôִ�����ǵ��������̡����������� �ر�event��
		//-------------------------------------------------------------
		SECURITY_DESCRIPTOR secutityDese;
		InitializeSecurityDescriptor(&secutityDese, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&secutityDese, TRUE, NULL, FALSE);

		SECURITY_ATTRIBUTES securityAttr;
		securityAttr.nLength = sizeof SECURITY_ATTRIBUTES;
		securityAttr.bInheritHandle = FALSE;
		securityAttr.lpSecurityDescriptor = &secutityDese;

		m_hLenovoHandle = ::CreateEvent(&securityAttr, FALSE, FALSE, _T("Global\\{82F34418-8931-43E4-AF1D-CF59E4EFA80C}"));

		if (m_hLenovoHandle != NULL)
		{
			// the name event already exist, just exit this update
			DWORD LastError = GetLastError();

			if (LastError == ERROR_ALREADY_EXISTS)
			{
				strData.Format(_T("other process updating firmware, so we exist-%d"),LastError);
				this->Save_Trace_Log_File(strData);
				this->Show_Status(strData);

				::CloseHandle(m_hLenovoHandle);

				AfxMessageBox(strData);
				return;

			
			}
			else // the name event not exist
			{
				//������������
				strData.Format(_T("LenovoHandle OK-%d"),LastError);
				this->Save_Trace_Log_File(strData);
				//AfxMessageBox(strData);
			}
		}
		else
		{
			// failed to create name event
			strData.Format(_T("CreateEvent failed, error code %d"),GetLastError());
			this->Save_Trace_Log_File(strData);
			this->Show_Status(strData);
			return;

			//AfxMessageBox(strData);
		}
	}
	//--------------------------------------------------------------------------------


	if((PF_QUALCOMM == this->m_iPlatform_Curr) && (MDM9X06 == this->m_cla_ARM_OP_QCM_Main.m_eu_BB_Chip))
	{
		this->m_cla_ARM_OP_QCM_Main.m_bAUTOSEL_MBN = FALSE;
		//
		CString strD;
		//20200622 ��ͨ��BG96ƽ̨�Ƿ���Ҫ������Ӫ�̵�MBN�ļ�
		CActivateMbnOpName ActivateMbnOpName;
		//���ý���
		if(ActivateMbnOpName.DoModal() == IDOK)
		{
			//
			if (TRUE == ActivateMbnOpName.m_bAUTOSEL_MBN)
			{
				this->m_cla_ARM_OP_QCM_Main.m_bAUTOSEL_MBN = TRUE;
				strD.Format(_T("AUTOSEL_MBN:1"));
			} 
			else
			{
				this->m_cla_ARM_OP_QCM_Main.m_bAUTOSEL_MBN = FALSE;
				strD.Format(_T("AUTOSEL_MBN:0"));
			}			   				
		}
		else
		{
			this->m_cla_ARM_OP_QCM_Main.m_bAUTOSEL_MBN = FALSE;
			strD.Format(_T("Not Set AUTOSEL_MBN"));	
		}
		this->Save_Trace_Log_File(strD);

	}


	if (1== m_iQFLASH_CMD)
	{
		CString strData;
		strData.Format(_T("DL Start....\r\n"));
		WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);

// 		CWnd*  hDwnd=FindWindow(_T("ConsoleWindowClass"),NULL);  
// 		HWND hCMDwnd;
// 		hCMDwnd = hDwnd->GetSafeHwnd();
// 		wchar_t wch[]=L"echo DL Start��Please Reset Module....\r\n";
// 		for(size_t i=0;i<wcslen(wch);i++)
// 		{
// 			::SendMessage(hCMDwnd,WM_CHAR,wch[i],NULL);
// 		}
	}
	else
	{

		//
		if((PF_ASR == this->m_iPlatform_Curr)&&(ASR3601 == this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip))
		{
		}
		else
		{
			this->UpdateData(TRUE);
		}
		
	}
	

	//ˢ��COM�ڵȲ���
	this->Load_Flash_Option_Params();

	//RDA���� 8955
//	if(this->m_iPlatform_Curr == PF_RDA)
//	{
//		this->m_cla_ARM_OP_RDA_Main.Load_Com_Conn();
//	}	

	//20191205 3601ƽ̨ʹ��
	this->m_ASR_DL_OK = false;

	//
	this->m_hThread_Handle = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)Operate_1,
		(void *)this,
		0,
		&this->m_dwThread_ID);
}

void Operate_1(LPVOID x)
{
	CDlg_Example *pSv = (CDlg_Example *)x;

	pSv->DO_DL_OP();

	//20200623
	if(PF_QUALCOMM == pSv->m_iPlatform_Curr)
	{
		::CloseHandle(pSv->m_hLenovoHandle);
	}
}

void CDlg_Example::Set_View_Ctrl_State(int i_State)
{
	if(i_State == 0)//�����ʼ����
	{
		this->m_ctr_cbxCom_Main.EnableWindow(FALSE);
		this->m_ctr_cbxBR_Main.EnableWindow(FALSE);
		this->m_ctr_btnSet_Scat_Cfg_File.EnableWindow(FALSE);
		this->m_ctr_btnStart.EnableWindow(FALSE);
		this->m_ctr_btnStop.EnableWindow(FALSE);

		this->m_ctr_cbxCtrl_Dl_Type_S.EnableWindow(FALSE);
		this->m_ctr_cbxCtrl_OP_Option_S.EnableWindow(FALSE);
		this->m_ctr_cekGet_Debug_Log.EnableWindow(FALSE);
		this->m_ctr_cbxCtrl_DLoad_Port_S.EnableWindow(FALSE);
		this->m_ctr_cbxOCPU_Name_S.EnableWindow(FALSE);
	}
	else if(i_State == 1)//����IDLE����ʼ��PASS
	{
		switch(this->m_iPlatform_Curr)
		{
		case PF_MTK:
			if (BB_MT2625 != m_iBBchip_Curr)
			{
				this->m_ctr_cbxCom_Main.EnableWindow((this->m_cla_Flash_OP_Main.m_iComm_Method == IN_USB)? FALSE : TRUE);
				this->m_ctr_cbxBR_Main.EnableWindow(TRUE);
			}
			else
			{
				//...
				this->m_ctr_cbxBR_Main.EnableWindow(TRUE);

			}


			break;
		case PF_INTEL:
		//case PF_ASR:
			this->m_ctr_cbxCom_Main.EnableWindow(FALSE);
			this->m_ctr_cbxBR_Main.EnableWindow(FALSE);

// 			this->m_ctr_cbxCom_Main.ShowWindow(SW_HIDE);
// 			this->m_ctr_cbxBR_Main.ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_PORT)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_BAUD)->ShowWindow(SW_HIDE);

			break;
		default://����ƽ̨
			this->m_ctr_cbxCom_Main.EnableWindow(TRUE);
			this->m_ctr_cbxBR_Main.EnableWindow(TRUE);
			break;
		}

		this->m_ctr_btnSet_Scat_Cfg_File.EnableWindow(TRUE);

//         if((PF_ASR == this->m_iPlatform_Curr)&&(ASR3601 == this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip))
// 		{
// 			this->m_ctr_btnStart.EnableWindow(FALSE);
// 		}
// 		else
		{
			this->m_ctr_btnStart.EnableWindow(TRUE);
		}
		

		this->m_ctr_btnStop.EnableWindow(FALSE);

		this->m_ctr_cbxCtrl_Dl_Type_S.EnableWindow(TRUE);
		this->m_ctr_cbxCtrl_OP_Option_S.EnableWindow(TRUE);
		this->m_ctr_cekGet_Debug_Log.EnableWindow(TRUE);
		this->m_ctr_cbxCtrl_DLoad_Port_S.EnableWindow(TRUE);
		this->m_ctr_cbxOCPU_Name_S.EnableWindow(TRUE);
	}
	else if(i_State == 2)//����IDLE����ʼ��FAIL
	{
		switch(this->m_iPlatform_Curr)
		{
		case PF_MTK:
			if (BB_MT2625 != m_iBBchip_Curr)
			{
				this->m_ctr_cbxCom_Main.EnableWindow((this->m_cla_Flash_OP_Main.m_iComm_Method == IN_USB)? FALSE : TRUE);
				this->m_ctr_cbxBR_Main.EnableWindow(TRUE);
			}
			else
			{
				this->m_ctr_cbxBR_Main.EnableWindow(TRUE);

			}
			break;
		case PF_INTEL:
		//case PF_ASR:
			this->m_ctr_cbxCom_Main.EnableWindow(FALSE);
			this->m_ctr_cbxBR_Main.EnableWindow(FALSE);
// 			this->m_ctr_cbxCom_Main.ShowWindow(SW_HIDE);
// 			this->m_ctr_cbxBR_Main.ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_PORT)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_BAUD)->ShowWindow(SW_HIDE);
			break;
		default://����ƽ̨
			this->m_ctr_cbxCom_Main.EnableWindow(TRUE);
			this->m_ctr_cbxBR_Main.EnableWindow(TRUE);
			break;
		}

		this->m_ctr_btnSet_Scat_Cfg_File.EnableWindow(TRUE);
		this->m_ctr_btnStart.EnableWindow(FALSE);
		this->m_ctr_btnStop.EnableWindow(FALSE);

		this->m_ctr_cbxCtrl_Dl_Type_S.EnableWindow(FALSE);
		this->m_ctr_cbxCtrl_OP_Option_S.EnableWindow(TRUE);
		this->m_ctr_cekGet_Debug_Log.EnableWindow(FALSE);
		this->m_ctr_cbxCtrl_DLoad_Port_S.EnableWindow(FALSE);
		this->m_ctr_cbxOCPU_Name_S.EnableWindow(TRUE);
	}
	else if(i_State == 3)//����������
	{
		this->m_ctr_cbxCom_Main.EnableWindow(FALSE);
		this->m_ctr_cbxBR_Main.EnableWindow(FALSE);
		this->m_ctr_btnSet_Scat_Cfg_File.EnableWindow(FALSE);
		this->m_ctr_btnStart.EnableWindow(FALSE);
		this->m_ctr_btnStop.EnableWindow(TRUE);

		this->m_ctr_cbxCtrl_Dl_Type_S.EnableWindow(FALSE);
		this->m_ctr_cbxCtrl_OP_Option_S.EnableWindow(FALSE);
		this->m_ctr_cekGet_Debug_Log.EnableWindow(FALSE);
		this->m_ctr_cbxCtrl_DLoad_Port_S.EnableWindow(FALSE);
		this->m_ctr_cbxOCPU_Name_S.EnableWindow(FALSE);
	}
}

void CDlg_Example::OnBnClickedButtonStop()
{
	this->m_cla_Flash_OP_Main.m_iStop_Flag = BOOT_STOP;
//	this->m_cla_ARM_OP_QCM_Main.m_iStop_Flag = 1;
	//ASRƽֹ̨ͣ����
	if(PF_ASR == this->m_iPlatform_Curr)
	{
		this->StopDownload();
	}

}

LRESULT CDlg_Example::OnGetMsg_Show_Progress(WPARAM wParam, LPARAM lParam)
{
	if(this->m_iPgr_Last == (int)wParam)
	{
		return LRESULT();
	}



	this->m_iPgr_Last = (int)wParam;

	int iTemp = this->m_ctr_pgrProgress_Main.GetPos();//��ǰ������״̬
	iTemp = iTemp % 100;//ȥ��ǰ����100�Ľ�����״̬����Ϊ����Ľ����������ǰٷֱ�
	iTemp = this->m_iPgr_Last - iTemp;//������Ҫ�����������ľ���ֵ

	if(iTemp > 0)
	{
		for(int a=0; a<iTemp; a++)
		{
			this->m_ctr_pgrProgress_Main.StepIt();
		}
	}

	return LRESULT();
}

void CDlg_Example::Reset_Progress_Ctrl(void)
{
	this->m_ctr_pgrProgress_Main.SetPos(0);
	this->m_iPgr_Last = 0;
}

void CDlg_Example::Init_Progress_Ctrl(int i_Platform, int i_OP_TYPE)
{
	int iPgr_Total;

	if (i_Platform == PF_MTK)
	{
		switch (this->m_iBBchip_Curr)
		{
		case BB_MT2625:
		{
			iPgr_Total = 100;
		}
		break;
		case BB_MT6880:
		{
			iPgr_Total = 100;
		}
		break;
		default:
		{
			switch (i_OP_TYPE)
			{
			case MTK_OPTION_DL_FORMAT:
				iPgr_Total = 400;
				break;
			case MTK_OPTION_DL_ONLY:
				iPgr_Total = 300;
				break;
			case MTK_OPTION_DL_CLEAR_NV:
				iPgr_Total = 300;
				break;
			default:
				iPgr_Total = 300;
				break;
			}
		}
		break;
		}
	}

	else if(i_Platform == PF_QUALCOMM)
	{
		iPgr_Total = 1000;
	}
	else if(i_Platform == PF_RDA)
	{
		iPgr_Total = 1000;
	}
	else if(i_Platform == PF_INTEL)
	{
		iPgr_Total = 1000;
	}
	else if(i_Platform == PF_HUAWEI)
	{
		iPgr_Total = 1000;
	}
	else if(i_Platform == PF_ASR)
	{
		iPgr_Total = 100;
	}
	else if(i_Platform == PF_EIGENCOMM)
	{
		iPgr_Total = 1000;
	}
	else if (i_Platform == PF_SPREADTRUM)
	{
		iPgr_Total = 100;
	}

	else if (i_Platform == PF_XINYI)
	{
		iPgr_Total = 100;
	}
	else if (i_Platform == PF_BEKEN)
	{
		iPgr_Total = 1000;
	}
	else
	{
		iPgr_Total = 0;
	}
	
	//��������ǰ״̬����
	this->Reset_Progress_Ctrl();

	this->m_ctr_pgrProgress_Main.SetRange(0, iPgr_Total);
	this->m_ctr_pgrProgress_Main.SetStep(1);
	this->m_iPgr_Last = 0;
}

void CDlg_Example::Set_View_Ctrl_Font(void)
{
	/*
	����˵��:
	nHeight         :����߶�.>0:����ĸ߶�ֵ;=0:�������ȱʡֱ.<0:��ֵ�ľ���ֵΪ�߶�.
	nWidth          :������.
	nEscapement     :�ı��е���б��.
	nOrientation    :�ַ����ߵ���б��.
	nWeight         :����Ĵ�ϸ.����:
	.FW_DONTCARE
	.FW_THIN
	.FW_EXTRALIGHT
	.....
	bItalic         :�����Ƿ�Ϊб��
	bUnderline      :�����Ƿ���»���
	cStrikeOut      :�����Ƿ��ɾ����
	nCharSet        :������ַ���
	.ANSI_CHARSET
	.DEFAULT_CHARSET
	.SYMBOL_CHARSET.....
	nOutPrecision   :�ַ����������
	nClipPrecision  :�ַ��ü��ľ���
	nQuality        :�ַ����������
	nPitchAndFamily :�ַ�����������(��λ˵�����,��λ˵���ַ���)
	lpszFacename    :��������
	*/
	this->m_cla_fontBtn_1.CreateFont(23, 9, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, FF_SWISS, _T("Arial"));

	this->m_cla_fontEdt_1.CreateFont(18, 8, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SWISS, _T("Arial"));


	this->m_cla_fontEdt_2.CreateFont(18, 8, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SWISS, _T("Arial"));

	//
	this->m_ctr_btnStart.SetFont(&this->m_cla_fontBtn_1);
	this->m_ctr_btnStop.SetFont(&this->m_cla_fontBtn_1);

	this->m_ctr_edtStatus_Main.SetFont(&this->m_cla_fontEdt_1);
	this->m_ctr_edtStatus_Main2.SetFont(&this->m_cla_fontEdt_1);
	this->m_ctr_edtTime_Main.SetFont(&this->m_cla_fontEdt_1);

	this->m_ctrlEditImei.SetFont(&this->m_cla_fontEdt_1);

}

void CDlg_Example::Clear_View_Bin_List(void)
{
	//����б�
	this->m_ctr_lscBin_List.DeleteAllItems();
}

HBRUSH CDlg_Example::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(pWnd == &this->m_ctr_edtStatus_Main)//��״̬��ʾ��
	{
		pDC->SetBkMode(TRANSPARENT);//���ñ���͸��������������ַ���ʱ�������ν�Ŀ����֣��������а׵ĵ�ɫ

		pDC->SetTextColor(this->m_colorEditText);//�ı��������ɫ

		/*
		HBRUSH newHbr = CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); 
		//  TODO:   Return   a   different   brush   if   the   default   is   not   desired 
		return newHbr;
		*/
	}
	if(pWnd == &this->m_ctr_edtStatus_Main2)//��״̬��ʾ��
	{
		pDC->SetBkMode(TRANSPARENT);//���ñ���͸��������������ַ���ʱ�������ν�Ŀ����֣��������а׵ĵ�ɫ

		pDC->SetTextColor(this->m_colorEditText);//�ı��������ɫ

		/*
		HBRUSH newHbr = CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); 
		//  TODO:   Return   a   different   brush   if   the   default   is   not   desired 
		return newHbr;
		*/
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CDlg_Example::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

	//�رմ���
	this->m_cla_ARM_OP_QCM_Main.COM_PORT_CLOSE();
	//�ر�Debug_Log�ļ�
	this->Close_Trace_Log_File();


	//����ȼ�
	::UnregisterHotKey(this->m_hWnd, HOTKEY_DLOAD_MSG);
	//::UnregisterHotKey(this->m_hWnd, HOTKEY_OPTION_MSG);
	::UnregisterHotKey(this->m_hWnd, HOTKEY_DEBUG_MSG);
	::UnregisterHotKey(this->m_hWnd, HOTKEY_DLOAD_PORT_MSG);
	::UnregisterHotKey(this->m_hWnd, HOTKEY_FIREHOSE_MSG);
	::UnregisterHotKey(this->m_hWnd, HOTKEY_NB_BC95_PARA_MSG);
	
}

LRESULT CDlg_Example::OnGetMsg_Ctrl_Colour(WPARAM wParam, LPARAM lParam)
{
	switch((int)wParam)
	{
	case COL_BLACK:
		this->m_colorEditText = RGB(0, 0, 0);
		break;
	case COL_RED:
		this->m_colorEditText = RGB(255, 0, 0);
		break;
	case COL_BLUE:
		this->m_colorEditText = RGB(0, 0, 217);
		break;
	default:
		return LRESULT();
	}

	//ˢ���µ���ɫ����
	// redraw edit control
	this->m_ctr_edtStatus_Main.SetRedraw(TRUE);
	// invalidate the entire control, force painting
	this->m_ctr_edtStatus_Main.Invalidate();
	this->m_ctr_edtStatus_Main.UpdateWindow();

	return LRESULT();
}

void CDlg_Example::Ctrl_Colour(int i_Colour)
{
	::SendMessage(this->m_hWnd, WM_COLOUR_MSG, (WPARAM)i_Colour, NULL);
}

void CDlg_Example::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	time_t time_Curr;
	CString csData;

	time(&time_Curr);

	switch(nIDEvent)
	{
	case 1://��1�š�time_Startʱ��
		csData.Format(_T("%d s"), (int)difftime(time_Curr, this->m_time_Start_1));
		this->Show_Time(csData);
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlg_Example::Show_Time(const wchar_t * wcp_Time)
{
	this->m_ctr_edtTime_Main.SetWindowTextW(wcp_Time);
}

void CDlg_Example::Reset_View(void)
{
	this->Show_Status(_T(""));
	this->Show_Time(_T(""));
	this->Reset_Progress_Ctrl();
	this->Ctrl_Colour(COL_BLACK);
}
//20181017
//ͨ������FirmwareͬĿ¼���Ƿ���BootLoader_XXXXX.bin�ļ���������APP����STD
bool CDlg_Example::FindBootLoaderFile(CString lpPath)
{
	CString strSubPath,strNewPath;
	CString  strName;
	bool bFind = false;

	//WIN32_FIND_DATA�ṹ������һ����FindFirstFile, 
	//FindFirstFileEx, ��FindNextFile�������ҵ����ļ���Ϣ
	WIN32_FIND_DATA FindFileData;

	//ͨ��FindFirstFile()�������ݵ�ǰ���ļ����·��
	//���Ҹ��ļ����Ѵ������ļ���������Զ�ȡ��WIN32_FIND_DATA�ṹ��ȥ
	strNewPath.Format(_T("%s*.*"),lpPath);
	HANDLE hFind=::FindFirstFile(strNewPath,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)    
	{
		return false;  //���hFind�ľ��ֵ��Ч������0
	}

	while(TRUE)
	{
		//��dwFileAttributes��FILE_ATTRIBUTE_DIRECTORY��λ"��"�������ж����ҵ�����Ŀ�ǲ����ļ��У�
		//��γ����Ŀ���ǲ����ļ��������ļ����е�����
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
	      //�������ļ��к���
		}
		else
		{		
			strName.Format(_T("%s"),FindFileData.cFileName);
			strName.MakeUpper();
			if (NULL != wcsstr(strName,_T("BOOTLOADER")))
			{
				//
				bFind = true;
				break;
			}			
		}
		if(!FindNextFile(hFind,&FindFileData))   			
		{
			bFind = false;
			break;//���û���ҵ���һ���ļ�����������ѭ��
		}
	}
	FindClose(hFind);
	if (true == bFind)
	{
		return true;
	} 
	else
	{
		return false;
	}

	
}
BOOL CDlg_Example::EditRegistry(IN HKEY hKey,IN LPCWSTR lpSubKey,IN REGSAM RegAccess,IN LPCWSTR lpValueName)
{
	HKEY hSubKey;
	LONG  lRet= 0;
	LPDWORD lpType=NULL;

	DWORD Size = 512;
	//��ע�����
	lRet = RegOpenKeyExW(hKey,lpSubKey,0,RegAccess,&hSubKey);
	if (ERROR_SUCCESS != lRet)
	{
		return FALSE;
	}
	//��Ŀ���
	wchar_t data[4096]; 
	DWORD lValueSize;
	CString   strOldPath,strNewPath;

	LONG lsRet = RegQueryValueExW(hSubKey,lpValueName,NULL,lpType,(LPBYTE)data,&lValueSize);

	strNewPath.Format(_T("%s\\HIS\\1\\;"),this->m_csCurr_Exe_Path);


	if (ERROR_SUCCESS ==lsRet)
	{
		strOldPath.Format(_T("%s"),data);
		
		if (strOldPath.Find(strNewPath)<0)
		{
			strNewPath.Format(_T("%s;%s\\HIS\\1\\;"),strOldPath,this->m_csCurr_Exe_Path);
		}
	}
		
	DWORD dwSize;
	dwSize = strNewPath.GetLength();

	//�޸�Ŀ���
	lsRet = RegSetValueExW(hSubKey,lpValueName,NULL,REG_EXPAND_SZ,(const BYTE*)strNewPath.GetBuffer(),dwSize*2);
	if (lsRet!=ERROR_SUCCESS)
	{
		return FALSE;
	}

	RegCloseKey(hSubKey);
	return TRUE;	
}


bool CDlg_Example::Get_Platform(const wchar_t * wcp_FW_File_Path, int * ip_Platform)
{
	int i;
	int iIdx = wcslen(wcp_FW_File_Path) - 1;
	CString csFormat,csData;

	CString csFW_PATH;
	csFW_PATH.Format(_T("%s"),wcp_FW_File_Path);
	csFW_PATH.MakeUpper();




	for(i=iIdx; i>=0; i--)
	{
		if(*(wcp_FW_File_Path + i) == '.')
		{
			break;
		}
	}

	if(i < 0)
	{
		return false;
	}
	else
	{
		csFormat.Format(_T("%s"), (wcp_FW_File_Path + i + 1));

		csFormat.MakeLower();

		if(wcscmp(csFormat, _T("txt")) == 0)
		{
			*ip_Platform = PF_MTK;
		}
		

		else if(wcscmp(csFormat, _T("cfg")) == 0)
		{
			if ((NULL != wcsstr(csFW_PATH,_T("BC20")))
				||(NULL != wcsstr(csFW_PATH,_T("BC26")))
				||(NULL != wcsstr(csFW_PATH,_T("BC66")))
				||(NULL != wcsstr(csFW_PATH,_T("BC950N")))
				||(NULL != wcsstr(csFW_PATH,_T("BC030"))))
			{
				*ip_Platform = PF_MTK;
				this->m_iBBchip_Curr = BB_MT2625;

			} 
			else
			{
				//*****************************************************************************
				//20181016 add ��Ҫ�����MTK��NB��OPEN CPU�Ļ���Firmware�޷���MTK��NB������
				if(false == FindBootLoaderFile(this->m_csScat_Cfg_File_Path))
				{
					//����OCPU����ѡ�������
					this->m_ctr_staOCPU_Name_S.ShowWindow(SW_SHOW);
					this->m_ctr_cbxOCPU_Name_S.ShowWindow(SW_SHOW);
					//��ȡOCPU����ѡ���������
					this->m_ctr_cbxOCPU_Name_S.SetCurSel(this->m_iOCPU_Name_S);//�����ʱOCPU����û��ˢ�µ����
					this->m_ctr_cbxOCPU_Name_S.GetWindowTextW(csData);
					if(wcsstr(csData,_T("MTK_NB"))!=NULL)
					{
						*ip_Platform = PF_MTK;
						this->m_iBBchip_Curr = BB_MT2625;
					}
					else
					{
						*ip_Platform = PF_MTK;
						this->m_iBBchip_Curr = BB_MT2503;
					}			
				}
				else
				{
					*ip_Platform = PF_MTK;
					this->m_iBBchip_Curr = BB_MT2503;
				}
			}
		}
		else if((wcscmp(csFormat, _T("mbn")) == 0)
			||(wcscmp(csFormat, _T("elf")) == 0))
		{
			*ip_Platform = PF_QUALCOMM;
		}
		else if(wcscmp(csFormat, _T("hex")) == 0)
		{
			this->m_bQDM002_flag  = false;
			this->m_bHoneywell_A4_flag = false;

			if (NULL != wcsstr(csFW_PATH,_T("CELLMFG")))
			{
				this->m_bHoneywell_A4_flag = true; //Honeywell_A4 ��Ŀר�õ�
			} 
			else  if (NULL != wcsstr(csFW_PATH,_T("QDM002")))
			{
				this->m_bQDM002_flag = true; //QDM002 ��Ŀר�õ�
			} 
			else
			{
				this->m_bQDM002_flag  = false;
				this->m_bHoneywell_A4_flag = false;
			}	
			*ip_Platform = PF_QUALCOMM;

		}
		else if(wcscmp(csFormat, _T("yaffs2")) == 0)
		{
			*ip_Platform = PF_QUALCOMM;
		}
		else if(wcscmp(csFormat, _T("img")) == 0)
		{
			*ip_Platform = PF_QUALCOMM;
		}
		else if(wcscmp(csFormat, _T("lod")) == 0)
		{
			*ip_Platform = PF_RDA;
		}
		else if(wcscmp(csFormat, _T("fls")) == 0)
		{
			*ip_Platform = PF_INTEL;
		}
		else if(wcscmp(csFormat, _T("ubi")) == 0)
		{
			*ip_Platform = PF_QUALCOMM;
		}
		else if(wcscmp(csFormat, _T("ubi")) == 0)
		{
			*ip_Platform = PF_QUALCOMM;
		}
		else if(wcscmp(csFormat, _T("fwpkg")) == 0)
		{
			*ip_Platform = PF_HUAWEI;
			//-----------------------------------------------------------------
			CStdioFile Ptfile;
			BYTE bTag[4] = { 0 };

			Ptfile.Open(wcp_FW_File_Path, CFile::typeBinary | CFile::modeRead);
			Ptfile.Seek(0, CFile::begin);
			Ptfile.Read(bTag, 4);
			//ͨ���̼��ļ���ǰ4���ֽ�ʶ������һ��оƬƽ̨�� 0X DF AD BE EF ʶ���Ǻ�˼B200оƬ
			//��˼�ʼ� 0X DF AD BE EF ���Ϊ DD AD BE EF 20210119
			if ((0xDD == bTag[0]) && (0xAD == bTag[1]) && (0xBE == bTag[2]) && (0xEF == bTag[3]))
			{
				this->m_iBBchip_Curr = BB_HI2120;
			}
			else
			{
				this->m_iBBchip_Curr = BB_HI2110;
			}

			Ptfile.Close();
			//-----------------------------------------------------------------
		}
		else if(wcscmp(csFormat, _T("bat")) == 0)//20191115
		{
			*ip_Platform = PF_HUAWEI;

			if (NULL != wcsstr(csFW_PATH,_T("RG80"))) //RG800/RG801 ��˼5G
			{
				this->m_iBBchip_Curr = BB_HI9500;
			}
			else
			{
				//20200628  ͨ�����ptable.bin�ļ������Ƿ�ƫ��17��λ���� 39 35 30 30(9500) ----5G

				unsigned long ul_File_Size =0;

				CString csFW_Ptable_PATH_str = csFW_PATH;
				int iAd = csFW_Ptable_PATH_str.ReverseFind('\\');
				csFW_Ptable_PATH_str = csFW_Ptable_PATH_str.Left(iAd+1);
				csFW_Ptable_PATH_str =csFW_Ptable_PATH_str +_T("ptable.bin");

				
				if(this->Get_File_Size(csFW_Ptable_PATH_str, &ul_File_Size) == false)
				{
					this->m_iBBchip_Curr = BB_HI2152;
				}
				else
				{
					CStdioFile Ptfile;
					BYTE bTag[4]={0};

					Ptfile.Open(csFW_Ptable_PATH_str, CFile::typeBinary | CFile::modeRead);
					Ptfile.Seek(16,CFile::begin);
					Ptfile.Read(bTag,4);
					if((0x39 == bTag[0])&&(0x35 == bTag[1])&&(0x30 == bTag[2])&&(0x30 == bTag[3]))
					{
						this->m_iBBchip_Curr = BB_HI9500;
					}
					else
					{
						this->m_iBBchip_Curr = BB_HI2152;
					}

					Ptfile.Close();
				}
			}		

			//20200525 ����ADB�Ļ�������
			//\HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Control\Session Manager\Environment
			this->EditRegistry(HKEY_CURRENT_USER,_T("Environment"),KEY_ALL_ACCESS,_T("Path"));

		}
		else if(wcscmp(csFormat, _T("zip")) == 0)
		{
			*ip_Platform = PF_ASR;

		}
		else if(wcscmp(csFormat, _T("bin")) == 0 && NULL != wcsstr(csFW_PATH,_T("BOOTLOADER_BC260")))
		{
			*ip_Platform = PF_EIGENCOMM;
			this->m_iBBchip_Curr = BB_EC616;
		}
		else if(wcscmp(csFormat, _T("bin")) == 0 && NULL != wcsstr(csFW_PATH,_T("BOOTLOADER_BC950FCN")))
		{
			*ip_Platform = PF_EIGENCOMM;
			this->m_iBBchip_Curr = BB_EC617;
		}
		else if((wcscmp(csFormat, _T("bin")) == 0) && (NULL != wcsstr(csFW_PATH,_T("BOOTLOADER_BC660K"))))
		{
			*ip_Platform = PF_EIGENCOMM;
			this->m_iBBchip_Curr = BB_EC616;
		}
		else if((wcscmp(csFormat, _T("bin")) == 0) && (NULL != wcsstr(csFW_PATH,_T("BOOTLOADER_BC95GF"))))
		{
			*ip_Platform = PF_EIGENCOMM;
			this->m_iBBchip_Curr = BB_EC617;
		}
		else if ((wcscmp(csFormat, _T("bin")) == 0) && (NULL != wcsstr(csFW_PATH, _T("FC41D"))))//20210321 ,��Ҫȷ��·��
		{
			*ip_Platform = PF_BEKEN;
		}
		//MTK 5G 
		else if ((wcscmp(csFormat, _T("xml")) == 0) && (NULL != wcsstr(csFW_PATH, _T("FLASH"))))
		{
			*ip_Platform = PF_MTK;
			this->m_iBBchip_Curr = BB_MT6880;
		}
		else if(wcscmp(csFormat, _T("pac")) == 0)
		{
			*ip_Platform = PF_SPREADTRUM;
		}
		else if (wcscmp(csFormat, _T("mimg")) == 0)
		{
			*ip_Platform = PF_XINYI;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CDlg_Example::Load_FW_Files(void)
{
	try
	{
		//�����ʾ
		this->Reset_View();
		this->Clear_View_Bin_List();

		this->Load_Flash_Option_Params();

		//--------------------------------------------------
		GetDlgItem(IDC_STATIC_AT_PORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_AT_PORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SET_DL_MODE)->ShowWindow(SW_HIDE);
		//--------------------------------------------------

		//���ݲ������ͣ����ֱ��Ƿ�Ҫ����FW�ļ�
		//���ݲ������ͣ����ò����İ�ť��ʾ��ͬ
		if((this->m_iPlatform_Curr == PF_QUALCOMM) && (this->m_iCtrl_OP_Option_S == QCMM_OPTION_BACKUP_ONLY))
		{
			//----------------------------------- ��ͨƽ̨�µ�NV���롢�������������ģʽ���ǲ���Ҫ����FW�ģ� ----------------------------------- 
			this->m_ctr_btnSet_Scat_Cfg_File.SetWindowTextW(_T("Set Backup File"));
		}
		else if((this->m_iPlatform_Curr == PF_QUALCOMM) && (this->m_iCtrl_OP_Option_S == QCMM_OPTION_RESTORE_ONLY))
		{
			//----------------------------------- ��ͨƽ̨�µ�NV���롢�������������ģʽ���ǲ���Ҫ����FW�ģ� ----------------------------------- 
			this->m_ctr_btnSet_Scat_Cfg_File.SetWindowTextW(_T("Load Restore File"));
		}
		else
		{
			//----------------------------------- ������ͨ��Ҫ����FW�Ĳ��� ----------------------------------- 
			this->m_ctr_btnSet_Scat_Cfg_File.SetWindowTextW(_T("Load FW Files"));

			//ʶ�𷽰�ƽ̨
			if(this->m_csScat_Cfg_File_Path.GetLength() > 0)
			{
				if(this->Get_Platform(this->m_csScat_Cfg_File_Path, &this->m_iPlatform_Curr) == false)
				{					
					this->Show_Status(_T("Error, Tool can not detect platform name!"));
					throw(0);
				}

			}
			else
			{
				this->Show_Status(_T("Please choose download firmware."));
				throw(0);
			}

			//����ƽ̨Option
			this->Load_View_Option(this->m_iPlatform_Curr);

			//���������õ�FW�ļ����Ѹ��ļ���������ͽṹ��
			switch(this->m_iPlatform_Curr)
			{
			case PF_MTK:
				{
					switch (m_iBBchip_Curr)
					{
						case BB_MT2625://BC20/BC26/BC66/BC950N/BC030
							//����FW�е�����ļ�
							if (this->Load_All_DL_Files_MTK_NB() == false)
							{
								throw(0);
							}
						break;
						case BB_MT6880://RM500K-CN
							//����FW�е�����ļ�
							if (this->Load_All_DL_Files_MTK_5G() == false)
							{
								throw(0);
							}
							break;
						default:
							//����FW�е�����ļ�
							if (this->Load_All_DL_Files_MTK() == false)
							{
								throw(0);
							}
						break;
					}
					
					
					break;
				}
			case PF_QUALCOMM:
				{
					//����FW�е�����ļ�
					if(this->Load_All_DL_Files_QCMM() == false)
					{
						throw(0);
					}

					break;
				}
			case PF_RDA:
				{
					//����FW�е�����ļ�
					if(this->Load_All_DL_Files_RDA() == false)
					{
						throw(0);
					}

					break;
				}
			case PF_INTEL:
				{
					//����FW�е�����ļ�
					if(this->Load_All_DL_Files_Intel() == false)
					{
						throw(0);
					}

					break;
				}
			case PF_HUAWEI:
				{
					//����FW�е�����ļ�
					if(this->Load_All_DL_Files_HUAWEI() == false)
					{
						throw(0);
					}

					break;
				}
			case PF_ASR:
				{
					//����FW�е�����ļ�
					if(this->Load_All_DL_Files_ASR() == false)
					{
						throw(0);
					}

					break;
				}
			case PF_EIGENCOMM:
				{
					//����FW�е�����ļ�
					if(this->Load_All_DL_Files_EIGEN() == false)
					{
						throw(0);
					}

					break;
				}
			case PF_SPREADTRUM:
				{
					//����FW�е�����ļ�
					if(this->Load_All_DL_Files_UNISOC() == false)
					{
						throw(0);
					}
					break;
				}
            //20201021
			case PF_XINYI:
			{
				//����FW�е�����ļ�
//				if (this->Load_All_DL_Files_XINYI() == false)
//				{
//					throw(0);
//				}
				break;
			}
			//20210321  ��ͨ�����ļ�����
			case PF_BEKEN:
			{
				//����FW�е�����ļ�
				if (this->Load_All_DL_Files_BEKEN() == false)
				{
					throw(0);
				}

				break;
			}
			default:
				throw(0);
			}
		}//����FW�ļ�

		
		//������������ã��������ĳ�ʼ��
		switch(this->m_iPlatform_Curr)
		{
		case PF_MTK:
			{
				switch (this->m_iBBchip_Curr)
				{
					case BB_MT2625:
					{
						//���롰�߼����ã��������ã�������ʼ������Ĭ�ϡ��������á�
						if (this->Load_Advanced_Config_MTK_NB() == false)
						{
							throw(0);
						}

						//MTKƽ̨���ԡ����ع�����Stop��
						this->m_ctr_btnStop.ShowWindow(SW_SHOW);
					}

					break;
					case BB_MT6880://MTK 5G
					{
						//��ս���bin�б�
						this->Clear_View_Bin_List();
						this->m_bLoad_Bin_List = true;
						//����.img�ļ�
						int i = 0;
						for (i = 0; i<MAX_BIN_FILE; i++)
						{
							if (this->m_cla_ARM_OP_5G_Main.m_strr_Bin_File_List[i].b_Enable_DL == true)
							{
								this->Add_ListCtrl_Bin_List_Data(
									this->m_cla_ARM_OP_5G_Main.m_strr_Bin_File_List[i].cs_Name,
									this->m_cla_ARM_OP_5G_Main.m_strr_Bin_File_List[i].cs_Path,
									true);
							}
						}
						this->m_bLoad_Bin_List = false;

						//��׼�����ع�����Stop��
						this->m_ctr_btnStop.ShowWindow(SW_HIDE);
						this->Init_Progress_Ctrl(PF_MTK, 0);

						this->Show_Status(_T("Do not remove USB or terminate the downloading process before it completes."));
					}
					break;
					default:
					{
						//���롰�߼����ã��������ã�������ʼ������Ĭ�ϡ��������á�
						if (this->Load_Advanced_Config_MTK(this->m_cla_Flash_OP_Main.m_csBin_Product) == false)
						{
							throw(0);
						}

						//MTKƽ̨���ԡ����ع�����Stop��
						this->m_ctr_btnStop.ShowWindow(SW_SHOW);
					}
					break;
				}	
				break;
			}
		case PF_QUALCOMM:
			{
				//���롰�߼����ã��������ã�������ʼ������Ĭ�ϡ��������á�
				if(this->Load_Advanced_Config_QCMM() == false)
				{
					throw(0);
				}

				//��ͨƽ̨��׼�����ع�����Stop��
				this->m_ctr_btnStop.ShowWindow(SW_HIDE);

				this->Show_Status(_T("Do not remove USB or terminate the downloading process before it completes."));

				break;
			}
		case PF_RDA:
			{
				//���롰�߼����ã��������ã�������ʼ������Ĭ�ϡ��������á�
				this->Load_Advanced_Config_RDA();

				//RDAƽ̨��׼�����ع�����Stop��
				this->m_ctr_btnStop.ShowWindow(SW_HIDE);

				this->Show_Status(_T("Do not remove USB or terminate the downloading process before it completes."));

				break;
			}
		case PF_INTEL:
			{
				//���롰�߼����ã��������ã�������ʼ������Ĭ�ϡ��������á�
				if(this->Load_Advanced_Config_Intel() == false)
				{
					throw(0);
				}

				//Intelƽ̨��׼�����ع�����Stop��
				this->m_ctr_btnStop.ShowWindow(SW_HIDE);

				this->Show_Status(_T("Do not remove USB or terminate the downloading process before it completes."));

				break;
			}
		case PF_HUAWEI:
			{
				//���롰�߼����ã��������ã�������ʼ������Ĭ�ϡ��������á�
				if(this->Load_Advanced_Config_HUAWEI() == false)
				{
					throw(0);
				}
				//��ս���bin�б�
				this->Clear_View_Bin_List();
				this->m_bLoad_Bin_List = true;

				if ((BB_HI2152 == this->m_iBBchip_Curr)
					||(BB_HI9500 == this->m_iBBchip_Curr))
				{
					//����.img�ļ�
					int i =0;
					for(i=0; i<MAX_IMG_FILE_HUAWEI; i++)
					{
						if(this->m_cla_ARM_OP_HUAWEI_Main.m_strr_Img_File_List[i].b_Enable_DL == true)
						{
							this->Add_ListCtrl_Bin_List_Data(
								this->m_cla_ARM_OP_HUAWEI_Main.m_strr_Img_File_List[i].cs_Name,
								this->m_cla_ARM_OP_HUAWEI_Main.m_strr_Img_File_List[i].cs_Path,
								true);
						}
					}
				} 
				else
				{

					if ((this->m_cla_ARM_OP_HUAWEI_Main.m_str_fwpkg_FW.cs_Name == _T(""))
						||(this->m_cla_ARM_OP_HUAWEI_Main.m_str_fwpkg_FW.cs_Path == _T("")))
					{
						this->Show_Status(_T("Firmware File Path Is Null"));
						throw(0);
					}

					this->Add_ListCtrl_Bin_List_Data(
						this->m_cla_ARM_OP_HUAWEI_Main.m_str_fwpkg_FW.cs_Name,
						this->m_cla_ARM_OP_HUAWEI_Main.m_str_fwpkg_FW.cs_Path,
						//this->m_cla_ARM_OP_HUAWEI_Main.m_str_fwpkg_FW.b_Enable_DL
						true);
				}
				this->m_bLoad_Bin_List = false;

				//NBƽ̨��׼�����ع�����Stop��
				this->m_ctr_btnStop.ShowWindow(SW_HIDE);

				this->Show_Status(_T("Do not remove USB or terminate the downloading process before it completes."));

				break;
			}
		case PF_ASR:
			{
				//���롰�߼����ã��������ã�������ʼ������Ĭ�ϡ��������á�
				if(this->Load_Advanced_Config_ASR() == false)
				{
					throw(0);
				}
				//��ս���bin�б�
				this->Clear_View_Bin_List();
				this->m_bLoad_Bin_List = true;

				if ((this->m_cla_ARM_OP_ASR_Main.m_str_ZIP_FW.cs_Name == _T(""))
					||(this->m_cla_ARM_OP_ASR_Main.m_str_ZIP_FW.cs_Path == _T("")))
				{
					this->Show_Status(_T("Firmware File Path Is Null"));
					throw(0);
				}

				this->Add_ListCtrl_Bin_List_Data(
					this->m_cla_ARM_OP_ASR_Main.m_str_ZIP_FW.cs_Name,
					this->m_cla_ARM_OP_ASR_Main.m_str_ZIP_FW.cs_Path,
					//this->m_cla_ARM_OP_HUAWEI_Main.m_str_fwpkg_FW.b_Enable_DL
					true);
				this->m_bLoad_Bin_List = false;

				//
				this->m_ctr_btnStop.ShowWindow(SW_HIDE);

				this->Show_Status(_T("Do not remove USB or terminate the downloading process before it completes."));

				break;
			}
		case PF_EIGENCOMM:
			{

				this->m_cla_ARM_OP_EIGEN_Main.m_iResetFlag = this->m_iResetFlag;
				//ƽ̨��׼�����ع�����Stop��
				this->m_ctr_btnStop.ShowWindow(SW_HIDE);

				this->Show_Status(_T("Do not remove USB or terminate the downloading process before it completes."));

				break;
			}
		case PF_BEKEN:
		{
			//ƽ̨��׼�����ع�����Stop��
			this->m_ctr_btnStop.ShowWindow(SW_HIDE);

			this->Show_Status(_T("Do not remove USB or terminate the downloading process before it completes."));

			break;
		}
		case PF_SPREADTRUM:
			{
				//ƽ̨��׼�����ع�����Stop��
				this->m_ctr_btnStop.ShowWindow(SW_HIDE);

				this->Show_Status(_T("Do not remove USB or terminate the downloading process before it completes."));

				break;
			}

			
		default:
			throw(0);
		}
	}
	catch(...)
	{
		this->Ctrl_Colour(COL_RED);
		return false;
	}

	this->Ctrl_Colour(COL_BLACK);
	return true;
}

bool CDlg_Example::Load_All_DL_Files_QCMM(void)
{
	try
	{
		this->m_cla_ARM_OP_QCM_Main.m_bHoneywell_A4_flag = m_bHoneywell_A4_flag;
		this->m_cla_ARM_OP_QCM_Main.m_bQDM002_flag = m_bQDM002_flag;
	

		this->m_cla_ARM_OP_QCM_Main.m_csCurr_Exe_Path = m_csCurr_Exe_Path;

		this->m_cla_ARM_OP_QCM_Main.m_bSaharaOnly = this->m_bSaharaOnly;//20200114 ����˵�������SaharaOnly������

		//��FW�ļ���ȡBB����Ϣ
		if(this->m_cla_ARM_OP_QCM_Main.LOAD_BB_INFO(this->m_csScat_Cfg_File_Path) == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_QCM_Main.m_csError_Code);
			throw(0);
		}



		//��ʼ������Ϣ�������ͨ�����ļ�
		if(this->m_cla_ARM_OP_QCM_Main.LOAD_FW_FILE(this->m_csScat_Cfg_File_Path) == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_QCM_Main.m_csError_Code);
			throw(0);
		}


		if (false ==  m_bHoneywell_A4_flag)
		{
			//NvDefinition�ļ�·��
			this->m_cla_ARM_OP_QCM_Main.m_csNv_Definition_File.Format(_T("%s\\NvDefinition.xml"), this->m_csCurr_Exe_Path);

			//���롰.exe����·�����͡�temp�ļ������ļ�·����
			this->m_cla_ARM_OP_QCM_Main.LOAD_TEMP_FW_FILE(this->m_csCurr_Exe_Path);

			//����ͨ���ò���
			this->Load_Params_QCMM(_T("QCM"));
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void CDlg_Example::Add_ListCtrl_Bin_List_Data(const wchar_t * wcp_Data_1, const wchar_t * wcp_Data_2, bool b_Check)
{
	int i, j;

	try
	{
		//��ǰlist�е�index��
		i = this->m_ctr_lscBin_List.GetItemCount();

		//����µ�һ�У�BIN�ļ�����
		j = this->m_ctr_lscBin_List.InsertItem(LVIF_TEXT|LVIF_STATE, i, wcp_Data_1,
			LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
		
		if((j < 0) || (j != i))
		{
			::AfxMessageBox(_T("FAIL, Show Bin Path!"));
			throw(0);
		}
		
		//BIN�ļ����ġ�CheckBox��
		this->m_ctr_lscBin_List.SetCheck(j, b_Check);

		//
		this->m_ctr_lscBin_List.SetItemText(j, 1, wcp_Data_2);

		//
		CSize ss(0, (j + 1));
		this->m_ctr_lscBin_List.Scroll(ss);
	}
	catch(...)
	{
		return;
	}

	return;
}

void CDlg_Example::OnCbnDropdownComboComMain()
{
	//��ʼ������ѡ�ˢ�´����б�
	this->Update_Com_Ctrl_Option();
}

void CDlg_Example::Load_View_Bin_List_QCMM(void)
{
	int i;

	this->m_bLoad_Bin_List = true;

	//��ս���bin�б�
	this->Clear_View_Bin_List();

	if ((HONEYWELL == this->m_cla_ARM_OP_QCM_Main.m_eu_BB_Chip)
		||(QDM002 == this->m_cla_ARM_OP_QCM_Main.m_eu_BB_Chip))
	{
		this->m_cla_ARM_OP_QCM_Main.m_bFW_DL = true;
	}


	//
	if(this->m_cla_ARM_OP_QCM_Main.m_bFW_DL == true)
	{
		//�������ļ���
		switch(this->m_cla_ARM_OP_QCM_Main.m_eu_BB_Chip)
		{
		case MDM6X00:
		case QSC6270:
		case MDM9X15:
			{
				//�������ص���HEX�ļ�
				this->Add_ListCtrl_Bin_List_Data(
					this->m_cla_ARM_OP_QCM_Main.m_str_Hex_E.cs_Name,
					this->m_cla_ARM_OP_QCM_Main.m_str_Hex_E.cs_Path,
					true);

				this->Add_ListCtrl_Bin_List_Data(
					this->m_cla_ARM_OP_QCM_Main.m_str_Hex_N.cs_Name,
					this->m_cla_ARM_OP_QCM_Main.m_str_Hex_N.cs_Path,
					true);
			}
			break;
		case HONEYWELL:
		case QDM002:
			{
				//�������ص���HEX�ļ�
				this->Add_ListCtrl_Bin_List_Data(
					this->m_cla_ARM_OP_QCM_Main.m_str_Hex_E.cs_Name,
					this->m_cla_ARM_OP_QCM_Main.m_str_Hex_E.cs_Path,
					true);
			}
			break;
		case MDM9X07:
		case MDM9X40:	
		//case SDX20: //EM20û�и��ļ������ⲻ��Ҫ��ʾ
		case MDM9X06:
		case MDM9X55:
		
			{
				//�������ص��������ļ�
				this->Add_ListCtrl_Bin_List_Data(
					this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_Emerg.cs_Name,
					this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_Emerg.cs_Path,
					this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_Emerg.b_Enable_DL);

				this->Add_ListCtrl_Bin_List_Data(
					this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_Norm.cs_Name,
					this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_Norm.cs_Path,
					this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_Norm.b_Enable_DL);
			}
			break;
		case MSM8909:
		case MSM8917:
		case MSM8937:
		case MSM8953:
		case SDM660:
		case SDM845:
		case SDX20:
		case SDX55:
		case MDM9X05:
		case SM6115:
		case AG215S:

			
			{
				//8909Ҫ��ʾ�ļ���·���ļ�
				this->Add_ListCtrl_Bin_List_Data(
					_T("File_Path"),
					this->m_cla_ARM_OP_QCM_Main.m_csFW_File_Path,
					true);
			}
			break;
		default:
			break;
		}


		//������ص���Partition�ļ�
		if(this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_Partition_Table.b_Enable_DL == true)
		{
			this->Add_ListCtrl_Bin_List_Data(
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_Partition_Table.cs_Name,
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_Partition_Table.cs_Path,
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_Partition_Table.b_Enable_DL);
		}


		//������ص���Partition-2�ļ�
		if(this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_PT_2.b_Enable_DL == true)
		{
			this->Add_ListCtrl_Bin_List_Data(
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_PT_2.cs_Name,
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_PT_2.cs_Path,
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_PT_2.b_Enable_DL);
		}


		//�����������Mbn�ļ�
		for(i=0; i<MAX_MBN_FILE; i++)
		{
			if(this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[i].b_Enable_DL == true)
			{
				this->Add_ListCtrl_Bin_List_Data(
					this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[i].cs_Name,
					this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[i].cs_Path,
					this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[i].b_Enable_DL);
			}
		}


		//����CEFS�ļ�
		if(this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_CEFS.b_Enable_DL == true)
		{
			this->Add_ListCtrl_Bin_List_Data(
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_CEFS.cs_Name,
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_CEFS.cs_Path,
				this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL);
		}


		//����FW QCN-A�ļ�
		if(this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_DL.b_Enable_DL == true)
		{
			this->Add_ListCtrl_Bin_List_Data(
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_DL.cs_Name,
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_DL.cs_Path,
				this->m_cla_ARM_OP_QCM_Main.m_bQCN_DL);
		}

		//��SBL2.mbn�ļ�
		if(this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_SBL2_TMP.b_Enable_DL == true)
		{
			this->Add_ListCtrl_Bin_List_Data(
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_SBL2_TMP.cs_Name,
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_SBL2_TMP.cs_Path, true);
		}
		//��APPSBL.mbn�ļ�
		if(this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_APPSBL_TMP.b_Enable_DL == true)
		{
			this->Add_ListCtrl_Bin_List_Data(
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_APPSBL_TMP.cs_Name,
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_APPSBL_TMP.cs_Path, true);
		}
	}//if(this->m_cla_ARM_OP_QCM_Main.m_bFW_DL == true


	//fastboot�ļ�
	if(this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL == true)
	{
		//����.yaffs2�ļ�
		for(i=0; i<MAX_YAFFS2_FILE; i++)
		{
			if(this->m_cla_ARM_OP_QCM_Main.m_strr_Yaffs2_File_List[i].b_Enable_DL == true)
			{
				this->Add_ListCtrl_Bin_List_Data(
					this->m_cla_ARM_OP_QCM_Main.m_strr_Yaffs2_File_List[i].cs_Name,
					this->m_cla_ARM_OP_QCM_Main.m_strr_Yaffs2_File_List[i].cs_Path,
					true);
			}
		}

		//����.img�ļ�
		for(i=0; i<MAX_IMG_FILE; i++)
		{
			if(this->m_cla_ARM_OP_QCM_Main.m_strr_Img_File_List[i].b_Enable_DL == true)
			{
				this->Add_ListCtrl_Bin_List_Data(
					this->m_cla_ARM_OP_QCM_Main.m_strr_Img_File_List[i].cs_Name,
					this->m_cla_ARM_OP_QCM_Main.m_strr_Img_File_List[i].cs_Path,
					true);
			}
		}
	}


	//.aa�ļ�
	if(this->m_cla_ARM_OP_QCM_Main.m_str_Yaffs_Head_aa.b_Enable_DL == true)
	{
		this->Add_ListCtrl_Bin_List_Data(
			this->m_cla_ARM_OP_QCM_Main.m_str_Yaffs_Head_aa.cs_Name,
			this->m_cla_ARM_OP_QCM_Main.m_str_Yaffs_Head_aa.cs_Path,
			this->m_cla_ARM_OP_QCM_Main.m_str_Yaffs_Head_aa.b_Enable_DL);
	}


	//.ab�ļ�
	if(this->m_cla_ARM_OP_QCM_Main.m_str_Yaffs_Head_ab.b_Enable_DL == true)
	{
		this->Add_ListCtrl_Bin_List_Data(
			this->m_cla_ARM_OP_QCM_Main.m_str_Yaffs_Head_ab.cs_Name,
			this->m_cla_ARM_OP_QCM_Main.m_str_Yaffs_Head_ab.cs_Path,
			this->m_cla_ARM_OP_QCM_Main.m_str_Yaffs_Head_ab.b_Enable_DL);
	}


	/*
	//�Ƿ����QCN������������Ҫ����NvDefinition�ļ�
	if(this->m_cla_ARM_OP_QCM_Main.m_bQCN_DL || this->m_cla_ARM_OP_QCM_Main.m_bQCN_BACKUP || this->m_cla_ARM_OP_QCM_Main.m_bQCN_RESTORE)
	{
		this->Add_ListCtrl_Bin_List_Data(_T("NvDefinition"), this->m_cla_ARM_OP_QCM_Main.m_csNv_Definition_File, true);
	}
	*/


	//���뵼�뵼�� QCN-B�ļ�
	if(this->m_cla_ARM_OP_QCM_Main.m_bQCN_BACKUP || this->m_cla_ARM_OP_QCM_Main.m_bQCN_RESTORE)
	{
		this->Add_ListCtrl_Bin_List_Data(
			this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Name,
			this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Path, 
			true);
	}


	//�ָ�
	this->m_bLoad_Bin_List = false;
}

void CDlg_Example::DO_DL_OP(void)
{
	int iRtn;
	bool bDl_ResFlag = false;


	//
	this->Set_View_Ctrl_State(3);

	//�����ʾ
	this->Reset_View();

	this->m_ctr_edtStatus_Main.SetFont(&this->m_cla_fontEdt_1);

	//��ʱ��ʼ��1�ſ�ʼʱ�䣩
	this->Start_Timer(1);

	this->m_irDL_OBJ_State=DL_OBJ_RUN;


	//ִ�к���
	switch(this->m_iPlatform_Curr)
	{
	case PF_MTK:
		{
			switch (m_iBBchip_Curr)
			{
				case BB_MT2625:
				{
					iRtn = this->m_cla_ARM_OP_NB_Main.Do_Execute();
					this->m_ctr_pgrProgress_Main.SetPos(300);
				}
				break;
				case BB_MT6880:
				{
					iRtn = this->m_cla_ARM_OP_5G_Main.Do_Execute();
					//this->m_ctr_pgrProgress_Main.SetPos(300);
				}
				break;
				default:
				{
					iRtn = this->m_cla_Flash_OP_Main.Do_Execute();
					//add
					switch (this->m_cla_Flash_OP_Main.m_iOP_TYPE)
					{
						case MTK_OPTION_DL_FORMAT:
							this->m_ctr_pgrProgress_Main.SetPos(400);
							break;
						case MTK_OPTION_DL_ONLY:
							this->m_ctr_pgrProgress_Main.SetPos(300);
							break;
						case MTK_OPTION_DL_CLEAR_NV:
							this->m_ctr_pgrProgress_Main.SetPos(300);
							break;
						default:
							this->m_ctr_pgrProgress_Main.SetPos(300);
							break;
					}
				}
				break;

			}	
			
		}

		break;
	case PF_QUALCOMM:
		{
			iRtn = this->m_cla_ARM_OP_QCM_Main.Do_Execute();

			//�ر�δ������ֹ�Ľ��̣�process��
			switch(this->m_cla_ARM_OP_QCM_Main.m_eu_BB_Chip)
			{
			case MDM9X15:
				this->m_cla_ARM_OP_QCM_Main.m_cla_GFC.Kill_Process(_T("fastboot.exe"));
				break;
           case MDM9X07:
			case MDM9X40:
			case MDM9X55:
			case MSM8909:
			case MSM8917:
			case MSM8953:
			case MSM8937:
			case SDX20:
			case SDX55:
			case SDM660:
			case SDM845:
			case MDM9X05:
			case SM6115:
			case AG215S:

				this->m_cla_ARM_OP_QCM_Main.m_cla_GFC.Kill_Process(_T("QSaharaServer.exe"));
				this->m_cla_ARM_OP_QCM_Main.m_cla_GFC.Kill_Process(_T("fh_loader.exe"));
				break;
			default:
				break;
			}
		}
		break;
	case PF_RDA:
		{
			iRtn = this->m_cla_ARM_OP_RDA_Main.Do_Execute();
		}
		break;
	case PF_INTEL:
		{
			iRtn = this->m_cla_ARM_OP_INTEL_Main.Do_Execute();

			//�ر�δ������ֹ�Ľ��̣�process��
			this->m_cla_ARM_OP_INTEL_Main.m_cla_GFC.Kill_Process(_T("DownloadTool.exe"));
		}
		break;
	case PF_HUAWEI:
		{
			iRtn = this->m_cla_ARM_OP_HUAWEI_Main.Do_Execute();

			//�ر�δ������ֹ�Ľ��̣�process��
			//this->m_cla_ARM_OP_Huawei_Main.m_cla_GFC.Kill_Process(_T("DownloadTool.exe"));
		}
		break;
	case PF_ASR:
		{
			//20181112
			//�ؼ�״̬�л�

			CString strDet_Port1,strDet_Port_Path1,csPathInfo,csInfo;
			char cTemp[1000] = { 0 };
			CStringA csTemp;

			m_hWaitTestFin_Main  = ::CreateEvent(NULL,FALSE,TRUE,NULL);
			::ResetEvent(m_hWaitTestFin_Main);

			this->Show_Status(_T("Start or Reset Module..."));

			this->Set_View_Ctrl_State(3);

			if (ASR3601 == this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip)
			{
				this->Show_Status(_T("Scanning port..."));

				//�ȼ��˿ں��ǲ���AT�˿ڣ������AT�˿�������AT+QDOWNLOAD=1
				if(this->m_cla_ARM_OP_ASR_Main.Confirm_Dynamic_Com_Port(this->m_cla_ARM_OP_ASR_Main.m_iCom_Port, 30) == true)
				{
					if (true == this->m_cla_ARM_OP_ASR_Main.IsAT_Port(this->m_cla_ARM_OP_ASR_Main.m_iCom_Port))
					{
						//AT+QDOWNLOAD=1
						this->m_cla_ARM_OP_ASR_Main.SendQD_Mode(this->m_cla_ARM_OP_ASR_Main.m_iCom_Port);
						Sleep(5000);
					}
				}



				CUSBDeviceDetection usbDeviceDetect;
				CStringArray strPhysicalAddress,strDeviceInstancePath;
//				UINT uDeviceCount;

// 				this->Show_Status(_T("Get USB addresss..."));
// 				this->Save_Trace_Log_File(_T("Get USB addresss..."));
// 
// 				time_t time_Curr;
// 				time_t time_start;
// 				int iTimer =0;
// 				CString strInf;
// 
// 				time(&time_start);
// 
// 
// 				while (1)
// 				{
// 					usbDeviceDetect.GetUsbPhysicalAddressList(strPhysicalAddress,strDeviceInstancePath,uDeviceCount);
// 					if (strPhysicalAddress.GetSize()>0)
// 					{
// 						break;
// 					}
// 					Sleep(500);
// 
// 					time(&time_Curr);
// 					iTimer =  (int)difftime(time_Curr, time_start);
// 
// 					if (iTimer > 20)
// 					{
// 						break;
// 					}
// 					else
// 					{
// 						strInf.Format(_T("Get USB addresss...%ds"),iTimer);
// 
// 						this->Show_Status(strInf);
// 					}
// 	
// 				}

				if (this->m_strUSBPortPath.GetLength()<=0)
				{
					//
					this->Save_Trace_Log_File(_T("Get USB addresss ��ʱ���뽫ģ�������ٽ��л�ȡ"));	
					iRtn = RTN_FAIL;
				}
				else
				{
					csPathInfo.Format(_T("\"path\":\"%s\""),this->m_strUSBPortPath);

					ConvertTCHARToChar(csPathInfo.GetBuffer(0), cTemp, sizeof(cTemp));
					csPathInfo.ReleaseBuffer();
					csTemp.Format("{\n%s\n}", cTemp);
					if (false == this->m_cla_ARM_OP_ASR_Main.m_bRuning)
					{
						this->m_cla_ARM_OP_ASR_Main.m_bRuning = true;

						//�����COM�ڵ�����
						aboot_download_enable_device(csTemp.GetBuffer(0));

						int iLen =0;
						wchar_t * wcJson = NULL;
						CString  csJson;
						//�����ڴ�
						this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcJson, strlen(csTemp) + 256);
						//��������
						this->m_cla_GFC.MultiByte_To_WideChar(csTemp, wcJson, &iLen);
						csJson.Format(_T("%s"), wcJson);
						//�ͷ��ڴ�
						this->m_cla_GFC.BUFF_WCHAR_FREE(&wcJson);



						//
						csInfo.Format(_T("++++aboot_download_enable_device - %s"),csJson);
						this->Save_Trace_Log_File(csInfo);

						this->Show_Status(_T("Download_Enable_Device"));



						csTemp.ReleaseBuffer();
					}
					else
					{
						this->Show_Status(_T("Download Running"));

					}

				}
			}
			else
			{
				iRtn = RSA_DL_Run();
			}
			

			if (RTN_FAIL != iRtn)
			{
				m_bWaitTestEvent_Main =TRUE;

				if (WAIT_TIMEOUT == ::WaitForSingleObject(m_hWaitTestFin_Main,200000))
				{
					//��ʱ 2����
					this->m_cla_ARM_OP_ASR_Main.m_iError_Code = 0x03000000|0x00003002;
					this->m_cla_ARM_OP_ASR_Main.m_csError_Code = _T("DL Time Out"); 
				}
				else
				{

				}
			}
			else
			{
				//
				//��ʱ 2����
				this->m_cla_ARM_OP_ASR_Main.m_iError_Code = 0x03000000|0x00003002;
				this->m_cla_ARM_OP_ASR_Main.m_csError_Code = _T("DL Time Out"); 

			}
			  

			if (ASR1802 == this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip)
			{
				//����ֱ�Ӱ�ASR�����̹߳ر�
				if(m_bInitializeBL)
				{
					StopDownload();
					m_bInitializeBL = false;
				}
			}

			m_bWaitTestEvent_Main =FALSE;
			this->Show_Status(_T("WaitTest End"));

			if (0xffffffff == this->m_cla_ARM_OP_ASR_Main.m_iError_Code)
			{
				iRtn = RTN_SUCCESS;
				this->Show_Status(_T("PASS"));
			} 
			else
			{
				iRtn = RTN_FAIL;
				this->Show_Status(this->m_cla_ARM_OP_ASR_Main.m_csError_Code);

			}

			if (ASR3601 == this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip)
			{
				CString strInfo;
				strInfo.Format(_T("If you do not upgrade the firmware, please close the tool!"));

				this->OnGetMsg_Show_Status2((WPARAM)strInfo.GetBuffer(),NULL);
			}

		}
		break;
	case PF_EIGENCOMM:
		{
			iRtn = this->m_cla_ARM_OP_EIGEN_Main.Do_Execute();
		}
		break;
	case PF_BEKEN:
 		{
		iRtn = this->m_cla_ARM_OP_BEKEN_Main.Do_Execute();
		}
 		break;
	case PF_SPREADTRUM:
		{
			iRtn = this->Do_Execute_Unisoc();
			break;
		}
	default:
		::AfxMessageBox(_T("ERROR, 103"));
		iRtn = RTN_ERROR;
		break;
	}


	//�����ؽ��
	this->m_ctr_edtStatus_Main.SetFont(&this->m_cla_fontEdt_2);

	if(iRtn == RTN_SUCCESS)
	{
		this->Show_Status(_T("PASS"));
		this->Ctrl_Colour(COL_BLUE);
	}
	else
	{
//		this->Show_Status(_T("FAIL"));
		this->Ctrl_Colour(COL_RED);
	}


	//��ʱ����
	this->KillTimer(1);

	//�ر�Debug_Log�ļ�
	this->Close_Trace_Log_File();


	//
	this->Set_View_Ctrl_State(1);

	if(1== this->m_iQFLASH_CMD )
	{
		Sleep(2000);
		if (1== m_iQFLASH_CMD)
		{
			CString strData;
			//�����ؽ��
			if(iRtn == RTN_SUCCESS)
			{
				strData.Format(_T("Download Pass"));
			}
			else
			{
				strData.Format(_T("Download Fail"));
			}
			WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);
		}

#if 0
		CWnd*  hDwnd=FindWindow(_T("ConsoleWindowClass"),NULL);  
		HWND hCMDwnd;
		hCMDwnd = hDwnd->GetSafeHwnd();
		//�����ؽ��
		if(iRtn == RTN_SUCCESS)
		{
			wchar_t wch[]=L"echo Download Pass\r\n";
			for(size_t i=0;i<wcslen(wch);i++)
			{
				::SendMessage(hCMDwnd,WM_CHAR,wch[i],NULL);
			}
		}
		else
		{
			wchar_t wch[]=L"echo Download Fail\r\n";
			for(size_t i=0;i<wcslen(wch);i++)
			{
				::SendMessage(hCMDwnd,WM_CHAR,wch[i],NULL);
			}
		}
#endif
		m_cla_GFC.Kill_Process(_T("QFlash_V4.20.exe"));
	}
}

void CDlg_Example::Start_Timer(int i_Timer_Number)
{
	switch(i_Timer_Number)
	{
	case 1://��1�š�time_Startʱ��
		time(&this->m_time_Start_1);
		break;
	default:
		return;
	}

	//������Ӧ��ʱ��
	this->SetTimer(i_Timer_Number, 1000, NULL);
}

void CDlg_Example::Load_Params_QCMM(const wchar_t * wcp_Product)
{
	//����ͨ�ŵ���ʽ
	this->m_cla_ARM_OP_QCM_Main.m_iComm_Type = COMM_SYNC;//this->m_cla_iniParams.ReadInt(wcp_Product, _T("COMM_SY_OV"), COMM_SYNC);
}

LRESULT CDlg_Example::OnGetMsg_Show_Progress_Entire(WPARAM wParam, LPARAM lParam)
{
	this->m_ctr_pgrProgress_Main.SetPos((int)wParam);

	return LRESULT();
}

LRESULT CDlg_Example::OnGetMsg_DL_Info(WPARAM wParam, LPARAM lParam)
{
	if(this->m_iPlatform_Curr == PF_QUALCOMM)
	{
		switch((int)wParam)
		{
		case QCMM_DL_TYPE_EMERG:
			this->Show_Status(_T("DL-EMERG"));
//			::AfxMessageBox(_T("DL-EMERG"));
			break;
		case QCMM_DL_TYPE_NORM:
			this->Show_Status(_T("DL-NORMAL"));
//			::AfxMessageBox(_T("DL-NORMAL"));
			break;
		default:
			return LRESULT();
		}		
	}

	return LRESULT();
}

void CDlg_Example::INIT(void)
{
	//��ʼ��ǰ���ð�ť�һ�
	this->Set_View_Ctrl_State(0);

	//������ҳConfig����
	this->Read_Config();

	//���·��Ϊ�գ�ֱ����ʾ�˳�
	if(this->m_csScat_Cfg_File_Path.IsEmpty() == true)
	{
		this->Show_Status(_T("Please choose download firmware."));
		this->Ctrl_Colour(COL_BLACK);
		this->Set_View_Ctrl_State(2);
		return;
	}

	//�ж������ļ��е�·���Ƿ����
	unsigned long ul_File_Size=0;
	if(this->Get_File_Size(this->m_csScat_Cfg_File_Path, &ul_File_Size) == false)
	{
		this->Show_Status(_T("Old path not exist,please choose download firmware."));
		this->Ctrl_Colour(COL_BLACK);
		this->Set_View_Ctrl_State(2);
		return;		
	}

	//20181007
	//20181007 ����·�����Ƿ���firehose����ʶ����Sahara����Firehose


	if((wcsstr(this->m_csScat_Cfg_File_Path,_T(" ")) != NULL)
		||(wcsstr(this->m_csCurr_Exe_Path,_T(" ")) != NULL))
	{
		::AfxMessageBox(_T("Fail,Tool And Firmware Path Have Space!"));
		this->Show_Status(_T("Fail,Tool And Firmware Path Have Space!"));
		this->Set_View_Ctrl_State(2);
		return;
	}



	if(this->Get_Platform(this->m_csScat_Cfg_File_Path, &this->m_iPlatform_Curr) == true)
	{
		if(PF_QUALCOMM == this->m_iPlatform_Curr)
		{
			CString strScat_Cfg_File_Path;
			strScat_Cfg_File_Path = m_csScat_Cfg_File_Path;
			strScat_Cfg_File_Path.MakeUpper();
			if(NULL != wcsstr(strScat_Cfg_File_Path,_T("FIREHOSE")))
			{
				this->m_cla_ARM_OP_QCM_Main.m_iFirhose_DB_Path = 1;
			}
			else
			{				
				this->m_cla_ARM_OP_QCM_Main.m_iFirhose_DB_Path = 0;

			}
		}
		else if(PF_ASR == this->m_iPlatform_Curr)
		{
			m_sExtractPackageFile.Format(_T("%s\\%s"),m_csCurr_Exe_Path,TEMP_PACKAGE);

			if(BrowseDirectory(m_sExtractPackageFile,m_sExtractPackageFile))
			{
				DeleteFolder(m_sExtractPackageFile);
			}
		}
		else
		{
			//...
		}


		if (1== m_iQFLASH_CMD)
		{
			CString strData;
			switch(this->m_iPlatform_Curr)
			{
				case PF_MTK:
					if (BB_MT2625 != m_iBBchip_Curr)
					{
						strData.Format(_T("Platform->MTK\r\n"));
					}
					else
					{
						strData.Format(_T("Platform->MTK NB\r\n"));
					}
					break;
				case PF_QUALCOMM:
					strData.Format(_T("Platform->MTK QUALCOMM\r\n"));
					break;
				case PF_HUAWEI:
					strData.Format(_T("Platform->HUAWEI\r\n"));
					break;
				case PF_RDA:
					strData.Format(_T("Platform->RDA\r\n"));
					break;
				case PF_ASR:
					strData.Format(_T("Platform->ASR\r\n"));
					break;
				case PF_INTEL:
					strData.Format(_T("Platform->INTEL\r\n"));
					break;
				default:
					strData.Format(_T("Platform->NULL\r\n"));
					break;
			}
			
			WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);
		}
	}
	else
	{
		if (1== m_iQFLASH_CMD)
		{
			CString strData;
			strData.Format(_T("Find Platform Fail\r\n"));
			WriteConsole(g_hdlWrite, strData, strData.GetLength(), NULL, NULL);
		}
	}

	//�������¹��ļ���������ͽṹ��
	if(this->Load_FW_Files())
	{
		this->Set_View_Ctrl_State(1);
	}
	else
	{
		this->Set_View_Ctrl_State(2);
	}

	//��ʼ������ѡ�ˢ�´����б������ݲ���ģʽ��ƽ̨���趨����CTRL��ѡ��
	//ע�⣺���������ѡ��ڲ�����Load_Flash_Option_Params��
	this->Update_Com_Ctrl_Option();

	//
	this->UpdateData(FALSE);

	//���ݽ���CTRL�����ã��������õı��������ز���
	this->Load_Flash_Option_Params();
}

bool CDlg_Example::Load_Advanced_Config_QCMM()
{
	int i;

	try
	{
		//---------------------------- ��ȡ���߼����ã��������ã����Ĳ���
		this->Get_Advanced_Config_Value(PF_QUALCOMM, NULL);



		//---------------------------- ���ݡ��߼����ã��������ã����Ĳ��������á���ͨ�ࡱ�еġ����������
		//��ͨ����ģʽ�ɽ����趨
		this->m_cla_ARM_OP_QCM_Main.m_iCTRL_DL_TYPE = this->m_iCtrl_Dl_Type_S;

		//��ͨ���ء��ڡ��趨
		this->m_cla_ARM_OP_QCM_Main.m_iCTRL_DLOAD_PORT = this->m_iCtrl_DLoad_Port_S;

		//���ݡ�Optionѡ������ø�ͨ���еĲ������Ʊ���
		switch(this->m_iCtrl_OP_Option_S)
		{
		case QCMM_OPTION_DL_AUTO://���ڹ��ߣ���FW�����Ƿ���EFS�ļ���QCN�ļ���ȷ���Ƿ������������ļ�
			{
				//�Ƿ�����FW�ļ�
				this->m_cla_ARM_OP_QCM_Main.m_bFW_DL = true;


				//�Ƿ��ʽ��
				switch(this->m_cla_ARM_OP_QCM_Main.m_eu_BB_Chip)
				{
				case MDM6X00:
				case QSC6270:
				case MDM9X15:
				case MDM9X07://���Ƿ���cefs�ļ�����
				case MDM9X55:
				case MDM9X40:
				case SDX20:
				case SDX55:
				case MDM9X05:
				case MDM9X06:
					{
						this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_CEFS.b_Enable_DL;
					}
					break;
				case MSM8909://���Ƿ���fs_image��zero�ļ�����
					{
						if((this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[FS_IMAGE_8909_IMG_FILE_I].b_Enable_DL == true) ||
							(this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[ZERO_8909_BIN_FILE_I].b_Enable_DL == true))
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = true;
						}
						else
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = false;
						}
					}
					break;
				case MSM8917://���Ƿ���fs_image��zero�ļ�����
					{
						if((this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[FS_IMAGE_8917_IMG_FILE_I].b_Enable_DL == true) ||
							(this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[ZERO_8917_BIN_FILE_I].b_Enable_DL == true))
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = true;
						}
						else
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = false;
						}
					}
					break;
				case MSM8937://���Ƿ���fs_image��zero�ļ�����
					{
						if((this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[FS_IMAGE_8937_IMG_FILE_I].b_Enable_DL == true) ||
							(this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[ZERO_8937_BIN_FILE_I].b_Enable_DL == true))
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = true;
						}
						else
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = false;
						}
					}
					break;
				case MSM8953://���Ƿ���fs_image��zero�ļ�����
					{
						if((this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[FS_IMAGE_8953_IMG_FILE_I].b_Enable_DL == true) ||
							(this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[ZERO_8953_BIN_FILE_I].b_Enable_DL == true))
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = true;
						}
						else
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = false;
						}
					}
					break;
				case SM6115://���Ƿ���fs_image��zero�ļ�����
					{
						if((this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[FS_IMAGE_SM6115_IMG_FILE_I].b_Enable_DL == true) ||
							(this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[ZERO_SM6115_BIN_FILE_I].b_Enable_DL == true))
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = true;
						}
						else
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = false;
						}
					}
					break;

					
				case SDM660://���Ƿ���fs_image��zero�ļ�����
					{
						if((this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[FS_IMAGE_SDM660_IMG_FILE_I].b_Enable_DL == true) ||
							(this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[ZERO_SDM660_BIN_FILE_I].b_Enable_DL == true))
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = true;
						}
						else
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = false;
						}
					}
					break;
				case SDM845://���Ƿ���fs_image��zero�ļ�����
					{
						if((this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[FS_IMAGE_SDM845_IMG_FILE_I].b_Enable_DL == true) ||
							(this->m_cla_ARM_OP_QCM_Main.m_strr_Mbn_File_List[ZERO_SDM845_BIN_FILE_I].b_Enable_DL == true))
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = true;
						}
						else
						{
							this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = false;
						}
					}
					break;
				default:
					this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = false;
					break;
				}








				this->m_cla_ARM_OP_QCM_Main.m_bQCN_DL = this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_DL.b_Enable_DL;

				this->m_cla_ARM_OP_QCM_Main.m_bQCN_BACKUP = false;
				this->m_cla_ARM_OP_QCM_Main.m_bQCN_RESTORE = false;

				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Name.Format(_T("QCN-B"));
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Path.Format(_T(""));
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.b_Enable_DL = false;

				//��fastbootģʽ����yaffs2��img�ļ�
				this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = false;

				for(i=0; i<MAX_YAFFS2_FILE; i++)
				{
					if(this->m_cla_ARM_OP_QCM_Main.m_strr_Yaffs2_File_List[i].b_Enable_DL == true)
					{
						this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = true;
						break;
					}
				}

				for(i=0; i<MAX_IMG_FILE; i++)
				{
					if(this->m_cla_ARM_OP_QCM_Main.m_strr_Img_File_List[i].b_Enable_DL == true)
					{
						this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = true;
						break;
					}
				}
			}
			break;
		case QCMM_OPTION_BACKUP_ONLY://���ڹ��ߣ�֧�ֵ�����������
			{
				this->m_cla_ARM_OP_QCM_Main.m_bFW_DL = false;
				this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = false;
				this->m_cla_ARM_OP_QCM_Main.m_bQCN_DL = false;

				this->m_cla_ARM_OP_QCM_Main.m_bQCN_BACKUP = true;
				this->m_cla_ARM_OP_QCM_Main.m_bQCN_RESTORE = false;

				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Name.Format(_T("QCN-BACKUP"));
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Path.Format(_T("%s"), this->m_csQCN_Temp_File_Path);//���ļ���չ��
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.b_Enable_DL = true;

				this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = false;
			}
			break;
		case QCMM_OPTION_RESTORE_ONLY://���ڹ��ߣ�֧�ֵ����������
			{
				this->m_cla_ARM_OP_QCM_Main.m_bFW_DL = false;
				this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = false;
				this->m_cla_ARM_OP_QCM_Main.m_bQCN_DL = false;

				this->m_cla_ARM_OP_QCM_Main.m_bQCN_BACKUP = false;
				this->m_cla_ARM_OP_QCM_Main.m_bQCN_RESTORE = true;

				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Name.Format(_T("QCN-RESTORE"));
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Path.Format(_T("%s"), this->m_csQCN_Temp_File_Path);//���ļ���չ��
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.b_Enable_DL = true;

				this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = false;
			}
			break;
		case QCMM_OPTION_UPGRADE_BACKUP://���ڹ��ߣ���������������FW���������
			{
				this->m_cla_ARM_OP_QCM_Main.m_bFW_DL = true;
				this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = true;
				this->m_cla_ARM_OP_QCM_Main.m_bQCN_DL = false;

				this->m_cla_ARM_OP_QCM_Main.m_bQCN_BACKUP = true;
				this->m_cla_ARM_OP_QCM_Main.m_bQCN_RESTORE = true;

				//��ʱ���뵼���ļ�����ȷ���ļ���չ��
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Name.Format(_T("QCN-B"));
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Path.Format(_T("%s\\qcn_backup"), this->m_csCurr_Exe_Path);//���ļ���չ��
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.b_Enable_DL = true;

				//��fastbootģʽ����yaffs2��img�ļ�
				this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = false;

				for(i=0; i<MAX_YAFFS2_FILE; i++)
				{
					if(this->m_cla_ARM_OP_QCM_Main.m_strr_Yaffs2_File_List[i].b_Enable_DL == true)
					{
						this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = true;
						break;
					}
				}

				for(i=0; i<MAX_IMG_FILE; i++)
				{
					if(this->m_cla_ARM_OP_QCM_Main.m_strr_Img_File_List[i].b_Enable_DL == true)
					{
						this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = true;
						break;
					}
				}
			}
			break;
		case QCMM_OPTION_UPGRADE_RELOAD://���ڹ��ߣ���������������FW������������ٵ���FW�Դ���QCN�ļ�����QCN-A������Quectel���ã�
			{
				this->m_cla_ARM_OP_QCM_Main.m_bFW_DL = true;
				this->m_cla_ARM_OP_QCM_Main.m_bCEFS_DL = true;
				this->m_cla_ARM_OP_QCM_Main.m_bQCN_DL = true;

				this->m_cla_ARM_OP_QCM_Main.m_bQCN_BACKUP = true;
				this->m_cla_ARM_OP_QCM_Main.m_bQCN_RESTORE = true;

				//��ʱ���뵼���ļ�����ȷ���ļ���չ��
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Name.Format(_T("QCN-B"));
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.cs_Path.Format(_T("%s\\qcn_backup"), this->m_csCurr_Exe_Path);//���ļ���չ��
				this->m_cla_ARM_OP_QCM_Main.m_str_Mbn_QCN_BU_RS.b_Enable_DL = true;

				//��fastbootģʽ����yaffs2��img�ļ�
				this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = false;

				for(i=0; i<MAX_YAFFS2_FILE; i++)
				{
					if(this->m_cla_ARM_OP_QCM_Main.m_strr_Yaffs2_File_List[i].b_Enable_DL == true)
					{
						this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = true;
						break;
					}
				}

				for(i=0; i<MAX_IMG_FILE; i++)
				{
					if(this->m_cla_ARM_OP_QCM_Main.m_strr_Img_File_List[i].b_Enable_DL == true)
					{
						this->m_cla_ARM_OP_QCM_Main.m_bFastboot_DL = true;
						break;
					}
				}
			}
			break;
		default:
			break;
		}

		//---------------------------- ���������Ҫ���صĸ�ͨ�ļ���������ʾ�����ļ��嵥��
		this->Load_View_Bin_List_QCMM();

		//---------------------------- �ж��ò����£��Ƿ�����֮ƥ���FW�ļ�����
		if(this->m_cla_ARM_OP_QCM_Main.Judge_Ctrl_OP_Option() == false)
		{
			throw(0);
		}

		//---------------------------- ��ȡ�����ļ����ܴ�С
		if(this->m_cla_ARM_OP_QCM_Main.m_bFW_DL == true)
		{
			if(!this->m_cla_ARM_OP_QCM_Main.GET_FW_SIZE_QCM())
			{
				this->Show_Status(this->m_cla_ARM_OP_QCM_Main.m_csError_Code);
				throw(0);
			}
		}

		//---------------------------- ���ý����������ݲ���ѡ����������Ȳ�ͬ��
		this->Init_Progress_Ctrl(PF_QUALCOMM, -1);
	}
	catch(...)
	{
		return false;
	}

	this->Show_Status(_T(""));
	return true;
}

LRESULT CDlg_Example::OnGetMsg_Hot_Key(WPARAM wParam , LPARAM lParam)
{
	switch((int)wParam)
	{
	case HOTKEY_DLOAD_MSG:
		::SendMessage(this->m_hWnd, WM_HOTKEY_DLOAD_MSG, NULL, NULL);
		::SendMessage(this->m_hWnd, WM_HOTKEY_UPDATE_GEN_MSG, NULL, NULL);
		break;
	case HOTKEY_OPTION_MSG:
		::SendMessage(this->m_hWnd, WM_HOTKEY_OPTION_MSG, NULL, NULL);
		::SendMessage(this->m_hWnd, WM_HOTKEY_UPDATE_GEN_MSG, NULL, NULL);
		break;
	case HOTKEY_DEBUG_MSG:
		::SendMessage(this->m_hWnd, WM_HOTKEY_DEBUG_MSG, NULL, NULL);
		::SendMessage(this->m_hWnd, WM_HOTKEY_UPDATE_GEN_MSG, NULL, NULL);
		break;
	case HOTKEY_DLOAD_PORT_MSG:
		::SendMessage(this->m_hWnd, WM_HOTKEY_DLOAD_PORT_MSG, NULL, NULL);
		::SendMessage(this->m_hWnd, WM_HOTKEY_UPDATE_GEN_MSG, NULL, NULL);
		break;

	case HOTKEY_FIREHOSE_MSG:
		::SendMessage(this->m_hWnd, WM_HOTKEY_FIREHOSE_MSG, NULL, NULL);
		::SendMessage(this->m_hWnd, WM_HOTKEY_UPDATE_GEN_MSG, NULL, NULL);
		break;

	case HOTKEY_NB_BC95_PARA_MSG:
		::SendMessage(this->m_hWnd, WM_HOTKEY_NB_BC95_PARA_MSG, NULL, NULL);
		::SendMessage(this->m_hWnd, WM_HOTKEY_UPDATE_GEN_MSG, NULL, NULL);
		break;

	case HOTKEY_OPTION_MSG_1:
		::SendMessage(this->m_hWnd, WM_HOTKEY_OPTION_MSG_1, NULL, NULL);
		::SendMessage(this->m_hWnd, WM_HOTKEY_UPDATE_GEN_MSG, NULL, NULL);
		break;

	case HOTKEY_OPTION_MSG_METHOD:
		::SendMessage(this->m_hWnd, WM_HOTKEY_OPTION_MSG_METHOD, NULL, NULL);
		::SendMessage(this->m_hWnd, WM_HOTKEY_UPDATE_GEN_MSG, NULL, NULL);
		break;

		

	default:
		break;
	}

	return LRESULT();
}

int CDlg_Example::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//��ϵͳ�Ǽ��ȼ�
	::RegisterHotKey(this->m_hWnd, HOTKEY_DLOAD_MSG, MOD_SHIFT | MOD_ALT, 'E');
	::RegisterHotKey(this->m_hWnd, HOTKEY_OPTION_MSG, MOD_SHIFT | MOD_ALT, 'M');
	::RegisterHotKey(this->m_hWnd, HOTKEY_DEBUG_MSG, MOD_SHIFT | MOD_ALT, 'D');
	::RegisterHotKey(this->m_hWnd, HOTKEY_DLOAD_PORT_MSG, MOD_SHIFT | MOD_ALT, 'P');
	::RegisterHotKey(this->m_hWnd, HOTKEY_FIREHOSE_MSG, MOD_SHIFT | MOD_ALT, 'H');
	::RegisterHotKey(this->m_hWnd, HOTKEY_NB_BC95_PARA_MSG, MOD_SHIFT | MOD_ALT, 'N');
	::RegisterHotKey(this->m_hWnd, HOTKEY_OPTION_MSG_1, MOD_SHIFT | MOD_ALT, 'A');
	::RegisterHotKey(this->m_hWnd, HOTKEY_OPTION_MSG_METHOD, MOD_SHIFT | MOD_ALT, 'T');
	

	return 0;
}

LRESULT CDlg_Example::OnGetMsg_Hot_Key_DLoad(WPARAM wParam, LPARAM lParam)
{
	//DLoadֻ�и�ͨƽ̨����
	if(this->m_iPlatform_Curr != PF_QUALCOMM)
	{
		return LRESULT();
	}

	if(this->m_bEnable_Emerg_DLoad == true)
	{
		//����
		this->m_bEnable_Emerg_DLoad = false;
		this->m_ctr_staCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
		this->m_ctr_cbxCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
		//�����󣬸ÿؼ�ֵ�ָ�Ĭ��ֵ���������³�ʼ��
		this->INIT();
	}
	else
	{
		//��ʾ
		this->m_bEnable_Emerg_DLoad = true;
		this->m_ctr_staCtrl_Dl_Type_S.ShowWindow(SW_SHOW);
		this->m_ctr_cbxCtrl_Dl_Type_S.ShowWindow(SW_SHOW);
		this->m_ctr_cbxCtrl_Dl_Type_S.SetCurSel(this->m_iCtrl_Dl_Type_S);
	}

	return LRESULT();
}


LRESULT CDlg_Example::OnGetMsg_Hot_Key_Firehose(WPARAM wParam, LPARAM lParam)
{
	//Firehose���ط�ʽֻ�и�ͨƽ̨����
	if(this->m_iPlatform_Curr != PF_QUALCOMM)
	{
		return LRESULT();
	}

	if(this->m_bFirehsoeFlag == TRUE)
	{
		//����
		this->m_bFirehsoeFlag = FALSE;
		this->m_ctrlFirehoseReset.ShowWindow(SW_HIDE);
		this->m_ctrlSaharaDL.ShowWindow(SW_HIDE);
		//�����󣬸ÿؼ�ֵ�ָ�Ĭ��ֵ���������³�ʼ��
		this->INIT();
	}
	else
	{
		//��ʾ
		this->m_bFirehsoeFlag = TRUE;
		this->m_ctrlFirehoseReset.ShowWindow(SW_SHOW);
		this->m_ctrlSaharaDL.ShowWindow(SW_HIDE);//�ù��ܲ���Ҫ----20181206
	}

	return LRESULT();
}

LRESULT CDlg_Example::OnGetMsg_Hot_Key_NB_BC95_PARA(WPARAM wParam, LPARAM lParam)
{
	//��˼NB����
	if(this->m_iPlatform_Curr != PF_HUAWEI)
	{
		return LRESULT();
	}

	if(this->m_bNB_BC95_Flag == TRUE)
	{
		//����
		this->m_bNB_BC95_Flag = FALSE;
		this->m_ctrlNoBaudSwitch.ShowWindow(SW_HIDE);
		this->m_ctrlSSB_Update.ShowWindow(SW_HIDE);
		this->m_ctrlWrCon.ShowWindow(SW_HIDE);
		this->m_ctrlErase.ShowWindow(SW_HIDE);
		this->m_ctrlWKV.ShowWindow(SW_HIDE);
		this->m_ctrlFEC.ShowWindow(SW_HIDE);

		GetDlgItem(IDC_R_Version)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_R_IMEI)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_E_IMEI)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_W_IMEI)->ShowWindow(SW_HIDE);
		
		//�����󣬸ÿؼ�ֵ�ָ�Ĭ��ֵ���������³�ʼ��
		this->INIT();
	}
	else
	{
		//��ʾ
		this->m_bNB_BC95_Flag = TRUE;
		this->m_ctrlNoBaudSwitch.ShowWindow(SW_SHOW);
		//this->m_ctrlSSB_Update.ShowWindow(SW_SHOW); //�ݲ�֧��
		this->m_ctrlWrCon.ShowWindow(SW_HIDE);

		//���ṩ��������� 20200429
		//this->m_ctrlErase.ShowWindow(SW_SHOW);
		this->m_ctrlErase.ShowWindow(SW_HIDE);

		this->m_ctrlWKV.ShowWindow(SW_SHOW);
		this->m_ctrlFEC.ShowWindow(SW_SHOW);



		GetDlgItem(IDC_R_Version)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_R_IMEI)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_E_IMEI)->ShowWindow(SW_SHOW);

		//���ṩд�ŵ���� 20200429
		//GetDlgItem(IDC_W_IMEI)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_W_IMEI)->ShowWindow(SW_HIDE);
	}

	return LRESULT();
}


LRESULT CDlg_Example::OnGetMsg_Hot_Key_Option_1(WPARAM wParam, LPARAM lParam)
{
	if((this->m_iPlatform_Curr != PF_MTK) &&
		(this->m_iPlatform_Curr != PF_QUALCOMM) &&
		(this->m_iPlatform_Curr != PF_INTEL) &&
		(this->m_iPlatform_Curr != PF_ASR))
	{
		return LRESULT();
	}

	if(this->m_bEnable_Advanced_Option_1 == true)
	{
		//����
		this->m_bEnable_Advanced_Option_1 = false;
		this->m_ctr_cbxOCPU_Name_S.ShowWindow(SW_HIDE);
		//�����󣬸ÿؼ�ֵ�ָ�Ĭ��ֵ���������³�ʼ��
		this->INIT();
	}
	else
	{
		//��ʾ
		this->m_bEnable_Advanced_Option_1 = true;
		this->m_ctr_cbxOCPU_Name_S.ShowWindow(SW_SHOW);
		this->m_ctr_cbxOCPU_Name_S.SetCurSel(this->m_iOCPU_Name_S);
	}

	return LRESULT();
}
LRESULT CDlg_Example::OnGetMsg_Hot_Key_Option_Method(WPARAM wParam, LPARAM lParam)
{
	if(this->m_iPlatform_Curr != PF_MTK)
	{
		return LRESULT();
	}

	if(this->m_bEnable_Com_Method == true)
	{
		//����
		this->m_bEnable_Com_Method = false;
		this->m_ctr_ComMethod.ShowWindow(SW_HIDE);
		this->m_ctrl_Dl_Type.ShowWindow(SW_HIDE);

		this->m_CtrlClearNv.ShowWindow(SW_HIDE);

		//�����󣬸ÿؼ�ֵ�ָ�Ĭ��ֵ���������³�ʼ��
		this->INIT();
	}
	else
	{
		//��ʾ
		this->m_bEnable_Com_Method = true;
		this->m_ctr_ComMethod.ShowWindow(SW_SHOW);
		this->m_ctrl_Dl_Type.ShowWindow(SW_SHOW);
		this->m_CtrlClearNv.ShowWindow(SW_SHOW);

		this->m_ctr_ComMethod.SetCurSel(this->m_iComMethod);
	}

	return LRESULT();
}





LRESULT CDlg_Example::OnGetMsg_Hot_Key_Option(WPARAM wParam, LPARAM lParam)
{
	if((this->m_iPlatform_Curr != PF_MTK) &&
		(this->m_iPlatform_Curr != PF_QUALCOMM) &&
		(this->m_iPlatform_Curr != PF_INTEL))
	{
		return LRESULT();
	}

	if(this->m_bEnable_Advanced_Option == true)
	{
		//����
		this->m_bEnable_Advanced_Option = false;
		this->m_ctr_staCtrl_OP_Option_S.ShowWindow(SW_HIDE);
		this->m_ctr_cbxCtrl_OP_Option_S.ShowWindow(SW_HIDE);
		//�����󣬸ÿؼ�ֵ�ָ�Ĭ��ֵ���������³�ʼ��
		this->INIT();
	}
	else
	{
		//��ʾ
		this->m_bEnable_Advanced_Option = true;
		this->m_ctr_staCtrl_OP_Option_S.ShowWindow(SW_SHOW);
		this->m_ctr_cbxCtrl_OP_Option_S.ShowWindow(SW_SHOW);
		this->m_ctr_cbxCtrl_OP_Option_S.SetCurSel(this->m_iCtrl_OP_Option_S);
	}

	return LRESULT();
}

LRESULT CDlg_Example::OnGetMsg_Hot_Key_Get_Debug_Log(WPARAM wParam, LPARAM lParam)
{
	//ץDEBUG LOGֻ��MTKƽ̨����
	if((this->m_iPlatform_Curr != PF_MTK) && (this->m_iPlatform_Curr != PF_INTEL) && (this->m_iPlatform_Curr != PF_QUALCOMM))
	{
		return LRESULT();
	}

	if(this->m_bEnable_Get_Debug_Log == true)
	{
		//����
		this->m_bEnable_Get_Debug_Log = false;
		this->m_ctr_cekGet_Debug_Log.ShowWindow(SW_HIDE);
		//�����󣬸ÿؼ�ֵ�ָ�Ĭ��ֵ���������³�ʼ��
		this->INIT();
	}
	else
	{
		//��ʾ
		this->m_bEnable_Get_Debug_Log = true;
		this->m_ctr_cekGet_Debug_Log.ShowWindow(SW_SHOW);
	}

	return LRESULT();
}

void CDlg_Example::Load_View_Option(int i_Platform)
{
	int iTemp = this->m_iCtrl_OP_Option_S;//������ǰѡ���


	this->m_ctr_ComMethod.ShowWindow(SW_HIDE);
	this->m_ctrl_Dl_Type.ShowWindow(SW_HIDE);

	//���Option�б���պ�Index���Ϊ��-1����
	while(this->m_ctr_cbxCtrl_OP_Option_S.GetCount())
	{
		this->m_ctr_cbxCtrl_OP_Option_S.DeleteString(0);
	}

	//������ѡ��
	switch(i_Platform)
	{
	case PF_MTK:
		{
			if (BB_MT2625 != m_iBBchip_Curr)
			{
				this->m_ctr_cbxCtrl_OP_Option_S.AddString(_T("Download FW and Format"));
				this->m_ctr_cbxCtrl_OP_Option_S.AddString(_T("Upgrade FW Only"));
				this->m_ctr_cbxCtrl_OP_Option_S.AddString(_T("Upgrade FW and Clear User Data"));

				if(iTemp >= 3)
				{
					iTemp = MTK_OPTION_DL_CLEAR_NV;
				}

				//MTKƽ̨û��DLoad
				this->m_ctr_staCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
				this->m_ctr_cbxCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
				this->m_ctr_staCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
				this->m_ctr_cbxCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
			}
			else
			{
				//...
			}
		}
		break;

	case PF_QUALCOMM:
		{
			this->m_ctr_cbxCtrl_OP_Option_S.AddString(_T("Download MBN & QCN"));
			this->m_ctr_cbxCtrl_OP_Option_S.AddString(_T("Backup QCN"));
			this->m_ctr_cbxCtrl_OP_Option_S.AddString(_T("Restore QCN"));
			this->m_ctr_cbxCtrl_OP_Option_S.AddString(_T("Backup & Restore QCN + Download MBN"));
			this->m_ctr_cbxCtrl_OP_Option_S.AddString(_T("Backup & Restore QCN + Download MBN & QCN"));

			if(iTemp >= 5)
			{
				iTemp = QCMM_OPTION_DL_AUTO;
			}

			//��ͨƽ̨û��Debug Log
			this->m_ctr_cekGet_Debug_Log.ShowWindow(SW_HIDE);
			//��ͨƽ̨û��OCPU
			this->m_ctr_staOCPU_Name_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxOCPU_Name_S.ShowWindow(SW_HIDE);
		}
		break;
	case PF_RDA:
		{
			//RDAƽ̨û��DLoad
			this->m_ctr_staCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
			this->m_ctr_staCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
			//RDAƽ̨û��Debug Log
			this->m_ctr_cekGet_Debug_Log.ShowWindow(SW_HIDE);
			//RDAƽ̨û��OCPU
			this->m_ctr_staOCPU_Name_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxOCPU_Name_S.ShowWindow(SW_HIDE);
		}
		break;

	case PF_INTEL:
		{
			this->m_ctr_cbxCtrl_OP_Option_S.AddString(_T("Download FW"));
			this->m_ctr_cbxCtrl_OP_Option_S.AddString(_T("Download FW and Format"));

			if(iTemp >= 2)
			{
				iTemp = INTEL_OPTION_DL_ONLY;
			}

			//Intelƽ̨û��DLoad
			this->m_ctr_staCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
			this->m_ctr_staCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
			//Intelƽ̨û��OCPU
			this->m_ctr_staOCPU_Name_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxOCPU_Name_S.ShowWindow(SW_HIDE);
		}
		break;
	case PF_HUAWEI:
		{
			//NBƽ̨û��DLoad
			this->m_ctr_staCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
			this->m_ctr_staCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
			//NBƽ̨û��Debug Log
			this->m_ctr_cekGet_Debug_Log.ShowWindow(SW_HIDE);
			//NBƽ̨û��OCPU
			this->m_ctr_staOCPU_Name_S.ShowWindow(SW_HIDE);
			this->m_ctr_cbxOCPU_Name_S.ShowWindow(SW_HIDE);
		}
		break;
	case PF_ASR:
		//ƽ̨û��DLoad
		this->m_ctr_staCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
		this->m_ctr_cbxCtrl_Dl_Type_S.ShowWindow(SW_HIDE);
		this->m_ctr_staCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
		this->m_ctr_cbxCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
		//ƽ̨û��Debug Log
		this->m_ctr_cekGet_Debug_Log.ShowWindow(SW_HIDE);
		//ƽ̨û��OCPU
		this->m_ctr_staOCPU_Name_S.ShowWindow(SW_HIDE);
		this->m_ctr_cbxOCPU_Name_S.ShowWindow(SW_HIDE);
		break;
	default:
		return;
	}

	//��ԭѡ���
	this->m_iCtrl_OP_Option_S = iTemp;
	this->m_ctr_cbxCtrl_OP_Option_S.SetCurSel(this->m_iCtrl_OP_Option_S);
}

bool CDlg_Example::Load_Advanced_Config_MTK(const wchar_t * wcp_Product)
{
	try
	{
		//---------------------------- ��ȡ���߼����ã��������ã����Ĳ���
		this->Get_Advanced_Config_Value(PF_MTK, wcp_Product);

		//20191130
		this->m_cla_Flash_OP_Main.m_iComm_Method = this->m_iComMethod;

// 		this->m_ctr_ComMethod.ShowWindow(SW_SHOW);
// 		this->m_ctrl_Dl_Type.ShowWindow(SW_SHOW);
// 		this->m_ctr_ComMethod.SetCurSel(m_iComMethod);

		//---------------------------- ���ݡ��߼����ã��������ã����Ĳ��������á�MTK�ࡱ�еġ�OP_TYPE���������������
		//MTK����ģʽ��Optionѡ���������
		if (MTK_OPTION_DL_FORMAT == this->m_iCtrl_OP_Option_S)
		{
			AfxMessageBox(_T("Formatting operations are not allowed"));
			throw(0);
		}
		this->m_cla_Flash_OP_Main.m_iOP_TYPE = this->m_iCtrl_OP_Option_S;
		//����MTKƽ̨LOG
		this->m_cla_Flash_OP_Main.Enable_Debug_Log(this->m_bGet_Debug_Log);

		//---------------------------- ���ý����������ݲ���ѡ����������Ȳ�ͬ��
		this->Init_Progress_Ctrl(PF_MTK, this->m_cla_Flash_OP_Main.m_iOP_TYPE);

		//---------------------------- ���������NV����������Ҫ��TYPE�ļ��б�
		if(this->m_cla_Flash_OP_Main.m_iOP_TYPE == MTK_OPTION_DL_CLEAR_NV)
		{
			//��Ҫ������NV�ļ��б�
			if(this->m_cla_Flash_OP_Main.Load_NV_Retain_File_List(this->m_iNv_Type, this->m_csCurr_Exe_Path) == false)
			{
				throw(0);
			}

			//��ʼ��META���
			if(this->m_cla_Flash_OP_Main.INIT_META_DLL() == false)
			{
				throw(0);
			}
		}
	}
	catch(...)
	{
		return false;
	}

	this->Show_Status(_T(""));
	return true;
}

bool CDlg_Example::Load_Advanced_Config_MTK_NB(void)
{
	try
	{
		//---------------------------- ���ý����������ݲ���ѡ����������Ȳ�ͬ��
		this->Init_Progress_Ctrl(PF_MTK, 0);

		this->m_cla_ARM_OP_NB_Main.m_iComMethod = this->m_iComMethod;
		this->m_cla_ARM_OP_NB_Main.m_iResetFlag = this->m_iResetFlag;
		this->m_cla_ARM_OP_NB_Main.m_iClearNVFlag= this->m_iClearNVFlag;

// 		this->m_ctr_ComMethod.ShowWindow(SW_SHOW);
// 		this->m_ctrl_Dl_Type.ShowWindow(SW_SHOW);
// 		this->m_ctr_ComMethod.SetCurSel(m_iComMethod);


	}
	catch(...)
	{
		return false;
	}

	this->Show_Status(_T(""));
	return true;
}

void CDlg_Example::OnCbnSelchangeComboCtrlDlTypeS()
{
	// TODO: Add your control notification handler code here
	if(this->m_bEnable_Emerg_DLoad && (this->m_iPlatform_Curr == PF_QUALCOMM))
	{
		this->UpdateData(TRUE);

		this->INIT();
	}
}

void CDlg_Example::OnCbnSelchangeComboCtrlOpOptionS()
{
	// TODO: Add your control notification handler code here
	if(this->m_bEnable_Advanced_Option == true)
	{
		this->UpdateData(TRUE);

		this->INIT();
	}
}

void CDlg_Example::OnBnClickedCheckGetDebugLog()
{
	// TODO: Add your control notification handler code here
	if(this->m_bEnable_Get_Debug_Log && ((this->m_iPlatform_Curr == PF_MTK) || (this->m_iPlatform_Curr == PF_INTEL)))
	{
		this->UpdateData(TRUE);

		this->INIT();
	}
}

void CDlg_Example::Get_Advanced_Config_Value(int i_Platform, const wchar_t * wcp_Product)
{
	//---------------------------- �����Ƿ���ˡ��������á����趨���߼����ã��������ã����Ĳ���
	//Optionѡ��
	if(this->m_bEnable_Advanced_Option == true)
	{
		//���Ѵ��������ã����ǡ��ֶ�����״̬�������Խ���ؼ��Ĳ���Ϊ׼
		this->m_iCtrl_OP_Option_S = this->m_ctr_cbxCtrl_OP_Option_S.GetCurSel();		
	}
	else
	{
		//��û�д��������ã��򡰸߼����ã��������ã�������Ĭ��ֵ
		switch(i_Platform)
		{
		case PF_MTK:
			//ע�⣺���ڹ��ߣ�MTKƽ̨������ �� ������ݲ���
			//MTKƽ̨��Optionѡ���Ǵ������ļ��ж�ȡ��
			this->m_iCtrl_OP_Option_S = this->m_cla_iniParams.ReadInt(wcp_Product, _T("OP_TYPE"), MTK_OPTION_DL_CLEAR_NV);

			break;
		case PF_QUALCOMM:
			this->m_iCtrl_OP_Option_S = QCMM_OPTION_DL_AUTO;
			break;
		case PF_INTEL:
			this->m_iCtrl_OP_Option_S = INTEL_OPTION_DL_ONLY;
			break;
		default:
			return;
		}
	}

	//��������ѡ��
	if(this->m_bEnable_Emerg_DLoad == true)
	{
		//���Ѵ��������ã����ǡ��ֶ�����״̬�������Խ���ؼ��Ĳ���Ϊ׼
		this->m_iCtrl_Dl_Type_S = this->m_ctr_cbxCtrl_Dl_Type_S.GetCurSel();	
	}
	else
	{
		//��û�д��������ã��򡰸߼����ã��������ã�������Ĭ��ֵ
		this->m_iCtrl_Dl_Type_S = QCMM_DL_TYPE_AUTO;
	}

	//ץDebug Log
	if(this->m_bEnable_Get_Debug_Log == true)
	{
		this->m_bGet_Debug_Log = (this->m_ctr_cekGet_Debug_Log.GetCheck() == BST_CHECKED)? true : false;	
	}
	else
	{
		this->m_bGet_Debug_Log = false;
	}

	//��ͨ���ؿ�
	if(this->m_bEnable_DLoad_Port == true)
	{
		//���Ѵ��������ã����ǡ��ֶ�����״̬�������Խ���ؼ��Ĳ���Ϊ׼
		this->m_iCtrl_DLoad_Port_S = this->m_ctr_cbxCtrl_DLoad_Port_S.GetCurSel();	
	}
	else
	{
		//��û�д��������ã��򡰸߼����ã��������ã�������Ĭ��ֵ
		this->m_iCtrl_DLoad_Port_S = QCMM_DL_PORT_AUTO;
	}
}

LRESULT CDlg_Example::OnGetMsg_Hot_Key_DLoad_Port(WPARAM wParam, LPARAM lParam)
{
	//DLoad_Portֻ�и�ͨƽ̨����
	if(this->m_iPlatform_Curr != PF_QUALCOMM)
	{
		return LRESULT();
	}

	if(this->m_bEnable_DLoad_Port == true)
	{
		//����
		this->m_bEnable_DLoad_Port = false;
		this->m_ctr_staCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
		this->m_ctr_cbxCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
		//�����󣬸ÿؼ�ֵ�ָ�Ĭ��ֵ���������³�ʼ��
		this->INIT();
	}
	else
	{
		//��ʾ
		this->m_bEnable_DLoad_Port = true;
		this->m_ctr_staCtrl_DLoad_Port_S.ShowWindow(SW_HIDE);
		this->m_ctr_cbxCtrl_DLoad_Port_S.ShowWindow(SW_SHOW);	
		this->m_ctr_cbxCtrl_DLoad_Port_S.SetCurSel(this->m_iCtrl_DLoad_Port_S);
	}

	return LRESULT();
}

void CDlg_Example::OnCbnSelchangeComboCtrlDlPortS()
{
	// TODO: Add your control notification handler code here
	if(this->m_bEnable_DLoad_Port && (this->m_iPlatform_Curr == PF_QUALCOMM))
	{
		this->UpdateData(TRUE);

		this->INIT();
	}
}

LRESULT CDlg_Example::OnGetMsg_Hot_Key_Update_Gen(WPARAM wParam, LPARAM lParam)
{
	if(this->m_bEnable_Advanced_Option || this->m_bEnable_Emerg_DLoad ||
		this->m_bEnable_Get_Debug_Log || this->m_bEnable_DLoad_Port)
	{
		this->m_ctr_staConfig_Adv.SetWindowTextW(_T("Config Advanced"));
	}
	else
	{
		this->m_ctr_staConfig_Adv.SetWindowTextW(_T("Config"));
	}

	return LRESULT();
}

bool CDlg_Example::Load_All_DL_Files_RDA(void)
{
	CString csData;

	try
	{
		//���LOD�ļ��ṹ�壬������ô��գ�����ṹ����������
	//	memset(&this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW, 0 , sizeof(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW));
	//	memset(&this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun, 0 , sizeof(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun));

		//��ǰ·����Ϊ����ʶ��flash��̬����ramrun�ļ�ʹ��
		this->m_cla_ARM_OP_RDA_Main.m_csCurr_Exe_Path.Format(_T("%s"), this->m_csCurr_Exe_Path);


		//--------------------------------------- FW �ļ� --------------------------------------- 
		//����FW LOD�ļ�·����������
		this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Name.Format(_T("FW"));
		this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path.Format(_T("%s"), this->m_csScat_Cfg_File_Path);


		/*
		//��ȡLod�ļ��еĻ���������Ϣ
		if(this->m_cla_ARM_OP_RDA_Main.Load_Lod_File_Info() == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
			throw(0);
		}
		*/


		//��ȡLod�ļ��еġ�#$��������Ϣ
		if(this->m_cla_ARM_OP_RDA_Main.Load_Lod_File_Params(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path) == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
			throw(0);
		}

		//��ȡLod�ļ��еġ�#$XCV_MODEL��������Ϣ 20190128	
		//#$XCV_MODEL=xcv_8809 
		if(this->m_cla_ARM_OP_RDA_Main.Get_Lod_File_XCV_MODEL_Params(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path) == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
			throw(0);
		}

		//20190227
		//#$FS_ROOT_BASE=0x390000
		//#$FS_ROOT_SIZE=0x66000
		int iFS_ROOT_BASE =0;
		if(this->m_cla_ARM_OP_RDA_Main.Get_Lod_File_FS_ROOT_BASE_Params(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path,&iFS_ROOT_BASE) == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
			throw(0);
		}

		if(this->m_cla_ARM_OP_RDA_Main.Get_Lod_File_FS_ROOT_SIZE_Params(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path) == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
			throw(0);
		}
		//--------------------------------------------------------------------------------------------------------------------
		//#$BOOT_SECTOR=0x10000
		int iBOOT_SECTOR =0;
		if(this->m_cla_ARM_OP_RDA_Main.Get_Lod_File_BOOT_SECTOR_Params(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path,&iBOOT_SECTOR) == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
			throw(0);
		}
		if ((iFS_ROOT_BASE>0)&&(iBOOT_SECTOR>0))
		{
			this->m_cla_ARM_OP_RDA_Main.m_csError_Code.Format(_T("This lod file cannot be upgraded"));
			this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
			throw(0);

		}
		//--------------------------------------------------------------------------------------------------------------------

		this->m_cla_ARM_OP_RDA_Main.m_chipe_flag=0;//��ʼ��
		//if ((this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path.Find(_T("MC25"))>0)
		//	||(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path.Find(_T("M25"))>0))
		if (this->m_cla_ARM_OP_RDA_Main.m_csLod_XCV_MODEL.Find(_T("8955"))>0)
		{
			//��Params_Main.ini�ļ��ж���ramrun�ļ���
			csData = this->m_cla_iniParams.ReadStr(_T("MC25_FLSH_SPI32M"), _T("ramrun"), _T(""));


			//--------------------------------------- ramrun �ļ� --------------------------------------- 
			//����ramrun LOD�ļ�·����������
			this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.cs_Name.Format(_T("RamRun"));
			this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.cs_Path.Format(_T("%s\\ramrun\\%s.lod"), this->m_csCurr_Exe_Path, csData);


			this->m_cla_ARM_OP_RDA_Main.m_iCfgFileNum= 0;

			this->m_cla_ARM_OP_RDA_Main.Find_lod_File(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path);

			//���������Ҫ���ص�LOD�ļ���������ʾ�����ļ��嵥��
			this->Load_View_Bin_List_RDA();

			if (this->m_cla_ARM_OP_RDA_Main.m_iCfgFileNum >0)
			{
				this->Save_Trace_Log_File(_T("need dl multiple lod fils"));

				//����������FW LOD�ļ�
				//-------------------------------------------20191227----------------------------
				if(this->m_cla_ARM_OP_RDA_Main.LOAD_LOD_FILE_Ext_Sub(&this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW) == false)
				{
					this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
					throw(0);
				}
			}
			else
			{
				//����������FW LOD�ļ�
				if(this->m_cla_ARM_OP_RDA_Main.LOAD_LOD_FILE_Ext(&this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW) == false)
				{
					this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
					throw(0);
				}
			}

		} 	
		//else if(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path.Find(_T("BC25"))>0)
		else if((this->m_cla_ARM_OP_RDA_Main.m_csLod_XCV_MODEL.Find(_T("8909"))>0)
			      &&(this->m_cla_ARM_OP_RDA_Main.m_csLod_Product.Find(_T("32"))>0))
		{
			//��Params_Main.ini�ļ��ж���ramrun�ļ���
			csData = this->m_cla_iniParams.ReadStr(_T("BC25_FLSH_SPI32M"), _T("ramrun"), _T(""));


			//--------------------------------------- ramrun �ļ� --------------------------------------- 
			//����ramrun LOD�ļ�·����������
			this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.cs_Name.Format(_T("RamRun"));
			this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.cs_Path.Format(_T("%s\\ramrun\\%s.lod"), this->m_csCurr_Exe_Path, csData);


			//������ʾLOD�ļ�
			this->Load_View_Bin_List_RDA();
			//����������FW LOD�ļ�
			if(this->m_cla_ARM_OP_RDA_Main.LOAD_LOD_FILE_Ext(&this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW) == false)
			{
				this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
				throw(0);
			}
		}
		//else if(this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path.Find(_T("BC32"))>0)
		else if((this->m_cla_ARM_OP_RDA_Main.m_csLod_XCV_MODEL.Find(_T("8909"))>0)
		        &&(this->m_cla_ARM_OP_RDA_Main.m_csLod_Product.Find(_T("64"))>0))
		{
			//��Params_Main.ini�ļ��ж���ramrun�ļ���
			csData = this->m_cla_iniParams.ReadStr(_T("BC32_FLSH_SPI64M"), _T("ramrun"), _T(""));


			//--------------------------------------- ramrun �ļ� --------------------------------------- 
			//����ramrun LOD�ļ�·����������
			this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.cs_Name.Format(_T("RamRun"));
			this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.cs_Path.Format(_T("%s\\ramrun\\%s.lod"), this->m_csCurr_Exe_Path, csData);


			//������ʾLOD�ļ�
			this->Load_View_Bin_List_RDA();
			//����������FW LOD�ļ�
			if(this->m_cla_ARM_OP_RDA_Main.LOAD_LOD_FILE_Ext(&this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW) == false)
			{
				this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
				throw(0);
			}
		}
		else if(this->m_cla_ARM_OP_RDA_Main.m_csLod_XCV_MODEL.Find(_T("8809"))>0)
		{
			//��Params_Main.ini�ļ��ж���ramrun�ļ���
			csData = this->m_cla_iniParams.ReadStr(this->m_cla_ARM_OP_RDA_Main.m_csLod_Product, _T("ramrun"), _T(""));


			//--------------------------------------- ramrun �ļ� --------------------------------------- 
			//����ramrun LOD�ļ�·����������
			this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.cs_Name.Format(_T("RamRun"));
			this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.cs_Path.Format(_T("%s\\ramrun\\%s.lod"), this->m_csCurr_Exe_Path, csData);


			//������ʾLOD�ļ�
			this->Load_View_Bin_List_RDA();
			//����������FW LOD�ļ�
			if(this->m_cla_ARM_OP_RDA_Main.LOAD_LOD_FILE(&this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW) == false)
			{
				this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
				throw(0);
			}


			//����������ramrun LOD�ļ�
			if(this->m_cla_ARM_OP_RDA_Main.LOAD_LOD_FILE(&this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun) == false)
			{
				this->Show_Status(this->m_cla_ARM_OP_RDA_Main.m_csError_Code);
				throw(0);
			}


			//�����ܹ�Ҫ���ص�LOD�ļ�byte����
			this->m_cla_ARM_OP_RDA_Main.m_ulAll_Bytes =
				this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.ul_File_Size + this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.ul_File_Size;
		}
		else //�����ȡ����оƬ���ͱ�FAIL
		{
			this->Show_Status(_T("Get XCV_MODEL Fail!"));
			AfxMessageBox(_T("Get XCV_MODEL Fail!"));
			throw(0);
		}




	}
	catch(...)
	{
		return false;
	}

	return true;
}

void CDlg_Example::Load_View_Bin_List_RDA(void)
{
	this->m_bLoad_Bin_List = true;

	//��ʾLOD�ļ�
	this->Add_ListCtrl_Bin_List_Data(
		this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.cs_Name,
		this->m_cla_ARM_OP_RDA_Main.m_str_Lod_RamRun.cs_Path,
		true);

	this->Add_ListCtrl_Bin_List_Data(
		this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Name,
		this->m_cla_ARM_OP_RDA_Main.m_str_Lod_FW.cs_Path,
		true);


	if (this->m_cla_ARM_OP_RDA_Main.m_iCfgFileNum >0)
	{
		int j =0;

		for(j=0; j<MAX_LOD_FILE; j++)
		{
			if (true == this->m_cla_ARM_OP_RDA_Main.m_strr_Lod_File_List[j].b_Enable_DL)
			{
				this->Add_ListCtrl_Bin_List_Data(
					this->m_cla_ARM_OP_RDA_Main.m_strr_Lod_File_List[j].cs_Name,
					this->m_cla_ARM_OP_RDA_Main.m_strr_Lod_File_List[j].cs_Path,
					true);
			} 
			else
			{
				//...
			}
		}

	}

	this->m_bLoad_Bin_List = false;
}

bool CDlg_Example::Load_Advanced_Config_RDA(void)
{
	//---------------------------- ���ý����������ݲ���ѡ����������Ȳ�ͬ��
	this->Init_Progress_Ctrl(PF_RDA, -1);

	this->Show_Status(_T(""));
	return true;
}
bool CDlg_Example::Load_Advanced_Config_HUAWEI(void)
{
	//��ʼ������Ϣ�����������ļ�
	if(this->m_cla_ARM_OP_HUAWEI_Main.LOAD_FW_FILE(this->m_csScat_Cfg_File_Path) == false)
	{
		this->Show_Status(this->m_cla_ARM_OP_HUAWEI_Main.m_csError_Code);
		throw(0);
	}
	//---------------------------- ���ý����������ݲ���ѡ����������Ȳ�ͬ��
	this->Init_Progress_Ctrl(PF_HUAWEI, -1);

	this->m_iBR_Main_Idx=0;
	this->m_iCom_Main_Idx=0;

	this->Show_Status(_T(""));

	//����ǰ.exe����·��
	this->m_cla_ARM_OP_HUAWEI_Main.m_csCurr_Exe_Path.Format(_T("%s"), this->m_csCurr_Exe_Path);


	//---------------------------- �ر�δ������ֹ�Ľ��̣�process��----------------------------
	this->m_cla_ARM_OP_HUAWEI_Main.m_cla_GFC.Kill_Process(_T("SsbLoader.exe"));
	this->m_cla_ARM_OP_HUAWEI_Main.m_cla_GFC.Kill_Process(_T("UEUpdater.exe"));
	this->m_cla_ARM_OP_HUAWEI_Main.m_cla_GFC.Kill_Process(_T("cmd.exe"));


	return true;
}
bool CDlg_Example::Load_Advanced_Config_ASR(void)
{
	//��ʼ������Ϣ�����������ļ�
	if(this->m_cla_ARM_OP_ASR_Main.LOAD_FW_FILE(this->m_csScat_Cfg_File_Path) == false)
	{
		this->Show_Status(this->m_cla_ARM_OP_ASR_Main.m_csError_Code);
		throw(0);
	}
	//---------------------------- ���ý����������ݲ���ѡ����������Ȳ�ͬ��
	this->Init_Progress_Ctrl(PF_ASR, -1);

	this->m_iBR_Main_Idx=0;
	this->m_iCom_Main_Idx=0;

	this->Show_Status(_T(""));

	//����ǰ.exe����·��
	this->m_cla_ARM_OP_ASR_Main.m_csCurr_Exe_Path.Format(_T("%s"), this->m_csCurr_Exe_Path);
	return true;
}

bool CDlg_Example::Load_All_DL_Files_HUAWEI(void)
{
	try
	{
		this->m_cla_ARM_OP_HUAWEI_Main.m_iBBchip =  this->m_iBBchip_Curr;

		this->m_cla_ARM_OP_HUAWEI_Main.m_csCurr_Exe_Path.Format(_T("%s"), this->m_csCurr_Exe_Path);//����ǰ.exe����·��

		//��ʼ������Ϣ������Intel�����ļ�
		if(this->m_cla_ARM_OP_HUAWEI_Main.LOAD_FW_FILE(this->m_csScat_Cfg_File_Path) == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_HUAWEI_Main.m_csError_Code);
			throw(0);
		}

		//����ǰ.exe����·��
		this->m_cla_ARM_OP_HUAWEI_Main.m_csCurr_Exe_Path.Format(_T("%s"), this->m_csCurr_Exe_Path);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CDlg_Example::Load_All_DL_Files_EIGEN(void)
{
	CString strFileName, strFilePath, strFwDir;
	try
	{
		m_cla_ARM_OP_EIGEN_Main.m_csCurr_Exe_Path = m_csCurr_Exe_Path;
		m_cla_ARM_OP_EIGEN_Main.m_cla_ConfigFile.m_csIniFileName.Format(_T("%s\\EFlashTool\\1\\config.ini"), m_csCurr_Exe_Path);

		m_cla_ARM_OP_EIGEN_Main.m_iBBchip_Curr = this->m_iBBchip_Curr;

		// ����firmware�ļ�
		if(!this->m_cla_ARM_OP_EIGEN_Main.LOAD_FW_FILE(m_csScat_Cfg_File_Path))
		{
			throw(0);
		}

		// config�ļ�
		if(this->m_cla_GFC.Get_File_Directory(m_csScat_Cfg_File_Path, &strFwDir) == false)
		{
			throw(0);
		}


		this->m_bLoad_Bin_List = true;
		//��ʾbin�ļ�
		int nPos = m_csScat_Cfg_File_Path.ReverseFind('\\');
		if (nPos != -1)
		{
			strFileName = m_csScat_Cfg_File_Path.Left(nPos);
			this->Add_ListCtrl_Bin_List_Data(
				_T("File_Path"),
				strFileName,
				true);

			// bootloader.bin
			strFilePath = m_cla_ARM_OP_EIGEN_Main.m_FWInfo.strBootLoader;
			nPos = strFilePath.ReverseFind('\\');
			if (nPos != -1)
			{
				strFileName = strFilePath.Right(strFilePath.GetLength() - nPos-1);
			}
			strFilePath.Format(_T("%s\\%s"), strFwDir, strFileName);
			this->Add_ListCtrl_Bin_List_Data(strFileName, strFilePath, true);
			// system
			strFilePath = m_cla_ARM_OP_EIGEN_Main.m_FWInfo.strSystem;
			nPos = strFilePath.ReverseFind('\\');
			if (nPos != -1)
			{
				strFileName = strFilePath.Right(strFilePath.GetLength() - nPos-1);
			}
			this->Add_ListCtrl_Bin_List_Data(strFileName, strFilePath, true);
#ifdef FW_FACTORY
			// RF
			strFilePath = m_cla_ARM_OP_EIGEN_Main.m_FWInfo.strRF;
			nPos = strFilePath.ReverseFind('\\');
			if (nPos != -1)
			{
				strFileName = strFilePath.Right(strFilePath.GetLength() - nPos-1);
			}
			this->Add_ListCtrl_Bin_List_Data(strFileName, strFilePath, true);
#endif
		}
		this->m_bLoad_Bin_List = false;


		//---------------------------- ���ý����������ݲ���ѡ����������Ȳ�ͬ��---------------------------- 
		this->Init_Progress_Ctrl(PF_EIGENCOMM, -1);
	}
	catch(...)
	{
		return false;
	}

	return true;

}

bool CDlg_Example::Load_All_DL_Files_BEKEN(void)
{
	CString strFileName, strFilePath, strFwDir;
	try
	{
		m_cla_ARM_OP_BEKEN_Main.m_csCurr_Exe_Path = m_csCurr_Exe_Path;
	
		// ����firmware�ļ�
		if (!this->m_cla_ARM_OP_BEKEN_Main.LOAD_FW_FILE(m_csScat_Cfg_File_Path))
		{
			throw(0);
		}

		this->m_bLoad_Bin_List = true;
		//��ʾ�ļ�
		int nPos = m_csScat_Cfg_File_Path.ReverseFind('\\');
		if (nPos != -1)
		{
			strFileName = m_csScat_Cfg_File_Path.Left(nPos);
			this->Add_ListCtrl_Bin_List_Data(
				_T("File_Path"),
				strFileName,
				true);

			// FC41DAAR01A01.bin
			strFilePath = m_cla_ARM_OP_BEKEN_Main.m_FWInfo.strBootLoader;
			nPos = strFilePath.ReverseFind('\\');
			if (nPos != -1)
			{
				strFileName = strFilePath.Right(strFilePath.GetLength() - nPos - 1);
			}
			strFilePath.Format(_T("%s\\%s"), strFwDir, strFileName);
			this->Add_ListCtrl_Bin_List_Data(strFileName, strFilePath, true);

		}
		this->m_bLoad_Bin_List = false;


		//---------------------------- ���ý����������ݲ���ѡ����������Ȳ�ͬ��---------------------------- 
		this->Init_Progress_Ctrl(PF_BEKEN, -1);
	}
	catch (...)
	{
		return false;
	}

	return true;

}

bool CDlg_Example::Load_All_DL_Files_UNISOC(void)
{
	try
	{
		CString strData;
		CHAR szErrorInfo[MAX_PATH] = {0};
		if (m_pDlCctrl == NULL)
		{
			// 20200108 ɾ��UFlashTool�ļ����µ�ImageFiles��ʱ�ļ���
			strData = m_csCurr_Exe_Path + _T("\\UFlashTool\\ImageFiles\\");
			DeleteFolder(strData);

			// ��ʼ��
			if (!CreateFlashToolObject(&m_pDlCctrl))
			{
				this->Show_Status(_T("ICreateFlashToolObject failed."));
				return false;
			}
			m_cbUIInterface.pIUI_Output_Text = Text_Output;
			m_cbUIInterface.pIUI_Update_Progress = Progress_Update;
			m_cbUIInterface.pUserData = this;
			m_pDlCctrl->RegisterUIInterface(&m_cbUIInterface);
			m_hwndUI = GetSafeHwnd();
			DWORD dwDownloadMode;
			BOOL bPowerOff = TRUE;
			WCHAR szLogPath[MAX_PATH] = {0};
			CString strLogPath = m_csCurr_Exe_Path + _T("\\log");
			BOOL bEraseAll = TRUE;
#ifdef FW_FACTORY
			dwDownloadMode = E_DOWNLOAD_MODE;
#else
			dwDownloadMode = E_UPGRADE_MODE;
#endif
			// download
			m_pDlCctrl->SetProperty(E_PROR_DOWNLOAD_MODE, &dwDownloadMode);
			// Power off device after finish download
			m_pDlCctrl->SetProperty(E_PROR_POWEROFF_DEV, &bPowerOff);
			// log path
			m_pDlCctrl->SetProperty(E_PROR_LOG_PATH,(LPVOID)(strLogPath.operator LPCTSTR())); 
			// whether reset after download
			if (m_cla_ARM_OP_SPREADTRUM_Main.m_bUnisoc_Reset_after_DL == TRUE)
			{
				int nResetMode = 0; // E_NORMAL_MODE
				m_pDlCctrl->SetProperty(E_PROR_RESET_DEV, &nResetMode); 
			}
		}


		DWORD dwRet = m_pDlCctrl->InitDownload(m_csScat_Cfg_File_Path.operator LPCTSTR());

		if(dwRet)
		{
			m_pDlCctrl->GetOprErrorCodeDescription(dwRet,szErrorInfo,MAX_PATH);
			USES_CONVERSION;
			strData.Format(_T("InitDownload FAIL[%s]"),A2T(szErrorInfo));
			Show_Status(strData);
			return false;
		}



		this->m_bLoad_Bin_List = true;
		//��ʾpac�ļ�
		int nPos = m_csScat_Cfg_File_Path.ReverseFind('\\');
		if (nPos != -1)
		{
			strData = m_csScat_Cfg_File_Path.Left(nPos);
			this->Add_ListCtrl_Bin_List_Data(
				_T("File_Path"),
				strData,
				true);

			strData = m_csScat_Cfg_File_Path.Right(m_csScat_Cfg_File_Path.GetLength() - nPos-1);
			this->Add_ListCtrl_Bin_List_Data(
				strData,
				m_csScat_Cfg_File_Path,
				true);
		}
		this->m_bLoad_Bin_List = false;


		//---------------------------- ���ý����������ݲ���ѡ����������Ȳ�ͬ��---------------------------- 
		this->Init_Progress_Ctrl(PF_SPREADTRUM, -1);
	}
	catch(...)
	{
		return false;
	}

	return true;

}





bool CDlg_Example::Load_All_DL_Files_ASR(void)
{
	try
	{
		//--------------------------------------------------
// 		GetDlgItem(IDC_STATIC_AT_PORT)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_AT_PORT)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_SET_DL_MODE)->ShowWindow(SW_SHOW);
		//--------------------------------------------------

		//��ʼ������Ϣ�����������ļ�
		if(this->m_cla_ARM_OP_ASR_Main.LOAD_FW_FILE(this->m_csScat_Cfg_File_Path) == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_ASR_Main.m_csError_Code);
			throw(0);
		}

		//����ǰ.exe����·��
		this->m_cla_ARM_OP_ASR_Main.m_csCurr_Exe_Path.Format(_T("%s"), this->m_csCurr_Exe_Path);


		this->m_sExtractPackageFile.Format(_T("%s\\%s"),m_csCurr_Exe_Path,TEMP_PACKAGE);
		//��ZIP�ļ���ѹ

		if(PathFileExists(this->m_sExtractPackageFile))
		{
			this->DeleteFolder(this->m_sExtractPackageFile);
		}


		if(TRUE ==this->ExactZipPackage())
		{
			//
			BOOL bGetType = FALSE;
			bGetType = this->ASR_Get_BB_Chip();

			if (ASR1802 == this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip)
			{
				//-------------------------------------------------
				if(PathFileExists(this->m_sExtractPackageFile))
				{
					this->DeleteFolder(this->m_sExtractPackageFile);
				}
				//-------------------------------------------------

			}
			else
			{

				this->m_str_ASR3601_FW= this->m_csScat_Cfg_File_Path;//20200319


				//3601ƽ̨��ɾ��������������ʹ�� 20191203

				//20191203
// 				if (false ==  this->m_b_aboot_engine_init_flag)
// 				{
// 					if(false   ==   this->Copy_FW_Bin())
// 					{
// 						this->Show_Status(_T("Copy_FW_Bin Fail!"));
// 						this->Save_Trace_Log_File(_T("Copy_FW_Bin Fail!"));
// 						throw(0);
// 					}
// 				}



			}



			if(FALSE ==  bGetType)
			{
				this->Show_Status(_T("Find Key Fail!"));
				this->Save_Trace_Log_File(_T("Find Key Fail!"));
				throw(0);
			}

		}
		else
		{
			this->Show_Status(_T("ExactZipPackage Fail!"));
			this->Save_Trace_Log_File(_T("ExactZipPackage Fail!"));
			throw(0);
		}

		if (ASR3601 == this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip)
		{
			this->m_iDisableGroupNum = 0;

			memset(this->m_iDisableGroupName, 0, sizeof(this->m_iDisableGroupName));

// 			if (GetEraseUserDataGroupInfo("\"nvm\"", "\"factory_a\"", "\"factory_b\"", this->m_iDisableGroupNum, this->m_iDisableGroupName))
// 			{
// 				this->m_bDisableGroupInit = TRUE;
// 			}
// 			else
// 			{
// 				this->m_bDisableGroupInit = FALSE;
// 			}
			this->m_bDisableGroupInit = FALSE;

			//-------------------------------------------------
			if(PathFileExists(this->m_sExtractPackageFile))
			{
				this->DeleteFolder(this->m_sExtractPackageFile);
			}

			//-------------------------------------------------
			CString csInfo;

			if (false ==  this->m_b_aboot_engine_init_flag)
			{

				
				csInfo.Format(_T("++++aboot_engine_init"));
				this->Save_Trace_Log_File(csInfo);

				csInfo.Format(_T("++++aboot_engine_register_cb"));
				this->Save_Trace_Log_File(csInfo);

				//�����
				aboot_engine_init();
				aboot_engine_register_cb(&aboot_download_callback, NULL);//

				//20191204
				this->aboot_download_init_Ext();

				this->m_b_aboot_engine_init_flag = true;

			}
			else
			{
				csInfo.Format(_T("++++aboot_download_stop"));
				this->Save_Trace_Log_File(csInfo);
				aboot_download_stop();
				Sleep(3000);
			}

			

		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool CDlg_Example::Load_All_DL_Files_Intel(void)
{
	try
	{
		//��ʼ������Ϣ������Intel�����ļ�
		if(this->m_cla_ARM_OP_INTEL_Main.LOAD_FW_FILE(this->m_csScat_Cfg_File_Path) == false)
		{
			this->Show_Status(this->m_cla_ARM_OP_INTEL_Main.m_csError_Code);
			throw(0);
		}

		//��Intel���ò���
		this->Load_Params_Intel(_T("INTEL"));

		//����ǰ.exe����·��
		this->m_cla_ARM_OP_INTEL_Main.m_csCurr_Exe_Path.Format(_T("%s"), this->m_csCurr_Exe_Path);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void CDlg_Example::Load_View_Bin_List_Intel(void)
{
	this->m_bLoad_Bin_List = true;

	//��ʾ���ص�FLS�ļ�
	for(int i=0; i<MAX_FLS_FILE; i++)
	{
		this->Add_ListCtrl_Bin_List_Data(
			this->m_cla_ARM_OP_INTEL_Main.m_strr_FLS_File_List[i].str_File_Base.cs_Name,
			this->m_cla_ARM_OP_INTEL_Main.m_strr_FLS_File_List[i].str_File_Base.cs_Path,
			this->m_cla_ARM_OP_INTEL_Main.m_strr_FLS_File_List[i].b_Enable_DL);
	}

	this->m_bLoad_Bin_List = false;
}

bool CDlg_Example::Load_Advanced_Config_Intel(void)
{
	try
	{
		//---------------------------- ��ȡ���߼����ã��������ã����Ĳ���----------------------------
		this->Get_Advanced_Config_Value(PF_INTEL, NULL);

		//---------------------------- ���ݡ��߼����ã��������ã����Ĳ��������á�Intel�ࡱ�еġ����������----------------------------
		//Intel����ģʽ��Optionѡ���������
		this->m_cla_ARM_OP_INTEL_Main.m_iOP_TYPE = this->m_iCtrl_OP_Option_S;
		//����Intelƽ̨LOG
		this->m_cla_ARM_OP_INTEL_Main.m_bGet_Debug_Log = this->m_bGet_Debug_Log;

		//---------------------------- ���������Ҫ���ص� Intel �ļ���������ʾ�����ļ��嵥��----------------------------
		this->Load_View_Bin_List_Intel();

		//---------------------------- ���ý����������ݲ���ѡ����������Ȳ�ͬ��----------------------------
		this->Init_Progress_Ctrl(PF_INTEL, -1);

		//---------------------------- ��գ��ָ�Ĭ�ϣ�reg��USB�豸channel�� ----------------------------
		this->m_cla_ARM_OP_INTEL_Main.Scan_Usb_Dev();

		for(int i=0; i<this->m_cla_ARM_OP_INTEL_Main.m_iUsb_Dev_Quan; i++)
		{
			this->m_cla_ARM_OP_INTEL_Main.Set_Reg_Usb_Dev_Ch_Value(
				this->m_cla_ARM_OP_INTEL_Main.m_csUSB_Detect_PID_VID,
				this->m_cla_ARM_OP_INTEL_Main.m_csrUsb_Dev_List[i],
				_T("USB0"));
		}

		//---------------------------- �ر�δ������ֹ�Ľ��̣�process��----------------------------
		this->m_cla_ARM_OP_INTEL_Main.m_cla_GFC.Kill_Process(_T("DownloadTool.exe"));
	}
	catch(...)
	{
		return false;
	}

	this->Show_Status(_T(""));
	return true;
}

LRESULT CDlg_Example::OnGetMsg_Trace_Log_Msg(WPARAM wParam, LPARAM lParam)
{
	this->Save_Trace_Log_File((wchar_t *)wParam);

	return LRESULT();
}

void CDlg_Example::Save_Trace_Log_File(const wchar_t * wcp_Data)
{
	CTime aTime = CTime::GetCurrentTime();
	CString csData;

	int y = aTime.GetYear();
	int m = aTime.GetMonth();
	int d = aTime.GetDay();
	int h = aTime.GetHour();
	int min = aTime.GetMinute();
	int s = aTime.GetSecond();

	if(this->m_iDebug_Log_Handle == 0)//LOG�ļ�δ�򿪣���Ҫ��
	{
		//20200701
		if (this->m_cla_Debug_Log_Stdio_File.m_hFile != CFile::hFileNull)
		{
			this->m_cla_Debug_Log_Stdio_File.Close();
		}

		if(this->m_cla_Debug_Log_Stdio_File.Open(this->m_csDebug_Log_File_Path, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::typeText) != NULL)
		{
			this->m_cla_Debug_Log_Stdio_File.SeekToEnd();

			csData.Format(_T("%d-%d-%d_%d:%d:%d <--> %s\n"), y, m, d, h, min, s, wcp_Data);
			this->m_cla_Debug_Log_Stdio_File.WriteString(csData);

			this->m_iDebug_Log_Handle = 1;//�ɹ���
		}
		else
		{
			this->m_iDebug_Log_Handle = -1;//��ʧ��
			::AfxMessageBox(_T("ERROR, ��LOG�ļ�ʧ��!"));
			return;
		}
	}
	else if(this->m_iDebug_Log_Handle == 1)
	{
		csData.Format(_T("%d-%d-%d_%d:%d:%d <--> %s\n"), y, m, d, h, min, s, wcp_Data);
		this->m_cla_Debug_Log_Stdio_File.WriteString(csData);
	}
}

void CDlg_Example::Close_Trace_Log_File(void)
{
	if(this->m_iDebug_Log_Handle == 1)
	{
		this->m_cla_Debug_Log_Stdio_File.Close();
		this->m_iDebug_Log_Handle = 0;
	}
}

void CDlg_Example::Load_Params_Intel(const wchar_t * wcp_Product)
{
	//���ط�ʽ
	this->m_cla_ARM_OP_INTEL_Main.m_iCTRL_DL_TYPE = this->m_cla_iniParams.ReadInt(wcp_Product, _T("DL_TYPE"), INTEL_DL_USB_SCAN);
}

void CDlg_Example::OnCbnSelchangeComboOcpuModelS()
{
	this->UpdateData(TRUE);
	this->Save_Config();
	//
	this->INIT();
}

void CDlg_Example::Load_View_Static_Value(void)
{
	CString csData;
	wchar_t * wcpSrc = NULL;
	wchar_t * wcpEach = NULL;

	//��OCPU APP���صĻ����б�
	csData = this->m_cla_iniParams.ReadStr(_T("OCPU"), _T("MODEL"), _T(""));
	//��OCPU��������view����
	//���OCPU����
	while(this->m_ctr_cbxOCPU_Name_S.GetCount())
	{
		this->m_ctr_cbxOCPU_Name_S.DeleteString(0);
	}
	//����OCPU����
	this->m_cla_Flash_OP_Main.m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpSrc, (csData.GetLength() + 8));

	//���롰Դ���ݡ�
	swprintf(wcpSrc, _T("%s"), csData);

	//��ȡ��һ��ֵ(0)
	wcpEach = wcstok(wcpSrc, _T(",;"));

	while(wcpEach != NULL)
	{
		this->m_ctr_cbxOCPU_Name_S.AddString(wcpEach);
		//��ȡ��һ��ֵ
		wcpEach = wcstok(NULL, _T(",;"));
	}//while(wcpEach != NULL


	//�ͷ��ڴ�
	this->m_cla_Flash_OP_Main.m_cla_GFC.BUFF_WCHAR_FREE(&wcpSrc);
}

void CDlg_Example::OnBnClickedFirehoseReset()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_ARM_OP_QCM_Main.m_bFirehose_Reset_after_DL =this->m_bFirehoseReset;	
	this->m_cla_iniFile.StoreB(_T("Main"), _T("Firehose_Reset_after_DL"), this->m_bFirehoseReset);
}

void CDlg_Example::OnBnClickedSaharaDl()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
//	this->m_cla_iniFile.StoreB(_T("Main"), _T("SAHARA_DL_Switch"), this->m_bSaharaDL);
}

void CDlg_Example::OnBnClickednobaudswitch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (this->m_bNoBaudSwitch == false)
	{
		this->m_cla_ARM_OP_HUAWEI_Main.m_iBC95BaudSwitch =0;
	} 
	else
	{
		this->m_cla_ARM_OP_HUAWEI_Main.m_iBC95BaudSwitch =1;
	}

	this->m_cla_iniFile.StoreB(_T("Main"), _T("BC95_Baud_Switch"), this->m_bNoBaudSwitch);
}

void CDlg_Example::OnBnClickedWrCon()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_ARM_OP_HUAWEI_Main.m_blWrCon =this->m_blWrCon;
	this->m_cla_iniFile.StoreB(_T("Main"), _T("Wirte_RF_Configure"), this->m_blWrCon);
}
//m_bWriteKV
void CDlg_Example::OnBnClickedWriteKV()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_ARM_OP_HUAWEI_Main.m_bWriteKV =this->m_bWriteKV;
	//this->m_cla_iniFile.StoreB(_T("Main"), _T("WriteKV"), this->m_bWriteKV);
}


void CDlg_Example::OnBnClickedSsbUp()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_ARM_OP_HUAWEI_Main.m_bSSB_Update =this->m_bSSB_Update;
	this->m_cla_iniFile.StoreB(_T("Main"), _T("SSB_Update"), this->m_bSSB_Update);
}

//��˼NBƽ̨��������
void Operate_Erase(LPVOID x)
{
	CDlg_Example *pSv = (CDlg_Example *)x;

	pSv->DO_Erase_OP();
}

void CDlg_Example::OnBnClickedErase()
{
	// TODO: Add your control notification handler code here
	//
	this->Ctrl_Colour(COL_BLACK);
	AfxMessageBox(_T("Erase Version?"));

	this->m_hThread_Erase_Handle = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)Operate_Erase,
		(void *)this,
		0,
		&this->m_dwThread_Erase_ID);
}

void CDlg_Example::DO_Erase_OP(void)
{
	m_cla_ARM_OP_HUAWEI_Main.Do_Execute_Erase();
}

void CDlg_Example::OnBnClickedRVersion()
{
	// TODO: Add your control notification handler code here
	this->Ctrl_Colour(COL_BLACK);
	bool bStatusFlag =false;
	bStatusFlag = m_cla_ARM_OP_HUAWEI_Main.NB_Port_Open(9600);
	if (true  == bStatusFlag)
	{
		m_cla_ARM_OP_HUAWEI_Main.FindVer(1);
		m_cla_ARM_OP_HUAWEI_Main.NB_Port_Close();

		this->OnGetMsg_Show_Status2((WPARAM)m_cla_ARM_OP_HUAWEI_Main.mAT_FirewareName.GetBuffer(),NULL);
	} 
	else
	{
		AfxMessageBox(_T("Port Open Fail"));
	}
	
}

void CDlg_Example::OnBnClickedRImei()
{
	// TODO: Add your control notification handler code here
	this->Ctrl_Colour(COL_BLACK);
	

	bool bStatusFlag =false;
	bStatusFlag = m_cla_ARM_OP_HUAWEI_Main.NB_Port_Open(9600);
	if (true  == bStatusFlag)
	{
		m_cla_ARM_OP_HUAWEI_Main.OnReadAtImei();
		m_cla_ARM_OP_HUAWEI_Main.NB_Port_Close();
		this->OnGetMsg_Show_Status2((WPARAM)m_cla_ARM_OP_HUAWEI_Main.mAT_IMEI.GetBuffer(),NULL);
	} 
	else
	{
		AfxMessageBox(_T("Port Open Fail"));
	}

}

void CDlg_Example::OnBnClickedWImei()
{
	// TODO: Add your control notification handler code here
	this->Ctrl_Colour(COL_BLACK);
	UpdateData(TRUE);
	if (m_strImei.GetLength()!=15)
	{
		this->Show_Status(_T("IMEI Length Error"));
	}
	m_cla_ARM_OP_HUAWEI_Main.mAT_IMEI=m_strImei;

		
	
    m_cla_ARM_OP_HUAWEI_Main.OnWriteAtImei();

	
}

/////////////////////////////////////////////////////////////////////
//RSA
LRESULT CDlg_Example::OnCallbackFunc_RSA(WPARAM wParam, LPARAM lParam)
{	
	NotifyStatus* l_pNotifyStatus = (NotifyStatus*)lParam;
	CString strState, strStatus;
	BOOL bIsError = FALSE;

	switch(l_pNotifyStatus->eProcState)
	{
	case kProcConnecting:
		AddDevice((NotifyStatus*)lParam);
		break;	 
	case kProcDownloading:
		OnProgressToView((LPARAM)l_pNotifyStatus);
		break;
	case kProcCompleted:
		{
			OnProgressToView((LPARAM)l_pNotifyStatus);
		}
		break;
	case kProcUsbRemove: 
		{
			//20190515 ����
			ReportError((NotifyStatus*)lParam);
		}
		break;
	case kProcEraseAllFlash:
		{
			OnProgressToView((LPARAM)l_pNotifyStatus);
		}
		break;
	case kProcErrorCode:
		{
			//20190515 ����
			ReportError((NotifyStatus*)lParam);
		}
		break;
	case kProcAborting:
		OnProgressToView((LPARAM)l_pNotifyStatus);
		//	SetUSBConnectStatus((NotifyStatus*)lParam);
		break;
	case kProcBurningFlash:
		OnProgressToView((LPARAM)l_pNotifyStatus);
		break;
	case kProcDebugLog:
		//ReportDebugLog((NotifyStatus*)lParam);
		break;
	case kProcFuseInfo: //�й�fuse��information
		//ReportDebugLog((NotifyStatus*)lParam);
		break;
	case kProcFuseInfoError: //����յ�������Ϣ��ʾ����fuseʧ��
		{
			//20190515 ����
			ReportError((NotifyStatus*)lParam);
		}
		break;
	case kProcUEInfo: //UEInfo report in message
		//ReportDebugLog((NotifyStatus*)lParam);
		break;
	default:
		{
			// To do your code
			break;
		}
	}
	//SetUSBConnectStatus((NotifyStatus*)lParam);
	//SetProcState(l_pNotifyStatus);
	delete l_pNotifyStatus;
	l_pNotifyStatus = NULL;
	return 0;
}
//ASR ������Ϣ�Ļ�ȡ
void CDlg_Example::ReportError(NotifyStatus* pNtStat)
{
	CString strTmp=_T("");
	int iDeviceID = 0;


	strTmp.Format(_T("Device %d:%s burning flash error: %s Error Code is:0x%x"),iDeviceID,pNtStat->lpUSBPortAddress,pNtStat->lpProcMsg,pNtStat->nDownloadedPercent);

	this->Show_Status(strTmp);

	this->m_cla_ARM_OP_ASR_Main.m_csError_Code =pNtStat->lpProcMsg;
	this->m_cla_ARM_OP_ASR_Main.m_iError_Code = pNtStat->nDownloadedPercent;

	m_cla_ARM_OP_ASR_Main.m_bRuning = false;
	::SetEvent(m_hWaitTestFin_Main);

}
void CDlg_Example::AddDevice(NotifyStatus* pNtStat)
{
	if(kBootRom==pNtStat->nDevType) //bootrom����������ʾ
		return;
	CListProgress* pListProgr = NULL;

	int index=0;
	int Size = 0;

	//�ȸ����豸��ַ���� Index,����Index���ҽ�����
	int iUSBPortIndex =0;
	bool bIsFound = false;
	if(kBootLoader==pNtStat->nDevType)
	{
		CString text, address = pNtStat->lpUSBPortAddress;

		Show_Status(_T("DL..."));

		Show_Status2(address);

		//�ؼ�״̬�л�
		this->Set_View_Ctrl_State( 3 );
	}
	return ;
}

BOOL CDlg_Example::AddDevice(int iUSBPort,int nDeviceType,CString sUSBAddress)
{

	if(kBootRom==nDeviceType) //bootrom����������ʾ
		return TRUE;
	BOOL bSuccess = false;
	CListProgress* pListProgr = NULL;
	if(kBootLoader==nDeviceType)
	{

		if(!m_IndexProgressMap.Lookup(sUSBAddress, (void*&)pListProgr))
		{
			//			int nItem = GetListCtrl().GetItemCount();
			CString text, address = sUSBAddress;

			// 			if(m_pMainFrame->IsChineseLocal())
			// 			{
			// 				text.Format(_T("�豸 %d"),nItem+1);
			// 			}
			// 			else
			// 			{
			// 				text.Format(_T("Device %d"),nItem+1);
			// 			}
			// 			GetListCtrl().InsertItem(nItem,_T(""),m_iUSBUnConnectedBitMapID);
			// 
			// 			GetListCtrl().SetItemText(nItem, kDeviceIndex, text);
			// 			GetListCtrl().SetItemText(nItem, kDeviceAddress,address );
			// 			GetListCtrl().SetItemText(nItem, kComment, aProcStateMsgTable[kProcIdle].szStateMsg);
			// 			CreateProgress(sUSBAddress, nItem);
		}
		else
		{
			//			pListProgr->SetBkColour(RGB(255,255,255));
			if (pListProgr && IsWindow(pListProgr->m_hWnd))
			{
				int nItem = pListProgr->GetIndex();
				// 				GetListCtrl().SetItemText(nItem, kComment, _T(""));
				// 				GetListCtrl().SetItemText(nItem, kElapsedTime, _T(""));
				// 				GetListCtrl().SetItemText(nItem, kDeviceAddress,sUSBAddress );
				bSuccess = true;
			}
		}

	}	

	return bSuccess;
}

LRESULT CDlg_Example::OnProgressToView(LPARAM lParam)
{
	NotifyStatus* pNtStat = (NotifyStatus*)lParam;
	eProcessStatus m_CurPrcStatus  = pNtStat->eProcStatus;
	SetProgress(pNtStat);
	return 0;
}
BOOL CDlg_Example::SetProgress(NotifyStatus* pNtStat)
{
	BOOL bSuccess = false;
	int index =0;
	CString strPercent;
	CString strMsg(aProcStateMsgTable[pNtStat->eProcState].szStateMsg);

	strPercent.Format(_T("Progress: %d%% State:%s\r\n"), pNtStat->nDownloadedPercent,strMsg);
	this->Show_Status(strMsg);
	this->Show_Progress(pNtStat->nDownloadedPercent);

	if(kProcCompleted == pNtStat->eProcState) //Download successfully 
	{
		m_cla_ARM_OP_ASR_Main.m_bRuning = false;
		this->m_cla_ARM_OP_ASR_Main.m_csError_Code =_T("");
		this->m_cla_ARM_OP_ASR_Main.m_iError_Code = 0xffffffff;
		::SetEvent(m_hWaitTestFin_Main);

	}
	else if(kProcAborting == pNtStat->eProcState)
	{
      //...����ԭ��ֵ�ĵط�����
	}
	else
	{
		if (pNtStat->nDownloadedPercent == 0)
		{		   
			//pListProgr->SetShowText(true);
		}
		bSuccess = true;
	}


	return bSuccess;
}



bool CDlg_Example::StopDownload()
{
	if(m_gWtptphModule)
	{
		TERMINATEBL pTerminateBL = (TERMINATEBL)GetProcAddress(m_gWtptphModule, "TerminateBL");
		if(NULL==pTerminateBL)
		{
			//m_pMainFrame->ShowErrorCodeMsg(_T("GetProcAddress for TerminateBL failed."),GetLastError());
		}
		if(!pTerminateBL())
		{
			AfxMessageBox(_T("TerminateBL failed")); 
			this->m_irDL_OBJ_State=DL_OBJ_FAIL;//����
			return false;
		}
		else
		{
		}
	}
	this->m_irDL_OBJ_State=DL_OBJ_FAIL;//����
	return true;	
}

bool CDlg_Example::DeleteFolder(const TCHAR* pszFolderPath)
{
	if(taccess(pszFolderPath,0)==0)  
	{
		if(!DeleteEntireDir(pszFolderPath))
		{
			return false;
		}
	} 

	return true;
}


bool CDlg_Example::DeleteEntireDir(const TCHAR * sPath,bool bDelDir/*=true*/)
{
	CFileFind ff;  
	BOOL bFound;  
	bFound = ff.FindFile(CString(sPath) + "\\*.*");  
	while(bFound)  
	{  
		bFound = ff.FindNextFile();  
		CString sFilePath = ff.GetFilePath();  

		if(ff.IsDirectory())  
		{  
			if(!ff.IsDots())  
			{  
				DeleteEntireDir(sFilePath);  
			}  
		}  
		else  
		{  
			if(ff.IsReadOnly())  
			{  
				SetFileAttributes(sFilePath, FILE_ATTRIBUTE_NORMAL);  
			}  
			DeleteFile(sFilePath);  
		}  
	}  
	ff.Close();  

	if (bDelDir)
	{
		SetFileAttributes(sPath, FILE_ATTRIBUTE_NORMAL);  //�����ļ��е�����  
		RemoveDirectory(sPath);
	}

	return true;
}

BOOL CDlg_Example::ExactZipPackage()
{
	m_sFileNameArray.RemoveAll();
	CString l_strDistription,l_strDest;
	CString strImlSrcPathName,strOutputFilePath, strOutputFileName;

	CString strZipAppPath = m_csCurr_Exe_Path;
	strZipAppPath += _T("\\7z.exe");

	if(!PathFileExists(strZipAppPath))
	{
		AfxMessageBox(_T("there is no 7z.exe , please check"));
		return	FALSE;
	}

	m_sExtractPackageFile.Format(_T("%s\\%s"),m_csCurr_Exe_Path,TEMP_PACKAGE);

	STARTUPINFO	l_sStartupInfo;
	PROCESS_INFORMATION l_sProcessInfo;
	ZeroMemory( &l_sStartupInfo, sizeof(l_sStartupInfo) );
	l_sStartupInfo.cb = sizeof(l_sStartupInfo);
	l_sStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	l_sStartupInfo.wShowWindow = SW_HIDE;
	ZeroMemory( &l_sProcessInfo, sizeof(l_sProcessInfo) );

	CString strCommandLine;
	strCommandLine.Format(_T("\"%s\" x \"%s\" -o\"%s\" -y"),strZipAppPath,this->m_csScat_Cfg_File_Path,m_sExtractPackageFile);

	if (!CreateProcess(NULL, strCommandLine.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &l_sStartupInfo,&l_sProcessInfo))
	{
		strCommandLine.ReleaseBuffer();
		return -1;
	}
	strCommandLine.ReleaseBuffer();

	int l_iTimeout=0;
	bool l_bStop = false;
	while ((!l_bStop) && (l_iTimeout < MAX_ZIP_PROCESS_TIMEOUT))
	{
		if (WaitForSingleObject(l_sProcessInfo.hProcess,DEFAULT_PROCESS_TIMEOUT) != WAIT_TIMEOUT)
			l_bStop = true;
		else
			l_iTimeout+= DEFAULT_PROCESS_TIMEOUT;
	}

	if (l_iTimeout >= MAX_ZIP_PROCESS_TIMEOUT)
	{
		TerminateProcess(l_sProcessInfo.hProcess,0);
	}
	else
	{
		// Close process and thread handles. 
		CloseHandle(l_sProcessInfo.hProcess);
		CloseHandle(l_sProcessInfo.hThread);

	}

	if(PathFileExists(m_sExtractPackageFile))
	{
		return	TRUE;
	}

	return	FALSE;
}
#define _A_SUBDIR 0x10 
bool CDlg_Example::BrowseDirectory(CString sFileName,CString sRootDir)
{
	_tfinddata_t fileinfo;  
	memset(&fileinfo, 0x0, sizeof(fileinfo));     
	CString strError;
	CString strdir = sFileName; 
	CString strRootDirTree = sRootDir;
	intptr_t iFind = _tfindfirst((strdir+_T("\\*.*")), &fileinfo);  
	if (iFind == -1)  
	{  
		return false;
	}
	else
	{
		strdir+=_T("\\");
	}

	CString strtmp = fileinfo.name;
	CString strfilePath(strdir + fileinfo.name);   

	while (_tfindnext(iFind, &fileinfo) == 0)  
	{  
		strfilePath = strdir + fileinfo.name;  
		if(fileinfo.attrib&_A_SUBDIR)  //�����һ����Ŀ¼���õݹ��������һ���� 
		{  
			if(_tcscmp(fileinfo.name,_T(".")) && _tcscmp(fileinfo.name,_T("..")))
			{
				BrowseDirectory(strfilePath,sRootDir); 
			}
		}

		else  //��ʾ��ǰ���ʵ��ļ� 
		{

			m_sFileNameArray.Add(strfilePath);

		}
	}  
	_findclose(iFind);  

	return true;
}

bool CDlg_Example::SupportCurentBlfVersion(CString strBlfFilePath)
{
	TCHAR szBuffer[MAX_PATH] = {0};
	GetPrivateProfileString(_T("BLF_Version"), _T("Blf_Version_Number"), _T(""), szBuffer, MAX_PATH, strBlfFilePath);
	CString strBlfVersion = szBuffer;
	GetPrivateProfileString(_T("TIM_Configuration"), _T("Processor_Type"), _T(""), szBuffer, MAX_PATH, strBlfFilePath);
	CString strProcesserType = szBuffer;

	//����Ƿ���Ҫ�������Զ�����UE_Boot_Option =1
	GetPrivateProfileString(_T("UE_Options"), _T("UE_Boot_Option"), _T(""), szBuffer, MAX_PATH, strBlfFilePath);
	CString strUE_Boot_Option = szBuffer;

	//
	if (_T("1") == strUE_Boot_Option)
	{
		CString csTemp;
		csTemp.Format(_T("0"));

		WritePrivateProfileString(_T("UE_Options"), _T("UE_Boot_Option"), csTemp, strBlfFilePath);//Ĭ�ϲ�����

	}


	if (0 == strProcesserType.CompareNoCase(_T("PXA1826")))
	{
		if (0 != strBlfVersion.CompareNoCase(_T("V3.0.0")))
		{
			return false;
		}
	}
	else if (0 != strProcesserType.CompareNoCase(_T("PXA1202"))
		&& 0 != strProcesserType.CompareNoCase(_T("PXA1802")))
	{
		if (0 != strBlfVersion.CompareNoCase(_T("V2.2.0")))
		{
			return false;
		}
	}
	return true;
}

BOOL CDlg_Example::PrepareDownloadFile()
{
	//Find Blf file 
	if(!BrowseDirectory(m_sExtractPackageFile,m_sExtractPackageFile))
	{
		return FALSE;
	}
	else
	{
		int iIndex = 0;
		CString strtmpFileName,strBlfFileName,strFBFFileName,strFlasherFileName;
		bool   bIsBlfFound = false;

		for(iIndex=0;iIndex<m_sFileNameArray.GetSize();iIndex++)
		{
			strtmpFileName = m_sFileNameArray.GetAt(iIndex);

			CFile File(strtmpFileName, CFile::modeRead);
			CString strFileName = File.GetFileName();
			File.Close();			
			if(strtmpFileName.Find(BLF_FILE_SUFFIX) == strtmpFileName.GetLength() - _tcslen(BLF_FILE_SUFFIX))
			{
				if (bIsBlfFound)
				{
					MessageBox(_T("Exist two blf files.\nPlease check!"), _T("Error"), MB_ICONERROR | MB_OK);
					DeleteFolder(m_sExtractPackageFile);//20190815
					return FALSE;
				}

				strBlfFileName = strtmpFileName;
				bIsBlfFound = true;
				_tcscpy_s(m_szBlfFilename, MAX_FILE_LENGTH, strBlfFileName.GetBuffer(strBlfFileName.GetLength()));
				strBlfFileName.ReleaseBuffer();
			}

			if (strFileName.CompareNoCase(PLATFORMINFO_FILENAME) == 0)
			{  
				//::SendMessage(m_pMainFrame->GetWorkInfoViewPtr()->GetSafeHwnd(),WM_SHOW_PLATFORM_INFO,(WPARAM)strtmpFileName.GetBuffer(),NULL);
			}			
		}

		if(!bIsBlfFound)
		{
			AfxMessageBox(_T("No blf file is found, this zip file maybe a invalid zip or you downloadmode is not right , please check it"));
			DeleteFolder(m_sExtractPackageFile);
			return false;
		}

		if (!SupportCurentBlfVersion(m_szBlfFilename))
		{
			MessageBox(_T("MPMDownloader is not matched with blf version."));
			DeleteFolder(m_sExtractPackageFile);//20190815
			return false;
		}
		//Check whether the ReliableData.bin is chosen or not. If this is upgrade mode, ReliableData.bin should not be chosen.
		//�������ݲ���Ҫ
		if (kUpgrade == GetSWDMode())
		{
			BOOL bEnableMRDFile = FALSE;
			if (!CheckReliableDataBinFile(m_szBlfFilename, &bEnableMRDFile))
			{
				DeleteFolder(m_sExtractPackageFile);//20190815
				return FALSE;
			}

			if (bEnableMRDFile)
			{
				if (IDOK != AfxMessageBox(_T("You have chosen ReliableData.bin file for Upgrade mode.\r\n It will cover MRD area and lose backup data!\
											 \r\n it's dangerous"), MB_OKCANCEL))

				{
					DeleteFolder(m_sExtractPackageFile);//20190815
					return FALSE;
				}
											 
			}
		}
		if (GetModule())
		{
			READBLF pReadBLF = (READBLF)GetProcAddress(GetModule(), "ReadBLF");
			if(NULL==pReadBLF)
			{
				//m_pMainFrame->ShowErrorCodeMsg(_T("GetProcAddress for ReadBLF failed."),GetLastError());
			}
			if(!pReadBLF(GetInstanceParam(),m_szBlfFilename))
			{
				AfxMessageBox(_T("ReadBLF failed"));
				DeleteFolder(m_sExtractPackageFile);//20190815
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CDlg_Example::CheckReliableDataBinFile(CString strBlfFilePath, BOOL *bEnable)
{
	TCHAR szImageID[MAX_PATH] = {0};
	BOOL bFindFile = FALSE;
	int nImageIndex = 1;
	int nNumber_of_Images = GetPrivateProfileInt(_T("TIM_Configuration"), _T("Number_of_Images"), 0, strBlfFilePath);
	for (int i = 1; i<=nNumber_of_Images; i++)
	{
		CString strImageID = _T("");
		strImageID.Format(_T("%d_Image_ID_Name"), i);
		GetPrivateProfileString(_T("Image_List"), strImageID, _T(""), szImageID, MAX_PATH, strBlfFilePath);
		strImageID = szImageID;
		if (0 == strImageID.Compare(RELIABLEDATA_IMAGE_ID))
		{
			if (bFindFile)
			{
				MessageBox(_T("There are two ReliableData.bin file in BLF file.\nPlease check!"), _T("Error"), MB_ICONERROR | MB_OK);
				return FALSE;
			}
			bFindFile = TRUE;
			nImageIndex = i;
		}
	}
	//read the parameter of Enable
	if (bFindFile)
	{
		CString strImagePath;
		strImagePath.Format(_T("%d_Image_Enable"), nImageIndex);
		*bEnable = GetPrivateProfileInt(_T("Image_List"), strImagePath, 1, strBlfFilePath);
	}
	else
	{
		*bEnable = FALSE;
	}
	return TRUE;
}
int CDlg_Example::RSA_DL_Run()
{
	bool bIsOk = true;

	if(m_bInitializeBL)
	{
		StopDownload();
		m_bInitializeBL = false;
	}
	try
	{
		m_gWtptphModule = LoadLibrary(WTPTPDOWNLOADDLL_NAME);
		if(NULL == m_gWtptphModule)
		{
			this->Show_Status(_T("LoadLibrary for WtptpDownLoad.dll failed"));
			throw(1);
		}

		//BeginWaitCursor();
		if(!this->m_sExtractPackageFile.IsEmpty())
		{ 
			if(PathFileExists(this->m_sExtractPackageFile))
			{
				this->DeleteFolder(this->m_sExtractPackageFile);
			}
		}

		if(TRUE ==this->ExactZipPackage())
		{
			if(!this->PrepareDownloadFile())
			{
				bIsOk = false;
				this->Show_Status(_T("PrepareDownloadFile failed"));
				throw(2);
			}
			else
			{

				GetInstanceParam()->CallbackProc = CallbackProc;
				GetInstanceParam()->UserData = (LPVOID)this;

				if (GetModule() && kFormalRelease != GetSWDMode())//Is it nessary?
				{
					SETERASEFLASHSTATUS pSetEraseFlashStatus = (SETERASEFLASHSTATUS)GetProcAddress(GetModule(), "SetEraseFlashStatus"); 
					if (NULL == pSetEraseFlashStatus)
					{
						this->Show_Status(_T("GetProcAddress for SetEraseFlashStatus failed"));
						bIsOk = false;
						throw(3);
					}
					else
					{
						if (kEraseAll == GetSWDMode())
							bIsOk = pSetEraseFlashStatus(GetInstanceParam(), true);
						else if(kUpgrade == GetSWDMode())
							bIsOk = pSetEraseFlashStatus(GetInstanceParam(), false);
					}
				}


				if(m_gWtptphModule && bIsOk)
				{
					INITIALIZEBL pInitializeBL =  (INITIALIZEBL)GetProcAddress(m_gWtptphModule, "InitializeBL");
					if(NULL==pInitializeBL)
					{
						this->Show_Status(_T("GetProcAddress for InitializeBL failed"));
						throw(4);
					}
					if(pInitializeBL(GetInstanceParam()))
					{
						m_bInitializeBL = true;
						bIsOk = true;
					}
					else
					{
						this->Show_Status(_T("InitializeBL failed"));
						bIsOk = false;
						throw(5);
					}
				}	
			}			
		}
		else
		{
			this->Show_Status(_T("ExactZipPackage failed"));

			bIsOk = false;
			throw(6);
		}

		this->m_irDL_OBJ_State=DL_OBJ_RUN;//����

		return  RTN_SUCCESS;

	}
	catch(...)
	{
		this->m_irDL_OBJ_State =DL_OBJ_FAIL;
		return  RTN_FAIL;
	}
}




PInstanceParams CDlg_Example::GetInstanceParam()
{
	return m_lPInstanceParams;
}

HMODULE CDlg_Example::GetModule()
{
	return m_gSWDhModule;
}


void CDlg_Example::Show_Progress(int i_Pgr_Percent)
{
	::SendMessage(this->m_hWnd, WM_PROGRESS_MSG, (WPARAM)i_Pgr_Percent, (LPARAM)NULL);
}


void CDlg_Example::Show_Progress_ENTIRE(int i_Pgr_Percent)
{
	::SendMessage(this->m_hWnd, WM_PROGRESS_ENTIRE_MSG, (WPARAM)i_Pgr_Percent, (LPARAM)NULL);
}



bool CDlg_Example::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
{
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;

	try
	{
		//��
		hFile_Handle = ::CreateFile(wcp_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			throw(0);
		}

		//��ȡ�ļ���BYTE��
		*ulp_Size = ::GetFileSize(hFile_Handle, NULL);

		//�ر�
		::CloseHandle(hFile_Handle);
		hFile_Handle = INVALID_HANDLE_VALUE;
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void CDlg_Example::OnCbnSelchangeComboCommMethod()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	this->m_cla_ARM_OP_NB_Main.m_iComMethod = this->m_iComMethod;

	//20191130 ��Ϊ������ʾ��ȡ���ط�ʽ
	this->m_cla_Flash_OP_Main.m_iComm_Method = this->m_iComMethod;

	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Com_Method"), this->m_iComMethod);

	if (IN_USB == this->m_iComMethod)
	{
		m_ctr_cbxCom_Main.EnableWindow(FALSE);
	} 
	else
	{
		m_ctr_cbxCom_Main.EnableWindow(TRUE);
	}
}

void CDlg_Example::OnBnClickedFec()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_ARM_OP_HUAWEI_Main.m_bWriteFEC =this->m_bWriteFEC;
	//this->m_cla_iniFile.StoreB(_T("Main"), _T("WriteFEC"), this->m_bWriteFEC);
}

void CDlg_Example::OnBnClickedClearnv()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (this->m_bClearNv == false)
	{
		this->m_iClearNVFlag=0;
	} 
	else
	{
		this->m_iClearNVFlag =1;
	}

	this->m_cla_ARM_OP_NB_Main.m_iClearNVFlag = this->m_iClearNVFlag;

	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Clear_User_Data"), this->m_iClearNVFlag);
}
bool CDlg_Example::Copy_FW_Bin()
{
	CString strInfo;

	int  pos =-1;

	CString strFW_NAME;

	BOOL bCopy =FALSE;


	int iIndex = 0;
	CString strtmpFileName;
	int    iIsAsrFound = -1;


	for(iIndex=0;iIndex<m_sFileNameArray.GetSize();iIndex++)
	{
		strtmpFileName = m_sFileNameArray.GetAt(iIndex);
		//�ҳ�Ҫ���ص�firmware.zip 20191203
		if(strtmpFileName.Find(ASR3601_FILE)>0)
		{
			//�����ļ����Ƶ�bin�ļ���Ŀ¼��-��������ʱ��ʹ��

			pos = strtmpFileName.ReverseFind('\\');

			if (-1 == pos)
			{
				continue;
			}

			strFW_NAME = strtmpFileName.Mid(pos+1);


			this->m_str_ASR3601_FW.Format(_T("%s\\bin\\%s"),m_csCurr_Exe_Path,strFW_NAME);

			//20200317
			if(PathFileExists(this->m_str_ASR3601_FW))
			{
				SetFileAttributes(this->m_str_ASR3601_FW, FILE_ATTRIBUTE_NORMAL);  

				bool bDel = DeleteFile(this->m_str_ASR3601_FW);  

				CString str;
				str.Format(_T("DeleteFile-%s-%d"),this->m_str_ASR3601_FW,bDel);
				this->Save_Trace_Log_File(str);

			}


			bCopy = ::CopyFile(strtmpFileName,this->m_str_ASR3601_FW,FALSE);

			if (FALSE == bCopy)
			{

				strInfo.Format(_T("%s Export Fail"), this->m_str_ASR3601_FW);
				this->Show_Status(strInfo);

				AfxMessageBox(strInfo);

			    return false;

			}
			else
			{

				strInfo.Format(_T("%s Export Success"), this->m_str_ASR3601_FW);

				this->Show_Status(strInfo);

				strInfo.Format(_T("ASR3601-%s"),this->m_str_ASR3601_FW);

				this->Show_Status(strInfo);

				return true;

			}
		}

	}	
	 return false;

}
BOOL CDlg_Example::ASR_Get_BB_Chip()
{
	//Find Blf or JSON file 
	if(!BrowseDirectory(m_sExtractPackageFile,m_sExtractPackageFile))
	{
		AfxMessageBox(_T("BrowseDirectory is FAIL"));
		return FALSE;
	}
	else
	{
		int iIndex = 0;
		CString strtmpFileName;
		int    iIsAsrFound = -1;

		for(iIndex=0;iIndex<m_sFileNameArray.GetSize();iIndex++)
		{
			strtmpFileName = m_sFileNameArray.GetAt(iIndex);

			if(strtmpFileName.Find(BLF_FILE_SUFFIX)>0)
			{
				this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip = ASR1802;

				iIsAsrFound = ASR1802;
				//this->Show_Out(_T("ASR1802"), 1);
				break;
			}	

			if(strtmpFileName.Find(JSON_FILE_SUFFIX)>0)
			{
				this->m_cla_ARM_OP_ASR_Main.m_eu_BB_Chip = ASR3601;

				iIsAsrFound = ASR3601;
				//this->Show_Out(_T("ASR3601"), 1);
				break;
			}

		}

		if(-1 == iIsAsrFound)
		{
			AfxMessageBox(_T("No Key(json,blf) file is found, this zip file maybe a invalid zip or you downloadmode is not right , please check it"));
			DeleteFolder(m_sExtractPackageFile);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CDlg_Example::GetEraseUserDataGroupInfo(const char* partitionName1, const char* partitionName2, const char* partitionName3, int &iDisableGroupNum, int* iDisableGroupName)
{
	CString csSoftwareFileName;
	csSoftwareFileName.Format(_T("%s"), this->m_csScat_Cfg_File_Path);

	CString csSwExtraFilePath;
	csSwExtraFilePath.Format(_T("%s"), this->m_csCurr_Exe_Path);
	csSwExtraFilePath += _T("\\extra Temp");
	m_sExtractPackageFile = csSwExtraFilePath;

	CString cs7zFilePath;
	cs7zFilePath.Format(_T("%s"), this->m_csCurr_Exe_Path);
	cs7zFilePath += _T("\\7z.exe");

	CString csCommand;
	csCommand.Format(_T("\"%s\" x \"%s\" -y -o\"%s\""), cs7zFilePath, csSoftwareFileName, csSwExtraFilePath);
	char strRevalue[100] = { 0 };
	strcpy_s(strRevalue, "Everything is Ok");

	BOOL bRet = system_hide(csCommand.GetBuffer(0), strRevalue, 10000, true);

	csCommand.Format(_T("taskkill /F /IM cmd.exe"));

	strcpy_s(strRevalue, "PID");
	system_hide(csCommand.GetBuffer(0), strRevalue, 10000, true);

	CString csDownloadjsonFileName;
	csDownloadjsonFileName = csSwExtraFilePath + _T("\\download.json");

	FILE* fPtr = NULL;
	char strDownloadjsonFileName[MAX_PATH] = { 0 };

	ConvertTCHARToChar(csDownloadjsonFileName.GetBuffer(0), strDownloadjsonFileName, sizeof(strDownloadjsonFileName));
	csDownloadjsonFileName.ReleaseBuffer();

	fopen_s(&fPtr, strDownloadjsonFileName, "r+");
	if (fPtr == NULL)
		return FALSE;

	fseek(fPtr, 0, SEEK_END);
	long lSize = ftell(fPtr);
	char* strFile = (char*)malloc(lSize + 1);

	rewind(fPtr);

	fread(strFile, sizeof(char), lSize, fPtr);
	strFile[lSize] = '\0';

	fclose(fPtr);

	CStringA csFileContent, csTemp, csGroup;
	csFileContent.Format("%s", strFile);

	free(strFile);

	iDisableGroupNum = 0;
	int iIndex = csFileContent.Find(partitionName1);
	if (iIndex < 0)
	{
// 		bRet = FALSE;
// 		return bRet;
	}
	else
	{

		csTemp = csFileContent.Left(iIndex);
		csTemp = csTemp.Left(csTemp.ReverseFind(','));
		csTemp = csTemp.Left(csTemp.ReverseFind('"'));
		csGroup = csTemp.Mid(csTemp.ReverseFind('"')+1);
		iDisableGroupName[iDisableGroupNum++] = atoi(csGroup);

		bRet = TRUE;
	}


	iIndex = csFileContent.Find(partitionName2);
	if (iIndex < 0)
	{
// 		bRet = FALSE;
// 		return bRet;
	}
	else
	{
		csTemp = csFileContent.Left(iIndex);
		csTemp = csTemp.Left(csTemp.ReverseFind(','));
		csTemp = csTemp.Left(csTemp.ReverseFind('"'));
		csGroup = csTemp.Mid(csTemp.ReverseFind('"')+1);
		if(iDisableGroupName[0] != atoi(csGroup))
		{
			iDisableGroupName[iDisableGroupNum++] = atoi(csGroup);
			bRet = TRUE;
		}
		
	}


	iIndex = csFileContent.Find(partitionName3);
	if (iIndex < 0)
	{
// 		bRet = FALSE;
// 		return bRet;
	}
	else
	{
		csTemp = csFileContent.Left(iIndex);
		csTemp = csTemp.Left(csTemp.ReverseFind(','));
		csTemp = csTemp.Left(csTemp.ReverseFind('"'));
		csGroup = csTemp.Mid(csTemp.ReverseFind('"')+1);

		BOOL bSame = FALSE;
		for (int i = 0; i < iDisableGroupNum; i++)
		{
			if (iDisableGroupName[i] == atoi(csGroup))
			{
				bSame = TRUE;
				break;
			}
		}
		if(!bSame)
		{
			iDisableGroupName[iDisableGroupNum++] = atoi(csGroup);
			bRet = TRUE;
		}
		
	}


	return bRet;
}
BOOL CDlg_Example::system_hide(TCHAR* CommandLine, char* ReturnValue, int timeout, bool timeshot)
{
	SECURITY_ATTRIBUTES   sa;
	HANDLE   hRead, hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return FALSE;
	}

	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if (!CreateProcess(NULL, CommandLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		return FALSE;
	}
	CloseHandle(hWrite);

	char   buffer[4096];
	int maxcount, i = 0;
	if (true == timeshot)
	{
		maxcount = timeout / 500;
	}
	else
	{
		maxcount = timeout / 2000;
	}
	DWORD   bytesRead;
	while (i < maxcount)
	{
		memset(buffer, 0, sizeof(buffer));
		ReadFile(hRead, buffer, 4095, &bytesRead, NULL);
		if (strstr(buffer, ReturnValue) != NULL)
		{
			break;
		}
		if (true == timeshot)
		{
			Sleep(10);
		}
		else
		{
			Sleep(10);
		}
		++i;
	}
	if (i >= maxcount)
	{
		CloseHandle(hRead);
		return FALSE;
	}
	CloseHandle(hRead);
	return   TRUE;
}
void   CDlg_Example::aboot_download_init_Ext(void)
{
	CStringA csFirPath;
	char cPathTemp[1000] = { 0 };
	//ConvertTCHARToChar(this->m_csScat_Cfg_File_Path.GetBuffer(0), cPathTemp, sizeof(cPathTemp));
	//20191203 �̼�·�����ս�ѹ���zip������

// 	if (true ==  this->m_b_aboot_engine_init_flag)//��������̼��ٵ���
// 	{
// 		if(false   ==   this->Copy_FW_Bin())
// 		{
// 			this->Show_Status(_T("Copy_FW_Bin Fail!"));
// 			this->Save_Trace_Log_File(_T("Copy_FW_Bin Fail!"));
// 			return;
// 		}
// 	}


	ConvertTCHARToChar(this->m_str_ASR3601_FW.GetBuffer(0), cPathTemp, sizeof(cPathTemp));



	csFirPath.Format("%s", cPathTemp);

	csFirPath.Replace("\\", "\\\\");
	csFirPath.Replace("\\", "\\\\");

	CStringA csInfoA, csTmp;
	csInfoA.Format("");
	csInfoA += "{\r\n";
	csTmp.Format("\"path\" : \"%s\"\r\n", csFirPath);
	csInfoA += csTmp;
	csTmp.Format("}\r\n");
	csInfoA += csTmp;

	int iLen =0;
	wchar_t * wcJson = NULL;
	CString  csJson;
	//�����ڴ�
	this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcJson, strlen(csInfoA) + 256);
	//��������
	this->m_cla_GFC.MultiByte_To_WideChar(csInfoA, wcJson, &iLen);
	csJson.Format(_T("++++aboot_download_init-%s"), wcJson);
	//�ͷ��ڴ�
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcJson);

	this->Save_Trace_Log_File(csJson);



	aboot_download_init(csInfoA.GetBuffer(0));
	csInfoA.ReleaseBuffer();
}

void CDlg_Example::OnBnClickedSetDlMode()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//AT+QDOWNLOAD=1
	if(this->Confirm_Dynamic_Com_Port(this->m_iAT_Port, 30) == false)
	{	
		this->Show_Status(_T("Check AT Port Fail"));
		return;
	}

	CCom_Port cCom_Port;
	time_t start, finish;
	int iTimeOut =6;//�򿪶˿ڸ�Ϊѭ�����Ҵ򿪵ķ�ʽ

	cCom_Port.m_iBaudRate = 115200;
	cCom_Port.m_iPort = this->m_iAT_Port;
	cCom_Port.m_dwDTR_CTRL = DTR_CONTROL_DISABLE;
	cCom_Port.m_dwRTS_CTRL = RTS_CONTROL_DISABLE;

	this->Show_Status(_T("Open AT Port..."));

	//�򿪶˿�
	try
	{
		//
		time( &start );
		do
		{
			Sleep( 200 );
			if(cCom_Port.Port_Open(300))
			{
				break;
			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				throw(0);
			}
		}
		while( true );
	}
	catch(...)
	{
		this->Show_Status(_T("Open AT Port Fail"));
		return;
	}
	//����AT
	cCom_Port.Empty_Com_And_Rx_Buff();
	
	//����
	cCom_Port.Write( _T( "AT+QDOWNLOAD=1" ) );

	//�رն˿�
	cCom_Port.Port_Close();

	this->Show_Status(_T("Please Wait DL Mode..."));

}
bool CDlg_Example::Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut)
{
	time_t t_Start, t_End;
	CString csA, csB;
	int j = 0;

	try
	{
		csB.Format(_T(" ."));

		time(&t_Start);
		csA.Format(_T("Wait port(%d) is normal..."),i_Tgt_Com_Port);
		this->Show_Status(csA);

		do
		{
			//����regedit�е�COM�б�
			if(this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == true)
			{
				//�ҵ�������
				//�ȴ�5���������һ��
				Sleep(5000);
				if(this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == false)
				{
					//����					
				}
				else
				{
					this->Show_Status(_T("Get Port Pass"));
					return true;
				}

			}

			//����Ƿ�ʱ
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				throw(0);
			}

			//���ȼ�����ʾ
			if((j % 10) == 0)
			{
				csA.Format(_T("Detect dynamic com port %d"), i_Tgt_Com_Port);
			}

			csA += csB;
			this->Show_Status(csA);
			j++;

			//ÿ�ζ�ȡע�����ӳ�
			Sleep(800);
		}
		while(true);
	}
	catch(...)
	{
		return false;
	}

	return false;
}
//UNISOCƽ̨����
int CDlg_Example::Do_Execute_Unisoc(void)
{
	int iRtn = RTN_ERROR;
	int iPort = 0;
	CString strData;
	CHAR   szErrorInfo[MAX_PATH] = {0};
	DWORD dwRet;
	DWORD dwDLRet;
	PORT_DATA portData;

	iPort = this->m_cla_ARM_OP_SPREADTRUM_Main.m_iCom_Port;

	// ���ͨ��COM���Ƿ��Ѿ�����;
	if (!m_PortMap.Lookup(iPort, portData))
	{
		portData.iIndex = 1;
		portData.bRuning = true;
		m_PortMap.SetAt(iPort, portData);
	}
	else if (m_PortMap.Lookup(iPort, portData))
	{
		if (portData.bRuning == true)
		{
			strData.Format(_T("[COM%d]Duplicate channel port ! "),iPort);					
			Show_Status(strData);
			return RTN_FAIL;
		}
		else
		{
			portData.iIndex = 1;
			portData.bRuning = true;
			m_PortMap.SetAt(iPort, portData);
		}
	}

	this->SetUnisocRunningState(1, true);
	//����
	dwRet = m_pDlCctrl->StartOnePortWork(iPort);
	if (0 == dwRet)
	{
		HANDLE hWaiting = m_pDlCctrl->GetDownloadThreadHandle(iPort);
		WaitForSingleObject(hWaiting,INFINITE);
		dwDLRet = m_pDlCctrl->GetDownloadState(iPort);
		// 
		if (dwDLRet == 0)
		{
			iRtn = RTN_SUCCESS;
		}
		else
		{
			iRtn = RTN_FAIL;
			m_pDlCctrl->GetOprErrorCodeDescription(dwDLRet,szErrorInfo,MAX_PATH,iPort);
			USES_CONVERSION;
			strData.Format(_T("[COM%d]Download fail[%s]!"),iPort,A2T(szErrorInfo));
			Show_Status(strData);
		}

		m_pDlCctrl->StopOnePortWork(iPort);
	}
	else
	{
		iRtn = RTN_FAIL;
		m_pDlCctrl->GetOprErrorCodeDescription(dwRet,szErrorInfo,MAX_PATH,iPort);
		USES_CONVERSION;
		strData.Format(_T("[COM%d]CHLib_StartDownload fail[%s]! "),iPort,A2T(szErrorInfo));
	}

	portData.bRuning = false;
	m_PortMap.SetAt(iPort, portData);
	this->SetUnisocRunningState(1, false);
	return iRtn;
}

void CDlg_Example::SetUnisocRunningState(int iCh, bool bState)
{
	this->m_cla_ARM_OP_SPREADTRUM_Main.m_bRuning = bState;
}
LRESULT CDlg_Example::OnGetMsg_Unisoc_Show_Status(WPARAM wParam, LPARAM lParam)
{
	//20190308 ����ƽֻ̨��ʾ4���˿�
	if (m_iPlatform_Curr == PF_SPREADTRUM)
	{
		PORT_DATA portData;
		if(m_PortMap.Lookup((int)lParam,portData))
		{
			lParam = (LPARAM)portData.iIndex;
		}
		this->Show_Status((wchar_t*)wParam);
	}
	return LRESULT();
}

