#pragma once

#include <locale>//ͷ�ļ�
//#include "gfc.h"


//DLL���롢�������
#ifdef DLL_IMPLEMENT
#define DLL_IMP _declspec(dllexport)
#else
#define DLL_IMP _declspec(dllimport)
#endif


//
//�������
#define RTN_SUCCESS					0
#define RTN_ERROR					-1
#define RTN_FAIL					1
//Ӧ����

#define NO_RESPONSE					2
#define READ_ERROR_DATA				3
#define NO_RIGHT_RESPONSE			4
#define NO_ENOUGH_RESPONSE			5


// #define SQL_BOF_ERROR				5
// #define SQL_EOF_ERROR				6
// #define SQL_BOF_EOF_ERROR			7
// #define SQL_FIND_TARGET				8
// #define SQL_FIND_NO_TARGET			9



//prototype of callback function
typedef void (__stdcall * CALLBACK_SHOW_REDIT_INFO)(const wchar_t * wcp_Msg, void * p_usr_Arg);
typedef void (__stdcall * CALLBACK_SHOW_STATE)(const wchar_t * wcp_State, void * p_usr_Arg);
typedef bool (__stdcall * CALLBACK_SHOW_TEST_RESULT)(const wchar_t * wcp_Name, double d_Low, double d_High, double d_Data, void * p_usr_Arg);
typedef void (__stdcall * CALLBACK_SAVE_TRACE_LOG)(const wchar_t * wcp_Trace_Log, void * p_usr_Arg);
typedef void (__stdcall * CALLBACK_SHOW_TITLE)(const wchar_t * wcp_Title, void * p_usr_Arg);
typedef void (__stdcall * CALLBACK_SET_PLC_POWER)(void * p_usr_Arg);

#if 0
//callback �ṹ��
typedef struct tagQTI_PROG_ARG
{
	//qti progress callback
	void *							p_usr_Arguments;
	CALLBACK_SHOW_REDIT_INFO		cb_Show_REdit_Info;
	CALLBACK_SHOW_STATE				cb_Show_State;
	CALLBACK_SHOW_TEST_RESULT		cb_Show_Test_Result;
	CALLBACK_SAVE_TRACE_LOG			cb_Save_Trace_Log;
	CALLBACK_SHOW_TITLE				cb_Show_Title;
}QTI_PROG_ARG;


typedef struct tagSQL_BASE_S
{
	CString cs_ITEM;
	CString cs_DATA;
}SQL_BASE_S;

typedef struct tagSQL_RECORD_SET_S
{
	int i_Index;//�ӡ�1����ʼ����
	int i_Item_Quantity;
	SQL_BASE_S strr_Base_Data[45];
	CString cs_KEY;
}SQL_RECORD_SET_S;
#endif

typedef struct tagFILE_BASE_S
{
	CString cs_Name;
	CString cs_Path;
}FILE_BASE_S;



//Login_User
#define LOG_IN_USER_NAME					0
#define LOG_IN_USER_PASSWORD				1
#define LOG_IN_USER_MODE					2
#define LOG_IN_USER_REMARK					3
#define LOG_IN_USER_RESERVED				4

//Flash����
#define NDF_FLASH					0
#define SST_FLASH					1
#define TOSHIBA_FLASH				2
#define ESMT_FLASH					3
#define SPANSION_FLASH				4
#define NUMONYX_FLASH				5
#define WINBOND_FLASH				6
#define MX_FLASH					7
#define MICRON_FLASH				8

//����;��
#define IN_UART						0
#define IN_USB						1
#define IN_UART_USB					2

//��ͨ���ط�ʽ����
#define QCMM_DL_TYPE_NDF			-1
#define QCMM_DL_TYPE_AUTO			0
#define QCMM_DL_TYPE_EMERG			1
#define QCMM_DL_TYPE_NORM			2

//��ͨ���ؿ�
#define QCMM_DL_PORT_NDF			-1
#define QCMM_DL_PORT_AUTO			0
#define QCMM_DL_PORT_DIAG			1
#define QCMM_DL_PORT_UART			2

//��ͨ��������ѡ�����ù������ʵ�ʵĲ���
#define QCMM_OPTION_DL_AUTO				0
#define QCMM_OPTION_BACKUP_ONLY			1
#define QCMM_OPTION_RESTORE_ONLY		2
#define QCMM_OPTION_UPGRADE_BACKUP		3
#define QCMM_OPTION_UPGRADE_RELOAD		4

//MTK��������ѡ�����ù������ʵ�ʵĲ���
#define MTK_OPTION_DL_FORMAT			0
#define MTK_OPTION_DL_ONLY				1
#define MTK_OPTION_DL_CLEAR_NV			2

//INTEL��������ѡ�����ù������ʵ�ʵĲ���
#define INTEL_OPTION_DL_ONLY			0
#define INTEL_OPTION_DL_ERASE			1

//INTEL���ط�ʽ
#define INTEL_DL_USB_SCAN				0
#define INTEL_DL_USB_MANUAL				1

//��ɫ
#define COL_BLACK					0
#define COL_RED						1
#define COL_BLUE					2

//3�����õ���ʾ��Ϣ�ؼ�
#define WM_MSGBOX_MSG							WM_USER + 1000
#define WM_INFO_MSG								WM_USER + 1001

#define WM_LOG_IN_MSG							WM_USER + 1010
#define WM_LOG_OUT_MSG							WM_USER + 1011
#define WM_LOG_CHANGE_MSG						WM_USER + 1012
#define WM_CHANGE_PASSWORD_MSG					WM_USER + 1013

#define WM_STATUS_MSG							WM_USER + 1020
#define WM_PROGRESS_MSG							WM_USER + 1021
#define WM_PROGRESS_ENTIRE_MSG					WM_USER + 1022
#define WM_COLOUR_MSG							WM_USER + 1023
#define WM_DL_INFO_MSG							WM_USER + 1024
#define WM_TRACE_LOG_MSG						WM_USER + 1025
#define WM_REDIT_INFO_MSG						WM_USER + 1026
#define WM_COMPORT_MSG							WM_USER + 1027
#define WM_OUT_STATUS_MSG						WM_USER + 1028

#define WM_BAR_IMEI_MSG							WM_USER + 1029
#define WM_STATUS_MSG_2							WM_USER + 1030
#define WM_STATUS_DOS							WM_USER + 1031
#define WM_PROGRESS_AUTO_DL_MSG                 WM_USER + 1032
 
#define WM_ZIP_DL_MSG							WM_USER + 1033
#define WM_FTP_Time_MSG							WM_USER + 1034

#define WM_PLC_OPEN_PATH_MSG						WM_USER + 1035
#define WM_PLC_CLOSE_PATH_MSG						WM_USER + 1036
#define WM_LOG_INFO_MSG						WM_USER + 1037
#define WM_PowerOn_SPC							WM_USER + 1038
#define WM_PowerOn_SPC_Only							WM_USER + 1039

#define WM_DL_PORT_MSG							WM_USER + 1101

//CWinSocketר��
#define WM_WS_TCP_ACCEPT_MSG					WM_USER + 2000//�ͻ���TCP������Ϣ
#define WM_WS_TCP_OP_MSG						WM_USER + 2001//�ͻ���TCP����д���ز�����Ϣ

//����DLL����Ϣ
//#define WM_START_TS_TEST_MSG					WM_USER + 3001//TS���Ե��������Ա�����
#define WM_START_TS_CLOSS_MSG					WM_USER + 3002//TS���Ե�����У׼������
#define WM_GET_LAN_BARCODE_MSG					WM_USER + 3003//�յ��Ӿ������������ݣ�������TS����̨��ʼ����
#define WM_GET_LAN_MO_NAME_MSG					WM_USER + 3004//�յ��Ӿ����繤�������ݣ�������TS����̨�����³�ʼ��
#define WM_CLEAR_TS_FPY_MSG						WM_USER + 3005////��ղ���̨��FPY
#define WM_GET_LAN_FIXTURE_CODE_MSG             WM_USER + 3006//�յ��ܿس������ļо߶�ά�����ݣ��Ѽо߶�ά�뷢��TS���Գ���
#define WM_GET_LAN_QUERY_MO_MSG                 WM_USER + 3007//�յ��ܿس������ġ�������,�ߺ�,OperatorID����Ϣ�������ѡ�TS���Գ��������ȡ��MO����




//�ȼ�����������Ϣ
#define WM_HOTKEY_UPDATE_GEN_MSG				WM_USER + 2010
#define WM_HOTKEY_DLOAD_MSG						WM_USER + 2011
#define WM_HOTKEY_OPTION_MSG					WM_USER + 2012
#define WM_HOTKEY_DEBUG_MSG						WM_USER + 2013
#define WM_HOTKEY_DLOAD_PORT_MSG				WM_USER + 2014
#define WM_HOTKEY_FIREHOSE_MSG					WM_USER + 2015
#define WM_HOTKEY_NB_BC95_PARA_MSG				WM_USER + 2016

#define WM_HOTKEY_OPTION_MSG_1                WM_USER + 2017
#define WM_HOTKEY_OPTION_MSG_METHOD            WM_USER + 2018

//�ȼ�ID����
#define HOTKEY_DLOAD_MSG						WM_USER + 2020
#define HOTKEY_OPTION_MSG						WM_USER + 2021
#define HOTKEY_DEBUG_MSG						WM_USER + 2022
#define HOTKEY_DLOAD_PORT_MSG					WM_USER + 2023

#define HOTKEY_FIREHOSE_MSG					WM_USER + 2024
#define HOTKEY_NB_BC95_PARA_MSG				WM_USER + 2025

#define HOTKEY_OPTION_MSG_1               WM_USER + 2026
#define HOTKEY_OPTION_MSG_METHOD          WM_USER + 2027

//����������Ϣ
#define WM_SCAN_DL_START_MSG					WM_USER + 3000

#define WM_SCAN_DL_START_FOUR_MSG             WM_USER + 3001

#define WM_SCAN_START_FOUR_MSG					WM_USER + 3002

//����ƽ̨
// #define PF_NDF					0
// #define PF_MTK					1
// #define PF_QUALCOMM				2
// #define PF_RDA					3
// #define PF_GCT					4
// #define PF_INTEL				5
// #define PF_HUAWEI				6

//�����������
#define FW_STD					0
#define FW_APP					1


//��ڹ��ߵĲ�������
#define LOAD_MANUAL				0
#define LOAD_SCAN_BCODE_DL			1 //
// #define LOAD_3_AXIS_ROBOT_V1	2    ���������(1��);
// #define LOAD_SEMI_FIXTURE		3 ���Զ��о�;
// #define LOAD_3_AXIS_ROBOT_V2	4  ���������(2��);
#define LOAD_AUTO_ROBOT			2//ȫ�Զ�������
#define LOAD_4_AUTO_DL			3 //���ذ岻���Ӿ�ϵͳ
#define LOAD_4_AUTO_DL_Vison			4 //���ذ���Ӿ�ϵͳ
#define LOAD_4_TOTAL_AUTO_DL			5
#define LOAD_BURN_IN			6 //Burn In����ģʽ
#define LOAD_BURN_IN_MANUAL				7 //ΰ���� �ֶ�ģʽBurn In����ģʽ
#define LOAD_AUTO_ROBOT_SPC			8//ȫ�Զ�������-ÿ���˿ڽ���һ���̿ص�Դ����
#define LOAD_SCAN_BCODE_DL_SPC			9 //���̿ص�Դ��ɨ������ģʽ
#define LOAD_AUTO_ROBOT_Pre_DL			10//Pre DownLoad һ���ĵ��Զ�������ģʽ

#define PreDL_Mode_Other    1  //��AG���������ͣ���MC60
#define PreDL_Mode_AG35     2  //Pre DL ��AG35
#define PreDL_Mode_AG52X    3  //Pre DL ��AG52X

//FX��ʹ��״̬
#define FX_HIDE			0//δ֪״̬
#define FX_NO_USE		1//��ʹ�ã�δ�����״̬���ر�ʹ��
#define FX_PASS			2//����pass
#define FX_FAIL			3//����fail
#define FX_STANDBY		4//����
#define FX_LOAD			5//�Ѽ���ģ�飬�о�δ����
#define FX_SCAN			6//start��ť���£���ɨ�����
#define FX_WORK			7//�Ѽ���ģ�飬�оߺ��ϣ������У����ϼо������ڲ���
#define FX_FINISH		8//���Խ������оߴ򿪣�ģ��δȡ��
#define FX_ALARM		9//����


//�����˿ڣ���������״̬
#define DL_OBJ_NDF			0//δ֪״̬
#define DL_OBJ_NO_USE		1//��ʹ�ã�δ�����״̬��δ����-δѡ��-��ʹ�õĶ˿�
#define DL_OBJ_STANDBY		2//�������Ѽ������ģ�飩
#define DL_OBJ_LOAD			3//����ģ�飨��ģ�飩
#define DL_OBJ_SCAN			4//ɨ��ģ��
#define DL_OBJ_RUN			5//ģ������FW������
#define DL_OBJ_PASS			6//����pass
#define DL_OBJ_FAIL			7//����fail


//����Э��
#define MODE_TCP					0
#define MODE_UDP					1


//�Ƿ�༭���ڱ�ϵͳ��
#define SENTRY_LOCK						1
#define SENTRY_NO						2


//MO_STD
#define MO_STD_MO							0
#define MO_STD_STATUS						1
#define MO_STD_MODEL_PRODU					2//��Ʒϵ�У�IMEI���ݿ�ķ�������
#define MO_STD_TOOL_OPTION					3//��Ʒ�ͺ�ȫ���������ͺţ�
#define MO_STD_PCB_VER						4
#define MO_STD_OC							5
#define MO_STD_PN							6
#define MO_STD_PRODU_TYPE					7//���
#define MO_STD_BOM							8
#define MO_STD_SMT_FILE						9
#define MO_STD_SW							10
#define MO_STD_PRODU_PROP					11
#define MO_STD_QUAN_ORDER					12
#define MO_STD_QUAN_SHIP					13
#define MO_STD_QUAN_SCRAP					14
#define MO_STD_SN_HEAD						15
#define MO_STD_IMEI_TAC						16
#define MO_STD_IMEI_START					17
#define MO_STD_IMEI_END						18
#define MO_STD_FACTORY						19
#define MO_STD_UPDATE_DATE					20
#define MO_STD_OP_USER						21
#define MO_STD_CF_USER						22
#define MO_STD_REMARK						23
#define MO_STD_CREATE_DATE					24
#define MO_STD_RESERVED						25//(0:������ַͷ, 1:������ַ��ʼ, 2:������ַ����, 3:MAC��ַͷ, 4:MAC��ַ��ʼ, 5:MAC��ַ����, 6:��ǩ������, 7:MEID�Ŷ�����)


//IMEI_STD_xxx
#define IMEI_STD_IMEI						0
#define IMEI_STD_SN							1
#define IMEI_STD_MO							2
#define IMEI_STD_CARTON_NUMB				3//���_���������
#define IMEI_STD_STATUS						4
#define IMEI_STD_FACTORY					5
#define IMEI_STD_CUSTOMER					6
#define IMEI_STD_UPDATE_DATE				7
#define IMEI_STD_RESERVED					8//(0:��Ӧ��������ַ, 1:���ڵ����, 2:ICCID����ʱ��ţ����ϵ�rar�ļ�·�����Ƿ�������Bodytrace_Core�ļ���, 3:IMSI, 4:��Ӧ��MAC��ַ, 5:MEID, 6:�ڶ�IMEI, 7:PIN_TEST���Խ��, 8:A/BƷ��ʶ)



//xqcn��������
DLL_IMP void QLIB_QCN_INIT_FIRST();
DLL_IMP bool QLIB_QCN_Backup_XQCN(int i_Port, const wchar_t * wcp_Path_And_File_Name);
DLL_IMP bool QLIB_QCN_Restore_XQCN(int i_Port, const wchar_t * wcp_Path_And_File_Name);

//code_reader
DLL_IMP bool CODE_READER_SCAN_CODE_ETHERNET(int i_Mode_TCP_UDP, const wchar_t * wcp_IP_Addr, int i_Port, CString * csp_Read_Code_Data);
DLL_IMP void CODE_READER_GET_ERROR_INFO(CString * csp_Error_Info);

//��Ʒ����
#define PRODU_TYPE_NDF                 0
#define PRODU_TYPE_PCBA                      1
#define PRODU_TYPE_MINIPCIE            2
#define PRODU_TYPE_TE_A                      3
#define PRODU_TYPE_TE_C                      4

//����ƽ̨
// typedef enum tagEU_PF_IDX
// {
// 	//
// 	PF_NDF = 0,//δ����
// 	//
// 	PF_MTK,
// 	PF_QUALCOMM,
// 	PF_RDA,
// 	PF_GCT,
// 	PF_INTEL,
// 	PF_HISILICON,
// 	//
// 	PF_COUNT
// }EU_PF_IDX;


#if 0
//BBоƬ
typedef enum tagEU_BB_CHIP_IDX
{
	//
	BB_NDF = 0,//δ����
	//
	BB_MDM6200,//��ͨ
	BB_QSC6270,//��ͨ
	BB_XMM6255,//Intel
	BB_XMM6250,//Intel
	BB_MT6261,//MTK
	BB_MDM9x15,//��ͨ
	BB_MSM8909,//��ͨ
	BB_MDM9x07,//��ͨ
	BB_RDA8851,//RDA
	BB_MSM8937,//��ͨ4GоƬ
	BB_MSM8953,//��ͨ4GоƬ
	BB_AR6003G,//��ͨWiFiоƬ
	BB_QCA1023,//��ͨWiFiоƬ
	BB_MDM9206,//��ͨ4GоƬ
	BB_MDM9628,//��ͨ4G����оƬ
	BB_HI2110,//��˼Hi2110 NB-IoTоƬ
	BB_MSM8917,//��ͨ4GоƬ
	BB_MDM9x40,//��ͨ4GоƬ CAT-6
	//
	BB_CHIP_COUNT
}EU_BB_CHIP_IDX;
#endif


//��Ʒ����
#define PRODU_TYPE_NDF              0
#define PRODU_TYPE_PCBA             1
#define PRODU_TYPE_MINIPCIE         2
#define PRODU_TYPE_TE_A             3
#define PRODU_TYPE_TE_C				4
#define PRODU_TYPE_TE_B				5
#define PRODU_TYPE_KIT				6
#define PRODU_TYPE_MOB				7//Module On Board
#define PRODU_TYPE_COB				8//Chip On Board

// ͨ����
#define DL_CHANNEL_NUM              20

//��Ʒ�汾
#define PRODU_VER_NDF               0
#define PRODU_VER_STD               1
#define PRODU_VER_GW                2//����
#define PRODU_VER_NW                3//����
#define PRODU_VER_ING               4//ingenico
#define PRODU_VER_ZJXD              5//�н��Ŵ�
#define PRODU_VER_ZJ01              6//�н��Ŵ�
#define PRODU_VER_HIK               7
#define PRODU_VER_KT                8
#define PRODU_VER_SKT               9
#define PRODU_VER_CM                10

#define TS_GSM					0
#define TS_WCDMA				1
#define TS_LTE					2
#define TS_2G_3G				3//2G + 3G ����
#define TS_2G_3G_4G				4//2G + 3G + 4G ����









