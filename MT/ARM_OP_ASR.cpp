#include "StdAfx.h"
#include "ARM_OP_ASR.h"
#include "ts_lan.h"


#include "USBDeviceDetection.h"

CARM_OP_ASR::CARM_OP_ASR(void)
: m_iCom_Port(0)
,m_bRuning(false)
, m_iStop_Flag(-1)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_Ts_Num(0)
, m_iOP_TYPE(0)
, m_csCurr_Exe_Path(_T(""))
, mBar_IMEI(_T(""))
, mBar_SN(_T(""))
, mAT_FirewareName(_T(""))
{
	m_iError_Code =0;

	m_PassNumb=0;
	m_ErrorNumb=0;

	mAT_IMEI.Format(_T(""));

	m_iCom_Port = 0;
	m_iSec_Port = 0;

	m_iAT_Port = 0;

	m_strAT_FirewareName.Format(_T(""));

	m_bWaitTestEventStart = false;

	m_b120M_Flag = false;

	m_strOC.Format(_T(""));


}


CARM_OP_ASR::~CARM_OP_ASR(void)
{
}
void CARM_OP_ASR::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_ASR::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_ASR::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

int CARM_OP_ASR::Do_Execute()
{
	return RTN_SUCCESS;
}

int CARM_OP_ASR::Do_Execute_MCU(void)
{
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpPercent = NULL;

	time_t t_Start, t_End;
	int iTimeout = 40;//û�������³���ʱ�䳬ʱ
	unsigned long ulActual_Read;
	CString csTotal_Info, csData, csTemp, csEach;
	int iTemp;
	bool bSuccess_Flag = false;
	double dCurr_Percent = 0.0;
	double dNew_Percent = 0.0;


	try
	{
		//��ʾ
		this->Show_Status(_T("Download MCU Fw file ......"));
		this->Save_Trace_Log(_T("Download MCU Fw file ......"));
		this->Show_Out(_T("Download MCU Fw file ..."));


		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if (CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE)
		{
			this->m_csError_Code.Format(_T("FAIL, DL_FW, Create pipe fail!"));
			this->m_iError_Code = DL_FW_MCU_FAIL1;
			throw(1);
		}
		//------------------------------ 


		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength() + 1024));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);


		//------------------------------ ���dosָ�� ------------------------------
		swprintf(wcpCmd, _T("cmd.exe %cc %s\\QDE007\\%d\\JLink.exe  \"%s\\QDE007\\%d\\eraseAndFlash.jlink\""),
			'/', this->m_csCurr_Exe_Path, m_iClass_Idx, this->m_csCurr_Exe_Path, m_iClass_Idx);

		this->Save_Trace_Log(wcpCmd);


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
			this->m_csError_Code.Format(_T("FAIL, DL_FW, Create process fail!"));
			this->m_iError_Code = DL_FW_MCU_FAIL2;
			throw(2);
		}
		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if (hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 



		//------------------------------ ѭ����ȡprocess�Ĵ�����Ϣ ------------------------------ 
		csTotal_Info.Format(_T(""));
		//��ʼ��ʱ
		time(&t_Start);
		//
		while (bSuccess_Flag == false)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, NULL);
			//


			//����info������
			if (ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//��������
				csData.Format(_T("%s"), wcpData);

				this->Save_Trace_Log(csData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));

				csData.MakeLower();

				csTotal_Info += csData;

				//�Ƿ��С�Script file read successfully ��ʼ���ؼ���
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("script file read successfully"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.1;
					csTemp.Format(_T("script file ok"));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.1);

					csTemp.Format(_T("script file read successfully"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);

				}

				//�Ƿ��С�Connecting to J-Link via USB...O.K���ؼ���
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("connecting to j-link via usb...o.k"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.2;
					csTemp.Format(_T("connecting usb ok"));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.2);

					csTemp.Format(_T("connecting to j-link via usb...o.k"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);
				}
				//Erasing device
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("erasing device"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.3;
					csTemp.Format(_T("erasing..."));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.3);

					csTemp.Format(_T("erasing device"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);
				}
				//Erasing done
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("erasing done"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.5;
					csTemp.Format(_T("erasing ok"));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.5);

					csTemp.Format(_T("erasing done"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);
				}
				//Downloading file
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("downloading file"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.6;
					csTemp.Format(_T("downloading file..."));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.6);

					csTemp.Format(_T("downloading file"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);
				}

				//error...
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("error"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.9;
					csTemp.Format(_T("download error"));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.9);

					this->m_csError_Code.Format(_T("FAIL, DL_FW, error!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, error!"));
					this->m_iError_Code = DL_FW_MCU_FAIL4;
					throw(4);

					bSuccess_Flag = false;//fail
					break;//�˳�while
				}

				//fail...
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("fail"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.9;
					csTemp.Format(_T("download fail"));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.9);

					this->m_csError_Code.Format(_T("FAIL, DL_FW, fail!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, fail!"));
					this->m_iError_Code = DL_FW_MCU_FAIL4;
					throw(4);

					bSuccess_Flag = false;//fail
					break;//�˳�while
				}

				//Failed to open file
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("failed to open file"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.7;
					csTemp.Format(_T("failed to open file!"));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.7);

					this->m_csError_Code.Format(_T("FAIL, DL_FW, Failed to open file!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, Failed to open file!"));
					this->m_iError_Code = DL_FW_MCU_FAIL4;
					throw(4);

					bSuccess_Flag = false;//fail
					break;//�˳�while
				}


				//Total time needed    OK
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("total time needed"));
				//
				if ((wcpPercent != NULL) && (NULL != wcsstr(csTotal_Info.GetBuffer(), _T("ok"))))
				{
					dCurr_Percent = 0.8;
					csTemp.Format(_T("download file ok"));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.8);

					csTemp.Format(_T("total time needed"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);
				}

				//Script processing completed
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("script processing completed"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.9;
					csTemp.Format(_T("download ok"));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.9);

					csTemp.Format(_T("script processing completed"));

					bSuccess_Flag = true;//pass
					break;//�˳�while

				}
				//cannot...
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("cannot"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.9;
					csTemp.Format(_T("download fail"));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.9);

					this->m_csError_Code.Format(_T("FAIL, DL_FW, cannot link!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, cannot link!"));
					this->m_iError_Code = DL_FW_MCU_FAIL3;
					throw(4);

					bSuccess_Flag = false;//fail
					break;//�˳�while

				}
				//Unknown command line option
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("unknown"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.9;
					csTemp.Format(_T("download fail"));
					this->Show_Status(csTemp);
					//��ʾ������
					this->Show_Progress(0.9);

					this->m_csError_Code.Format(_T("FAIL, DL_FW, Unknown command line option!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, Unknown command line option!"));
					this->m_iError_Code = DL_FW_MCU_FAIL4;
					throw(4);

					bSuccess_Flag = false;//fail
					break;//�˳�while

				}
				//��ʱ���
				time(&t_End);
				//
				if ((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
				{
					this->m_csError_Code.Format(_T("FAIL, DL_FW, Time out!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, Time out!"));
					this->m_iError_Code = DL_FW_MCU_FAIL5;
					throw(4);
				}
			}

		}//while(bSuccess_Flag == false
		//------------------------------


		//�ͷž����ָ��
		if (hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	}
	catch (...)
	{
		//��ʾʧ����Ϣ
		this->Show_Status(this->m_csError_Code);


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

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		this->Show_Progress(1.0);
		//
		return RTN_FAIL;
	}

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

	//�ͷ��ڴ�
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	this->Show_Progress(1.0);
	//
	return RTN_SUCCESS;
}


void CARM_OP_ASR::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_ASR::Show_Progress(double d_Pgr_Percent)
{
	//�˺�����Ӧ�ð����������ٷֱȵļ��㣬Ȼ������Ϣ��Ӧ��������ɾ��岽�������ļ���
	//Progress�ܳ�����1000
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}

bool CARM_OP_ASR::FindHexFile(CString lpPath)
{
	CString strSubPath, strNewPath, cs_Name;

	//WIN32_FIND_DATA�ṹ������һ����FindFirstFile, 
	//FindFirstFileEx, ��FindNextFile�������ҵ����ļ���Ϣ
	WIN32_FIND_DATA FindFileData;

	//ͨ��FindFirstFile()�������ݵ�ǰ���ļ����·��
	//���Ҹ��ļ����Ѵ������ļ���������Զ�ȡ��WIN32_FIND_DATA�ṹ��ȥ
	strNewPath.Format(_T("%s*.*"), lpPath);
	HANDLE hFind = ::FindFirstFile(strNewPath, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return false;  //���hFind�ľ��ֵ��Ч������0
	}

	while (TRUE)
	{
		//��dwFileAttributes��FILE_ATTRIBUTE_DIRECTORY��λ"��"�������ж����ҵ�����Ŀ�ǲ����ļ��У�
		//��γ����Ŀ���ǲ����ļ��������ļ����е�����
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				strSubPath.Format(_T("%s%s\\"), lpPath, FindFileData.cFileName);
				if (NULL == wcsstr(strSubPath, _T("DBG")))
				{
					FindHexFile(strSubPath);
				}

				
			}
		}
		else
		{
			//EC600SCNAAR01A03M16_QDM512_V01.002.hex
			cs_Name.Format(_T("%s"), FindFileData.cFileName);

			if ((NULL != wcsstr(cs_Name,_T("_QDM512_")))
				&& (NULL != wcsstr(cs_Name, _T(".hex"))))
			{
				m_str_MCU_FW.cs_Name.Format(_T("%s"), cs_Name);

				m_str_MCU_FW.cs_Path.Format(_T("%s%s"), lpPath,cs_Name);

				m_str_MCU_FW.b_Enable_DL = true;

				break;
			}
		}
		if (!FindNextFile(hFind, &FindFileData))
			break;//���û���ҵ���һ���ļ�����������ѭ��
	}
	FindClose(hFind);

	return true;
}

bool CARM_OP_ASR::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	//�ж��� NB FW ·�����ж��ٿ���FW�ļ�
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir, cs_ASR_FW_Dir;
	CFileFind cla_Finder; //�ļ�������
	BOOL b_Found;
	int j=0;
	wchar_t * wcprFW_Name[] = {_T("zip")};

	bool b_MCU_Find = false;

	try
	{
		//--------------------------------------- ��ʼ������Ϣ������Intel�����ļ� --------------------------------------- 
		//���Intel�������ļ��б�
		this->m_str_ZIP_FW.cs_Name.Format(_T(""));
		this->m_str_ZIP_FW.cs_Path.Format(_T(""));
		this->m_str_ZIP_FW.b_Enable_DL = false;

		this->m_str_MCU_FW.cs_Name.Format(_T(""));
		this->m_str_MCU_FW.cs_Path.Format(_T(""));
		this->m_str_MCU_FW.b_Enable_DL = false;

		//��ȡ���·��
		if(this->m_cla_GFC.Get_File_Directory(wcp_FW_File_Path, &cs_FW_Dir) == false)
		{
			this->Show_Out(_T("ERROR, Get FW_File_Path fail"));
			this->m_csError_Code.Format(_T("ERROR, Get FW_File_Path fail!"));
			throw(0);
		}

		//m_stroc 20210510
		//QDM512CNES - N06 - AAAAA
		if (_T("QDM512CNES-N06-AAAAA") == this->m_strOC)
		{
			//\Fir\ASR\EC600SCNAAR01A03V04M16_factory\EC600SCNAAR01A03M16_QDM512_V01.002.hex
			cs_ASR_FW_Dir.Format(_T("%s\\ASR\\"), cs_FW_Dir);

			b_MCU_Find = this->FindHexFile(cs_ASR_FW_Dir);

			if (false == this->m_str_MCU_FW.b_Enable_DL)
			{
				this->Show_Out(_T("ERROR, Find MCU File fail"));
				this->m_csError_Code.Format(_T("ERROR, Find MCU File fail!"));
				throw(0);
			}
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
				CString strFW_File_Path = wcp_FW_File_Path;
				strFW_File_Path.MakeUpper();


				//�ж��ļ����ͣ�����.zip�ļ�
				if(wcsstr(strFW_File_Path,cs_FW_Name) != NULL)
				{
					//ȥ���ļ�������ʾ
					cs_FW_Name.Replace(_T(".zip"), _T("\0"));

					m_str_ZIP_FW.cs_Name.Format(_T("%s"), cs_FW_Name);

					m_str_ZIP_FW.cs_Path.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

					m_str_ZIP_FW.b_Enable_DL = true;
				}//if(wcsstr(cs_FW_Name, _T(".fwpkg")) != NULL
			}
		}//while(b_Found

		//
		cla_Finder.Close();
		//

		if(m_str_ZIP_FW.b_Enable_DL == false)
		{
			this->m_csError_Code.Format(_T("ERROR, LOAD_FW_FILE, There is no %s.file!"), wcprFW_Name[j]);
			throw(0);
		}

		//MCU������Jlink�ű��ļ�
		//m_stroc 20210510
		//QDM512CNES - N06 - AAAAA
		if (_T("QDM512CNES-N06-AAAAA") == this->m_strOC)
		{
			if (CreatQDM512_Jlink_CMD() == false)
			{
				this->Show_Out(_T("ERROR, CreatQDM512_Jlink_CMD fail"));
				this->m_csError_Code.Format(_T("ERROR, CreatQDM512_Jlink_CMD Fail"));
				throw(0);
			}
		}

	}
	catch(...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}

	return true;
}

bool CARM_OP_ASR::CreatQDM512_Jlink_CMD(void)
{
	//
	//device GD32E230x8	//ѡ��mcu�ͺ�
	//	usb 0
	//	si 1				//ѡ��swd�ӿ�
	//	speed 2000			//ѡ���ٶ�
	//	erase				//����
	//	sleep 400
	//	loadfile "EC600SCNAAR01A03M16_QDM512_V01.002.hex"	0x08000000//ѡ����Ҫ���ص��ļ���������FW��¼��Ŀ��оƬ�ĵ�ַ
	//	sleep 100
	//	r
	//	g
	//	q

	CStdioFile Cmd_File;
	CString   strCmd_file_name, csData;
	unsigned long ul_File_Size;

	strCmd_file_name.Format(_T("%s\\QDE007\\%d\\eraseAndFlash.jlink"), this->m_csCurr_Exe_Path, m_iClass_Idx);

	if (this->Get_File_Size(strCmd_file_name, &ul_File_Size) == true)
	{
		::SetFileAttributes(strCmd_file_name, FILE_ATTRIBUTE_NORMAL);

		::DeleteFile(strCmd_file_name);

		Sleep(2000);
	}

	if (Cmd_File.Open(strCmd_file_name, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::typeText) != NULL)
	{
		Cmd_File.SeekToEnd();

		csData.Format(_T("device GD32E230x8\n"));
		Cmd_File.WriteString(csData);

		csData.Format(_T("usb 0\n"));
		Cmd_File.WriteString(csData);

		csData.Format(_T("si 1\n"));
		Cmd_File.WriteString(csData);

		csData.Format(_T("speed 2000\n"));
		Cmd_File.WriteString(csData);

		csData.Format(_T("erase\n"));
		Cmd_File.WriteString(csData);

		csData.Format(_T("sleep 400\n"));
		Cmd_File.WriteString(csData);

		csData.Format(_T("loadfile \"%s\" 0x08000000\n"), m_str_ZIP_FW.cs_Path);
		Cmd_File.WriteString(csData);

		csData.Format(_T("sleep 100\n"));
		Cmd_File.WriteString(csData);

		csData.Format(_T("r\n"));
		Cmd_File.WriteString(csData);

		csData.Format(_T("g\n"));
		Cmd_File.WriteString(csData);

		csData.Format(_T("q\n"));
		Cmd_File.WriteString(csData);


		Cmd_File.Close();

		this->Show_Out(_T("CMD File Save OK"));
		return true;
	}
	else
	{
		this->m_csError_Code.Format(_T("CMD File(%s) Open Fail!"), strCmd_file_name);
		this->Show_Out(this->m_csError_Code);
		return false;

	}

}

bool CARM_OP_ASR::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
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

bool CARM_OP_ASR::Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut)
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
		this->Show_Out(csA);
		Save_Trace_Log(csA);

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
					this->Show_Out(_T("Get Port Pass"));
					Save_Trace_Log(_T("Get Port Pass"));
					return true;
				}

			}

			//ϵͳ��ͣ
			if(this->m_iStop_Flag > 0)
			{
				this->m_csError_Code.Format(_T("Operation has been stopped!"));
				throw(0);
			}

			//����Ƿ�ʱ
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port %d timeout!"), i_Tgt_Com_Port);
				//this->m_iError_Code  = Confirm_Dynamic_Com_Port_fail1;
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
		this->Show_Status(m_csError_Code);
		this->Show_Out(m_csError_Code);
		Save_Trace_Log(m_csError_Code);
		return false;
	}

	return false;
}

bool CARM_OP_ASR::IsAT_Port(int iPort)
{

	CUSBDeviceDetection usbDeviceDetect;
	CStringArray strAT_PortList;
	CString strAT_Port;

	CString strS_Port;
	strS_Port.Format(_T("COM%d"),iPort);

	UINT uDeviceCount =0;

	usbDeviceDetect.GetUsbAT_PortList(strAT_PortList, uDeviceCount);

	int i =0;
	for(i =0;i<strAT_PortList.GetSize();i++)
	{
		strAT_Port= strAT_PortList.GetAt(i);
		strAT_Port.MakeUpper();
		if (strAT_Port.Find(strS_Port)>=0)
		{
			return true;
		}
		else
		{
			continue;
		}
	}
	return false;
}

//AT+QDOWNLOAD=1
bool CARM_OP_ASR::SendQD_Mode(int iPort)
{
	CString strData;
	//------------------------------ ��鴮���Ƿ���� ------------------------------ 
	if(this->m_cla_GFC.Confirm_Reg_Com_Port(iPort) == false)
	{
		this->m_csError_Code.Format(_T("FAIL, Confirm com port %d fail!"), iPort);
		this->Show_Status(this->m_csError_Code);
		this->Show_Out(this->m_csError_Code);
		return false;
	}

	this->m_cla_Com_Port.m_iPort = iPort;
	this->m_cla_Com_Port.m_iBaudRate = 115200;
	this->m_cla_Com_Port.m_dwDTR_CTRL = DTR_CONTROL_DISABLE;
	this->m_cla_Com_Port.m_dwRTS_CTRL = RTS_CONTROL_DISABLE;


	if(this->m_cla_Com_Port.Port_Open(800))
	{
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();
		//����
		this->m_cla_Com_Port.Write( _T("AT+QDOWNLOAD=1") );

		this->m_cla_Com_Port.Port_Close();

		strData.Format(_T("Send AT+QDOWNLOAD=1-Port%d"),iPort);

		this->Show_Status(strData);
		this->Show_Out(strData);
	}
	else
	{
		//....
		this->m_csError_Code.Format(_T("FAIL, Port_Open %d fail!"), iPort);
		this->Show_Status(this->m_csError_Code);
		this->Show_Out(this->m_csError_Code);

		//AfxMessageBox(_T("Port_Open Fail"));
		return false;
	}
	return true;
}

void  CARM_OP_ASR::AT_SendQD_Mode(int iPort)
{
	bool bAT_PortFlag = false;

	bAT_PortFlag = this->IsAT_Port(iPort);

	if (true ==  bAT_PortFlag)
	{
		this->SendQD_Mode(iPort);
	}
}

int CARM_OP_ASR::AT_DO_OP(void)
{
	CString strData;
	//------------------------------ ��鴮���Ƿ���� ------------------------------ 
	this->Show_Status(_T("Wait reboot..."));
	Sleep(20000);

	if (this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iAT_Port) == false)
	{

		if (this->Confirm_Dynamic_Com_Port(this->m_iAT_Port, 30) == false)
		{
			this->m_iError_Code = FAIL_FIND_PORT;
			this->m_csError_Code.Format(_T("FAIL, Confirm com port %d fail!"), this->m_iAT_Port);
			this->Show_Status(this->m_csError_Code);
			this->Show_Out(this->m_csError_Code);
			return RTN_FAIL;
		}
		else
		{
			Sleep(3000);
		}
	}
	else
	{
		Sleep(3000);
	}

	this->m_cla_Com_Port.m_iPort = this->m_iAT_Port;
	this->m_cla_Com_Port.m_iBaudRate = 115200;
	this->m_cla_Com_Port.m_dwDTR_CTRL = DTR_CONTROL_DISABLE;
	this->m_cla_Com_Port.m_dwRTS_CTRL = RTS_CONTROL_DISABLE;


	if (false == this->m_cla_Com_Port.Port_Open(800))
	{
		//....
		this->m_iError_Code = FAIL_OPEN_PORT;
		this->m_csError_Code.Format(_T("FAIL, Port_Open %d fail!"), this->m_iAT_Port);
		this->Show_Status(this->m_csError_Code);
		this->Show_Out(this->m_csError_Code);

		return RTN_FAIL;
	}

	if (0 == this->FindVer())
	{
		this->m_iError_Code = FAIL_FIND_VERSION;
		this->m_csError_Code = _T("FAIL, Find Version Fail!");

		this->Show_Status(_T("FAIL, Find Version Fail!"));
		this->Show_Out(this->m_csError_Code);

		this->m_cla_Com_Port.Port_Close();
		return RTN_FAIL;
	}
	else
	{
		this->Show_Status(_T("Pass, Find Version Pass!"));
		this->Show_Out(_T("Pass, Find Version Pass!"));
		this->Show_Out(m_strAT_FirewareName);

		
	}
	//20210122
	if (true == this->m_b120M_Flag)
	{
		//����ģʽ
		if (false  == this->Set_Prod_Mode(1))
		{
			this->m_iError_Code = FAIL_SET_PROD_MODE;
			this->m_csError_Code = _T("FAIL, Set_Prod_Mode Fail!");
			this->Show_Status(this->m_csError_Code);
			this->Show_Out(this->m_csError_Code);

			this->m_cla_Com_Port.Port_Close();
			return RTN_FAIL;
		}
		else
		{
			this->Show_Out(_T("Pass, Set_Prod_Mode Pass!"));
		}
		//����120M
		//AT + QSAWCFG = 120
		if (false == this->Set_Saw_Band(120))
		{
			this->m_iError_Code = FAIL_SET_SAW_BAND;
			this->m_csError_Code = _T("FAIL, Set_Saw_Band Fail!");
			this->Show_Status(this->m_csError_Code);
			this->Show_Out(this->m_csError_Code);

			this->m_cla_Com_Port.Port_Close();
			return RTN_FAIL;
		}
		else
		{
			this->Show_Out(_T("Pass, Set_Saw_Band Pass!"));
		}

		//�˳�����ģʽ
	}


	this->m_cla_Com_Port.Port_Close();

	return RTN_SUCCESS;
}


//��ѯ�Ƿ���Բ�ѯ���汾��
int CARM_OP_ASR::FindVer()
{
	// TODO: Add your control notification handler code here
	CString strATCom, csData;
	CString csCOM, csTemp;
	CString info;

	time_t start, finish;
	CString csAT_Cmd, strAT_FirewareName;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;
	int iError = 0;

	//
	try
	{
		//��� COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- ���FW------------------------- 
		iTimeOut = 15;
		csAT_Cmd.Format(_T("ATI"));

		//
		time(&start);
		//
		do
		{
			//����
			this->m_cla_Com_Port.Write(csAT_Cmd);
			//
			memset(wcrData, 0, sizeof(wcrData));
			Sleep(200);
			//��ȡ
			this->m_cla_Com_Port.Read(wcrData, &iRec_Bits, 100);//
			//
			if (wcsstr(wcrData, _T("OK")))
			{
				//��ȡ��������
				if (this->m_cla_GFC.GET_AT_RSP_STRING(wcrData, _T("OK"), -1, &m_strAT_FirewareName) == true)
				{
					m_strAT_FirewareName.MakeUpper();
					if (wcsstr(m_strAT_FirewareName, _T("REVISION")))
					{
						m_strAT_FirewareName.Remove(_T(' '));
						m_strAT_FirewareName.Replace(_T("REVISION:"), _T(""));
						break;//pass, �˳�while
					}
				}

			}
			//
			time(&finish);
			//
			if ((int)difftime(finish, start) >= iTimeOut)
			{
				throw(0);
			}
		} while (true);
		//-------------------------
	}
	catch (...)
	{
		//fail
		return 0;
	}
	return 1;

}
bool CARM_OP_ASR::Read_IMEI(wchar_t * wcp_IMEI)
{
	CString csAT_Cmd;
	wchar_t * wcpData = NULL;
	wchar_t * wcpHead = NULL;
	int iRec_Bits;

	csAT_Cmd.Format(_T("AT+EGMR=0,7"));

	//�����ڴ�
	wcpData = (wchar_t *)calloc(128, sizeof(wchar_t));

	for (int i = 0; i < 3; i++)
	{
		Sleep(1000);
		memset(wcpData, 0, 128);
		this->m_cla_Com_Port.Query(csAT_Cmd, wcpData, &iRec_Bits, 100);

		wcpHead = wcsstr(wcpData, _T("+EGMR:"));

		if ((wcsstr(wcpData, _T("OK")) != NULL) && (wcpHead != NULL))
		{
			wcpHead += 6;

			wmemset(wcp_IMEI, 0, sizeof(wcp_IMEI));
			int j = 0;
			bool bFirst = false;//IMEIͷ��β������""
			bool bSecond = false;

			while ((*wcpHead != '\0') && (*wcpHead != '\r') && (*wcpHead != '\n'))
			{
				if (*wcpHead == '"')
				{
					if (bFirst)
					{
						bSecond = true; //�ҵ��ڶ�����,������ȡIMEI����
					}
					else
					{
						bFirst = true; //�ҵ���һ����,��ʼ��ȡIMEI����
						wcpHead++;  //�ƿ���һ����,��һ�βſ�ʼ��ȡIMEI����
						continue;  //��һ�βſ�ʼ��ȡIMEI����
					}
				}

				if (bFirst)    //��ʼ��ȡIMEI����
				{
					//���յ�����
					if (bSecond)
					{
						*(wcp_IMEI + j) = '\0';

						//�ͷ��ڴ�
						if (wcpData != NULL)
						{
							free(wcpData);
						}
						return true;
					}
					else
					{
						*(wcp_IMEI + j) = *wcpHead;
						j++;
					}
				}

				wcpHead++;
			}
		}
	}

	//�ͷ��ڴ�
	if (wcpData != NULL)
	{
		free(wcpData);
	}

	return false;

}










#if 0
bool CARM_OP_ASR::Read_Reg_USB_VID_PID_AT_Port_Of_QUD(int * ip_QDLoader_Port_List, int * ip_Port_Quan)
{
	//��ע����ȡ��AT���б�
	/*
	//
	USB\VID_05C6&PID_9008&REV_0000
	USB\VID_05C6&PID_9008
	Qualcomm HS-USB QDLoader 9008 (COM11)
	*/

	wchar_t wcrUSB_PID_VID[64];//DM��
	CString csrUsb_Dev_List[64], csFriendly_Name;
	int iUsb_Dev_Quan, i, iCnt;
	wchar_t wcrData[128];
	wchar_t * wcpPoint = NULL;

	g_cla_Lock_QDLoader_Port.Lock();
	try
	{
		//��ͬƽ̨USB�������ؼ��ʲ�ͬ
		switch(this->m_eu_BB_Chip)
		{
		case MSM8909:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//8909��QDLoader��
			break;
		case MSM8937:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//8937��QDLoader��
			break;
		case MSM8953:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//8953��QDLoader��
			break;				
		case MSM8917:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//8917��QDLoader��
			break;
		case MDM9X07:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//9X07��QDLoader�� ??
			break;		
		case MDM9X40:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//9X07��QDLoader�� ??
			break;	
		case SDX20:
		case SDX55:
		case AG215S:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//SDX20/SDX24/SDX55��QDLoader�� ??
			break;
		case MDM9X06:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//9X06��QDLoader��
			break;
		case SDM660:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//SDM660��QDLoader��
			break;
		case MDM9X55:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//MDM9X55��QDLoader��
			break;
		case MDM9X05:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//MDM9X05��QDLoader��
			break;

		case SDM845:
			swprintf_s(wcrUSB_PID_VID, _T("VID_05C6&PID_9008"));//SDM845��QDLoader��
			break;
		default:
			throw(0);
		}

		//
		*ip_Port_Quan = 0;
		iUsb_Dev_Quan = 0;
		iCnt = 0;

		//��PC�Ͽ��ܶ��QDLoader�����豸�б�
		this->m_cla_GFC.Get_Reg_Usb_Dev_List(wcrUSB_PID_VID, csrUsb_Dev_List, &iUsb_Dev_Quan);

		//
		for(i=0; i<iUsb_Dev_Quan; i++)
		{
			csFriendly_Name.Format(_T(""));
			
			//��ȡ�豸�ġ�FriendlyName�������磺Qualcomm HS-USB QDLoader 9008 (COM11)��
			if(this->m_cla_GFC.Get_Reg_Usb_Dev_Friendly_Name(wcrUSB_PID_VID, csrUsb_Dev_List[i], &csFriendly_Name) == false)
			{
				if((MSM8953 == this->m_eu_BB_Chip)
					||(MSM8909 == this->m_eu_BB_Chip)
					||(MSM8917 == this->m_eu_BB_Chip)
					||(MDM9X40 == this->m_eu_BB_Chip)
					||(SDX20 == this->m_eu_BB_Chip)
					||(SDX55 == this->m_eu_BB_Chip)
					||(MDM9X07 == this->m_eu_BB_Chip)
					||(MDM9X06 == this->m_eu_BB_Chip)
					||(MDM9X55 == this->m_eu_BB_Chip)
					||(MDM9X05 == this->m_eu_BB_Chip)
					||(SDM660 == this->m_eu_BB_Chip)
					||(SDM845 == this->m_eu_BB_Chip)
					||(AG215S == this->m_eu_BB_Chip)
					)
				{
					//����������������û��FriendlyName
					continue;
				}
				else
				{
					::AfxMessageBox(_T("ERROR, Read_Reg_QDLoader_Of_QUD, Fail!"));
					throw(0);
				}
			}


			//��ֵת������ȡCOM�ں��룩
			csFriendly_Name.MakeUpper();
			csFriendly_Name.Replace(')', '\0');

			swprintf_s(wcrData, _T("%s"), csFriendly_Name);

			wcpPoint = wcsstr(wcrData, _T("(COM"));
			if (NULL != wcpPoint)
			{
				wcpPoint += 4;
				*(ip_QDLoader_Port_List + iCnt) = _wtoi(wcpPoint);
			}
			else
			{
				::AfxMessageBox(_T("COM NULL, Fail!"));
				throw(0);
			}
			


			//�ۼ�
			iCnt++;
		}//for(i=0; i<iUsb_Dev_Quan; i++

		//
		*ip_Port_Quan = iCnt;
	}
	catch(...)
	{
		g_cla_Lock_QDLoader_Port.Unlock();
		return false;
	}
	g_cla_Lock_QDLoader_Port.Unlock();
	return true;
}
#endif

void CARM_OP_ASR::FindAT_Switch_DL_Mode(void)
{
	CString strData;
	time_t t_Start, t_End;

	//------------------------------ ��鴮���Ƿ���� ------------------------------
	this->m_cla_Com_Port.m_iPort = this->m_iAT_Port;
	this->m_cla_Com_Port.m_iBaudRate = 115200;
	this->m_cla_Com_Port.m_dwDTR_CTRL = DTR_CONTROL_DISABLE;
	this->m_cla_Com_Port.m_dwRTS_CTRL = RTS_CONTROL_DISABLE;

	time(&t_Start);
	while (1)
	{
		//����ܵļ��ʱ���ѵ�Ҳ�������ҿڣ�ֱ���˳�
		if (false == this->m_bWaitTestEventStart)
		{
			this->Show_Out(_T("WaitTestEventStart = true break"));
			break;
		}
		time(&t_End);
		if ((int)difftime(t_End, t_Start) >= 40)
		{
			this->Show_Out(_T("Out Time break"));
			break;
		}

		//��AT��
		if (this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iAT_Port) == false)
		{			
			Sleep(3000);
			continue;			
		}
		else
		{
			Sleep(3000);
			if (this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iAT_Port) == false)
			{
				continue;
			}
			else
			{
				strData.Format(_T("Find AT Port(%d) Pass"), this->m_iAT_Port);
				this->Show_Out(strData);
				Sleep(3000);
			}
		}

		if (false == this->m_cla_Com_Port.Port_Open(800))
		{
			//....
			Sleep(3000);
			continue;
		}

		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();
		//����
		this->m_cla_Com_Port.Write(_T("AT+QDOWNLOAD=1"));

		Sleep(2000);

		this->m_cla_Com_Port.Port_Close();

		strData.Format(_T("Send AT+QDOWNLOAD=1-Port%d"), this->m_iAT_Port);

		this->Show_Out(strData);
	}

}

bool CARM_OP_ASR::Set_Prod_Mode(const int iMode)
{

	//��ȡ��ǰģʽ
	//[2019-02-25_15:44:36:360]at*prod? 
	//[2019-02-25_15:44:36:360]*PROD: 0
	//[2019-02-25_15:44:36:360]OK

	//����Ƿ�֧��
	//[2019-02-25_15:44:44:868]at*prod=? 
	//[2019-02-25_15:44:44:868]*PROD=(0,1)
	//[2019-02-25_15:44:44:868]OK

	//���ù���ģʽ
	//[2019-02-25_15:44:49:281]at*prod=1 
	//[2019-02-25_15:44:49:281]OK

	//�����û�ģʽ
	//[2019-02-25_15:44:52:032]at*prod=0 
	//[2019-02-25_15:44:52:032]OK



	CString csAT_Cmd;
	wchar_t * wcpData = NULL;
	int iRec_Bits, i;


	//
	try
	{
		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 128);


		//ָ��
		csAT_Cmd.Format(_T("at*prod=%d"), iMode);


		//
		for (i = 0; i < 2; i++)
		{
			//����
			this->m_cla_Com_Port.Write(csAT_Cmd);
			//
			Sleep(500);
			//
			memset(wcpData, 0, 128);
			//��ȡ
			this->m_cla_Com_Port.Read(wcpData, &iRec_Bits, 120);

			//�˶Թؼ���
			if (wcsstr(wcpData, _T("OK")) != NULL)
			{
				break;
			}
			if (wcsstr(wcpData, _T("ERROR")) != NULL)
			{
				throw(0);
			}
		}//for(i=0; i<2; i++
		//
		if (i >= 2)
		{
			throw(0);
		}


		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	}
	catch (...)
	{
		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		//fail
		return false;
	}

	//pass
	return true;
}

bool CARM_OP_ASR::Set_Saw_Band(const int Band)
{
	CString csAT_Cmd;
	wchar_t * wcpData = NULL;
	int iRec_Bits, i;

	//
	try
	{
		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 128);
		//ָ��
		csAT_Cmd.Format(_T("AT+QSAWCFG=%d"), Band);

		//
		for (i = 0; i < 2; i++)
		{
			//����
			this->m_cla_Com_Port.Write(csAT_Cmd);
			//
			Sleep(1000);
			//
			memset(wcpData, 0, 128);
			//��ȡ
			this->m_cla_Com_Port.Read(wcpData, &iRec_Bits, 120);

			//�˶Թؼ���
			if (wcsstr(wcpData, _T("OK")) != NULL)
			{
				break;
			}
			if (wcsstr(wcpData, _T("ERROR")) != NULL)
			{
				throw(0);
			}
		}//for(i=0; i<2; i++
		//
		if (i >= 2)
		{
			throw(0);
		}


		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	}
	catch (...)
	{
		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		//fail
		return false;
	}

	//pass
	return true;
}
