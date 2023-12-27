#include "StdAfx.h"
#include "Flash_OP_AG.h"


CFlash_OP_AG::CFlash_OP_AG(void)
{
	m_iCom_Port = 0;
	m_iBaud_Rate = 0;

	m_bRuning = false;
	m_iClass_Idx = 0;
	m_csError_Code.Format(_T(""));
	m_iError_Code = 0;
	m_bPLC_Flag = false;
	m_iPLC_Port = 0;
	m_iComMethod = 0;
	m_iResetFlag = 0;
	m_iClearNVFlag = 0;
	m_bSaveDebugLog = false;
}

CFlash_OP_AG::~CFlash_OP_AG(void)
{
	//

}
//cb Show_State
static void __stdcall cb_show_state_r(const wchar_t * wcp_State, void * p_usr_Arg)
{
	CFlash_OP_AG * pSv = (CFlash_OP_AG *)p_usr_Arg;

	pSv->Show_Status(wcp_State);
}
static void __stdcall cb_show_out_r(const wchar_t * wcp_State, void * p_usr_Arg)
{
	CFlash_OP_AG * pSv = (CFlash_OP_AG *)p_usr_Arg;

	pSv->Show_Out(wcp_State);
}

void CFlash_OP_AG::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CFlash_OP_AG::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}
void CFlash_OP_AG::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

void CFlash_OP_AG::Show_Progress(int i_Pgr_Percent)
{
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)i_Pgr_Percent, (LPARAM)this->m_iClass_Idx);
}

int CFlash_OP_AG::Do_Execute()
{
	CString strData;
	try
	{
		m_bRuning = true;

		this->m_csError_Code.Format(_T(""));
		this->m_iError_Code = 0;

		//1. ���ع̼�Download the boot loader and system
		if (this->DL_ImageFile(strData) == false)
		{
			throw(3);
		}
	}
	catch (...)
	{
		m_bRuning = false;
		return RTN_FAIL;
	}

	m_bRuning = false;
	return RTN_SUCCESS;
}

bool CFlash_OP_AG::DL_ImageFile(const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpNextToken = NULL;
	wchar_t * wcpPercent = NULL;
	const  wchar_t * wcpHead = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo, strSubInfo,strEach, strBinFile;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 80;//��ʱ5s
	int iTemp = 0;
	int nPos = 0;
	double dCurr_Percent = 0.0;
	double dNew_Percent = 0.0;

	int iFirst = 0;

	try
	{
		//��ʾ
		this->Show_Status(_T("Download Image file......"));
		this->Save_Trace_Log(_T("Download Image file ......"));

		SECURITY_DESCRIPTOR secutityDese;
		InitializeSecurityDescriptor(&secutityDese, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&secutityDese, TRUE, NULL, FALSE);

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if (CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL_ImageFile, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH+1024;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 1024);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		//////////////////////////////////////////////////////////////////////////
		//python flashimage.py
		//��ȡ���·��
		CString cs_FW_Dir;
		if (this->m_cla_GFC.Get_File_Directory(m_csFlash_Path, &cs_FW_Dir) == false)
		{
			throw(0);
		}

		//swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc %s --productdir %s"), '/', m_csFlash_Path, cs_FW_Dir);
		
		//swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\MTK\\%d\\MT2731R\\python.exe %s --toolsdir %s --productdir %s \""), '/', m_csCurr_Exe_Path, m_iClass_Idx, m_csFlash_Path, cs_FW_Dir, cs_FW_Dir);
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"python %s --toolsdir %s --productdir %s \""), '/', m_csFlash_Path, cs_FW_Dir, cs_FW_Dir);

		this->Save_Trace_Log(wcpCmd);


		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//�����ӽ���
		if (CreateProcessW(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL_ImageFile, Create process fail");
			throw(0);
		}

		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if (hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//��ʼ��ʱ
		time(&t_Start);
		while (bSuccess_Flag == FALSE)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 1024);
			ulActual_Read = 0;
			//
			if (NULL == hRead_Pipe)
			{
				ulActual_Read = 0;
			}

			if (TRUE == ::PeekNamedPipe(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, 0, 0))
			{
				ReadFile(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, NULL);
			}
			
			//����info������
			if (ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 1024);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//��������
				strSubInfo.Format(_T("%s"), wcpData);
				this->Save_Trace_Log(strSubInfo);
				strInfo += strSubInfo;

				//��ʾģ��Reset
				wcpHead = wcsstr(strInfo, _T("Waiting for fastboot mode"));
				if (wcpHead != NULL)
				{
					//��ʾ
					if (0 == iFirst)
					{
						this->Show_Progress((10));

						strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("Waiting for fastboot mode"))));

						this->Show_Status(_T("Reset ......"));
					}
					iFirst++;

				}
				//1,scan device succeed
				wcpHead = wcsstr(strInfo, _T("sending"));
				if (wcpHead != NULL)
				{
					//��ʾ
					this->Show_Progress((20));
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("sending"))));

					this->Show_Status(strSubInfo);
				}
				//2,connect brom START
				wcpHead = wcsstr(strInfo, _T("connect brom START"));
				if (wcpHead != NULL)
				{
					//��ʾ
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("connect brom START"))));
					this->Show_Status(_T("connect brom START"));
					//this->Save_Trace_Log(_T("connect brom START"));
				}
				
				//connect brom successed
				wcpHead = wcsstr(strInfo, _T("connect brom successed"));
				if (wcpHead != NULL)
				{
					//��ʾ
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("connect brom successed"))));
					this->Show_Status(_T("connect brom successed"));
					//this->Save_Trace_Log(_T("connect brom successed"));
				}
				//enter DA mode
				wcpHead = wcsstr(strInfo, _T("DA data..."));
				if (wcpHead != NULL)
				{
					//��ʾ
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("DA data..."))));
					this->Show_Status(_T("DA data..."));
					//this->Save_Trace_Log(_T("DA data..."));
				}
				
				//Success
				//����Ƿ�pass
				if (wcsstr(strInfo, _T("Success")) != NULL)
				{
					this->Save_Trace_Log(_T("Success"));
					this->Show_Progress((100));
					bSuccess_Flag = TRUE;//pass
					break;//�˳�while
				}
				
				//����Ƿ��д��󱨳�
				if (wcsstr(strInfo, _T("<<FAILED>>")) != NULL)
				{					
					this->m_csError_Code.Format(_T("Fail,%s"), strInfo);					
					//this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_FAIL_EXT;
					throw(0);
				}

				if (wcsstr(strInfo, _T("Burn Failed")) != NULL)
				{
					this->m_csError_Code.Format(_T("FAIL, DL_BOOTLOADER_SYSTEM fail!"));
					//this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_FAIL;
					throw(3);
				}


				//����Ƿ�pass
				if (wcsstr(strInfo, _T("FLASH-ALL command execute successed")) != NULL)
				{
					this->Show_Progress((90));
					this->Show_Status(_T("FLASH-ALL command execute successed,wait..."));
				}

			}
			//��ʱ���
			time(&t_End);
			//
			if ((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				this->Show_Progress((100));

				m_csError_Code = _T("FAIL, DL_ImageFile Time out!");
				//this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		this->Show_Progress((100));

		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);

		//�ͷž����ָ��
		if (hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if (hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		return false;
	}

	this->Show_Progress((100));

	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	//�ͷž����ָ��
	if (hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if (hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}

	return true;
}









