#include "StdAfx.h"
#include "ARM_OP_INTEL.h"


//���������ã�ɨ��USB��ʱÿ��ֻ����һ��Intel������������
CCriticalSection g_cla_Lock_Scan;//ͬһ��Դ�����ã������ڲ�ͬ�ĺ����ڣ���Ҫʹ��ͬһCCriticalSection����

CARM_OP_INTEL::CARM_OP_INTEL(void)
: m_bRuning(false)
, m_iStop_Flag(-1)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_csUSB_Detect_PID_VID(_T(""))
, m_iOP_TYPE(0)
, m_bGet_Debug_Log(false)
, m_iUsb_Dev_Quan(0)
, m_iCTRL_DL_TYPE(0)
, m_csCurr_Exe_Path(_T(""))
, m_bPLC_Flag(false)
, m_iPLC_Port(0)
, m_iError_Code(0)
{
	m_csUSB_Detect_PID_VID.Format(_T("VID_058B&PID_0041"));//Intel_6255оƬ��ע���USB��
}

CARM_OP_INTEL::~CARM_OP_INTEL(void)
{
}

bool CARM_OP_INTEL::Scan_Active_Usb_Ch(int * ip_Act_USB_CH, CString * csp_Act_USB_DEV, int i_TimeOut)
{
	//ɨ��ע������Ƿ���USB�ڱ��������������

	//��������
	::g_cla_Lock_Scan.Lock();

	//
	time_t t_Start, t_End;
	int irUsb_Ch_List_1[64], irUsb_Ch_List_2[64], i, iTemp;
	CString csData;
	bool bFlag = false;

	try
	{
		//����һ������
		//1. ������USB�豸��channel��
		if(this->Read_Reg_Usb_Dev_Ch(this->m_csUSB_Detect_PID_VID, this->m_csrUsb_Dev_List, this->m_iUsb_Dev_Quan, irUsb_Ch_List_1) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Scan_Active_Usb_Ch, Get 1 usb(%s) ch fail!"), this->m_csUSB_Detect_PID_VID);
			throw(0);//��Ŀ��USB�豸channelʧ��
		}

		//��ʼ��ʱ
		time(&t_Start);

		do
		{
			//����Ƿ�ʱ
			time(&t_End);
			iTemp = (int)difftime(t_End, t_Start);
			if(iTemp >= i_TimeOut)
			{
				this->m_csError_Code.Format(_T("FAIL, Scan_Active_Usb_Ch, Time out fail!"));
				throw(0);
			}


			//2. ������USB�豸��channel��
			if(this->Read_Reg_Usb_Dev_Ch(this->m_csUSB_Detect_PID_VID, this->m_csrUsb_Dev_List, this->m_iUsb_Dev_Quan, irUsb_Ch_List_2) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Scan_Active_Usb_Ch, Get 2 usb(%s) ch fail!"), this->m_csUSB_Detect_PID_VID);
				throw(0);//��Ŀ��USB�豸channelʧ��
			}


			//��ʾ
			for(i=0; i<this->m_iUsb_Dev_Quan; i++)
			{
				csData.Format(_T("Scan active usb dev( %s ), ch( %d ), %dS"), this->m_csrUsb_Dev_List[i], irUsb_Ch_List_2[i], iTemp);
				this->Show_Status(csData);

				if((irUsb_Ch_List_1[i] != irUsb_Ch_List_2[i]) && (irUsb_Ch_List_2[i] > 0))
				{
					//���ּ����USB channelö��
					*ip_Act_USB_CH = irUsb_Ch_List_2[i];
					csp_Act_USB_DEV->Format(_T("%s"), this->m_csrUsb_Dev_List[i]);

					csData.Format(_T("Scan active usb dev( %s ), ch( %d => %d )"), *csp_Act_USB_DEV, irUsb_Ch_List_1[i], *ip_Act_USB_CH);
					this->Show_Status(csData);

					bFlag = true;
					break;//�˳�for(j=0
				}
				else
				{
					irUsb_Ch_List_1[i] = irUsb_Ch_List_2[i];
				}
			}//for(i=0; i<his->m_iUsb_Dev_Quan; i++


			//
			if(bFlag == true)
			{
				break;//�˳�while
			}
		}
		while(true);
	}
	catch(...)
	{
		this->Show_Status(this->m_csError_Code);

		//��������
		::g_cla_Lock_Scan.Unlock();

		return false;
	}

	//��������
	::g_cla_Lock_Scan.Unlock();

	return true;
}

void CARM_OP_INTEL::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

int CARM_OP_INTEL::Do_Execute(void)
{
	try
	{
		//��ʼ��ʾ
		this->Show_Status(_T("Waiting......"));

		//��ʼ���������е���ز���
		this->m_iStop_Flag = -1;
		this->m_bRuning = true;
		this->m_csError_Code.Format(_T(""));

	
		//------------------------------ �������ڵ�USB Device ------------------------------ 
		if(this->Scan_Usb_Dev() == false)
		{
			this->Show_Status(this->m_csError_Code);
			this->m_iError_Code = INTEL_SCAN_USB_DEV_FAIL;

			throw(1);
		}

		//------------------------------ ����.FLS�ļ�------------------------------ 
		if(this->DL_FLS_FILE_IN_EXE() == false)
		{
			this->Show_Status(this->m_csError_Code);

			throw(2);
		}

		//
		this->Show_Status(_T("FW upgrade success."));
	}
	catch(...)
	{
		//
		this->m_iStop_Flag = -1;
		this->m_bRuning = false;

		return RTN_FAIL;
	}

	//
	this->m_iStop_Flag = -1;
	this->m_bRuning = false;

	return RTN_SUCCESS;
}

bool CARM_OP_INTEL::Get_FLS_File_Index(CString cs_FLS_Name, int * ip_Index)
{
	if(wcscmp(cs_FLS_Name, _T("PSI")) == 0)
	{
		*ip_Index = PSI_FLS_FILE_I;
	}
	else if(wcscmp(cs_FLS_Name, _T("SLB")) == 0)
	{
		*ip_Index = SLB_FLS_FILE_I;
	}
	else if(wcsstr(cs_FLS_Name, _T("_FW")) != NULL)
	{
		*ip_Index = FW_FLS_FILE_I;
	}
	else if(wcsstr(cs_FLS_Name, _T("MSY")) != NULL)
	{
		*ip_Index = MSY_FLS_FILE_I;
	}
	else if((wcsstr(cs_FLS_Name, _T("MOD_")) != NULL) && (wcsstr(cs_FLS_Name, _T("MSY")) == NULL))
	{
		*ip_Index = MOD_FLS_FILE_I;
	}
	else if((wcsstr(cs_FLS_Name, _T("SAM_")) != NULL) && (wcsstr(cs_FLS_Name, _T("MSY")) == NULL))
	{
		*ip_Index = MOD_FLS_FILE_I;
	}
	else
	{
		this->m_csError_Code.Format(_T("FAIL, Get %s.fls file index error!"), cs_FLS_Name);
		return false;
	}

	return true;
}

bool CARM_OP_INTEL::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	//�ж��� Intel FW ·�����ж��ٿ���FW�ļ�
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir;
	CFileFind cla_Finder; //�ļ�������
	BOOL b_Found;
	int j;
	wchar_t * wcprFW_Name[] = {_T("PSI"), _T("SLB"), _T("FW"), _T("MOD"), _T("MSY")};

	try
	{
		//--------------------------------------- ��ʼ������Ϣ������Intel�����ļ� --------------------------------------- 
		//���Intel�������ļ��б�
		for(j=0; j<MAX_FLS_FILE; j++)
		{
			this->m_strr_FLS_File_List[j].str_File_Base.cs_Name.Format(_T(""));
			this->m_strr_FLS_File_List[j].str_File_Base.cs_Path.Format(_T(""));
			this->m_strr_FLS_File_List[j].b_Enable_DL = false;
		}

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

				//�ж��ļ����ͣ�����.fls�ļ�
				if(wcsstr(cs_FW_Name, _T(".FLS")) != NULL)
				{
					//ȥ���ļ�������ʾ
					cs_FW_Name.Replace(_T(".FLS"), _T("\0"));


					//�������ص���ͨ.fls�ļ�
					if(this->Get_FLS_File_Index(cs_FW_Name, &j) == false)
					{
						throw(0);
					}

					this->m_strr_FLS_File_List[j].str_File_Base.cs_Name.Format(_T("%s"), cs_FW_Name);

					this->m_strr_FLS_File_List[j].str_File_Base.cs_Path.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

					this->m_strr_FLS_File_List[j].b_Enable_DL = true;
				}//if(wcsstr(cs_FW_Name, _T(".FLS")) != NULL
			}
		}//while(b_Found

		//
		cla_Finder.Close();


		//
		for(j=0; j<MAX_FLS_FILE; j++)
		{
			if(this->m_strr_FLS_File_List[j].b_Enable_DL == false)
			{
				this->m_csError_Code.Format(_T("ERROR, LOAD_FW_FILE, There is no %s.file!"), wcprFW_Name[j]);
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

bool CARM_OP_INTEL::DL_FLS_FILE_IN_EXE()
{
	//ʹ��Intel��DownloadTool.exe�ļ��Ľӿں�������.fls�ļ�

	int iActive_USB_CH;
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	bool bBreak_Flag = false;
	unsigned long ulActual_Read;
	int iTemp;
	CString csTotal_Info, csData, csUSB_Dev;
	CString csCode_Flag;//����code���ı�ʶ
	double dPercent = 0.0;
	time_t t_Start, t_End;
	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;
	wchar_t wcrUsed_Data[64];

	bool bBoot_Flag = false;
	wchar_t * wcpFlag_Booting = NULL;
	wchar_t * wcpFlag_PSI = NULL;
	wchar_t * wcpFlag_SLB = NULL;
	wchar_t * wcpFlag_Code = NULL;
	wchar_t * wcpFlag_MSY = NULL;
	wchar_t * wcpFlag_DSP = NULL;

	bool bDownloading_Flag = false;
	wchar_t * wcpFlag_DL = NULL;
	double dAdd_Percent, dNew_Percent;//���ظ�������ռ�еĽ������ݶ�
	CString csPSI, csSLB, csCODE, csMSY, csDSP, csTemp;

	try
	{
		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, DL_FLS_FILE_IN_EXE, Create pipe fail!"));
			throw(0);
		}
		//------------------------------ 


		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 4096);


		//------------------------------ �趨�����USB�� ------------------------------ 
		switch(this->m_iCTRL_DL_TYPE)
		{
		case INTEL_DL_USB_SCAN://ɨ���Ƿ��м����USB��
			{
				if(this->Scan_Active_Usb_Ch(&iActive_USB_CH, &csUSB_Dev, 15) == false)
				{
					throw(1);
				}
			}
			break;
		case INTEL_DL_USB_MANUAL:
			{
				iActive_USB_CH = this->m_iClass_Idx;
			}
			break;
		default:
			throw(0);
		}
		

		//------------------------------ ���dosָ�� ------------------------------ 
		//��ͼ�ϵ�4��ֱ��Ӧ--erase-mode 0, 1, 2, 3��
		//Intel�Լ���log��ò�Ҫ��
		//DownloadTool.exe �Cc USBx --erase-mode 1 slb.fls psi.fls MOD_6255.fls AENEAS_FW.fls
		//sprintf(command,"Cmd.exe /C downloadtool\\downloadtool.exe -c USB1 -l downloadtool\\trace_%s.log --boot-timeout 8000 --erase-mode 1 %s",time_string, fls_filename.GetBuffer());
		/*
		cd \   ���˻ص���Ŀ¼��
		&      ��ִ������ߵ�����ٽ���ִ�����������
		cls    ������
		cmd /k ��ִ��������󣬱������ڣ����˳�
		cmd /c ��ִ������������ʱ�����ڹر�
		*/

		swprintf(wcpData, _T("cmd.exe %cc %s\\INT\\CH%d\\DownloadTool.exe -c USB%d --boot-timeout 10000 --erase-mode %d \"%s\" \"%s\" \"%s\" \"%s\" \"%s\""),
			'/', this->m_csCurr_Exe_Path, this->m_iClass_Idx, iActive_USB_CH, this->m_iOP_TYPE,
			this->m_strr_FLS_File_List[PSI_FLS_FILE_I].str_File_Base.cs_Path,
			this->m_strr_FLS_File_List[SLB_FLS_FILE_I].str_File_Base.cs_Path,
			this->m_strr_FLS_File_List[FW_FLS_FILE_I].str_File_Base.cs_Path,
			this->m_strr_FLS_File_List[MOD_FLS_FILE_I].str_File_Base.cs_Path,
			this->m_strr_FLS_File_List[MSY_FLS_FILE_I].str_File_Base.cs_Path);

		//this->Save_Trace_Log(wcpData); // 20171115 test


		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//
		if(CreateProcess(NULL, wcpData, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, DL_FLS_FILE_IN_EXE, Create process fail!"));

			this->m_iError_Code = INTEL_CREATE_PROCESS_FAIL;

			throw(2);
		}
		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 


		//------------------------------ ѭ����ȡprocess�Ĵ�����Ϣ ------------------------------ 
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 4096);
		csTotal_Info.Format(_T(""));

		//���ļ�·������Ϊ�����ؽ׶Ρ����ж���׼��Ϊ׼ȷ
		csPSI.Format(_T("%s"), this->m_strr_FLS_File_List[PSI_FLS_FILE_I].str_File_Base.cs_Path);
		csSLB.Format(_T("%s"), this->m_strr_FLS_File_List[SLB_FLS_FILE_I].str_File_Base.cs_Path);
		csCODE.Format(_T("%s"), this->m_strr_FLS_File_List[MOD_FLS_FILE_I].str_File_Base.cs_Path);
		csMSY.Format(_T("%s"), this->m_strr_FLS_File_List[MSY_FLS_FILE_I].str_File_Base.cs_Path);
		csDSP.Format(_T("%s"), this->m_strr_FLS_File_List[FW_FLS_FILE_I].str_File_Base.cs_Path);
		csPSI.MakeLower();
		csSLB.MakeLower();
		csCODE.MakeLower();
		csMSY.MakeLower();
		csDSP.MakeLower();

		//��ʼ��ʱ
		time(&t_Start);

		while(bBreak_Flag == false) 
		{
			//��info����.exe�����ѽ�����������read�Ļ�����������һ����
			memset(cpRead_Buff, 0, 4096);
			ulActual_Read = 0;
			//
			if(ReadFile(hRead_Pipe, cpRead_Buff, 4095, &ulActual_Read, NULL) == NULL)
			{
				bBreak_Flag = true;
			}


			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 4096);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);
				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));

				this->Show_Status(csData);

				if(this->m_bGet_Debug_Log == true)
				{
					this->Save_Trace_Log(csData);
				}

				//��������
				csTotal_Info += csData;
				csTotal_Info.MakeLower();

				//20210407
				this->Show_Progress(0.2);

#if 0
				//
				if(bBoot_Flag == true)//boot�ɹ�����ʼ�����ļ�
				{
					if(bDownloading_Flag == true)//��������ĳ���ļ�
					{
						//ѭ����ʾ��ǰ�ļ��Ľ���
						swprintf(wcpData, _T("%s"), csTotal_Info);
						do
						{
							wcpHead = wcsstr(wcpData, _T("progress:"));

							if(wcpHead != NULL)
							{
								wcpHead += 9;

								wcpEnd = wcsstr(wcpHead, _T("%"));

								if(wcpEnd != NULL)
								{
									iTemp = wcpEnd - wcpHead;

									csTemp.Format(_T("%s"), wcpHead);

									if ((iTemp > 0) && (iTemp< csTemp.GetLength()))
									{
										
										csTemp = csTemp.Left(iTemp);
										csTemp.Replace('%', '\0');//ȥ����β

										//�ַ�ת����
										dNew_Percent = _wtof(csTemp);
										this->Show_Progress((dPercent + (dAdd_Percent * (dNew_Percent / 100.0))));

										//memset(wcrUsed_Data, 0, sizeof(wcrUsed_Data));
										//wmemcpy(wcrUsed_Data, wcpHead, iTemp);

										//this->Show_Progress((dPercent + (dAdd_Percent * (_wtof(wcrUsed_Data) / 100.0))));
									}

									//ȥ���Ѿ���ʾ�Ľ��Ȱٷֱ�
									csTotal_Info.Format(_T("%s"), (wcpEnd + 1));
									//
									swprintf(wcpData, _T("%s"), csTotal_Info);

									//if(wcsstr(wcrUsed_Data, _T("100")) != NULL)
									if (wcsstr(wcpHead, _T("100")) != NULL)
									{
										dPercent += dAdd_Percent;
										bDownloading_Flag = false;//��ǰ�ļ����ؽ���
										break;//�˳�while
									}
								}//if(wcpEnd != NULL
								else
								{
									break;//�˳�while
								}
							}//if(wcpHead != NULL
							else
							{
								break;//�˳�while
							}
						}
						while(true);
					}
					else//�ȴ���ʼ������ĳ���ļ�
					{
						swprintf(wcpData, _T("%s"), csTotal_Info);

						wcpFlag_PSI = wcsstr(wcpData, csPSI);
						wcpFlag_SLB = wcsstr(wcpData, csSLB);
						wcpFlag_Code = wcsstr(wcpData, csCODE);
						wcpFlag_MSY = wcsstr(wcpData, csMSY);
						wcpFlag_DSP = wcsstr(wcpData, csDSP);

						//����С����Чָ��
						wcpFlag_DL = NULL;

						if(wcpFlag_PSI != NULL)
						{
							if((wcpFlag_DL == NULL) || ((wcpFlag_DL != NULL) && (wcpFlag_PSI < wcpFlag_DL)))
							{
								wcpFlag_DL = wcpFlag_PSI;
								dAdd_Percent = 0.1;
							}
						}
						if(wcpFlag_SLB != NULL)
						{
							if((wcpFlag_DL == NULL) || ((wcpFlag_DL != NULL) && (wcpFlag_SLB < wcpFlag_DL)))
							{
								wcpFlag_DL = wcpFlag_SLB;
								dAdd_Percent = 0.1;
							}
						}
						if(wcpFlag_Code != NULL)//SAM��MOD�ļ�
						{
							if((wcpFlag_DL == NULL) || ((wcpFlag_DL != NULL) && (wcpFlag_Code < wcpFlag_DL)))
							{
								wcpFlag_DL = wcpFlag_Code;
								dAdd_Percent = 0.5;
							}
						}
						if(wcpFlag_MSY != NULL)//MSY�ļ�
						{
							if((wcpFlag_DL == NULL) || ((wcpFlag_DL != NULL) && (wcpFlag_MSY < wcpFlag_DL)))
							{
								wcpFlag_DL = wcpFlag_MSY;
								dAdd_Percent = 0.1;
							}
						}
						if(wcpFlag_DSP != NULL)//AENEAS_FW.fls
						{
							if((wcpFlag_DL == NULL) || ((wcpFlag_DL != NULL) && (wcpFlag_DSP < wcpFlag_DL)))
							{
								wcpFlag_DL = wcpFlag_DSP;
								dAdd_Percent = 0.1;
							}
						}
						


						if(wcpFlag_DL != NULL)//�ҵ���һ���������ص��ļ�
						{
							//��λ������ʾ�������ļ����ȡ�����ʼ�ؼ��ʣ���Ϊ��֮ǰ���ܻ��С�Erasing�Ľ��ȡ�
							wcpHead = wcsstr(wcpFlag_DL, _T("sending data. (usb raw mode)"));

							if(wcpHead == NULL)
							{
								continue;
							}
						

							//
							csTotal_Info.Format(_T("%s"), wcpHead);
							swprintf(wcpData, _T("%s"), csTotal_Info);


							//ѭ����ʾ��ǰ�ļ��Ľ���
							bDownloading_Flag = true;
							do
							{
								wcpHead = wcsstr(wcpData, _T("progress:"));

								if(wcpHead != NULL)
								{
									wcpHead += 9;

									wcpEnd = wcsstr(wcpHead, _T("%"));

									if(wcpEnd != NULL)
									{
										iTemp = wcpEnd - wcpHead;

										csTemp.Format(_T("%s"), wcpHead);

										if ((iTemp > 0) && (iTemp< csTemp.GetLength()))
										{
											//memset(wcrUsed_Data, 0, sizeof(wcrUsed_Data));
											//wmemcpy(wcrUsed_Data, wcpHead, iTemp);

											//this->Show_Progress((dPercent + (dAdd_Percent * (_wtof(wcrUsed_Data) / 100.0))));

											csTemp = csTemp.Left(iTemp);
											csTemp.Replace('%', '\0');//ȥ����β

											//�ַ�ת����
											dNew_Percent = _wtof(csTemp);
											this->Show_Progress((dPercent + (dAdd_Percent * (dNew_Percent / 100.0))));
										}

										//ȥ���Ѿ���ʾ�Ľ��Ȱٷֱ�
										csTotal_Info.Format(_T("%s"), (wcpEnd + 1));
										//
										swprintf(wcpData, _T("%s"), csTotal_Info);

										if(wcsstr(wcrUsed_Data, _T("100")) != NULL)
										{
											dPercent += dAdd_Percent;
											bDownloading_Flag = false;//��ǰ�ļ����ؽ���
											break;//�˳�while
										}
									}//if(wcpEnd != NULL
									else
									{
										break;//�˳�while
									}
								}//if(wcpHead != NULL
								else
								{
									break;//�˳�while
								}
							}
							while(true);
						}//if(wcpFlag_DLing != NULL
					}
				}
				else
				{
					//�ȴ�boot
					swprintf(wcpData, _T("%s"), csTotal_Info);
					wcpFlag_Booting = wcsstr(wcpData, _T("boot-loader is active"));

					if(wcpFlag_Booting != NULL)
					{
						//Boot�ɹ�����
						dPercent += 0.05;
						this->Show_Progress(dPercent);

						//
						bBoot_Flag = true;

						//ȥ����ʶ���info
						csTotal_Info.Format(_T("%s"), (wcpFlag_Booting + 21));

						//������ʼ���غ����¿�ʼ��ʱ
						time(&t_Start);
					}
				}//�ȴ�boot

#endif
			}//if(ulActual_Read > 0
			


			//����Ƿ�pass
			if((wcsstr(csTotal_Info, _T("success!")) != NULL) &&
				(wcsstr(csTotal_Info, _T("verify ok")) != NULL) && (dPercent >= 0.95))
			{
				this->Show_Progress(1.0);
				bBreak_Flag = true;//pass
				break;
			}


			//����Ƿ�����ˣ������ DownloadTool.exe ���п��ܲ����Զ��رա���ʧ��
			if(wcsstr(csTotal_Info, _T("erro")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_FLS_FILE_IN_EXE, Download error!"));

				this->m_iError_Code = INTEL_DOWNLOAD_ERROR_FAIL;

				throw(3);
			}


			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 35)//��ʱ����fail
			{
				this->m_csError_Code.Format(_T("FAIL, DL_FLS_FILE_IN_EXE, Timeout!"));

				this->m_iError_Code = INTEL_DOWNLOAD_TIMEOUT_FAIL;

				throw(4);
			}
		}//while(bBreak_Flag == false
		//------------------------------ 


		//
		Sleep(800);


		//
		if(this->m_iCTRL_DL_TYPE == INTEL_DL_USB_SCAN)
		{
			if(this->Set_Reg_Usb_Dev_Ch_Value(this->m_csUSB_Detect_PID_VID, csUSB_Dev, _T("USB0")) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_FLS_FILE_IN_EXE, Reset usb dev ch fail!"));

				this->m_iError_Code = INTEL_DOWNLOAD_RESET_USB_DEV_FAIL;

				throw(5);
			}
		}


		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
	}
	catch(...)
	{
		this->Show_Status(this->m_csError_Code);

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

		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	this->Show_Progress(1.0);

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	return true;
}

void CARM_OP_INTEL::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_INTEL::Show_Progress(double d_Pgr_Percent)
{
	//�˺�����Ӧ�ð����������ٷֱȵļ��㣬Ȼ������Ϣ��Ӧ��������ɾ��岽�������ļ���
	//Progress�ܳ�����1000
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}

bool CARM_OP_INTEL::Scan_Usb_Dev(void)
{
	//1. ����regedit�е�����usb�豸
	this->m_iUsb_Dev_Quan = 0;

	if(this->m_cla_GFC.Get_Reg_Usb_Dev_List(this->m_csUSB_Detect_PID_VID, this->m_csrUsb_Dev_List, &this->m_iUsb_Dev_Quan) == false)
	{
		this->m_csError_Code.Format(_T("FAIL, Scan_Usb_Dev, Get usb(%s) dev fail!"), this->m_csUSB_Detect_PID_VID);
		return false;
	}

	return true;
}

bool CARM_OP_INTEL::Read_Reg_Usb_Dev_Ch(const wchar_t * wcp_USB_PID_VID, CString * csp_USB_Dev_List, int i_USB_Dev_Quan, int * ip_USB_Ch_List)
{
	//��USB�豸��CreateFileName�����е�usb channel��

	HKEY hKey_Root = HKEY_LOCAL_MACHINE;
	CString csSub_Key;
	HKEY hKey_Handle;
	wchar_t * wcpValue_Data = NULL;
	DWORD dwValue_Size = 128;
	DWORD dwData_Size;
	DWORD dwData_Type = REG_SZ;
	wchar_t * wcpPoint;
	int i;

	try
	{
		if(i_USB_Dev_Quan <= 0)
		{
			throw(0);
		}

		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpValue_Data, dwValue_Size);

		//
		for(i=0; i<i_USB_Dev_Quan; i++)
		{
			//sub key
			csSub_Key.Format(_T("SYSTEM\\CurrentControlSet\\Enum\\USB\\%s\\%s\\Device Parameters"), wcp_USB_PID_VID, *(csp_USB_Dev_List + i));

			//open reg
			if(::RegOpenKeyEx(hKey_Root, csSub_Key, 0, KEY_READ, &hKey_Handle) != ERROR_SUCCESS) 
			{
				throw(1);
			}

			//
			dwData_Size = dwValue_Size;
			memset(wcpValue_Data, 0, dwValue_Size);

			//��ָ����Ŀ������
			if(::RegQueryValueEx(hKey_Handle, _T("CreateFileName"), (LPDWORD)NULL, &dwData_Type, (BYTE *)wcpValue_Data, &dwData_Size) != ERROR_SUCCESS) 
			{
				throw(2);
			}

			//close reg
			::RegCloseKey(hKey_Handle);

			//��ȡchannel��
			wcpPoint = wcsstr(wcpValue_Data, _T("USB"));

			if(wcpPoint != NULL)    
			{
				wcpPoint += 3;
				*(ip_USB_Ch_List + i) = _wtoi(wcpPoint);
			}
			else
			{
				*(ip_USB_Ch_List + i) = 0;
			}
		}//for(i=0; i<i_USB_Dev_Quan; i++
	}
	catch(...)
	{
		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpValue_Data);
		return false;
	}

	//�ͷ��ڴ�
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpValue_Data);
	return true;
}

bool CARM_OP_INTEL::Set_Reg_Usb_Dev_Ch_Value(const wchar_t * wcp_USB_PID_VID, const wchar_t * wcp_USB_Dev, const wchar_t * wcp_USB_Ch)
{
	//�趨USB�豸��CreateFileName������usb channel��

	HKEY hKey_Root = HKEY_LOCAL_MACHINE;
	CString csSub_Key;
	HKEY hKey_Handle = NULL;

	try
	{
		//sub key
		csSub_Key.Format(_T("SYSTEM\\CurrentControlSet\\Enum\\USB\\%s\\%s\\Device Parameters"), wcp_USB_PID_VID, wcp_USB_Dev);

		//open reg
		if(::RegOpenKeyEx(hKey_Root, csSub_Key, 0, (KEY_WRITE | KEY_READ), &hKey_Handle) != ERROR_SUCCESS) 
		{
			throw(1);
		}

		if(::RegSetValueEx(hKey_Handle, _T("CreateFileName"), NULL, REG_SZ, (BYTE *)wcp_USB_Ch, (wcslen(wcp_USB_Ch) * 2)) != ERROR_SUCCESS)
		{
			throw(2);
		}

		//close reg
		::RegCloseKey(hKey_Handle);
	}
	catch(...)
	{
		if(hKey_Handle != NULL)
		{
			::RegCloseKey(hKey_Handle);
		}

		return false;
	}

	return true;
}
void CARM_OP_INTEL::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}
//д�������
bool CARM_OP_INTEL::_W_C_IMEI_SN_Intel( const wchar_t * wcp_IMEI, const wchar_t * wcp_SN)
{
	CString  csData,csSN_Buff_In_Write;
	wchar_t wcrData[128];
	
	memset( wcrData, 0, sizeof( wcrData ) );

	this->m_cla_Com_Port.m_iPort= m_iAt_Port;
	this->m_cla_Com_Port.m_iBaudRate = 115200;

	try
	{
		//���˿�
		this->Show_Status(_T("д�ų�ʼ����......"));
		this->Show_Out(_T("д�ų�ʼ����......"));
		//
		Sleep(3000);

		//------------------------------------- �ϵ�󣬼�� COM�� �Ƿ��Ѿ�������� ------------------------------------- 
		if(this->Detect_Com_Port( this->m_cla_Com_Port.m_iPort, 20 ) == false)
		{
			csData.Format(_T("ERROR, Detect_Com_Port(%d) fail!"), this->m_cla_Com_Port.m_iPort);
			this->Show_Status(csData);
			this->Show_Out(csData);
			this->m_iError_Code = ERR_Detect_AT_PORT_Error;
			this->m_csError_Code = _T( "FAIL, Detect_AT_PORT Fail" );
			throw(0);
		}
		//-------------------------------------

		//------------------------------------- ��ʼ������ -------------------------------------
		if(this->m_cla_Com_Port.Port_Open( 400 ) == false)
		{
			csData.Format(_T("ERROR, Open COM(%d) fail!"), this->m_cla_Com_Port.m_iPort);
			this->Show_Status(csData);
			this->Show_Out(csData);
			this->m_iError_Code = ERR_OPEN_AT_PORT_Error;
			this->m_csError_Code = _T( "FAIL, OPEN_AT_PORT Fail" );
			//
			throw(0);
		}
		//-------------------------------------
		//----------------------------------------д ��IMEI �� ---------------------------------------- 
		this->Show_Status( _T( "Write IMEI" ) );
		this->Show_Out( _T( "Write IMEI" ) );

		//�رմ���
		this->m_cla_Com_Port.Port_Close();

		//��� IMEI ����
		if( wcslen(wcp_IMEI) != 15 )
		{
			//fail
			this->Show_Status( _T( "Check_IMEI_Data Error" ));
			this->Show_Out( _T( "Check_IMEI_Data Error" ));

			this->m_iError_Code = ERR_Write_IMEI_LEN_Error;
			this->m_csError_Code.Format( _T( "FAIL, Check_IMEI_Data" ) );
			//
			throw(0);
		}

		//дIMEI
		if(this->_W_Write_Pri_IMEI_Intel( wcp_IMEI ) == false)
		{
			this->Show_Status( _T( "Write IMEI Fail"));
			this->Show_Out( _T( "Write IMEI Fail" ));

			this->m_iError_Code = ERR_Write_IMEI_Error;
			this->m_csError_Code = _T( "FAIL, Write IMEI Fail" );
			//
			throw(0);
		}
		else
		{
			this->Show_Out( _T( "Write IMEI Pass" ));
		}

		//�ؿ�����
		this->m_cla_Com_Port.Port_Open( 500 );

		//дSN
		//----------------------------------------д SN �� ---------------------------------------- 
		this->Show_Status( _T( "Write SN" ) );
		this->Show_Out( _T( "Write SN" ) );

		//���SN��־λ
		
		csSN_Buff_In_Write.Format( _T( "%s%s" ), wcp_SN, _T( "00" ) );//������λ00�ı�־λ

		//��� SN ����
		if( csSN_Buff_In_Write.GetLength() != 17 )
		{
			//fail
			this->Show_Status( _T( "Check_SN_Data Error" ));
			this->Show_Out( _T( "Check_SN_Data Error" ));

			this->m_iError_Code = ERR_Write_SN_LEN_Error;
			this->m_csError_Code.Format( _T( "FAIL, Check_SN_Data" ) );
			//
			throw(0);
		}

		if(this->_W_Write_SN_Intel( csSN_Buff_In_Write ) == false)
		{
			this->Show_Status( _T( "Write SN Fail"));
			this->Show_Out( _T( "Write SN Fail" ));

			this->m_iError_Code = ERR_Write_SN_Error;
			this->m_csError_Code = _T( "FAIL, Write SN Fail" );
			//
			throw(0);
		}
		else
		{
			this->Show_Out( _T( "Write SN Pass" ) );
		}

		//��IMEI
		//---------------------------------------- �� ��IMEI �� ---------------------------------------- 
		this->Show_Status( _T( "Read IMEI" ) );
		this->Show_Out( _T( "Read IMEI" ) );

		if(this->_Read_Pri_IMEI_Intel( wcrData ) == false)
		{
			this->Show_Status( _T( "Read IMEI Fail"));
			this->Show_Out( _T( "Read IMEI Fail" ));

			this->m_iError_Code = ERR_Read_IMEI_Error;
			this->m_csError_Code = _T( "FAIL, Read IMEI Fail" );
			//
			throw(0);
		}
		else
		{
			this->Show_Out( _T( "Read IMEI Pass" ) );
		}
		//�˶�IMEI
		//---------------------------------------- �˶� ��IMEI �� ---------------------------------------- 
		this->Show_Out( _T( "Confirm_Pri_IMEI" ) );
		if( wcscmp( wcrData, wcp_IMEI ) == 0 )
		{
			this->Show_Out( _T( "Confirm_Pri_IMEI Pass" ));
		}
		else
		{
			this->Show_Status( _T( "Confirm_Pri_IMEI Fail"));
			this->Show_Out( _T( "Confirm_Pri_IMEI Fail" ));

			this->m_iError_Code = ERR_Confirm_IMEI_Error;
			this->m_csError_Code = _T( "FAIL, Confirm_Pri_IMEI Fail" );
			throw(0);
		}
		//��SN
		//---------------------------------------- �� SN �� ---------------------------------------- 
		this->Show_Status( _T( "Read SN" ) );
		this->Show_Out( _T( "Read SN" ) );

		if(this->_Read_SN_Intel( wcrData ) == false)
		{
			csData.Format(_T("ERROR, Read SN fail!"));
			this->Show_Status(csData);
			this->Show_Out( _T( "Read SN Fail" ));

			this->m_iError_Code = ERR_Read_SN_Error;
			this->m_csError_Code = _T( "FAIL, Read SN Fail" );
			//
			throw(0);
		}
		else
		{
			this->Show_Out( _T( "Read SN Pass" ) );
		}
		//�˶�SN
		//---------------------------------------- �˶� SN �� ---------------------------------------- 
		//ȥ����־λ
		wcrData[15] = '\0';
		wcrData[16] = '\0';
		this->Show_Out( _T( "Confirm_SN" ) );

		//�˶�SN��
		if( wcscmp( wcrData, wcp_SN ) == 0 )
		{
			this->Show_Out( _T( "Confirm_SN Pass" ));
		}
		else
		{
			this->Show_Status( _T( "Confirm_SN Fail"));
			this->Show_Out( _T( "Confirm_SN Fail" ));

			this->m_iError_Code = ERR_Confirm_SN_Error;
			this->m_csError_Code = _T( "FAIL, Confirm_SN Fail" );
			throw(0);
		}
		//�رն˿�
		this->m_cla_Com_Port.Port_Close();
	}
	catch (...)
	{
		//�رն˿�
		this->m_cla_Com_Port.Port_Close();	
		return false;
	}
	return true;
	
}
bool CARM_OP_INTEL::Detect_Com_Port(int i_Tgt_Com, int i_TimeOut)
{
	time_t t_Start, t_End;
	CString csData;
	int i;

	//
	try
	{
		/*
		//��ʾ
		this->Show_Title(_T("< Connect_Port >"));


		if(this->m_bDebug_Log_File)
		{
			this->Save_Trace_Log(_T("< Detect_Com_Port >"));
			//
			csData.Format(_T("Detect_Com_Port( Tgt_Com = %d; TimeOut = %d )"), i_Tgt_Com, i_TimeOut);
			this->Save_Trace_Log(csData);
		}
		*/


		//
		time(&t_Start);

		//ѭ��
		do
		{
			//����
			if(this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com) == true)
			{
				break;//�˳�while
			}

			//��ʱ���
			time(&t_End);
			i = i_TimeOut - (int)difftime(t_End, t_Start);

			//
			if(i <= 0)
			{
				throw(0);
			}
			else
			{
				csData.Format(_T("Detect COM(%d) %d ......"), i);
				this->Show_Status(csData);
				Sleep(1200);
			}
		}
		while(true);

		//
		this->Show_Status(_T(""));


		/*
		//��ʾpass
		csData.Format(_T("Detect_Port_%d"), i_Tgt_Com);
		this->Show_Test_Result(csData, i_Tgt_Com, i_Tgt_Com, i_Tgt_Com);
		*/
	}
	catch(...)
	{
		//
		csData.Format(_T("FAIL, Detect COM(%d) fail!"), i_Tgt_Com);
		this->Show_Status(csData);

		//fail
		return false;
	}

	//pass
	return true;
}



//дIMEI
bool CARM_OP_INTEL::_W_Write_Pri_IMEI_Intel( const wchar_t * wcp_IMEI )
{
	int iTemp;
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	bool bBreak_Flag = false;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData;
	time_t t_Start, t_End;


	try
	{
		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof( SECURITY_ATTRIBUTES );
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if( CreatePipe( &hRead_Pipe, &hWrite_Pipe, &sa, 0 ) == FALSE )
		{
			throw(0);
		}
		//------------------------------ 


		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC( &wcpData, 4096 );


		//------------------------------ ���dosָ�� ------------------------------ 
		swprintf( wcpData, _T( "cmd.exe %cc %s\\INT\\CH1\\sec_provision.exe -c%d -i1 %s" ),
			'/', this->m_csCurr_Exe_Path, this->m_iAt_Port, wcp_IMEI );

		this->Show_Out(wcpData);

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof( STARTUPINFO );
		GetStartupInfo( &si );//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//
		if( CreateProcess( NULL, wcpData, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi ) == FALSE )
		{
			throw(2);
		}
		//------------------------------ 


		if( hWrite_Pipe != NULL )
		{
			CloseHandle( hWrite_Pipe );
			hWrite_Pipe = NULL;
		}


		//------------------------------ ѭ����ȡprocess�Ĵ�����Ϣ ------------------------------ 
		this->m_cla_GFC.BUFF_CHAR_CALLOC( &cpRead_Buff, 4096 );
		csTotal_Info.Format( _T( "" ) );

		//��ʼ��ʱ
		time( &t_Start );

		while( bBreak_Flag == false )
		{
			//��info����.exe�����ѽ�����������read�Ļ�����������һ����
			memset( cpRead_Buff, 0, 4096 );
			ulActual_Read = 0;

			//
			if( ReadFile( hRead_Pipe, cpRead_Buff, 4095, &ulActual_Read, NULL ) == NULL )
			{
				//һ�����õ�.exe�ļ����н������������д˲��裬�������������
				//����һ�������н����ؼ��ʿ����ж����Ͳ�Ҫ������ReadFile
				bBreak_Flag = true;
			}

			//����info������
			if( ulActual_Read > 0 )
			{
				//�ַ�ת��
				memset( wcpData, 0, 4096 );
				this->m_cla_GFC.MultiByte_To_WideChar( cpRead_Buff, wcpData, &iTemp );

				csData.Format( _T( "%s" ), wcpData );
				csData.Replace( _T( '\r' ), _T( ' ' ) );
				csData.Replace( _T( '\n' ), _T( ' ' ) );

				//��������
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
			}//if(ulActual_Read > 0

			//����Ƿ�pass
			if( wcsstr( csTotal_Info, _T( "success: operation completed with success" ) ) != NULL )
			{
				bBreak_Flag = true;//pass
				break;//�˳�while
			}

			//����Ƿ�����ˣ�failure: some error occured, see log info.��
			if( wcsstr( csTotal_Info, _T( "error" ) ) != NULL )
			{
				this->Show_Out(csTotal_Info);
				throw(3);
			}

			//��ʱ���
			time( &t_End );
			if( (int)difftime( t_End, t_Start ) >= 10 )//��ʱ����fail
			{
				throw(4);
			}

			Sleep( 200 );
		}//while(bBreak_Flag == false
		//------------------------------ 

		//�ͷž����ָ��
		if( hRead_Pipe != NULL )
		{
			CloseHandle( hRead_Pipe );
			hRead_Pipe = NULL;
		}
	}
	catch( ... )
	{
		//�ͷž����ָ��
		if( hRead_Pipe != NULL )
		{
			CloseHandle( hRead_Pipe );
			hRead_Pipe = NULL;
		}
		if( hWrite_Pipe != NULL )
		{
			CloseHandle( hWrite_Pipe );
			hWrite_Pipe = NULL;
		}

		this->m_cla_GFC.BUFF_WCHAR_FREE( &wcpData );
		this->m_cla_GFC.BUFF_CHAR_FREE( &cpRead_Buff );

		//��ֹ���ܴ��ڵĲ���Ҫ���߳�
		this->m_cla_GFC.Kill_Process( _T( "sec_provision.exe" ) );

		return false;
	}

	this->m_cla_GFC.BUFF_WCHAR_FREE( &wcpData );
	this->m_cla_GFC.BUFF_CHAR_FREE( &cpRead_Buff );

	//��ֹ���ܴ��ڵĲ���Ҫ���߳�
	this->m_cla_GFC.Kill_Process( _T( "sec_provision.exe" ) );

	return true;
}

//дSN
bool CARM_OP_INTEL::_W_Write_SN_Intel( const wchar_t * wcp_SN )
{
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iRec_Bits;

	csAT_Cmd.Format( _T( "AT+EGMR=1,5,\"%s\"" ), wcp_SN );

	for( int i = 0; i < 4; i++ )
	{
		memset( wcrData, 0, sizeof( wcrData ) );
		this->m_cla_Com_Port.Query( csAT_Cmd, wcrData, &iRec_Bits, 100 );

		if( wcsstr( wcrData, _T( "OK" ) ) != NULL )
		{
			return true;
		}
	}

	return false;
}
//��IMEI
bool CARM_OP_INTEL::_Read_Pri_IMEI_Intel( wchar_t * wcp_IMEI )
{
	/*
	at@1,18sec:imei_read(0)

	@1,18 imei[0] = "004999010640000"
	result_cause = 0
	@1,18 OK
	*/

	CString csData, csTemp;

	try
	{
		//����
		if( this->m_cla_Com_Port.Query_Tgt_CHAR( _T( "at@1,18sec:imei_read(0)" ), _T( "OK" ), 3 ) == false )
		{
			throw(0);
		}

		//��ȡAT��������
		this->m_cla_Com_Port.GET_RX_PKT_BUFF_DATA( &csData );

		//��ȡ��������
		if( this->m_cla_GFC.GET_AT_RSP_MID_STRING( csData, _T( "imei[0] = \"" ), _T( "\"" ), &csTemp ) == false )
		{
			throw(0);
		}

		//
		swprintf( wcp_IMEI, _T( "%s" ), csTemp );
	}
	catch( ... )
	{
		return false;
	}

	return true;
}

//��SN

bool CARM_OP_INTEL::_Read_SN_Intel( wchar_t * wcp_SN )
{
	CString csData, csTemp;

	try
	{
		//����
		if( this->m_cla_Com_Port.Query_Tgt_CHAR( _T( "at+egmr=0,5" ), _T( "OK" ), 3 ) == false )
		{
			throw(0);
		}

		//��ȡAT��������
		this->m_cla_Com_Port.GET_RX_PKT_BUFF_DATA( &csData );

		//��ȡ��������
		if( this->m_cla_GFC.GET_AT_RSP_MID_STRING( csData, _T( "+EGMR: \"" ), _T( "\"" ), &csTemp ) == false )
		{
			throw(0);
		}

		//
		swprintf( wcp_SN, _T( "%s" ), csTemp );
	}
	catch( ... )
	{
		return false;
	}

	return true;
}
