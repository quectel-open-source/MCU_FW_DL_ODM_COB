#pragma once


//
#ifdef DLL_IMPLEMENT

#define DLL_IMP _declspec(dllexport)//#define DLL_IMP extern "C" _declspec(dllexport)

#else

#define DLL_IMP _declspec(dllimport)//#define DLL_IMP extern "C" _declspec(dllimport)
#include <vector>

#endif



//SQL������״ֵ̬����
#define SQL_BOF_ERROR						101
#define SQL_EOF_ERROR						102
#define SQL_BOF_EOF_ERROR					103
#define SQL_FIND_TARGET						104
#define SQL_FIND_NO_TARGET					105



//varchar [ ( n | max ) ]
//�ɱ䳤�ȣ��� Unicode �ַ������ݡ� n ���ڶ����ַ������ȣ�����������Ϊ 1 �� 8,000 ֮���ֵ�� max ָʾ���洢��С�� 2^31-1 ���ֽ� (2 GB)�� �洢��СΪ���������ݵ�ʵ�ʳ��� + 2 ���ֽڡ� varchar �� ISO ͬ���Ϊ charvarying �� charactervarying��



//״̬�����б�
#define STATUS_ERROR						0
//����״̬��MO��
#define STATUS_MO_DISABLE					1//��������
#define STATUS_MO_GENERATE					2//������
#define STATUS_MO_CONFIRMED					3//��ȷ��
#define STATUS_MO_ACTIVATED					4//��Activated
#define STATUS_MO_RELEASE					5//�ѷ���
#define STATUS_MO_RUN						6//�ѿ�ʼ����������
#define STATUS_MO_FINISH_IMEI				7//��ȫ��д��IMEI��
#define STATUS_MO_FINISH_PACK				8//��ȫ����װ��
#define STATUS_MO_CLOSE						9//��ȫ�������������ر�
//IMEI״̬
#define STATUS_IMEI_NO						20//δ������δ����
#define STATUS_IMEI_GEN						21//�����ɣ���������ȡPC���ı���
#define STATUS_IMEI_WRITE					22//��д��
#define STATUS_IMEI_CHECK					23//�Ѻ˶�����IMEI
#define STATUS_IMEI_PACK					24//��װ��
#define STATUS_IMEI_SHIP					25//�ѳ���
//Carton��״̬
#define STATUS_CARTON_GEN					30//�·�������
#define STATUS_CARTON_PACK					31//�Ѱ�װ��һ��
#define STATUS_CARTON_SHIP					32//�����ѳ���
#define STATUS_CARTON_CANCEL				33//�������ȡ��
#define STATUS_CARTON_HOLD					34//�������HOLD״̬
//SI״̬
#define STATUS_SI_RELEASE					41//�ѷ���
#define STATUS_SI_RUN						42//���ڳ�����
#define STATUS_SI_CLOSE						43//�ѳ��������
//GPSģ��״̬
#define STATUS_GPS_SN_NO					50//δ����
#define STATUS_GPS_SN_GEN					51//�����ɣ���ӡ��
#define STATUS_GPS_DL						52//����FW
#define STATUS_GPS_TEST_1					53//GPS�������ࡰ1��
#define STATUS_GPS_TEST_2					54//GPS�������ࡰ2��
#define STATUS_GPS_TEST_3					55//GPS�������ࡰ3��
#define STATUS_GPS_FIX_CNO					56//��λ��CNo����(PASS)
#define STATUS_GPS_PACK						57//��װ��
#define STATUS_GPS_SHIP						58//�ѳ���


//MINI-PCIE��Ʒ
#define STATUS_MINI_NO						60//δ����
#define STATUS_MINI_GEN						61//�����ɡ��Ѱ��¡���������������
#define STATUS_MINI_DL_FW					62//
#define STATUS_MINI_CAL						63//
#define STATUS_MINI_FT						64//
#define STATUS_MINI_FCT						65//
#define STATUS_MINI_WRITE					66//
#define STATUS_MINI_CHECK					67//
#define STATUS_MINI_PACK					68//
#define STATUS_MINI_SHIP					69//


//������Ʒ(WM-A PCBA)
#define STATUS_WM_SN_NO						80//δ����
#define STATUS_WM_SN_GEN					81//������������ƷSN
#define STATUS_WM_SN_DL						82//DL
#define STATUS_WM_SN_FCT					83//FCT
#define STATUS_WM_SN_FT						84//FT��ϲ���
#define STATUS_WM_SN_PACK					85//��װ


//�� PCBA (IMEI) ����״̬
#define STATUS_PCBA_IMEI_NO					100//δ������δ����
#define STATUS_PCBA_IMEI_GEN				101//�����ɣ����ϴ������ݿ�
#define STATUS_PCBA_DL_FW					102//�������
#define STATUS_PCBA_CAL						103//RF-У׼
#define STATUS_PCBA_PRE_FT					104//RF-PRE-FT
#define STATUS_PCBA_FT						105//RF-FT
#define STATUS_PCBA_FCT_ONLY				106//������FCT
#define STATUS_PCBA_IMEI_WRITE				107//дIMEI��SN�Ȳ���
#define STATUS_PCBA_IMEI_CHECK				108//�˶Բ���
#define STATUS_PCBA_FCT_IMEI				109//һվʽ��FCT + д�� + Check��
#define STATUS_PCBA_1PT						110//�Զ����ߵ�ALL_PIN��λ����ʱ��־λ��PCBA_1PT��
#define STATUS_PCBA_PT						111//PIN TEST
#define STATUS_PCBA_PACK					112//�ռ���װ
#define STATUS_PCBA_SHIP					113//����


//�� MOB��Ʒ ����״̬
#define STATUS_MOB_SN_GEN					121//MOB������
#define STATUS_MOB_SN_DL					122//
#define STATUS_MOB_SN_FCT					123//
#define STATUS_MOB_SN_FT					124//
#define STATUS_MOB_SN_PACK					125//



//�Ƿ�༭���ڱ�ϵͳ��
#define SENTRY_LOCK							1
#define SENTRY_NO							2



//IMEI_Assign
#define IMEI_ASSIGN_MODEL_TAC				0
#define IMEI_ASSIGN_IMEI_TAC				1
#define IMEI_ASSIGN_IMEI_FROM				2
#define IMEI_ASSIGN_IMEI_TO					3
#define IMEI_ASSIGN_IMEI_CURR				4
#define IMEI_ASSIGN_REMAIN					5
#define IMEI_ASSIGN_UPDATE_DATE				6
#define IMEI_ASSIGN_CREATE_DATE				7
#define IMEI_ASSIGN_REMARK					8
//IMEI_Assign_Record
#define IMEI_ASI_RCD_MODEL_PRODU			0
#define IMEI_ASI_RCD_MODEL_TAC				1
#define IMEI_ASI_RCD_MO						2
#define IMEI_ASI_RCD_IMEI_TAC				3
#define IMEI_ASI_RCD_SUB					4
#define IMEI_ASI_RCD_SUB_IDX				5
#define IMEI_ASI_RCD_IMEI_START				6
#define IMEI_ASI_RCD_IMEI_END				7
#define IMEI_ASI_RCD_QUANTITY				8
#define IMEI_ASI_RCD_FACTORY				9
#define IMEI_ASI_RCD_GET_USER				10
#define IMEI_ASI_RCD_GET_DATE				11
#define IMEI_ASI_RCD_GET_PC					12
#define IMEI_ASI_RCD_REMARK					13
#define IMEI_ASI_RCD_FETCH					14

//Login_User
#define LOG_IN_USER_NAME					0
#define LOG_IN_USER_PASSWORD				1
#define LOG_IN_USER_MODE					2
#define LOG_IN_USER_REMARK					3
#define LOG_IN_USER_RESERVED				4

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
#define MO_STD_PRODU_PROP					11//MP��REP�ȹ������ԡ���������
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
#define MO_STD_RESERVED						25//(0:������ַͷ, 1:������ַ��ʼ, 2:������ַ����, 3:MAC��ַͷ, 4:MAC��ַ��ʼ, 5:MAC��ַ����, 6:��ǩ������, 7:MEID�Ŷ�����, 8:�����Ķ�������,�����Ƿ����GPS���ڶ�IMEI��PT��, 9:MINI����������PCBAģ���OC��PN)

//MO_LITE
#define MO_LITE_MO							0
#define MO_LITE_SN_CURR						1
#define MO_LITE_IMEI_CURR					2
#define MO_LITE_CARTON_CURR					3//�����ĵ�ǰ���
#define MO_LITE_CARTON_COLL					4//��ǰ�����յ���IMEI��Ŀ
#define MO_LITE_QUAN_SN						5//��ǰ��д���SN������
#define MO_LITE_QUAN_IMEI					6//��ǰ��д���IMEI������
#define MO_LITE_QUAN_PACK					7//��ǰ�Ѱ�װ���ռ����ϴ���SQL���ݿ⣩�õ�IMEI������
#define MO_LITE_QUAN_CARTON					8//��ǰ�Ѳ�����������������������������ѳ���������
#define MO_LITE_RESERVED					9//(0:�����ռ�����, 1:�����ռ�����, 2:�ض��������ѡ�Ԥ������ģ������ͳ��, 3:SN��ͷ�еĹ�����ˮ��, 4:������ַ��ǰ����ֵ, 5:MAC��ַ��ǰ����ֵ, 6:MEID��ǰ����ֵ, 7:�ռ���λPCʶ����, 8:�ռ���λ�����б�ʶ, 9:�ص����λPCʶ����)

//Sentry
#define SENTRY_OP_ENABLE					0
#define SENTRY_OP_USER						1
#define SENTRY_RESERVED						2

//IMEI_STD_xxx
#define IMEI_STD_IMEI						0
#define IMEI_STD_SN							1
#define IMEI_STD_MO							2
#define IMEI_STD_CARTON_NUMB				3//���_���������
#define IMEI_STD_STATUS						4
#define IMEI_STD_FACTORY					5
#define IMEI_STD_CUSTOMER					6
#define IMEI_STD_UPDATE_DATE				7
#define IMEI_STD_RESERVED					8//(0:��Ӧ��������ַ, 1:���ڵ����, 2:ICCID����ʱ��ţ����ϵ�rar�ļ�·�����Ƿ�������Bodytrace_Core�ļ����ͻ�����SN��, 3:IMSI, 4:��Ӧ��MAC��ַ, 5:MEID, 6:�ڶ�IMEI, 7:PIN_TEST��MTKƽ̨����GPS���ܣ���GPS���Խ��, 8:A/BƷ��ʶ, 9:���ݵ�PCBA����ʱ�Ĺ�����)

//PACK_STD
#define PACK_STD_CARTON_NUMB				0
#define PACK_STD_PACK_QUAN					1
#define PACK_STD_MO							2
#define PACK_STD_STATUS						3
#define PACK_STD_CUSTOMER					4
#define PACK_STD_UPDATE_DATE				5
#define PACK_STD_RESERVED					6//(0:�ϸ�֤QCP��װ��־����Ϣ)

//SI_STD
#define SI_STD_NAME							0
#define SI_STD_FACTORY						1
#define SI_STD_MO							2
#define SI_STD_CUSTOMER						3
#define SI_STD_EXP_INFO						4
#define SI_STD_REMARK						5
#define SI_STD_STATUS						6
#define SI_STD_UPDATE_DATE					7
#define SI_STD_RESERVED						8

//QUECTEL_TRACK��MES���� TRACK_xxx��SQL��
#define QT_IMEI								0
#define QT_SN								1
#define QT_MO								2//������
#define QT_CARTON_NUMB						3//���_���������
#define QT_STATUS							4
#define QT_TEST_HISTROY_1					5
#define QT_TEST_HISTROY_2					6
#define QT_UPDATE_DATE						7
#define QT_RESERVED							8//(0:��Ӧ��������ַ, 1:MO_SN����MO�е������, 2:���ڵ����, 3:��Ӧ��MAC��ַ)

//HISTORY_STD��������ʷ��¼��
#define HISTORY_STD_IMEI					0
#define HISTORY_STD_SN						1
#define HISTORY_STD_FAIL_REC				2//����FAIL�ļ�¼��Ϣ�� ������fail�Ĺ�λ��, fail�Ĵ�����
#define HISTORY_STD_TH_1					3//ģ����ǰ���Թ�λ��ʷ��¼����ʽ�����ݼ䶺�Ÿ�����=> [��λ, ���Խ��, fail��Ϣ, ����, ����PC��, ����]��
#define HISTORY_STD_TH_2					4
#define HISTORY_STD_RESERVED				5

//DB_IMEI_STD_2017
#define DB_IMEI_STD_2017_IMEI				0
#define DB_IMEI_STD_2017_SN					1
#define DB_IMEI_STD_2017_STATUS				2
#define DB_IMEI_STD_2017_OC_PCBA			3
#define DB_IMEI_STD_2017_PN_PCBA			4
#define DB_IMEI_STD_2017_MO_PCBA			5
#define DB_IMEI_STD_2017_OC_AP				6
#define DB_IMEI_STD_2017_PN_AP				7
#define DB_IMEI_STD_2017_MO_AP				8
#define DB_IMEI_STD_2017_SW					9
#define DB_IMEI_STD_2017_BOM_SMT			10
#define DB_IMEI_STD_2017_IMEI_2				11
#define DB_IMEI_STD_2017_BT_ADDR			12
#define DB_IMEI_STD_2017_MAC_ADDR			13
#define DB_IMEI_STD_2017_MEID				14
#define DB_IMEI_STD_2017_ICCID				15
#define DB_IMEI_STD_2017_IMSI				16
#define DB_IMEI_STD_2017_AB_FLAG			17
#define DB_IMEI_STD_2017_FACTORY			18
#define DB_IMEI_STD_2017_CARTON_NUMB		19
#define DB_IMEI_STD_2017_CUSTOMER			20
#define DB_IMEI_STD_2017_UPDATE_DATE		21
#define DB_IMEI_STD_2017_RESERVED			22//(0:����, 1:����, 2:ԭʼPCBA������, 3:����, 4:����GPS���Ժϸ��־λ, 5:����VRTC���Ժϸ��־λ, 6:ƽ���ȼ����[FD_PASS][FD_FAIL], 7:����, 8:ά��״̬, 9:�Ƿ�������Bodytrace_Core�ļ�)

//SN_Sequence_Code
#define SN_SC_DATE_KEY						0
#define SN_SC_SEQ_CODE						1
#define SN_SC_RESERVED						2

//FPY_STD
#define FPY_STD_ROBOT_ID					0
#define FPY_STD_MO_NAME						1
#define FPY_STD_FACTORY						2
#define FPY_STD_LINE						3
#define FPY_STD_UPDATE_DATE					4
#define FPY_STD_TS_DATA						5
#define FPY_STD_RESERVED					6

//Abstract_Log ժҪ��־���ݱ�
#define ABSTRACT_LOG_ID						0
#define ABSTRACT_LOG_IMEI					1
#define ABSTRACT_LOG_SN						2
#define ABSTRACT_LOG_STAT_NAME				3
#define ABSTRACT_LOG_TSNUM					4
#define ABSTRACT_LOG_TEST_RESULT			5
#define ABSTRACT_LOG_TEST_TOOL				6
#define ABSTRACT_LOG_ERR_CODE				7
#define ABSTRACT_LOG_ERR_INFO				8
#define ABSTRACT_LOG_MO						9
#define ABSTRACT_LOG_TEST_PC				10
#define ABSTRACT_LOG_PC_IP					11
#define ABSTRACT_LOG_TEST_DATE				12
#define ABSTRACT_LOG_LINE					13
#define ABSTRACT_LOG_OPERATOR				14
#define ABSTRACT_LOG_IS_SYNC				15

//REF_STD_2017 ������ݱ�ÿ������(�����Ϸ�)��IMEI���ݿ��ж��и����ݱ�����ʹ�øñ��е����ݡ���
#define REF_STD_2017_IMEI					0
#define REF_STD_2017_NAME					1
#define REF_STD_2017_REF_DATA				2
#define REF_STD_2017_FOR_OC					3
#define REF_STD_2017_INSTR_INFO				4
#define REF_STD_2017_TEST_LOSS_INFO			5
#define REF_STD_2017_COUNTER				6
#define REF_STD_2017_UPDATE_DATE			7
#define REF_STD_2017_ENABLE_FLAG			8
#define REF_STD_2017_RESERVED				9

//REF_STD_2019 ������ݱ��Ϸʵ�IMEI���ݿ����и����ݱ���������������˱���д�������ݡ���
#define REF_STD_2019_IMEI					0
#define REF_STD_2019_NAME					1
#define REF_STD_2019_REF_DATA				2
#define REF_STD_2019_FOR_OC					3
#define REF_STD_2019_INSTR_INFO				4
#define REF_STD_2019_TEST_LOSS_INFO			5
#define REF_STD_2019_COUNTER				6
#define REF_STD_2019_UPDATE_DATE			7
#define REF_STD_2019_ENABLE_FLAG			8
#define REF_STD_2019_RESERVED				9

//Factory_Info��
#define FACTORY_INFO_NAME					0//ȡֵ��HF/XT/WCL/JSD/KD/TT
#define FACTORY_INFO_FTP_DCC_DIR			1//����/quectel/DCC/quectel-hf

//TestFailureTimes��
#define TESTFAILURETIMES_ID							0
#define TESTFAILURETIMES_MO							1
#define TESTFAILURETIMES_REPAIREDTIMES				2
#define TESTFAILURETIMES_LASERMARK_NGTIMES			3
#define TESTFAILURETIMES_PREPRINTLABEL_NGTIMES		4
#define TESTFAILURETIMES_PREPROCESS_NGTIMES			5
#define TESTFAILURETIMES_DL_FW_NGTIMES				6
#define TESTFAILURETIMES_RF_CAL_NGTIMES				7
#define TESTFAILURETIMES_RF_FT_NGTIMES				8
#define TESTFAILURETIMES_FCT_NGTIMES				9
#define TESTFAILURETIMES_WRITE_NGTIMES				10
#define TESTFAILURETIMES_CHECK_NGTIMES				11
#define TESTFAILURETIMES_ALLPIN_NGTIMES				12
#define TESTFAILURETIMES_GPS_NGTIMES				13
#define TESTFAILURETIMES_VRTC_NGTIMES				14
#define TESTFAILURETIMES_APPEARANCE_NGTIMES			15
#define TESTFAILURETIMES_COLLECT_NGTIMES			16
#define TESTFAILURETIMES_LATESTERRORCODE			17
#define TESTFAILURETIMES_LATESTNGDATETIME			18
#define TESTFAILURETIMES_MMW_CAL_NGTIMES			19
#define TESTFAILURETIMES_MMW_IF_FT_NGTIMES			20
#define TESTFAILURETIMES_MMW_RF_FT_NGTIMES			21

//Instrument_Info��
#define INSTRUMENT_INFO_SERIAL_NUMBER				0
#define INSTRUMENT_INFO_INSTRUMENT_TYPE				1
#define INSTRUMENT_INFO_CAL_DATE					2
#define INSTRUMENT_INFO_OWNER						3
#define INSTRUMENT_INFO_FACTORY_NAME				4
#define INSTRUMENT_INFO_ENABLE						5
#define INSTRUMENT_INFO_RESERVED					6

//KIT_STD_2018��  
#define KIT_STD_2018_SN						0
#define KIT_STD_2018_STATUS					1
#define KIT_STD_2018_MO						2
#define KIT_STD_2018_OC						3
#define KIT_STD_2018_PN						4
#define KIT_STD_2018_INNER_ID				5
#define KIT_STD_2018_CARTON_NUM				6
#define KIT_STD_2018_UPDATE_DATE			7
#define KIT_STD_2018_RESERVED				8

//CPK_2019 ���ݱ�
#define CPK_2019_ID							0
#define CPK_2019_CPK_NAME					1
#define CPK_2019_CPK_VALUE		            2
#define CPK_2019_SAMPLE_VALUE			    3
#define CPK_2019_MO			                4
#define CPK_2019_TEST_STATION			    5
#define CPK_2019_TS_NO			            6
#define CPK_2019_FIXTURE_CODE			    7
#define CPK_2019_LINE			            8
#define CPK_2019_OPERATOR_ID			    9
#define CPK_2019_PC_IP			           10
#define CPK_2019_PC_MAC			           11
#define CPK_2019_UPDATE_TIME			   12
#define CPK_2019_RESERVED			       13

//Fenban_Result ���ݱ�
#define FENBAN_IMEI							0
#define FENBAN_RESULT						1
#define FENBAN_UPDATE_TIME					2

//MO_Add_Att ���������
#define MO_ADD_ATT_MO							0	//����Ψһ
#define MO_ADD_ATT_TOOL_INFO					1	//���߰汾��Ϣ
#define MO_ADD_ATT_B_COUNT						2	//B��Ʒ����
#define MO_ADD_ATT_TT_PASS_COUNT				3	//TT����PASS����
#define MO_ADD_ATT_TT_FAIL_COUNT				4	//TT����FAIL����
#define MO_ADD_ATT_TE_MINI_COUNT				5	//������������
#define MO_ADD_ATT_REP_COUNT					6	//REP��������
#define MO_ADD_ATT_SILVER_LABEL_SN_CURR			7	//ת�Ӱ���ɫ��ǩSN����ֵ
#define MO_ADD_ATT_SILVER_LABEL_SN_SEQ_CODE		8	//���湤����˳���룬����ת�Ӱ���ɫ��ǩSN���
#define MO_ADD_ATT_CUSTOMTESTFLOW				9	//�Զ����������, Json��ʽ
#define MO_ADD_ATT_NETWORK_CARTON_LABEL_VER		10	//���������籾�汾�ţ�����V1.0

//MOB_STD_2019 �����Ʒ����MOB�����ݿ��
#define MOB_STD_2019_SN_MOB					0	//���� Ψһ(���пͻ�SNд��ͻ�SN�����޿ͻ�SN��д��quectel SN)
#define MOB_STD_2019_IMEI					1
#define MOB_STD_2019_STATUS					2
#define MOB_STD_2019_OC						3
#define MOB_STD_2019_PN						4
#define MOB_STD_2019_MO						5
#define MOB_STD_2019_MO_ORIGINAL			6
#define MOB_STD_2019_SW_DL					7
#define MOB_STD_2019_AB_FLAG				8
#define MOB_STD_2019_ICCID					9
#define MOB_STD_2019_IMSI					10
#define MOB_STD_2019_CARTON_NUMB			11
#define MOB_STD_2019_RMD_REPRINT_NUM		12
#define MOB_STD_2019_UPDATE_DATE			13
#define MOB_STD_2019_RESERVED				14
#define MOB_STD_2019_MOB_BARCODE			24	//MOB���ӵı�ǩ�ϵĶ�ά����Ϣ��Ψһ������

//�׼� FirstProductRecord ��
#define FIRST_PRODU_RECORD_IMEI				0
#define FIRST_PRODU_RECORD_MO				1
#define FIRST_PRODU_RECORD_CREATE_TIME		2

//EVB_STD_2018 
#define EVB_STD_2018_SN						0
#define EVB_STD_2018_MO						1
#define EVB_STD_2018_OC						2
#define EVB_STD_2018_PN						3
#define EVB_STD_2018_Module_IMEI			4
#define EVB_STD_2018_Module_Ver				5
#define EVB_STD_2018_Status					6
#define EVB_STD_2018_AB_FLAG				7
#define EVB_STD_2018_Update_Date			8
#define EVB_STD_2018_Carton_Num				9
#define EVB_STD_2018_Rework_MO				10
#define EVB_STD_2018_RESERVED				11

//ƴ�塢����� PANEL_CODE_INFO ��
#define PANEL_INFO_PANEL_CODE				0	//���� Ψһ
#define PANEL_INFO_MO						1	//���� ����
#define PANEL_INFO_MODULE_COUNT				2	//ƴ������
#define PANEL_INFO_MODULE_1					3	//��1λ��ģ��
#define PANEL_INFO_MODULE_2					4
#define PANEL_INFO_MODULE_3					5
#define PANEL_INFO_MODULE_4					6
#define PANEL_INFO_MODULE_5					7
#define PANEL_INFO_MODULE_6					8
#define PANEL_INFO_MODULE_7					9
#define PANEL_INFO_MODULE_8					10
#define PANEL_INFO_MODULE_9					11
#define PANEL_INFO_MODULE_10				12
#define PANEL_INFO_MODULE_11				13
#define PANEL_INFO_MODULE_12				14
#define PANEL_INFO_MODULE_13				15
#define PANEL_INFO_MODULE_14				16
#define PANEL_INFO_MODULE_15				17
#define PANEL_INFO_MODULE_16				18
#define PANEL_INFO_MODULE_17				19
#define PANEL_INFO_MODULE_18				20
#define PANEL_INFO_MODULE_19				21
#define PANEL_INFO_MODULE_20				22
#define PANEL_INFO_MODULE_21				23
#define PANEL_INFO_MODULE_22				24
#define PANEL_INFO_MODULE_23				25
#define PANEL_INFO_MODULE_24				26
#define PANEL_INFO_MODULE_25				27
#define PANEL_INFO_MODULE_26				28
#define PANEL_INFO_MODULE_27				29
#define PANEL_INFO_MODULE_28				30
#define PANEL_INFO_MODULE_29				31
#define PANEL_INFO_MODULE_30				32
#define PANEL_INFO_MODULE_31				33
#define PANEL_INFO_MODULE_32				34
#define PANEL_INFO_UPDATE_TIME				35	//����ʱ��

//QDE007_PCBA �ͻ��������ݱ�
#define QDE007_PCBA_ID						0	//��ҵ���ֶΣ���ˮ�ţ�����
#define QDE007_PCBA_IMEI					1	//ģ��IMEI��Ψһ����Ӧ���������13_IMEI
#define QDE007_PCBA_SN						2	//����SN��Ψһ

//QDE007_ASM �ͻ��������ݱ�
#define QDE007_ASM_ID						0	//��ҵ���ֶΣ���ˮ�ţ�����
#define QDE007_ASM_SN						1	//����SN��Ψһ
#define QDE007_ASM_IMEI						2	//ģ��IMEI��Ψһ����Ӧ���������13_IMEI

//TestToolVersion ���߰汾��¼��
#define TESTTOOLVERSION_TOOL_NAME               0
#define TESTTOOLVERSION_LATEST_VERSION          1
#define TESTTOOLVERSION_STATUS                  2
#define TESTTOOLVERSION_HISTORY_VERSION         3
#define TESTTOOLVERSION_DATE                    4
#define TESTTOOLVERSION_SCOPE_APPLICATION       5
#define TESTTOOLVERSION_SKIP_FILE               6
#define TESTTOOLVERSION_DELETE_FILE             7
#define TESTTOOLVERSION_ISREQUIREUPDATE         8
#define TESTTOOLVERSION_UPDATECONTENT           9


//SQL��Ŀ+���ݽṹ��
typedef struct tagSQL_BASE_S
{
	CString cs_ITEM;
	CString cs_DATA;
}SQL_BASE_S;


//SQL������¼�ṹ��
typedef struct tagSQL_RECORD_SET_S
{
	int i_Index;//�ӡ�1����ʼ����
	int i_Item_Quantity;
	SQL_BASE_S strr_Base_Data[45];
	CString cs_KEY;//ÿ����¼�ġ�Ψһ������ڶԼ�¼�ġ���λ��������
}SQL_RECORD_SET_S;





//��������ӿ�
//Update( ��� �� �༭ )
//Upload( ����ӡ�׷�� �� �༭ )
//��������
DLL_IMP void SQL_DB_INIT_FIRST();
DLL_IMP void SQL_DB_FREE_ALL();
DLL_IMP void SQL_DB_LOAD_ARG( int i_SQL_Numb, QTI_PROG_ARG * strp_Qti_Arg );
DLL_IMP void SQL_DB_LOAD_SERVER_PARAMS( int i_SQL_Numb, const wchar_t * wcp_Server_Name, const wchar_t * wcp_DB_Name, const wchar_t * wcp_User_ID, const wchar_t * wcp_User_Password );
DLL_IMP int SQL_DB_GET_STATUS_IDX( const wchar_t * wcp_Status_Name );
DLL_IMP bool SQL_DB_GET_STATUS_NAME( int i_Status_Index, CString * csp_Status_Name );
DLL_IMP void SQL_DB_GET_SQL_ERR_INFO( int i_SQL_Numb, CString * csp_Error_Info );
DLL_IMP bool SQL_DB_LOAD_SQL_ITEM( const wchar_t * wcp_SQL_Recordset_Name, SQL_RECORD_SET_S * strp_SQL_ITEM_DATA );
//CONN ���
DLL_IMP bool SQL_DB_CONN_SERVER( int i_SQL_Numb, int i_Connect_Ver );
DLL_IMP void SQL_DB_DISCON_SERVER( int i_SQL_Numb, int i_Connect_Ver );
//PIN_TEST ���
DLL_IMP BOOL SQL_DB_UPDATE_PIN_TEST( int i_IP_Type, char * cp_IMEI, BOOL b_Result );
//MO_STD ���
DLL_IMP bool SQL_DB_FIND_MO_STD_CONN_V1( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_MO_Name, SQL_RECORD_SET_S * strp_SQL_MO_STD );
DLL_IMP bool SQL_DB_FIND_MO_STD_CONN_V2( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_MO_Name, SQL_RECORD_SET_S * strp_SQL_MO_STD, int * ip_SQL_Time_Data );
//IMEI_STD ���
DLL_IMP int SQL_DB_FIND_IMEI_STD_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_Produ_Name, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_IMEI_STD );
DLL_IMP int SQL_DB_FIND_IMEI_STD_V1( int i_SQL_Numb, const wchar_t * wcp_Produ_Name, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_IMEI_STD );
DLL_IMP bool SQL_DB_UPDATE_IMEI_STD_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_Produ_Name, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_IMEI_STD );
DLL_IMP bool SQL_DB_UPDATE_IMEI_STD_V1( int i_SQL_Numb, const wchar_t * wcp_Produ_Name, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_IMEI_STD );
DLL_IMP bool SQL_DB_UPLOAD_IMEI_STD_STATUS_V1( int i_SQL_Numb, const wchar_t * wcp_Produ_Name, const wchar_t * wcp_IMEI, int i_New_Status );
//HISTORY_STD ���
DLL_IMP bool SQL_DB_UPLOAD_HISTORY_STD_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_IMEI, bool b_Result, const wchar_t * wcp_Test_Station, const wchar_t * wcp_Err_Info, const wchar_t * wcp_PC_Name, const wchar_t * wcp_MO );
DLL_IMP bool SQL_DB_UPLOAD_HISTORY_STD_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, bool b_Result, const wchar_t * wcp_Test_Station, const wchar_t * wcp_Err_Info, const wchar_t * wcp_PC_Name, const wchar_t * wcp_MO );
DLL_IMP bool SQL_DB_UPDATE_HISTORY_STD_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_IMEI, const wchar_t * wcp_SN, bool b_Result, const wchar_t * wcp_Test_Station, const wchar_t * wcp_Err_Info, const wchar_t * wcp_PC_Name, const wchar_t * wcp_MO );
DLL_IMP bool SQL_DB_UPDATE_HISTORY_STD_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, const wchar_t * wcp_SN, bool b_Result, const wchar_t * wcp_Test_Station, const wchar_t * wcp_Err_Info, const wchar_t * wcp_PC_Name, const wchar_t * wcp_MO );
DLL_IMP bool SQL_DB_UPLOAD_HISTORY_STD_TH_STR_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, const wchar_t * wcp_TH_STR );
DLL_IMP bool SQL_DB_UPDATE_HISTORY_STD_TH_STR_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, const wchar_t * wcp_SN, const wchar_t * wcp_TH_STR );
//DB_IMEI_STD_2017 ���
DLL_IMP int SQL_DB_FIND_DB_IMEI_STD_2017_CONN_V1( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_IMEI_2017 );
DLL_IMP int SQL_DB_FIND_DB_IMEI_STD_2017_CONN_V2( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_IMEI_2017, int * ip_SQL_Time_Data );
DLL_IMP int SQL_DB_FIND_DB_IMEI_STD_2017_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_IMEI_2017 );
DLL_IMP bool SQL_DB_UPDATE_DB_IMEI_STD_2017_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_IMEI_2017 );
DLL_IMP bool SQL_DB_UPDATE_DB_IMEI_STD_2017_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_IMEI_2017 );
DLL_IMP bool SQL_DB_UPLOAD_DB_IMEI_STD_2017_STATUS_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, int i_New_Status );
DLL_IMP bool SQL_DB_UPLOAD_DB_IMEI_STD_2017_STATUS_V2( int i_SQL_Numb, const wchar_t * wcp_IMEI, const wchar_t * wcp_New_Status );
DLL_IMP bool SQL_DB_UPLOAD_DB_IMEI_STD_2017_SPE_ITEM_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, const wchar_t * wcp_Specified_Item, const wchar_t * wcp_New_Data );
DLL_IMP bool SQL_DB_UPLOAD_DB_IMEI_STD_2017_STATUS_SPE_ITEM_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, int i_New_Status, const wchar_t * wcp_Specified_Item, const wchar_t * wcp_New_Data );
DLL_IMP bool SQL_DB_UPLOAD_DB_IMEI_STD_2017_STATUS_SPE_ITEM_V2( int i_SQL_Numb, const wchar_t * wcp_IMEI, const wchar_t * wcp_New_Status, const wchar_t * wcp_Specified_Item, const wchar_t * wcp_New_Data );
//�ж�IMEI���ڵġ��¾�IMEI��¼�����ĸ�
DLL_IMP bool SQL_DB_JUDGE_DB_IMEI_REC_V1( int i_SQL_Numb, const wchar_t * wcp_Produ_Name, const wchar_t * wcp_IMEI, int * ip_Use_DB_IMEI_Sequence );
//��ȡSQL������ʱ��
DLL_IMP bool SQL_DB_GET_DB_TIME_STR_CONN( int i_SQL_Numb, int i_Connect_Ver, CString * csp_SQL_Time );
DLL_IMP bool SQL_DB_GET_DB_TIME_STR( int i_SQL_Numb, CString * csp_SQL_Time );
DLL_IMP bool SQL_DB_GET_DB_TIME_NUMB_CONN( int i_SQL_Numb, int i_Connect_Ver, int * ip_SQL_Time_Data );
DLL_IMP bool SQL_DB_GET_DB_TIME_NUMB( int i_SQL_Numb, int * ip_SQL_Time_Data );
//FPY_STD ���
DLL_IMP bool SQL_DB_UPDATE_FPY_STD_CONN( int i_SQL_Numb, int i_Connect_Ver, SQL_RECORD_SET_S * strp_SQL_FPY_STD );
//REF_STD_2017 ���
DLL_IMP bool SQL_DB_UPDATE_REF_STD_2017_CONN( int i_SQL_Numb, int i_Connect_Ver, SQL_RECORD_SET_S * strp_SQL_REF_STD_2017 );
DLL_IMP bool SQL_DB_ADD_CNT_ONE_REF_STD_2017_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_Key_IMEI, int * ip_SQL_Time_Data );
DLL_IMP int SQL_DB_FIND_REF_STD_2017_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_REF_STD_2017 );
DLL_IMP int SQL_DB_FIND_REF_STD_2017_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S * strp_SQL_REF_STD_2017 );
//Abstract_Log ���
DLL_IMP bool SQL_DB_ADD_ABSTRACT_LOG_CONN( int i_SQL_Numb, int i_Connect_Ver, SQL_RECORD_SET_S * strp_SQL_Abstract_Log );
DLL_IMP bool SQL_DB_ADD_ABSTRACT_LOG_V1( int i_SQL_Numb, SQL_RECORD_SET_S * strp_SQL_Abstract_Log );
//Factory_Info ���
DLL_IMP bool SQL_DB_READ_FACTORY_INFO_F_NAME_CONN( int i_SQL_Numb, int i_Connect_Ver, CString * csp_Factory_Name );
DLL_IMP bool SQL_DB_READ_FACTORY_INFO_F_NAME_V1( int i_SQL_Numb, CString * csp_Factory_Name );
DLL_IMP bool SQL_DB_READ_FACTORY_INFO_CONN( int i_SQL_Numb, int i_Connect_Ver, SQL_RECORD_SET_S * strp_SQL_Factory_Info );
//CPK_2019 ���
DLL_IMP bool SQL_DB_ADD_CPK_2019_CONN( int i_SQL_Numb, int i_Connect_Ver, SQL_RECORD_SET_S & oj_str_SQL_CPK_2019 );
DLL_IMP bool SQL_DB_ADD_CPK_2019_V1( int i_SQL_Numb, SQL_RECORD_SET_S & oj_str_SQL_CPK_2019 );
//REF_STD_2019 ���
DLL_IMP bool SQL_DB_UPDATE_REF_STD_2019_CONN( int i_SQL_Numb, int i_Connect_Ver, SQL_RECORD_SET_S & oj_str_SQL_REF_STD_2019 );
DLL_IMP bool SQL_DB_ADD_CNT_ONE_REF_STD_2019_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_Key_IMEI, int * ip_SQL_Time_Data );
DLL_IMP int SQL_DB_FIND_REF_STD_2019_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S & oj_str_SQL_REF_STD_2019 );
DLL_IMP int SQL_DB_FIND_REF_STD_2019_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S & oj_str_SQL_REF_STD_2019 );
//Qisda_Fenban_Result ���
DLL_IMP int SQL_DB_FIND_QISDA_FENBAN_RESULT_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S & oj_str_SQL_Fenban_Result );
DLL_IMP int SQL_DB_FIND_QISDA_FENBAN_RESULT_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S & oj_str_SQL_Fenban_Result );
//ִ��SQLָ���ȡ����ֵ
DLL_IMP bool SQL_DB_READ_VALUE_IN_SQL_CMD_V1( int i_SQL_Numb, const wchar_t * wcp_Cmd, CString & oj_cs_Value );
//TestFailureTimes ���
DLL_IMP int SQL_DB_FIND_TEST_FAIL_TIMES_CONN( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_ID, SQL_RECORD_SET_S & oj_str_SQL_TEST_FAIL_TIMES );
DLL_IMP int SQL_DB_FIND_TEST_FAIL_TIMES_V1( int i_SQL_Numb, const wchar_t * wcp_ID, SQL_RECORD_SET_S & oj_str_SQL_TEST_FAIL_TIMES );
DLL_IMP bool SQL_DB_ADD_TEST_FAIL_TIMES_V1( int i_SQL_Numb, SQL_RECORD_SET_S str_SQL_TEST_FAIL_TIMES );
DLL_IMP bool SQL_DB_EDIT_TEST_FAIL_TIMES_SPE_ITEM_V1( int i_SQL_Numb, const wchar_t * wcp_ID, const wchar_t * wcp_Specified_Item, const wchar_t * wcp_New_Data );
DLL_IMP bool SQL_DB_EDIT_TEST_FAIL_TIMES_SPE_ITEM_V2( int i_SQL_Numb, const wchar_t * wcp_ID, const wchar_t * wcp_NGTimes_Item, const wchar_t * wcp_New_NGTimes_Data, const wchar_t * wcp_New_Latest_ErrorCode_Data, const wchar_t * wcp_New_Latest_NGDateTime_Data );
//��ȡָ��Table����Ϣ
DLL_IMP int SQL_DB_FIND_DB_SPECIAL_TABLE( int i_SQL_Numb, const wchar_t * wcp_Table_Name, const wchar_t * wcp_Key_Item, const wchar_t * wcp_Key_Data, SQL_RECORD_SET_S * strp_SQL_Data, bool b_Lock = false );
DLL_IMP bool SQL_DB_FIND_DB_SPECIAL_TABLE_ITEM( int i_SQL_Numb, const wchar_t * wcp_Table_Name, const wchar_t * wcp_Key_Item, const wchar_t * wcp_Key_Data, const wchar_t * wcp_Special_Item, CString & oj_cs_Special_Data, bool b_Lock = false );
DLL_IMP bool SQL_DB_UPDATE_DB_SPECIAL_TABLE_ITEM( int i_SQL_Numb, const wchar_t * wcp_Table_Name, const wchar_t * wcp_Key_Item, const wchar_t * wcp_Key_Data, const wchar_t * wcp_Special_Item, const wchar_t * wcp_Special_Data );
//������
DLL_IMP void SQL_DB_BEGIN_TRANS( int i_SQL_Numb );
DLL_IMP void SQL_DB_COMMIT_TRANS( int i_SQL_Numb );
DLL_IMP void SQL_DB_ROLLBACK_TRANS( int i_SQL_Numb );
//MOB_STD_2019 ���
DLL_IMP int SQL_DB_FIND_MOB_STD_2019_CONN_V1( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_SN_MOB, SQL_RECORD_SET_S & oj_str_SQL_MOB_STD_2019 );
DLL_IMP int SQL_DB_FIND_MOB_STD_2019_V1( int i_SQL_Numb, const wchar_t * wcp_SN_MOB, SQL_RECORD_SET_S & oj_str_SQL_MOB_STD_2019 );
DLL_IMP int SQL_DB_FIND_MOB_STD_2019_V2( int i_SQL_Numb, const wchar_t * wcp_IMEI, SQL_RECORD_SET_S & oj_str_SQL_MOB_STD_2019 );
DLL_IMP int SQL_DB_FIND_MOB_STD_2019_V3( int i_SQL_Numb, const wchar_t * wcp_IMEI, std::vector<SQL_RECORD_SET_S> & oj_ve_str_SQL_MOB_STD_2019, int & oj_i_Quan_Of_Read );
DLL_IMP int SQL_DB_FIND_MOB_STD_2019_V4( int i_SQL_Numb, const wchar_t * wcp_MOB_Barcode, SQL_RECORD_SET_S & oj_str_SQL_MOB_STD_2019 );
DLL_IMP bool SQL_DB_UPDATE_MOB_STD_2019_CONN_V1( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_SN_MOB, SQL_RECORD_SET_S & oj_str_SQL_MOB_STD_2019 );
DLL_IMP bool SQL_DB_UPDATE_MOB_STD_2019_V1( int i_SQL_Numb, const wchar_t * wcp_SN_MOB, SQL_RECORD_SET_S & oj_str_SQL_MOB_STD_2019 );
DLL_IMP bool SQL_DB_UPLOAD_MOB_STD_2019_STATUS_V1( int i_SQL_Numb, const wchar_t * wcp_Key_Word_Item, const wchar_t * wcp_Key_Word_Data, int i_New_Status );
DLL_IMP bool SQL_DB_UPLOAD_MOB_STD_2019_STATUS_V2( int i_SQL_Numb, const wchar_t * wcp_Key_Word_Item, const wchar_t * wcp_Key_Word_Data, const wchar_t * wcp_New_Status );
DLL_IMP bool SQL_DB_UPLOAD_MOB_STD_2019_IMEI_V1( int i_SQL_Numb, const wchar_t * wcp_SN_MOB, const wchar_t * wcp_New_IMEI );
DLL_IMP bool SQL_DB_UPLOAD_MOB_STD_2019_IMEI_V2( int i_SQL_Numb, const wchar_t * wcp_MOB_Barcode, const wchar_t * wcp_New_IMEI );
//FirstProductRecord ���
DLL_IMP int SQL_DB_SEARCH_FIRST_PRODU_RECORD_V1( int i_SQL_Numb, const wchar_t * wcp_IMEI );
//MO_Add_Att ���
DLL_IMP int SQL_DB_FIND_TOOL_VER_BY_MO_AND_TOOL_NAME_CONN_V1( int i_SQL_Numb, int i_Connect_Ver, const CString cs_MO_Name, const CString cs_Tool_Name, CString & oj_cs_Tool_Version );
DLL_IMP int SQL_DB_FIND_TOOL_VER_BY_MO_AND_TOOL_NAME_V1( int i_SQL_Numb, const CString cs_MO_Name, const CString cs_Tool_Name, CString & oj_cs_Tool_Version );
DLL_IMP int SQL_DB_FIND_MO_ADD_ATT_CUSTOM_TEST_FLOW_CONN_V1( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_MO_Name, CString & oj_cs_Custom_Test_Flow );
DLL_IMP int SQL_DB_FIND_MO_ADD_ATT_CUSTOM_TEST_FLOW_V1( int i_SQL_Numb, const wchar_t * wcp_MO_Name, CString & oj_cs_Custom_Test_Flow );
//PANEL_CODE_INFO ���
DLL_IMP int SQL_DB_FIND_PANEL_CODE_INFO_CONN_V1( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_Panel_Code, SQL_RECORD_SET_S & oj_str_SQL_PANEL_CODE_INFO );
DLL_IMP int SQL_DB_FIND_PANEL_CODE_INFO_V1( int i_SQL_Numb, const wchar_t * wcp_Panel_Code, SQL_RECORD_SET_S & oj_str_SQL_PANEL_CODE_INFO );
DLL_IMP bool SQL_DB_UPDATE_PANEL_CODE_INFO_CONN_V1( int i_SQL_Numb, int i_Connect_Ver, SQL_RECORD_SET_S & oj_str_SQL_PANEL_CODE_INFO );
DLL_IMP bool SQL_DB_UPDATE_PANEL_CODE_INFO_V1( int i_SQL_Numb, SQL_RECORD_SET_S & oj_str_SQL_PANEL_CODE_INFO );
//QDE007_PCBA ���
DLL_IMP int SQL_DB_FIND_QDE007_PCBA_V1( int i_SQL_Numb, const wchar_t * wcp_Key_IMEI, SQL_RECORD_SET_S & oj_str_SQL_QDE007_PCBA );
DLL_IMP bool SQL_DB_EDIT_QDE007_PCBA_SPE_ITEM_DATA_V1( int i_SQL_Numb, const wchar_t * wcp_Key_IMEI, const wchar_t * wcp_Specified_Item, const wchar_t * wcp_New_Data );
DLL_IMP bool SQL_DB_EDIT_QDE007_PCBA_SPE_ITEM_DATA_V2( int i_SQL_Numb, const wchar_t * wcp_Key_IMEI, int i_Update_Item_Quan, CStringArray & oj_csa_Specified_Items, CStringArray & oj_csa_New_Datas );
//QDE007_ASM ���
DLL_IMP int SQL_DB_FIND_QDE007_ASM_V1( int i_SQL_Numb, const wchar_t * wcp_Key_SN, SQL_RECORD_SET_S & oj_str_SQL_QDE007_ASM );
DLL_IMP bool SQL_DB_EDIT_QDE007_ASM_SPE_ITEM_DATA_V1( int i_SQL_Numb, const wchar_t * wcp_Key_SN, const wchar_t * wcp_Specified_Item, const wchar_t * wcp_New_Data );
DLL_IMP bool SQL_DB_EDIT_QDE007_ASM_SPE_ITEM_DATA_V2( int i_SQL_Numb, const wchar_t * wcp_Key_SN, int i_Update_Item_Quan, CStringArray & oj_csa_Specified_Items, CStringArray & oj_csa_New_Datas );
//TestToolVersion ���
DLL_IMP int SQL_DB_FIND_TESTTOOLVERSION_LATEST_VER_CONN_V1( int i_SQL_Numb, int i_Connect_Ver, const wchar_t * wcp_Tool_Name, CString & oj_cs_Latest_Version );
DLL_IMP int SQL_DB_FIND_TESTTOOLVERSION_LATEST_VER_V1( int i_SQL_Numb, const wchar_t * wcp_Tool_Name, CString & oj_cs_Latest_Version );
