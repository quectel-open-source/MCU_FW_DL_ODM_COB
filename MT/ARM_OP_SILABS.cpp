#include "StdAfx.h"
#include "ARM_OP_SILABS.h"
#include "ts_lan.h"


#include "USBDeviceDetection.h"


CARM_OP_SILABS::CARM_OP_SILABS(void)
: m_iCom_Port(0)
,m_bRuning(false)
, m_iStop_Flag(-1)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_Ts_Num(0)
, m_iOP_TYPE(0)
, m_csCurr_Exe_Path(_T(""))

, mAT_FirewareName(_T(""))
{
	m_iError_Code =0;

	m_PassNumb=0;
	m_ErrorNumb=0;

	mAT_IMEI.Format(_T(""));

	m_iCom_Port = 0;
	m_iSec_Port = 0;

	m_iAT_Port = 0;

}

CARM_OP_SILABS::~CARM_OP_SILABS(void)
{
}
void CARM_OP_SILABS::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_SILABS::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_SILABS::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

int CARM_OP_SILABS::Do_Execute()
{
	return RTN_SUCCESS;
}
int CARM_OP_SILABS::Do_Execute_MCU(void)
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
	wchar_t * wcpPercent1 = NULL;


	time_t t_Start, t_End;
	int iTimeout = 400;//û�������³���ʱ�䳬ʱ  //40
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
		swprintf(wcpCmd, _T("cmd.exe %cc %s\\Jlink\\%d\\JLink.exe  \"%s\\Jlink\\%d\\eraseAndFlash.jlink\""),
			'/', this->m_csCurr_Exe_Path, m_iClass_Idx, this->m_csCurr_Exe_Path, m_iClass_Idx);

		this->Save_Trace_Log(wcpCmd);
		this->Show_Out(wcpCmd);


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

				wcpPercent1 = wcsstr(csTotal_Info.GetBuffer(), _T("o.k."));
				//
				if ((wcpPercent != NULL) && (wcpPercent1 != NULL))
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
void CARM_OP_SILABS::Show_Progress(double d_Pgr_Percent)
{
	//�˺�����Ӧ�ð����������ٷֱȵļ��㣬Ȼ������Ϣ��Ӧ��������ɾ��岽�������ļ���
	//Progress�ܳ�����100
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 100.0), (LPARAM)this->m_iClass_Idx);
}


void CARM_OP_SILABS::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

bool CARM_OP_SILABS::FindHexFile(CString lpPath)
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
			//rail_empty.hex
			cs_Name.Format(_T("%s"), FindFileData.cFileName);
			//
			if ((NULL != wcsstr(cs_Name,_T("_rail_")))  
				&& (NULL != wcsstr(cs_Name, _T(".hex"))))
			{
				m_FWInfo.cs_Name.Format(_T("%s"), cs_Name);

				m_FWInfo.cs_Path.Format(_T("%s%s"), lpPath, cs_Name);

				m_FWInfo.b_Enable_DL = true;

				break;
			}
		}
		if (!FindNextFile(hFind, &FindFileData))
			break;//���û���ҵ���һ���ļ�����������ѭ��
	}
	FindClose(hFind);

	return true;
}
bool CARM_OP_SILABS::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir;
	CString strData;
	CFileFind cla_Finder; //�ļ�������
	BOOL b_Found;

	try
	{
		//--------------------------------------- ��ʼ������Ϣ������ZG100S�����ļ� --------------------------------------- 
		//���ZG100S�������ļ��б�
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

				// �ж���hex�ļ�
				if (wcsstr(cs_FW_Name, _T(".HEX")) != NULL)
				{
					//�ж�RAIL�ļ�  rail_empty.hex ���ݿͻ����������ؼ���
					//if (wcsstr(cs_FW_Name, _T("RAIL")) != NULL)//20220422
					{
						m_FWInfo.strHexFile.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

						m_FWInfo.b_Enable_DL = true;
					}
				}

			}
		}
		cla_Finder.Close();

		if (m_FWInfo.strHexFile.IsEmpty())
		{
			strData = _T(".hex file");
			m_FWInfo.b_Enable_DL = false;
		}

		if (m_FWInfo.b_Enable_DL == false)
		{
			this->m_csError_Code.Format(_T("error,load_fw_file,There is no %s.file!"), strData);
			throw(0);
		}
#if 1

		CStdioFile Cmd_File;
		CString   strCmd_file_name, csData;
		unsigned long ul_File_Size;

		strCmd_file_name.Format(_T("%s\\Jlink\\%d\\eraseAndFlash.jlink"), this->m_csCurr_Exe_Path, m_iClass_Idx);

		if (this->Get_File_Size(strCmd_file_name, &ul_File_Size) == true)
		{
			::SetFileAttributes(strCmd_file_name, FILE_ATTRIBUTE_NORMAL);

			::DeleteFile(strCmd_file_name);

			Sleep(2000);
		}

		if (Cmd_File.Open(strCmd_file_name, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::typeText) != NULL)
		{


			Cmd_File.SeekToEnd();

			csData.Format(_T("device EFR32BG21BXXXF1024\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("usb 0\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("si 1\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("speed 9600\n"));
			Cmd_File.WriteString(csData);

			//csData.Format(_T("erase\n"));
			//Cmd_File.WriteString(csData);

			//csData.Format(_T("sleep 400\n"));
			//Cmd_File.WriteString(csData);


			//csData.Format(_T("erase\n"));
			//Cmd_File.WriteString(csData);

			csData.Format(_T("sleep 400\n"));
			Cmd_File.WriteString(csData);


			csData.Format(_T("loadfile \"%s\"\n"), this->m_FWInfo.strHexFile);
			Cmd_File.WriteString(csData);
			this->Show_Out(this->m_FWInfo.strHexFile);
			//-----------------------------------------------
			csData.Format(_T("sleep 400\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("loadfile \"%s\"\n"), this->m_FWInfo.strHexFile);
			Cmd_File.WriteString(csData);
			this->Show_Out(this->m_FWInfo.strHexFile);

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

		}
		else
		{
			this->m_csError_Code.Format(_T("CMD File(%s) Open Fail!"), strCmd_file_name);
			this->Show_Out(this->m_csError_Code);
			//throw(1);

		}

#endif


	}
	catch (...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}

	return true;
}
bool CARM_OP_SILABS::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
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
//bool CARM_OP_SILABS::Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut)
//{
//	time_t t_Start, t_End;
//	CString csA, csB;
//	int j = 0;
//
//	try
//	{
//		csB.Format(_T(" ."));
//
//		time(&t_Start);
//		csA.Format(_T("Wait port(%d) is normal..."), i_Tgt_Com_Port);
//		this->Show_Status(csA);
//		this->Show_Out(csA);
//		Save_Trace_Log(csA);
//
//		do
//		{
//			//����regedit�е�COM�б�
//			if (this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == true)
//			{
//				//�ҵ�������
//				//�ȴ�5���������һ��
//				Sleep(5000);
//				if (this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == false)
//				{
//					//����					
//				}
//				else
//				{
//					this->Show_Status(_T("Get Port Pass"));
//					this->Show_Out(_T("Get Port Pass"));
//					Save_Trace_Log(_T("Get Port Pass"));
//					return true;
//				}
//
//			}
//
//			//ϵͳ��ͣ
//			if (this->m_iStop_Flag > 0)
//			{
//				this->m_csError_Code.Format(_T("Operation has been stopped!"));
//				throw(0);
//			}
//
//			//����Ƿ�ʱ
//			time(&t_End);
//			if ((int)difftime(t_End, t_Start) >= i_TimeOut)
//			{
//				this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port %d timeout!"), i_Tgt_Com_Port);
//				//this->m_iError_Code  = Confirm_Dynamic_Com_Port_fail1;
//				throw(0);
//			}
//
//			//���ȼ�����ʾ
//			if ((j % 10) == 0)
//			{
//				csA.Format(_T("Detect dynamic com port %d"), i_Tgt_Com_Port);
//			}
//
//			csA += csB;
//			this->Show_Status(csA);
//			j++;
//
//			//ÿ�ζ�ȡע�����ӳ�
//			Sleep(800);
//		} while (true);
//	}
//	catch (...)
//	{
//		this->Show_Status(m_csError_Code);
//		this->Show_Out(m_csError_Code);
//		Save_Trace_Log(m_csError_Code);
//		return false;
//	}
//
//	return false;
//}
