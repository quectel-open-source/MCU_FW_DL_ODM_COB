#include "StdAfx.h"
#include "ARM_OP_EIGEN.h"
#include "ts_lan.h"



CARM_OP_EIGEN::CARM_OP_EIGEN(void)
: m_iCom_Port(0)
, m_bRuning(false)
, m_iStop_Flag(-1)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_csCurr_Exe_Path(_T(""))
, m_iResetFlag(0)
, m_iProdu_Type(0)
, m_csSrcIni_File(_T(""))
, m_iBBchip_Curr(0)
{
	m_iOP_Robot = 0;
	m_Ts_Num = 0;
	m_iTimeInterval = 0;
}


CARM_OP_EIGEN::~CARM_OP_EIGEN(void)
{


}
void CARM_OP_EIGEN::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_EIGEN::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_EIGEN::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_EIGEN::Show_Progress(double d_Pgr_Percent)
{
	//�˺�����Ӧ�ð����������ٷֱȵļ��㣬Ȼ������Ϣ��Ӧ��������ɾ��岽�������ļ���
	//Progress�ܳ�����1000
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}


void CARM_OP_EIGEN::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}


bool CARM_OP_EIGEN::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir;
	CString strData;
	CFileFind cla_Finder; //�ļ�������
	BOOL b_Found;

	try
	{
		//--------------------------------------- ��ʼ������Ϣ������EIGEN�����ļ� --------------------------------------- 
		//���EIGEN�������ļ��б�
		this->m_FWInfo.Clear();

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

			if(!cla_Finder.IsDirectory())	//��������ļ��У������ļ�
			{
				cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//��ȡ�ҵ����ļ���

				cs_FW_Name.MakeUpper();

				//20200806 flash_download.ini
				if (wcsstr(cs_FW_Name, _T(".INI")) != NULL)
				{
					m_csSrcIni_File.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());
	
				}

				

				// �ж���bin�ļ�
				if (wcsstr(cs_FW_Name, _T(".BIN")) != NULL)
				{
					//�ж�AGENTBOOT.BIN�ļ�
					if(wcsstr(cs_FW_Name, _T("AGENTBOOT.BIN")) != NULL)
					{
						m_FWInfo.strAgentBoot.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

						m_FWInfo.b_Enable_DL = true;
					}
					if (wcsstr(cs_FW_Name, _T("_HEAD")) == NULL)
					{
						//�ж�bootloader.bin�ļ�
						if((wcsstr(cs_FW_Name, _T("BOOTLOADER_BC260")) != NULL)
							||(wcsstr(cs_FW_Name, _T("BOOTLOADER_BC950FCN")) != NULL)
							||(wcsstr(cs_FW_Name, _T("BOOTLOADER_BC660K")) != NULL)
							||(wcsstr(cs_FW_Name, _T("BOOTLOADER_BC95GF")) != NULL)
							||(wcsstr(cs_FW_Name, _T("BOOTLOADER_BC28F")) != NULL)
							)

						{
							m_FWInfo.strBootLoader.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

							m_FWInfo.b_Enable_DL = true;
						}
						else //�ж�system�ļ�
						{
							if((wcsstr(cs_FW_Name, _T("BC260")) != NULL)
								||(wcsstr(cs_FW_Name, _T("BC950")) != NULL)
								||(wcsstr(cs_FW_Name, _T("BC660K")) != NULL)
								||(wcsstr(cs_FW_Name, _T("BC95GF")) != NULL)
								||(wcsstr(cs_FW_Name, _T("BC28F")) != NULL)
								)
							{
								m_FWInfo.strSystem.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

								m_FWInfo.b_Enable_DL = true;
							}
						}
					}
#ifdef FW_FACTORY
					if (PRODU_TYPE_MOB != this->m_iProdu_Type)
					{
						//�ж�MergeRfTable.bin�ļ�
						if(wcsstr(cs_FW_Name, _T("MERGERFTABLE")) != NULL)
						{
							m_FWInfo.strRF.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

							m_FWInfo.b_Enable_DL = true;
						}
					}
					else
					{
						//...
					}
#endif
				}

			}
		}//while(b_Found

		//
		cla_Finder.Close();
		//

		// save parameters in config.ini
		this->Save_Config();

		if (m_FWInfo.strBootLoader.IsEmpty())
		{
			strData = _T("bootloader bin file");
			m_FWInfo.b_Enable_DL = false;
		}
		else if (m_FWInfo.strSystem.IsEmpty())
		{
			strData = _T("system bin file");
			m_FWInfo.b_Enable_DL = false;
		}
#ifdef FW_FACTORY
		else 
		{
			if ((PRODU_TYPE_MOB != this->m_iProdu_Type) && (m_FWInfo.strRF.IsEmpty()))
			{
				strData = _T("RF bin file");
				m_FWInfo.b_Enable_DL = false;
			}
		}
#endif
		if(m_FWInfo.b_Enable_DL == false)
		{
			this->m_csError_Code.Format(_T("error,load_fw_file,There is no %s.file!"), strData);
			throw(0);
		}

	}
	catch(...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}

	return true;
}

int CARM_OP_EIGEN::Do_Execute(void)
{
	CString strData;
	try
	{
		m_bRuning = true;

		this->m_csError_Code.Format(_T(""));
		this->m_iError_Code = 0;

		// kill flashtoolCLI.exe

		strData.Format(_T("flashtoolcli%d.exe"), m_iClass_Idx);
		m_cla_GFC.Kill_Process(strData);

		//������QFLASH�Ķ˿����������������Ҫ�ٸ����������ļ�
#ifndef FW_MULTI	
		// save parameters in config.ini
		this->Save_Config();
#endif
	

		// 1. ̽������
		if(this->DL_Probe(strData) == false)
		{
			throw(1);
		}

#ifdef FW_FACTORY
		if (PRODU_TYPE_MOB != this->m_iProdu_Type)
		{
			// 2. ȫ��
			if(this->DL_EraseAll(strData) == false)
			{
				throw(1);
			}
		}
#endif

		//3. ���ع̼�Download the boot loader and system
		if(this->DL_Bootloader_System(strData) == false)
		{
			throw(3);
		}

#ifdef FW_FACTORY
		if (PRODU_TYPE_MOB != this->m_iProdu_Type)
		{
			// 4. ����RF Download the flex file 
			if (this->DL_FlexFile(1, strData) == false)
			{
				throw(4);
			}
		
			if (this->DL_FlexFile(2, strData) == false)
	 		{
	 			throw(4);
	 		}
		}
#endif

#ifndef FW_FACTORY
		// 5. ����NV����
		if(this->DL_EraseNV(strData) == false)
		{
			throw(1);
		}
// 		//6. ����������
// 		if(this->DL_Reset(strData) == false)
// 		{
// 			throw(1);
// 		}


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
//�Զ��߳̿ص�Դ�ϵ�
void CARM_OP_EIGEN::Set_PowerOn_SPC(int i_Index)
{
	this->Open_Power_SPC(i_Index);
	//::SendMessage(this->m_hWnd_View, WM_PowerOn_SPC, (WPARAM)i_Index, (LPARAM)i_Index);
}
//ÿƬģ������ǰ�������ÿ��ͨ���ĳ̿ص�Դ�ϵ�
void  CARM_OP_EIGEN::Open_Power_SPC(int iIndex)
{
#ifdef	FW_MULTI
#ifndef FW_UPGRADE

	switch (iIndex)
	{
	case 1:
	{
		//�򿪵�Դ
		this->m_cla_PS_1->Power_Off(1);

		this->Show_Out(_T("----Power_Off----"));

		Sleep(2000);

		//�ϵ�
		//�̿ص�Դ�ϵ�
		if (this->m_cla_PS_1->m_bCtrl_PS == true)
		{
			this->m_cla_PS_1->POWER_ON_V1(1);

			this->Show_Out(_T("----Power_On----"));

			//
			Sleep(500);
		}
		else
		{
			this->Show_Out(_T("m_cla_PS_1->m_bCtrl_PS= false"));
		}

	}
	break;
	case 2:
	{
		//�򿪵�Դ
		this->m_cla_PS_2->Power_Off(1);

		this->Show_Out(_T("----Power_Off----"));

		Sleep(2000);

		//�ϵ�
		//�̿ص�Դ�ϵ�
		if (this->m_cla_PS_2->m_bCtrl_PS == true)
		{
			this->m_cla_PS_2->POWER_ON_V1(1);

			this->Show_Out(_T("----Power_On----"));
			//
			Sleep(500);
		}
		else
		{
			this->Show_Out(_T("m_cla_PS_2->m_bCtrl_PS= false"));
		}

	}
	break;
	case 3:
	{
		//�򿪵�Դ
		this->m_cla_PS_3->Power_Off(1);

		this->Show_Out(_T("----Power_Off----"));

		Sleep(2000);

		//�ϵ�
		//�̿ص�Դ�ϵ�
		if (this->m_cla_PS_3->m_bCtrl_PS == true)
		{
			this->m_cla_PS_3->POWER_ON_V1(1);

			this->Show_Out(_T("----Power_On----"));
			//
			Sleep(500);
		}
		else
		{
			this->Show_Out(_T("m_cla_PS_3->m_bCtrl_PS= false"));
		}

	}
	break;
	case 4:
	{
		//�򿪵�Դ
		this->m_cla_PS_4->Power_Off(1);

		this->Show_Out(_T("----Power_Off----"));

		Sleep(2000);

		//�ϵ�
		//�̿ص�Դ�ϵ�
		if (this->m_cla_PS_4->m_bCtrl_PS == true)
		{
			this->m_cla_PS_4->POWER_ON_V1(1);

			this->Show_Out(_T("----Power_On----"));

			//
			Sleep(500);
		}
		else
		{
			this->Show_Out(_T("m_cla_PS_4->m_bCtrl_PS= false"));
		}

	}
	break;
	default:break;
	}

#endif
#endif
}
bool CARM_OP_EIGEN::DL_Probe(const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 15;//��ʱ5s
	int iTemp = 0;
	int nPos = 0;

	try
	{
		//��ʾ
		this->Show_Status(_T("Probe ......"));
		this->Save_Trace_Log(_T("Probe ......"));

		int iTimeInterval = m_iTimeInterval;

		if (0 == iTimeInterval)
		{
			iTimeInterval = 4000;//
		}

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_Probe, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		// FlashToolCLI.exe --cfgfile config.ini --port="COM39" probe
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --cfgfile %s\\EFlashTool\\%d\\config.ini  --port=\"COM%d\" probe \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe,m_csCurr_Exe_Path, m_iClass_Idx, m_iCom_Port);

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//�����ӽ���
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_Probe, Create process fail");
			throw(0);
		}

		this->Save_Trace_Log(wcpCmd);

		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//��ʼ��ʱ
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//��������
				strInfo.Format(_T("%s"), wcpData);

				this->Show_Out(strInfo);
				this->Save_Trace_Log(strInfo);
			}
			//��ʾģ��Reset
			if(wcsstr(strInfo, _T("BootPreemptDet start")) != NULL)//20201213
			{
				//��ʾ
				this->Show_Status(_T("Reset ......"));
				this->Save_Trace_Log(_T("Reset ......"));

				if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
				{		
					this->Save_Trace_Log(_T("DC Reset AUTO_ROBOT......"));

					//���ܿ�PC����DC����ָ��
					//bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("REON") );
					::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC(m_Ts_Num, _T("OFF"));
					Sleep(iTimeInterval);
					::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC(m_Ts_Num, _T("ON"));
				}
				else  if (LOAD_AUTO_ROBOT_SPC == m_iOP_Robot) //ȫ�Զ�SPC
				{
					this->Save_Trace_Log(_T("DC Reset ROBOT_SPC......"));

					this->Set_PowerOn_SPC(this->m_iClass_Idx);
				}
				else
				{
					//�������µ�
					//...
				}


			}
			//����Ƿ��д��󱨳�
			if(wcsstr(strInfo, _T("[Error Info]")) != NULL)
			{
				nPos = strInfo.Find(_T("[Error Info]"));
				strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
				strData.Replace('[', ' ');//ȥ��������ʾ���ַ�
				strData.Replace(']', ' ');//ȥ��������ʾ���ַ�
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if(wcsstr(strInfo, _T("failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Probe fail!"));
				this->m_iError_Code = ERR_DL_PROBE_TIME_FAIL;
				throw(0);
			}


			//����Ƿ�pass
			if(wcsstr(strInfo, _T("Burn OK")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//�˳�while
			}

			//��ʱ���
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				m_csError_Code = _T("FAIL, DL_Probe Time out!");
				this->m_iError_Code = ERR_DL_PROBE_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
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
	if(hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if(hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}

	return true;
}

bool CARM_OP_EIGEN::DL_EraseAll(const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 30;//��ʱ5s
	int iTemp = 0;
	int nPos = 0;

	try
	{
		//��ʾ
		this->Show_Status(_T("erase flash ......"));
		this->Save_Trace_Log(_T("erase flash ......"));

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_EraseAll, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		// FlashToolCLI.exe --skipconnect 1 --cfgfile config.ini --port="COM39" flasherase 0x0 0x400000
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --skipconnect 1  --cfgfile %s\\EFlashTool\\%d\\config.ini --port=\"COM%d\" flasherase 0x0 0x400000 \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe,m_csCurr_Exe_Path, m_iClass_Idx, m_iCom_Port);

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//�����ӽ���
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_EraseAll, Create process fail");
			throw(0);
		}
		this->Save_Trace_Log(wcpCmd);

		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//��ʼ��ʱ
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//��������
				strInfo.Format(_T("%s"), wcpData);
// 				strInfo.Replace(_T('\r'), _T(' '));
// 				strInfo.Replace(_T('\n'), _T(' '));
				this->Show_Out(strInfo);
				this->Save_Trace_Log(strInfo);
				//���ӳ٣��򿴲�����ʾ��Ϣ�ı仯
			}
			//����Ƿ��д��󱨳�
			if(wcsstr(strInfo, _T("[Error Info]")) != NULL)
			{
				nPos = strInfo.Find(_T("[Error Info]"));
				strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
				strData.Replace('[', ' ');//ȥ��������ʾ���ַ�
				strData.Replace(']', ' ');//ȥ��������ʾ���ַ�
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if(wcsstr(strInfo, _T("Burn Failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_EraseAll fail!"));
				this->m_iError_Code = ERR_DL_ERASEALL_FAIL;
				throw(3);
			}


			//����Ƿ�pass
			if(wcsstr(strInfo, _T("Erase OK")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//�˳�while
			}

			//��ʱ���
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				m_csError_Code = _T("FAIL, DL_EraseAll Time out!");
				this->m_iError_Code = ERR_DL_ERASEALL_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);

		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		return false;
	}
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	//�ͷž����ָ��
	if(hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if(hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}

	return true;
}

bool CARM_OP_EIGEN::DL_Bootloader_System(const CString& strCmdExe)
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
	CString strData, strInfo, strEach, strBinFile, csTotal_Info;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 60;//��ʱ5s
	int iTemp = 0;
	int nPos = 0;
	double dCurr_Percent = 0.0;
	double dNew_Percent = 0.0;

	try
	{
		//��ʾ
		this->Show_Status(_T("Download bootloader and system file ......"));
		this->Save_Trace_Log(_T("Download bootloader and system file ......"));

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_BOOTLOADER_SYSTEM, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		// FlashToolCLI.exe --skipconnect 1 --cfgfile config.ini --port="COM39" burn
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --skipconnect 1 --cfgfile %s\\EFlashTool\\%d\\config.ini --port=\"COM%d\" burn \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe,m_csCurr_Exe_Path, m_iClass_Idx, m_iCom_Port);

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//�����ӽ���
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_BOOTLOADER_SYSTEM,, Create process fail");
			throw(0);
		}

		this->Save_Trace_Log(wcpCmd);

		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//��ʼ��ʱ
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//��������
				strInfo.Format(_T("%s"), wcpData);

				//��ȡ��Ҫ�ġ����Ȱٷֱ���Ϣ��
				//��ȡ��һ��ֵ(0)
				wcpEach = wcstok_s(wcpData, _T("\r\n"), &wcpNextToken);
				//
				while(wcpEach != NULL)
				{
					//�Ƿ��С�percent ��ʼ���ؼ���
					strBinFile.Format(_T("{%s-percent files transferred"), m_strBootloaderFile);
					wcpPercent = wcsstr(wcpEach, strBinFile);
					if (wcpPercent == NULL)
					{
						strBinFile.Format(_T("{%s-percent files transferred"),m_strSystemFile);
						wcpPercent = wcsstr(wcpEach, strBinFile);
					}
					//
					if(wcpPercent != NULL)
					{
						//���ݵ�������
						strEach.Format(_T("%s"), wcpPercent);

						//��ȡ�ٷֱ�����
						wcpPercent += strBinFile.GetLength();

						//�Ƿ��С�percent �������ؼ���
						if(wcsstr(wcpPercent, _T("%")) != NULL)
						{
							//���ְٷֱ����ݵġ���β��
							strData.Format(_T("%s"), wcpPercent);
							strData.Replace('%', '\0');//ȥ����β

							//�ַ�ת����
							dNew_Percent = _wtof(strData);

							//ˢ�°ٷֱ�����
							if(dNew_Percent > dCurr_Percent)
							{
								//��ֵ�°ٷֱ�����
								dCurr_Percent = dNew_Percent;

 
 								//��ʾ��������Ϣ���ݡ�
								this->Show_Out(strInfo);
								//���ӳ٣��򿴲�����ʾ��Ϣ�ı仯
//								Sleep(20);


								//��ʾ������
								if(wcsstr(strInfo, m_strBootloaderFile) != NULL)
								{
#ifdef FW_FACTORY
									this->Show_Progress( (0.1 * dCurr_Percent / 100.0) );
#else
									this->Show_Progress( (0.2 * dCurr_Percent / 100.0) );
#endif
									strData.Format(_T("DL_BOOTLOADER_SYSTEM(%.0f%%), %s"), dCurr_Percent, m_strBootloaderFile);
									this->Show_Status(strData);
									if (wcsstr(strInfo, _T("100")) != NULL)
									{
										dCurr_Percent = 0.0;
									}
								}
								else if (wcsstr(strInfo, m_strSystemFile) != NULL)
								{
#ifdef FW_FACTORY
									this->Show_Progress( (0.1 + 0.85 * dCurr_Percent / 100.0) );
#else
									this->Show_Progress( (0.2 + 0.8 * dCurr_Percent / 100.0) );
#endif
									strData.Format(_T("DL_BOOTLOADER_SYSTEM(%.0f%%), %s"), dCurr_Percent, m_strSystemFile);
									this->Show_Status(strData);
								}
							}
						}//if(wcsstr(wcpPercent, _T("%")) != NULL
					}//if(wcpPercent != NULL

					//��ȡ��һ��ֵ
					wcpEach = wcstok_s(NULL, _T("\r\n"), &wcpNextToken);
				}//while(wcpEach != NULL
 			}
			
			this->Save_Trace_Log(strInfo);


			//��������
			if ((dCurr_Percent > 90.0)
				|| (dCurr_Percent<1.0))//
			{
				csTotal_Info += strInfo;
			}
			else
			{
				csTotal_Info.Format(_T("%s"), strInfo);
			}

			//����Ƿ��д��󱨳�
			if (wcsstr(csTotal_Info, _T("[Error Info]")) != NULL)
			{
				nPos = csTotal_Info.Find(_T("[Error Info]"));

				strInfo = csTotal_Info.Right(csTotal_Info.GetLength() - nPos);

				strData = strInfo.Right(strInfo.GetLength()- 13);

				strData.Replace('[', ' ');//ȥ��������ʾ���ַ�
				strData.Replace(']', ' ');//ȥ��������ʾ���ַ�
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_FAIL_EXT;
				throw(0);
			}

			if (wcsstr(csTotal_Info, _T("Burn Failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_BOOTLOADER_SYSTEM fail!"));
				this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_FAIL;
				throw(3);
			}


			//����Ƿ�pass
			if (wcsstr(csTotal_Info, _T("Burn OK")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//�˳�while
			}

			//��ʱ���
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				m_csError_Code = _T("FAIL, DL_BOOTLOADER_SYSTEM Time out!");
				this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);

		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		return false;
	}
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	//�ͷž����ָ��
	if(hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if(hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}

	return true;
}
bool CARM_OP_EIGEN::DL_FlexFile(int iIndex, const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 10;//��ʱ5s
	int iTemp = 0;
	int nPos = 0;

	try
	{
		//��ʾ
		strData.Format(_T("Download flexfile%d file"), iIndex-1);
		this->Show_Status(strData);
		this->Save_Trace_Log(strData);

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code.Format(_T("FAIL, DL_FlexFile%d, Create pipe fail!"), iIndex-1);
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --skipconnect 1 --cfgfile %s\\EFlashTool\\%d\\config.ini --port=\"COM%d\" burnone flexfile%d \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe, m_csCurr_Exe_Path, m_iClass_Idx,m_iCom_Port, iIndex-1);

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//�����ӽ���
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			m_csError_Code.Format(_T("FAIL, DL_FlexFile%d, Create process fail!"), iIndex-1);
			throw(0);
		}

		this->Save_Trace_Log(wcpCmd);

		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//��ʼ��ʱ
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//��������
				strInfo.Format(_T("%s"), wcpData);
				this->Save_Trace_Log(strInfo);
			}
			//����Ƿ��д��󱨳�
			if(wcsstr(strInfo, _T("[Error Info]")) != NULL)
			{
				nPos = strInfo.Find(_T("[Error Info]"));
				strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
				strData.Replace('[', ' ');//ȥ��������ʾ���ַ�
				strData.Replace(']', ' ');//ȥ��������ʾ���ַ�
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if(wcsstr(strInfo, _T("Burn Failed")) != NULL)
			{
				m_csError_Code.Format(_T("FAIL, DL_FlexFile%d fail!"), iIndex-1);
				this->m_iError_Code = ERR_DL_FLEX_FAIL;
				throw(0);
			}

			//����Ƿ�pass
			if(wcsstr(strInfo, _T("Burn OK")) != NULL)
			{
				if (iIndex == 1)
				{
					this->Show_Progress(1.0);
				}
				else
				{
					this->Show_Progress(1.0);
				}
				bSuccess_Flag = TRUE;//pass
				break;//�˳�while
			}

			//��ʱ���
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				m_csError_Code.Format(_T("FAIL, DL_FlexFile%d Time out!"), iIndex-1);
				this->m_iError_Code = ERR_DL_FLEX_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);

		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}



		return false;
	}
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	//�ͷž����ָ��
	if(hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if(hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}


	return true;

}
void CARM_OP_EIGEN::Save_Config()
{
	CString strData, strPort;
	int nPos;
	//20200806 �����ӹ̼����е�flash_download.ini������config.ini
	//1.ɾ������Ŀ¼�µ��ϵ������ļ�////////////////////////////////////////////////////////////////////////////////
	BOOL bCopy = FALSE;

	unsigned long ul_File_Size;

	if(this->Get_File_Size(m_cla_ConfigFile.m_csIniFileName, &ul_File_Size) == true)
	{
		::SetFileAttributes(m_cla_ConfigFile.m_csIniFileName, FILE_ATTRIBUTE_NORMAL);  

		::DeleteFile(m_cla_ConfigFile.m_csIniFileName); 

		Sleep(2000);
	}
	//2.���̼����е�INI�����ļ����Ƶ�����Ŀ¼��////////////////////////////////////////////////////////////////////////////////

	CStdioFile cla_File_Dest;
	CString csData_Buff1;

	if (cla_File_Dest.Open(m_cla_ConfigFile.m_csIniFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::typeText) != NULL)
	{
		cla_File_Dest.SeekToBegin();
		//[config]
		csData_Buff1 = _T("[config]\r\n");
		cla_File_Dest.WriteString(csData_Buff1);
		//line_0_com = COM29
		strData.Format(_T("line_%d_com = COM%d\r\n"), m_iClass_Idx - 1, m_iCom_Port);
		csData_Buff1 = strData;
		cla_File_Dest.WriteString(csData_Buff1);
		//agbaud = 921600
		csData_Buff1.Format(_T("agbaud = %d\r\n"), m_iBaud_Rate);
		cla_File_Dest.WriteString(csData_Buff1);
		//;bootloader.bin file infomation
		csData_Buff1.Format(_T(";bootloader.bin file infomation\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//[bootloader]
		csData_Buff1.Format(_T("[bootloader]\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//blpath = .\image_ec616s\bootloader.bin		
		csData_Buff1.Format(_T("blpath = %s\r\n"), m_FWInfo.strBootLoader);
		cla_File_Dest.WriteString(csData_Buff1);
		//blloadskip = 0
		csData_Buff1.Format(_T("blloadskip = 0\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//;system.bin file infomation
		csData_Buff1.Format(_T(";system.bin file infomation\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//[system]
		csData_Buff1.Format(_T("[system]\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//syspath = .\image_ec616s\app - demo - flash.bin
		csData_Buff1.Format(_T("syspath = %s\r\n"), m_FWInfo.strSystem);
		cla_File_Dest.WriteString(csData_Buff1);
		//sysloadskip = 0
		csData_Buff1.Format(_T("sysloadskip = 0\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//;control such as reset before download
		csData_Buff1.Format(_T(";control such as reset before download\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//[control]
		csData_Buff1.Format(_T("[control]\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
//#ifdef FW_FACTORY
		if (BB_EC617 == m_iBBchip_Curr)
		{
			//detect = 2
			//csData_Buff1.Format(_T("detect = 2\r\n"));
			csData_Buff1.Format(_T("detect = 1\r\n"));
			cla_File_Dest.WriteString(csData_Buff1);

			csData_Buff1.Format(_T("atreset = at+ecrst=delay,500\r\n"));
			cla_File_Dest.WriteString(csData_Buff1);
			
			//#else
			//		//detect = 1
			//		csData_Buff1.Format(_T("detect = 1\r\n"));
			//		cla_File_Dest.WriteString(csData_Buff1);
			//		//atreset = at + ecrst = delay, 200
			//		csData_Buff1.Format(_T("atreset=at+ecrst=delay,200\r\n"));
			//		cla_File_Dest.WriteString(csData_Buff1);
			//#endif

			//prempt_detect_time = 6
			csData_Buff1.Format(_T("prempt_detect_time = 6\r\n"));
			cla_File_Dest.WriteString(csData_Buff1);
		}
		//msg_waittime = 2
		csData_Buff1.Format(_T("msg_waittime = 2\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//max_preamble_cnt = 8
		csData_Buff1.Format(_T("max_preamble_cnt = 8\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//lpc_recover_en = 0
		csData_Buff1.Format(_T("lpc_recover_en = 0\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//; cfg reset to 2, skip reset pin
		csData_Buff1.Format(_T(";cfg reset to 2, skip reset pin\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);

		if (BB_EC617 == m_iBBchip_Curr)
		{
			//pullup_qspi = 1-------EC616S
			csData_Buff1.Format(_T("pullup_qspi = 1\r\n"));
			cla_File_Dest.WriteString(csData_Buff1);
		}

		//[flexfile0]
		csData_Buff1.Format(_T("[flexfile0]\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//filepath = .\rfCaliTb_ec616s\MergeRfTable.bin
		csData_Buff1.Format(_T("filepath = %s\r\n"), m_FWInfo.strRF);
		cla_File_Dest.WriteString(csData_Buff1);
		//burnaddr = 0x3A4000
		csData_Buff1.Format(_T("burnaddr = 0x3A4000\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//[flexfile1]
		csData_Buff1.Format(_T("[flexfile1]\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//filepath = .\rfCaliTb_ec616s\MergeRfTable.bin
		csData_Buff1.Format(_T("filepath = %s\r\n"), m_FWInfo.strRF);
		cla_File_Dest.WriteString(csData_Buff1);
		//burnaddr = 0x16000
		csData_Buff1.Format(_T("burnaddr = 0x16000\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);

		cla_File_Dest.Close();

	}
	else
	{
		//
		strData.Format(_T("%s Open Fail"), m_cla_ConfigFile.m_csIniFileName);
		Show_Out(strData);

	}


	//3.����ʵ�ʹ̼��ļ�·���޸Ĺ��߰��µ������ļ�////////////////////////////////////////////////////////////////////////////////
	nPos = m_FWInfo.strBootLoader.ReverseFind('\\');
	if (nPos != -1)
	{
		m_strBootloaderFile = m_FWInfo.strBootLoader.Right(m_FWInfo.strBootLoader.GetLength() - nPos - 1);
	}
	nPos = m_FWInfo.strSystem.ReverseFind('\\');
	if (nPos != -1)
	{
		m_strSystemFile = m_FWInfo.strSystem.Right(m_FWInfo.strSystem.GetLength() - nPos - 1);
	}	
}

bool CARM_OP_EIGEN::DL_EraseNV(const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 30;//��ʱ5s
	int iTemp = 0;
	int nPos = 0;

	try
	{
		//��ʾ
		this->Show_Status(_T("erase flash NV......"));
		this->Save_Trace_Log(_T("erase flash NV......"));

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_EraseNV, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		// FlashToolCLI.exe --port="COM39" flasherase 0x350000  0x5400
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --skipconnect 1 --cfgfile %s\\EFlashTool\\%d\\config.ini --port=\"COM%d\" flasherase 0x350000  0x54000 \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe,m_csCurr_Exe_Path, m_iClass_Idx, m_iCom_Port);

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//�����ӽ���
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_EraseNV, Create process fail");
			throw(0);
		}
		this->Save_Trace_Log(wcpCmd);

		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//��ʼ��ʱ
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//��������
				strInfo.Format(_T("%s"), wcpData);
				// 				strInfo.Replace(_T('\r'), _T(' '));
				// 				strInfo.Replace(_T('\n'), _T(' '));
				this->Show_Out(strInfo);
				this->Save_Trace_Log(strInfo);
				//���ӳ٣��򿴲�����ʾ��Ϣ�ı仯
			}
			//����Ƿ��д��󱨳�
			if(wcsstr(strInfo, _T("[Error Info]")) != NULL)
			{
				nPos = strInfo.Find(_T("[Error Info]"));
				strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
				strData.Replace('[', ' ');//ȥ��������ʾ���ַ�
				strData.Replace(']', ' ');//ȥ��������ʾ���ַ�
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if(wcsstr(strInfo, _T("Burn Failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_EraseNV fail!"));
				throw(3);
			}


			//����Ƿ�pass
			if(wcsstr(strInfo, _T("Erase success")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//�˳�while
			}

			//��ʱ���
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				m_csError_Code = _T("FAIL, DL_EraseNV Time out!");
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
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
	if(hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if(hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}


	return true;
}

bool CARM_OP_EIGEN::DL_Reset(const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 30;//��ʱ5s
	int iTemp = 0;
	int nPos = 0;

	try
	{
		//��ʾ
		this->Show_Status(_T("DL_Reset......"));
		this->Save_Trace_Log(_T("DL_Reset....."));

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_Reset, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		// FlashToolCLI.exe --port="COM39" flasherase 0x350000  0x5400
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --skipconnect 1 --cfgfile %s\\EFlashTool\\%d\\config.ini  --port=\"COM%d\" sysreset \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe, m_csCurr_Exe_Path, m_iClass_Idx,m_iCom_Port);

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//�����ӽ���
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_Reset, Create process fail");
			throw(0);
		}
		this->Save_Trace_Log(wcpCmd);

		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//��ʼ��ʱ
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//��������
				strInfo.Format(_T("%s"), wcpData);
				// 				strInfo.Replace(_T('\r'), _T(' '));
				// 				strInfo.Replace(_T('\n'), _T(' '));
				this->Show_Out(strInfo);
				this->Save_Trace_Log(strInfo);
				//���ӳ٣��򿴲�����ʾ��Ϣ�ı仯
			}
			//����Ƿ��д��󱨳�
			if(wcsstr(strInfo, _T("[Error Info]")) != NULL)
			{
				nPos = strInfo.Find(_T("[Error Info]"));
				strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
				strData.Replace('[', ' ');//ȥ��������ʾ���ַ�
				strData.Replace(']', ' ');//ȥ��������ʾ���ַ�
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if(wcsstr(strInfo, _T("Burn Failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Reset fail!"));
				this->m_iError_Code = ERR_DL_RESET_FAIL;

				throw(3);
			}


			//����Ƿ�pass
			if(wcsstr(strInfo, _T("Erase success")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//�˳�while
			}

			//��ʱ���
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				m_csError_Code = _T("FAIL, DL_Reset Time out!");
				this->m_iError_Code = ERR_DL_RESET_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
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
	if(hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if(hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}


	return true;
}

bool CARM_OP_EIGEN::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
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
