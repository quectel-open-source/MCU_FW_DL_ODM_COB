#pragma once
#include "gfc.h"
#include "Com_Port.h"

#ifdef FW_MULTI
#ifndef FW_UPGRADE
#include "Power_Supply.h"
#endif
#endif

#define   FB_MAX   64

typedef struct tagFWPKG_FILE_S
{
	CString cs_Name;
	CString cs_Path;
	bool b_Enable_DL;
}FWPKG_FILE_S;

typedef struct tagSET_FILE_S
{
	CString cs_Set;
	CString cs_Name;
}SET_FILE_S;

typedef struct tagHUAWEI_MBN_FILE_S
{
	CString cs_Name;
	CString cs_Path;
	unsigned long ul_File_Size;
	bool b_Enable_DL;
}HUAWEI_IMG_FILE_S;

#define VERSION_G_INFO  _T("BC95G")
#define VERSION_BC28_INFO  _T("BC28")
#define VERSION_BC68_INFO  _T("BC68")
#define VERSION_BC35_INFO  _T("BC35G")
#define VERSION_BC95D_INFO  _T("BC95D")
#define VERSION_BC39_INFO  _T("BC39")

#define VERSION_BC95B5JB_INFO  _T("BC95B5JB")
#define VERSION_BC95B8JB_INFO  _T("BC95B8JB")
#define VERSION_BC95B5JC_INFO  _T("BC95B5JC")
#define VERSION_BC95B8JC_INFO  _T("BC95B8JC")

#define VERSION_BC95B5JD_INFO  _T("BC95B5JD")
#define VERSION_BC95B8JD_INFO  _T("BC95B8JD")
#define VERSION_BC35GJD_INFO  _T("BC35GJD")


#define  ERR_CREATE_PIPE_FAIL		0x03002033
#define  ERR_CREATE_PROCESS_FAIL    0x03002034
#define  ERR_SSB_DL_TIMEOUT			0x03003002
#define  ERR_SSB_DL_FAIL			0x03002035
#define  ERR_FW_DL_TIMEOUT          0x03002036
#define  ERR_FW_DL_FAIL				0x03002037
#define  ERR_OPEN_PORT_FAIL			0x03002029
#define  ERR_FW_ReadFile_NULL				0x03002030
#define  ERR_SSB_ReadFile_NULL				0x03002031
#define  ERR_IMEI_LEN						0x03002028
#define  ERR_FIND_FW_VERSION				0x03002032
#define  ERR_FW_VER_NOT_MATCH_DB			0x03002033
#define  ERR_W_IMEI							0x03002023
#define  ERR_W_SN							0x03002026
#define  ERR_FIND_IMEI_AT					0x03002024
#define  ERR_IMEI_AT_NOT_MATCH_DB			0x03002025
#define  ERR_FIND_SN_AT						0x03002027
#define  ERR_CONFIRM_PORT								0x03002028
#define  ERR_FIND_UEUPDATE_EXE_FAIL						0x03002029
#define  ERR_FIND_PATH_HAS_SPACE						0x03002030

#define  ERR_DL_USBLOADER_FAIL					0x03002031
#define  ERR_DL_USBLOADER_TIMEOUT				0x03002032
#define  ERR_ADB_PORT_ADDRESS_NULL				0x03002033
#define  ERR_ADB_PORT_ADDRESS_FIND_TIME_OUT		0x03002034
#define  ERR_GET_FLASH_SIZE						0x03002035
#define  ERR_FB_DL_FILE_FAIL					0x03002036
#define  ERR_FB_DL_FILE_TIME_OUT				0x03002037

#define  DL_Usbloader_Create_Pipe_Fail         0x03002038
#define  DL_Usbloader_Create_Process_Fail      0x03002039

#define  DL_HiBurn_Create_Pipe_Fail    0x03002040
#define  DL_HiBurn_Create_Process_Fail  0x03002041
#define  DL_HiBurn_FAIL             0x03002042

#define  DL_HiBurn_HI2120_Create_Pipe_Fail    0x03002043
#define  DL_HiBurn_HI2120_Create_Process_Fail  0x03002044
#define  DL_HiBurn_HI2120_FAIL             0x03002045

//20201209 HI B200��HIBURN
#define  ERR_TIMEOUT_WAIT_HISILICON			1
#define  ERR_SELOADER_ACK_IS_FAIL			15
#define  ERR_TIMEOUT_WAIT_BURN_START_C		4 // ��λ���豸��HiBurn������ʼ�ź�CCC֮ǰ�µ�
#define  ERR_TIMEOUT_WAIT_BURN_INIT_ACK		5 // ��HiBurn�յ���ʼ����ACK�ظ�֮ǰ�µ�
#define  ERR_TIMEOUT_WAIT_BURN_INTER_ACK	6 //��HiBurn�յ��м����ɰ���ACK�ظ�֮ǰ�µ�
#define  ERR_TIMEOUT_WAIT_BURN_EOT_C		7 // ��HiBurn�յ������ź�C֮ǰ�µ�
#define  ERR_TIMEOUT_WAIT_ZERO_ACK			8 //��HiBurn�յ���������ACK֮ǰ�µ�
#define  ERR_TIMEOUT_WAIT_SELOADER_ACK_TIME 14 // ��HiBurn�յ�SELOADER��ACK֮ǰ�µ�
#define  ERR_OPEN_FILE						16  //�̼�����ռ�ã�
#define  ERR_TIMEOUT_WAIT_BURN_START_C		4
#define  ERR_FW_NOT_FIND					52



typedef struct tagPKT_BUFF_V2_S_Ext
{
	unsigned char ucr_Buff[32];
	unsigned long ul_Data_Length;
}PKT_BUFF_V2_S_Ext;

typedef struct tagRSP_INFO_V2_S_Ext
{
	unsigned char ucr_Expect_Rsp[32];
	unsigned int ui_Rsp_Data_Length;
	unsigned int ui_Rsp_Pkt_Quan;
}RSP_INFO_V2_S_Ext;

//.img�ļ���index
typedef enum tagHUAWEI_IDX_IMG_FILE_E
{
	SYSTEM_IMG_2K_FILE_I  = 0,
	MODEM_IMG_2K_FILE_I	  = 2,
	SYSTEM_IMG_4K_FILE_I  = 3,
	MODEM_IMG_4K_FILE_I		= 4,

	PTABLE_IMG_FILE_I	= 5,
	M3BOOT_IMG_FILE_I 	= 6,
	M3IMAGE_IMG_FILE_I 	= 7,
	FASTBOOT_IMG_FILE_I 	= 8,
	KERNEL_IMG_FILE_I 	= 9,
	DTS_IMG_FILE_I 	= 10, 
	DSP_IMG_FILE_I 	= 11, 
	HIFI_IMG_FILE_I 	= 12,  
	NVDLOAD_IMG_FILE_I 	= 13,  
	MODEM_IMG_FILE_I = 14,
	USERDATA_IMG_FILE_I = 15,

	//������ص���ͨ.img�ļ�
	MAX_IMG_FILE_HUAWEI
}HUAWEI_IDX_IMG_FILE_E;



class CARM_OP_HUAWEI
{
public:
	CARM_OP_HUAWEI(void);
	~CARM_OP_HUAWEI(void);

	HWND m_hWnd_View;
	bool m_bRuning;
	int m_iStop_Flag;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iClass_Idx;

	int m_Ts_Num;
	CGFC m_cla_GFC;
	void Show_Status(const wchar_t * wcp_Status);
	void Show_Out(const wchar_t * wcp_Status);
	void SetPLC_Open_Path(void);
	void SetPLC_Close_Path(void);
	int Do_Execute(void);
	bool LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path);
	int m_iOP_TYPE;
	bool DL_FLS_FILE_IN_EXE();
	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	void Show_Progress(double d_Pgr_Percent);
	bool m_bGet_Debug_Log;
	CString m_csCurr_Exe_Path;
	FWPKG_FILE_S m_str_fwpkg_FW;
	int m_iCom_Port;
	
	void DO_DL_OP(int i_Index);
	bool Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size);

	BOOL m_bCheckSaveLog;
	BOOL m_DL_FLAG;
	BOOL m_bPowFlag;
	int  m_iPsc;
	int  m_iPpc;
	int  m_iPac;

	int  m_iPsuc;
	int  m_iPauc;
	int  m_iPgc;

	int iPsca_num;
	int iPpa_num;
	int iPsc_num;
	int iPac_num;
	int iSgc_num;

	int  m_iUpdateall;

	int m_iBC95BaudSwitch;
	int m_iBcg_ssb_COM_update_B200;
	int  m_iBcg_ssb_COM_update_B200_B180;
	int  m_PassNumb;
	int  m_ErrorNumb;

	bool  m_bBootFlag; //150ƽ̨�����û�а汾

	int m_iTimeInterval;

	int m_iBC95_EraseSwitch;

	int m_iBC95_EraseSwitchFirst;

	int m_iOP_Robot;
	int  m_iBC95_DC_ResetTime;

	BOOL  m_iBC95_DC_ResetTime_Flag;

	int Do_Execute_BC95G(void);
	int DL_FLS_FILE_IN_EXE_BCG(CString strCommand);

	//�Ƿ��ô�������SSB.BIN ----��Ҫ������Ƭ��Ҫ��������SSB.BIN
	int m_iBcg_ssb_com;//1:������Ƭ��Ҫ��������SSB.BIN
	int DL_FLS_FILE_IN_EXE_BCG_COM(CString strCommand);

	int m_iREP_NO_SSB;

	CCom_Port m_cla_Com_Port;
	CString  mAT_IMEI;
	CString  mAT_SN;//2019092 add SN
	CString  mAT_FirewareName;
	CString  mDB_FirewareName;


	bool OnReadAtImei(void);
	void OnWriteAtImei(void);
	bool ERASE_FLS_FILE_IN_EXE( int iClass_Idx);
	int FindVer(int index);

	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);

	BOOL m_blWrCon;//�����Ƿ���Ҫ����RF�Ȳ���
	BOOL m_bSSB_Update;//�����Ƿ���Ҫ����SSB�ļ� 180--200SP1

	BOOL m_bWriteKV;//����ʱ������Ҫ
	BOOL m_bWriteFEC;

	int  m_iHS_DL_MODE_M;


	int m_iFindFlagBCG_BC95D;

	bool Do_Execute_Erase();

	CString  mBar_IMEI;//�ܿ��ṩ��IMEI
	CString  mBar_SN;//�ܿ��ṩ��SN

	bool WRITE_IMEI_SN();
	bool OnWriteAtImei_Auto(void);
	bool OnWriteAtSN_Auto(void);
	bool OnReadAtSN_Auto(void);

	bool NB_Port_Open(int iBaudRate);
	void NB_Port_Close();

	int m_iFindVerFlag;//���ģ�����Ѿ���Firmware�汾

	int m_i150ResetTime1;//150ƽ̨����SSB��λ1
	int m_i150ResetTime2;//150ƽ̨����SSB��λ2
	int m_i150ResetTime3;//150ƽ̨����FW��λ1
	int m_i150ResetTime4;//150ƽ̨����FW��λ2

	//��ѯ�Ƿ���Բ�ѯ���汾��
	bool FindVerAuto(void);

	bool m_bPLC_PowFlag;//�Ƿ���PLC���� true
	int  m_iPLC_Port;

	//��GNSSʹ�� BC39
	// �����ڷ��� AT+QGNSSC=1  Power on GNSS 
	bool Set_QGNSSC_PowerOn(void);

	//�ر�GNSSʹ�� BC39
	// �����ڷ��� AT+QGNSSC=0  Power OFF GNSS 
	bool Set_QGNSSC_PowerOFF(void);

	BOOL m_bNetFlagDL;//�����־λ���>=DL_FW,������Ѿ����ڰ汾����Ҫ���Ͳ��� TRUE

	bool m_bIsRepMo;//�Ƿ��Ƿ�������

	BOOL m_bOutLine;//���ߺ����߷�ʽ

	int m_iDL_flag;//0:��ʼ��  1:SSB�Ѿ��������  2:������ʧ��

	//������һ·PLC�ĵ�Դ
	BOOL OpenPLC_Power_Path_Huawei(int Index);
	//�ر�����һ·PLC�ĵ�Դ
	BOOL ClosePLC_Power_Path_Huawei(int Index);

	CCom_Port m_cla_Com_PLC_Power_Huawei;

	bool  CheckSSB(void);

	HANDLE m_hThread_PLC_Pow_Handle_BC;
	DWORD  m_dwThread_PLC_Pow_BC_ID;

	HANDLE m_hThread_PLC_Pow_Handle_BC_1;
	DWORD  m_dwThread_PLC_Pow_BC_ID_1;

	HANDLE m_hThread_PLC_Pow_Handle_BC_2;
	DWORD  m_dwThread_PLC_Pow_BC_ID_2;

	HANDLE m_hThread_PLC_Pow_Handle_BC_3;
	DWORD  m_dwThread_PLC_Pow_BC_ID_3;

	HANDLE m_hThread_PLC_Pow_Handle_BC_4;
	DWORD  m_dwThread_PLC_Pow_BC_ID_4;

	HANDLE m_hThread_PLC_Pow_Handle_BC_5;
	DWORD  m_dwThread_PLC_Pow_BC_ID_5;

	HANDLE m_hThread_PLC_Pow_Handle_BC_6;
	DWORD  m_dwThread_PLC_Pow_BC_ID_6;

	HANDLE m_hThread_Auto_PLC_Pow_Handle_BC;
	DWORD  m_dwThread_Auto_PLC_Pow_BC_ID;

	HANDLE m_hThread_Auto_SPC_Pow_Handle_BC;
	DWORD m_dwThread_Auto_SPC_Pow_BC_ID;

	//��˼120ƽ̨���غ���
	int Do_Execute_BC95_120();

	bool WRITE_BYTE(char * cp_Send_Data, unsigned long ul_Data_Quan);
	void Clear_Com_Rx_Buffer(void);
	bool READ_PKT_RSP_V2(RSP_INFO_V2_S_Ext str_Rsp_Info_V2, int i_TimeOut);
	bool READ_PKT_RSP_SYNC_V2(RSP_INFO_V2_S_Ext str_Rsp_Info_V2, int i_TimeOut);
	bool READ_BYTE(unsigned char ** ucp2_Buff_Pointer, unsigned long * ulp_Data_Length);
	bool QUERY_PKT(PKT_BUFF_V2_S_Ext str_Pkt_V2_Write, RSP_INFO_V2_S_Ext str_Rsp_Info_V2, int i_TimeOut);
	bool Send_Open_PLC_Mode_Cmd(void);
	bool Send_Close_PLC_Mode_Cmd(void);

	CString m_csOC;//ģ��OC��Ϣ

	int m_iBBchip;
	bool LOAD_FW_FILE_HI2110(const wchar_t * wcp_FW_File_Path);
	bool LOAD_FW_FILE_HI2152(const wchar_t * wcp_FW_File_Path);

	HUAWEI_IMG_FILE_S m_strr_Img_File_List[FB_MAX];

	bool Get_Img_File_Index(CString cs_Img_Name, int * ip_Index);


	CString m_strFastBootCmd[FB_MAX];
	int m_iFastBootCmdNum;

	bool FB_DEVICE_V1();

	bool FB_DL_IMAGE_FILE(const CString strFastBootCmd, CString  csFB_Type_Name,unsigned long csFB_size);

	unsigned long m_ul_DL_All_Bytes;
	unsigned long m_ul_Transmit_Bytes;

	int Do_Execute_HI2110();
	int Do_Execute_HI2152();

	CString m_strUSBPortAddress;

	//��ѯADB�˿�
	bool Find_ADB(void);
	bool FB_GET_FLASH_SIZE(int*  p_iFlash_size);
	bool DL_Usbloader(void);

	SET_FILE_S m_SET_Files[32];//����������õĿɱ����

	CString m_strUsbLoadPath;//usbloader.bin��·��

	int   m_iRebootFlag;// LTEƽ̨�Ƿ���Ҫ����

	bool DL_HiBurn(int  iQDLoader_Port);

	void Show_Status_In_Each(const wchar_t * wcp_Status, const wchar_t * wcp_Title_In_Each);

	int Do_Execute_HI2152_5G();

	//�Զ��߳̿ص�Դ�ϵ�
	void Set_PowerOn_SPC(int i_Index);


#ifdef FW_MULTI
#ifndef FW_UPGRADE
	CPower_Supply* m_cla_PS_1;
	CPower_Supply* m_cla_PS_2;
	CPower_Supply* m_cla_PS_3;
	CPower_Supply* m_cla_PS_4;
#endif
#endif



	//ÿƬģ������ǰ�������ÿ��ͨ���ĳ̿ص�Դ�ϵ�
	void  Open_Power_SPC(int iIndex);

	bool Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut);

	bool Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(int * ip_QDLoader_Port_List, int * ip_Port_Quan);

	bool Switch_EDL(int * ip_New_QDLoader_Port);

	bool Switch_HS_EDL(void);

	bool Send_HS_EDL_Mode_Cmd(void);

	bool Update_HS_USB_QDLoader_Port(int i_Old_Port, int * ip_New_QDLoader_Port);

	bool Get_HS_New_Dynamic_Com_Port(int * ip_New_Port_List, int * ip_New_Port_Quan, int i_TimeOut);

	bool Set_QDOWNLOAD_Mode(void);

	int m_irCom_List_old[64], m_iCom_Quan_old;

	int Do_Execute_HI2120();

	//---------------------------------------------------------
	bool DL_HiBurn_HI2120(int  iQDLoader_Port);

	//---------------------------------------------------------
	HANDLE m_hi_2120_handle;
	HANDLE hWaitThread_Handle;
	DWORD  dwWait_ID;
	bool   m_hi_2120_wait_flag;
	//---------------------------------------------------------



};
