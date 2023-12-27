#include "StdAfx.h"
#include "ARM_OP_BEKEN.h"
#include "ts_lan.h"



CARM_OP_BEKEN::CARM_OP_BEKEN(void)
	: m_iCom_Port(0)
	, m_bRuning(false)
	, m_iStop_Flag(-1)
	, m_csError_Code(_T(""))
	, m_iClass_Idx(0)
	, m_csCurr_Exe_Path(_T(""))
	, m_iResetFlag(0)
	, m_iProdu_Type(0)
{
	m_iOP_Robot = 0;
	m_Ts_Num = 0;
	m_iTimeInterval = 0;
}

void CARM_OP_BEKEN::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_BEKEN::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_BEKEN::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_BEKEN::Show_Progress(double d_Pgr_Percent)
{
	//�˺�����Ӧ�ð����������ٷֱȵļ��㣬Ȼ������Ϣ��Ӧ��������ɾ��岽�������ļ���
	//Progress�ܳ�����100
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 100.0), (LPARAM)this->m_iClass_Idx);
}


void CARM_OP_BEKEN::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

bool CARM_OP_BEKEN::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir;
	CString strData;
	CFileFind cla_Finder; //�ļ�������
	BOOL b_Found;

	try
	{
		//--------------------------------------- ��ʼ������Ϣ������BEKEN�����ļ� --------------------------------------- 
		//���EBKEN�������ļ��б�
		this->m_FWInfo.Clear();

		//��ȡ���·��
		if (this->m_cla_GFC.Get_File_Directory(wcp_FW_File_Path, &cs_FW_Dir) == false)
		{
			this->m_csError_Code.Format(_T("ERROR, Get fls file directory fail!"));
			throw(0);
		}

		//Ѱ���ļ���ʼ
		cs_Finder_Dir.Format(_T("%s\\*"), cs_FW_Dir);

		b_Found = cla_Finder.FindFile(cs_Finder_Dir);

		while (b_Found)
		{
			b_Found = cla_Finder.FindNextFile();

			if (cla_Finder.IsDots())
			{
				continue;//�ҵ����ǵ�ǰĿ¼���ϼ�Ŀ¼������
			}

			if (!cla_Finder.IsDirectory())	//��������ļ��У������ļ�
			{
				cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//��ȡ�ҵ����ļ���

				cs_FW_Name.MakeUpper();

				// �ж���bin�ļ�
				if (wcsstr(cs_FW_Name, _T(".BIN")) != NULL)
				{
					//�ж�FC41DAAR01A01.BIN�ļ�
					if (wcsstr(cs_FW_Name, _T("FC41D")) != NULL)
					{
						m_FWInfo.strBootLoader.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

						m_FWInfo.b_Enable_DL = true;
					}
				

				}

			}
		}//while(b_Found

		//
		cla_Finder.Close();

		if (m_FWInfo.strBootLoader.IsEmpty())
		{
			strData = _T("bootloader bin file");
			m_FWInfo.b_Enable_DL = false;
		}

		if (m_FWInfo.b_Enable_DL == false)
		{
			this->m_csError_Code.Format(_T("error,load_fw_file,There is no %s.file!"), strData);
			throw(0);
		}

	}
	catch (...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}

	return true;
}
int CARM_OP_BEKEN::Do_Execute(void)
{
	CString strData,strInfo;
	try
	{
		m_bRuning = true;

		this->m_csError_Code.Format(_T(""));
		this->m_iError_Code = 0;

	
		strData.Format(_T("bk_writer%d.exe"), m_iClass_Idx);
		m_cla_GFC.Kill_Process(strData);

		//20210521
		strInfo.Format(_T("Port(%d) find..."), this->m_iCom_Port);
		this->Show_Out(strInfo);
		if (this->Confirm_Dynamic_Com_Port(this->m_iCom_Port, 35) == false)//
		{
			strInfo.Format(_T("Port(%d) find Fail"), this->m_iCom_Port);
			this->Show_Out(strInfo);
			this->m_csError_Code = strInfo;
			this->m_iError_Code = ERR_DL_FIND_PORT;
			throw(1);
		}

		if (this->DL(strData) == false)
		{
			throw(1);
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

bool CARM_OP_BEKEN::Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut)
{
	time_t t_Start, t_End;
	CString csA, csB;
	int j = 0;

	try
	{
		csB.Format(_T(" ."));

		time(&t_Start);
		csA.Format(_T("Wait port(%d) is normal..."), i_Tgt_Com_Port);
		this->Show_Status(csA);
		this->Show_Out(csA);
		Save_Trace_Log(csA);

		do
		{
			//����regedit�е�COM�б�
			if (this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == true)
			{
				//�ҵ�������
				//�ȴ�5���������һ��
				Sleep(2000);
				if (this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == false)
				{
					//����					
				}
				else
				{
					this->Show_Status(_T("Get Port Pass"));
					this->Show_Out(_T("Get Port Pass"));
					Save_Trace_Log(_T("Get Port Pass"));
					return true;
				}

			}

			//ϵͳ��ͣ
			if (this->m_iStop_Flag > 0)
			{
				this->m_csError_Code.Format(_T("Operation has been stopped!"));
				throw(0);
			}

			//����Ƿ�ʱ
			time(&t_End);
			if ((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port %d timeout!"), i_Tgt_Com_Port);
				throw(0);
			}

			//���ȼ�����ʾ
			if ((j % 10) == 0)
			{
				csA.Format(_T("Detect dynamic com port %d"), i_Tgt_Com_Port);
			}

			csA += csB;
			this->Show_Status(csA);
			j++;

			//ÿ�ζ�ȡע�����ӳ�
			Sleep(800);
		} while (true);
	}
	catch (...)
	{
		this->Show_Status(m_csError_Code);
		this->Show_Out(m_csError_Code);
		Save_Trace_Log(m_csError_Code);
		return false;
	}

	return false;
}
//�Զ��߳̿ص�Դ�ϵ�
void CARM_OP_BEKEN::Set_PowerOn_SPC(int i_Index)
{
	this->Open_Power_SPC(i_Index);
	//::SendMessage(this->m_hWnd_View, WM_PowerOn_SPC, (WPARAM)i_Index, (LPARAM)i_Index);
}
//ÿƬģ������ǰ�������ÿ��ͨ���ĳ̿ص�Դ�ϵ�
void  CARM_OP_BEKEN::Open_Power_SPC(int iIndex)
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
bool CARM_OP_BEKEN::DL(const CString& strCmdExe)
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
	int iTimeout = 120;//��ʱ5s
	int iTemp = 0;
	int nPos = 0;
	double dCurr_Percent = 0.0;
	double dNew_Percent = 0.0;

	try
	{
		//��ʾ
		this->Show_Status(_T("DL Start......"));
		this->Save_Trace_Log(_T("DL Start ......"));

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if (CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL Start, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 4095);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 4095);
		//////////////////////////////////////////////////////////////////////////
		//bk_writer.20210322.exe -tBK7231N -pCOM223@115200 -fF:\06-Factory_Tools\FC41D\exe\FC41DAAR01A01.bin@0 -e2 -R
		swprintf_s(wcpCmd, nLen, _T("cmd.exe  %cc  \"%s\\Beken\\%d\\%s -tBK7231N -pCOM%d@%d  -f%s@0 -e2 -R\""), '/',
			m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe, m_iCom_Port, m_iBaud_Rate, m_FWInfo.strBootLoader);
		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//�����ӽ���
		if (CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL Start, Create process fail");
			throw(0);
		}

		this->Save_Trace_Log(wcpCmd);

		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if (hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//��ʼ��ʱ
		time(&t_Start);
		this->Show_Status(_T("Erasing Flash ..."));
		while (bSuccess_Flag == FALSE)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 4095);
			ulActual_Read = 0;
			ReadFile(hRead_Pipe, cpRead_Buff, 2048, &ulActual_Read, NULL);

			//����info������
			if (ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 4095);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);
				this->Show_Out(wcpData);

				//��������
				strInfo.Format(_T("%s"), wcpData);
				//��ȡ��Ҫ�ġ����Ȱٷֱ���Ϣ��
				//��ȡ��һ��ֵ(0)  Writing Flash
				wcpEach = wcstok(wcpData, _T("\r\n"));
			
				while (wcpEach != NULL)
				{
					wcpPercent = wcsstr(wcpEach, _T("Writing Flash"));

					if (wcpPercent != NULL)
					{
						//���ݵ�������
						strEach.Format(_T("%s"), wcpPercent);

						//��ȡ�ٷֱ�����
					    wcpPercent += strlen("Erasing Flash ... ");

						//�Ƿ��С�percent �������ؼ���
						if (wcsstr(wcpPercent, _T("%")) != NULL)
						{
							//���ְٷֱ����ݵġ���β��
							strData.Format(_T("%s"), wcpPercent);
							strData.Replace('%', '\0');//ȥ����β
							//�ַ�ת����
							dNew_Percent = _wtof(strData);

							//ˢ�°ٷֱ�����
							if (dNew_Percent > dCurr_Percent)
							{
								//��ֵ�°ٷֱ�����
								dCurr_Percent = dNew_Percent;

								//��ʾ��������Ϣ���ݡ�
//								this->Show_Out(strEach);
								this->Show_Status(strEach);
								//���ӳ٣��򿴲�����ʾ��Ϣ�ı仯
								Sleep(20);
								//��ʾ������
								this->Show_Progress((1.0 * (dCurr_Percent / 100.0)));
							}
						}
					}
					wcpEach = wcstok(NULL, _T("\r\n"));
				}
			}

			this->Save_Trace_Log(strInfo);


			//��������
			if ((dCurr_Percent > 90.0)
				|| (dCurr_Percent < 1.0))//
			{
				csTotal_Info += strInfo;
			}
			else
			{
				csTotal_Info.Format(_T("%s"), strInfo);
			}

			//����Ƿ��д��󱨳�
			if (wcsstr(strInfo, _T("Writing Flash Failed")) != NULL)
			{
				nPos = strInfo.Find(_T("Writing Flash Failed"));
				strData = strInfo.Right(strInfo.GetLength() - nPos);
				strData.Replace('-', ' ');//ȥ��������ʾ���ַ�
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if (wcsstr(strInfo, _T("Failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Probe fail!"));
				this->m_iError_Code = ERR_DL_PROBE_TIME_FAIL;
				throw(0);
			}


			//����Ƿ�pass  //Writing Flash OK
			if (wcsstr(strInfo, _T("Writing Flash OK")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//�˳�while
			}

			//��ʱ���
			time(&t_End);
			//
			if ((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
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

	this->Show_Progress(1.0);
	return true;
}
bool CARM_OP_BEKEN::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
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

		if (hFile_Handle == INVALID_HANDLE_VALUE)
		{
			throw(0);
		}

		//��ȡ�ļ���BYTE��
		*ulp_Size = ::GetFileSize(hFile_Handle, NULL);

		//�ر�
		::CloseHandle(hFile_Handle);
		hFile_Handle = INVALID_HANDLE_VALUE;
	}
	catch (...)
	{
		return false;
	}

	return true;
}

CARM_OP_BEKEN::~CARM_OP_BEKEN()
{
}
