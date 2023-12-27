#include "StdAfx.h"
#include "ARM_OP_HUAWEI.h"
#include "ts_lan.h"

#include "USBDeviceDetection.h"

extern CCriticalSection g_PLC_Pow_Ctrl;
extern CCom_Port g_cla_Com_PLC_Power;//���һ���ĵİ��Զ��о߿���

CCriticalSection g_SSB_Ctrl;

CCriticalSection g_PLC_Power;

CCriticalSection g_cla_HS_Lock_Enter_EDL;
CCriticalSection g_cla_HS_Lock_QDLoader_Port;


void Multi_PLC_Pow_BC(LPVOID x);
void Multi_PLC_Pow_BC_1(LPVOID x);
void Multi_PLC_Pow_BC_2(LPVOID x);
void Multi_PLC_Pow_BC_3(LPVOID x);
void Multi_PLC_Pow_BC_4(LPVOID x);
void Multi_PLC_Pow_BC_5(LPVOID x);
void Multi_PLC_Pow_BC_6(LPVOID x);
void Multi_PLC_Pow_BC_7(LPVOID x);
void Multi_PLC_Pow_BC_8(LPVOID x);

CARM_OP_HUAWEI::CARM_OP_HUAWEI(void)
: m_iCom_Port(0)
,m_bRuning(false)
, m_iStop_Flag(-1)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_Ts_Num(0)
, m_iOP_TYPE(0)
, m_bGet_Debug_Log(false)
, m_csCurr_Exe_Path(_T(""))
, m_bCheckSaveLog(FALSE)
, m_DL_FLAG(FALSE)
, m_bPowFlag(FALSE)
, m_blWrCon(TRUE)
, m_bWriteKV(FALSE)
, m_bWriteFEC(FALSE)
, m_bSSB_Update(FALSE)
, m_iFindFlagBCG_BC95D(0)
,m_bBootFlag(false)
, mBar_IMEI(_T(""))
, mBar_SN(_T(""))
, mAT_FirewareName(_T(""))
, m_iPLC_Port(0)
, m_bNetFlagDL(FALSE)
, m_bIsRepMo(false)
, m_bOutLine(FALSE)
, m_iDL_flag(0)
, m_iHS_DL_MODE_M(0)
{
	m_iPpc=0;
	m_iPsc=0;
	m_iPac=0;

	m_iPsuc=0;
	m_iPauc=0;
	m_iPgc=0;

	m_bCheckSaveLog = FALSE;

	m_str_fwpkg_FW.b_Enable_DL =false;
	m_str_fwpkg_FW.cs_Name.Format(_T(""));
	m_str_fwpkg_FW.cs_Path.Format(_T(""));

	m_iUpdateall =0;

	m_iBC95BaudSwitch =0;
	m_iOP_Robot =0;
	m_iBC95_EraseSwitch =0;
	m_iBC95_EraseSwitchFirst =0;
	m_iBC95_DC_ResetTime =0;
	m_iError_Code =0;

	m_iBcg_ssb_COM_update_B200=0;
	m_iBcg_ssb_COM_update_B200_B180=0;
	m_PassNumb=0;
	m_ErrorNumb=0;
	m_iBcg_ssb_com=0;

	m_iREP_NO_SSB = 0;

	mAT_IMEI.Format(_T(""));
	mAT_SN.Format(_T(""));

	m_iBC95_DC_ResetTime_Flag= FALSE;

	m_iTimeInterval=3;

	m_iFindVerFlag=0;

	m_i150ResetTime1=0;//150ƽ̨����SSB��λ1
	m_i150ResetTime2=0;//150ƽ̨����SSB��λ2
	m_i150ResetTime3=0;//150ƽ̨����FW��λ1
	m_i150ResetTime4=0;//150ƽ̨����FW��λ2

	m_csOC.Format(_T(""));

	m_ul_DL_All_Bytes =0;
	m_ul_Transmit_Bytes =0;


	m_strUSBPortAddress.Format(_T(""));

	m_iRebootFlag =0;


}

CARM_OP_HUAWEI::~CARM_OP_HUAWEI(void)
{
}
void CARM_OP_HUAWEI::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_HUAWEI::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_HUAWEI::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

//�Զ��߳̿ص�Դ�ϵ�
void CARM_OP_HUAWEI::Set_PowerOn_SPC(int i_Index)
{
	this->Open_Power_SPC(i_Index);
	//::SendMessage(this->m_hWnd_View, WM_PowerOn_SPC, (WPARAM)i_Index, (LPARAM)i_Index);
}

//��������
bool CARM_OP_HUAWEI::ERASE_FLS_FILE_IN_EXE( int iClass_Idx)
{

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
	CString csTotal_Info, csData,info2,strPow,strEraseInfo;
	time_t t_Start,t_Start_T, t_End;

	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;

	bool bDownloading_Flag = false;

	CString strClassIdx,strBatPath,strDis;
	wchar_t * wcpCmd = NULL;

	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;

	//g_dl_load_proc.Lock();
	strClassIdx.Format(_T("%d"),iClass_Idx);

	if(TRUE == m_bCheckSaveLog)
	{
		//����־�ļ�
		CString strLogPath = m_csCurr_Exe_Path + "\\NB-IoT\\" + strClassIdx + "\\log.txt";	

		hFile_Handle = ::CreateFile(strLogPath,
			GENERIC_WRITE,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			CREATE_ALWAYS,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			CString info = strLogPath+" open failure";
			this->Show_Out(info);
			//AfxMessageBox(info);
			this->Show_Out(_T("Fail,ERASE_FLS_FILE_IN_EXE ----1"));
			return false;
		}
	}
	
	try  
	{
	
		//��ʼ��ʾ
		this->Show_Status(_T("Waiting......"));

		//��ʼ���������е���ز���
		this->m_csError_Code.Format(_T(""));

//֮ǰ�м��˿ڵ�����
// 		//------------------------------ ��鴮���Ƿ���� ------------------------------ 
// 		if(this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iCom_Port) == false)
// 		{
// 			this->m_csError_Code.Format(_T("FAIL, Confirm com port %d fail!"), this->m_iCom_Port);
// 			this->Show_Status(this->m_csError_Code);
// 			this->Show_Out(this->m_csError_Code);
// 			throw(0);
// 		}
// 
// 		//��鴮���Ƿ�ɴ� 20180227
// 		//
// 		this->m_cla_Com_Port.m_iPort = this->m_iCom_Port;
// 		this->m_cla_Com_Port.m_iBaudRate = 9600;
// 
// 		if(this->m_cla_Com_Port.Port_Open(400))
// 		{
// 			this->m_cla_Com_Port.Port_Close();
// 		}
// 		else
// 		{
// 			this->m_csError_Code.Format(_T("FAIL,����:Port-%d In Using!"), this->m_iCom_Port);
// 			this->Show_Status(this->m_csError_Code);
// 			this->Show_Out(this->m_csError_Code);
// 			this->m_cla_Com_Port.Port_Close();
// 			throw(0);
// 		}

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code = _T("Create pipe fail");
			this->Show_Out(this->m_csError_Code);
			throw(0);

		}
		//------------------------------ 
		csTotal_Info.Format(_T(""));	

		//------------------------------ ���dosָ�� ------------------------------ 

		CString strCom;
		strCom.Format(_T("com%d"),this->m_iCom_Port);

		CString strExeName = _T("");
		switch(iClass_Idx)
		{
			case 1: strExeName = _T("UEUpdater1.exe"); break;
			case 2: strExeName = _T("UEUpdater2.exe"); break;
			case 3: strExeName = _T("UEUpdater3.exe"); break;
			case 4: strExeName = _T("UEUpdater4.exe"); break;
			default: strExeName = _T("UEUpdater.exe"); break;
		}


		//CString pathtmp = "\""+strBatPath+"\"";
		//UEUpdater.exe erase --port com3
		CString strBatPath = m_csCurr_Exe_Path + "\\NB-IoT\\" + strClassIdx +"\\UEUpdaterCLI\\"+strExeName;
		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 4096);
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (strBatPath.GetLength() + 128));
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 4096);

		swprintf(wcpCmd, _T("cmd.exe %cc \"%s erase \-\-port %s\""), '/', strBatPath,strCom);

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			this->m_csError_Code = _T("Create process fail");
			this->Show_Out(this->m_csError_Code);
			throw(1);
		}
		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 
		//------------------------------ ѭ����ȡprocess�Ĵ�����Ϣ ------------------------------ 
		//��ʼ��ʱ
		time(&t_Start);

		while(false == bDownloading_Flag) 
		{
			//��info
			memset(cpRead_Buff, 0, 4096);
			ulActual_Read = 0;

			//������
			if(::ReadFile(hRead_Pipe, cpRead_Buff, 4095, &ulActual_Read, NULL) == NULL)
			{
				bDownloading_Flag = true;//20190626
			}
			else
			{
				//����
			}

			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 15)//��ʱ����fail
			{
				//����쳣��ʹ�����ذ�ť����
				this->m_csError_Code = _T("Erase TimeOut");
				this->Show_Out(this->m_csError_Code);
				throw(4);
			}	

			if(TRUE == m_bCheckSaveLog)
			{
				//дͷ����
				unsigned long dwNumb=0;  
				if(::WriteFile(hFile_Handle, cpRead_Buff, ulActual_Read, &dwNumb, NULL) != TRUE)
				{
					//AfxMessageBox(_T("log file Write fail!"));
					this->m_csError_Code = _T("log file Write fail");
					this->Show_Out(this->m_csError_Code);
					throw(0);
				}
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

				//��������
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
				//Please reset the UE
				strEraseInfo.Format(_T("trying to reset ue using bootloader commands"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), strEraseInfo);
				if (NULL != wcpHead)
				{
					// ģ��������ȴ�
					info2.Format(_T("1%%"));
					csTotal_Info.Format(_T("%s"), (wcpHead + strEraseInfo.GetLength()));

					strPow.Format(_T("reset ue using at command(1)"));
					this->Show_Status(strPow);
					this->Show_Progress(0.1);
				}

				strEraseInfo.Format(_T("trying to reset ue using at command"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), strEraseInfo);
				if (NULL != wcpHead)
				{
					csTotal_Info.Format(_T("%s"), (wcpHead + strEraseInfo.GetLength()));
					
					strPow.Format(_T("reset ue using at command"));
					this->Show_Status(strPow);
					this->Show_Progress(0.1);
				}
				strEraseInfo.Format(_T("verifying connection"));

				wcpHead = wcsstr(csTotal_Info.GetBuffer(),strEraseInfo);
				if (NULL != wcpHead)
				{
					csTotal_Info.Format(_T("%s"), (wcpHead + strEraseInfo.GetLength()));
					this->Show_Status(_T("Verifying connection..."));
					this->Show_Progress(0.3);
				}
				strEraseInfo.Format(_T("performing full chip erase"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), strEraseInfo);
				if (NULL != wcpHead)
				{
					csTotal_Info.Format(_T("%s"), (wcpHead + strEraseInfo.GetLength()));
					this->Show_Status(_T("Performing full chip erase..."));
					this->Show_Progress(0.6);
				}

				strEraseInfo.Format(_T("erase complete"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("erase complete"));
				if (NULL != wcpHead)
				{
					//csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					this->Show_Status(_T("Erase complete..."));
					this->Show_Out(_T("Erase complete..."));
					this->Show_Progress(1);
					bDownloading_Flag = true;//���
					break;
				}
			}
			//
			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 30)//��ʱ����fail
			{
				//����쳣��ʹ�����ذ�ť����
				this->m_csError_Code = _T("Erase Timeout");
				this->Show_Out(this->m_csError_Code);
				throw(4);
			}				

		}
	}
	catch(...)
	{
		m_DL_FLAG = false;//�˿�����ȡ�� ���ڿ���̬

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
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);

		//�ر��ļ�

		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		this->Show_Out(_T("Fail,ERASE_FLS_FILE_IN_EXE ----2"));


		return false;
	}
	//������ʱ
	time(&t_End);

	m_DL_FLAG = false;//�˿�������� �ص�����̬

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

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	//�ر��ļ�
	if(hFile_Handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile_Handle);
	}

	this->Show_Out(_T("Pass,ERASE_FLS_FILE_IN_EXE ----1"));

	return true;
}

//------------------------------------------------------------------
//��˼120ƽ̨���غ���
int CARM_OP_HUAWEI::Do_Execute_BC95_120()
{

	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpData = NULL;
	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;
	wchar_t * wcpHeadExt = NULL;

	wchar_t * wcpCmd = NULL;

	char * cpRead_Buff = NULL;

	time_t t_Start, t_End;

	int iTemp;

	bool bStatusFlag =false;
	bool bDownloading_Flag = false;
	bool bDC_Reset =false;
	bool bEraseFlag = false;

	BOOL bResetDC = TRUE;
	BOOL bDeDlflag = FALSE;
	BOOL bDeDlflagExt = FALSE;
	
	CString strClassIdx,strBatPath,strDis;
	CString  info1,info2,strDisI,strDinfo;
	CString strPos,strInfoDis;
	CString csTotal_Info, csData;

	double pos=0.0;

	unsigned long ulActual_Read;

	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;

	strClassIdx.Format(_T("%d"),m_iClass_Idx);

	m_iPpc=0;
	m_iPsc=0;
	m_iPac=0;

	m_iPsuc=0;
	m_iPauc=0;
	m_iPgc=0;


	iPsca_num=0;
	iPpa_num=0;
	iPsc_num=0;
	iPac_num=0;
	iSgc_num=0;

	//����ԭ�ȵĺ�˼��120ƽ̨������

	int iTimeInterval= m_iTimeInterval;

	if (0 == iTimeInterval)
	{
		iTimeInterval = 4000;//20190613 ��˼120ƽ̨һ����Ҫ��2��3����ϵ縴λ����ʱ
	}

		
	if (1 == m_iBC95_EraseSwitch)//�ⲿ���ؿ����Ƿ���Ҫ����
	{
#if 1	
		//�������µ�
		if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
		{
			this->Show_Out(_T("�ϵ�(���Ұ汾ǰ)"));
			::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
		}
		else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�SPC
		{
			csData.Format(_T("SPC�����ϵ�-CH%d"),this->m_iClass_Idx);

			this->Show_Out(csData);

			this->Set_PowerOn_SPC(this->m_iClass_Idx);
		}
		else
		{
			//����ǰ�ϵ�
			if(true == this->m_bPLC_PowFlag)
			{
				this->Show_Out(_T("�ϵ�(���Ұ汾ǰ)"));
				this->Send_Open_PLC_Mode_Cmd();
				Sleep(3000);
			}

		}

		this->Show_Status(_T("Read FW Version......"));
		this->Show_Out(_T("Read FW Version......"));

		//***************************************************************
		//�򿪶˿�
		bStatusFlag = this->NB_Port_Open(9600);
		if (false == bStatusFlag)
		{
			csData.Format(_T("Port(%d) Open Fail-Read FW check"),m_iCom_Port);

			this->Show_Status(csData);	
			//������Ϣ
			this->m_csError_Code.Format(_T("%s"),csData);
			this->m_iError_Code= ERR_OPEN_PORT_FAIL;

			csData.Format(_T("%s"),csData);
			this->Show_Out(csData);
			//throw(0);
		}
		else
		{
			m_iFindVerFlag = this->FindVer(m_iClass_Idx);

			if (1 == this->m_iFindVerFlag)//�����汾
			{		
				this->Show_Status(_T("��ʼ����......"));
				this->Show_Out(_T("��ʼ����......"));

				bEraseFlag = this->ERASE_FLS_FILE_IN_EXE(m_iClass_Idx);

				Sleep(500);
				this->Show_Status(_T("��������......"));
				this->Show_Out(_T("��ʼ����......"));
			}
			else
			{
				this->Show_Out(_T("δ�ҵ��汾����Ҫ����"));
			}

			this->NB_Port_Close();

		}
#endif
	}

#if 1
	if(TRUE == m_bCheckSaveLog)
	{
		//����־�ļ�

		CString strLogPath = m_csCurr_Exe_Path + "\\NB-IoT\\" + strClassIdx + "\\log.txt";	

		hFile_Handle = ::CreateFile(strLogPath,
			GENERIC_WRITE,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			CREATE_ALWAYS,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			CString info = strLogPath+" open failure";
			this->Show_Status(info);
			AfxMessageBox(info);
			return RTN_FAIL;
		}
	}
#endif
	try  
	{
		//��ʼ��ʾ
		this->Show_Status(_T("Waiting......"));


		if (0 == m_iBC95_EraseSwitch)
		{
			//�������µ�
			if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
			{
				this->Show_Out(_T("�ϵ�(���Ұ汾ǰ)"));
				::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
			}
			else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�SPC
			{
				csData.Format(_T("�ϵ�(���Ұ汾ǰ)-CH%d"),this->m_iClass_Idx);

				this->Show_Out(csData);

				this->Set_PowerOn_SPC(this->m_iClass_Idx);
			}
			else
			{
				//����ǰ�ϵ�
				if(true == this->m_bPLC_PowFlag)
				{
					this->Show_Out(_T("�ϵ�(���Ұ汾ǰ)"));
					this->Send_Open_PLC_Mode_Cmd();
					Sleep(3000);
				}

			}
		}

		//��ʼ��ʱ
		time(&t_Start);

		//��ʼ���������е���ز���
		this->m_bRuning = true;
		this->m_csError_Code.Format(_T(""));

		//------------------------------ ��鴮���Ƿ���� ------------------------------ 
		if(this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iCom_Port) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Confirm com port %d fail!"), this->m_iCom_Port);
			this->m_iError_Code=ERR_CONFIRM_PORT;

			this->Show_Status(this->m_csError_Code);
			throw(1);
		}

		//��鴮���Ƿ�ɴ� 20180227
		this->m_cla_Com_Port.m_iPort = this->m_iCom_Port;
		this->m_cla_Com_Port.m_iBaudRate = 9600;

		if(this->m_cla_Com_Port.Port_Open(400))
		{
			this->m_cla_Com_Port.Port_Close();
		}
		else
		{
			//������Ϣ
			this->m_csError_Code.Format(_T("FAIL, Port-%d In Using!"), this->m_iCom_Port);
			
			this->m_iError_Code = ERR_OPEN_PORT_FAIL;

			this->Show_Status(this->m_csError_Code);

			this->m_cla_Com_Port.Port_Close();
			throw(1);
		}
		
		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			AfxMessageBox(_T("Create pipe fail"));

			this->m_csError_Code.Format(_T("Create pipe fail"));
			this->m_iError_Code = ERR_CREATE_PIPE_FAIL;
			throw(1);

		}
		//------------------------------ 
		csTotal_Info.Format(_T(""));	

		//------------------------------ ���dosָ�� ------------------------------ 
		CString strCom;
		strCom.Format(_T("COM%d"),m_iCom_Port);

		CString strFirmwarePath = m_str_fwpkg_FW.cs_Path;
		CString strUEUpdaterPath;

		CString strPow;

		CString strExeName = _T("");
		switch(m_iClass_Idx)
		{
			case 1: strExeName = _T("UEUpdater1.exe"); break;
			case 2: strExeName = _T("UEUpdater2.exe"); break;
			case 3: strExeName = _T("UEUpdater3.exe"); break;
			case 4: strExeName = _T("UEUpdater4.exe"); break;
			default: strExeName = _T("UEUpdater.exe"); break;
		}

		strUEUpdaterPath.Format(_T("%s\\NB-IoT\\%d\\UEUpdaterCLI\\%s"),m_csCurr_Exe_Path,m_iClass_Idx,strExeName);
		//UEUpdater.exe����ļ��Ƿ����
		unsigned long ul_File_Size =0;
		if(this->Get_File_Size(strUEUpdaterPath, &ul_File_Size) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, UEUpdater.exe File Not Find!"), this->m_iCom_Port);
			this->m_iError_Code = ERR_FIND_UEUPDATE_EXE_FAIL;

			this->Show_Status(this->m_csError_Code);
			throw(1);
		}

		if (strFirmwarePath.Find(_T('\ '))>=0)
		{
			this->m_csError_Code.Format(_T("FAIL,Firmware ·���пո�!"), this->m_iCom_Port);
			this->m_iError_Code = ERR_FIND_PATH_HAS_SPACE;

			this->Show_Status(this->m_csError_Code);
			throw(1);
		}

		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 4096);
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (strFirmwarePath.GetLength() +strUEUpdaterPath.GetLength() + 128));
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 4096);

		CString strDlFlag = _T("updateall");

// 		CString strDlFlag = _T("updatecores");
// 		if (m_iUpdateall == 1)
// 		{
/*			strDlFlag = _T("updateall");*/
// 		}
// 		else
// 		{
// 			strDlFlag = _T("updatecores");
// 		}

		//UEUpdater.exe updateall --in B650.fwpkg �C-port com3
		if (m_iBC95BaudSwitch == 0) //Ĭ��֧�ָ��ٲ�����912600�������л�
 		{
 			swprintf(wcpCmd, _T("cmd.exe %cc \"%s updateall \-\-in %s \-\-port %s\""), '/', strUEUpdaterPath,strFirmwarePath,strCom);
 		}
 		else
		{
			swprintf(wcpCmd, _T("cmd.exe %cc \"%s %s \-\-in %s \-\-port %s \-\-noBaudSwitch \-\-passive\""), '/', strUEUpdaterPath,strDlFlag,strFirmwarePath,strCom);
		}

		//--------------------����Firmware ��������
		if(TRUE == m_bCheckSaveLog)
		{
			int iTemp;
			char* cp_Buff=NULL;
			unsigned long dwNumb=0; 
			//�����ڴ�
			iTemp = (strFirmwarePath.GetLength() +strUEUpdaterPath.GetLength() + 128)*2+8;
			this->m_cla_GFC.BUFF_CHAR_CALLOC(&cp_Buff, iTemp);
			//��������
			this->m_cla_GFC.WideChar_To_MultiByte(wcpCmd, cp_Buff, &iTemp);

			if(::WriteFile(hFile_Handle, cp_Buff, iTemp, &dwNumb, NULL) != TRUE)
			{
				AfxMessageBox(_T("log file Write fail!"));
				this->m_cla_GFC.BUFF_CHAR_FREE(&cp_Buff);
				throw(1);
			}
			this->m_cla_GFC.BUFF_CHAR_FREE(&cp_Buff);
		}
		//-------------------


		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����

		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			AfxMessageBox(_T("Create process fail!"));

			this->m_csError_Code.Format(_T("Create process fail"));
			this->m_iError_Code = ERR_CREATE_PROCESS_FAIL;

			throw(1);
		}
		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 
		//------------------------------ ѭ����ȡprocess�Ĵ�����Ϣ ------------------------------ 
		this->Show_Status(_T("DL Start!"));
		this->Show_Progress(0.0);

		//����
		csTotal_Info.Format(_T(""));
		while(false == bDownloading_Flag) 
		{
			//��info
			memset(cpRead_Buff, 0, 4096);
			ulActual_Read = 0;

			//������
			if(::ReadFile(hRead_Pipe, cpRead_Buff, 4095, &ulActual_Read, NULL) == NULL)
			{
				bDownloading_Flag = true;//20190626
			}

			if(TRUE == m_bCheckSaveLog)
			{
				//дͷ����
				unsigned long dwNumb=0;  
				if(::WriteFile(hFile_Handle, cpRead_Buff, ulActual_Read, &dwNumb, NULL) != TRUE)
				{
					this->Show_Out(_T("log file Write fail"));
				}
			}
	
			//����info������
			if(ulActual_Read > 0)
			{
				CString strNum,strInfo;

				m_DL_FLAG=	TRUE;

				//�ַ�ת��
				memset(wcpData, 0, 4096);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				//��������
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
				//Please reset the UE
				strDinfo.Format(_T("trying to reset ue using at command"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), strDinfo);

				if (NULL != wcpHead)
				{
					this->Show_Status(_T("Please wait for a few minutes"));
					this->Show_Progress(0.1);
				}
				else
				{
					//Trying to reset UE using bootloader commands...
					wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("trying to reset ue using bootloader commands"));
					if (NULL == wcpHead)
					{
						//Waiting for bootloader to send info message...
						wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("waiting for bootloader"));
						if (NULL != wcpHead)
						{
							this->Show_Status(_T("Waiting for bootloader to send info message..."));
							//EVB�����µ�һ���ؼ���Ϣ
							bDeDlflag = TRUE;
							this->Show_Progress(0.1);
						}
						else
						{
							//Unable to verify bootloader connection - try resetting again...
							wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("try resetting again"));
							if (NULL != wcpHead)
							{
								this->Show_Status(_T("Unable to verify bootloader connection - try resetting again..."));

								//EVB�����¿��ܵڶ����ؼ���Ϣ
								bDeDlflagExt = TRUE;
								this->Show_Progress(0.1);
							}
						}
					}
					else
					{
						this->Show_Status(_T("Trying to reset ue using BootLoader commands"));
						this->Show_Out(_T("Trying to reset ue using BootLoader commands"));
						//ģ��������������µ�һ���ؼ���Ϣ
						bDeDlflag = TRUE;
						this->Show_Progress(0.1);
					}					
				}		
				if (NULL != wcpHead)
				{
					//m_iInfoDis++;
					this->Show_Progress(0.1);
					csTotal_Info.Format(_T("%s"), (wcpHead + 10));

					//if (1 < m_iInfoDis[iClass_Idx-1]) //��һ����ʾ���ڶ��λ��Զ�������������
					if((TRUE ==bDeDlflag) ||(TRUE ==bDeDlflagExt))
					{		
						// ģ��������ȴ�
						if(TRUE ==bDeDlflag)
						{
							this->Show_Status(_T("Module Reset"));
						}
						else
						{
							this->Show_Status(_T("Module Reset Again"));
						}

						if(TRUE == m_bPowFlag)
						{
// 							ClosePower(iClass_Idx);//�رյ�Դ	
// 							Sleep(4000);
// 							OpenPower(iClass_Idx);//�򿪵�Դ
// 							Sleep(500);

						}
						else
						{		
							if(TRUE ==bDeDlflag)
							{
								this->Show_Status(_T("Module Reset By Hand"));

							}
							else
							{
								this->Show_Status(_T("Module Reset By Hand Again"));
							}			
						}
						

						if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
						{
							if ((m_iBC95_DC_ResetTime==1)
								&&(m_iBC95_DC_ResetTime_Flag == FALSE))
							{
								strPow.Format(_T("Module Reset2(״̬:%d:ͨ��:%d-TS:%d-��ʼ)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
								this->Show_Status(strPow);
								this->Show_Out(strPow);

								m_iBC95_DC_ResetTime_Flag = TRUE;
#ifdef FW_MULTI							
								//���ܿ�PC����DC����ָ��
								//bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("REON") );
								bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
								Sleep(iTimeInterval);
								bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );

#endif
								strPow.Format(_T("Module Reset2(״̬:%d:ͨ��:%d-TS:%d-���)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
								this->Show_Status(strPow);
								this->Show_Out(strPow);
							} 
							else
							{
								strPow.Format(_T("Module Reset2(״̬:%d:ͨ��:%d-TS:%d-�Ѹ�λ,��ȴ�)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
								this->Show_Status(strPow);
								this->Show_Out(strPow);
								//��DC��һ������
								if (TRUE == bResetDC)
								{
									bResetDC = FALSE;

									strPow.Format(_T("Module Reset2(״̬:%d:ͨ��:%d-TS:%d-����2)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
									this->Show_Status(strPow);
									this->Show_Out(strPow);
#ifdef FW_MULTI
									bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
									Sleep(iTimeInterval);
									bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
#endif
									strPow.Format(_T("Module Reset2(״̬:%d:ͨ��:%d-TS:%d-���2)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
									this->Show_Out(strPow);
								}
							}
						}
						else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�SPC
						{
							csData.Format(_T("Module Reset-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}
						else
						{
							//�������µ�
							strPow.Format(_T("Module Reset By Hand"));
							this->Show_Status(strPow);
							this->Show_Out(strPow);
						}

						bDeDlflag = FALSE;//ʧЧ
						bDeDlflagExt= FALSE;//ʧЧ
					}
					//goto start;

				}
				//-------------------------------------------------------------
				//-------------------------------------------------------------
				strDinfo.Format(_T("initial bootinfo"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(),strDinfo);
				if (NULL != wcpHead)
				{
					info2.Format(_T("Initial bootInfo"));
					this->Show_Status(info2);
					this->Show_Progress(0.1);
					csTotal_Info.Format(_T("%s"), (wcpHead + info2.GetLength()));
					//Save_Trace_Log_File(strInfoDis);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("programming securitya core"));
				//��Լ30��
				if (NULL != wcpHead)
				{

					info2.Format(_T("Programming SecurityA core"));
					wcpHead+= info2.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					iPsca_num++;
					if (iPsca_num<30)
					{
						pos=(double)iPsca_num/30;
					} 
					else
					{
						pos=1;
					}
					this->Show_Progress(pos);

					strPos.Format(_T("Programming SecurityA core......"));
					this->Show_Status(strPos);
					//Save_Trace_Log_File(strInfoDis);
				}

				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("programming protocola core"));
				//��Լ200��
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Programming ProtocolA core"));

					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					iPpa_num++;
					if (iPpa_num<200)
					{
						pos=(double)iPpa_num/200;
					} 
					else
					{
						pos=1;
					}
					this->Show_Progress(pos);
					strPos.Format(_T("Programming ProtocolA core......"));
					this->Show_Status(strPos);
					//Save_Trace_Log_File(strInfoDis);
				}
				//15��
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("programming securityb core"));				
				if (NULL != wcpHead)
				{			
					strInfoDis.Format(_T("Programming SecurityB core"));
					wcpHead+=strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					iPsc_num++;
					if (iPsc_num<15)
					{
						pos=(double)iPsc_num/15;
					} 
					else
					{
						pos=1;
					}
					this->Show_Progress(pos);
					strPos.Format(_T("Programming SecurityB core......"));
					this->Show_Status(strPos);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("programming applicationa core"));
				//33��				
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Programming ApplicationA core"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					iPac_num++;
					if (iPac_num<33)
					{
						pos=(double)iPac_num/33;
					} 
					else
					{
						pos=1;
					}
					this->Show_Progress(pos);
					strPos.Format(_T("Programming ApplicationA core......"));
					this->Show_Status(strPos);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("setting front-end configuration"));		
				//4��
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Setting front-end configuration"));
					this->Show_Progress(0.7);
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					this->Show_Status(_T("Setting front-end configuration......"));
					//Save_Trace_Log_File(strInfoDis);

				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("setting general configuration"));				
				//20��
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Setting general configuration"));
					wcpHead+=  strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					iSgc_num++;
					if (iSgc_num<20)
					{
						pos=(double)iSgc_num/20;
					} 
					else
					{
						pos=1;
					}
					this->Show_Progress(pos);
					strPos.Format(_T("Setting general configuration......"));
					this->Show_Status(strPos);
					//Save_Trace_Log_File(strInfoDis);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("sorry"));
				if (NULL != wcpHead)
				{
					//csTotal_Info.Format(_T("%s"), (wcpHead + 1));

					//
					this->m_csError_Code.Format(_T("FAIL,FW DL FAIL!"));
					this->m_iError_Code = ERR_FW_DL_FAIL;

					this->Show_Status(this->m_csError_Code);
					this->Show_Out(this->m_csError_Code);
					//���Ӷ�ʵ��ԭ��Ĵ�ӡ
					this->Show_Out(csTotal_Info);

					this->Show_Progress(1.0);

					bDownloading_Flag =true;
					//Save_Trace_Log_File(strInfoDis);
					throw(1);
				}				 
				//wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("application finished"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("update complete"));
				if (NULL != wcpHead)
				{
					this->Show_Status(_T("DownLoad PASS"));
					
					this->Show_Out(_T("DownLoad PASS"));

					this->Show_Progress(1.0);

					bDownloading_Flag = true;//���
					break;
				}
				//------------------------------
			}

			//
			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 300)//��ʱ����fail
			{
				//����쳣��ʹ�����ذ�ť����
				this->m_csError_Code.Format(_T("FW Download Time Out"));
				this->m_iError_Code = ERR_FW_DL_TIMEOUT;

				this->Show_Out(_T("FW Download Time Out"));
				throw(4);
			}				

		}
	}

	catch(...)
	{
		time(&t_End);

		m_DL_FLAG = FALSE;//�˿�����ȡ�� ���ڿ���̬

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
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);

		//�ر��ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//
		this->m_bRuning = false;
		this->Show_Out(_T("DownLoad FAIL"));

		return RTN_FAIL;
	}
	//������ʱ
	time(&t_End);

	m_DL_FLAG= FALSE;//�˿�������� �ص�����̬

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

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);


	//�ر��ļ�
	if(hFile_Handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile_Handle);
	}


	if(TRUE == m_bPowFlag)
	{
		//ClosePower(iClass_Idx);//�رյ�Դ	
	}

	//
	this->m_bRuning = false;

	this->Show_Out(_T("DownLoad PASS"));

	return RTN_SUCCESS;
}

int CARM_OP_HUAWEI::Do_Execute()
{
	int iDl_Res =RTN_FAIL;

	switch(m_iBBchip)
	{
	case BB_HI2152:
		iDl_Res = Do_Execute_HI2152();

		break;

	case BB_HI9500:
		iDl_Res = Do_Execute_HI2152_5G();

		break;

	case BB_HI2110:
		iDl_Res = Do_Execute_HI2110();

		break;
    //��˼B200ƽ̨
	case BB_HI2120:
		iDl_Res = Do_Execute_HI2120();

		break;
		
	default:break;
	}
	return iDl_Res;
}



int CARM_OP_HUAWEI::Do_Execute_HI2110()
{
	CString  csData;
	int iDl_Res =RTN_FAIL;

	//��ʼ��
	m_iBC95_DC_ResetTime_Flag= FALSE;
	m_iFindVerFlag =0;
	m_iPpc=0;
	m_iPsc=0;
	m_iPac=0;
	m_iPsuc=0;
	m_iPauc=0;
	m_iPgc=0;

	csData.Format(_T("DL Begin...(CH%d-TS%d)"),m_iClass_Idx,m_Ts_Num);

	this->Show_Out(csData);

	if ( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95D_INFO)>=0)
	{
		m_iFindFlagBCG_BC95D =1;
	} 
	else
	{
		m_iFindFlagBCG_BC95D =0;
	}

	//���
	this->m_csError_Code.Format(_T(""));
	this->m_iError_Code=0;
	
   //---��˼150ƽ̨����--------------------------------------------------------------
	if(( m_str_fwpkg_FW.cs_Path.Find(VERSION_G_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC28_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC68_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC35_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95D_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B5JB_INFO)>=0)	
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B8JB_INFO)>=0)		
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B5JC_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B8JC_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC39_INFO)>=0)

		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B5JD_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B8JD_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC35GJD_INFO)>=0)

		)
	{
		//���ô�������SSB.BIN,Ȼ��������Firmware
		this->m_iDL_flag =0;//��ʼ��
		this->m_bRuning = true;//�Ƶ�ǰ��

		bool bBootFlag = this->m_bBootFlag;//�����������ļ��в��� 20190611
		int  iBC95_EraseSwitch = this->m_iBC95_EraseSwitch;
		bool  bNetFlagDL = m_bNetFlagDL;

		//**************************************************
		this->Show_Out(_T("DownLoad--1"));
		this->m_bBootFlag =false;//��һ������������SSB

		if (FALSE == m_bOutLine)//����
		{
			//20191017
			if ((true == m_bIsRepMo) && (0 == m_iREP_NO_SSB))////����ƬREP����
			{
				this->m_iBC95_EraseSwitch = 1;
			}
			else
			{
				this->m_iBC95_EraseSwitch = 0; //��һ������������Ұ汾����
			}
		}
		else
		{
			//���ߵİ��տ�����
		}

		

		iDl_Res = this->Do_Execute_BC95G();

		this->m_bBootFlag = bBootFlag;//������ԭ
		this->m_iBC95_EraseSwitch = iBC95_EraseSwitch;//������ԭ
		//**************************************************


		//20190404 ���SSB���سɹ�����������ʧ��
		//�����£�SSB����ʧ��
		//----�ٴ����صڶ���
		if (RTN_FAIL == iDl_Res)
		{
			if ((2 == this->m_iDL_flag)  //SSB ����ʧ��
				||(3 ==this->m_iDL_flag))//SSB ���سɹ�,��������ʧ��
			{
				//**************************************************
				this->Show_Out(_T("DownLoad--2"));
				//m_bNetFlagDL = TRUE;//������Ϊ�Ѿ����ع�	
				//�����Ҫ����SSB���

				//���
				this->m_csError_Code.Format(_T(""));
				this->m_iError_Code=0;

				iDl_Res = this->Do_Execute_BC95G();
				//m_bNetFlagDL = bNetFlagDL;//������ԭ
				//**************************************************

				//����һ�����ز���-----��ֹǰ��һ��SSB���ʧ�� 20190603
				if (RTN_FAIL == iDl_Res)
				{
					//**************************************************
					this->Show_Out(_T("DownLoad--3"));

					this->m_bNetFlagDL = TRUE;//������Ϊ�Ѿ����ع�
					this->m_iBC95_EraseSwitch = 0; //������Ҳ�������
					this->m_bBootFlag =false;//������Ҳ����SSB�����߼��SSB

					this->m_iFindVerFlag =1;//������Ϊ�汾�ɲ��

					//���
					this->m_csError_Code.Format(_T(""));
					this->m_iError_Code=0;
					
					iDl_Res = this->Do_Execute_BC95G();

					this->m_bNetFlagDL = bNetFlagDL;//������ԭ
					this->m_bBootFlag = bBootFlag;//������ԭ
					this->m_iBC95_EraseSwitch = iBC95_EraseSwitch;//������ԭ
					//**************************************************
				}
			}
			else
			{
				//...
				csData.Format(_T("DownLoad Fail---->%d"),this->m_iDL_flag);
				this->Show_Out(csData);
			}
		}
		else
		{
			//...
			csData.Format(_T("DownLoad Firmware Pass"));
			this->Show_Out(csData);
		}

		//������ϵ�
		if (LOAD_AUTO_ROBOT != m_iOP_Robot) //�ֶ���
		{
			if( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC39_INFO)<0)
			{
				if(true == this->m_bPLC_PowFlag)
				{
					csData.Format(_T("14-CH%d DC OFF"),m_iClass_Idx);
					this->Show_Out(csData);
					this->Send_Close_PLC_Mode_Cmd();
					Sleep(2000);
				}
			}

		}
		this->m_bRuning = false;	
	}
	//-----------------------------------------------------------------------
	else
	{
		//����ԭ�ȵĺ�˼��120ƽ̨������
		iDl_Res = this->Do_Execute_BC95_120();
	}

	return iDl_Res;
}

void CARM_OP_HUAWEI::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_HUAWEI::Show_Progress(double d_Pgr_Percent)
{
	//�˺�����Ӧ�ð����������ٷֱȵļ��㣬Ȼ������Ϣ��Ӧ��������ɾ��岽�������ļ���
	//Progress�ܳ�����1000
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}
bool CARM_OP_HUAWEI::LOAD_FW_FILE_HI2110(const wchar_t * wcp_FW_File_Path)
{

	//�ж��� NB FW ·�����ж��ٿ���FW�ļ�
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir;
	CFileFind cla_Finder; //�ļ�������
	BOOL b_Found;
	int j=0;
	wchar_t * wcprFW_Name[] = {_T("FWPKG")};

	try
	{
		//--------------------------------------- ��ʼ������Ϣ�����������ļ� --------------------------------------- 
		//���Intel�������ļ��б�
		this->m_str_fwpkg_FW.cs_Name.Format(_T(""));
		this->m_str_fwpkg_FW.cs_Path.Format(_T(""));

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
				CString strFW_File_Path = wcp_FW_File_Path;
				strFW_File_Path.MakeUpper();

				
				//�ж��ļ����ͣ�����.fls�ļ�
				//if(wcsstr(cs_FW_Name, _T(".FWPKG")) != NULL)
				if(wcsstr(strFW_File_Path,cs_FW_Name) != NULL)

				{
					//ȥ���ļ�������ʾ
					cs_FW_Name.Replace(_T(".FWPKG"), _T("\0"));

					m_str_fwpkg_FW.cs_Name.Format(_T("%s"), cs_FW_Name);

					m_str_fwpkg_FW.cs_Path.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

					m_str_fwpkg_FW.b_Enable_DL = true;
				}//if(wcsstr(cs_FW_Name, _T(".fwpkg")) != NULL
			}
		}//while(b_Found

		//
		cla_Finder.Close();
		//

		if(m_str_fwpkg_FW.b_Enable_DL == false)
		{
			this->m_csError_Code.Format(_T("ERROR, LOAD_FW_FILE, There is no %s.file!"), wcprFW_Name[j]);
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

bool CARM_OP_HUAWEI::LOAD_FW_FILE_HI2152(const wchar_t * wcp_FW_File_Path)
{
	//�ж��� LTE FW ·�����ж��ٿ���FW�ļ�
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Dir;
	CFileFind cla_Finder; //�ļ�������
	BOOL b_Found;
	int j=0;

	try
	{
	    //20200428  ����Ҫ����BAT�ļ���ֻҪ��batͬһ���ļ��еĶ�����
		//
		//����������ļ��б�
		for(j=0; j<FB_MAX; j++)
		{
			this->m_strr_Img_File_List[j].cs_Name.Format(_T(""));
			this->m_strr_Img_File_List[j].cs_Path.Format(_T(""));
			this->m_strr_Img_File_List[j].b_Enable_DL = false;
		}


		//��ȡ���·��
		if(this->m_cla_GFC.Get_File_Directory(wcp_FW_File_Path, &cs_FW_Dir) == false)
		{
			this->m_csError_Code.Format(_T("Error, Get mbn file directory fail!"));
			throw(0);
		}

		this->m_strUsbLoadPath = cs_FW_Dir;

		cs_Finder_Dir.Format(_T("%s\\*"), cs_FW_Dir);

		b_Found = cla_Finder.FindFile(cs_Finder_Dir);

		j=0;

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
				if (j<FB_MAX)
				{
					this->m_strr_Img_File_List[j].cs_Name.Format(_T("%s"),cs_FW_Name);
					this->m_strr_Img_File_List[j].cs_Path.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());
					this->m_strr_Img_File_List[j].b_Enable_DL = true;
					j++;
				}
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

bool CARM_OP_HUAWEI::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	bool bRet = false;

	switch(m_iBBchip)
	{
		case BB_HI2152:
		case BB_HI9500:
			bRet = LOAD_FW_FILE_HI2152(wcp_FW_File_Path);

			break;
		case BB_HI2110:
		case BB_HI2120:
			bRet = LOAD_FW_FILE_HI2110(wcp_FW_File_Path);

			break;
		default:break;
	}
	return bRet;
}

bool CARM_OP_HUAWEI::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
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


int CARM_OP_HUAWEI::Do_Execute_BC95G(void)
{
	int iDL_Result1(RTN_FAIL),iDL_Result1_COM(RTN_FAIL),iDL_SSB_Result(RTN_FAIL);

	CString strCommand1,strCommand1_Com,strInfo;
	CString csData;

	unsigned char* cpData =NULL;
	wchar_t * wcpData = NULL;

	//����iIndexΪ 1��2��3��4. ...
	int iIxIndex = this->m_iClass_Idx-1;

	int iBcg_ssb_com = m_iBcg_ssb_com;//ͨ�������ļ����� �Ƿ���Ҫ����SSB

	bool bCheckSSB_Flag = false;
	bool bErase_Flag = false;
	bool bStatusFlag =false;

	m_i150ResetTime1=0;//150ƽ̨����SSB��λ1
	m_i150ResetTime2=0;//150ƽ̨����SSB��λ2
	m_i150ResetTime3=0;//150ƽ̨����FW��λ1
	m_i150ResetTime4=0;//150ƽ̨����FW��λ2

	CString strExeName = _T("");
	CString strSSB_ExeName = _T("");
	bool bFindExe = false;

	switch(m_iClass_Idx)
	{
		case 1: strExeName = _T("UEUpdater1.exe"); break;
		case 2: strExeName = _T("UEUpdater2.exe"); break;
		case 3: strExeName = _T("UEUpdater3.exe"); break;
		case 4: strExeName = _T("UEUpdater4.exe"); break;
		default:
			{
				AfxMessageBox(_T("m_iClass_Idx ERR"));
				strSSB_ExeName = _T("UEUpdater.exe"); 
			}
			break;

	}

	switch(m_iClass_Idx)
	{
		case 1: strSSB_ExeName = _T("SsbLoader1.exe"); break;
		case 2: strSSB_ExeName = _T("SsbLoader2.exe"); break;
		case 3: strSSB_ExeName = _T("SsbLoader3.exe"); break;
		case 4: strSSB_ExeName = _T("SsbLoader4.exe"); break;
		default: 
			{
				AfxMessageBox(_T("m_iClass_Idx ERR"));
				strSSB_ExeName = _T("SsbLoader.exe"); 
			}
			break;
	}

	if (FALSE == m_bOutLine)//����
	{
		if ((true == m_bIsRepMo) && (0 == m_iREP_NO_SSB))////����ƬREP���� ////REP���� ����Ҫ���SSB,�϶���SSB
		{
			if (1 == m_iBC95_EraseSwitch)//�ⲿ���ؿ����Ƿ���Ҫ����
			{
				bErase_Flag = true;
			}
			else
			{
				//...
			}
			//
			bCheckSSB_Flag = false;//����Ҫ���ssb

		}

		if (TRUE == m_bNetFlagDL)//�����־λ>=DL //20190603 �����־λ�������ر�־λ�������ټ��SSB --------------
		{
			if (1 == m_iBC95_EraseSwitch)//�ⲿ���ؿ����Ƿ���Ҫ����
			{
				bErase_Flag = true;
			}
			else
			{
				//...
			}
			//
			bCheckSSB_Flag = false;//����Ҫ���ssb
			
		}
		else //
		{
			if(true == m_bBootFlag)//���SSB�Ŀ���
			{
				bCheckSSB_Flag = true;
			}
			else
			{
				//...
				bCheckSSB_Flag = false;
			}

			//...
			if (1 == m_iBC95_EraseSwitch)//�ⲿ���ؿ����Ƿ���Ҫ����
			{
				bErase_Flag = true;
			}
			else
			{
				//...
				bErase_Flag = false;
			}

		}
	} 
	else //����
	{
		if(true == m_bBootFlag)//���SSB�Ŀ���
		{
			bCheckSSB_Flag = true;
		}
		else
		{
			//...
			bCheckSSB_Flag = false;
		}
		if (1 == m_iBC95_EraseSwitch)//�ⲿ���ؿ����Ƿ���Ҫ����
		{
			bErase_Flag = true;
		}
		else
		{
			//...
			bErase_Flag = false;
		}
	}


	try
	{
		//����ǰ�ϵ� 
		if (LOAD_AUTO_ROBOT ==  m_iOP_Robot) //ȫ�Զ�
		{
			csData.Format(_T("1-�ϵ�(����ǰ)-TS%d-CH%d"),this->m_Ts_Num,this->m_iClass_Idx);

			this->Show_Out(csData);

			::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( this->m_Ts_Num, _T("ON") );
		}
		else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�
		{
			csData.Format(_T("1-SPC�ϵ�(����ǰ)-CH%d"),this->m_iClass_Idx);

			this->Show_Out(csData);

			this->Set_PowerOn_SPC(this->m_iClass_Idx);
		}
		else
		{
			if(true == this->m_bPLC_PowFlag)//PLC
			{
				//csData.Format(_T("1-�ϵ�(����ǰ)-CH%d"),this->m_iClass_Idx);

				//this->Show_Out(csData);

				//this->Send_Open_PLC_Mode_Cmd();//20191125 �Ƿ���Ҫ��������
				//Sleep(2000);

			}
		}

		this->Show_Status(_T("Waiting......(150)"));

		this->Show_Out(_T("Waiting......(150)"));

		//��ʼ���������е���ز���
		this->m_csError_Code.Format(_T(""));

		//------------------------------ ��鴮���Ƿ���� ------------------------------ 
		if(this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iCom_Port) == false)
		{
			//20190713
			//������Ϣ
			this->m_csError_Code.Format(_T("FAIL, Confirm com port %d fail!"), this->m_iCom_Port);
			this->m_iError_Code=ERR_CONFIRM_PORT;

			this->Show_Status(this->m_csError_Code);
			this->Show_Out(this->m_csError_Code);
			throw(0);
		}
		//��һ�λ������û��Boot���� Ȼ���ٹػ�
		//����Ƭ��ȥ���SSB��Щ 20190312

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//������������  ---------20200429
// 		bCheckSSB_Flag  = false;
// 		bErase_Flag = true;
// 		m_iFindVerFlag  = 1;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


		if(true == bCheckSSB_Flag)	
		{	
			this->Show_Status(_T("check SSB..."));
			this->Show_Out(_T("check SSB..."));
			//***************************************************************
			//�򿪶˿�
			g_SSB_Ctrl.Lock();

			bStatusFlag = this->NB_Port_Open(115200);
			if (false == bStatusFlag)
			{
				csData.Format(_T("Port(%d) Open Fail-SSB check"),m_iCom_Port);

				this->Show_Status(csData);	
				this->Show_Out(csData);
				//������Ϣ
				this->m_csError_Code.Format(_T("%s"),csData);
				this->m_iError_Code=ERR_OPEN_PORT_FAIL;

				g_SSB_Ctrl.Unlock();
				throw(0);
			}
			//Check SSB
			if(false == this->CheckSSB())
			{
				iBcg_ssb_com =1;//û�м�⵽SSB��Ϣ��������Ҫ����SSB
			}
			else
			{
				iBcg_ssb_com =0;//��⵽SSB��Ϣ����������Ҫ����SSB
			}
			//�رն˿�
			this->NB_Port_Close();

			g_SSB_Ctrl.Unlock();
			//***************************************************************
			
		}//(true == bCheckSSB_Flag) 
		else
		{
			//...
		}

		csData.Format(_T("BC95_First_Erase:%d"),m_iBC95_EraseSwitchFirst);
		this->Show_Out(csData);

		if ((true == bErase_Flag)//�Ƿ���Ҫ����
			||(1 == m_iBC95_EraseSwitchFirst))//�����ļ��еĴο��ش򿪾ͱ������
		{
//20191017 �������������ֱ�Ӳ���
			m_iFindVerFlag =1;//
#if 0
			this->Show_Status(_T("Read FW Version......"));
			this->Show_Out(_T("Read FW Version......"));

			//�򿪶˿�
			bStatusFlag = this->NB_Port_Open(9600);
			if (false == bStatusFlag)
			{
				csData.Format(_T("Port(%d) Open Fail-Erase check"),m_iCom_Port);

				this->Show_Status(csData);	
				//������Ϣ
				this->m_csError_Code.Format(_T("%s"),csData);
				this->m_iError_Code=ERR_OPEN_PORT_FAIL;

				csData.Format(_T("%s"),csData);
				this->Show_Out(csData);
				throw(0);
			}
			if (true ==  this->FindVerAuto())
			{
				m_iFindVerFlag =1;
				
			}
			else
			{
				m_iFindVerFlag =0;
				
			}
			//�رն˿�
			this->NB_Port_Close();
#endif

			//�������򿪶˿ڲ�ѯ�汾�رն˿ں�������������
			if (1 == m_iFindVerFlag)
			{
				this->Show_Status(_T("��ʼ����......"));
				this->Show_Out(_T("��ʼ����......"));

				this->ERASE_FLS_FILE_IN_EXE(m_iClass_Idx);

				//�����һ����û������UEUpdater1.exe

				bFindExe = m_cla_GFC.Find_Process( strExeName);
				if (true == bFindExe) //.exe �����ڣ������ȴ�
				{
					bFindExe = m_cla_GFC.Kill_Process(strExeName);
					if (false ==  bFindExe)
					{
						csData.Format(_T("%s Del FAIL"),strExeName);
						this->Show_Out(csData);
					} 
					else
					{
						csData.Format(_T("%s Del PASS"),strExeName);
						this->Show_Out(csData);
					}
				}

				this->Show_Status(_T("��������......"));
				this->Show_Out(_T("��������......"));
			} 
			else
			{
				this->Show_Out(_T("δ�ҵ��汾����Ҫ����"));
			}

		}//true == bErase_Flag
		else
		{
			//...
			this->Show_Out(_T("�������عرղ���Ҫ����"));

		}
		//����ܲ�ѯ��Firmware�ģ�����Ϊ��ģ���Ѿ���SSB������������SSB
		if (1 == m_iFindVerFlag)
		{
			iBcg_ssb_com =0;
		}

		strInfo.Format(_T("CH%d-\\(WrCon=%d-BaudSwitch=%d-Bcg_ssb_com=%d-WriteKV=%d-WriteFEC=%d\\)"),this->m_iClass_Idx,m_blWrCon,m_iBC95BaudSwitch,iBcg_ssb_com,m_bWriteKV,m_bWriteFEC);
		this->Show_Out(strInfo);

		//--WriteKV QFLASH ������Ҫ����

		if (m_blWrCon == TRUE)//��������������Ƶ������
		{
			if (m_iBC95BaudSwitch == 0) //Ĭ��֧�ָ��ٲ�����912600�������л�
			{
				strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updateall -p COM%d -i %s"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
			}
			else
			{
				strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updateall -p COM%d -i %s  --noBaudSwitch  --passive"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
			}
		}
		else
		{
			if ((TRUE ==  m_bWriteKV) && (TRUE ==  m_bWriteFEC))
			{
				if (m_iBC95BaudSwitch == 0) //Ĭ��֧�ָ��ٲ�����912600�������л�
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s --writeKV --writeFec"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
				else
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s  --noBaudSwitch  --passive --writeKV --writeFec"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
			}
			else if ((FALSE ==  m_bWriteKV) && (TRUE ==  m_bWriteFEC))
			{
				if (m_iBC95BaudSwitch == 0) //Ĭ��֧�ָ��ٲ�����912600�������л�
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s  --writeFec"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
				else
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s  --noBaudSwitch  --passive --writeFec"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
			}
			else if ((TRUE ==  m_bWriteKV) && (FALSE ==  m_bWriteFEC))
			{
				if (m_iBC95BaudSwitch == 0) //Ĭ��֧�ָ��ٲ�����912600�������л�
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s --writeKV"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
				else
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s  --noBaudSwitch  --passive --writeKV"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
			}
			else
			{
				if (m_iBC95BaudSwitch == 0) //Ĭ��֧�ָ��ٲ�����912600�������л�
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
				else
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s  --noBaudSwitch  --passive"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
			}
		}

		

		// 	//������Ƭ��ʽ����
		if(m_iFindFlagBCG_BC95D ==1)
		{
			strCommand1_Com.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\BC95_G\\SsbUpdater\\%s loadfirst -p COM%d -i %s\\NB-IoT\\%d\\BC95_G\\ssb-BC95-D.bin"),'/',m_csCurr_Exe_Path,m_iClass_Idx,strSSB_ExeName,this->m_iCom_Port,m_csCurr_Exe_Path,m_iClass_Idx);
		}
		else //BC95-G/BC35/BC68ϵ��
		{
			strCommand1_Com.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\BC95_G\\SsbUpdater\\%s loadfirst -p COM%d -i %s\\NB-IoT\\%d\\BC95_G\\ssb-BC95-G.bin"),'/',m_csCurr_Exe_Path,m_iClass_Idx,strSSB_ExeName,this->m_iCom_Port,m_csCurr_Exe_Path,m_iClass_Idx);
		}

		this->Show_Progress(0.0);

		// 	//����ǰ���ѯIMEI������
		// 	OnReadAtImei();

		//�Ƿ��ô�������SSB.BIN ----��Ҫ������Ƭ��Ҫ��������SSB.BIN
#if 1
		if (1 == iBcg_ssb_com)
		{	
			this->Show_Status(_T("DL SSB..."));
			this->Show_Out(_T("DL SSB..."));

			iDL_Result1_COM = DL_FLS_FILE_IN_EXE_BCG_COM(strCommand1_Com);//����SSB

			//���ܴ���̫�죬SSB.EXE��δ���ü��ͷ�
			Sleep(1000);


			bFindExe = m_cla_GFC.Find_Process( strSSB_ExeName);
			if (true == bFindExe) //.exe �����ڣ������ȴ�
			{
				bFindExe = m_cla_GFC.Kill_Process(strSSB_ExeName);
				if (false ==  bFindExe)
				{
					csData.Format(_T("%s Del ssb exe FAIL"),strSSB_ExeName);
					this->Show_Out(csData);
				} 
				else
				{
					csData.Format(_T("%s Del ssb exe PASS"),strSSB_ExeName);
					this->Show_Out(csData);
				}
			}

			bFindExe = m_cla_GFC.Find_Process( strExeName);
			if (true == bFindExe) //.exe �����ڣ������ȴ�
			{
				bFindExe = m_cla_GFC.Kill_Process(strExeName);
				if (false ==  bFindExe)
				{
					csData.Format(_T("%s Del FAIL"),strExeName);
					this->Show_Out(csData);
				} 
				else
				{
					csData.Format(_T("%s Del PASS"),strExeName);
					this->Show_Out(csData);
				}
			}

			if (RTN_SUCCESS == iDL_Result1_COM)
			{
				m_iDL_flag = 1;//SSB ���سɹ�

				this->Show_Out(_T("DL Firmware..."));
				this->Show_Status(_T("DL Firmware..."));
				this->Show_Out(strCommand1);

				iDL_Result1 = DL_FLS_FILE_IN_EXE_BCG(strCommand1);//����FW

				if (RTN_SUCCESS == iDL_Result1)
				{
					m_iDL_flag = 4;//SSB ���سɹ�,�����سɹ�
					m_PassNumb++;
					Sleep(2000);
				} 
				else
				{
					m_iDL_flag = 3;//SSB ���سɹ�,��������ʧ��				
					throw(1);
				}
				
			}
			else
			{
				m_iDL_flag = 2;//SSB ����ʧ��
				throw(2);
			}
		} 
		else
#endif
		{
			this->Show_Status(_T("DL Firmware..."));

			iDL_Result1 = DL_FLS_FILE_IN_EXE_BCG(strCommand1);//����FW
			if (RTN_SUCCESS == iDL_Result1)
			{
				m_PassNumb++;
			} 
			else
			{
				throw(3);
			}		 
			Sleep(2000);
			//
			bFindExe = m_cla_GFC.Find_Process( strSSB_ExeName);
			if (true == bFindExe) //.exe �����ڣ������ȴ�
			{
				bFindExe = m_cla_GFC.Kill_Process(strSSB_ExeName);
				if (false ==  bFindExe)
				{
					csData.Format(_T("%s Del ssb exe FAIL"),strSSB_ExeName);
					this->Show_Out(csData);
				} 
				else
				{
					csData.Format(_T("%s Del ssb exe PASS"),strSSB_ExeName);
					this->Show_Out(csData);
				}
			}

			bFindExe = m_cla_GFC.Find_Process( strExeName);
			if (true == bFindExe) //.exe �����ڣ������ȴ�
			{
				bFindExe = m_cla_GFC.Kill_Process(strExeName);
				if (false ==  bFindExe)
				{
					csData.Format(_T("%s Del FAIL"),strExeName);
					this->Show_Out(csData);
				} 
				else
				{
					csData.Format(_T("%s Del PASS"),strExeName);
					this->Show_Out(csData);
				}
			}


		}
	}
	catch (...)
	{

		this->m_DL_FLAG = false;

		this->m_ErrorNumb++;
		Sleep(2000);

		return RTN_FAIL;

	}
	return RTN_SUCCESS;

}

int CARM_OP_HUAWEI::DL_FLS_FILE_IN_EXE_BCG(CString strCommand)
{
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;

	unsigned long ulActual_Read;
	int iTemp;
	CString csTotal_Info, csData,strDataInfo;
	double dPercent = 0.0;
	time_t t_Start, t_End;
	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;

	wchar_t * wcpHead_Temp = NULL;

	bool bDownloading_Flag = false,bDC_Reset =false;

	CString strDis,strInfo,strPow;
	wchar_t * wcpCmd = NULL;

	int iPpa_num  =0,iPsc_num  =0,iPac_num =0,iSgc_num =0,iPsca_num=0;
	int pos =0;
	CString strPos,strInfoDis,strInfoDis_T;

	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;

	BOOL bLogSave = FALSE;

	CString strLogPath;

	time_t start, finish;

	time_t start_T, finish_T;

	int iTimeOut =150;//

#ifdef FW_MULTI
	iTimeOut =150;
#else
	iTimeOut =230; //��Ҫ�ǿ��ǿͻ��������ܲ����������أ����ܳ���150��
#endif

	try  
	{
		time( &start );

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			AfxMessageBox(_T("Create pipe fail"));

			this->m_csError_Code.Format(_T("Create pipe fail"));
			this->m_iError_Code = ERR_CREATE_PIPE_FAIL;

			throw(0);

		}
		//------------------------------ 
#ifndef FW_MULTI 
		if(TRUE == m_bCheckSaveLog)
		{
			//����־�ļ�

			strLogPath.Format(_T("%s\\NB-IoT\\%d\\log.txt"),m_csCurr_Exe_Path,m_iClass_Idx);

			hFile_Handle = ::CreateFile(strLogPath,
				GENERIC_WRITE,				//�ļ��Ĳ������� Read only required
				FILE_SHARE_READ,			//�ļ��������� read share file
				NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
				CREATE_ALWAYS,				//�ļ����� how to create
				FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
				NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
				);

			CString info;
			if(hFile_Handle == INVALID_HANDLE_VALUE)
			{
				info.Format(_T("%s open Fail"),strLogPath);
				this->Show_Status(info);
				this->Show_Out(info);
				bLogSave = FALSE;
			}
			else
			{

				info.Format(_T("%s open Pass"),strLogPath);
				this->Show_Out(info);

				bLogSave = TRUE;

			}
		}
		else
		{
			bLogSave = FALSE;
		}
#endif
		//---------------------------------------------------------

		csTotal_Info.Format(_T(""));
		strDataInfo.Format(_T(""));

		//------------------------------ ���dosָ�� ------------------------------ 

		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (strCommand.GetLength() + 128));

		swprintf(wcpCmd, _T("%s"), strCommand);

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��e
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��

		csData.Format(_T("FW:Write_Pipe=%d,hRead_Pipe=%d"),hWrite_Pipe,hRead_Pipe);
		this->Show_Out(csData);

		//
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			this->m_csError_Code.Format(_T("Create process fail"));
			this->m_iError_Code = ERR_CREATE_PROCESS_FAIL;


			AfxMessageBox(_T("Create process fail!"));
			throw(1);
		}
		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 
		//------------------------------ ѭ����ȡprocess�Ĵ�����Ϣ ------------------------------ 
		//��ʼ��ʱ
		time(&t_Start);
		while(false == bDownloading_Flag) 
		{
			//��info
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;

			//������
			if(::ReadFile(hRead_Pipe, cpRead_Buff, 2047, &ulActual_Read, NULL) == NULL)
			{
				bDownloading_Flag = true;//20190626
				
				this->Show_Status(_T("FW ReadFile NULL"));
				this->Show_Progress(1.0);

				this->Show_Out(_T("FW ReadFile NULL"));
				this->m_csError_Code.Format(_T("FW ReadFile NULL"));
				this->m_iError_Code = ERR_FW_ReadFile_NULL;	
				throw(0);
			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				this->Show_Status(_T("FW Download Time Out"));
				this->Show_Progress(1.0);

				this->Show_Out(_T("FW Download Time Out"));
				this->m_csError_Code.Format(_T("FW Download Time Out"));
				this->m_iError_Code = ERR_FW_DL_TIMEOUT;
				throw(0);
			}

			//����info������
			if(ulActual_Read > 0)
			{
				m_DL_FLAG = true;

				//�ַ�ת��
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));

				if(TRUE == bLogSave)
				{
					//дͷ����
					unsigned long dwNumb=0;  
					if(::WriteFile(hFile_Handle, cpRead_Buff, ulActual_Read, &dwNumb, NULL) != TRUE)
					{
						this->Show_Out(_T("log file Write fail"));
					}
				}
				else
				{
					//..
				}
				//��������
				csTotal_Info += csData;
				strDataInfo = csTotal_Info;

				//Waiting for bootloader to send info message
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Waiting for bootloader to send info message"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Waiting for bootloader to send info message"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));

					this->Show_Status(strInfoDis);
					this->Show_Out(strInfoDis);
					this->Show_Progress(0.1);

					if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
					{						
#ifdef FW_MULTI							
						//�����һ��û��Ӧ�������ڶ��θ�λ
						m_i150ResetTime3++;
						if(m_i150ResetTime3 ==2)
						{
							strPow.Format(_T("Module Reset2(FW1-״̬:%d:ͨ��:%d-TS:%d-��ʼ)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);

							csData.Format(_T("10-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(2000);

							csData.Format(_T("10-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );

							strPow.Format(_T("Module Reset2(FW1-״̬:%d:ͨ��:%d-TS:%d-���)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);
						}
#endif
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�SPC
					{
						m_i150ResetTime3++;
						if(m_i150ResetTime3 ==2)
						{
							csData.Format(_T("10-SPC�����ϵ�-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}
					}
					else
					{
						if(true == this->m_bPLC_PowFlag)
						{
							m_i150ResetTime3++;
							if(m_i150ResetTime3 ==2)
							{
								strPow.Format(_T("Module Reset2(FW1-״̬:%d:ͨ��:%d-TS:%d-��ʼ)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
								this->Show_Status(strPow);
								this->Show_Out(strPow);

								csData.Format(_T("10-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Close_PLC_Mode_Cmd();
								Sleep(2000);

								csData.Format(_T("10-CH%d DC 0N"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Open_PLC_Mode_Cmd();

								strPow.Format(_T("Module Reset2(FW1-״̬:%d:ͨ��:%d-TS:%d-���)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
								this->Show_Status(strPow);
								this->Show_Out(strPow);
							}
						}
						else
						{						
							m_i150ResetTime3++;
							if(m_i150ResetTime3 ==2)
							{
								this->Show_Status(_T("Reset"));
							}
						}
					}
				}
				//Trying to reset UE using bootloader commands
				//Trying to reset UE using AT command
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Trying to reset UE using bootloader commands"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Trying to reset UE using bootloader commands"));
					wcpHead+= strInfoDis.GetLength();

					csTotal_Info.Format(_T("%s"), (wcpHead + 1));

					this->Show_Status(strInfoDis);
					this->Show_Out(strInfoDis);
					this->Show_Progress(0.1);

					//���������µ�
					if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
					{
						strPow.Format(_T("Module Reset1(FW2״̬:%d:ͨ��:%d-TS:%d-��ʼ)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);
						this->Show_Out(strPow);
#ifdef FW_MULTI	
						//�����һ��û��Ӧ�������ڶ��θ�λ
						m_i150ResetTime4++;
						if(m_i150ResetTime4 ==2)
						{
							csData.Format(_T("11-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(2000);

							csData.Format(_T("11-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
						}
#endif
						strPow.Format(_T("Module Reset1(FW2״̬:%d:ͨ��:%d-TS:%d-���)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);
						this->Show_Out(strPow);
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�SPC
					{
						//�����һ��û��Ӧ�������ڶ��θ�λ
						m_i150ResetTime4++;
						if(m_i150ResetTime4 ==2)
						{
							csData.Format(_T("11-SPC�����ϵ�-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}					
					}
					else
					{
						//PLCģʽ�ݲ��������µ�
						if(true == this->m_bPLC_PowFlag)
						{
							m_i150ResetTime4++;
							if(m_i150ResetTime4 ==2)
							{
								csData.Format(_T("11-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Close_PLC_Mode_Cmd();
								Sleep(2000);

								csData.Format(_T("11-CH%d DC ON"),m_iClass_Idx);
								this->Show_Out(csData);
								this->Send_Open_PLC_Mode_Cmd();
							}
						}
						else
						{
							this->Show_Out(_T("PLC NOT SUPPORT(FW DL)"));
							
							//�����һ��û��Ӧ�������ڶ��θ�λ
							m_i150ResetTime4++;
							if(m_i150ResetTime4 ==2)
							{
								this->Show_Status(_T("Reset"));
							}				

						}
					}
				}
				//try resetting again
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("try resetting again"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("try resetting again"));
					wcpHead+= strInfoDis.GetLength();

					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					//Save_Trace_Log_File(_T("UE RESET"));
					this->Show_Status(strInfoDis);
					this->Show_Progress(0.1);

					this->Show_Out(strInfoDis);

					if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
					{
						csData.Format(_T("12-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
						this->Show_Out(csData);

						bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
						Sleep(2000);

						csData.Format(_T("12-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
						this->Show_Out(csData);

						bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�SPC
					{
						csData.Format(_T("12-SPC�����ϵ�-CH%d"),this->m_iClass_Idx);

						this->Show_Out(csData);

						this->Set_PowerOn_SPC(this->m_iClass_Idx);
					}
					else
					{
						if(true == this->m_bPLC_PowFlag)//���Զ�PLC���ؼо�
						{
							csData.Format(_T("12-CH%d DC OFF"),m_iClass_Idx);
							this->Show_Out(csData);

							this->Send_Close_PLC_Mode_Cmd();
							Sleep(2000);

							csData.Format(_T("12-CH%d DC ON"),m_iClass_Idx);
							this->Show_Out(csData);

							this->Send_Open_PLC_Mode_Cmd();
						}
						else
						{
							this->Show_Status(_T("Reset..."));
						}
					}
					continue;

				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Initial bootInfo"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Initial bootInfo"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					this->Show_Status(strInfoDis);
					this->Show_Progress(0.12);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming SecurityA core"));
				//��Լ10��
				if (NULL != wcpHead)
				{
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					iPsca_num++;
					if (iPsca_num<10)
					{
						pos=(iPsca_num*100)/10;
					} 
					else
					{
						pos=100;
					}
					dPercent = (double)pos/100;
					
					strPos.Format(_T("Programming SecurityA core:%d%%..."),pos);

					this->Show_Status(strPos);
					this->Show_Progress(dPercent);
				}

				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming ProtocolA core...  "));

				//��Լ200��
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Programming ProtocolA core..."));

					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 2));
					iPpa_num++;
					if (iPpa_num<200)
					{
						pos=(iPpa_num*100)/200;
					} 
					else
					{
						pos=100;
					}
					dPercent = (double)pos/100;
					strPos.Format(_T("Programming ProtocolA core:%d%%......"),pos);

					this->Show_Status(strPos);
					this->Show_Progress(dPercent);

				}

				//15��
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming SecurityB core"));				
				if (NULL != wcpHead)
				{			
					strInfoDis.Format(_T("Programming SecurityB core..."));
					wcpHead+=strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 2));
					iPsc_num++;
					if (iPsc_num<15)
					{
						pos=(iPsc_num*100)/15;
					} 
					else
					{
						pos=100;
					}
					dPercent = (double)pos/100;
					strPos.Format(_T("Programming SecurityB core:%d%%......"),pos);

					this->Show_Status(strPos);
					this->Show_Progress(dPercent);
				}
				//wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Setting front-end configuration"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming ApplicationA core"));
				//33��				
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Programming ApplicationA core..."));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 2));
					iPac_num++;
					if (iPac_num<33)
					{
						pos=(iPac_num*100)/33;
					} 
					else
					{
						pos=100;
					}
					dPercent = (double)pos/100;
					strPos.Format(_T("Programming ApplicationA core:%d%%......"),pos);		

					this->Show_Status(strPos);
					this->Show_Progress(dPercent);
				}
				//wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Setting general configuration"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Setting front-end configuration"));		
				//4��
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Setting front-end configuration"));

					wcpHead+= strInfoDis.GetLength();
		
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
	
					this->Show_Status(strInfoDis);
					this->Show_Progress(0.65);

				}
				//wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming Application core"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Setting general configuration"));				
				//20��
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Setting general configuration"));
					wcpHead+=  strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					iSgc_num++;
					if (iSgc_num<20)
					{
						pos=(iSgc_num*100)/20;
					} 
					else
					{
						pos=100;
					}
					dPercent = (double)pos/100;
					strPos.Format(_T("Setting general configuration:%d%%......"),pos);

					this->Show_Status(strPos);
					this->Show_Progress(dPercent);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Sorry"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Sorry"));
					bDownloading_Flag =true;

					this->m_csError_Code.Format(_T("FAIL,FW DL FAIL!"));
					this->m_iError_Code = ERR_FW_DL_FAIL;

					this->Show_Out(strInfoDis);

					throw(1);
				}	
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Update complete"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Update complete"));
					this->Show_Out(strInfoDis);
					bDownloading_Flag =true;
					break;
				}			
			}
		}
	}
	catch(...)
	{
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
		m_DL_FLAG = false;


		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);

		this->Show_Status(_T("Download Fail"));
		this->Show_Progress(1.0);

		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}


		return RTN_FAIL;
	}
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

	m_DL_FLAG= false;

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
	//Save_Trace_Log_File(_T("DL PASS"));
	this->Show_Status(_T("Download Pass"));
	this->Show_Progress(1.0);

	if(hFile_Handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile_Handle);
	}
	return RTN_SUCCESS;
}


int CARM_OP_HUAWEI::DL_FLS_FILE_IN_EXE_BCG_COM(CString strCommand)
{

	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int iTemp;
	CString csTotal_Info, csData,strDataInfo;
	time_t t_Start, t_End;

	time_t t_Start_T, t_End_T;

	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;


	bool bDownloading_Flag = false,bDC_Reset =false;

	double dAdd_Percent;//���ظ�������ռ�еĽ������ݶ�

	CString strDis,strInfo,strPow;
	wchar_t * wcpCmd = NULL;

	int iPpa_num  =0,iPsc_num  =0,iPac_num =0,iSgc_num =0,iPsca_num=0;
	int pos =0;
	CString strPos,strInfoDis;

	int iTimeOut =40;//��˼�ṩ����45���ʱ��

	int iTimeOut_DC =8;// ���8��û���κ���Ӧ�Ļ�����DC�����³���

	int  iStart =0;

	BOOL  bFind_Process =FALSE;


	CString  strSSB_ExeName =_T("");
	switch(m_iClass_Idx)
	{
		case 1: strSSB_ExeName = _T("SsbLoader1.exe"); break;
		case 2: strSSB_ExeName = _T("SsbLoader2.exe"); break;
		case 3: strSSB_ExeName = _T("SsbLoader3.exe"); break;
		case 4: strSSB_ExeName = _T("SsbLoader4.exe"); break;
		default:  
			{
				AfxMessageBox(_T("m_iClass_Idx ERR"));
				strSSB_ExeName = _T("SsbLoader.exe"); 
			}
			break;

	}

	try  
	{

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�


		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			AfxMessageBox(_T("Create pipe fail"));

			this->m_csError_Code.Format(_T("Create pipe fail"));
			this->m_iError_Code = ERR_CREATE_PIPE_FAIL;

			throw(0);
		}
		//------------------------------ 

		csTotal_Info.Format(_T(""));
		strDataInfo.Format(_T(""));
		//------------------------------ ���dosָ�� ------------------------------ 

		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (strCommand.GetLength() + 128));

		swprintf(wcpCmd, _T("%s"), strCommand);
		//Save_Trace_Log_File(strCommand);

		//------------------------------ ���� process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//�����������ȡ�õ�ǰ���̵�StartupInfo,�����½��Ľ��̻�������ǰ���̵�StartupInfo���
		si.hStdError = hWrite_Pipe;//��������ľ��
		si.hStdOutput = hWrite_Pipe;//��׼����ľ��
		si.wShowWindow = SW_HIDE;//�ƶ����½��̴���ʱ���ڵ���ʵ״̬����������� SW_HIDE �����½���DOS����
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//�����ƶ� STARTUPINFO����һ�Ѳ����Ǹ���Ч��
		//
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			AfxMessageBox(_T("Create process fail!"));
			this->m_csError_Code.Format(_T("Create process fail"));
			this->m_iError_Code = ERR_CREATE_PROCESS_FAIL;	
			throw(1);
		}
		//�ѹܵ���д����ȹرգ�����ReadFile�����Ͳ���������
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 
		//------------------------------ ѭ����ȡprocess�Ĵ�����Ϣ ------------------------------ 
		//��ʼ��ʱ
		time(&t_Start);

		time( &t_Start_T );//20190725
		
		int iFirstFlag =1,iDC_Flag=1;

		int i =0,iTimeTemp =0;
		
		while(false == bDownloading_Flag) 
		{
			//��info
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;

			if (FALSE == ::PeekNamedPipe(hRead_Pipe,cpRead_Buff,2047,&ulActual_Read,0,0))
			{
				//...
				//this->Show_Out(_T("PeekNamedPipe Fail"));

				//------------------------------------------------
				time( &t_End );
				iTimeTemp = (int)difftime( t_End,t_Start);
				//
				if( iTimeTemp>= iTimeOut )
				{
					//������Ϣ��SSB.EXE�ļ�ɱ��
					bool bKill_Flag =false;
					bKill_Flag = m_cla_GFC.Kill_Process(strSSB_ExeName);
					if (false ==  bKill_Flag)
					{
						csData.Format(_T("%s Del FAIL_1"),strSSB_ExeName);
						this->Show_Out(csData);
					} 
					else
					{
						csData.Format(_T("%s Del PASS_1"),strSSB_ExeName);
						this->Show_Out(csData);
					}

				
					for(i =0; i<20;i++)
					{	
						csData.Format(_T("Wait SSB Del......%d"),i);

						this->Show_Status(csData);

						bFind_Process = m_cla_GFC.Find_Process( strSSB_ExeName);

						if (TRUE == bFind_Process) //SsbLoader.exe �����ڣ������ȴ�
						{
							Sleep(500);
							continue;
						}
						else
						{
							break;//SsbLoader.exe�Ѿ�ɾ���ɹ�
						}
											
					}
	
					this->Show_Status(_T("SSB.BIN Download Time Out_1"));
					this->Show_Progress(1.0);
					this->Show_Out(_T("SSB.BIN Download Time Out_1"));

					this->m_csError_Code.Format(_T("SSB.BIN Download Time Out_1"));
					this->m_iError_Code = ERR_SSB_DL_TIMEOUT;

					throw(0);
				}
				else
				{
					//...
				}
				//------------------------------------------------

			} 
			else
			{
				if(ulActual_Read>0)
				{
					//������
					if(::ReadFile(hRead_Pipe, cpRead_Buff, 2047, &ulActual_Read, NULL) == NULL)
					{
						bDownloading_Flag = true;//20190626
						this->Show_Out(_T("SSB ReadFile NULL"));
						this->m_csError_Code.Format(_T("SSB ReadFile NULL"));
						this->m_iError_Code = ERR_SSB_ReadFile_NULL;	
					}
					else
					{
						//
						time( &t_Start_T );
					}
				}
				else
				{
					//...
					time( &t_End );
					iTimeTemp = (int)difftime( t_End,t_Start_T);
					//
					if( iTimeTemp >= iTimeOut )
					{
						//������Ϣ��SSB.EXE�ļ�ɱ��
						bool bKill_Flag =false;
						bKill_Flag = m_cla_GFC.Kill_Process(strSSB_ExeName);
						if (false ==  bKill_Flag)
						{
							csData.Format(_T("%s Del FAIL_2"),strSSB_ExeName);
							this->Show_Out(csData);
						} 
						else
						{
							csData.Format(_T("%s Del PASS_2"),strSSB_ExeName);
							this->Show_Out(csData);
						}

						for(i =0; i<20;i++)
						{	
							csData.Format(_T("Wait SSB Del......%d"),i);

							this->Show_Status(csData);

							bFind_Process = m_cla_GFC.Find_Process( strSSB_ExeName);

							if (TRUE == bFind_Process) //SsbLoader.exe �����ڣ������ȴ�
							{
								Sleep(500);
								continue;
							}
							else
							{
								break;//SsbLoader.exe�Ѿ�ɾ���ɹ�
							}

						}

						this->Show_Status(_T("SSB.BIN Download Time Out_2"));
						this->Show_Progress(1.0);

						this->Show_Out(_T("SSB.BIN Download Time Out_2"));
						this->m_csError_Code.Format(_T("SSB.BIN Download Time Out_2"));
						this->m_iError_Code = ERR_SSB_DL_TIMEOUT;
						throw(0);
					}
					else
					{
						if( iTimeTemp == iTimeOut_DC )//��ʱ8������Ӧ�Ļ���RESET��DC
						{
							if (LOAD_AUTO_ROBOT ==  m_iOP_Robot)
							{	
								//����һ�ε�
								if (0 == iStart)//�״�
								{
									csData.Format(_T("3-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
									this->Show_Out(csData);

									bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF"));
									Sleep(2000);

									csData.Format(_T("3-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
									this->Show_Out(csData);

									bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
								} 
								iStart++;

							}//(LOAD_AUTO_ROBOT ==  m_iOP_Robot)

							else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�
							{
								csData.Format(_T("3-SPC�����ϵ�-CH%d"),this->m_iClass_Idx);

								this->Show_Out(csData);

								this->Set_PowerOn_SPC(this->m_iClass_Idx);
							}
							else
							{
								if(true == this->m_bPLC_PowFlag)
								{
									//����һ�ε�
									if (0 == iStart)//�״�
									{
										csData.Format(_T("3-CH%d DC OFF"),m_iClass_Idx);
										this->Show_Out(csData);

										this->Send_Close_PLC_Mode_Cmd();
										Sleep(2000);

										csData.Format(_T("3-CH%d DC ON"),m_iClass_Idx);
										this->Show_Out(csData);

										this->Send_Open_PLC_Mode_Cmd();
									} 
									iStart++;
								}
								else
								{
									if (0 == iStart)//�״�
									{
										csData.Format(_T("���ֶ�ģ�鸴λ(2)"));
										this->Show_Status(csData);
									}
									iStart++;
								}
							}//else

						}//( iTimeTemp == 8 )
						else
						{
							//
// 							csData.Format(_T("Auto Reset,Please Wait...%d"),iTimeTemp);
// 							this->Show_Status(csData);
						}

					}//else
				}

			}

			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));
				this->Show_Out(csData);

				//��������
				csTotal_Info += csData;
				strDataInfo = csTotal_Info;

				//Waiting for bootloader to send info message
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Waiting for bootloader to send info message"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Waiting for bootloader to send info message"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					this->Show_Progress(0.1);

					if (LOAD_AUTO_ROBOT ==  m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
					{
						strPow.Format(_T("Module Reset1(SSB1״̬:%d:ͨ��:%d-TS:%d-��ʼ)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);

						strPow.Format(_T("%s"),strPow);
						this->Show_Out(strPow);
#ifdef FW_MULTI	
						if (m_iTimeInterval>0)
						{
							//���ܿ�PC����DC����ָ��
							csData.Format(_T("4-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(m_iTimeInterval);

							csData.Format(_T("4-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
						}					

						//�����һ��û��Ӧ�������ڶ��θ�λ
						m_i150ResetTime1++;
						if(m_i150ResetTime1 ==2)
						{
							csData.Format(_T("5-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(2000);

							csData.Format(_T("5-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);
							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
						}
#endif
						strPow.Format(_T("Module Reset1(SSB1״̬:%d:ͨ��:%d-TS:%d-���)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);

						strPow.Format(_T("%s"),strPow);
						this->Show_Out(strPow);
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�SPC
					{
						if (m_iTimeInterval>0)//20200529 �����ø÷�ʽ�������µ�
						{
							csData.Format(_T("4-SPC�����ϵ�-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}
						//�����һ��û��Ӧ�������ڶ��θ�λ
						m_i150ResetTime1++;
						if(m_i150ResetTime1 ==2)
						{
							csData.Format(_T("4-SPC�����ϵ�-2-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}

					}
					else
					{
						//PLCģʽ�ݲ��������µ�
						if(true == this->m_bPLC_PowFlag)
						{
							strPow.Format(_T("Module Reset1_PLC(SSB1״̬:%d:ͨ��:%d-TS:%d-��ʼ)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);
#ifdef FW_MULTI	

 							if (m_iTimeInterval>0)
 							{
								csData.Format(_T("4-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Close_PLC_Mode_Cmd();
								Sleep(m_iTimeInterval);

								csData.Format(_T("4-CH%d DC ON"),m_iClass_Idx);
								this->Show_Out(csData);
								this->Send_Open_PLC_Mode_Cmd();
 							}		

							//�����һ��û��Ӧ�������ڶ��θ�λ
							m_i150ResetTime1++;
							if(m_i150ResetTime1 ==2)
							{
								csData.Format(_T("5-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Close_PLC_Mode_Cmd();
								Sleep(2000);

								csData.Format(_T("5-CH%d DC ON"),m_iClass_Idx);
								this->Show_Out(csData);
								this->Send_Open_PLC_Mode_Cmd();
							}
#endif
							strPow.Format(_T("Module Reset1_PLC(SSB1״̬:%d:ͨ��:%d-TS:%d-���)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);
						}
						else
						{
							m_i150ResetTime1++;
							if(m_i150ResetTime1 ==2)
							{
								this->Show_Status(_T("���ֶ���ģ�鸴λ(3)"));
							    this->Show_Out(_T("PLC NOT SUPPORT(SSB DL)"));
							}
						}
					
					}
					continue;
				}
				//Waiting for UE to reboot to establish communication...
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Waiting for UE to reboot"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Waiting for UE to reboot"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));

					//this->Show_Status(strInfoDis);
					this->Show_Progress(0.1);

					if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
					{
						strPow.Format(_T("Module Reset1(SSB2״̬:%d:ͨ��:%d-TS:%d-��ʼ)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);
						this->Show_Out(strPow);
#ifdef FW_MULTI					
						if (m_iTimeInterval>0)
						{
							//���ܿ�PC����DC����ָ��
							csData.Format(_T("6-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(m_iTimeInterval);

							csData.Format(_T("6-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);
							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
						}
						
						//�����һ��û��Ӧ�������ڶ��θ�λ
						m_i150ResetTime2++;
						if(m_i150ResetTime2 ==2)
						{
							csData.Format(_T("7-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(2000);

							csData.Format(_T("7-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
						}
						
#endif
						strPow.Format(_T("Module Reset1(SSB2״̬:%d:ͨ��:%d-TS:%d-���)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);

						strPow.Format(_T("%s"),strPow);
						this->Show_Out(strPow);
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�SPC
					{

						if (m_iTimeInterval>0)//�����ô˷�ʽ �������� 20200529
						{
							csData.Format(_T("5-SPC�����ϵ�-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}
						//�����һ��û��Ӧ�������ڶ��θ�λ
						m_i150ResetTime2++;
						if(m_i150ResetTime2 ==2)
						{
							csData.Format(_T("5-SPC�����ϵ�-2-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}
					}
					else
					{
						//PLCģʽ�ݲ��������µ�
						if(true == this->m_bPLC_PowFlag)
						{
							strPow.Format(_T("Module Reset1_PLC2(SSB2״̬:%d:ͨ��:%d-TS:%d-��ʼ)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);
#ifdef FW_MULTI	
 							if (m_iTimeInterval>0)
 							{
 								//���ܿ�PC����DC����ָ��
								csData.Format(_T("6-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);
								this->Send_Close_PLC_Mode_Cmd();
								Sleep(m_iTimeInterval);

								csData.Format(_T("6-CH%d DC ON"),m_iClass_Idx);
								this->Show_Out(csData);
								this->Send_Open_PLC_Mode_Cmd();
 							}					
// 							
							//�����һ��û��Ӧ�������ڶ��θ�λ
							m_i150ResetTime2++;
							if(m_i150ResetTime2 ==2)
							{
								csData.Format(_T("7-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Close_PLC_Mode_Cmd();
								Sleep(2000);

								csData.Format(_T("7-CH%d DC ON"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Open_PLC_Mode_Cmd();
							}
#endif
							strPow.Format(_T("Module Reset1_PLC2(SSB2״̬:%d:ͨ��:%d-TS:%d-���)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);
						}
						else
						{

							//�����һ��û��Ӧ�������ڶ��θ�λ
							m_i150ResetTime2++;
							if(m_i150ResetTime2 ==2)
							{
								this->Show_Out(_T("PLC NOT SUPPORT(SSB DL)(4)"));
								this->Show_Status(_T("���ֶ���ģ�鸴λ"));
							}

						}
					}
					continue;

				}
				//try resetting again
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("try resetting again"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("try resetting again"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					this->Show_Status(strInfoDis);
					this->Show_Progress(0.1);

					this->Show_Out(_T("try resetting again"));

					strPow.Format(_T("Module Reset1(SSB3״̬:%d:ͨ��:%d-TS:%d-��ʼ)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
					this->Show_Status(strPow);
					this->Show_Out(strPow);

					//20190716
					if (LOAD_AUTO_ROBOT ==  m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
					{
#ifdef FW_MULTI							
						csData.Format(_T("8-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
						this->Show_Out(csData);

						bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
						Sleep(2000);

						csData.Format(_T("8-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
						this->Show_Out(csData);
						bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
#endif
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�SPC
					{
						csData.Format(_T("8-SPC�����ϵ�-CH%d"),this->m_iClass_Idx);

						this->Show_Out(csData);

						this->Set_PowerOn_SPC(this->m_iClass_Idx);
					}
					else
					{
						//PLCģʽ�ݲ��������µ�
						if(true == this->m_bPLC_PowFlag)
						{
							csData.Format(_T("8-CH%d DC OFF"),m_iClass_Idx);
							this->Show_Out(csData);

							this->Send_Close_PLC_Mode_Cmd();
							Sleep(2000);

							csData.Format(_T("8-CH%d DC ON"),m_iClass_Idx);
							this->Show_Out(csData);
							this->Send_Open_PLC_Mode_Cmd();
						}
						else
						{
							this->Show_Status(_T("���ֶ���ģ�鸴λ(4)"));
							this->Show_Out(_T("PLC NOT SUPPORT(SSB DL)"));
						}
					}
					strPow.Format(_T("Module Reset1_PLC(SSB3״̬:%d:ͨ��:%d-TS:%d-���)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
					this->Show_Status(strPow);
					this->Show_Out(strPow);

					continue;

				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Initial bootInfo"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Initial bootInfo"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					this->Show_Progress(0.2);

					this->Show_Out(_T("Initial bootInfo"));

				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming image using FSB"));
				//��Լ30��
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Programming image using FSB..."));

					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					iPsca_num++;
					if (iPsca_num<10)
					{
						pos=(iPsca_num*100)/10;
					} 
					else
					{
						pos=100;
					}
					dAdd_Percent =(double)pos/100;

					strPos.Format(_T("Programming image using FSB:%d%%......"),pos);

					this->Show_Status(strPos);
					this->Show_Progress(dAdd_Percent);

				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Sorry"));
				if (NULL != wcpHead)
				{
					if (NULL == wcsstr(csTotal_Info.GetBuffer(), _T("first-stage")))
					{
						strInfoDis.Format(_T("Sorry"));
						bDownloading_Flag =true;//ѭ��������־

						this->Show_Status(_T("SSB.BIN Download Fail"));
						this->Show_Progress(1.0);
						this->Show_Out(_T("SSB.BIN Download Fail"));

						this->m_csError_Code.Format(_T("SSB.BIN Download Fail!"));
						this->m_iError_Code = ERR_SSB_DL_FAIL;
						throw(1);
					} 
					//Sorry - something went wrong - The UE is not running the first-stage bootloader
					//��ʶģ�����Ѿ�����SSB�������ٽ������� 20190625
					else
					{
						strInfoDis.Format(_T("Finished"));
						bDownloading_Flag =true;//ѭ��������־

						this->Show_Status(_T("SSB.BIN Download Finished"));
						this->Show_Out(_T("SSB.BIN Download Finished"));

						this->Show_Progress(1.0);
						continue;
					}

				}	

				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Finished"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Finished"));
					bDownloading_Flag =true;//ѭ��������־
					this->Show_Status(_T("SSB.BIN Download Finished"));
					this->Show_Out(_T("SSB.BIN Download Finished"));

					this->Show_Progress(1.0);
					continue;
				}			
			}
		}
	}
	catch(...)
	{
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
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);

		return RTN_FAIL;
	}
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

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	return RTN_SUCCESS;
}

bool CARM_OP_HUAWEI::NB_Port_Open(int iBaudRate)
{
	time_t start, finish;
	int iTimeOut =6;//�򿪶˿ڸ�Ϊѭ�����Ҵ򿪵ķ�ʽ

	this->m_cla_Com_Port.m_iBaudRate = iBaudRate;
	this->m_cla_Com_Port.m_iPort = m_iCom_Port;
	this->m_cla_Com_Port.m_dwDTR_CTRL = DTR_CONTROL_DISABLE;
	this->m_cla_Com_Port.m_dwRTS_CTRL = RTS_CONTROL_DISABLE;


	//�󻪶���
	if (FALSE == this->m_bOutLine)
	{
		if (0 == wcscmp(this->m_csOC,_T("BC35GJB-02-DAH")))
		{
			this->m_cla_Com_Port.CommDcb.StopBits = TWOSTOPBITS;
		}
		
	}	

	

	try
	{
		//
		time( &start );
		do
		{
			Sleep( 200 );
			if(this->m_cla_Com_Port.Port_Open(300))
			{
				return true;
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
		return false;
	}
}
void CARM_OP_HUAWEI::NB_Port_Close()
{
	this->m_cla_Com_Port.Port_Close();
}

bool CARM_OP_HUAWEI::OnReadAtImei(void)
{
	// TODO: Add your control notification handler code here
	CString csCOM,strATCom,csData,csTemp;
	CString info;

	mAT_IMEI.Format(_T(""));

	time_t start, finish;
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;
	int iError = 0;

	//
	try
	{
		//��� COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- ���IMEI------------------------- 
		iTimeOut = 6;
		csAT_Cmd.Format( _T( "AT+CGSN=1" ) );

		//
		time( &start );
		//
		do
		{
			//����
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//��ȡ
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//+CGSN:86XXXXXXXXX	OK
			//
			if(wcsstr(wcrData, _T("OK")))
			{
				//��ȡ��������
				if (this->m_cla_GFC.GET_AT_RSP_STRING(wcrData, _T("OK"), -1, &mAT_IMEI) == true)
				{
					csData.Format(_T("AT_IMEI:1=%s"),mAT_IMEI);
					this->Show_Out(csData);

					mAT_IMEI.MakeUpper();
					mAT_IMEI.Remove(_T(' '));
					mAT_IMEI.Replace(_T("+CGSN:"),_T(""));

					csData.Format(_T("AT_IMEI:2=%s"),mAT_IMEI);
					this->Show_Out(csData);

					if (false == mAT_IMEI.IsEmpty())
					{
						return true;//pass, �˳�while
					} 
					
				}				
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
		//-------------------------
	}
	catch( ... )
	{
		//fail
		return false;
	}
	return false;
	
}
void CARM_OP_HUAWEI::OnWriteAtImei(void)
{
	// TODO: Add your control notification handler code here
	CString csCOM,strATCom,csData;
	CString strImei=_T("");

	this->m_cla_Com_Port.m_iBaudRate = 9600;
	this->m_cla_Com_Port.m_iPort = m_iCom_Port;

	if(!this->m_cla_Com_Port.Port_Open(400))
	{
		this->Show_Status(_T("Port is using"));
		return;
	}
	try
	{ 
		//����
		//AT+NTSETID=1,mAT_IMEI
		this->Show_Status(_T("Write IMEI Start..."));
		strImei.Format(_T("AT+QUSETID=1,%s\r\n"),mAT_IMEI);
		if(this->m_cla_Com_Port.Query_Tgt_CHAR(strImei, _T("OK"), 3) == false)
		{
			strImei.Format(_T("AT+NTSETID=1,%s\r\n"),mAT_IMEI);

			if(this->m_cla_Com_Port.Query_Tgt_CHAR(strImei, _T("OK"), 3) == false)
			{
				throw(0);
			}	
		}
			
		//��ȡAT��������
		strImei.Format(_T("%s Write Sucess"),mAT_IMEI);
		this->Show_Status(strImei);

		this->m_cla_Com_Port.Port_Close();

	}
	catch(...)
	{	
		strImei.Format(_T("%s Write Fail"),mAT_IMEI);
		this->Show_Status(strImei);
		mAT_IMEI.Format(_T(""));
		this->m_cla_Com_Port.Port_Close();
		return ;
	}

}

//��ѯ�Ƿ���Բ�ѯ���汾��
int CARM_OP_HUAWEI::FindVer(int index)
{
	// TODO: Add your control notification handler code here
	CString strATCom,csData;
	CString csCOM,csTemp;
	CString info;

	time_t start, finish;
	CString csAT_Cmd,strAT_FirewareName;
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
		csAT_Cmd.Format( _T( "ATI" ) );

		//
		time( &start );
		//
		do
		{
			//����
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//��ȡ
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			if(wcsstr(wcrData, _T("OK")))
			{	
				//��ȡ��������
				if (this->m_cla_GFC.GET_AT_RSP_STRING(wcrData, _T("OK"), -1, &mAT_FirewareName) == true)
				{
					mAT_FirewareName.MakeUpper();
					if(wcsstr(mAT_FirewareName, _T("REVISION")))
					{						
						mAT_FirewareName.Remove(_T(' '));
						mAT_FirewareName.Replace(_T("REVISION:"),_T(""));
						break;//pass, �˳�while
					}
				}			

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
		//-------------------------
	}
	catch( ... )
	{
		//fail
		return 0;
	}
	return 1;

}


bool CARM_OP_HUAWEI::Do_Execute_Erase()
{
	CString strDis;
	bool bEraseFlag = false;

	CString csCOM,strATCom,csData;

	this->m_cla_Com_Port.m_iBaudRate = 9600;
	this->m_cla_Com_Port.m_iPort = m_iCom_Port;

	if(this->m_cla_Com_Port.Port_Open(400))
	{
		m_cla_Com_Port.Port_Close();
	}
	else
	{
		this->Show_Status(_T("Port is using"));
		return false;
	}


	this->Show_Status(_T("Erase Start......"));
	this->Show_Out(_T("Erase Start......"));

	bEraseFlag = ERASE_FLS_FILE_IN_EXE(m_iClass_Idx);

	if (true == bEraseFlag)
	{
		strDis.Format(_T("Erase Pass..."));
	} 
	else
	{
		strDis.Format(_T("Erase Fail..."));
	}
	this->Show_Out(strDis);
	this->Show_Status(strDis);

	return bEraseFlag;

}

bool CARM_OP_HUAWEI::OnWriteAtImei_Auto(void)
{
	// TODO: Add your control notification handler code here
	CString csData;
	CString strImei=_T("");

	try
	{ 
		//����
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();
		//AT+QUSETID=1,mBar_IMEI
		this->Show_Status(_T("Write IMEI Start..."));

		strImei.Format(_T("AT+QUSETID=1,%s\r\n"),mBar_IMEI);

		if(this->m_cla_Com_Port.Query_Tgt_CHAR(strImei, _T("OK"), 5) == false)
		{
			strImei.Format(_T("AT+NTSETID=1,%s\r\n"),mBar_IMEI);

			if(this->m_cla_Com_Port.Query_Tgt_CHAR(strImei, _T("OK"), 5) == false)
			{
				throw(0);
			}	
		}

		//��ȡAT��������
		strImei.Format(_T("%s Write Sucess"),mBar_IMEI);
		this->Show_Status(strImei);
	}
	catch(...)
	{	
		strImei.Format(_T("%s Write Fail"),mBar_IMEI);
		this->Show_Status(strImei);
		mAT_IMEI.Format(_T(""));
		return false;
	}
	return true;

}

bool CARM_OP_HUAWEI::OnWriteAtSN_Auto(void)
{
	// TODO: Add your control notification handler code here
	CString strSN=_T("");
	try
	{ 
		//����
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();
		//AT+QUSETID=1,mBar_IMEI
		this->Show_Status(_T("Write SN Start..."));

		strSN.Format(_T("AT+QUSETID=0,%s\r\n"),mBar_SN);

		if(this->m_cla_Com_Port.Query_Tgt_CHAR(strSN, _T("OK"), 5) == false)
		{
			throw(0);
		}

		//��ȡAT��������
		strSN.Format(_T("%s Write Sucess"),mBar_SN);
		this->Show_Status(strSN);

	}
	catch(...)
	{	
		strSN.Format(_T("%s Write Fail"),mBar_SN);
		this->Show_Status(strSN);
		return false;
	}
	return true;
}
bool CARM_OP_HUAWEI::OnReadAtSN_Auto(void)
{
	// TODO: Add your control notification handler code here
	CString strSN=_T("");
	CString csCOM,strATCom,csData,csTemp;
	CString info;

	mAT_SN.Format(_T(""));

	time_t start, finish;
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;
	int iError = 0;

	//
	try
	{
		//��� COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- ���IMEI------------------------- 
		iTimeOut = 6;
		csAT_Cmd.Format( _T( "AT+CGSN=0" ) );

		//
		time( &start );
		//
		do
		{
			//����
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//��ȡ
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			if(wcsstr(wcrData, _T("OK")))
			{
				//��ȡ��������
				if (this->m_cla_GFC.GET_AT_RSP_STRING(wcrData, _T("OK"), -1, &mAT_SN) == true)
				{
					csData.Format(_T("AT_SN:1=%s"),mAT_SN);
					this->Show_Out(csData);

					mAT_SN.MakeUpper();
					mAT_SN.Remove(_T(' '));
					mAT_SN.Replace(_T("+CGSN:"),_T(""));

					csData.Format(_T("AT_SN:2=%s"),mAT_SN);
					this->Show_Out(csData);

					return true;//pass, �˳�while
				}				
		
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
		//-------------------------
	}
	catch( ... )
	{
		//fail
		return false;
	}
	return true;
}


//д�ţ�SN
bool CARM_OP_HUAWEI::WRITE_IMEI_SN()
{
    bool  bStatusFlag =false,bGetAtImei=false;
	CString csData;

	bool  bImeiCheck =false,bSnCheck=false;

	this->Show_Out(_T("Check and Write Start......"));
	//���Ӽ��
	//*******************************************************
	if(mBar_IMEI.GetLength() != 15)
	{
		csData.Format(_T("FAIL,д��IMEI����!=15(%s)!"),mBar_IMEI);

		this->Show_Status(csData);		
		this->Show_Out(csData);	

		//������Ϣ
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_IMEI_LEN;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));
		return false;
	}
   //�򿪿�
	//*******************************************************
	bStatusFlag = NB_Port_Open(9600);
	if (false == bStatusFlag)
	{
		csData.Format(_T("�˿ڴ�ʧ��!"));

		this->Show_Status(csData);	
		//������Ϣ
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_OPEN_PORT_FAIL;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));
		return false;
	}

	//��ѯ�汾
	//*******************************************************
	Sleep(500);
	bStatusFlag = FindVerAuto();
	if (false == bStatusFlag)
	{
		csData.Format(_T("Fail,δ��ѯ��ģ��汾��!"));

		//������Ϣ
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_FIND_FW_VERSION;

		this->Show_Status(csData);	
		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));
		//20181024
		NB_Port_Close();

		return false;
	}
	else
	{
		csData.Format(_T("��ѯģ��汾(%s)"),mAT_FirewareName);

		this->Show_Status(csData);
		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	}


	//���汾��DB��ƥ��
	//*******************************************************
	//Sleep(500);
	mDB_FirewareName.MakeUpper();
	if(wcscmp(mDB_FirewareName,mAT_FirewareName)!= 0)
	{
		//��ƥ��
		csData.Format(_T("Fail,��ѯ�İ汾�����ݿⲻƥ��!"));

		//������Ϣ
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_FW_VER_NOT_MATCH_DB;

		this->Show_Status(csData);	
		csData.Format(_T("%s-AT(%s)DB(%s)"),csData,mAT_FirewareName,mDB_FirewareName);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));

		//20181024
		NB_Port_Close();

		return false;
	}
	else
	{
		csData.Format(_T("ģ��汾�����ݿ�ƥ��"));

		this->Show_Status(csData);
	
		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	}

	//*******************************************************

	//�����Ȳ�ѯIMEI
	//*******************************************************
	//Sleep(500);
	bImeiCheck = OnReadAtImei();

	//*******************************************************

	//�����Ȳ�ѯSN
	//*******************************************************
	bSnCheck = OnReadAtSN_Auto();

	if((true == bImeiCheck) && (true == bSnCheck))
	{
		//20190822 ���� AT IMEI�� bar IMEI�Ƚ�
		if (0 != wcscmp(mAT_IMEI,mBar_IMEI))
		{
			csData.Format(_T("FAIL,��ѯģ��IMEI(%s)+BAR IMEI��ƥ��"),mAT_IMEI);
			this->Show_Status(csData);
			this->Show_Out(csData);
			//20181024
			NB_Port_Close();
			return false;
		}
		   

		csData.Format(_T("PASS,��ѯģ��IMEI+SN(1)(%s)"),mBar_IMEI);

		this->Show_Status(csData);
		bGetAtImei=true;
		//20181024
		NB_Port_Close();

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));
		return  true;
	}
	if (false == bImeiCheck)
	{
		csData.Format(_T("δ��ѯ��ģ��IMEI(1)(%s)!"),mBar_IMEI);

		this->Show_Status(csData);	

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	}

	if (false == bSnCheck)
	{
		csData.Format(_T("δ��ѯ��ģ��SN(1)(%s)!"),mBar_SN);

		this->Show_Status(csData);	
		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	}


	//��û��ѯ��IMEI������Ϊ��дIMEI
	//*******************************************************
	//Sleep(500);
	if (false == bImeiCheck)
	{
		bStatusFlag = OnWriteAtImei_Auto();
		if (false == bStatusFlag)
		{
			csData.Format(_T("FAIL,дIMEI(%s)!"),mBar_IMEI);

			this->Show_Status(csData);		
			//������Ϣ
			this->m_csError_Code.Format(_T("%s"),csData);
			this->m_iError_Code= ERR_W_IMEI;

			csData.Format(_T("%s"),csData);
			this->Show_Out(csData);
			this->Show_Out(_T("Check and Write End......"));
			//20181024
			NB_Port_Close();
			return false;
		}
		else
		{
			csData.Format(_T("PASS,дIMEI(%s)"),mBar_IMEI);

			this->Show_Status(csData);

			csData.Format(_T("%s"),csData);
			this->Show_Out(csData);
		}
	}
	//*******************************************************
	//дSN	
	//Sleep(500);
	if (false == bSnCheck)
	{
		bStatusFlag = OnWriteAtSN_Auto();
		if (false == bStatusFlag)
		{
			csData.Format(_T("FAIL,дSN(%s)"),mBar_SN);

			this->Show_Status(csData);

			//������Ϣ
			this->m_csError_Code.Format(_T("%s"),csData);
			this->m_iError_Code= ERR_W_SN;

			csData.Format(_T("%s"),csData);
			this->Show_Out(csData);
			this->Show_Out(_T("Check and Write End......"));

			//20181024
			NB_Port_Close();

			return false;
		}
		else
		{
			csData.Format(_T("PASS,дSN(%s)"),mBar_SN);

			this->Show_Status(csData);

			csData.Format(_T("%s"),csData);
			this->Show_Out(csData);
		}
	}

	//�ٲ�ѯIMEI
	//*******************************************************
	//Sleep(500);
	bStatusFlag = OnReadAtImei();
	if (false == bStatusFlag)
	{
		csData.Format(_T("FAIL,��ѯģ��IMEI(%s)(2)!"),mBar_IMEI);

		this->Show_Status(csData);

		//������Ϣ
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_FIND_IMEI_AT;

		bGetAtImei=false;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));

		//20181024
		NB_Port_Close();

		return false;
	}
	else
	{
		csData.Format(_T("PASS,��ѯģ��IMEI(%s)(2)"),mBar_IMEI);

		this->Show_Status(csData);

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);

		bGetAtImei=true;
	}






	//*******************************************************
	//AT��IMEI���ܿ��ṩ��mBar_IMEIƥ��
	if (NULL != wcsstr(mAT_IMEI,mBar_IMEI))
	{
		csData.Format(_T("PASS,IMEI_BarCode+AT(%s-%s)!"),mBar_IMEI,mAT_IMEI);

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	} 
	else
	{
		csData.Format(_T("FAIL,IMEI_BarCode+AT(%s-%s)!"),mBar_IMEI,mAT_IMEI);

		this->Show_Status(csData);

		::AfxMessageBox(csData);

		//������Ϣ
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_IMEI_AT_NOT_MATCH_DB;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));

		//20181024
		NB_Port_Close();

		return false;
	}


	//*******************************************************
	//��SN
	bStatusFlag = OnReadAtSN_Auto();
	if (false == bStatusFlag)
	{
		csData.Format(_T("FAIL,��SN(%s)"),mBar_SN);

		this->Show_Status(csData);

		//������Ϣ
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_FIND_SN_AT;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));

		//20181024
		NB_Port_Close();

		return false;
	}
	else
	{
		csData.Format(_T("PASS,��SN(%s)"),mBar_SN);

		this->Show_Status(csData);

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	}
	
	mBar_IMEI.Format(_T(""));
	mBar_SN.Format(_T(""));
	this->Show_Out(_T("Check and Write End......"));

	//20181024
	NB_Port_Close();
	return true;
}
//��ѯ�Ƿ���Բ�ѯ���汾��
bool CARM_OP_HUAWEI::FindVerAuto(void)
{
	// TODO: Add your control notification handler code here
	CString csCOM,strATCom,csData,csTemp;
	CString info;

	mAT_FirewareName.Format(_T(""));

	time_t start, finish;
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;
	int iError = 0;

	//
	try
	{
		//��� COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- ���IMEI------------------------- 
		iTimeOut = 8;
		csAT_Cmd.Format( _T( "ATI" ) );

		//
		time( &start );
		//
		do
		{
			//��� COM BUFF
			this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

			//����
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//��ȡ
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			if(wcsstr(wcrData, _T("OK")))
			{
				//��ȡ��������
				if (this->m_cla_GFC.GET_AT_RSP_STRING(wcrData, _T("OK"), -1, &mAT_FirewareName) == true)
				{
					mAT_FirewareName.MakeUpper();
					if(wcsstr(mAT_FirewareName, _T("REVISION")))
					{						
						mAT_FirewareName.Remove(_T(' '));
						mAT_FirewareName.Replace(_T("REVISION:"),_T(""));
						break;//pass, �˳�while
					}
				}				
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
		//-------------------------
	}
	catch( ... )
	{
		//fail
		return false;
	}
	return true;

}

//��GNSSʹ�� BC39
// �����ڷ��� AT+QGNSSC=1  Power on GNSS 
bool CARM_OP_HUAWEI::Set_QGNSSC_PowerOn(void)
{
	// TODO: Add your control notification handler code here
	CString csData;
	CString info;
	//�򿪿�
	//*******************************************************
	bool bStatusFlag = false;
	bStatusFlag = NB_Port_Open(9600);
	if (false == bStatusFlag)
	{
		csData.Format(_T("Set_QGNSSC_PowerOn:Open Port Fail!"));

		this->Show_Out(csData);	
		//������Ϣ
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code=0x03000000|0x00002029;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		return false;
	}


	time_t start, finish;
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;

	//
	try
	{
		//��� COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- ���IMEI------------------------- 
		iTimeOut = 8;
		csAT_Cmd.Format( _T( "AT+QGNSSC=1" ) );

		//
		time( &start );
		//
		do
		{
			//��� COM BUFF
			this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

			//����
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//��ȡ
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			if(wcsstr(wcrData, _T("OK")))
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
		//-------------------------
	}
	catch( ... )
	{
		//fail
		this->Show_Out(_T("Set_QGNSSC_PowerOn Fail"));
		NB_Port_Close();
		return false;
	}
	this->Show_Out(_T("Set_QGNSSC_PowerOn Pass"));
	NB_Port_Close();
	return true;

}
//�ر�GNSSʹ�� BC39
// �����ڷ��� AT+QGNSSC=0  Power OFF GNSS 
bool CARM_OP_HUAWEI::Set_QGNSSC_PowerOFF(void)
{
	CString csData;
	CString info;
	//�򿪿�
	//*******************************************************
	bool bStatusFlag = false;
	bStatusFlag = NB_Port_Open(9600);
	if (false == bStatusFlag)
	{
		csData.Format(_T("Set_QGNSSC_PowerOFF:Open Port Fail!"));

		this->Show_Out(csData);	
		//������Ϣ
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code=0x03000000|0x00002029;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		return false;
	}

	time_t start, finish;
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;

	//
	try
	{
		//��� COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- ���IMEI------------------------- 
		iTimeOut = 8;
		csAT_Cmd.Format( _T( "AT+QGNSSC=0" ) );

		//
		time( &start );
		//
		do
		{
			//��� COM BUFF
			this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

			//����
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//��ȡ
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			if(wcsstr(wcrData, _T("OK")))
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
		//-------------------------
	}
	catch( ... )
	{
		//fail
		this->Show_Out(_T("Set_QGNSSC_PowerOFF Fail"));
		NB_Port_Close();
		return false;
	}
	this->Show_Out(_T("Set_QGNSSC_PowerOFF Pass"));
	NB_Port_Close();
	return true;

}

//20190130 ����ÿ��ͨ��PLC�ĵ�Դ���µ�
void CARM_OP_HUAWEI::SetPLC_Open_Path(void)
{
	BOOL ret = FALSE;
	CString  csData;
	this->Show_Status(_T("Wait DC ON"));
	ret = OpenPLC_Power_Path_Huawei(this->m_iClass_Idx);

	if (TRUE == ret)
	{
		csData.Format(_T("PLC_Open Sucess"));
	} 
	else
	{
		csData.Format(_T("PLC_Open Fail"));
	}
	this->Show_Out(csData);	
}
void CARM_OP_HUAWEI::SetPLC_Close_Path(void)
{
	BOOL ret = FALSE;
	CString  csData;
	//this->Show_Status(_T("Wait DC OFF"));
	ret = ClosePLC_Power_Path_Huawei(this->m_iClass_Idx);
	if (TRUE == ret)
	{
		csData.Format(_T("PLC_OFF Sucess"));
	} 
	else
	{
		csData.Format(_T("PLC_OFF Fail"));
	}
	this->Show_Out(csData);	

}

//������һ·PLC�ĵ�Դ
BOOL CARM_OP_HUAWEI::OpenPLC_Power_Path_Huawei(int Index)
{
	time_t t_Start, t_End;
	unsigned long ulIndex=0;
	int  i_TimeOut=2;
	unsigned char ucOpenCmd[4],ucrTgt_Key[4];
	CString csData;
	char cp_Read_Buff[8];
	int ulTgt_Key_Quan=0;
	unsigned long ul_Expect_Read_Bytes=5;
	unsigned long ulp_Real_Rec_Bytes=0;

	try
	{	
		g_PLC_Pow_Ctrl.Lock();

		//����һ��ָ��
		ucOpenCmd[0]=0xFF;
		ucOpenCmd[1]=Index;
		ucOpenCmd[2]=0x1A;
		ucOpenCmd[3]=0x00;

		//������ֵ
		ucrTgt_Key[0] = 0xFF;
		ucrTgt_Key[1] = Index;
		ucrTgt_Key[2] = 0x1B;
		ucrTgt_Key[3] = 0x00;
		ulTgt_Key_Quan = 4;

		if (g_cla_Com_PLC_Power.Port_Open(500))
		{
			g_PLC_Pow_Ctrl.Unlock();
			return FALSE;
		}


		//��ս���BUFF
		 g_cla_Com_PLC_Power.Empty_Com_And_Rx_Buff();

		//ѭ��
		time(&t_Start);
		//
		do
		{
			g_cla_Com_PLC_Power.Write_Byte((char*)ucOpenCmd,4);

			//����ֵΪ��0xFF 01 1B 00 
			memset(&cp_Read_Buff,0,sizeof(cp_Read_Buff));
			Sleep(20);
			g_cla_Com_PLC_Power.Read_Byte(cp_Read_Buff,ul_Expect_Read_Bytes,&ulp_Real_Rec_Bytes);

			if(ulp_Real_Rec_Bytes!=0)
			{
				if(this->m_cla_GFC.mem_uc_str((unsigned char*)cp_Read_Buff, ulp_Real_Rec_Bytes, ucrTgt_Key, ulTgt_Key_Quan, &ulIndex) == true)
				{
					g_cla_Com_PLC_Power.Port_Close();

					g_PLC_Pow_Ctrl.Unlock();
					return TRUE;
				}			
			}

			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				throw(0);
			}
		}
		while(true);
	}
	catch(...)
	{
		g_cla_Com_PLC_Power.Port_Close();

		g_PLC_Pow_Ctrl.Unlock();
		return FALSE;
	}
	return TRUE;
}

//�ر�����һ·PLC�ĵ�Դ
BOOL CARM_OP_HUAWEI::ClosePLC_Power_Path_Huawei(int Index)
{
	time_t t_Start, t_End;
	unsigned long ulIndex=0;
	int  i_TimeOut=2;
	unsigned char ucOpenCmd[4],ucrTgt_Key[4];
	CString csData;
	char cp_Read_Buff[8];
	int ulTgt_Key_Quan=0;
	unsigned long ul_Expect_Read_Bytes=5;
	unsigned long ulp_Real_Rec_Bytes=0;

	try
	{
		g_PLC_Pow_Ctrl.Lock();

		if (g_cla_Com_PLC_Power.Port_Open(500))
		{
			g_PLC_Pow_Ctrl.Unlock();
			return FALSE;
		}

		//����һ��ָ��
		ucOpenCmd[0]=0xFF;
		ucOpenCmd[1]=Index;
		ucOpenCmd[2]=0x1C;
		ucOpenCmd[3]=0x00;

		//������ֵ
		ucrTgt_Key[0] = 0xFF;
		ucrTgt_Key[1] = Index;
		ucrTgt_Key[2] = 0x1D;
		ucrTgt_Key[3] = 0x00;
		ulTgt_Key_Quan = 4;


		//��ս���BUFF
		g_cla_Com_PLC_Power.Empty_Com_And_Rx_Buff();
		//ѭ��
		time(&t_Start);
		//
		do
		{
			g_cla_Com_PLC_Power.Write_Byte((char*)ucOpenCmd,4);

			//����ֵΪ��0xFF 01 1B 00 
			memset(&cp_Read_Buff,0,sizeof(cp_Read_Buff));
			Sleep(20);
			m_cla_Com_PLC_Power_Huawei.Read_Byte(cp_Read_Buff,ul_Expect_Read_Bytes,&ulp_Real_Rec_Bytes);

			if(ulp_Real_Rec_Bytes!=0)
			{
				if(this->m_cla_GFC.mem_uc_str((unsigned char*)cp_Read_Buff, ulp_Real_Rec_Bytes, ucrTgt_Key, ulTgt_Key_Quan, &ulIndex) == true)
				{
					g_cla_Com_PLC_Power.Port_Close();
					g_PLC_Pow_Ctrl.Unlock();
					return TRUE;
				}			
			}

			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				throw(0);
			}
		}
		while(true);
	}
	catch(...)
	{
		g_cla_Com_PLC_Power.Port_Close();
		g_PLC_Pow_Ctrl.Unlock();
		return FALSE;
	}
}

void Multi_PLC_Pow_BC(LPVOID x)
{
	CString strData;
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;

	//�ر�����һ·PLC�ĵ�Դ
	strData.Format(_T("2-CH%d-DC OFF"),pSv->m_iClass_Idx);
	pSv->Send_Close_PLC_Mode_Cmd();

	Sleep(500);
	//������һ·PLC�ĵ�Դ
	strData.Format(_T("2-CH%d-DC ON"),pSv->m_iClass_Idx);
	pSv->Send_Open_PLC_Mode_Cmd();

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_1(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//�ر�����һ·PLC�ĵ�Դ
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path2"));
	Sleep(500);
	//������һ·PLC�ĵ�Դ
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path2"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_2(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//�ر�����һ·PLC�ĵ�Դ
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path2"));
	Sleep(500);
	//������һ·PLC�ĵ�Դ
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path2"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_3(LPVOID x)
{
	g_PLC_Power.Lock();


	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//�ر�����һ·PLC�ĵ�Դ
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path3"));
	Sleep(500);
	//������һ·PLC�ĵ�Դ
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path3"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_4(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//�ر�����һ·PLC�ĵ�Դ
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path4"));
	Sleep(500);
	//������һ·PLC�ĵ�Դ
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path4"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_5(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//�ر�����һ·PLC�ĵ�Դ
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path5"));
	Sleep(500);
	//������һ·PLC�ĵ�Դ
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path5"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_6(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//�ر�����һ·PLC�ĵ�Դ
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path6"));
	Sleep(500);
	//������һ·PLC�ĵ�Դ
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path6"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_7(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//�ر�����һ·PLC�ĵ�Դ
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path7"));
	Sleep(500);
	//������һ·PLC�ĵ�Դ
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path7"));

	g_PLC_Power.Unlock();
}



void Multi_Auto_PLC_Pow_BC_1(LPVOID x)
{
	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;

	//�ر�����һ·PLC�ĵ�Դ
	CString strTsInfo;
	strTsInfo.Format(_T("2-TS%d-CH%d DC OFF"),pSv->m_Ts_Num,pSv->m_iClass_Idx);
	::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( pSv->m_Ts_Num, _T("OFF") );
	pSv->Show_Out(strTsInfo);

	Sleep(2000);
	//������һ·PLC�ĵ�Դ
	strTsInfo.Format(_T("2-TS%d-CH%d DC ON"),pSv->m_Ts_Num,pSv->m_iClass_Idx);
	::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( pSv->m_Ts_Num, _T("ON") );
	pSv->Show_Out(strTsInfo);
}

void Multi_Auto_SPC_Pow_BC_1(LPVOID x)
{
	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;

	pSv->Set_PowerOn_SPC(pSv->m_iClass_Idx);

	
}




bool  CARM_OP_HUAWEI::CheckSSB(void)
{
	bool  bDC_Reset =false;

	unsigned char* cpData =NULL;
	wchar_t * wcpData = NULL;
	time_t start, finish;

	unsigned char cData[2048];

	unsigned long BUFF_LEN=0;

	CString csData,strInfo;

	int iTimeOut=8;//�����ʱ8��

	try
	{
		//��һ�����ݼ��
		m_cla_Com_Port.Empty_Com_And_Rx_Buff();
		//�ر���DC
		//���ܿ�PC����DC����ָ��
		if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0Ϊ�ֶ�ģʽ����
		{		
			this->m_hThread_Auto_PLC_Pow_Handle_BC = CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE)Multi_Auto_PLC_Pow_BC_1,
				(void *)this,
				0,
				&this->m_dwThread_Auto_PLC_Pow_BC_ID);
		}
		else if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //ȫ�Զ�SPC
		{
			this->m_hThread_Auto_SPC_Pow_Handle_BC = CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE)Multi_Auto_SPC_Pow_BC_1,
				(void *)this,
				0,
				&this->m_dwThread_Auto_SPC_Pow_BC_ID);
		}
		else
		{
			if(true == this->m_bPLC_PowFlag)
			{
				this->m_hThread_PLC_Pow_Handle_BC = CreateThread(NULL,
					0,
					(LPTHREAD_START_ROUTINE)Multi_PLC_Pow_BC,
					(void *)this,
					0,
					&this->m_dwThread_PLC_Pow_BC_ID);
			}
			else
			{
				this->Show_Status(_T("��Ҫ�ֶ���λ���SSB..."));
				Sleep(5000);
			}


		}

		//�����ڴ�		
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 1024+128);
		time( &start );
		while (1)
		{
			m_cla_Com_Port.GET_BUFF_BYTE(&cpData,&BUFF_LEN);

			if ((BUFF_LEN>0)&&(BUFF_LEN<2048))
			{
				//�ַ�ת��
				memset(wcpData, 0, 1024+128);
				memset(cData, 0, 2048);
				memcpy(cData,cpData,BUFF_LEN);
				for (int i=0;i<(int)BUFF_LEN;i++)
				{
					if (cData[i]==0)
					{
						cData[i]='_';
					}
				}

				int iTemp=0;
				this->m_cla_GFC.MultiByte_To_WideChar((const char *)cData, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);
				csData.MakeUpper();
				if (wcsstr(csData.GetBuffer(), _T("SSB")))//�����û��SSB�ֶ���Ϣ
				{
					this->Show_Status(_T("check SSB OK..."));

					strInfo.Format(_T("CH%d-check SSB OK"),m_iClass_Idx);
					this->Show_Out(strInfo);
					
					break;
				} 
				else
				{						
					//...

				}
			}
			time( &finish );

			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				this->Show_Status(_T("check SSB Fail"));
				this->Show_Out(_T("check SSB Fail"));
				throw(1);
				break;
			}

		}
		
	}
	catch(...)
	{
		//
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		return false;

	}

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);

	return true;


}


bool CARM_OP_HUAWEI::WRITE_BYTE(char * cp_Send_Data, unsigned long ul_Data_Quan)
{
	return g_cla_Com_PLC_Power.Write_Byte(cp_Send_Data, ul_Data_Quan);
}

void CARM_OP_HUAWEI::Clear_Com_Rx_Buffer(void)
{
	g_cla_Com_PLC_Power.Empty_Com_And_Rx_Buff();
}
bool CARM_OP_HUAWEI::READ_PKT_RSP_V2(RSP_INFO_V2_S_Ext str_Rsp_Info_V2, int i_TimeOut)
{
	return this->READ_PKT_RSP_SYNC_V2(str_Rsp_Info_V2, i_TimeOut);
}
bool CARM_OP_HUAWEI::READ_PKT_RSP_SYNC_V2(RSP_INFO_V2_S_Ext str_Rsp_Info_V2, int i_TimeOut)
{
	//�á��˶Է���������Ƿ�����ȷ��RSP
	//�������ݰ�����ֵ�ǹ̶����ݵ����
	time_t t_Start, t_End;
	int i, j;
	int iRsp_Pkt_Numb = 0;//������ʵ��PKT��������ͳ��
	int iData_Index = 0;//��鴮��BUFF���ݵĵ�ǰIndexֵ�����Ѻ˶������PKT�����ݾͲ����ٺ˶��ˣ�
	unsigned char * ucpRx_Buff = NULL;
	unsigned long ulRx_Quan = 0;

	try
	{
		//��ʱ��ʼ
		time(&t_Start);

		//��ѭ����ʱ�䳬ʱ�������˳�
		do
		{
			//��ͣ
			if(this->m_iStop_Flag > 0)
			{
				this->m_csError_Code.Format(_T("Operation has been stopped!"));
				throw(0);
			}

			//��ʱ���
			time(&t_End);
			if(difftime(t_End, t_Start) >= i_TimeOut)
			{
				if(g_cla_Com_PLC_Power.m_str_Com_Rx_Buff.ul_Data_Length <= 0)
				{
					this->m_csError_Code.Format(_T("FAIL, Rsp reply nothing!"));
				}
				else
				{
					this->m_csError_Code.Format(_T("FAIL, Rsp does not reply right data!"));
				}

				throw(0);
			}


			//���һ�ν���BUFF�������ݺ����������BUFF�ĵ�ַ��������
			this->READ_BYTE(&ucpRx_Buff, &ulRx_Quan);


			//-------------------------------------- �������ݣ����ж��Ƿ�Ҫ�˶ԡ�����ֵ�� -------------------------------------- 
			//����COM_BUFF����Ч���ݵ�������ж��Ƿ�Ҫ���н�������
			if(ulRx_Quan <= 0)
			{
				Sleep(1);//1�����㹻�ˣ�ÿ�μ�鴮���ڿɶ����ݵĲ�������ʱ�䲻��1����
				continue;
			}
			else if(ulRx_Quan < str_Rsp_Info_V2.ui_Rsp_Data_Length)//û�дﵽ���ٵ��������ȴ�
			{
				Sleep(20);//���ʱ�����Ҫ��Ӱ�������ȶ�
				continue;
			}

			//��ǰ״̬�����㹻����������Ӧ
			//�ѵ�ǰ�˶Ե��Ĵ���Indexֵ��Ϊ��ʼֵ
			for(i=iData_Index; i<ulRx_Quan; i++)
			{
				//���ִ���BUFF�и��ַ��롰������Ӧ�������ַ�һ��
				if(*(ucpRx_Buff + i) == str_Rsp_Info_V2.ucr_Expect_Rsp[0])
				{
					//��ʼ����������Ӧ�������ݽ���ȫ��˶�
					for(j=0; j<str_Rsp_Info_V2.ui_Rsp_Data_Length; j++)
					{
						//��鴮��BUFF��Index���Ƿ�Խ�˴���BUFF�е�����
						if((i + j) < ulRx_Quan)
						{
							if(*(ucpRx_Buff + i + j) != str_Rsp_Info_V2.ucr_Expect_Rsp[j])
							{
								break;//�����������ݲ�ͬ�����˳���û�ҵ���������
							}
						}
						else
						{
							break;//�������ݽ����ˣ�Ҳ�˳�ѭ��
						}
					}//for(j=0; j<str_Rsp_Info_V2.ui_Rsp_Data_Length;

					//������ֵ�˶Ե������һλ�������ҵ�һ��ƥ�����Ӧ
					if(j == str_Rsp_Info_V2.ui_Rsp_Data_Length)
					{
						//ͳ�ƣ��ҵ���һ���������ݰ�
						iRsp_Pkt_Numb++;
						//�������֮ǰ�Ĵ���BUFF���ݣ����Ѽ����������㲻�ټ��
						iData_Index = i + str_Rsp_Info_V2.ui_Rsp_Data_Length;
						//��iData_Index��Ϊ�´μ�����㣬����ѭ���ᡰ+1�������Ը�ֵ�ǡ�-1��
						i = iData_Index - 1;
					}
					else if(j > str_Rsp_Info_V2.ui_Rsp_Data_Length)
					{
						this->m_csError_Code.Format(_T("ERROR, READ_PKT_RSP_SYNC_V2, j = %d, Rsp_Data_Length = %d;"), j, str_Rsp_Info_V2.ui_Rsp_Data_Length);
						::AfxMessageBox(this->m_csError_Code);
						throw(0);
					}
				}//���ִ���BUFF�и��ַ��롰������Ӧ�������ַ�һ��
			}//for(i=iData_Index; i<ulRx_Quan; i++

			//��鴮��BUFF�е����ݽ�����
			//�����Ӧ���������Բ���
			if(iRsp_Pkt_Numb == str_Rsp_Info_V2.ui_Rsp_Pkt_Quan)
			{
				//�������ݰ���������ȷ������while
				break;//pass
			}
		}
		while(true);
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool CARM_OP_HUAWEI::READ_BYTE(unsigned char ** ucp2_Buff_Pointer, unsigned long * ulp_Data_Length)
{
	//�����ڣ����ش���ʵ�ʽ��յ����ݵ�BUFF�ĵ�ַ��BUFF�е�byte������
	bool bRtn = false;

	bRtn = g_cla_Com_PLC_Power.Read_Byte_Sync_V1();
	*ucp2_Buff_Pointer = g_cla_Com_PLC_Power.m_str_Com_Rx_Buff.ucp_Buff;
	*ulp_Data_Length = g_cla_Com_PLC_Power.m_str_Com_Rx_Buff.ul_Data_Length;

	return bRtn;
}
bool CARM_OP_HUAWEI::Send_Open_PLC_Mode_Cmd(void)
{
	PKT_BUFF_V2_S_Ext str_Pkt_Cmd;
	RSP_INFO_V2_S_Ext str_Rsp_Info;
	CString strData;

	strData.Format(_T("Send Open PLC Port cmd(Port%d) ....."),g_cla_Com_PLC_Power.m_iPort);
	this->Show_Status(strData);

	g_PLC_Pow_Ctrl.Lock();

	try
	{	
		//������FF 01 1A 00��
		str_Pkt_Cmd.ucr_Buff[0] = 0xFF;
		str_Pkt_Cmd.ucr_Buff[1] = this->m_iClass_Idx;
		str_Pkt_Cmd.ucr_Buff[2] = 0x1A;
		str_Pkt_Cmd.ucr_Buff[3] = 0x00;
		str_Pkt_Cmd.ucr_Buff[4] = NULL;
		//
		str_Pkt_Cmd.ul_Data_Length = 4;

		//�趨Ԥ�ƻظ�//������FF 01 1B 00��
		str_Rsp_Info.ucr_Expect_Rsp[0] = 0xFF;
		str_Rsp_Info.ucr_Expect_Rsp[1] = this->m_iClass_Idx;
		str_Rsp_Info.ucr_Expect_Rsp[2] = 0x1B;
		str_Rsp_Info.ucr_Expect_Rsp[3] = 0x00;
		str_Rsp_Info.ucr_Expect_Rsp[4] = NULL;
		//
		str_Rsp_Info.ui_Rsp_Data_Length = 4;
		str_Rsp_Info.ui_Rsp_Pkt_Quan = 1;

		//����	
		if(this->QUERY_PKT(str_Pkt_Cmd, str_Rsp_Info, 4) == false)
		{
			throw(0);
		}
	}
	catch(...)
	{
		this->m_csError_Code += _T(" [Send Open PLC Port]");
		this->Show_Out(_T("Send Open PLC Port Fail"));

		//20190716
		Sleep(300);

		g_PLC_Pow_Ctrl.Unlock();
		return false;
	}

	//20190716
	Sleep(300);

	g_PLC_Pow_Ctrl.Unlock();

	return true;
}
bool CARM_OP_HUAWEI::Send_Close_PLC_Mode_Cmd(void)
{
	PKT_BUFF_V2_S_Ext str_Pkt_Cmd;
	RSP_INFO_V2_S_Ext str_Rsp_Info;
	CString strData;

	//strData.Format(_T("Send Close PLC Port cmd(Port%d) ....."),g_cla_Com_PLC_Power.m_iPort);
	//this->Show_Status(strData);

	g_PLC_Pow_Ctrl.Lock();

	try
	{	
		//������FF 01 1C 00��
		str_Pkt_Cmd.ucr_Buff[0] = 0xFF;
		str_Pkt_Cmd.ucr_Buff[1] = this->m_iClass_Idx;
		str_Pkt_Cmd.ucr_Buff[2] = 0x1C;
		str_Pkt_Cmd.ucr_Buff[3] = 0x00;
		str_Pkt_Cmd.ucr_Buff[4] = NULL;
		//
		str_Pkt_Cmd.ul_Data_Length = 4;

		//�趨Ԥ�ƻظ�//������FF 01 1D 00��
		str_Rsp_Info.ucr_Expect_Rsp[0] = 0xFF;
		str_Rsp_Info.ucr_Expect_Rsp[1] = this->m_iClass_Idx;
		str_Rsp_Info.ucr_Expect_Rsp[2] = 0x1D;
		str_Rsp_Info.ucr_Expect_Rsp[3] = 0x00;
		str_Rsp_Info.ucr_Expect_Rsp[4] = NULL;
		//
		str_Rsp_Info.ui_Rsp_Data_Length = 4;
		str_Rsp_Info.ui_Rsp_Pkt_Quan = 1;

		//����	
		if(this->QUERY_PKT(str_Pkt_Cmd, str_Rsp_Info, 4) == false)
		{
			throw(0);
		}
	}
	catch(...)
	{
		this->m_csError_Code += _T(" [Send Close PLC Port]");
		this->Show_Out(_T("Send Close PLC Port Fail"));

		g_PLC_Pow_Ctrl.Unlock();

		return false;
	}

	//this->Show_Out(_T("Send Close PLC Port Pass"));

	g_PLC_Pow_Ctrl.Unlock();

	return true;
}

bool CARM_OP_HUAWEI::QUERY_PKT(PKT_BUFF_V2_S_Ext str_Pkt_V2_Write, RSP_INFO_V2_S_Ext str_Rsp_Info_V2, int i_TimeOut)
{
	time_t t_Start, t_End;

	try
	{
		//��ս���BUFF
		this->Clear_Com_Rx_Buffer();
		//this->m_csError_Code.Format(_T(""));

		//ѭ��
		time(&t_Start);
		//
		do
		{
			if(this->m_iStop_Flag > 0)
			{
				this->m_csError_Code.Format(_T("Operation has been stopped!"));
				throw(0);
			}

			//����ָ��
			this->WRITE_BYTE((char *)str_Pkt_V2_Write.ucr_Buff, str_Pkt_V2_Write.ul_Data_Length);

			//�˶���û����������
			
			if(this->READ_PKT_RSP_V2(str_Rsp_Info_V2, 3) == true)
			{
				//����У����ȷ��Ӧ��
				break;//pass
			}

			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				throw(0);
			}
		}
		while(true);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CARM_OP_HUAWEI::Get_Img_File_Index(CString cs_Img_Name, int * ip_Index)
{
	if(wcsstr(cs_Img_Name, _T("SYSTEM_2K")) != NULL)
	{
		*ip_Index = SYSTEM_IMG_2K_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("BALONG_MODEM_FW_2K")) != NULL)
	{
		*ip_Index = MODEM_IMG_2K_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("SYSTEM_4K")) != NULL)
	{
		*ip_Index = SYSTEM_IMG_4K_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("BALONG_MODEM_FW_4K")) != NULL)
	{
		*ip_Index = MODEM_IMG_4K_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("PTABLE")) != NULL)
	{
		*ip_Index = PTABLE_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("M3BOOT")) != NULL)
	{
		*ip_Index = M3BOOT_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("BSP_MCORE")) != NULL)
	{
		*ip_Index = M3IMAGE_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("FASTBOOT")) != NULL)
	{
		*ip_Index = FASTBOOT_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("BOOT")) != NULL)
	{
		*ip_Index = KERNEL_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("BALONG_MODEM")) != NULL)
	{
		*ip_Index = MODEM_IMG_FILE_I; 
	}
	else if(wcsstr(cs_Img_Name, _T("USERDATA")) != NULL)
	{
		*ip_Index = USERDATA_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("MODEM_DT")) != NULL)
	{
		*ip_Index = DTS_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("LPHY")) != NULL)
	{
		*ip_Index = DSP_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("HIFI")) != NULL)
	{
		*ip_Index = HIFI_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("NV")) != NULL)
	{
		*ip_Index =NVDLOAD_IMG_FILE_I ;
	}
	else
	{
		this->m_csError_Code.Format(_T("FAIL, Get %s.bin/img file index error!"), cs_Img_Name);
		return false;
	}

	return true;
}


bool CARM_OP_HUAWEI::FB_DEVICE_V1()
{
	//ʹ��fastboot.exe̽��һ���Ƿ���Adb�豸

	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;

	unsigned long ulActual_Read;
	CString csTotal_Info;
	int iTemp;
	bool bSuccess_Flag = false;


	try
	{
		//
		csTotal_Info.Format(_T(""));


		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, FB_DEVICE, Create pipe fail!"));
			throw(0);
		}
		//------------------------------ 


		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength() + 128));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);


		//------------------------------ ���dosָ�� ------------------------------ 
		swprintf(wcpCmd, _T("cmd.exe %cc %s\\NB-IoT\\CH%d\\fastboot.exe devices"), '/', this->m_csCurr_Exe_Path, this->m_iClass_Idx);


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
			this->m_csError_Code.Format(_T("FAIL, FB_DEVICE, Create process fail!"));
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
		//��info����.exe�����ѽ�����������read�Ļ�����������һ����
		memset(cpRead_Buff, 0, 2048);
		ulActual_Read = 0;
		//
		ReadFile(hRead_Pipe, cpRead_Buff, 2000, &ulActual_Read, NULL);

		//����info������
		if(ulActual_Read > 0)
		{
			//�ַ�ת��
			memset(wcpData, 0, 2048);
			this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

			csTotal_Info.Format(_T("%s"), wcpData);
			csTotal_Info.Replace(_T('\r'), _T(' '));
			csTotal_Info.Replace(_T('\n'), _T(' '));

			//			this->Save_Trace_Log(csTotal_Info);

			//��������
			csTotal_Info.MakeLower();

			this->Show_Status(csTotal_Info);
		}//if(ulActual_Read > 0

		//����Ƿ�pass
		if((wcsstr(csTotal_Info, _T("mdm")) != NULL) && (wcsstr(csTotal_Info, _T("fastboot")) != NULL))
		{
			bSuccess_Flag = true;//pass
		}
		//------------------------------


		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
	}
	catch(...)
	{
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

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	return bSuccess_Flag;
}
//---------------------------------------------------------
bool CARM_OP_HUAWEI::FB_DL_IMAGE_FILE(const CString strFastBootCmd, CString  csFB_Type_Name,unsigned long csFB_size)
{
	//ʹ��fastboot.exe����һ���ļ�

	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;

	time_t t_Start, t_End;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData, csTemp;
	int iTemp;
	bool bSuccess_Flag = false;

	if(strFastBootCmd.IsEmpty() == true)
	{
		this->m_csError_Code.Format(_T("ERROR, FB_DL_IMAGE_FILE, FastBootCmd is null!"));
		this->Show_Status(this->m_csError_Code);
		return false;
	}

	CString strFastBootCmdTmp;
	strFastBootCmdTmp = strFastBootCmd;

	try
	{
		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, FB_DL_FILE, Create pipe fail!"));
			throw(0);
		}
		//------------------------------ 

		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength() + strFastBootCmd.GetLength() + 2048));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);


		//
		CString strFastbootPath,subPath;
		strFastbootPath.Format(_T("%s\\NB-IoT\\%d\\fastboot.exe"),this->m_csCurr_Exe_Path, this->m_iClass_Idx);

		//ȥ��������fastboot�������滻Ϊfastboot.exeȫ·��
		subPath = strFastBootCmd.Right(strFastBootCmd.GetLength()-8);

		//------------------------------ ���dosָ�� ------------------------------ 
		//ȥ��>> burn_log.txt
		subPath.Replace(_T(">> burn_log.txt"),_T(""));

		swprintf(wcpCmd, _T("cmd.exe %cc %s %s"),'/', strFastbootPath,subPath);

		this->Save_Trace_Log(wcpCmd);


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
			this->m_csError_Code.Format(_T("FAIL, FB_DL_FILE, Create process fail!"));
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
		csTotal_Info.Format(_T(""));
		//��ʼ��ʱ
		time(&t_Start);
		//
		while(bSuccess_Flag == false)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 2000, &ulActual_Read, NULL);

			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				this->Save_Trace_Log(csData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));
			

				//��ʾÿ�εĶ���data
				csTemp.Format(_T("FB Download [ %s ] %s"), csFB_Type_Name, csData);
				this->Show_Status(csTemp);

				//��������
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
			}//if(ulActual_Read > 0


			//����Ƿ��д��󱨳�
			if(wcsstr(csTotal_Info, _T("fail")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, FB_DL_FILE, Download %s fail!"), csFB_Type_Name);
				this->m_iError_Code = ERR_FB_DL_FILE_FAIL;

				throw(3);
			}

			//����Ƿ�pass  
			if((wcsstr(csTotal_Info, _T("finished")) != NULL) && (wcsstr(csTotal_Info, _T("total time")) != NULL))
			{
				//��ʾ������
				this->m_ul_Transmit_Bytes += csFB_size;
// 
				double dwPor =0.0;
				dwPor = (double)this->m_ul_Transmit_Bytes / this->m_ul_DL_All_Bytes;


				this->Show_Progress(dwPor);

				bSuccess_Flag = true;//pass
				break;//�˳�while
			}


			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 20)//��ʱ����fail
			{
				this->m_csError_Code.Format(_T("FAIL, FB_DL_FILE, Time out!"));
				this->m_iError_Code = ERR_FB_DL_FILE_TIME_OUT;
				throw(4);
			}
		}//while(bSuccess_Flag == false
		//------------------------------


		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
	}
	catch(...)
	{
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

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	return bSuccess_Flag;
}

int CARM_OP_HUAWEI::Do_Execute_HI2152_5G()
{
	CString  csData;
	int iDl_Res =RTN_FAIL;
	int iTimeOut = 30;//̽��adb�豸�ĳ�ʱ
	int iTemp=0,iTemp1=0, i;
	time_t t_Start, t_End;

	try
	{

		this->m_bRuning = true;

		int irQDL_Port_List[64];
		int iQDL_Port_Quan;
		bool bEDL_Now = false;
		int iQDLoader_Port =0;

//��֧�ֵ���
#ifndef FW_MULTI
		//------------------------------ �ж�COM���Ƿ����ǡ�EDL��QDLoader�ڡ� ------------------------------ 
		//���
		iQDL_Port_Quan = 0;
		memset(irQDL_Port_List, 0, sizeof(irQDL_Port_List));

		//��PC�����ڵ����С�QDLoader���ڼ�¼
		this->Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(irQDL_Port_List, &iQDL_Port_Quan);

		//�˶ԡ���ǰ���ڡ��ǲ����Ѿ��ǡ�QDLoader�ڡ�
		for(i=0; i<iQDL_Port_Quan; i++)
		{
			if(this->m_iCom_Port == irQDL_Port_List[i])
			{
				//����ǰ���ڡ��Ѿ��ǡ�QDLoader�ڡ�
				bEDL_Now = true;
				break;//�Ѿ���EDLģʽ
			}
		}//for(j=0; j<iQDL_Port_Quan; j++
		//ȷ�������õġ�QDLoader�ڡ�
		if(bEDL_Now == true)
		{
			//�Ѿ���EDLģʽ
			iQDLoader_Port = this->m_iCom_Port;

			csData.Format(_T("QDLoader Port(%d)..."),this->m_iCom_Port);
			this->Show_Out(csData);
		}
		else
		{
			//�л���EDLģʽ
			if(this->Switch_EDL( &iQDLoader_Port ) == false)
			{
				this->Show_Out(m_csError_Code);
				this->Show_Status(this->m_csError_Code);
				throw(2);
			}

		}
#else
		//�Ѿ���EDLģʽ
		iQDLoader_Port = this->m_iCom_Port;
#endif
		//ʹ��burn.exe���ع̼����е�usbloader.bin�ļ�
		this->Show_Status(_T("Start download usbloader ......"));
		this->Show_Out(_T("Start download usbloader ......"));

	

		//Hiburn.exe    b5000            burn       115200    "HUAWEI Mobile Connect - DownLoad port"    D:\images      COM10
		if (false  == this->DL_HiBurn(iQDLoader_Port))
		{
			this->Show_Progress(1.0);
			iDl_Res =RTN_FAIL;
			this->m_bRuning = false;
			return RTN_FAIL;
		}
		else
		{
			this->Show_Progress(1.0);
			this->Show_Status(_T("Download Firwmare Pass"));
		}
		this->m_bRuning = false;

		return RTN_SUCCESS;
	}
	catch(...)
	{
		iDl_Res =RTN_FAIL;

		this->m_bRuning = false;

	}

	this->m_bRuning = false;

	return iDl_Res;
}

int CARM_OP_HUAWEI::Do_Execute_HI2152()
{
	CString  csData;
	int iDl_Res =RTN_FAIL;
	int iTimeOut = 30;//̽��adb�豸�ĳ�ʱ
	int iTemp=0,iTemp1=0, i;
	time_t t_Start, t_End;

	try
	{
		this->m_bRuning = true;

		int irQDL_Port_List[64];
		int iQDL_Port_Quan;
		bool bEDL_Now = false;
		int iQDLoader_Port =0;

//��֧�ֵ���
#ifndef FW_MULTI
		//------------------------------ �ж�COM���Ƿ����ǡ�EDL��QDLoader�ڡ� ------------------------------ 
		//���
		iQDL_Port_Quan = 0;
		memset(irQDL_Port_List, 0, sizeof(irQDL_Port_List));

		//��PC�����ڵ����С�QDLoader���ڼ�¼
		this->Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(irQDL_Port_List, &iQDL_Port_Quan);

		//�˶ԡ���ǰ���ڡ��ǲ����Ѿ��ǡ�8909��QDLoader�ڡ�
		for(i=0; i<iQDL_Port_Quan; i++)
		{
			if(this->m_iCom_Port == irQDL_Port_List[i])
			{
				//����ǰ���ڡ��Ѿ��ǡ�8909��QDLoader�ڡ�
				bEDL_Now = true;
				break;//�Ѿ���EDLģʽ
			}
		}//for(j=0; j<iQDL_Port_Quan; j++
		//ȷ�������õġ�QDLoader�ڡ�
		if(bEDL_Now == true)
		{
			//�Ѿ���EDLģʽ
			iQDLoader_Port = this->m_iCom_Port;

			csData.Format(_T("QDLoader Port(%d)..."),this->m_iCom_Port);
			this->Show_Out(csData);
		}
		else
		{
			//�л���EDLģʽ
			if(this->Switch_EDL( &iQDLoader_Port ) == false)
			{
				this->Show_Out(m_csError_Code);
				this->Show_Status(this->m_csError_Code);
				throw(2);
			}

		}
#else
		iQDLoader_Port = this->m_iCom_Port;
#endif

		//ʹ��burn.exe���ع̼����е�usbloader.bin�ļ�
		this->Show_Status(_T("Start download usbloader ......"));
		this->Show_Out(_T("Start download usbloader ......"));


		//Hiburn.exe    v711            burn       115200    "HUAWEI Mobile Connect - DownLoad port"    D:\images      COM10
		if (false  == this->DL_HiBurn(iQDLoader_Port))
		{
			this->Show_Progress(1.0);

			iDl_Res =RTN_FAIL;
			this->m_bRuning = false;
			return RTN_FAIL;
		}
		else
		{
			this->Show_Progress(1.0);

			this->Show_Status(_T("Download Firwmare Pass"));
		}
		this->m_bRuning = false;

		return RTN_SUCCESS;
	}
	catch(...)
	{
		iDl_Res =RTN_FAIL;

		this->m_bRuning = false;

	}

	this->m_bRuning = false;

	return iDl_Res;
}

//��ѯADB�˿�
bool CARM_OP_HUAWEI::Find_ADB(void)
{
	CUSBDeviceDetection usbDeviceDetect;
	CStringArray strPhysicalAddress,strDeviceInstancePath;
	UINT uDeviceCount;
	int i =0;

	CString strPAD;

	usbDeviceDetect.GetUsbPhysicalAddressList(strPhysicalAddress,strDeviceInstancePath,uDeviceCount);

	if (strPhysicalAddress.GetSize()>0)
	{	
#ifdef FW_MULTI
#ifndef FW_UPGRADE

		for (i=0;i<strPhysicalAddress.GetSize();i++)
		{
			 strPAD = strPhysicalAddress.GetAt(i);
			 if(0 == wcscmp(strPAD ,m_strUSBPortAddress))
			 {
				 return true;
			 }
		}
		return false;
#endif
#else
		return true;
#endif

	}
	else
	{
		return false;
	}

}
//---------------------------------------------------------
bool CARM_OP_HUAWEI::FB_GET_FLASH_SIZE(int*  p_iFlash_size)
{
	//ʹ��fastboot.exe���ػ�ȡFLASH ��С

	//��ȷ����2K����4K
	//fastboot -i 0x12d1 getvar pagesize 2>&1
	//pagesize: 2048
	//finished. total time: 0.008s


	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;

	time_t t_Start, t_End;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData, csTemp;
	int iTemp;
	bool bSuccess_Flag = false;

	try
	{
		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, FB_GET_FLASH_SIZE, Create pipe fail!"));
			throw(0);
		}
		//------------------------------ 

		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength() + 512));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);


		//
		CString strFastbootCmd;
		strFastbootCmd.Format(_T("%s\\NB-IoT\\%d\\fastboot.exe -i 0x12d1 getvar pagesize 2>&1"),this->m_csCurr_Exe_Path, this->m_iClass_Idx);


		//------------------------------ ���dosָ�� ------------------------------ 
		swprintf(wcpCmd, _T("cmd.exe %cc %s"),'/', strFastbootCmd);

		this->Save_Trace_Log(wcpCmd);


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
			this->m_csError_Code.Format(_T("FAIL, FB_GET_FLASH_SIZE, Create process fail!"));
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
		csTotal_Info.Format(_T(""));
		//��ʼ��ʱ
		time(&t_Start);
		//
		while(bSuccess_Flag == false)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 2000, &ulActual_Read, NULL);

			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				this->Save_Trace_Log(csData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));

				//pagesize: 2048
				//finished. total time: 0.008s


				//��ֵת������ȡCOM�ں��룩
				csData.MakeLower();

				wchar_t wcrData[128];

				wchar_t * wcpPoint = NULL;

				swprintf_s(wcrData, _T("%s"), csData);

				wcpPoint = wcsstr(wcrData, _T("pagesize:"));
				if (NULL != wcpPoint)
				{
					wcpPoint += 10;
					*p_iFlash_size = _wtoi(wcpPoint);
				}
				else
				{
					::AfxMessageBox(_T("pagesize get fail!"));
					this->Show_Out(_T("pagesize get fail!"));
					this->Show_Status(_T("pagesize get fail!"));
					throw(0);
				}

				this->Show_Out(csData);

				//��������
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
			}//if(ulActual_Read > 0


			//����Ƿ��д��󱨳�
			if(wcsstr(csTotal_Info, _T("fail")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, FB_GET_FLASH_SIZE, Download fail!"));
				throw(3);
			}

			//����Ƿ�pass  
			if((wcsstr(csTotal_Info, _T("finished")) != NULL) && (wcsstr(csTotal_Info, _T("total time")) != NULL))
			{
				bSuccess_Flag = true;//pass
				break;//�˳�while
			}


			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 20)//��ʱ����fail
			{
				this->m_csError_Code.Format(_T("FAIL, FB_GET_FLASH_SIZE, Time out!"));
				throw(4);
			}
		}//while(bSuccess_Flag == false
		//------------------------------


		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
	}
	catch(...)
	{
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

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	return bSuccess_Flag;
}
//---------------------------------------------------------
bool CARM_OP_HUAWEI::DL_Usbloader(void)
{
	//ʹ��burn.exe���ع̼����е�usbloader.bin�ļ�

// 	------------------------------------------------------------------------
// 
// 		ports number :  1
// 		Starting at: Thu Dec  5 18:58:25 2019
// 		thread number : 0 ,Find Device on : COM33
//		COM33 : Download RAMInit -->
//		 COM33 : Download BOOTLOADER -->
// 		All done at : %s Thu Dec  5 18:58:28 2019
// 
// 		------------------------------------------------------------------------


	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;

	time_t t_Start, t_End;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData, csTemp;
	int iTemp;
	bool bSuccess_Flag = false;

	try
	{
		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, DL_Usbloader, Create pipe fail!"));
			this->m_iError_Code = DL_Usbloader_Create_Pipe_Fail;
			return false;
		}
		//------------------------------ 

		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength()*2 + 512));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);


		//
		CString strFastbootCmd;
		//20200120 --usbloader.bin�ļ���Ϊ�̼����е��ļ�

		strFastbootCmd.Format(_T("%s\\NB-IoT\\%d\\burn.exe %s"),this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_strUsbLoadPath);


		//------------------------------ ���dosָ�� ------------------------------ 
		swprintf(wcpCmd, _T("cmd.exe %cc %s"),'/', strFastbootCmd);

		this->Save_Trace_Log(wcpCmd);


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
			this->m_csError_Code.Format(_T("FAIL, DL_Usbloader, Create process fail!"));
			this->m_iError_Code = DL_Usbloader_Create_Process_Fail;
			bSuccess_Flag =false;
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
		csTotal_Info.Format(_T(""));
		//��ʼ��ʱ
		time(&t_Start);
		//
		while(bSuccess_Flag == false)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 2000, &ulActual_Read, NULL);

			//����info������
			if(ulActual_Read > 0)
			{
				//�ַ�ת��
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				this->Save_Trace_Log(csData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));

				this->Show_Out(csData);

				//��������
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
			}//if(ulActual_Read > 0


			//����Ƿ��д��󱨳�
			if(wcsstr(csTotal_Info, _T("fail")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Usbloader, Download fail!"));
				this->m_iError_Code = ERR_DL_USBLOADER_FAIL;
				bSuccess_Flag =false;
				throw(3);
			}

			//����Ƿ�pass  
			if(wcsstr(csTotal_Info, _T("all done")) != NULL)
			{
				bSuccess_Flag = true;//pass
				break;//�˳�while
			}


			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 30)//��ʱ����fail
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Usbloader, Time out!"));
				this->m_iError_Code = ERR_DL_USBLOADER_TIMEOUT;
				bSuccess_Flag =false;
				throw(4);
			}
		}//while(bSuccess_Flag == false
		//------------------------------


		//�ͷž����ָ��
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
	}
	catch(...)
	{
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

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	return bSuccess_Flag;
}


//---------------------------------------------------------
bool CARM_OP_HUAWEI::DL_HiBurn(int  iQDLoader_Port)
{
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpBengin = NULL;
	wchar_t * wcpEnd = NULL;

	char * cpRead_Buff = NULL;
	double dNew_Percent = 0.0;

	time_t t_Start, t_End;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData;
	int iTemp;
	bool bSuccess_Flag = false;
	int iTimeout = 300;//300���Ϊ240��


	int iBenginPos =-1;
	int iBenginEnd =-1;
	CString strPos;


	try
	{
		//����¶˿���û�г��� 20200511
		this->Show_Status(_T("Check Port......"));
		this->Save_Trace_Log(_T("Check Port......"));

		if(this->Confirm_Dynamic_Com_Port(iQDLoader_Port, 30) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port:%d"), this->m_iCom_Port);
			this->Show_Status(_T("Check Port Fail"));
			this->Save_Trace_Log(_T("Check Port Fail"));
			throw(0);
		}
		this->Show_Status(_T("Check Port Pass"));
		this->Save_Trace_Log(_T("Check Port Pass"));



		//��ʾ
		this->Show_Status(_T("DL_HiBurn......"));
		this->Save_Trace_Log(_T("DL_HiBurn......"));

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, DL_HiBurn, Create pipe fail!"));
			this->m_iError_Code  = DL_HiBurn_Create_Pipe_Fail;
			throw(1);
		}
		//------------------------------ 


		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength()*3 + 256));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 1024);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 1024);



		//------------------------------ ���dosָ�� ------------------------------  


       if ( BB_HI2152 == this->m_iBBchip)
       {
		   if(0 == m_iHS_DL_MODE_M)// Quectel DLoader Port
		   {
			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe v711 burn  115200 \"Quectel DLoader Port\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);

		   }
		   else if(1 == m_iHS_DL_MODE_M)//HUAWEI Mobile Connect - DownLoad port
		   {
			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe v711 burn  115200 \"HUAWEI Mobile Connect - DownLoad port\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);

		   }
// 		   else if(2 == m_iHS_DL_MODE_M)//HUAWEI Mobile Connect - PC UI Interface
// 		   {
// 			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe v711 update  115200 \"HUAWEI Mobile Connect - PC UI Interface\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);
// 
// 		   }
		   else
		   {
			   //...
		   }

       } 
       else if ( BB_HI9500 == this->m_iBBchip)
       {
		   if(0 == m_iHS_DL_MODE_M)// Quectel DLoader Port
		   {
			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe b5000 burn  115200 \"Quectel DLoader Port\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);

		   }
		   else if(1 == m_iHS_DL_MODE_M)//HUAWEI Mobile Connect - DownLoad port
		   {
			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe b5000 burn  115200 \"HUAWEI Mobile Connect - DownLoad port\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);

		   }
// 		   else if(2 == m_iHS_DL_MODE_M)//HUAWEI Mobile Connect - PC UI Interface
// 		   {
// 			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe b5000 update  115200 \"HUAWEI Mobile Connect - PC UI Interface\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);
// 
// 		   }
		   else
		   {
			   //...
		   }

	   }
	   else
	   {
		   AfxMessageBox(_T("BB chip Error,Please Check"));
		   this->m_csError_Code.Format(_T("FAIL, DL_HiBurn,BB chip Error!"));
		   this->m_iError_Code  = DL_HiBurn_Create_Process_Fail;
		   throw(2);
	   }
        
		this->Save_Trace_Log(wcpCmd);


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
			this->m_csError_Code.Format(_T("FAIL, DL_HiBurn, Create process fail!"));
			this->m_iError_Code  = DL_HiBurn_Create_Process_Fail;
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
		csTotal_Info.Format(_T(""));
		//��ʼ��ʱ
		time(&t_Start);
		//
		while(bSuccess_Flag == false)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 1024);
			ulActual_Read = 0;


			if (TRUE == ::PeekNamedPipe(hRead_Pipe,cpRead_Buff,1024,&ulActual_Read,0,0))
			{
				//
				if (ulActual_Read > 0)
				{								
					ReadFile(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, NULL);
					//ע�⣺����˿�״̬�쳣�Ļ����������������һֱ�޷��ء�


					//����info������
					if(ulActual_Read > 0)
					{
						//�ַ�ת��
						memset(wcpData, 0, 1024);
						this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

						//��������
						csData.Format(_T("%s"), wcpData);
						csData.Replace(_T('\r'), _T(' '));
						csData.Replace(_T('\n'), _T(' '));
						csData.MakeLower();



						this->Save_Trace_Log(wcpData);
						this->Show_Out(wcpData);


						//������ʾÿ�εĶ���data
						this->Show_Status_In_Each(wcpData, _T("DL_HiBurn "));


						//��������
						csTotal_Info = csData;

						

						while(1)
						{
							wcpBengin = wcsstr(csTotal_Info.GetBuffer(), _T("[ "));
							if (NULL != wcpBengin)
							{
								csTotal_Info.Format(_T("%s"),wcpBengin+5);
							}
							else
							{
								break;
							}						
						}
						
						wcpBengin = wcsstr(csTotal_Info.GetBuffer(), _T("["));
						wcpEnd = wcsstr(csTotal_Info.GetBuffer(), _T("%]"));

						if ((NULL != wcpBengin)&&(NULL != wcpEnd))
						{

							iBenginPos =  csTotal_Info.Find( _T("["));
							iBenginEnd =  csTotal_Info.Find( _T("%]"));

							strPos = csTotal_Info.Mid(iBenginPos+1,iBenginEnd-iBenginPos-1);
							//�ַ�ת����
							dNew_Percent = _wtof(strPos.GetBuffer());
							dNew_Percent =dNew_Percent/100;

							this->Show_Progress(dNew_Percent);
						}

						//20180425
					}//if(ulActual_Read > 0
				}
				else
				{
					//
					//this->Show_Status(_T("Data Waiting..."));
				}
			}
			else
			{
				//
				//this->Show_Status(_T("PeekNamedPipe NULL..."));

			}



			//����Ƿ��д��󱨳�
			if(wcsstr(csTotal_Info, _T("failed:1")) != NULL) 
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn error!"));
				this->m_iError_Code  = DL_HiBurn_FAIL;
				throw(3);
			}

			if(wcsstr(csTotal_Info, _T("not scanned")) != NULL) 
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn error!"));
				this->m_iError_Code  = DL_HiBurn_FAIL;
				throw(3);
			}

			if(wcsstr(csTotal_Info, _T("failed to be written")) != NULL) 
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn error!"));
				this->m_iError_Code  = DL_HiBurn_FAIL;
				throw(3);
			}

			if(wcsstr(csTotal_Info, _T("oserror")) != NULL) 
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn error!"));
				this->m_iError_Code  = DL_HiBurn_FAIL;
				throw(3);
			}

			if(wcsstr(csTotal_Info, _T("image file download fail")) != NULL) 
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn error!"));
				this->m_iError_Code  = DL_HiBurn_FAIL;
				throw(3);
			}			


			//����Ƿ�pass
			if(wcsstr(csTotal_Info, _T("image file download pass")) != NULL)
			{
				bSuccess_Flag = true;//pass
				break;//�˳�while
			}


			//��ʱ���
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn, Time out!"));
				this->m_iError_Code  = ERR_DL_USBLOADER_TIMEOUT;
				throw(4);
			}
		}//while(bSuccess_Flag == false
		//------------------------------


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
		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	}
	catch(...)
	{
		//��ʾʧ����Ϣ
		this->Show_Status(this->m_csError_Code);
		this->Save_Trace_Log(m_csError_Code);



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

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
		//
		return false;
	}

	//
	return true;
}
void CARM_OP_HUAWEI::Show_Status_In_Each(const wchar_t * wcp_Status, const wchar_t * wcp_Title_In_Each)
{
	CString csTemp;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//��Դ���ݡ���buff��wcstok����ȡ����ʱ��ı�Դ�������ݣ������Ȱ�Դ���ݷ���buff������buff���ݣ����˺�����Դ��
	unsigned long ulBuff_Len = wcslen(wcp_Status) + 8;//���ڴ�š�Դ���ݡ���buff����


	//�����ڴ�
	this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpSrc, ulBuff_Len);


	//���롰Դ���ݡ�
	swprintf(wcpSrc, _T("%s"), wcp_Status);

	//��ȡ��һ��ֵ(0)
	wcpEach = wcstok(wcpSrc, _T("\r\n"));
	//
	while(wcpEach != NULL)
	{
		//��ʾ
		csTemp.Format(_T("%s%s"), wcp_Title_In_Each, wcpEach);
		this->Show_Status(csTemp);
		//this->Save_Trace_Log(csTemp);

		//���ӳ٣��򿴲�����ʾ��Ϣ�ı仯
		Sleep(20);

		//��ȡ��һ��ֵ
		wcpEach = wcstok(NULL, _T("\r\n"));
	}//while(wcpEach != NULL


	//�ͷ��ڴ�
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpSrc);
}


//ÿƬģ������ǰ�������ÿ��ͨ���ĳ̿ص�Դ�ϵ�
void  CARM_OP_HUAWEI::Open_Power_SPC(int iIndex)
{
#ifdef	FW_MULTI
#ifndef FW_UPGRADE

	switch(iIndex)
	{
	case 1:
		{
			//�򿪵�Դ
			this->m_cla_PS_1->Power_Off(1);

			this->Show_Out(_T("----Power_Off----"));

			Sleep(2000);

			//�ϵ�
			//�̿ص�Դ�ϵ�
			if(this->m_cla_PS_1->m_bCtrl_PS == true)
			{
				this->m_cla_PS_1->POWER_ON_V1( 1 );

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
			if(this->m_cla_PS_2->m_bCtrl_PS == true)
			{
				this->m_cla_PS_2->POWER_ON_V1( 1 );

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
			if(this->m_cla_PS_3->m_bCtrl_PS == true)
			{
				this->m_cla_PS_3->POWER_ON_V1( 1 );

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
			if(this->m_cla_PS_4->m_bCtrl_PS == true)
			{
				this->m_cla_PS_4->POWER_ON_V1( 1 );

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

bool CARM_OP_HUAWEI::Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut)
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

bool CARM_OP_HUAWEI::Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(int * ip_QDLoader_Port_List, int * ip_Port_Quan)
{
	//��ע����ȡ��QDLoader�����豸���б�

	wchar_t wcrUSB_PID_VID[64];//DM��
	CString csrUsb_Dev_List[64], csFriendly_Name;
	int iUsb_Dev_Quan, i, iCnt;
	wchar_t wcrData[128];
	wchar_t * wcpPoint = NULL;

	g_cla_HS_Lock_QDLoader_Port.Lock();
	try
	{
		swprintf_s(wcrUSB_PID_VID, _T("VID_12D1&PID_1443"));
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
			
			//��ȡ�豸�ġ�FriendlyName�������磺Quectel DLoader Port (COM11)��
			if(this->m_cla_GFC.Get_Reg_Usb_Dev_Friendly_Name(wcrUSB_PID_VID, csrUsb_Dev_List[i], &csFriendly_Name) == false)
			{				
				//����������������û��FriendlyName
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
		g_cla_HS_Lock_QDLoader_Port.Unlock();
		return false;
	}
	g_cla_HS_Lock_QDLoader_Port.Unlock();
	return true;
}
bool CARM_OP_HUAWEI::Switch_EDL(int * ip_New_QDLoader_Port)
{
	//�л�EDLģʽ����̽�⡢�����������õ�QDLoader��
	int iNew_QDLoader_Port = 0;
	CString csData;

	int iQD_1(0),iQD_2(0),iQD_3(0),iQD_4(0);
	BOOL bFind=FALSE;

	//
	try
	{
		//��ʾ

		csData.Format(_T("Switch EDL mode(Port%d) ......"),this->m_iCom_Port);
		this->Show_Status(csData);
		this->Show_Out(csData);
		this->Save_Trace_Log(csData);


		//��������
		::g_cla_HS_Lock_Enter_EDL.Lock();

		//���buff
		m_iCom_Quan_old = 0;
		memset(m_irCom_List_old, 0, sizeof(m_irCom_List_old));
		if(this->m_cla_GFC.Get_Reg_Com_Port_List(m_irCom_List_old, &m_iCom_Quan_old) == false)
		{		
			this->m_csError_Code.Format(_T("FAIL, Get_Reg_Com_Port_List, Read reg fail!"));
			throw(0);
		}



		//�����л�EDLģʽ��ָ�AT+QDOWNLOAD=1��
		if (false ==Switch_HS_EDL())
		{
			//�ٽ���һ��
			if(false ==Switch_HS_EDL())
			{
				throw(0);
			}
		}

		//----------------------- ȷ��EDL�õġ������ɵġ�QDLoader���ؿڣ��˴�QDLoader���ؿڻᡰ�޷�Ԥ֪�ı�š���-----------------------
		
		if(this->Update_HS_USB_QDLoader_Port(this->m_iCom_Port, &iNew_QDLoader_Port) == false)
		{	
			throw(2);
		}
		else
		{
			//������
			this->Show_Status(_T("Find QDLoader Port Success(First)"));
			this->Show_Out(_T("Find QDLoader Port Success(First)"));
		}
		//��������
		*ip_New_QDLoader_Port = iNew_QDLoader_Port;


		//��ʾ
		this->Show_Status(_T("Switch EDL mode completely"));
		this->Show_Out(_T("Switch EDL mode completely"));
	}
	catch(...)
	{
		//�������
		::g_cla_HS_Lock_Enter_EDL.Unlock();
		//
		return false;
	}


	//�������
	::g_cla_HS_Lock_Enter_EDL.Unlock();
	//
	return true;
}

bool CARM_OP_HUAWEI::Switch_HS_EDL(void)
{
	CString csData;
	try
	{  
		//----------------------- �򿪴��ڣ���һ�Σ�----------------------- 
		csData.Format(_T("Open Port%d ......"),this->m_iCom_Port);
		this->Show_Status(csData);
		this->Show_Out(csData);
		this->Save_Trace_Log(csData);

		//��鴮���Ƿ�ɴ� 
		this->m_cla_Com_Port.m_iPort = this->m_iCom_Port;
		this->m_cla_Com_Port.m_iBaudRate = 115200;
		//this->m_cla_Com_Port.m_dwDTR_CTRL = DTR_CONTROL_DISABLE;
		//this->m_cla_Com_Port.m_dwRTS_CTRL = RTS_CONTROL_DISABLE;

		if(false == this->m_cla_Com_Port.Port_Open(400)) 
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
					if(true == this->m_cla_Com_Port.Port_Open(400))
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
				csData.Format(_T("Open Port fail"));
				this->Show_Status(csData);
				m_csError_Code = csData;
				this->Show_Out(csData);
				this->Save_Trace_Log(csData);
				throw(0);
			}

		}


		//----------------------- �����л�EDLģʽ��ָ�AT+QDOWNLOAD=1��-----------------------
		if(this->Send_HS_EDL_Mode_Cmd() == false)
		{
			throw(1);
		}
		//----------------------- �رմ��� -----------------------
		this->m_cla_Com_Port.Port_Close();
		this->Show_Status(_T("Close DM Port"));
		this->Show_Out(_T("Close DM Port"));
		this->Save_Trace_Log(_T("Close DM Port"));
	}
	catch (...)
	{
		//�رմ���
		this->m_cla_Com_Port.Port_Close();
		return false;
	}
	return true;
}
bool CARM_OP_HUAWEI::Send_HS_EDL_Mode_Cmd(void)
{
	CString strData;

	try
	{
		//20180523	
		strData.Format(_T("Send edl mode cmd(Port%d) ....."),this->m_iCom_Port);
		this->Show_Status(strData);
		this->Show_Out(strData);
		this->Save_Trace_Log(strData);

		if(this->Set_QDOWNLOAD_Mode() == false)
		{
			throw(0);
		}

	}
	catch(...)
	{
		this->m_csError_Code += _T(" [Send_EDL_Mode]");
		this->Show_Out(_T("Send_EDL_Mode Fail"));
		this->Save_Trace_Log(_T("Send_EDL_Mode Fail"));
		return false;
	}

	this->Show_Status(_T("Send_EDL_Mode Pass"));
	this->Show_Out(_T("Send_EDL_Mode Pass"));
	this->Save_Trace_Log(_T("Send_EDL_Mode Pass"));
	return true;
}

bool CARM_OP_HUAWEI::Update_HS_USB_QDLoader_Port(int i_Old_Port, int * ip_New_QDLoader_Port)
{
	int irNew_Port[64], iNew_Quan, i, j,index(0);
	int irQDL_Port_List[64], iQDL_Port_Quan;//���С�QDLoader���ڵļ�¼


	try
	{
		iNew_Quan = 0;
		memset(irNew_Port, 0, sizeof(irNew_Port));

		//��ȡ�������ɡ���com port���б�
		if(this->Get_HS_New_Dynamic_Com_Port(irNew_Port, &iNew_Quan, 60) == false)//add 20180509
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//��������ؿ���ʧ�ˣ���Ҫ����ȷ�����ܵ����ؿ�
		iQDL_Port_Quan = 0;
		memset(irQDL_Port_List, 0, sizeof(irQDL_Port_List));


		//��PC�����ڵ����С�QDLoader���ڼ�¼
		this->Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(irQDL_Port_List, &iQDL_Port_Quan);


		//�˶ԡ������ɵ�COM�ڡ��ǲ��ǡ�QDLoader�ڡ�
		for(i=0; i<iNew_Quan; i++)
		{
			for(j=0; j<iQDL_Port_Quan; j++)
			{
				if(irNew_Port[i] == irQDL_Port_List[j])
				{
					
					*ip_New_QDLoader_Port = irNew_Port[i];
					return true;

				}						
			}//for(j=0; j<iQDL_Port_Quan; j++
		}//for(i=0; i<iNew_Quan; i++
	}
	catch(...)
	{
		this->m_csError_Code.Format(_T("FAIL, Update_USB_QDLoader_Port, No right port!"));
		return false;
	}

	
	return false;
}

bool CARM_OP_HUAWEI::Get_HS_New_Dynamic_Com_Port(int * ip_New_Port_List, int * ip_New_Port_Quan, int i_TimeOut)
{
	time_t t_Start, t_End;
	CString csNew, csData;
	int irCom_List_Min[64], iCom_Quan_Min, irList_Buff[64], iQuan_Buff, x, y, iTime;
	int irCom_List_New[64], iCom_Quan_New;
	int iGet_New_Port_Flag = 0;


	try
	{
		//���buff
		iCom_Quan_Min = 0;
		memset(irCom_List_Min, 0, sizeof(irCom_List_Min));
		iQuan_Buff = 0;
		memset(irList_Buff, 0, sizeof(irList_Buff));
		iCom_Quan_New = 0;
		memset(irCom_List_New, 0, sizeof(irCom_List_New));
		csNew.Format(_T(""));

		Sleep(3000);

		//��ʼֵ��min״̬����buff
// 		if(this->m_cla_GFC.Get_Reg_Com_Port_List(irCom_List_Min, &iCom_Quan_Min) == false)
// 		{		
// 			this->m_csError_Code.Format(_T("FAIL, Get_New_Dynamic_Com_Port, Read reg fail!"));
// 			throw(0);
// 		}
		//
		iCom_Quan_Min = m_iCom_Quan_old;
		memcpy(irCom_List_Min, m_irCom_List_old, sizeof(irCom_List_Min));


		time(&t_Start);
		//
		do
		{
			//����regedit�е�COM�б�
			if(this->m_cla_GFC.Get_Reg_Com_Port_List(irList_Buff, &iQuan_Buff) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Get_New_Dynamic_Com_Port, Read reg fail!"));
				throw(0);
			}
			//
			for(x=0; x<iQuan_Buff; x++)
			{
				for(y=0; y<iCom_Quan_Min; y++)
				{
					if(irList_Buff[x] == irCom_List_Min[y])
					{
						break;
					}
				}
				//����ȫ�µ�com��
				if(y >= iCom_Quan_Min)
				{
					irCom_List_New[iCom_Quan_New] = irList_Buff[x];
					iCom_Quan_New++;

					//
					csData.Format(_T("%d "), irList_Buff[x]);
					csNew += csData;

					//����com port����
					iGet_New_Port_Flag++;
				}
			}//for(x=0; x<iQuan_Buff; x++

			//ϵͳ��ͣ
			if(this->m_iStop_Flag > 0)
			{
				this->m_csError_Code.Format(_T("Operation has been stopped!"));
				throw(0);
			}

			//����Ƿ�ʱ
			time(&t_End);
			iTime = i_TimeOut - (int)difftime(t_End, t_Start);

			if(csNew.GetLength() > 0)
			{
				csData.Format(_T("Detect dynamic new com port( %s) ...... %d"), csNew, iTime);
			}
			else
			{
				csData.Format(_T("Detect dynamic new com port ...... %d"), iTime);
			}

			this->Show_Status(csData);

			if(iTime <= 0)
			{
				this->m_csError_Code.Format(_T("FAIL, Get_New_Dynamic_Com_Port, Timeout fail!"));
				throw(0);
			}
			if(iGet_New_Port_Flag >= 1)
			{
				break;//�˳�while
			}

			//ÿ�ζ�ȡע�����ӳ�
			if(iGet_New_Port_Flag > 0)
			{
				Sleep(1000);
			}
			else
			{
				Sleep(400);
			}
		}
		while(true);


		//����ֵ������
		*ip_New_Port_Quan = iCom_Quan_New;
		//
		for(x=0; x<iCom_Quan_New; x++)
		{
			*(ip_New_Port_List + x) = irCom_List_New[x];
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}
bool CARM_OP_HUAWEI::Set_QDOWNLOAD_Mode(void)
{
	// TODO: Add your control notification handler code here
	CString csCOM,strATCom,csData,csTemp;
	CString info;

	mAT_FirewareName.Format(_T(""));

	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut;
	int iError = 0;

	//
	try
	{
		//��� COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//-------------------------------------------------- 
		iTimeOut = 1;
		csAT_Cmd.Format( _T( "AT+QDLOAD=1" ) );

// 		//
// 		time( &start );
// 		//
// 		do
// 		{
			//��� COM BUFF
			this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

			//����
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
// 			memset( wcrData, 0, sizeof( wcrData ) );
// 			//Sleep( 200 );
// 			//��ȡ
// 			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
// 			//
// 			if(wcsstr(wcrData, _T("OK")))
// 			{
// 				break;//pass, �˳�while
// 			}
			//
// 			time( &finish );
// 			//
// 			if( (int)difftime( finish, start ) >= iTimeOut )
// 			{
// 				throw(0);
// 			}
// 		}
// 		while( true );
		//-------------------------
	}
	catch( ... )
	{
		//fail
		return false;
	}
	return true;

}
//-onlyburn:kv.bin
//-onlyburn:RadioConfig.tlv
//-onlyburn:acpu_3rd_sign_ecc.bin
//-onlyeraseall

int CARM_OP_HUAWEI::Do_Execute_HI2120()
{
	int iDl_Res = RTN_FAIL;
	int iQDLoader_Port = 0;
	CString csData;

	try
	{
		this->m_bRuning = true;
		iQDLoader_Port = this->m_iCom_Port;

		//ʹ��burn.exe���ع̼����е�usbloader.bin�ļ�
		this->Show_Status(_T("Start download usbloader ......"));
		this->Show_Out(_T("Start download usbloader ......"));

		//
		if (false == this->DL_HiBurn_HI2120(iQDLoader_Port))
		{
			this->Show_Progress(1.0);

			// kill HiBurn.exe
			csData.Format(_T("HiBurn%d.exe"), m_iClass_Idx);
			m_cla_GFC.Kill_Process(csData);

			iDl_Res = RTN_FAIL;
			this->m_bRuning = false;
			return RTN_FAIL;
		}
		else
		{
			this->Show_Progress(1.0);

			this->Show_Status(_T("Download Firwmare Pass"));

			// kill HiBurn.exe
			csData.Format(_T("HiBurn%d.exe"), m_iClass_Idx);
			m_cla_GFC.Kill_Process(csData);
		}
		this->m_bRuning = false;

		return RTN_SUCCESS;
	}
	catch (...)
	{
		iDl_Res = RTN_FAIL;

		this->m_bRuning = false;

	}

	this->m_bRuning = false;

	return iDl_Res;
}
//---------------------------------------------------------
void Wait_DL(LPVOID x)
{
	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;

	DWORD exitCode = 0;

	WaitForSingleObject(pSv->m_hi_2120_handle, INFINITE);
	pSv->m_hi_2120_wait_flag = true;
}

//-onlyburn:kv.bin
//-onlyburn:RadioConfig.tlv
//-onlyburn:acpu_3rd_sign_ecc.bin
//-onlyeraseall
//---------------------------------------------------------
bool CARM_OP_HUAWEI::DL_HiBurn_HI2120(int  iQDLoader_Port)
{
	HANDLE hRead_Pipe = NULL;	//ָ����˾����ָ��
	HANDLE hWrite_Pipe = NULL;	//ָ��д�˾����ָ��
	SECURITY_ATTRIBUTES sa;//ָ��ȫ���Խṹ��ָ��
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpBengin = NULL;
	wchar_t * wcpEnd = NULL;


	wchar_t * wcpHead = NULL;

	char * cpRead_Buff = NULL;
	double dNew_Percent = 0.0;

	time_t t_Start, t_End;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData, csInfo;
	int iTemp;
	bool bSuccess_Flag = false;
	int iTimeout = 60;//300���Ϊ240��

	double d_pos = 0.0;


	int iBenginPos = -1;
	int iBenginEnd = -1;
	CString strPos;
	CString strProgress;

	int iPos = 0;

	CString strFirmwarePath = m_str_fwpkg_FW.cs_Path;


	try
	{
		//����¶˿���û�г���
		this->Show_Status(_T("Check Port......"));
		this->Save_Trace_Log(_T("Check Port......"));

		if (this->Confirm_Dynamic_Com_Port(iQDLoader_Port, 30) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port:%d"), this->m_iCom_Port);
			this->Show_Status(_T("Check Port Fail"));
			this->Save_Trace_Log(_T("Check Port Fail"));
			throw(0);
		}
		this->Show_Status(_T("Check Port Pass"));
		this->Save_Trace_Log(_T("Check Port Pass"));



		//��ʾ
		this->Show_Status(_T("DL_HiBurn_HI2120......"));
		this->Save_Trace_Log(_T("DL_HiBurn_HI2120......"));

		// kill HiBurn.exe
		csData.Format(_T("HiBurn%d.exe"), m_iClass_Idx);
		m_cla_GFC.Kill_Process(csData);

		//------------------------------ ���� pipe ------------------------------ 
		//Windows��ʹ�ý϶�Ĺܵ�Ҳ�������ܵ�����ͨ��API����CreatePipe����
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������
		sa.bInheritHandle = TRUE;//һ��ҪΪTRUE����Ȼ������ܱ��̳�

		if (CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE)
		{
			this->m_csError_Code.Format(_T("FAIL, DL_HiBurn_HI2120, Create pipe fail!"));
			this->m_iError_Code = DL_HiBurn_HI2120_Create_Pipe_Fail;
			throw(1);
		}
		//------------------------------ 


		//�����ڴ�
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength() * 3 + 1024));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 1024);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 1024);

		//------------------------------ ���dosָ�� ------------------------------
		//HiBurn.exe - com:72 - bin : D : \BC280VAAR01A01_BETA1106.fwpkg - signalbaud:921600
		//swprintf(wcpCmd, _T("cmd.exe %cc %s\\HI2120\\%d\\HiBurn%d.exe -com:%d -bin:%s -signalbaud:921600  -console"), '/', this->m_csCurr_Exe_Path, this->m_iClass_Idx, this->m_iClass_Idx, iQDLoader_Port, strFirmwarePath);
		
#ifdef FW_MULTI
#ifndef FW_UPGRADE
		//20210420 ����һ��ð��
		//swprintf(wcpCmd, _T("%s\\HI2120\\%d\\HiBurn%d.exe -com:%d -erasemode:1 -bin:%s -signalbaud:921600  -console"), this->m_csCurr_Exe_Path, this->m_iClass_Idx, this->m_iClass_Idx, iQDLoader_Port, strFirmwarePath);
		swprintf(wcpCmd, _T("%s\\HI2120\\%d\\HiBurn%d.exe -com:%d -erasemode:1 -bin:%s -signalbaud:921600  -console"), this->m_csCurr_Exe_Path, this->m_iClass_Idx, this->m_iClass_Idx, iQDLoader_Port, strFirmwarePath);


#endif
#else
		//swprintf(wcpCmd, _T("%s\\HI2120\\%d\\HiBurn%d.exe -com:%d -bin:%s -signalbaud:921600  -console"), this->m_csCurr_Exe_Path, this->m_iClass_Idx, this->m_iClass_Idx, iQDLoader_Port, strFirmwarePath);

		//HiBurn.exe - com:113 - bin : C : \Users\w00522586\Desktop\BC280VCNAAR01A01_BETA0118.fwpkg - onlyburn:params_area_ecc_combine.bin - onlyburn : sebl_hisi_sign_enc.bin - onlyburn : semain_hisi_sign_enc.bin - onlyburn : protocol_hisi_sign.bin - onlyburn : acpu_3rd_sign_ecc.bin

		swprintf(wcpCmd, _T("%s\\HI2120\\%d\\HiBurn%d.exe -com:%d -bin:%s -onlyburn:params_area_ecc_combine.bin -onlyburn:sebl_hisi_sign_enc.bin -onlyburn:semain_hisi_sign_enc.bin -onlyburn:protocol_hisi_sign.bin -onlyburn:acpu_3rd_sign_ecc.bin -signalbaud:921600  -console"), this->m_csCurr_Exe_Path, this->m_iClass_Idx, this->m_iClass_Idx, iQDLoader_Port, strFirmwarePath);

#endif
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
			this->m_csError_Code.Format(_T("FAIL, DL_HiBurn_HI2120, Create process fail!"));
			this->m_iError_Code = DL_HiBurn_HI2120_Create_Process_Fail;
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

		//�½�һ���ȴ������߳�
		//////////////////////////////////////////////////////////////////////////
		this->m_hi_2120_handle = pi.hProcess;
		this->m_hi_2120_wait_flag = false;
		hWaitThread_Handle = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)Wait_DL,
			(void *)this,
			0,
			&dwWait_ID);

		//////////////////////////////////////////////////////////////////////////
		//
		while (bSuccess_Flag == false)
		{
			//��pipe�ܵ�
			memset(cpRead_Buff, 0, 1024);
			ulActual_Read = 0;
			//------------------------------------------------
			if (true == this->m_hi_2120_wait_flag)
			{
				this->Save_Trace_Log(_T("hi_2120_wait_flag  ......"));
				this->Show_Out(csData);

				bSuccess_Flag = true;
				throw(4);
			}
			//------------------------------------------------

			if (TRUE == ::PeekNamedPipe(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, 0, 0))
			{
				//
				if (ulActual_Read > 0)
				{
					ReadFile(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, NULL);
					//ע�⣺����˿�״̬�쳣�Ļ����������������һֱ�޷��ء�


					//����info������
					if (ulActual_Read > 0)
					{
						//�ַ�ת��
						memset(wcpData, 0, 1024);
						this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

						//��������
						csData.Format(_T("%s"), wcpData);

						this->Show_Out(csData);
						this->Save_Trace_Log(csData);

						csData.Replace(_T('\r'), _T(' '));
						csData.Replace(_T('\n'), _T(' '));
						csData.MakeLower();

						csTotal_Info += csData;


						//��ʾģ��Reset
						if (wcsstr(csTotal_Info.GetBuffer(), _T("please reset device")) != NULL)
						{
							//��ʾ
							this->Show_Status(_T("Reset ......"));
							this->Save_Trace_Log(_T("Reset ......"));
						}
						//--------------------------------------------
						//progress[0.33%]
						if (csTotal_Info.Find(_T("100.00")) >= 0)
						{
							this->Show_Progress(1.0);
							this->Show_Status(_T("DL BIN Pass"));
							this->Show_Progress(1.0);
							bSuccess_Flag = true;
							Sleep(1000);
							break;
						}


						wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("progress["));
						if (NULL != wcpHead)
						{
							csTotal_Info.Format(_T("%s"), wcpHead + 9);

							wcpEnd = wcsstr(csTotal_Info.GetBuffer(), _T("%]"));
						
							iPos = csTotal_Info.Find(_T("%]"));
							if (iPos >0)
							{
								strPos = csTotal_Info.Left(iPos);

								if (strPos.Find(_T("100"))>=0)
								{
									this->Show_Progress(1.0);
									this->Show_Status(_T("DL BIN Pass"));
									this->Show_Progress(1.0);
									bSuccess_Flag = true;
									Sleep(1000);
									break;
								}
								else
								{
									strProgress.Format(_T("progress[%s%%]"), strPos);
									//progress[0.57%]
									this->Show_Status(strProgress);
								}
								//�ַ�ת����
								dNew_Percent = _wtof(strPos);

								dNew_Percent = dNew_Percent / 100.0;

								this->Show_Progress(dNew_Percent);

								csTotal_Info.Format(_T("%s"), wcpEnd + 2);
							}

						}
					}//if(ulActual_Read > 0
				}
				else
				{
					//
					//this->Show_Status(_T("Data Waiting..."));
				}
			}
			else
			{
				//
				//this->Show_Status(_T("PeekNamedPipe NULL..."));
			}

			//��ʱ���
			time(&t_End);
			//
			if ((int)difftime(t_End, t_Start) >= iTimeout)//��ʱ����fail
			{
				bSuccess_Flag = true;
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn_HI2120, Time out!"));
				this->m_iError_Code = ERR_DL_USBLOADER_TIMEOUT;
				throw(4);
			}
		}//while(bSuccess_Flag == false
		//------------------------------
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

	}
	catch (...)
	{
		//��ʾʧ����Ϣ
		this->Show_Status(this->m_csError_Code);
		this->Save_Trace_Log(m_csError_Code);



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

		//����߳̽�����ķ�����Ϣ
		DWORD exitCode = 0;

	    if (GetExitCodeProcess(pi.hProcess, &exitCode))
	    {
	      TRACE(_T("Exit code = %d/n"), exitCode);
		  switch (exitCode)
		  {
			case  ERR_TIMEOUT_WAIT_HISILICON://��2��	��дʱѡ���˴����COM�ڣ���ѡ����AT�ڣ�����д��ʼ�׶�δ���и�λ
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_HISILICON"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_HISILICON;
			break;
			case  ERR_SELOADER_ACK_IS_FAIL://			15
				this->m_csError_Code.Format(_T("ERR_SELOADER_ACK_IS_FAIL"));
				this->m_iError_Code = 0x03000000 | ERR_SELOADER_ACK_IS_FAIL;
			break;
			case  ERR_TIMEOUT_WAIT_BURN_START_C:		//4 // ��λ���豸��HiBurn������ʼ�ź�CCC֮ǰ�µ�
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_BURN_START_C"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_BURN_START_C;
			break;
			case  ERR_TIMEOUT_WAIT_BURN_INIT_ACK:		//5 // ��HiBurn�յ���ʼ����ACK�ظ�֮ǰ�µ�
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_BURN_INIT_ACK"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_BURN_INIT_ACK;
			break;
			case  ERR_TIMEOUT_WAIT_BURN_INTER_ACK:	//6 //��HiBurn�յ��м����ɰ���ACK�ظ�֮ǰ�µ�
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_BURN_INTER_ACK"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_BURN_INTER_ACK;
			break;
			case  ERR_TIMEOUT_WAIT_BURN_EOT_C:		//7 // ��HiBurn�յ������ź�C֮ǰ�µ�
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_BURN_EOT_C"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_BURN_EOT_C;
			break;
			case  ERR_TIMEOUT_WAIT_ZERO_ACK:			//8 //��HiBurn�յ���������ACK֮ǰ�µ�
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_ZERO_ACK"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_ZERO_ACK;
			break;
			case  ERR_TIMEOUT_WAIT_SELOADER_ACK_TIME: // 14 // ��HiBurn�յ�SELOADER��ACK֮ǰ�µ�
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_SELOADER_ACK_TIME"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_SELOADER_ACK_TIME;
			break;
			case  ERR_OPEN_FILE:						//16  //�̼�����ռ�ã�
				this->m_csError_Code.Format(_T("ERR_OPEN_FILE"));
				this->m_iError_Code = 0x03000000 | ERR_OPEN_FILE;
			break;
			case   ERR_FW_NOT_FIND:					//52
				this->m_csError_Code.Format(_T("ERR_FW_NOT_FIND"));
				this->m_iError_Code = 0x03000000 | ERR_FW_NOT_FIND;
			break;
			  
		  default:
			  
			  this->m_csError_Code.Format(_T("ERR_TIMEOUT_%d"), exitCode);
			  this->m_iError_Code = 0x03000000 | exitCode;
			  break;
		  }

		  this->Save_Trace_Log(this->m_csError_Code);
		  this->Show_Status(this->m_csError_Code);

	    }
	    else
	    {
	      TRACE(_T("GetExitCodeProcess() failed: %ld/n"), GetLastError());
	      ASSERT(0);
	    }

		//
		return false;
	}
	//����߳̽�����ķ�����Ϣ
	DWORD exitCode = 0;
	if (GetExitCodeProcess(pi.hProcess, &exitCode))
	{
		TRACE(_T("Exit code = %d/n"), exitCode);
	}
	else
	{
		TRACE(_T("GetExitCodeProcess() failed: %ld/n"), GetLastError());
		ASSERT(0);
	}
	//
	return true;
}