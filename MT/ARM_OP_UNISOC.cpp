#include "StdAfx.h"
#include "ARM_OP_UNISOC.h"
#include "ts_lan.h"
#include "EnumSerial.h"


CCriticalSection g_cla_Lock_UNISOC_EDL;//����ģʽ >>>> EDLģʽ
CCriticalSection g_cla_Lock_UNISOC_QDLPort;//ע����ȡQDLOADER��
Channel_Port_Info CARM_OP_UNISOC::m_arrCh_Port_Info[DL_CHANNEL_NUM];

CARM_OP_UNISOC::CARM_OP_UNISOC(void)
: m_iCom_Port(0)
, m_bRuning(false)
, m_iStop_Flag(-1)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_csCurr_Exe_Path(_T(""))
, m_bUnisoc_Reset_after_DL(false)
, m_iCom_Port_Auto_Sec(0)
{
}


CARM_OP_UNISOC::~CARM_OP_UNISOC(void)
{


}

void CARM_OP_UNISOC::Show_Bar_IMEI(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_BAR_IMEI_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_UNISOC::Show_DL_Port(const int wip_port)
{
	::SendMessage(this->m_hWnd_View, WM_DL_PORT_MSG, (WPARAM)wip_port, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_UNISOC::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_UNISOC::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_UNISOC::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_UNISOC::Show_Progress(double d_Pgr_Percent)
{
	//�˺�����Ӧ�ð����������ٷֱȵļ��㣬Ȼ������Ϣ��Ӧ��������ɾ��岽�������ļ���
	//Progress�ܳ�����1000
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}


void CARM_OP_UNISOC::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_UNISOC::Show_COMPORT(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_COMPORT_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

bool CARM_OP_UNISOC::LOAD_BB_INFO(const wchar_t * wcp_FW_File_Path)
{
	//�Ӽ��ص�FW�ļ����У��ж����ص�оƬƽ̨
	CString cs_FW_Dir, cs_Finder_Dir, cs_FW_Name;
	CFileFind cla_Finder; //�ļ�������
	BOOL b_Found;
	bool bRtn = false;


	try
	{
		//��ȡ���·��
		if(this->m_cla_GFC.Get_File_Directory(wcp_FW_File_Path, &cs_FW_Dir) == false)
		{
			this->m_csError_Code.Format(_T("Error, Get pac file directory fail!"));
			throw(0);
		}

		//Ѱ���ļ���ʼ
		cs_Finder_Dir.Format(_T("%s\\*"), cs_FW_Dir);

		b_Found = cla_Finder.FindFile(cs_Finder_Dir);

		//
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

				cs_FW_Name.MakeUpper();//��д


				//�����.pac�ļ�
				if(wcsstr(cs_FW_Name, _T(".PAC")) != NULL)
				{
					if(wcsstr(cs_FW_Name, _T("SL8541")) != NULL)
					{
						this->m_eu_BB_Chip = SL8541;
						bRtn = true;
						break;
					}
					else if(wcsstr(cs_FW_Name, _T("UIS8581")) != NULL)
					{
						this->m_eu_BB_Chip = UIS8581;
						bRtn = true;
						break;
					}
					else if(wcsstr(cs_FW_Name, _T("UDX710")) != NULL)
					{
						this->m_eu_BB_Chip = UDX710;
						bRtn = true;
						break;
					}
					else if(wcsstr(cs_FW_Name, _T("8915DM")) != NULL)
					{
						this->m_eu_BB_Chip = UIS8910;
						bRtn = true;
						break;
					}
					else if (wcsstr(cs_FW_Name, _T("BC95GU")) != NULL)
					{
						this->m_eu_BB_Chip = UIS8811;
						bRtn = true;
						break;
					}
					
				}//if(wcsstr(cs_FW_Name, _T(".HEX")) != NULL
				else
				{
					bRtn = true;
					break;
				}
			}

		}//while(b_Found


		//�ر�find
		cla_Finder.Close();


		//������Ϣ
		if(bRtn == false)
		{
			this->m_csError_Code.Format(_T("Error, Detect BB chip info fail!"));
			throw(0);
		}

	}
	catch(...)
	{
		return false;
	}

	//
	return bRtn;
}


bool CARM_OP_UNISOC::GetDownloadPort(int &iDLPort)
{
	CString csData, csCOM;
	bool bDL_Flag = true;
	bool bEDL_Now = false;//�Ƿ��Ѿ���EDLģʽ��
	int irQDL_Port_List[64], iQDL_Port_Quan;//���С�QDLoader���ڵļ�¼

	//20210413
	if (UIS8811 == this->m_eu_BB_Chip)
	{
		return true;
	}

	try
	{
		iDLPort = this->m_iCom_Port;

		csData.Format(_T("Port(%d) find...\r\n"),this->m_iCom_Port);
		Show_Out(csData);
		if(this->Confirm_Dynamic_Com_Port(this->m_iCom_Port, 35) == false)//
		{
			//20210407
			if (this->m_iCom_Port_Auto_Sec > 0)
			{			
				if (this->Confirm_Dynamic_Com_Port(this->m_iCom_Port_Auto_Sec, 35) == false)//
				{
					bDL_Flag = false;
				}
				else
				{
					this->m_iCom_Port = this->m_iCom_Port_Auto_Sec;
					iDLPort = this->m_iCom_Port;
				}
			}
			else
			{
				bDL_Flag = false;
			}
	
		}

		if (false == bDL_Flag)
		{
			csData.Format(_T("Port(%d) can't find...\r\n"),this->m_iCom_Port);
			Show_Out(csData);

			this->Show_Status(this->m_csError_Code);
			throw(1);
		}
		csData.Format(_T("Port(%d) find sucess...\r\n"),this->m_iCom_Port);
		Show_Out(csData);

		//COM�ڸ����ɺ�ģ���ʼ��δ��ɣ�ģ���޷�����
		//��������
//		Sleep(3000);

		//------------------------------ �ж�COM���Ƿ����ǡ�EDL��QDLoader�ڡ� ------------------------------ 
		//���
		iQDL_Port_Quan = 0;
		memset(irQDL_Port_List, 0, sizeof(irQDL_Port_List));

		csData.Format(_T("Port(%d) check QDLoader...\r\n"),this->m_iCom_Port);
		Show_Out(csData);

		//��PC�����ڵ����С�QDLoader���ڼ�¼
		this->Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(irQDL_Port_List, &iQDL_Port_Quan);

		//�˶ԡ���ǰ���ڡ��ǲ����Ѿ��ǡ�SPRD U2S Diag���ؿڡ�
		for(int i=0; i<iQDL_Port_Quan; i++)
		{
			if(this->m_iCom_Port == irQDL_Port_List[i])
			{
				//����ǰ���ڡ�SPRD U2S Diag���ؿڡ�
				bEDL_Now = true;
				break;//�Ѿ���EDLģʽ
			}
		}//for(j=0; j<iQDL_Port_Quan; j++

		//ȷ�������õġ�QDLoader�ڡ�
		if(bEDL_Now == true)
		{
			//�Ѿ���EDLģʽ
			csCOM.Format(_T("QDLoader %d"), this->m_iCom_Port);
			Save_Trace_Log(csCOM);

			csData.Format(_T("Port(%d) is QDLoader port\r\n"),this->m_iCom_Port);
			Show_Out(csData);

			iDLPort = this->m_iCom_Port;
			//20181115  ����
			csCOM.Format(_T("QDLoader(%d)\r\n"), this->m_iCom_Port,iDLPort);
			Show_Out(csCOM);
			//���浱ǰʹ��QDLoader��COM��
// 			if((m_iClass_Idx>0)&&(m_iClass_Idx<DL_CHANNEL_NUM+1))
// 			{
// 				g_QDL_USE_Port_List[m_iClass_Idx-1] = iDLPort;
// 			}
		}
		else
		{
			csCOM.Format(_T("AT %d"), this->m_iCom_Port);
			Show_COMPORT(csCOM);
			Save_Trace_Log(csCOM);

			csData.Format(_T("Port(%d) is AT port\r\n"),this->m_iCom_Port);
			Show_Out(csData);


			//�л���EDLģʽ
			if(this->Switch_EDL( &iDLPort ) == false)
			{
				this->Show_Status(this->m_csError_Code);
				csData.Format(_T("QDownload Fail(%s)\r\n"),this->m_csError_Code);
				Show_Out(csData);
				throw(2);
			}
			else
			{
				//���ҵ���Qloader�ڿ��ܻ�û׼���� 20180508
//				Sleep(1000);
				csData.Format(_T("QDownload Success(%d)\r\n"),iDLPort);
				this->Show_Status(csData);
				Show_Out(csData);
			}
			//ˢ�¶˿ڵ���ʾ
			//20181115  ����
			csCOM.Format(_T("AT(%d)->QDownload(%d)"), this->m_iCom_Port,iDLPort);
			Show_Bar_IMEI(csCOM);
			Show_DL_Port(iDLPort);
			csCOM.Format(_T("AT(%d)->QDownload(%d)\r\n"), this->m_iCom_Port,iDLPort);
			Show_Out(csCOM);

			csCOM.Format(_T("QDownload %d"), iDLPort);
			Show_COMPORT(csCOM);
// 			m_iCom_Old_Port=m_iCom_Port;
// 			Save_Trace_Log(csCOM);
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool CARM_OP_UNISOC::Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut)
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
				Sleep(2000);
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


bool CARM_OP_UNISOC::Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(int * ip_QDLoader_Port_List, int * ip_Port_Quan)
{
	//��ע����ȡ��QDLoader�����豸���б�
	/*
	//8909ƽ̨
	USB\VID_05C6&PID_9008&REV_0000
	USB\VID_05C6&PID_9008
	Qualcomm HS-USB QDLoader 9008 (COM11)
	*/

	wchar_t wcrUSB_PID_VID[64];//DM��
	CString csrUsb_Dev_List[64], csFriendly_Name;
	int iUsb_Dev_Quan, i, iCnt;
	wchar_t wcrData[128];
	wchar_t * wcpPoint = NULL;

	g_cla_Lock_UNISOC_QDLPort.Lock();
	try
	{
		//��ͬƽ̨USB�������ؼ��ʲ�ͬ
		switch(this->m_eu_BB_Chip)
		{
		case SL8541:
			swprintf_s(wcrUSB_PID_VID, _T("VID_1782&PID_4D00"));// SC200L\SC100Y
			break;
		case UIS8581:
			swprintf_s(wcrUSB_PID_VID, _T("VID_1782&PID_4D00"));// SC200K
			break;
		case UDX710:
			swprintf_s(wcrUSB_PID_VID, _T("VID_1782&PID_4D00"));// RG500U
			break;
		case UIS8910:
			swprintf_s(wcrUSB_PID_VID, _T("VID_0525&PID_A4A7"));// EC200U
			break;
		//case UIS8811:
		//	swprintf_s(wcrUSB_PID_VID, _T("VID_1782&PID_4D00"));// BC95GU
		//	break;
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
			
			//��ȡ�豸�ġ�FriendlyName�������磺SPRD U2S Diag (COM4)��
			if(this->m_cla_GFC.Get_Reg_Usb_Dev_Friendly_Name(wcrUSB_PID_VID, csrUsb_Dev_List[i], &csFriendly_Name) == false)
			{
				continue;
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
		g_cla_Lock_UNISOC_QDLPort.Unlock();
		return false;
	}
	g_cla_Lock_UNISOC_QDLPort.Unlock();
	return true;
}


bool CARM_OP_UNISOC::Switch_EDL(int * ip_New_QDLoader_Port)
{
	//�л�EDLģʽ����̽�⡢�����������õ�QDLoader��
	int iNew_QDLoader_Port = 0;
	CString csData;
	CArray<int, int&> arrOldQDLoadPort;  // ����ָ��ǰ�ľɿ�
	CArray<int, int&> arrQDLoadPort;     // ����ָ���Ľ������ؿ�
	CArray<int, int&> arrNewQDLoadPort;  // ����ָ����������
	CArray<SSerInfo, SSerInfo&> asi;     // ����ָ�����豸������
	int iTimeOut = 50;                   // ��ȡ�����������ؿڵĳ�ʱ
	int iQDLoader_Port = 0;              // �������ؿڵ���ʱ����
	BOOL bIsNew_QDLoader_Port = FALSE;
	int iLoop = 0;

	try
	{
		//��ʾ
		csData.Format(_T("Switch EDL mode(Port%d) ......"),this->m_iCom_Port);
		this->Show_Status(csData);
		this->Show_Out(csData);

		//��������
		::g_cla_Lock_UNISOC_EDL.Lock();

		//�����л�EDLģʽ��ָ�at+qdownload=1��
		arrOldQDLoadPort.RemoveAll();
		if (FALSE ==Switch_EDL_Sub(arrOldQDLoadPort))
		{
			throw(0);
			//�ٽ���һ��
// 			arrOldQDLoadPort.RemoveAll();
// 			if(FALSE ==Switch_EDL_Sub(arrOldQDLoadPort))
// 			{
// 				throw(0);
// 			}
		}


		do 
		{
			arrQDLoadPort.RemoveAll();
			arrNewQDLoadPort.RemoveAll();
			asi.RemoveAll();
			iLoop ++;
			Sleep(1000);

			EnumSerialPorts(asi, FALSE);
			// ���˽������ؿ�
			for (int i = 0; i < asi.GetSize(); i ++)
			{
				//QDloader 9008
				if (wcsstr(asi[i].strPortDesc, _T("SPRD U2S Diag")) != NULL)
				{
					iQDLoader_Port = _wtoi(asi[i].strPortName);
					arrQDLoadPort.Add(iQDLoader_Port);
					// 					csData.Format(_T("Ch%d:Qdloader %d: %d\n"), m_iClass_Idx, arrQDLoadPort.GetSize(), iQDLoader_Port);
					// 					this->Show_Out(csData);
				}
			}
			// ��ȡ�Ľ������ؿں;ɵĽ������ؿ���ƥ��
			if (arrQDLoadPort.GetSize() > 0)
			{
				for (int i = 0; i < arrQDLoadPort.GetSize(); i ++)
				{
					int j = 0;
					for (j = 0; j < arrOldQDLoadPort.GetSize(); j ++)
					{
						if (arrQDLoadPort[i] == arrOldQDLoadPort[j])
						{
							break;
						}
					}
					if (j == arrOldQDLoadPort.GetSize())
					{
						arrNewQDLoadPort.Add(arrQDLoadPort[i]);
						csData.Format(_T("Ch%d:New Qdloader %d: %d\n"), m_iClass_Idx, arrNewQDLoadPort.GetSize(), arrQDLoadPort[i]);
						this->Show_Out(csData);
						bIsNew_QDLoader_Port = TRUE;
					}
				}
				if (arrOldQDLoadPort.GetSize() == 0)// �ɵĿڲ����ڣ�˵������������
				{
					for (int i = 0; i < arrQDLoadPort.GetSize(); i ++)
					{
						arrNewQDLoadPort.Add(arrQDLoadPort[i]);
						csData.Format(_T("Ch%d:New Qdloader %d: %d\n"), m_iClass_Idx, i+1, arrQDLoadPort[i]);
						this->Show_Out(csData);
						bIsNew_QDLoader_Port = TRUE;
					}
				}
			}

			//
			if (TRUE == bIsNew_QDLoader_Port)
			{
				break;
			}
			else
			{
				//
				if (iLoop >= iTimeOut)
				{
					bIsNew_QDLoader_Port = FALSE;
					csData.Format(_T("Ch%d:New Qdloader Fail\n"), m_iClass_Idx);
					this->Show_Out(csData);
					throw(0);
				}
			}

		} while (TRUE);
		//
		int j = 0;
		for (int i = 0; i < arrNewQDLoadPort.GetSize(); i ++)
		{
			for ( j = 0; j < DL_CHANNEL_NUM; j ++)
			{
				if (j != (m_iClass_Idx - 1) 
					&& CARM_OP_UNISOC::m_arrCh_Port_Info[j].bIsRunning == TRUE
					&& arrNewQDLoadPort[i] == CARM_OP_UNISOC::m_arrCh_Port_Info[j].iQDLoader_Port)
				{
					break;
				}
			}
			if (j == DL_CHANNEL_NUM)
			{
				iNew_QDLoader_Port = arrNewQDLoadPort[i];
				break;
			}
		}

		//����Ϊ��
		if (iNew_QDLoader_Port ==0)
		{
			csData.Format(_T("Ch%d:New Qdloader = 0\n"), m_iClass_Idx);
			this->Show_Out(csData);
			throw(2);
		}
		//���˿�
		//����ȡ����QDloader���Ƿ�����
		if(this->Confirm_Dynamic_Com_Port(iNew_QDLoader_Port, 15) == false)
		{
			csData.Format(_T("Ch%d:New Qdloader not normal\n"), m_iClass_Idx);
			this->Show_Out(csData);
			throw(2);
		}
		//��������
		*ip_New_QDLoader_Port = iNew_QDLoader_Port;
		CARM_OP_UNISOC::m_arrCh_Port_Info[m_iClass_Idx-1].iQDLoader_Port = iNew_QDLoader_Port;

		//��ʾ
		this->Show_Status(_T("Switch EDL mode completely"));
		this->Show_Out(_T("Switch EDL mode completely"));
	}
	catch(...)
	{
		//�رմ���
		this->m_cla_Com_Port.Port_Close();

		//��������
		*ip_New_QDLoader_Port = iNew_QDLoader_Port;
		CARM_OP_UNISOC::m_arrCh_Port_Info[m_iClass_Idx-1].iQDLoader_Port = iNew_QDLoader_Port;
		//�������
		::g_cla_Lock_UNISOC_EDL.Unlock();
		//
		return false;
	}


	//�������
	::g_cla_Lock_UNISOC_EDL.Unlock();
	//
	return true;

}

BOOL CARM_OP_UNISOC::Switch_EDL_Sub(CArray<int, int&> &arrOldQDLoadPort)
{
	CString csData, strATCmd;
	int iTimeOut = 0, iRec_Bits;
	time_t start, finish;
	wchar_t wcrData[128];
	try
	{
		//----------------------- �򿪴��ڣ���һ�Σ�----------------------- 
		csData.Format(_T("Open Port%d ......"),this->m_iCom_Port);
		this->Show_Status(csData);
		this->Show_Out(csData);
		this->Save_Trace_Log(csData);

		//�Խ��沨���ʴ򿪴���
		//ͬ������
		this->m_cla_Com_Port.m_iBaudRate = 115200;
		this->m_cla_Com_Port.m_iPort = m_iCom_Port;
		this->m_cla_Com_Port.m_dwDTR_CTRL = DTR_CONTROL_ENABLE;
		this->m_cla_Com_Port.m_dwRTS_CTRL = RTS_CONTROL_ENABLE;

		//�򿪴���
		if(this->m_cla_Com_Port.Port_Open(500) == false)  
		{
			time_t start,finish;
			int iTimeOut =9;//�򿪶˿ڸ�Ϊѭ�����Ҵ򿪵ķ�ʽ
			try
			{
				//
				time( &start );
				do
				{
					Sleep( 200 );
					if(true == this->m_cla_Com_Port.Port_Open(500))
					{
						this->Show_Status(_T("Open Port Pass"));
						this->Show_Out(_T("Open Port Pass"));
						this->Save_Trace_Log(_T("Open Port Pass"));
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
				csData.Format(_T("%s"), this->m_cla_Com_Port.m_csError_Info);
				this->Show_Status(csData);
				m_csError_Code = csData;
				this->Show_Out(csData);
				this->Save_Trace_Log(csData);
				throw(0);
			}
		}
		//����ָ��֮ǰ����ע��� HEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM �еĶ˿�ֵ
//		Sleep(1000);
		CArray<SSerInfo, SSerInfo&> asi;
		// Populate the list of serial ports.
		EnumSerialPorts(asi, FALSE);
		for (int i = 0; i < asi.GetSize(); i++)
		{
			int iPortTemp = _wtoi(asi[i].strPortName);
			//QDloader 9008
			if (wcsstr(asi[i].strPortDesc, _T("SPRD U2S Diag")) != NULL)
			{
				//����Ŀ��
				arrOldQDLoadPort.Add(iPortTemp);
				// 					csData.Format(_T("Ch%d:Old Qdloader %d: %d\n"), m_iClass_Idx, arrOldQDLoadPort.GetSize(), iPortTemp);
				// 					this->Show_Out(csData);
			}

		}


		//----------------------- �����л�EDLģʽ��ָ�at+qdownload=1��-----------------------
		//��� COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------------------------------- 
		switch (this->m_eu_BB_Chip)
		{
		case SL8541:
		case UIS8581:
		case UDX710:
			strATCmd = _T( "at+qdownload=1" );
			break;
		case UIS8910:
			strATCmd = _T( "at+qdownload" );
			break;
		//case UIS8811:
		//	strATCmd = _T("AT^FORCEDNLD");
		//	break;
		default:
			strATCmd = _T( "at+qdownload=1" );
			break;
		}
		iTimeOut = 5;
		//
		time( &start );
		//
		do
		{
			//��� COM BUFF
			this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();
			// AT��תΪ���ؿ�
			//����
			this->m_cla_Com_Port.Write( strATCmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200);
			//��ȡ
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			this->Show_Status(_T("Switch Download Port..."));
			this->Show_Out(_T("Switch Download Port success..."));
			this->Save_Trace_Log(_T("Switch Download Port success..."));
			break;
// 			if(wcsstr(wcrData, _T("OK")))
// 			{	
// 				break;//pass, �˳�while
// 			}
// 			else
// 			{
// 				// at+qdownload=1
// // 				strATCmd = _T("at+qdownload=1");
// 				//����
// 				this->m_cla_Com_Port.Write( strATCmd );
// 			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				break;
			}
		}
		while( true );

		//----------------------- �رմ��� -----------------------
		this->m_cla_Com_Port.Port_Close();
// 		this->Show_Status(_T("Close AT Port"));
// 		this->Show_Out(_T("Close AT Port"));
// 		this->Save_Trace_Log(_T("Close AT Port"));
		// 		//-----------------------
		// 		//-----------------------
		// 		//���DM���Ƿ񻹴���
		Sleep(500);
		if (false ==  Confirm_Dynamic_Com_Port_N(this->m_iCom_Port, 30))
		{
		    //���DM������Ϊ��FAIL
		   throw(0);
		} 
	}
	catch(...)
	{
		//�رմ���
		this->m_cla_Com_Port.Port_Close();
		return FALSE;

	}

	this->m_cla_Com_Port.Port_Close();
	return TRUE;
}

//�������ʧ��Ϊ��������
bool CARM_OP_UNISOC::Confirm_Dynamic_Com_Port_N(const int i_Tgt_Com_Port, int i_TimeOut)
{
	time_t t_Start, t_End;
	CString csA, csB;
	int j = 0;

	try
	{
		csB.Format(_T(" ."));

		time(&t_Start);
		this->Show_Status(_T("Wait AT Port normal exist..."));
		this->Show_Out(_T("Wait AT Port normal exist..."));
		this->Save_Trace_Log(_T("Wait AT Port normal exist..."));
		do
		{
			//����regedit�е�COM�б�
			if(this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == false)
			{	
				this->Show_Status(_T("AT Port not exist"));
				this->Show_Out(_T("AT Port not exist"));
				this->Save_Trace_Log(_T("AT Port not exist"));
				return true;
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
				this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port %d timeout(N)!"), i_Tgt_Com_Port);
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
		this->Show_Out(_T("Query AT port exist"));
		this->Save_Trace_Log(_T("AT DM port exist"));
		this->Show_Status(_T("Query AT port exist"));
		return false;
	}

	return false;
}





int CARM_OP_UNISOC::Do_Execute_MCU_JLink(CString str_FolderName)
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
		this->Show_Status(_T("PF_SPREADTRUM, Download MCU Fw file ......"));
		this->Save_Trace_Log(_T("PF_SPREADTRUM, Download MCU Fw file ......"));
		this->Show_Out(_T("PF_SPREADTRUM, Download MCU Fw file ..."));


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
		swprintf(wcpCmd, _T("cmd.exe %cc .%s\\%s\\%d\\JLink.exe  \".%s\\%s\\%d\\eraseAndFlash.jlink\""),
			'/', this->m_csCurr_Exe_Path, str_FolderName, m_iClass_Idx, this->m_csCurr_Exe_Path, str_FolderName, m_iClass_Idx);

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
			this->m_csError_Code.Format(_T("FAIL, DL_MCU, Create process fail!"));
			this->m_iError_Code = DL_FW_MCU_FAIL1;
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
// 				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("script file read successfully"));
// 				//
// 				if (wcpPercent != NULL)
// 				{
// 					dCurr_Percent = 0.1;
// 					csTemp.Format(_T("script file ok"));
// 					this->Show_Status(csTemp);
// 					//��ʾ������
// 					this->Show_Progress(0.1);
// 
// 					csTemp.Format(_T("script file read successfully"));
// 
// 					wcpPercent += csTemp.GetLength();
// 
// 					csTotal_Info.Format(_T("%s"), wcpPercent);
// 
// 				}

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


bool CARM_OP_UNISOC::CreatQDM536_Jlink_CMD()
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

	strCmd_file_name.Format(_T(".%s\\QDM536\\%d\\eraseAndFlash.jlink"), this->m_csCurr_Exe_Path, m_iClass_Idx);

	if (this->Get_File_Size(strCmd_file_name, &ul_File_Size) == true)
	{
		::SetFileAttributes(strCmd_file_name, FILE_ATTRIBUTE_NORMAL);

		::DeleteFile(strCmd_file_name);

		Sleep(2000);
	}

	TCHAR buf[512];

	memset(buf, 0, 512);

	GetPrivateProfileString(_T("Multi"), _T("Scat_Cfg_File_Path"), _T("GetError"), buf, sizeof(buf), _T(".\\Multi_Config.ini"));


	if (Cmd_File.Open(strCmd_file_name, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::typeText) != NULL)
	{
		Cmd_File.SeekToEnd();

		csData.Format(_T("device gd32f303ze\n"));
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

		csData.Format(_T("loadfile \"%s\" 0x08000000\n"), buf);
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


bool CARM_OP_UNISOC::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
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


bool CARM_OP_UNISOC::Creat_Jlink_CMD(CString str_EraseAndFlash_File, CString str_DeviceName, CString str_FWAllPath)
{
	CStdioFile Cmd_File;
	CString   strCmd_file_name, csData;
	unsigned long ul_File_Size;

	strCmd_file_name.Format(_T(".%s\\%s\\%d\\eraseAndFlash.jlink"), this->m_csCurr_Exe_Path, str_EraseAndFlash_File, m_iClass_Idx);

	if (this->Get_File_Size(strCmd_file_name, &ul_File_Size) == true)
	{
		::SetFileAttributes(strCmd_file_name, FILE_ATTRIBUTE_NORMAL);

		::DeleteFile(strCmd_file_name);

		Sleep(2000);
	}

	TCHAR buf[512];

	memset(buf, 0, 512);

	GetPrivateProfileString(_T("Multi"), _T("Scat_Cfg_File_Path"), _T("GetError"), buf, sizeof(buf), _T(".\\Multi_Config.ini"));


	if (Cmd_File.Open(strCmd_file_name, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::typeText) != NULL)
	{
		Cmd_File.SeekToEnd();

		csData.Format(_T("device %s\n"), str_DeviceName);
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

		csData.Format(_T("loadfile \"%s\" 0x08000000\n"), buf);
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


CString CARM_OP_UNISOC::GetExeFilePath()
{
	TCHAR  exepath[MAX_PATH];
	CString  strdir, tmpdir;
	memset(exepath, 0, MAX_PATH);
	GetModuleFileName(NULL, exepath, MAX_PATH);
	tmpdir = exepath;
	strdir = tmpdir.Left(tmpdir.ReverseFind('\\'));
	return strdir;
}
