#include "StdAfx.h"
#include "ARM_OP_ALTAIR.h"
#include "ts_lan.h"


CCriticalSection g_Port_Ctrl;

CARM_OP_ALTAIR::CARM_OP_ALTAIR(void)
: m_iCom_Port(0)
, m_bRuning(false)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_csCurr_Exe_Path(_T(""))
, m_bIsBaudHighSpeed(true)
, m_iFlowControl(1)
, m_iDLMode(0)
, m_strAppFW(_T(""))
, m_strModemFW(_T(""))
, m_strModemFW2(_T(""))
, m_strFWFolderPath(_T(""))
, m_strChipType(_T(""))
{
}


CARM_OP_ALTAIR::~CARM_OP_ALTAIR(void)
{


}
void CARM_OP_ALTAIR::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_ALTAIR::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_ALTAIR::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_ALTAIR::Show_Progress(double d_Pgr_Percent)
{
	//�˺�����Ӧ�ð����������ٷֱȵļ��㣬Ȼ������Ϣ��Ӧ��������ɾ��岽�������ļ���
	//Progress�ܳ�����1000
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}


void CARM_OP_ALTAIR::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}


bool CARM_OP_ALTAIR::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir;
	CString strData;
	CFileFind cla_Finder; //�ļ�������
	BOOL b_Found;
	bool bIsFactory = false;
	bool bIsAppFW = false;
	bool bIsModemFW = false;
	bool bIsModemFW2 = false;
#if 1
	try
	{
		//--------------------------------------- ��ʼ������Ϣ������ALTAIR�����ļ� --------------------------------------- 
		//��ȡ���·��
		if(this->m_cla_GFC.Get_File_Directory(wcp_FW_File_Path, &cs_FW_Dir) == false)
		{
			this->m_csError_Code.Format(_T("ERROR, Get fls file directory fail!"));
			throw(0);
		}

		//Ѱ���ļ���ʼ
		cs_Finder_Dir.Format(_T("%s\\*"), cs_FW_Dir);

		b_Found = cla_Finder.FindFile(cs_Finder_Dir);

		while(b_Found) 
		{
			b_Found = cla_Finder.FindNextFile();

			if(cla_Finder.IsDots())
			{
				continue;//�ҵ����ǵ�ǰĿ¼���ϼ�Ŀ¼������
			}

			if (cla_Finder.IsDirectory())	//������ļ���
			{
				cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//��ȡ�ҵ����ļ���

				cs_FW_Name.MakeUpper();
				// �ж���RK_03_00�ļ���
				if (wcsstr(cs_FW_Name, _T("RK_03_00")) != NULL)
				{
					//��RK_03_00�ļ���
					m_strFWFolderPath.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());
					bIsFactory = true;
				}
			}
#if 0
			if (!cla_Finder.IsDirectory())	//��������ļ��У������ļ�
			{
				cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//��ȡ�ҵ����ļ���

				cs_FW_Name.MakeUpper();

				// �ж���bin�ļ�
				if (wcsstr(cs_FW_Name, _T(".BIN")) != NULL)
				{

#ifdef FW_FACTORY
					//����golden image�ļ�
					m_strFactoryImagePath.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());
					bIsFactory = true;
					break;
#else				
					if (wcsstr(cs_FW_Name, _T("APPFW_FLASH")) != NULL)// AppFW_flash.bin
					{
						m_strAppFW.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());
						bIsAppFW = true;
					}
#endif
				}
#ifndef FW_FACTORY
				if (cs_FW_Name == _T("UE_LTE.FW"))
				{
					m_strModemFW.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());
					bIsModemFW = true;
				}
				else if (cs_FW_Name == _T("UE_LTE.FW2"))// UE_LTE.FW2
				{
					m_strModemFW2.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());
					bIsModemFW2 = true;
				}

				if (true == bIsAppFW && true == bIsModemFW && true == bIsModemFW2)
				{
					break;
				}
#endif
			}//!cla_Finder.IsDirectory()
#endif
		}//while(b_Found

		//
		cla_Finder.Close();
		//


#ifdef FW_FACTORY
		if (false == bIsFactory)
		{
			this->m_csError_Code.Format(_T("error,load_fw_file,There is RK_03_00 folder!"));
			throw(0);
		}
#else
		if (false == bIsAppFW)
		{
			this->m_csError_Code.Format(_T("error,load_fw_file,There is no AppFW_flash.bin file!"));
			throw(0);
		}
		if (false == bIsModemFW)
		{
			this->m_csError_Code.Format(_T("error,load_fw_file,There is no ue_lte.fw file!"));
			throw(0);
		}
		if (false == bIsModemFW2)
		{
			this->m_csError_Code.Format(_T("error,load_fw_file,There is no ue_lte.fw2 file!"));
			throw(0);
		}
#endif
	}
	catch(...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}
#endif

	return true;
}

int CARM_OP_ALTAIR::Do_Execute(void)
{
	CString strData;
	try
	{
		m_bRuning = true;

		this->m_csError_Code.Format(_T(""));
		this->m_iError_Code = 0;

		// kill flashtoolCLI.exe

		strData.Format(_T("ImageBurnTool%d.exe"), m_iClass_Idx);
		m_cla_GFC.Kill_Process(strData);
#ifdef FW_FACTORY
		// �ж�оƬ�ǿ�Ƭ���Ƿǿ�Ƭ
		if (this->CheckModuleMode() == false)
		{
			throw(1);
		}
		// ����
		if (0 == m_iDLMode)// ����Ĭ��Ϊ��recovery modeģʽ����
		{
			if (this->DL_FactoryRecoveryModeFW() == false)
			{
				throw(1);
			}
		}
		else
		{
			if (this->DL_FactoryFW() == false)
			{
				throw(1);
			}
		}
#else
		if (this->DL_UpdateFW() == false)
		{
			throw(1);
		}
#endif
	}
	catch (...)
	{
		m_bRuning = false;
		return RTN_FAIL;
	}

	m_bRuning = false;
	return RTN_SUCCESS;
}
bool CARM_OP_ALTAIR::CheckModuleMode()
{
	int iTimeOut, iRec_Bits;
	time_t start, finish;
	time_t start_1, finish_1;
	CString strCmd;
	wchar_t wcrData[128];
	try
	{
		this->Show_Status(_T("Open port ......"));
		this->Save_Trace_Log(_T("Open port ......"));

		this->m_cla_Com_Port.m_iPort = this->m_iCom_Port;
 		this->m_cla_Com_Port.m_iBaudRate = 115200;
 
		g_Port_Ctrl.Lock();

		time(&start_1);
		do
		{
			//
			time(&finish_1);
			//
			if ((int)difftime(finish_1, start_1) >= 20)
			{
				this->m_csError_Code.Format(_T("Open port Fail"));

				this->Show_Status(this->m_csError_Code);
				this->Show_Out(this->m_csError_Code);
				
				g_Port_Ctrl.Unlock();
				return false;
			}

			if (this->m_cla_Com_Port.Port_Open(400) == false)
			{
				continue;
				Sleep(1000);
			}
			else
			{
				this->Show_Out(_T("Open port pass"));
				break;
			}

		} while (true);

		//��� COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- ����Ƭ���Ƿǿ�Ƭ------------------------- 
		iTimeOut = 30;
		strCmd.Format(_T("\r\n"));
		//
		time(&start);
		//
		do
		{
			//����
			this->m_cla_Com_Port.Write(strCmd);
			//
			memset(wcrData, 0, sizeof(wcrData));
			Sleep(200);
			//��ȡ
			this->m_cla_Com_Port.Read(wcrData, &iRec_Bits, 100);

			this->Show_Out(wcrData);
			//
			if (wcsstr(wcrData, _T("Boot>")))
			{
				this->m_iDLMode = 0;
				break;
			}
			else if (wcsstr(wcrData, _T("#")))
			{
				this->m_iDLMode = 1;
				break;
			}
			//
			time(&finish);
			//
			if ((int)difftime(finish, start) >= iTimeOut)
			{
				this->m_csError_Code.Format(_T("FAIL, Check module mode time out"), this->m_iCom_Port);
				throw(0);
			}

			if ((int)difftime(finish, start) >= 3)
			{
				this->Show_Status(_T("Please reset..."));
			}

		} while (true);
		this->Show_Status(wcrData);
		this->Show_Out(wcrData);
		this->Save_Trace_Log(wcrData);
	}
	catch (...)
	{
		this->Show_Status(this->m_csError_Code);
		this->Show_Out(this->m_csError_Code);
		this->Save_Trace_Log(m_csError_Code);
		this->m_cla_Com_Port.Port_Close();
		g_Port_Ctrl.Unlock();
		return false;
	}

	this->Show_Status(_T("Check module success"));
	this->Show_Out(_T("Check module success"));
	this->Save_Trace_Log(_T("Check module success"));
	this->m_cla_Com_Port.Port_Close();
	g_Port_Ctrl.Unlock();
	return true;
}

bool CARM_OP_ALTAIR::DL_FactoryRecoveryModeFW()
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
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo, strEach, strBinFile;
	time_t t_Start, t_End;
	bool bCompletedSuccess = false;
	bool bResetDevice = false;
	bool bSuccess_Flag = false;
	int iTimeout = 300;//��ʱ300
	int iTemp = 0;
	int nPos = 0;
	double dCurr_Percent = 0.0;
	double dNew_Percent = 0.0;

	try
	{
		//��ʾ
		//this->Show_Status(_T("Reset the module ......"));
		//this->Save_Trace_Log(_T("Reset the module ......"));
		this->Show_Status(_T("DL start ......"));
		this->Save_Trace_Log(_T("DL start ......"));

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if (CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL_FactoryRecoveryModeFW, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + m_strFWFolderPath.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH * 2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH * 2);
		//////////////////////////////////////////////////////////////////////////
		// "ImageBurnTool.exe" -v "D:\BG770AGLAAR01A02_01.003V01.01.003V01_suggest\RK_03_00" E0_SB /0 /1 /2 /3 /4 /6 /7 /11 /12 /18 -uartparams 0 23 3000000 1 -s 1 -r 23 115200 -n 1
		
		if (m_bIsBaudHighSpeed == true)
		{
			swprintf_s(wcpCmd, nLen, _T("powershell.exe %cc \"%s\\Altair\\%d\\ImageBurnTool%d.exe -v \"%s\" %s /0 /1 /2 /3 /4 /6 /7 /11 /12 /18 -uartparams 0 %d 3000000 %d -s 1 -r %d 115200 -n 1\""),
				'/', m_csCurr_Exe_Path, m_iClass_Idx, m_iClass_Idx, m_strFWFolderPath, m_strChipType, m_iCom_Port, m_iFlowControl, m_iCom_Port);
		}
		else
		{
			swprintf_s(wcpCmd, nLen, _T("powershell.exe %cc \"%s\\Altair\\%d\\ImageBurnTool%d.exe -v \"%s\" %s /0 /1 /2 /3 /4 /6 /7 /11 /12 /18 -uartparams 0 %d 921600 %d -s 1 -r %d 115200 -n 1\""),
				'/', m_csCurr_Exe_Path, m_iClass_Idx, m_iClass_Idx, m_strFWFolderPath, m_strChipType, m_iCom_Port, m_iFlowControl, m_iCom_Port);
		}

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.hStdInput = hRead_Pipe;
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��

		//�����ӽ���
		if (CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL_FactoryRecoveryModeFW, Create process fail");
			throw(0);
		}

		this->Save_Trace_Log(wcpCmd);


		if (!SetHandleInformation(hRead_Pipe, HANDLE_FLAG_INHERIT, 0))
			//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
			if (hWrite_Pipe != NULL)
			{
				CloseHandle(hWrite_Pipe);
				hWrite_Pipe = NULL;
			}

		strData.Empty();
		//��ʼ��ʱ
		time(&t_Start);
		while (bSuccess_Flag == false)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 512, &ulActual_Read, NULL);
			//����info������
			if (ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//��������
				strInfo.Format(_T("%s"), wcpData);
				//				TRACE(strInfo);
				strInfo.MakeLower();
				strInfo.Replace(_T("\r"), _T(""));
				strInfo.Replace(_T("\n"), _T(""));
				//��ȡ��Ҫ�ġ����Ȱٷֱ���Ϣ��
				wcpEach = wcstok(wcpData, _T("\r\n"));
				//
				this->Save_Trace_Log(strInfo);
				while (wcpEach != NULL)
				{					
					wcpPercent = wcsstr(wcpEach, _T("|"));
					if (wcpPercent != NULL)//�Ƿ���"|"��copying u-boot.bin to device
					{
						dNew_Percent += 1.0;
						this->Show_Out(wcpEach);
						this->Show_Status(_T("Copying u-boot.bin to device"));

						//��ʾ������
						this->Show_Progress((1.0 * (dNew_Percent / 68.0)));
					}
					else //�Ƿ��С�to device ...  ��ʼ���ؼ���Writing image "RK_03_00_00_00_04121_001-full_img_1250.bin" to device ... (100% completed)
					{
						wcpPercent = wcsstr(wcpEach, _T("to device ... "));
						//
						if (wcpPercent != NULL)
						{
							//���ݵ�������
							strEach.Format(_T("%s"), wcpPercent);

							//��ȡ�ٷֱ�����
							wcpPercent += strlen("to device ... ");

							//�Ƿ��С�percent �������ؼ���
							if (wcsstr(wcpPercent, _T("%")) != NULL)
							{
								//���ְٷֱ����ݵġ���β��(28% completed)  
								strData.Format(_T("%s"), wcpPercent);
								strData.Replace(_T(" "), _T(""));//ȥ����β
								strData.Replace(_T("%"), _T(""));//ȥ����β
								strData.Replace(_T("("), _T(""));//ȥ����β
								strData.Replace(_T(")"), _T(""));//ȥ����β
								strData.Replace(_T("completed"), _T(""));//ȥ����β

								//�ַ�ת����
								dNew_Percent = _wtof(strData);

								//ˢ�°ٷֱ�����
								if (dNew_Percent > dCurr_Percent)
								{
									//��ֵ�°ٷֱ�����
									dCurr_Percent = dNew_Percent;
									strData.Format(_T("Writing image( %.0f%% )"), dCurr_Percent);
									this->Show_Out(wcpEach);
									this->Show_Status(strData);
									//this->Save_Trace_Log(csTemp);
									//��ʾ������
									this->Show_Progress((1.0 * (dCurr_Percent / 100.0)));
								}
							}//if(wcsstr(wcpPercent, _T("%")) != NULL
						}//if(wcpPercent != NULL
						else
						{
							this->Show_Out(wcpEach);
							if (wcsstr(strInfo, _T("writing image")) == NULL)
							{
								this->Show_Status(wcpEach);
							}
						}
					}

					//��ȡ��һ��ֵ
					wcpEach = wcstok(NULL, _T("\r\n"));
				}
			}//ulActual_Read > 0

			//����Ƿ�completed successfully
			if (wcsstr(strInfo, _T("completed successfully")) != NULL)
			{
				this->Save_Trace_Log(strInfo);
				bCompletedSuccess = true;//pass
			}

			//���error��additional information
			if (wcsstr(strInfo, _T("error")) != NULL
				&& wcsstr(strInfo, _T("additional information")) != NULL)
			{
				this->Save_Trace_Log(strInfo);
				m_csError_Code.Format(_T("FAIL, %s"), strInfo);
//				m_csError_Code = _T("FAIL, DL_FactoryRecoveryModeFW error!");
				// ������Ļس�����,����button����Ӧenter��Ϣ����������
				Sleep(2000);
				throw(0);
			}
			// System.NullReferenceException
			if (wcsstr(strInfo, _T("system.nullreferenceexception")) != NULL)
			{
				this->Save_Trace_Log(strInfo);
				m_csError_Code.Format(_T("FAIL, %s"), strInfo);
//				m_csError_Code = _T("FAIL, DL_FactoryRecoveryModeFW error!");
				// ������Ļس�����,����button����Ӧenter��Ϣ����������
				Sleep(2000);
				throw(0);
			}

			//����Ƿ�Resetting device�� ����log���жϸ��ַ�����Ϊ�˹�����Ļس�����,����button����Ӧenter��Ϣ����������
			if (wcsstr(strInfo, _T("resetting device")) != NULL)
			{
				this->Save_Trace_Log(strInfo);
				bResetDevice = true;//pass
			}

			if (true == bCompletedSuccess && true == bResetDevice)
			{
				bSuccess_Flag = true;
				Sleep(2000);
				break;//�˳�while
			}

			//��ʱ���
			time(&t_End);
			//
			if ((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				this->Save_Trace_Log(strInfo);
				m_csError_Code = _T("FAIL, DL_FactoryRecoveryModeFW Time out!");
				//				this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_TIME_OUT;
				throw(0);
			}
		}// bSuccess_Flag == false

		Sleep(10);
	}
	catch (...)
	{
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

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);
		return false;
	}
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
// �ǿ�Ƭ
bool CARM_OP_ALTAIR::DL_FactoryFW()
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
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo, strEach, strBinFile;
	time_t t_Start, t_End;
	bool bCompletedSuccess = false;
	bool bResetDevice = false;
	bool bSuccess_Flag = false;
	int iTimeout = 200;//��ʱ5s
	int iTemp = 0;
	int nPos = 0;
	double dCurr_Percent = 0.0;
	double dNew_Percent = 0.0;

	try
	{
		//��ʾ
		//this->Show_Status(_T("Reset the module ......"));
		//this->Save_Trace_Log(_T("Reset the module ......"));
		this->Show_Status(_T("DL start ......"));
		this->Save_Trace_Log(_T("DL start ......"));

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if (CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL_FactoryFW, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + m_strFWFolderPath.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH * 2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH * 2);
		//////////////////////////////////////////////////////////////////////////
		// ImageBurnTool.exe -v "D:\BG770AGLAAR01A02_01.003V01.01.003V01_suggest\RK_03_00" E0_SB /0 /1 /2 /3 /4 /6 /7 /11 /12 /18 -uartparams 0 23 3000000 1 -s 1 -n 1
		if (m_bIsBaudHighSpeed == true)
		{
			swprintf_s(wcpCmd, nLen, _T("powershell.exe %cc \"%s\\Altair\\%d\\ImageBurnTool%d.exe -v \"%s\" %s /0 /1 /2 /3 /4 /6 /7 /11 /12 /18 -uartparams 0 %d 3000000 %d -s 1 -n 1\""),
				'/', m_csCurr_Exe_Path, m_iClass_Idx, m_iClass_Idx, m_strFWFolderPath, m_strChipType, m_iCom_Port, m_iFlowControl);
		}
		else
		{
			swprintf_s(wcpCmd, nLen, _T("powershell.exe %cc \"%s\\Altair\\%d\\ImageBurnTool%d.exe -v \"%s\" %s /0 /1 /2 /3 /4 /6 /7 /11 /12 /18 -uartparams 0 %d 921600 %d -s 1 -n 1\""),
				'/', m_csCurr_Exe_Path, m_iClass_Idx, m_iClass_Idx, m_strFWFolderPath, m_strChipType, m_iCom_Port, m_iFlowControl);
		}

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.hStdInput = hRead_Pipe;
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��

		//�����ӽ���
		if (CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL_FactoryFW, Create process fail");
			throw(0);
		}

		this->Save_Trace_Log(wcpCmd);


		if (!SetHandleInformation(hRead_Pipe, HANDLE_FLAG_INHERIT, 0))
		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if (hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//��ʼ��ʱ
		time(&t_Start);
		while (bSuccess_Flag == false)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 512, &ulActual_Read, NULL);
			//����info������
			if (ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//��������
				strInfo.Format(_T("%s"), wcpData);
//				TRACE(strInfo);
				strInfo.MakeLower();
				strInfo.Replace(_T("\r"), _T(""));
				strInfo.Replace(_T("\n"), _T(""));
				//��ȡ��Ҫ�ġ����Ȱٷֱ���Ϣ��
				wcpEach = wcstok(wcpData, _T("\r\n"));
				//
				while (wcpEach != NULL)
				{
					//�Ƿ��С�to device ...  ��ʼ���ؼ���
					//Writing image "RK_03_00_00_00_04121_001-full_img_1250.bin" to device ... (100% completed)
					wcpPercent = wcsstr(wcpEach, _T("to device ... "));
					//
					if (wcpPercent != NULL)
					{
						//���ݵ�������
						strEach.Format(_T("%s"), wcpPercent);

						//��ȡ�ٷֱ�����
						wcpPercent += strlen("to device ... ");

						//�Ƿ��С�percent �������ؼ���
						if (wcsstr(wcpPercent, _T("%")) != NULL)
						{
							//���ְٷֱ����ݵġ���β��(28% completed)  
							strData.Format(_T("%s"), wcpPercent);
							strData.Replace(_T(" "), _T(""));//ȥ����β
							strData.Replace(_T("%"), _T(""));//ȥ����β
							strData.Replace(_T("("), _T(""));//ȥ����β
							strData.Replace(_T(")"), _T(""));//ȥ����β
							strData.Replace(_T("completed"), _T(""));//ȥ����β

							//�ַ�ת����
							dNew_Percent = _wtof(strData);

							//ˢ�°ٷֱ�����
							if (dNew_Percent > dCurr_Percent)
							{
								//��ֵ�°ٷֱ�����
								dCurr_Percent = dNew_Percent;
								strData.Format(_T("Writing image( %.0f%% )"), dCurr_Percent);
								this->Show_Out(wcpEach);
								this->Show_Status(strData);
								//this->Save_Trace_Log(csTemp);
								//���ӳ٣��򿴲�����ʾ��Ϣ�ı仯

								//��ʾ������
								this->Show_Progress((1.0 * (dCurr_Percent / 100.0)));
							}
						}//if(wcsstr(wcpPercent, _T("%")) != NULL
					}//if(wcpPercent != NULL
					else
					{
						this->Show_Out(wcpEach);
						if (wcsstr(strInfo, _T("writing image")) == NULL)
						{
							this->Show_Status(wcpEach);
						}
					}

					//��ȡ��һ��ֵ
					wcpEach = wcstok(NULL, _T("\r\n"));
				}
			}//ulActual_Read > 0

			//����Ƿ�completed successfully
			if (wcsstr(strInfo, _T("completed successfully")) != NULL)
			{
				this->Save_Trace_Log(strInfo);
				bCompletedSuccess = true;//pass
			}

			//���error��additional information
			if (wcsstr(strInfo, _T("error")) != NULL
				&& wcsstr(strInfo, _T("additional information")) != NULL)
			{
				this->Save_Trace_Log(strInfo);
				m_csError_Code.Format(_T("FAIL, %s"), strInfo);
				//				m_csError_Code = _T("FAIL, DL_FactoryRecoveryModeFW error!");
				// ������Ļس�����,����button����Ӧenter��Ϣ����������
				Sleep(2000);
				throw(0);
			}
			// System.NullReferenceException
			if (wcsstr(strInfo, _T("system.nullreferenceexception")) != NULL)
			{
				this->Save_Trace_Log(strInfo);
				m_csError_Code.Format(_T("FAIL, %s"), strInfo);
				//				m_csError_Code = _T("FAIL, DL_FactoryRecoveryModeFW error!");
				// ������Ļس�����,����button����Ӧenter��Ϣ����������
				Sleep(2000);
				throw(0);
			}

			//����Ƿ�Resetting device�� ����log���жϸ��ַ�����Ϊ�˹�����Ļس�����,����button����Ӧenter��Ϣ����������
			if (wcsstr(strInfo, _T("resetting device")) != NULL)
			{
				this->Save_Trace_Log(strInfo);
				bResetDevice = true;//pass
			}

			if (true == bCompletedSuccess && true == bResetDevice)
			{
				Sleep(2000);
				bSuccess_Flag = true;
				break;//�˳�while
			}

			//��ʱ���
			time(&t_End);
			//
			if ((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				this->Save_Trace_Log(strInfo);
				m_csError_Code = _T("FAIL, DL_FactoryFW Time out!");
//				this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_TIME_OUT;
				throw(0);
			}
		}// bSuccess_Flag == false

		Sleep(10);
	}
	catch (...)
	{
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

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);
		return false;
	}
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
