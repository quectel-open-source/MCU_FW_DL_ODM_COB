#include "stdafx.h"
#include "GFC.h"

#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��


CCriticalSection g_del_exe_Ctrl;

/*
Ver2.0.67 (2022-4-24)

2014-2-26��xjli
1.��Ӷ�PC���ĺ�����Get_Local_PC_Name����

2014-3-5��xjli
1.�����ˡ�WCSTOK_STRING_S������WCSTOK_DOUBLE������WCSTOK_INT����������ʱ���ƻ���Դ���ݡ�Ӱ�췴����ȡ���ݵ����⡣

2014-3-9��xjli
1.�����ˡ�WCSTOK_STRING_S������WCSTOK_DOUBLE������WCSTOK_INT�������ļ����Ϊ�� ,;\r\n����

2014-3-18��xjli
1.����ˡ�WCSTOK_STRING_C�������������Զ��塰���������

2014-3-28��xjli
1.����ˡ�GET_AT_RSP_STRING���͡�GET_AT_RSP_DIGIT_DATA��������

2014-5-27��xjli
1.����ˡ�Get_Reg_Usb_Dev_List������Get_File_Directory������Kill_Process��������

2014-6-19��xjli
1.����ˡ�Calcu_ARFCN_GSM������Calcu_Center_Freq_GSM������Calcu_Center_Freq_WCDMA��������

2014-6-23��xjli
1.����ˡ�GET_AT_RSP_MID_STRING��������

2014-6-24��xjli
1.�����ˡ�WCSTOK_STRING_C���͡�WCSTOK_STRING_S���������ڴ�û���ͷŵ����⡣

2014-8-8��xjli
1.����ˡ�Empty_Directory��������

2014-12-4��xjli
1.����ˡ�Get_Reg_Usb_Dev_Friendly_Name��������

2015-2-9��xjli
1.����ˡ�HEX_WCHAR_TO_LONG_INT��������

2015-4-29��xjli
1.����ˡ�WCSTOK_STRING_T��������

2015-7-11��xjli
1.����ˡ�Calcu_Center_Freq_LTE������Calcu_ARFCN_LTE������Get_LTE_Band_Index��������

2015-7-15��xjli
1.����ˡ�Get_LTE_Band_Name������Trans_UL_ARFCN_LTE������Trans_UL_ARFCN_WCDMA������Calcu_Center_Freq_TD������Calcu_ARFCN_TD������Get_TD_Band_Name������Get_TD_Band_Index������Encode_RF_Ref_Tx_Data_To_Str������Decode_RF_Ref_Tx_Data_From_Str��������

2015-9-14��xjli
1.�ѡ�RF_TX_UL_INPUT��RF_RX_DL_OUTPUT���塱�͡���Ƶ�ε�index���塱�ķŵ���GFC.h���ļ��С�

2015-9-18��xjli
1.����ˡ�WCSTOK_STRING_T_C()��������

2015-10-9��xjli
1.����ˡ�Calcu_IMEI_Checksum()��������

2015-11-8��xjli
1.������LTEƵ�κš�Ƶ�ʡ�Ƶ��index�ŵļ��㺯����
2.�������µ�cdma2000��ö�١�EU_C2K_BAND_IDX����

2015-12-8��xjli
1.�����ˡ�TD_BAND_39�����塣
2.����ˡ�Get_WCDMA_Band_Name()���͡�Get_WCDMA_Band_Index()��������

2015-12-30��xjli
1.����ˡ�Get_RF_Offset_Freq_Band_Seg()��������

2016-1-6��xjli
1.����ˡ�GET_ARFCN_L_M_H_TYPE()��������

2016-1-12��xjli
1.����ˡ�Adjust_From_Small_To_Large_Array_Int()��������

2016-2-2��xjli
1.�༭�ˡ�Calcu_Center_Freq_TD()��������������39Ƶ�ε�֧�֡�

2016-2-22��xjli
1.����ˡ�Rand_One_Number()��������

2016-3-11��xjli
1.����ˡ�BAND_LIST_S���ṹ��Ķ��壬�����ˡ�Ƶ��������������

2016-4-6��xjli
1.����ˡ�QTI_PROG_ARG���ṹ��Ķ��壬ȥ���ˡ�debug log��������

2016-4-6��xjli
1.����ˡ�Calcu_MEID_Check_Digit()��������

2016-4-6��xjli
1.����ˡ�TS_CONFIG_S���ṹ��Ķ��壬�����ˡ�debug log��������

2016-4-12��xjli
1.����ˡ�TS_CONFIG_S���ṹ��Ķ��壬�����ˡ�i_Com_AT_Port��������

2016-4-15��xjli
1.����ˡ�WCSTOK_STRING_T_Q()��������

2016-5-19��xjli
1.�޸��ˡ�Trans_UL_ARFCN_WCDMA()�������͡�Calcu_Center_Freq_WCDMA()�������Ĳ���������ȷ�˲�����index����name��

2016-6-7��xjli
1.����ˡ�Adjust_From_Small_To_Large_Array_Double()��������

2016-7-28��xjli
1.����˶�ȡPCӲ�̺ŵġ�ReadPhysicalDriveInNTUsingSmart()��������

2016-8-5��xjli
1.����ˡ�HEX_WCHAR_TO_SIGNED_INT32()���͡�HEX_WCHAR_TO_UNSIGNED_INT32()��������

2016-10-18��xjli
1.����ˡ�Encode_RF_Ref_Tx_Data_To_Str_V2()�����޸��˺�������Encode_RF_Ref_Tx_Data_To_Str_V1()����
2.����ˡ�Decode_RF_Ref_Tx_Data_From_Str_V2()�����޸��˺�������Decode_RF_Ref_Tx_Data_From_Str_V1()����

2016-10-31��xjli
1.����ˡ�LTE B13��B18��B19��B25��B26��B28 Ƶ�Ρ��Ķ��壬�Լ�����ŵ���Ƶ������֧�֡�

2017-2-17��xjli
1.��callback�ص������Ķ���ת�Ƶ�GFC.h�ļ��С�
2.����ˡ�CTU_PROG_ARG��callback�ص������ṹ�嶨�塣

2017-3-1��xjli
1.����ˡ�Get_Data_In_Quotation_Marks()�����������ڴ����������š�����ȡ���ݡ�
2.�����ˡ�WCDMA�� B6 �� B19 Ƶ�Ρ��Ķ����������ݡ��㷨��֧�֡�
3.�޸��ˡ�Get_WCDMA_Band_Name()�������͡�Get_WCDMA_Band_Index()��������ת�����롣

2017-4-5��xjli
1.����ˡ��ⲿ����Ŀ��� callback �ṹ�塱�Ķ��塣

2017-7-3��xjli
1.����ˡ�LTE B34 Ƶ�Ρ��Ķ��壬�Լ�����ŵ���Ƶ������֧�֡�

2017-7-7��xjli
1.����ˡ�Get_BB_Name()������Get_PF_Name()������Get_Produ_Type_Name()������Get_Produ_Ver_Name()��������

2017-7-27��xjli
1.�����ˡ�CTU_PROG_ARG���Ľṹ�嶨�壬����ˡ�CALLBACK_UPDATE_TS_PFQUAN()��������

2017-8-6��xjli
1.�����ˡ�CTU_PROG_ARG���Ľṹ�嶨�壬����ˡ�CALLBACK_UPDATE_TS_TRD()��������

2017-8-10��xjli
1.����ˡ�HEX_WCHAR_TO_LONG_INT()��������

2017-10-11��xjli
1.����ˡ�HEX_WCHAR_TO_SIGNED_INT32()�������Ĵ��루�㷨��������˸���HEX�ִ�ת�����з���int����������⡣

2017-11-3��xjli
1.����ˡ���ͨ SDM450��оƬ�Ķ��塣

2017-11-22��xjli
1.����ˡ���ͨ BB_WCN3610��BB_WCN3660��BB_WCN3680��оƬ�Ķ��塣
2.�����WiFi�ġ�WIFI_FREQ_B24G��WIFI_FREQ_B5G��Ƶ�εĶ��塣
3.����˼���WiFi����Ƶ��ĺ�����Calcu_Center_Freq_WiFi()����

2017-11-30��xjli
1.����ˡ�Get_WiFi_FreqBand_Name()���͡�Get_WiFi_Standard_Name()��������

2017-12-1��xjli
1.�����Bluetooth�ġ������汾���кš���EU_BLU_VER_IDX���塣
2.����ˡ�Calcu_Center_Freq_Bluetooth()��������

2017-12-8��xjli
1.����ˡ�Get_Bluetooth_Version_Name()��������
2.����ˡ���ͨ MSM8905��оƬ�Ķ��塣

2018-1-5��xjli
1.����ˡ�MTK MT2625��оƬ�Ķ��塣

2018-1-23��xjli
1.����ˡ�Get_FileName_From_FilePath()��������

2018-2-7��xjli
1.�������VS2013ϵͳ��ת��������ˡ�swprintf������wcstok_s������_ui64tow_s��������ش��룬�����ˡ�C4996���󡱡�

2018-4-2��xjli
1.����ˡ�MTK MT2503��оƬ�Ķ��塣

2018-5-9��xjli
1.����ˡ�WCSTOK_STRING_S_C_Ex()��������

2018-6-6��xjli
1.����ˡ�Trans_UL_ARFCN_LTE()���������ж��ṹ��
2.����ˡ�WCDMA B3���͡�LTE B14/B29/B30/B66/B71��Ƶ�εĶ��塣

2018-6-26��xjli
1.����ˡ�PRODU_TYPE_TE_B����Ʒ���͵Ķ��塣
2.ɾ������ʾ��ص���Ϣ�е����ġ������˽������ʱ��������ʾ��

2018-7-2��xjli
1.����ˡ�WIFI_STAN_80211N_5G_20M��WiFi��׼�Ķ������ش���
2.����ˡ�Get_WiFi_Standard_FreqBand()��������

2018-7-16��xjli
1.����ˡ���ͨ SDX20��оƬ�Ķ��塣

2018-7-29��xjli
1.�ѡ�tagEU_LTE_CAT_IDX��ֵ�Ķ���ת�Ƶ��ˡ�GFC.h���ļ��ж��塣
2.����ˡ�Get_LTE_Category_Name()��������

2018-9-3��xjli
1.����ˡ�LTE B42/B43/B48��Ƶ�εĶ����Ƶ��������ݵĴ���
2.���SN���ݵı��롢���뺯���еġ�Ƶ����ĸ���롱�ӡ�a~z����չ����A~Z������Ӧ��Ƶ������������26���������
3.����ˡ�Get_GSM_Band_Name()��������

2018-9-21��xjli
1.����ˡ�RDA��2GоƬ RDA8955L���͡�ASR��4GоƬ ASR1802S��оƬ�Ķ�����������ת����
2.����ˡ�ASR������ƽ̨�Ķ�����������ת����
3.����ˡ�CAT-18����Categoryֵ�Ķ�����������ת����

2018-9-26��xjli
1.����ˡ�PRODU_TYPE_KIT����Ʒ���͵Ķ���ʹ���

2018-10-28��xjli
1.����ˡ�BB_MDM9150��BBоƬ����ʹ���
2.����ˡ�LTE_CAT_NDF��CATֵ����ʹ���

2018-11-20��xjli
1.����ˡ�BB_QCA6564��BBоƬ����ʹ���
2.����ˡ�WCDMA B9���͡�LTE B9/B11/B21��Ƶ�εĶ��塣

2018-11-22��xjli
1.����ˡ�BB_SDM660��BBоƬ����ʹ���

2018-11-28��xjli
1.����ˡ�Get_Process_Attributes_Index()���͡�Get_Process_Attributes_Name()����
2.����ˡ������������ԡ��ĺ궨�塣

2018-12-4��xjli
1.����ˡ�LTE B47��Ƶ�εĶ��塣

2018-12-18��xjli
1.����ˡ�Get_NBIoT_Band_Index()���͡�Get_NBIoT_Band_Name()��������
2.����ˡ�NB-IoTƵ�������͡�NB-IoTƵ�����кš��ĺ궨�塣

2018-12-20��xjli
1.��Ƶ�ε�ö�ٶ��������ˡ�SIZE�����������ڿ���Ƶ����صĴ洢BUFF�ߴ硣
2.�ṹ�塰tagTESTER_RF_OFFSET_S������tagBAND_LIST_S������tagBAND_ENABLE_S�����ʹ�á�SIZE����������BUFF�ߴ硣
3.�ṹ�塰tagTESTER_RF_OFFSET_S������tagBAND_LIST_S������tagBAND_ENABLE_S���������NB-IoT��ز��֡�

2018-12-21��xjli
1.����ˡ�BB_RDA8908A���͡�BB_RDA8909B��BBоƬ����ʹ���
2.�����ˡ�tagREAD_OCP_PARAMS_ARG��ocp callback �ṹ�壬����ˡ�CALLBACK_READ_PRODU_NBIOT_BAND_LIST��������

2018-12-25��xjli
1.����ˡ�BB_MT2621��BBоƬ����ʹ���

2019-4-22��xjli
1.����ˡ�BB_SA415M���͡�BB_SDX24��BBоƬ����ʹ���
2.����ˡ�LTE_CAT_16���͡�LTE_CAT_20��CATֵ����ʹ���

2019-5-10��xjli
1.����ˡ�BB_MDM9205������BB_SDX55��BBоƬ����ʹ���
2.����ˡ�LTE B46������LTE B27���͡�LTE B85��Ƶ�εĶ��塣

2019-7-3��xjli
1.����ˡ�FORMAT_NBIOT_S������FORMAT_NBIOT_NS�����塣
2.�����ˡ�Get_RF_Offset_Freq_Band_Seg()�������������NB-IoT���ֵ�Ƶ�Ρ�Ƶ���ȡ��
3.�����ˡ�Get_RF_Offset_Freq_Band_Seg()�������������ˡ�LTE B4����Ƶ�Ρ�Ƶ�����ݡ�

2019-7-19��xjli
1.����ˡ�PF_REALTEK��ƽ̨����BB_RTL8189��BBоƬ�Ķ���ʹ���
2.����ˡ�Get_LTE_Band_DD_FDD_TDD()��������ͳһ����LTEƵ�ε�FDD��TDD���͡�
3.����ˡ�Get_DD_FDD_TDD_Name()����������ȡFDD��TDD���͵��ַ����ơ�
4.��FDD��TDD�Ķ����Global_Header.h�ļ�ת�Ƶ�GFC.h�ļ��С�

2019-8-24��xjli
1.����ˡ�BB_QM215���͡�BB_QCM2150��BBоƬ�Ķ���ʹ���
2.����ˡ�EU_NR5GSUB6_BAND_INDEX������EU_ANT_TYPE_INDEX������TESTER_RF_OFFSET_V2_S������BAND_CONFIG_S������BAND_LIST_V2_S������ANT_PROP_S������ANT_LIST_S������ANT_CFG_ALL_S��struct��enum�Ķ���ʹ���
3.����ˡ�REF_DATA_BASIC_S������REF_DATA_BAND_S������REF_DATA_ALL_S��struct��enum�Ķ���ʹ���
4.����ˡ�Get_NR_5G_SUB6_Band_Index()������Get_NR_5G_SUB6_Band_Name()��������
5.����ˡ�Get_ANT_Type_Name()��������
6.����ˡ�FORMAT_NR5GSUB6_NS������FORMAT_GPS_NS������FORMAT_WIFI_NS������FORMAT_BLUETOOTH_NS���Ķ��塣

2019-8-30��xjli
1.����ˡ�LTE_CAT_19��CATֵ����ʹ���
2.����ˡ�BB_SA515M��BBоƬ����ʹ���
3.����ˡ�IS_SMART_MODULE( int i_BB_Chip )��������

2019-9-3��xjli
1.����ˡ�Empty_Struct_TESTER_RF_OFFSET_S()������Empty_Struct_TESTER_RF_OFFSET_V2_S()������Empty_Struct_ANT_CFG_ALL_S()������Empty_Struct_ANT_LIST_S()������Empty_Struct_REF_DATA_ALL_S()������Empty_Struct_REF_DATA_BAND_S()��������
2.����ˡ�Copy_Struct_ANT_CFG_ALL_S()������Copy_Struct_ANT_LIST_S()������Copy_Struct_TESTER_RF_OFFSET_V2_S()������Copy_Struct_TESTER_RF_OFFSET_V2_S_To_TESTER_RF_OFFSET_S()������Copy_Struct_TESTER_RF_OFFSET_S_To_TESTER_RF_OFFSET_V2_S()��������

2019-9-11��xjli
1.����ˡ�EU_RF_KEY_TYPE_INDEX��ASRƽ̨ RF Type ����ʹ���
2.����ˡ�Get_RF_Key_Type_Name()��������

2019-9-16��xjli
1.����ˡ�BB_WCN3990��BBоƬ����ʹ���

2019-10-8��xjli
1.����ˡ�Unicode_To_UTF8()��������

2019-10-11��xjli
1.�á��ṹ��/���캯����ʼ�����ķ�ʽ�Խṹ�塰PKT_BUFF_S������ʼ����ֵ��

2019-10-18��xjli
1.����ˡ�BB_ASR3601��BBоƬ����ʹ���

2019-11-6��xjli
1.����ˡ�PF_SPREADTRUM��ƽ̨����BB_SL8541E��BBоƬ�Ķ���ʹ���

2019-11-18��xjli
1.����ˡ�PRODU_TYPE_WM_A����Ʒ���͵Ķ���ʹ���

2019-11-20��xjli
1.�����ˡ�Get_RF_Offset_Freq_Band_Seg()������������� NR 5G SUB6 ���ֵ�Ƶ�Ρ�Ƶ���ȡ��

2019-11-25��xjli
1.����ˡ�BB_HI2152���͡�BB_QCA6391��BBоƬ�Ķ���ʹ���

2019-12-2��xjli
1.����ˡ�FORMAT_CV2X_NS���Ķ��塣
2.�ṹ�塰TESTER_RF_OFFSET_V2_S���еġ�dr_WiFi_RF_Loss����չΪ���͡��С����ŵ�����ֵ����

2019-12-20��xjli
1.����ˡ�PF_EIGENCOMM��ƽ̨����BB_EC616��BBоƬ�Ķ���ʹ���

2019-12-25��xjli
1.ȡ���ˡ�PRODU_TYPE_WM_A����Ʒ���͵Ķ���ʹ���
2.����ˡ�PRODU_TYPE_MOB���͡�PRODU_TYPE_COB����Ʒ���͵Ķ���ʹ���
3.����˴洢�����������������ġ�ANT_TYPE_BUFF_SIZE�����塣��δ���书�ܺ�ʹ�÷�ʽ��Ҫ���Ż���
4.�޸ġ�ANT_TYPE_IDX_SIZE��ֵ��26��ߵ�28��
5.����ˡ�ANT_TYPE_IDX_MAIN_2������ANT_TYPE_IDX_DIV_2������ANT_TYPE_IDX_MULTI_M3������ANT_TYPE_IDX_MULTI_M4������ANT_TYPE_IDX_WIFI_MIMO������ANT_TYPE_IDX_V2X_MAIN������ANT_TYPE_IDX_V2X_DIV���������������кŵĶ���ʹ���
6.������µ�ocp�ص�API�������塰CALLBACK_READ_PRODU_PS_CH2_VOLT_AND_CURR���͡�CALLBACK_READ_PRODU_NETWORK_FORMAT_BAND_LIST����
7.������µ�V2��ocp�ص��ṹ�嶨�塰READ_OCP_PARAMS_V2_ARG����

2020-1-8��xjli
1.����ˡ�EU_CV2X_BAND_IDX��C-V2XƵ�����кŵĶ���ʹ���
2.����ˡ�Get_CV2X_Band_Index()���͡�Get_CV2X_Band_Name()��������������C-V2XƵ���������кŵ�ת����
3.�����ˡ�TESTER_RF_OFFSET_V2_S�����壬����ˡ�double dr_CV2X_RF_Loss[CV2X_BAND_SIZE * 6];��������
4.�����ˡ�Empty_Struct_TESTER_RF_OFFSET_V2_S()���͡�Copy_Struct_TESTER_RF_OFFSET_V2_S()������������ˡ�dr_CV2X_RF_Loss���Ĵ���
5.�����ˡ�Get_RF_Offset_Freq_Band_Seg()��������Ϊ��FORMAT_CV2X_NS����ʽ��������Ƶ����ȡ��

2020-1-10��xjli
1.�����ˡ�BAND_LIST_V2_S������REF_DATA_BAND_S������BAND_ENABLE_S���ṹ�嶨�壬����ˡ�C-V2X������ر�����

2020-1-12��xjli
1.����ˡ�NB_SCSP_S3K75������NB_SCSP_S15K������NB_MSCH_BPSK������NB_MSCH_QPSK������NB_SINGLE_TONE������NB_MULTI_TONE_3������NB_MULTI_TONE_6������NB_MULTI_TONE_12���ĺ궨��ʹ���
2.����ˡ�Copy_Struct_BAND_ENABLE_S()������Assign_Struct_BAND_ENABLE_S()������Get_NB_SCSP_Name()������Get_NB_Mod_Scheme_Name()��������
3.�����ˡ�Empty_Struct_REF_DATA_BAND_S()���������������ա�strr_CV2X_REF_Data���Ĳ�����

2020-1-14��xjli
1.�����ˡ�PRODU_PARAMS_GEN_S���ṹ�嶨�壬�ѡ�str_Band_List�������Ͷ���ӡ�BAND_LIST_S�����Ϊ��BAND_LIST_V2_S����

2020-2-12��xjli
1.����ˡ�NR5GSUB6_N_12������NR5GSUB6_N_25������NR5GSUB6_N_48���Ķ���ʹ���

2020-2-17��xjli
1.����ˡ�BB_ASR1601��BBоƬ����ʹ���

2020-2-20��xjli
1.����ˡ�NR5GMMW_N_257������NR5GMMW_N_258������NR5GMMW_N_260������NR5GMMW_N_261���Ķ���ʹ����Լ�Ƶ�����䡣

2020-3-3��xjli
1.����ˡ�NR5GSUB6_N_38������NR5GSUB6_N_40���Ķ���ʹ���

2020-3-11��xjli
1.����ˡ�PF_TI��ƽ̨����BB_CC1200������BB_HI9500��BBоƬ�Ķ���ʹ���

2020-3-16��xjli
1.����ˡ�BB_ASR1802SL������BB_ASR1826��BBоƬ�Ķ���ʹ���

2020-3-27��xjli
1.����ˡ�Get_ANT_Type_Index()��������

2020-4-8��xjli
1.����ˡ�L_BAND_31������L_BAND_72������L_BAND_73���Ķ���ʹ����Լ�Ƶ�����䡣
2.LTEƵ����صĴ洢BUFF�ߴ硰L_BAND_SIZE����40����46��

2020-4-17��xjli
1.����ˡ�BB_HI2115��BBоƬ�Ķ���ʹ���
2.����ˡ�WIFI_STAN_80211AX_20M������WIFI_STAN_80211AX_40M������WIFI_STAN_80211AX_5G_20M������WIFI_STAN_80211AX_5G_40M������WIFI_STAN_80211AX_5G_80M��WiFi��׼(WiFi Standard)���кŵĶ���ʹ���

2020-4-30��xjli
1.����ˡ�BB_QCA6696��BBоƬ����ʹ���
2.�ѡ������ͺš��ĺ궨��ӡ�Global_Header.h����ת�Ƶ���GFC.h���С�
3.����ˡ�LP_5G_IQXS�������ĺ궨��ʹ���
4.����ˡ�Get_RF_Instru_Name()��������

2020-5-23��xjli
1.���¡�Calcu_Center_Freq_LTE()�������������Ƶ�Ρ�NB_BAND_10��NB_BAND_22��NB_BAND_24��NB_BAND_68��NB_BAND_70��NB_BAND_74����Ƶ����㡣

2020-6-10��xjli
1.����ˡ�ANT_CFG_ALL_V2_S������REF_DATA_ALL_V2_S���ṹ�嶨�塣
2.�����ˡ�ANT_LIST_S���ṹ�嶨�壬�ѡ�strr_ANT_Prop_List���ĳߴ�ӡ�ANT_TYPE_IDX_SIZE�����Ϊ��ANT_TYPE_BUFF_SIZE����
3.����ˡ�L_BAND_32���Ķ���ʹ����Լ�Ƶ�����䡣
4.����ˡ�BB_SDA845������BB_UDX710��BBоƬ����ʹ���
5.����ˡ�Empty_Struct_REF_DATA_ALL_V2_S()��������
6.����ˡ�Get_ANT_Type_List_Buff_Index()��������
7.����ˡ�ANT_TYPE_IDX_DSDA_MAIN������ANT_TYPE_IDX_DSDA_DIV���������������кŵĶ���ʹ���
8.����ˡ�Empty_Struct_ANT_CFG_ALL_V2_S()��������

2020-6-30��xjli
1.����ˡ�Copy_Struct_ANT_CFG_ALL_V2_S_To_ANT_CFG_ALL_S()���͡�Copy_Struct_ANT_CFG_ALL_S_To_ANT_CFG_ALL_V2_S()��������
2.����ˡ�Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S()��������
3.����ˡ�BB_QCX212��BBоƬ����ʹ���

2020-7-30��xjli
1.����ˡ�BB_SM6115������BB_QTM527��BBоƬ����ʹ���

2020-8-3��xjli
1.����ˡ�BB_EC617��BBоƬ����ʹ���

2020-8-6��xjli
1.���¡�IS_SMART_MODULE()�����������á�BB_SDA845��BB_SM6115��Ϊ����ģ�顣

2020-8-26��xjli
1.����ˡ�BB_WCN3980��BBоƬ����ʹ���

2020-9-2��xjli
1.����ˡ�PRODU_TYPE_ASM����Ʒ���͵Ķ���ʹ���

2020-9-11��xjli
1.����ˡ�NR5GSUB6_N_14���Ķ���ʹ���

2020-9-22��xjli
1.����ˡ�BB_ESP8285������BB_UIS8910DM������BB_UIS8581��BBоƬ����ʹ���
2.���¡�IS_SMART_MODULE()����������ӡ�BB_UIS8581��BBоƬ��

2020-10-16��xjli
1.����ˡ�PF_XINYI��ƽ̨����BB_XY1100������BB_HI2120������BB_MT2731��BBоƬ�Ķ���ʹ���

2020-10-17��xjli
1.����ˡ�BB_WCN3988��BBоƬ�Ķ���ʹ���

2020-11-6��xjli
1.����ˡ�PF_ALTAIR��ƽ̨����BB_ALT1250������BB_QCA6595������BB_ASR1603��BBоƬ�Ķ���ʹ���

2020-11-9��xjli
1.����ˡ�BB_QCA1064������BB_MT6880��BBоƬ�Ķ���ʹ���
2.����ˡ�RF_KEY_TYPE_IDX_CRANE_M������RF_KEY_TYPE_IDX_CRANE_G��ASR ƽ̨ RF Key Type �Ķ���ʹ���

2020-11-12��xjli
1.����ˡ�BB_MT6890������BB_WCN3615��BBоƬ�Ķ���ʹ���

2020-11-30��xjli
1.����ˡ�BB_EC616S��BBоƬ�Ķ���ʹ���

2020-12-10��xjli
1.����ˡ�BB_QCM6125��BBоƬ�Ķ���ʹ���

2021-2-10��xjli
1.����ˡ�BB_UIS8811��BBоƬ����ʹ���

2021-3-9��xjli
1.����ˡ�NB_BAND_86������NB_IDX_B_86���Ķ���ʹ���
2.����ˡ�BB_SDX12������BB_QCM2290������BB_BK7231��BBоƬ����ʹ���
3.����ˡ�PF_BEKEN��ƽ̨�Ķ���ʹ���

2021-3-11��xjli
1.����ˡ�BB_SDX62��BBоƬ����ʹ���

2021-3-17��xjli
1.����ˡ�BB_QCA2064��BBоƬ����ʹ���

2021-3-26��xjli
1.����ˡ�BB_SM4350��BBоƬ����ʹ���

2021-3-30��xjli
1.����ˡ�LTE_CAT_15��CATֵ����ʹ���

2021-4-7��xjli
1.����ˡ�BB_ASR1803��BBоƬ�Ķ���ʹ���
2.�޸�ö�١�RF_KEY_TYPE_IDX_PXA1802_RF858�������Ϊ��RF_KEY_TYPE_IDX_PXA1802S_RF858����
3.���ö�١�RF_KEY_TYPE_IDX_1803_FAICON����

2021-4-9��xjli
1.����ˡ�BB_MT2735��BBоƬ�Ķ���ʹ���

2021-4-16��xjli
1.����ˡ�CALLBACK_FIND_READ_REF_STD_2017_CONN_V1������OP_SQL_DB_V1_ARG���ص��������Ͷ���ͽṹ��Ķ���ʹ���

2021-5-19��xjli
1.����ˡ�PLAN_ANT4_MAIN1_DIV1_GPS1_WIFI1���͡�PLAN_ANT4_MAIN2_DIV2���궨�塣

2021-5-20��xjli
1.����ˡ�Get_Rx_Chain_Type_Name()��������

2021-5-26��xjli
1.����ˡ�NR5GSUB6_N_13������NR5GSUB6_N_18������NR5GSUB6_N_26������NR5GSUB6_N_29������NR5GSUB6_N_30������NR5GSUB6_N_75������NR5GSUB6_N_76������NR5GSUB6_N_91������NR5GSUB6_N_92������NR5GSUB6_N_93������NR5GSUB6_N_94���Ķ���ʹ���
2.ö�١�NR5GSUB6_N_SIZE������ֵ���Ϊ��49����

2021-6-1��xjli
1.����ˡ�ANT_TYPE_IDX_V2X_TRX0������ANT_TYPE_IDX_V2X_TRX1���������������кŵĶ���ʹ���
2.ö�١�ANT_TYPE_IDX_SIZE������ֵ���Ϊ��39����

2021-6-3��xjli
1.����ˡ�LTE_CAT_13��CATֵ����ʹ���

2021-6-21��xjli
1.����ˡ�BB_WCN3991��BBоƬ����ʹ���
2.����ˡ�WIFI_STAN_80211N_5G_40M��WiFi��׼�Ķ������ش���
3.������º�����Determine_Type_Of_Plan_Connect_ANT_And_RF_Port()����
4.������µ�ocp�ص�API�������塰CALLBACK_READ_PRODU_NETWORK_FORMAT_BAND_ANT_LIST����
5.������½ṹ�嶨�塰READ_OCP_PARAMS_V3_ARG����

2021-6-29��xjli
1.����ˡ�BB_SL8521E��BBоƬ�Ķ���ʹ���

2021-7-9��xjli
1.����ˡ�BB_QCM6490��BBоƬ����ʹ���

2021-7-14��xjli
1.����ˡ�BB_QCA9377��BBоƬ����ʹ���

2021-7-21��xjli
1.�����ˡ�Get_RF_Key_Type_Name()��������RF_KEY_TYPE_IDX_RF866 ���ִ����Ϊ��1826+RF866����

2021-8-6��xjli
1.����ˡ�PF_CYPRESS��ƽ̨�Ķ���ʹ���
2.����ˡ�BB_CYW54591��BBоƬ����ʹ���
3.����ˡ�NR5GSUB6_N_70���Ķ���ʹ���

2021-8-11��xjli
1.BBоƬ��BB_ASR1803���Ķ���ʹ���ͳһ���Ϊ��BB_ASR1803S����
2.����ˡ�BB_ASR1803E���͡�BB_ASR1803L��BBоƬ�Ķ���ʹ���

2021-8-12��xjli
1.����ˡ�BB_WCN3998��BBоƬ�Ķ���ʹ���

2021-8-24��xjli
1.����ˡ�BB_QCM4290��BBоƬ�Ķ���ʹ���

2021-9-1��xjli
1.������µ�ocp�ص�API�������塰CALLBACK_READ_PRODU_ANT_LIST����
2.����ˡ�BB_QCX315��BBоƬ�Ķ���ʹ���

2021-9-16��xjli
1.����ˡ�BB_QCS8250���͡�BB_CYW43455��BBоƬ�Ķ���ʹ���

2021-9-23��xjli
1.����ˡ�BB_UIS8310AM��BBоƬ�Ķ���ʹ���

2021-9-27��xjli
1.����ˡ�BB_SDX65��BBоƬ����ʹ���
2.�����˺�����Calcu_ARFCN_WCDMA()���͡�Calcu_Center_Freq_WCDMA()����WCDMA B4�Ŀ�ʼ�ͽ����ŵ��޸�Ϊ��1312~1513���͡�1537~1738����

2021-9-29��xjli
1.����ˡ�BB_UWE5623DU��BBоƬ�Ķ���ʹ���

2021-10-24��xjli
1.����ˡ�BB_EC618��BBоƬ�Ķ���ʹ���

2021-10-25��xjli
1.����ˡ�TS_CONFIG_MOB_S���ṹ��Ķ��塣

2021-11-5��xjli
1.�ں�����IS_SMART_MODULE()���ж�BBоƬ��BB_QCM4290���͡�BB_QCS8250���趨Ϊ����SMART��ƷоƬ��

2021-11-10��xjli
1.����ˡ�PF_SILICON_LABS��ƽ̨�Ķ���ʹ���
2.����ˡ�BB_EFR32BG21��BBоƬ����ʹ���

2021-11-26��xjli
1.����ˡ�BB_CYW43438��BBоƬ�Ķ���ʹ���

2021-12-1��xjli
1.����ˡ�ANT_TYPE_IDX_NR5GSUB6_9������ANT_TYPE_IDX_NR5GSUB6_10������ANT_TYPE_IDX_NR5GSUB6_11������ANT_TYPE_IDX_NR5GSUB6_12��ANT���ߵĶ���ʹ���
2.����ˡ�NR5GSUB6_N_257������NR5GSUB6_N_258������NR5GSUB6_N_260������NR5GSUB6_N_261���Ķ���ʹ���

2021-12-3��xjli
1.����ˡ�ANT_TYPE_IDX_WIFI_GPS��ANT���ߵĶ���ʹ���

2021-12-4��xjli
1.ANT���ߡ�ANT_TYPE_IDX_WIFI_GPS�������޸�Ϊ��ANT_TYPE_IDX_WIFI_BLU_GPS����

2021-12-6��xjli
1.����ˡ�BB_UIS8850DG��BBоƬ�Ķ���ʹ���

2021-12-16��xjli
1.����ˡ�BB_CYW8X072��BBоƬ�Ķ���ʹ���

2021-12-22��summer
1.����ASR RF Type���壬�ṹ�塰EU_RF_KEY_TYPE_INDEX����������Get_RF_Key_Type_Name����

2022-1-6��xjli
1.����ˡ�BB_WCN3950������BB_W155S1��BBоƬ�Ķ���ʹ���
2.����ˡ�PF_AMLOGIC��ƽ̨�Ķ���ʹ���

2022-1-24��xjli
1.����ˡ�BB_CYW43439��BBоƬ�Ķ���ʹ���

2022-1-28��xjli
1.����ˡ�BB_CB0201��BBоƬ�Ķ���ʹ���
2.����ˡ�NB_BAND_87������NB_BAND_88������NB_IDX_B_87������NB_IDX_B_88������L_BAND_87������L_BAND_88���Ķ���ʹ���

2022-2-7��xjli
1.�����˺�����Get_LTE_Band_DD_FDD_TDD()��������LTEƵ��B87��B88ʶ��ΪFDDƵ�Ρ�

2022-2-16��xjli
1.����ˡ�BB_ASR1606��BBоƬ�Ķ���ʹ���

2022-2-18��xjli
1.�ѡ�BB_ASR1606��BBоƬ�޸�Ϊ��BB_ASR1606C���͡�BB_ASR1606S������оƬ�������¶���ʹ���
2.���ö�١�RF_KEY_TYPE_IDX_1606����
3.����ˡ�BB_WCN6856��BBоƬ�Ķ���ʹ���

2022-2-23��xjli
1.����ˡ�BB_QCX216��BBоƬ�Ķ���ʹ���

2022-3-30��xjli
1.����ˡ�CMP180�������ĺ궨��ʹ���
2.����ˡ�BB_QCA206x��BBоƬ�Ķ���ʹ���

2022-4-24��xjli
1.����ˡ�BB_ASR1828��BBоƬ�Ķ���ʹ���
2.���ö�١�RF_KEY_TYPE_IDX_1828_RF866��RF-Type�Ķ���ʹ���
*/




//
CGFC::CGFC( void )
	: m_csError_Info( _T( "" ) )
{
}

CGFC::~CGFC( void )
{
}

void CGFC::WCSTOK_INT( const wchar_t * wcp_StrToken, int * ip_List, int * ip_Quan )
{
	//��˫�ַ�������ȡ��ȫ����int����
	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//��Դ���ݡ���buff��wcstok����ȡ����ʱ��ı�Դ�������ݣ������Ȱ�Դ���ݷ���buff������buff���ݣ����˺�����Դ��
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//���ڴ�š�Դ���ݡ���buff����
	WCHAR * wcpResult = NULL;

	//�����ڴ�
	this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

	//���롰Դ���ݡ�
	swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

	//��ȡ��һ��ֵ(0)
	wcpEach = wcstok_s( wcpSrc, _T( " ,;\r\n" ), &wcpResult );

	//ѭ����ȡ
	while( wcpEach != NULL )
	{
		//��ֵ
		*(ip_List + i) = _wtoi( wcpEach );

		//����
		i++;

		//��ȡ��һ��ֵ
		wcpEach = wcstok_s( NULL, _T( " ,;\r\n" ), &wcpResult );
	}

	//��ȡ��Ԫ������ͳ��
	*ip_Quan = i;

	//�ͷ��ڴ�
	this->BUFF_WCHAR_FREE( &wcpSrc );
}

void CGFC::WCSTOK_DOUBLE( const wchar_t * wcp_StrToken, double * dp_List, int * ip_Quan )
{
	//��˫�ַ�������ȡ��ȫ����double����
	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//��Դ���ݡ���buff��wcstok����ȡ����ʱ��ı�Դ�������ݣ������Ȱ�Դ���ݷ���buff������buff���ݣ����˺�����Դ��
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//���ڴ�š�Դ���ݡ���buff����
	WCHAR * wcpResult = NULL;

	//�����ڴ�
	this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

	//���롰Դ���ݡ�
	swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

	//��ȡ��һ��ֵ(0)
	wcpEach = wcstok_s( wcpSrc, _T( " ,;\r\n" ), &wcpResult );

	//ѭ����ȡ
	while( wcpEach != NULL )
	{
		//��ֵ
		*(dp_List + i) = _wtof( wcpEach );

		//����
		i++;

		//��ȡ��һ��ֵ
		wcpEach = wcstok_s( NULL, _T( " ,;\r\n" ), &wcpResult );
	}

	//��ȡ��Ԫ������ͳ��
	*ip_Quan = i;

	//�ͷ��ڴ�
	this->BUFF_WCHAR_FREE( &wcpSrc );
}

bool CGFC::WCSTOK_STRING_C( const wchar_t * wcp_StrToken, const wchar_t * wcp_StrDelimit, int i_Get_Index, CString * csp_Get_CString )
{
	//��˫�ַ�������ȡ��ָ��λ�á����ַ������ݣ�λ����ʼ��Ϊ��0����
	//�������Ϊ�������Զ��塱

	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//��Դ���ݡ���buff��wcstok����ȡ����ʱ��ı�Դ�������ݣ������Ȱ�Դ���ݷ���buff������buff���ݣ����˺�����Դ��
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//���ڴ�š�Դ���ݡ���buff����
	bool bFlag = false;
	WCHAR * wcpResult = NULL;

	//
	try
	{
		//�����ڴ�
		this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

		//���롰Դ���ݡ�
		swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

		//��ȡ��һ��ֵ(0)
		wcpEach = wcstok_s( wcpSrc, wcp_StrDelimit, &wcpResult );

		//ѭ����ȡ
		while( wcpEach != NULL )
		{
			if( i == i_Get_Index )
			{
				//��ֵ
				csp_Get_CString->Format( _T( "%s" ), wcpEach );

				bFlag = true;
				break;//�˳�while
			}

			//��ȡ��һ��ֵ
			wcpEach = wcstok_s( NULL, wcp_StrDelimit, &wcpResult );

			//����
			i++;
		}//while(wcpEach != NULL
	}
	catch( ... )
	{
		//�ͷ��ڴ�
		this->BUFF_WCHAR_FREE( &wcpSrc );
		//fail
		return false;
	}

	//�ͷ��ڴ�
	this->BUFF_WCHAR_FREE( &wcpSrc );

	//����
	return bFlag;
}

bool CGFC::WCSTOK_STRING_S( const wchar_t * wcp_StrToken, int i_Get_Index, CString * csp_Get_CString )
{
	//��˫�ַ�������ȡ��ָ��λ�á����ַ������ݣ�λ����ʼ��Ϊ��0����
	//�������Ϊ���ո񣬶��ţ��ֺţ�\r��\n��
	//ע�⣺����������ָ���֮�������ݣ������ָ������������򲻡���ȡ�������Ա����ǡ������ָ���֮�������ݡ�

	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//��Դ���ݡ���buff��wcstok����ȡ����ʱ��ı�Դ�������ݣ������Ȱ�Դ���ݷ���buff������buff���ݣ����˺�����Դ��
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//���ڴ�š�Դ���ݡ���buff����
	bool bFlag = false;
	WCHAR * wcpResult = NULL;

	//
	try
	{
		//�����ڴ�
		this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

		//���롰Դ���ݡ�
		swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

		//��ȡ��һ��ֵ(0)
		wcpEach = wcstok_s( wcpSrc, _T( " ,;\r\n" ), &wcpResult );

		while( wcpEach != NULL )
		{
			if( i == i_Get_Index )
			{
				//��ֵ
				csp_Get_CString->Format( _T( "%s" ), wcpEach );

				bFlag = true;
				break;//�˳�while
			}

			//��ȡ��һ��ֵ
			wcpEach = wcstok_s( NULL, _T( " ,;\r\n" ), &wcpResult );

			//����
			i++;
		}//while(wcpEach != NULL
	}
	catch( ... )
	{
		//�ͷ��ڴ�
		this->BUFF_WCHAR_FREE( &wcpSrc );
		//fail
		return false;
	}

	//�ͷ��ڴ�
	this->BUFF_WCHAR_FREE( &wcpSrc );

	//���ؽ��
	return bFlag;
}

void CGFC::BUFF_BYTE_CALLOC( unsigned char ** ucp2_Buff, unsigned long ul_Buff_Size )
{
	//����unsigned char��BYTE���ַ����ڴ�

	if( *ucp2_Buff != NULL )
	{
		return;
	}

	//�����ڴ�
	*ucp2_Buff = (unsigned char *)calloc( ul_Buff_Size, sizeof( unsigned char ) );
}

void CGFC::BUFF_BYTE_FREE( unsigned char ** ucp2_Buff )
{
	//�ͷ�unsigned char��BYTE���ַ����ڴ�

	if( *ucp2_Buff != NULL )
	{
		free( *ucp2_Buff );
		*ucp2_Buff = NULL;
	}
}

void CGFC::BUFF_CHAR_CALLOC( char ** cp2_Buff, unsigned long ul_Buff_Size )
{
	//����char�ַ����ڴ�
	if( *cp2_Buff != NULL )
	{
		return;
	}

	//�����ڴ�
	*cp2_Buff = (char *)calloc( ul_Buff_Size, sizeof( char ) );
}

void CGFC::BUFF_CHAR_FREE( char ** cp2_Buff )
{
	//�ͷ�char�ַ����ڴ�
	if( *cp2_Buff != NULL )
	{
		free( *cp2_Buff );
		*cp2_Buff = NULL;
	}
}

void CGFC::BUFF_DOUBLE_CALLOC( double ** dp2_Buff, unsigned long ul_Buff_Size )
{
	//����double�ַ����ڴ�
	if( *dp2_Buff != NULL )
	{
		return;
	}

	//�����ڴ�
	*dp2_Buff = (double *)calloc( ul_Buff_Size, sizeof( double ) );
}

void CGFC::BUFF_DOUBLE_FREE( double ** dp2_Buff )
{
	//�ͷ�double�ַ����ڴ�
	if( *dp2_Buff != NULL )
	{
		free( *dp2_Buff );
		*dp2_Buff = NULL;
	}
}

void CGFC::BUFF_DWORD_CALLOC( DWORD ** dwp2_Buff, unsigned long ul_Buff_Size )
{
	//����unsigned long��DWORD���ַ����ڴ�

	if( *dwp2_Buff != NULL )
	{
		return;
	}

	//�����ڴ�
	*dwp2_Buff = (unsigned long *)calloc( ul_Buff_Size, sizeof( unsigned long ) );
}

void CGFC::BUFF_DWORD_FREE( DWORD ** dwp2_Buff )
{
	//�ͷ�unsigned long��DWORD���ַ����ڴ�

	if( *dwp2_Buff != NULL )
	{
		free( *dwp2_Buff );
		*dwp2_Buff = NULL;
	}
}

void CGFC::BUFF_WCHAR_CALLOC( wchar_t ** wcp2_Buff, unsigned long ul_Buff_Size )
{
	//����wchar_t�ַ����ڴ�
	//����Ϊ��ַ���ݡ������ָ��ĵ�ַ������ָ�룩

	if( *wcp2_Buff != NULL )//���ݱ�����ַ��Ϣ�ҵ��京�е�ָ������
	{
		return;
	}

	//�����ڴ棨�޸�ָ�����ݣ�
	*wcp2_Buff = (wchar_t *)calloc( ul_Buff_Size, sizeof( wchar_t ) );//���ص��ǡ������ڴ��ָ�루��ַ����
}

void CGFC::BUFF_WCHAR_FREE( wchar_t ** wcp2_Buff )
{
	//�ͷ�wchar_t�ַ����ڴ�
	if( *wcp2_Buff != NULL )
	{
		free( *wcp2_Buff );
		*wcp2_Buff = NULL;
	}
}

bool CGFC::Confirm_Reg_Com_Port( const int i_Tgt_Com_Port )
{
	//��ע����ж�����������
	HKEY hKey;						// handle to key to query
	DWORD dwIndex;					// index of value to query
	wchar_t * wcpValueName = NULL;	// address of buffer for value string
	DWORD dwValueSize;				// address for size of value buffer
	DWORD dwTypeCode;				// address of buffer for type code
	wchar_t * wcpValueData = NULL;	// address of buffer for value data
	DWORD dwDataSize;				// address for size of data buffer
	CRegKey theRegKey;
	wchar_t * wcpPoint;
	int iTemp;
	bool bRtn = false;

	//
	try
	{
		//�����ڴ�
		wcpValueName = (wchar_t *)calloc( 128, sizeof( wchar_t ) );
		wcpValueData = (wchar_t *)calloc( 128, sizeof( wchar_t ) );

		//
		if( ERROR_SUCCESS == ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T( "HARDWARE\\DEVICEMAP\\SERIALCOMM" ), 0, KEY_READ, &hKey ) )
		{
			for( dwIndex = 0; dwIndex < 256; dwIndex++ )
			{
				dwValueSize = 254;
				dwDataSize = 254;
				wmemset( wcpValueName, 0, 128 );
				wmemset( wcpValueData, 0, 128 );

				//If the function succeeds, the return value is ERROR_SUCCESS
				if( ::RegEnumValue( hKey, dwIndex, wcpValueName, &dwValueSize, 0, &dwTypeCode, (BYTE *)wcpValueData, &dwDataSize ) != ERROR_SUCCESS )
				{
					break;//�������˳�ѭ��
				}

				//
				wcpPoint = wcsstr( wcpValueData, _T( "COM" ) );

				//
				if( wcpPoint )
				{
					wcpPoint += 3;
					iTemp = _wtoi( wcpPoint );
					//
					if( iTemp == i_Tgt_Com_Port )
					{
						bRtn = true;//�ҵ�Ŀ�괮�ڣ��˳�ѭ��
						break;
					}
				}
			}//for(dwIndex=0; dwIndex<256;

			//
			::RegCloseKey( hKey );
		}
		else
		{
			throw(0);
		}
	}
	catch( ... )
	{
		//�ͷ��ڴ�
		if( wcpValueName != NULL )
		{
			free( wcpValueName );
		}
		//
		if( wcpValueData != NULL )
		{
			free( wcpValueData );
		}

		//fail
		return false;
	}

	//�ͷ��ڴ�
	if( wcpValueName != NULL )
	{
		free( wcpValueName );
	}
	//
	if( wcpValueData != NULL )
	{
		free( wcpValueData );
	}

	//���ؽ��
	return bRtn;
}

bool CGFC::Get_Reg_Com_Port_List( int * ip_COM_List, int * ip_COM_Quan )
{
	//��ע����ж�����������
	HKEY hKey;						// handle to key to query
	DWORD dwIndex;					// index of value to query
	wchar_t * wcpValueName = NULL;	// address of buffer for value string
	DWORD dwValueSize;				// address for size of value buffer
	DWORD dwTypeCode;				// address of buffer for type code
	wchar_t * wcpValueData = NULL;	// address of buffer for value data
	DWORD dwDataSize;				// address for size of data buffer
	CRegKey theRegKey;
	wchar_t * wcpPoint;

	//
	try
	{
		//�����ڴ�
		this->BUFF_WCHAR_CALLOC( &wcpValueName, 128 );
		this->BUFF_WCHAR_CALLOC( &wcpValueData, 128 );

		if( ERROR_SUCCESS == ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T( "HARDWARE\\DEVICEMAP\\SERIALCOMM" ), 0, KEY_READ, &hKey ) )
		{
			for( dwIndex = 0; dwIndex < 256; dwIndex++ )
			{
				dwValueSize = 254;
				dwDataSize = 254;
				wmemset( wcpValueName, 0, 128 );
				wmemset( wcpValueData, 0, 128 );

				if( ::RegEnumValue( hKey, dwIndex, wcpValueName, &dwValueSize, 0, &dwTypeCode, (BYTE *)wcpValueData, &dwDataSize ) != ERROR_SUCCESS )
				{
					break;
				}

				//
				wcpPoint = wcsstr( wcpValueData, _T( "COM" ) );

				//
				if( wcpPoint )
				{
					wcpPoint += 3;
					*(ip_COM_List + dwIndex) = _wtoi( wcpPoint );
				}
			}

			//
			::RegCloseKey( hKey );

			//
			*ip_COM_Quan = dwIndex;//�ҵ��Ĵ��ڵ���Ŀ

			//��С��������
			this->Adjust_From_Small_To_Large_Array_Int( ip_COM_List, *ip_COM_Quan );
		}
		else
		{
			throw(0);
		}
	}
	catch( ... )
	{
		//�ͷ��ڴ�
		this->BUFF_WCHAR_FREE( &wcpValueName );
		this->BUFF_WCHAR_FREE( &wcpValueData );
		//fail
		return false;
	}

	//�ͷ��ڴ�
	this->BUFF_WCHAR_FREE( &wcpValueName );
	this->BUFF_WCHAR_FREE( &wcpValueData );
	//pass
	return true;
}

bool CGFC::PKT_BUFF_ADD_DATA( PKT_BUFF_S * strp_Pkt_Buff, const unsigned char * ucp_Input_Data, const unsigned long ul_Add_Quan )
{
	CString csData;

	//
	if( strp_Pkt_Buff->ucp_Buff == NULL )
	{
		::AfxMessageBox( _T( "ERROR, PKT_BUFF_ADD_DATA, ucp_Buff = NULL" ) );
		//fail
		return false;
	}

	//
	if( strp_Pkt_Buff->ul_Total_Size < strp_Pkt_Buff->ul_Data_Length )
	{
		csData.Format( _T( "ERROR, PKT_BUFF_ADD_DATA, Total_Size = %d, Data_Length = %d, Add_Quan = %d;" ),
			strp_Pkt_Buff->ul_Total_Size, strp_Pkt_Buff->ul_Data_Length, ul_Add_Quan );
		::AfxMessageBox( csData );

		//fail
		return false;
	}
	else
	{
		unsigned long ul_SpaceFree = strp_Pkt_Buff->ul_Total_Size - strp_Pkt_Buff->ul_Data_Length;//�ܿռ�-���ÿռ�=���ÿռ�

		//
		if( ul_SpaceFree > ul_Add_Quan )//���ÿռ��㹻��������
		{
			memcpy( (strp_Pkt_Buff->ucp_Buff + strp_Pkt_Buff->ul_Data_Length), ucp_Input_Data, ul_Add_Quan );

			strp_Pkt_Buff->ul_Data_Length += ul_Add_Quan;

			//pass
			return true;
		}
		else
		{
			csData.Format( _T( "ERROR, PKT_BUFF_ADD_DATA, Total_Size = %d, Data_Length = %d, Add_Quan = %d;" ),
				strp_Pkt_Buff->ul_Total_Size, strp_Pkt_Buff->ul_Data_Length, ul_Add_Quan );
			::AfxMessageBox( csData );

			//fail
			return false;
		}
	}

	//fail
	return false;
}

void CGFC::PKT_BUFF_CALLOC( PKT_BUFF_S * strp_Pkt_Buff, const unsigned long ul_Buff_Size )
{
	if( strp_Pkt_Buff->ucp_Buff != NULL )
	{
		::AfxMessageBox( _T( "ERROR, PKT_BUFF_CALLOC, ucp_Buff != NULL" ) );
		//
		return;
	}

	//�����ڴ�
	strp_Pkt_Buff->ucp_Buff = (unsigned char *)calloc( ul_Buff_Size, sizeof( unsigned char ) );

	//ֵ��ʼ��
	memset( strp_Pkt_Buff->ucp_Buff, 0, ul_Buff_Size );
	strp_Pkt_Buff->ul_Total_Size = ul_Buff_Size;
	strp_Pkt_Buff->ul_Data_Length = 0;
}

bool CGFC::PKT_BUFF_DEL_ALL( PKT_BUFF_S * strp_Pkt_Buff )
{
	if( strp_Pkt_Buff->ucp_Buff == NULL )
	{
		//::AfxMessageBox( _T( "ERROR, PKT_BUFF_DEL_ALL, ucp_Buff = NULL" ) );
		return false;
	}

	strp_Pkt_Buff->ul_Data_Length = 0;
	memset( strp_Pkt_Buff->ucp_Buff, 0, strp_Pkt_Buff->ul_Total_Size );
	return true;
}

void CGFC::PKT_BUFF_DEL_DATA( PKT_BUFF_S * strp_Pkt_Buff, const unsigned long ul_Del_Quan )
{
	if( NULL == strp_Pkt_Buff->ucp_Buff )
	{
		::AfxMessageBox( _T( "ERROR, PKT_BUFF_DEL_DATA, ucp_Buff = NULL" ) );
		//fail
		return;
	}

	if( strp_Pkt_Buff->ul_Data_Length < ul_Del_Quan )
	{
		CString csData;
		//
		csData.Format( _T( "ERROR, PKT_BUFF_DEL_DATA, Data_Length = %d, Del_Quan = %d;" ), strp_Pkt_Buff->ul_Data_Length, ul_Del_Quan );
		::AfxMessageBox( csData );
		//fail
		return;
	}
	else if( strp_Pkt_Buff->ul_Data_Length == ul_Del_Quan )
	{
		strp_Pkt_Buff->ul_Data_Length = 0;
		memset( strp_Pkt_Buff->ucp_Buff, 0, strp_Pkt_Buff->ul_Total_Size );
	}
	else
	{
		strp_Pkt_Buff->ul_Data_Length -= ul_Del_Quan;
		memcpy( strp_Pkt_Buff->ucp_Buff, (strp_Pkt_Buff->ucp_Buff + ul_Del_Quan), strp_Pkt_Buff->ul_Data_Length );
	}
}

unsigned long CGFC::PKT_BUFF_EXTRACT_DATA( PKT_BUFF_S * strp_Pkt_Buff, unsigned char * ucp_Get_Data, const unsigned long ul_Get_Quan )
{
	if( NULL == strp_Pkt_Buff->ucp_Buff )
	{
		::AfxMessageBox( _T( "ERROR, PKT_BUFF_EXTRACT_DATA, ucp_Buff = NULL" ) );
		//fail
		return 0;
	}

	//
	unsigned long ul_Extract_Quan = ul_Get_Quan;

	//
	if( ul_Extract_Quan > strp_Pkt_Buff->ul_Data_Length )
	{
		ul_Extract_Quan = strp_Pkt_Buff->ul_Data_Length;
	}

	//��ȡҪ��ȡ������
	memcpy( ucp_Get_Data, strp_Pkt_Buff->ucp_Buff, ul_Extract_Quan );
	//����ʣ����������
	strp_Pkt_Buff->ul_Data_Length -= ul_Extract_Quan;

	if( strp_Pkt_Buff->ul_Data_Length > 0 )
	{
		//ʣ�����ݡ�ǰ�ơ�
		memcpy( strp_Pkt_Buff->ucp_Buff, (strp_Pkt_Buff->ucp_Buff + ul_Extract_Quan), strp_Pkt_Buff->ul_Data_Length );
	}
	else if( strp_Pkt_Buff->ul_Data_Length == 0 )
	{
		//��ʣ��������
		memset( strp_Pkt_Buff->ucp_Buff, 0, strp_Pkt_Buff->ul_Total_Size );
	}
	else
	{
		CString csData;
		//
		csData.Format( _T( "ERROR, PKT_BUFF_EXTRACT_DATA, Total_Size = %d, Data_Length = %d;" ),
			strp_Pkt_Buff->ul_Total_Size, strp_Pkt_Buff->ul_Data_Length );
		::AfxMessageBox( csData );
		//fail
		return 0;
	}

	//����
	return ul_Extract_Quan;
}

void CGFC::PKT_BUFF_FREE( PKT_BUFF_S * strp_Pkt_Buff )
{
	if( strp_Pkt_Buff->ucp_Buff != NULL )
	{
		free( strp_Pkt_Buff->ucp_Buff );
	}

	strp_Pkt_Buff->ucp_Buff = NULL;
	strp_Pkt_Buff->ul_Total_Size = 0;
	strp_Pkt_Buff->ul_Data_Length = 0;
}

void CGFC::PKT_BUFF_INIT( PKT_BUFF_S * strp_Pkt_Buff )
{
	strp_Pkt_Buff->ucp_Buff = NULL;
	strp_Pkt_Buff->ul_Total_Size = 0;
	strp_Pkt_Buff->ul_Data_Length = 0;
}

DWORD CGFC::HEX_BYTE_TO_DWORD( unsigned char * ucp_Hex_Char, int i_Hex_Char_Quan )
{
	char * cpData = NULL;
	char * d;
	int i, j;
	DWORD dwRtn;

	//�����ڴ�
	j = i_Hex_Char_Quan + 3;
	cpData = (char *)calloc( j, sizeof( char ) );

	*(cpData + 0) = '0';
	*(cpData + 1) = 'X';

	for( i = 0; i < i_Hex_Char_Quan; i++ )
	{
		*(cpData + 2 + i) = (char)*(ucp_Hex_Char + i);
	}

	*(cpData + 2 + i) = '\0';

	dwRtn = strtoul( cpData, &d, 16 );

	//�ͷ��ڴ�
	free( cpData );

	//����
	return dwRtn;
}

void CGFC::MultiByte_To_WideChar( const char * cp_Char_8, wchar_t * wcp_WCHAR_16, int * ip_Len_16 )
{
	//utf8���룬char*  ---> Unicode��wchar_t*

	int nLen = strlen( cp_Char_8 ) + 1;
	int nwLen = MultiByteToWideChar( CP_ACP, 0, cp_Char_8, nLen, NULL, 0 );

	//
	MultiByteToWideChar( CP_ACP, 0, cp_Char_8, nLen, wcp_WCHAR_16, nwLen );//CP_UTF8(�������޷���ʾ)

	//
	*ip_Len_16 = nwLen - 1;//ע�⣺nwLen������ȣ��ַ������ǰ�������ֹ����\0����

	//
	*(wcp_WCHAR_16 + nwLen - 1) = '\0';
}

bool CGFC::WideChar_To_MultiByte( const wchar_t * wcp_WCHAR_16, char * cp_Char_8, int * ip_Len_8 )
{
	//Unicode��wchar_t* ---> utf8���룬char*

	int iSize = WideCharToMultiByte( CP_ACP, 0, wcp_WCHAR_16, -1, NULL, 0, NULL, NULL );

	//
	if( !WideCharToMultiByte( CP_ACP, 0, wcp_WCHAR_16, -1, cp_Char_8, iSize, NULL, NULL ) )
	{
		//fail
		return false;
	}

	//
	*ip_Len_8 = iSize;

	//pass
	return true;
}

bool CGFC::mem_uc_str( unsigned char * ucp_Buff_Src, unsigned long ul_Quan_Src, unsigned char * ucp_Buff_Tgt, unsigned long ul_Quan_Tgt, unsigned long * ulp_Tgt_Index )
{
	unsigned long i, j, ulRtn;

	//
	for( i = 0; i < ul_Quan_Src; i++ )
	{
		//����BUFF_1�и��ַ���BUFF_2�����ַ�һ��
		if( *(ucp_Buff_Src + i) == *(ucp_Buff_Tgt + 0) )
		{
			ulRtn = i;//��¼������ܵ���ʵIndexֵ

			//��ʼ��BUFF_2�����ݽ���ȫ��˶�
			for( j = 0; j < ul_Quan_Tgt; j++ )
			{
				//��鴮��BUFF��Index���Ƿ�Խ�˴���BUFF�е�����
				if( (i + j) < ul_Quan_Src )
				{
					if( *(ucp_Buff_Src + i + j) != *(ucp_Buff_Tgt + j) )
					{
						break;//�����������ݲ�ͬ�����˳���û�ҵ���������
					}
				}
				else
				{
					break;//�������ݽ����ˣ�Ҳ�˳�ѭ��
				}
			}//for(j=0; j<ul_Quan_Tgt;

			//��BUFF_2�˶Ե������һλ��������BUFF_1���ҵ�һ��BUFF_2��ƥ��
			if( j == ul_Quan_Tgt )
			{
				//�ҵ���BUFF_TGTƥ��
				*ulp_Tgt_Index = ulRtn;
				//��ƥ��
				return true;
			}
		}//if(*(ucp_Buff_Src + i) == *(ucp_Buff_Tgt + 0), ����BUFF_SRC�и��ַ���BUFF_TGT�����ַ�һ��
	}//for(i=0; i<ul_Quan_Src;


	//��ƥ��
	return false;
}

bool CGFC::Confirm_Reg_Dynamic_Com_Port( int i_Tgt_Com_Port, int i_TimeOut )
{
	time_t t_Start, t_End;

	//
	try
	{
		time( &t_Start );

		//
		do
		{
			//����regedit�е�COM�б�
			if( this->Confirm_Reg_Com_Port( i_Tgt_Com_Port ) == true )
			{
				break;
			}

			//����Ƿ�ʱ
			time( &t_End );

			if( (int)difftime( t_End, t_Start ) >= i_TimeOut )
			{
				throw(0);
			}

			Sleep( 500 );
		}
		while( true );
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_Local_PC_Name( CString * csp_PC_Name )
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char crData[64];
	wchar_t wcrTemp[64];
	int i;

	memset( crData, 0, sizeof( crData ) );
	memset( wcrTemp, 0, sizeof( wcrTemp ) );

	//Get Computer Name
	wVersionRequested = MAKEWORD( 2, 0 );
	WSAStartup( wVersionRequested, &wsaData );

	//
	if( ::gethostname( crData, 60 ) != 0 )
	{
		memset( crData, 0, sizeof( crData ) );
	}

	//
	WSACleanup();

	//
	if( strlen( crData ) > 0 )
	{
		this->MultiByte_To_WideChar( crData, wcrTemp, &i );
		csp_PC_Name->Format( _T( "%s" ), wcrTemp );
		return true;
	}
	else
	{
		csp_PC_Name->Format( _T( "No Name" ) );
		return false;
	}
}

bool CGFC::GET_AT_RSP_DIGIT_DATA( const wchar_t * wcp_Src_Data, const wchar_t * wcp_Key_Word, int i_Line_Offset, double * dp_Digit_Data )
{
	//��ȡ������ATָ��ظ�ʽ��Ӧ�����롰�ؼ��ʡ�����ϵ���ض����С����ݣ���\r\n���з�Ϊ�����
	//Ȼ��ȥ���ؼ��ʣ���ȡһ������ֵ����Ϊ����ֵ
	//�롰���֡���صġ��ؼ��ַ���������������

	CString csUseful, csData;
	int iLen, i;
	wchar_t wcByte;
	bool bGet_Numb_Byte = false;//�ҵ���һ���롰���֡���صġ��ؼ��ַ���

	try
	{
		//ѭ���������ҹؼ������ݶ�
		if( this->GET_AT_RSP_STRING( wcp_Src_Data, wcp_Key_Word, i_Line_Offset, &csUseful ) == false )
		{
			throw(0);
		}

		//�С�key word����Ҫȥ���ؼ����Լ�֮ǰ�����ݣ�����ȡ��ֵ
		iLen = csUseful.Find( wcp_Key_Word );
		if( iLen >= 0 )
		{
			csUseful.Delete( 0, (iLen + wcslen( wcp_Key_Word )) );
		}

		//���ַ���ת��Ϊ����
		csUseful.MakeUpper();
		iLen = csUseful.GetLength();
		csData.Format( _T( "" ) );

		for( i = 0; i < iLen; i++ )
		{
			wcByte = csUseful.GetAt( i );

			if( bGet_Numb_Byte == false )//��ʼ�ҵ�һ���롰���֡���صġ��ؼ��ַ���
			{
				//û�п�ʼ���롰���֡�����

				if( ((wcByte >= '0') && (wcByte <= '9')) ||
					(wcByte == '+') || (wcByte == '-') || (wcByte == '.') || (wcByte == 'E') )
				{
					//��Ч�����֡����ݣ�����buff
					csData += wcByte;
					bGet_Numb_Byte = true;
				}
			}
			else
			{
				//�Ѿ���ʼ���롰���֡�����

				if( ((wcByte >= '0') && (wcByte <= '9')) ||
					(wcByte == '+') || (wcByte == '-') || (wcByte == '.') || (wcByte == 'E') )
				{
					//��Ч�����֡����ݣ�����buff
					csData += wcByte;
				}
				else
				{
					//��Ч�����֡����ݣ���������buff
					break;//�˳�for
				}
			}
		}//for(i=0; i<iTemp; i++

		//ת����doubleֵ������
		if( csData.GetLength() > 0 )
		{
			*dp_Digit_Data = _wtof( csData );
		}
		else
		{
			throw(0);
		}
	}
	catch( ... )
	{
		return false;
	}

	return true;
}

bool CGFC::GET_AT_RSP_STRING( const wchar_t * wcp_Src_Data, const wchar_t * wcp_Key_Word, int i_Line_Offset, CString * csp_String )
{
	//��ȡ������ATָ��ظ�ʽ��Ӧ�����롰�ؼ��ʡ�����ϵ���ض����С����ݣ���\r\n���з�Ϊ�����

	int iIndex = 0;//��������ʼindex���ӵ�һ����ʼ�ҡ�
	bool bGet_Key = false;
	CString csUseful;

	//
	try
	{
		//ѭ���������ҹؼ������ݶ�
		while( this->WCSTOK_STRING_C( wcp_Src_Data, _T( "\r\n" ), iIndex, &csUseful ) == true )
		{
			if( wcsstr( csUseful, wcp_Key_Word ) != NULL )//�ҵ�OK��
			{
				bGet_Key = true;
				break;//�ҵ������ֶ��ˣ��˳�while
			}
			else
			{
				iIndex++;//����һ��
			}
		}

		if( bGet_Key == false )//û���ҵ��ؼ���
		{
			throw(0);
		}

		if( i_Line_Offset != 0 )//���Ǵӡ��ؼ��ʡ�����ȡ���ݣ�������Ҫ��������������
		{
			iIndex += i_Line_Offset;

			if( iIndex < 0 )
			{
				throw(0);
			}

			//�����������ٴ���ȡ����
			if( this->WCSTOK_STRING_C( wcp_Src_Data, _T( "\r\n" ), iIndex, &csUseful ) == false )
			{
				throw(0);
			}
		}

		//����ֵ
		csp_String->Format( _T( "%s" ), csUseful );
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_Reg_Usb_Dev_List( const wchar_t * wcp_USB_PID_VID, CString * csp_USB_Dev_List, int * ip_USB_Quan )
{
	/*
	��ע���������Ӧ�ġ�USB_PID_VID�����е�����USB�豸���б�
	scan register, and list all USB Port installed with USB Flash Utility Driver
	����USB�淶�Ĺ涨�����е�USB�豸���й�Ӧ��ID��VID���Ͳ�Ʒʶ���루PID��������ͨ����ͬ��VID��PID������ͬ���豸��
	VID��PID���������ֽڳ������У���Ӧ��ID��VID���ɹ�Ӧ����USBִ����̳���룬ÿ����Ӧ�̵�VID��Ψһ�ģ�PID�ɹ�Ӧ�����о�����
	��������˵����ͬ�Ĳ�Ʒ����ͬ��Ʒ�Ĳ�ͬ�ͺš���ͬ�ͺŵĲ�ͬ��ƵĲ�Ʒ��ò��ò�ͬ��PID���Ա�������ͬ���ҵĲ�ͬ�豸��
	*/

	HKEY hKey_Root = HKEY_LOCAL_MACHINE;
	CString csSub_Key;
	HKEY hKey_Handle;
	DWORD dwIndex = 0;
	wchar_t * wcpValueName = NULL;
	DWORD dwValueSize = 128;
	DWORD dwDataSize;

	//
	try
	{
		//sub key
		csSub_Key.Format( _T( "SYSTEM\\CurrentControlSet\\Enum\\USB\\%s" ), wcp_USB_PID_VID );

		//open reg
		if( ::RegOpenKeyEx( hKey_Root, csSub_Key, 0, KEY_READ, &hKey_Handle ) != ERROR_SUCCESS )
		{
			throw(1);
		}

		//�����ڴ�
		this->BUFF_WCHAR_CALLOC( &wcpValueName, dwValueSize );

		//
		do
		{
			//
			dwDataSize = dwValueSize;
			memset( wcpValueName, 0, dwValueSize );

			//��reg
			if( ::RegEnumKeyEx( hKey_Handle, dwIndex, wcpValueName, &dwDataSize, (LPDWORD)NULL, (LPTSTR)NULL, (LPDWORD)NULL, (PFILETIME)NULL ) != ERROR_SUCCESS )
			{
				break;//�˳�while, ERROR_NO_MORE_ITEMS
			}

			//��������
			(csp_USB_Dev_List + dwIndex)->Format( _T( "%s" ), wcpValueName );

			//����
			dwIndex++;

			if( dwIndex >= 64 )
			{
				::AfxMessageBox( _T( "ERROR, Get_Reg_Usb_Dev_List, Too many usb device!" ) );
				throw(2);
			}
		}
		while( true );

		//close reg
		::RegCloseKey( hKey_Handle );

		//
		*ip_USB_Quan = dwIndex;//�ҵ���USB����Ŀ
	}
	catch( ... )
	{
		//�ͷ��ڴ�
		this->BUFF_WCHAR_FREE( &wcpValueName );
		//fail
		return false;
	}

	//�ͷ��ڴ�
	this->BUFF_WCHAR_FREE( &wcpValueName );
	//pass
	return true;
}

bool CGFC::Get_File_Directory( const wchar_t * wcp_File_Path, CString * csp_File_Directory )
{
	//��ȡ�ļ�·���������ļ��У�

	int i, iIdx;
	CString csPath;

	csPath.Format( _T( "%s" ), wcp_File_Path );

	iIdx = csPath.GetLength() - 1;

	for( i = iIdx; i >= 0; i-- )
	{
		if( csPath.GetAt( i ) == '\\' )
		{
			csPath.SetAt( i, '\0' );
			break;
		}
	}

	if( i < 0 )
	{
		return false;
	}
	else
	{
		csp_File_Directory->Format( _T( "%s" ), csPath );
	}

	return true;
}

bool CGFC::Kill_Process( const wchar_t * wcp_Process_Name )
{
	//��ֹĳ���̣�process��

	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
	HANDLE hTargetProcess = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 strProcess_Info;
	CString csCheck, csTgt_Name;
	BOOL bStart;

	g_del_exe_Ctrl.Lock();//ֻ����һ��һ���߳̽���ɾ��

	try
	{
		//��ϵͳ�����еĽ����ĸ�����
		hProcessSnap = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

		if( hProcessSnap == INVALID_HANDLE_VALUE )
		{
			throw(0);
		}


		//����Ŀ����
		csTgt_Name.Format( _T( "%s" ), wcp_Process_Name );
		csTgt_Name.MakeLower();


		//��ʹ������ṹǰ�����������Ĵ�С
		strProcess_Info.dwSize = sizeof( PROCESSENTRY32 );

		//������ǰ�Ľ����б�Ѱ���Ƿ����û�����Ľ���
		//�������̿��գ�������ʾÿ�����̵���Ϣ
		bStart = Process32First( hProcessSnap, &strProcess_Info );

		while( bStart )
		{
			//��ý����б��еĽ�����
			csCheck.Format( _T( "%s" ), strProcess_Info.szExeFile );
			csCheck.MakeLower();

			if( csCheck.Compare( csTgt_Name ) == 0 )//Zero if the strings are identical
			{
				//�ӿ��ս����л�ȡ�ý��̵�PID(������������е�PID)
				hTargetProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, strProcess_Info.th32ProcessID );

				if( hTargetProcess == NULL )
				{
					//					::AfxMessageBox(_T("TargetProcess = NULL"));

					throw(0);
				}

				//��ֹ����
				TerminateProcess( hTargetProcess, 0 );
				CloseHandle( hTargetProcess );
			}

			//����һ��
			bStart = Process32Next( hProcessSnap, &strProcess_Info );
		}//while(bStart

		//ע�⣺�ر��������ķ�ʽ��������
		CloseHandle( hProcessSnap );
	}
	catch( ... )
	{
		g_del_exe_Ctrl.Unlock();

		return false;
	}

	g_del_exe_Ctrl.Unlock();
	return true;
}

bool CGFC::Calcu_ARFCN_GSM( int i_G_Band_Index, double d_Center_Freq, int i_Up_Down_Link, int * ip_ARFCN )
{
	//��GSMƵ�㣨MHz�������ȡ�ŵ���
	//ע�⣺��������������������������ܻ���ΪС���������ݶ���׼�����Լ����ŵ�ʱ����MHzת��ΪKHz�ܱ�֤��ȷ
	//ע�⣺�����㡱ת�������͡������ܶ�ʧС����ǰ��һ�����ݣ����磺1.0 = 0.999999999 = 0

	try
	{
		switch( i_G_Band_Index )
		{
		case G_BAND_GSM900:
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//Rx����
				{
					if( (d_Center_Freq >= 925.2) && (d_Center_Freq <= 934.8) )//G900: 975 ~ 1023
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 925200) / 200 + 975);
					}
					else if( (d_Center_Freq >= 935.0) && (d_Center_Freq <= 959.8) )//G900: 0 ~ 124
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 935000) / 200 + 0);
					}
					else
					{
						throw(0);
					}
				}
				else//Tx����
				{
					if( (d_Center_Freq >= 880.2) && (d_Center_Freq <= 889.8) )//G900: 975 ~ 1023
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 880200) / 200 + 975);
					}
					else if( (d_Center_Freq >= 890.0) && (d_Center_Freq <= 914.8) )//G900: 0 ~ 124
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 890000) / 200 + 0);
					}
					else
					{
						throw(0);
					}
				}
			}
			break;
		case G_BAND_DCS1800:
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//Rx����
				{
					if( (d_Center_Freq >= 1805.2) && (d_Center_Freq <= 1879.8) )//DCS: 512 ~ 885
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1805200) / 200 + 512);
					}
					else
					{
						throw(0);
					}
				}
				else//Tx����
				{
					if( (d_Center_Freq >= 1710.2) && (d_Center_Freq <= 1784.8) )//DCS: 512 ~ 885
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1710200) / 200 + 512);
					}
					else
					{
						throw(0);
					}
				}
			}
			break;
		case G_BAND_PCS1900:
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//Rx����
				{
					if( (d_Center_Freq >= 1930.2) && (d_Center_Freq <= 1989.8) )//PCS: 512 ~ 810
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1930200) / 200 + 512);
					}
					else
					{
						throw(0);
					}
				}
				else//Tx����
				{
					if( (d_Center_Freq >= 1850.2) && (d_Center_Freq <= 1909.8) )//PCS: 512 ~ 810
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1850200) / 200 + 512);
					}
					else
					{
						throw(0);
					}
				}
			}
			break;
		case G_BAND_GSM850:
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//Rx����
				{
					if( (d_Center_Freq >= 869.2) && (d_Center_Freq <= 893.8) )//G850: 128 ~ 251
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 869200) / 200 + 128);
					}
					else
					{
						throw(0);
					}
				}
				else//Tx����
				{
					if( (d_Center_Freq >= 824.2) && (d_Center_Freq <= 848.8) )//G850: 128 ~ 251
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 824200) / 200 + 128);
					}
					else
					{
						throw(0);
					}
				}
			}
			break;
		default:
			throw(0);
		}//switch( i_G_Band_Index
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "ERROR, Calcu_ARFCN_GSM(%d, %.2f, %d), Data is wrong!" ), i_G_Band_Index, d_Center_Freq, i_Up_Down_Link );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Calcu_Center_Freq_GSM( int i_G_Band_Index, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq )
{
	//��GSM�ŵ��ż����ȡƵ�㣨MHz��

	switch( i_G_Band_Index )
	{
	case G_BAND_GSM900:
		{
			if( (0 <= i_ARFCN) && (i_ARFCN <= 124) ) // PGSM, EGSM
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					*dp_Center_Freq = 890.0 + 0.2 * i_ARFCN + 45.0;
				}
				else
				{
					*dp_Center_Freq = 890.0 + 0.2 * i_ARFCN;
				}
			}
			else if( (975 <= i_ARFCN) && (i_ARFCN <= 1023) ) // EGSM, PGSM
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					*dp_Center_Freq = 890.0 + 0.2 * (i_ARFCN - 1024) + 45.0;
				}
				else
				{
					*dp_Center_Freq = 890.0 + 0.2 * (i_ARFCN - 1024);
				}
			}
			else
			{
				return false;
			}
		}
		break;

	case G_BAND_DCS1800:
		{
			if( (512 <= i_ARFCN) && (i_ARFCN <= 885) )
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					*dp_Center_Freq = 1710.2 + 0.2 * (i_ARFCN - 512) + 95.0;
				}
				else
				{
					*dp_Center_Freq = 1710.2 + 0.2 * (i_ARFCN - 512);
				}
			}
			else
			{
				return false;
			}
		}
		break;

	case G_BAND_PCS1900:
		{
			if( (512 <= i_ARFCN) && (i_ARFCN <= 810) )
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					*dp_Center_Freq = 1850.2 + 0.2 * (i_ARFCN - 512) + 80.0;
				}
				else
				{
					*dp_Center_Freq = 1850.2 + 0.2 * (i_ARFCN - 512);
				}
			}
			else
			{
				return false;
			}
		}
		break;

	case G_BAND_GSM850:
		{
			if( (128 <= i_ARFCN) && (i_ARFCN <= 251) )
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					*dp_Center_Freq = 824.2 + 0.2 * (i_ARFCN - 128) + 45.0;
				}
				else
				{
					*dp_Center_Freq = 824.2 + 0.2 * (i_ARFCN - 128);
				}
			}
			else
			{
				return false;
			}
		}
		break;

	default:
		return false;
	}//switch( i_G_Band_Index

	//pass
	return true;
}

bool CGFC::Calcu_Center_Freq_WCDMA( int i_W_Band_Index, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq )
{
	//��WCDMA�ŵ��ż����ȡƵ�㣨MHz��
	//WCDMA�¼�Ƶ�δ���λ��

	//
	try
	{
		switch( i_W_Band_Index )
		{
		case W_BAND_1:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (9575 <= i_ARFCN) && (i_ARFCN <= 9925) ) //W_BAND_1 ����
					{
						*dp_Center_Freq = 1922.6 + 0.2 * (i_ARFCN - 9613);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (10563 <= i_ARFCN) && (i_ARFCN <= 10838) ) //W_BAND_1 ����
					{
						*dp_Center_Freq = 2112.6 + 0.2 * (i_ARFCN - 10563);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_2:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (9225 <= i_ARFCN) && (i_ARFCN <= 9575) ) //W_BAND_2 ����
					{
						*dp_Center_Freq = 1852.6 + 0.2 * (i_ARFCN - 9263);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (9662 <= i_ARFCN) && (i_ARFCN <= 9938) ) //W_BAND_2 ����
					{
						*dp_Center_Freq = 1932.6 + 0.2 * (i_ARFCN - 9663);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_3:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (937 <= i_ARFCN) && (i_ARFCN <= 1288) ) //W_BAND_3 ����
					{
						*dp_Center_Freq = 1712.4 + 0.2 * (i_ARFCN - 937);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (1162 <= i_ARFCN) && (i_ARFCN <= 1513) ) //W_BAND_3 ����
					{
						*dp_Center_Freq = 1807.4 + 0.2 * (i_ARFCN - 1162);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_4:
			{
				if( RF_TX_UL_INPUT == i_Up_Down_Link )
				{
					if( (1312 <= i_ARFCN) && (i_ARFCN <= 1513) ) //W_BAND_4 ����
					{
						*dp_Center_Freq = 1712.4 + 0.2 * (i_ARFCN - 1312);
					}
					else
					{
						throw(0);
					}
				}
				else if( RF_RX_DL_OUTPUT == i_Up_Down_Link )
				{
					if( (1537 <= i_ARFCN) && (i_ARFCN <= 1738) ) //W_BAND_4 ����
					{
						*dp_Center_Freq = 2112.4 + 0.2 * (i_ARFCN - 1537);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case W_BAND_5:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (4107 <= i_ARFCN) && (i_ARFCN <= 4257) ) //W_BAND_5 ����
					{
						*dp_Center_Freq = 826.6 + 0.2 * (i_ARFCN - 4133);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (4357 <= i_ARFCN) && (i_ARFCN <= 4458) ) //W_BAND_5 ����
					{
						*dp_Center_Freq = 871.6 + 0.2 * (i_ARFCN - 4358);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_6:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//W_BAND_6 ����
				{
					if( i_ARFCN == 812 )
					{
						*dp_Center_Freq = 832.5;
					}
					else if( i_ARFCN == 837 )
					{
						*dp_Center_Freq = 837.5;
					}
					else if( (4162 <= i_ARFCN) && (i_ARFCN <= 4188) )
					{
						*dp_Center_Freq = 832.4 + 0.2 * (i_ARFCN - 4162);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//W_BAND_6 ����
				{
					if( i_ARFCN == 1037 )
					{
						*dp_Center_Freq = 877.5;
					}
					else if( i_ARFCN == 1062 )
					{
						*dp_Center_Freq = 882.5;
					}
					else if( (4387 <= i_ARFCN) && (i_ARFCN <= 4413) )
					{
						*dp_Center_Freq = 877.4 + 0.2 * (i_ARFCN - 4387);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_8:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (2687 <= i_ARFCN) && (i_ARFCN <= 2887) ) //W_BAND_8 ����
					{
						*dp_Center_Freq = 882.4 + 0.2 * (i_ARFCN - 2712);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (2937 <= i_ARFCN) && (i_ARFCN <= 3088) ) //W_BAND_8 ����
					{
						*dp_Center_Freq = 927.4 + 0.2 * (i_ARFCN - 2937);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_9:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (8762 <= i_ARFCN) && (i_ARFCN <= 8912) ) //W_BAND_9 ����
					{
						*dp_Center_Freq = 1752.4 + 0.2 * (i_ARFCN - 8762);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (9237 <= i_ARFCN) && (i_ARFCN <= 9387) ) //W_BAND_9 ����
					{
						*dp_Center_Freq = 1847.4 + 0.2 * (i_ARFCN - 9237);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_19:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//W_BAND_19 ����
				{
					if( i_ARFCN == 387 )
					{
						*dp_Center_Freq = 832.5;
					}
					else if( i_ARFCN == 412 )
					{
						*dp_Center_Freq = 837.5;
					}
					else if( i_ARFCN == 437 )
					{
						*dp_Center_Freq = 842.5;
					}
					else if( (312 <= i_ARFCN) && (i_ARFCN <= 363) )
					{
						*dp_Center_Freq = 832.4 + 0.2 * (i_ARFCN - 312);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//W_BAND_19 ����
				{
					if( i_ARFCN == 787 )
					{
						*dp_Center_Freq = 877.5;
					}
					else if( i_ARFCN == 812 )
					{
						*dp_Center_Freq = 882.5;
					}
					else if( i_ARFCN == 837 )
					{
						*dp_Center_Freq = 887.5;
					}
					else if( (712 <= i_ARFCN) && (i_ARFCN <= 763) )
					{
						*dp_Center_Freq = 877.4 + 0.2 * (i_ARFCN - 712);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		default:
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "Calcu_Center_Freq_WCDMA( W_Band_Index = %d, ARFCN = %d, Up_Down_Link = %d ), Fail !" ), i_W_Band_Index, i_ARFCN, i_Up_Down_Link );
		//
		return false;
	}

	//
	return true;
}

bool CGFC::GET_AT_RSP_MID_STRING( const wchar_t * wcp_Src_Data, const wchar_t * wcp_Start_Key, const wchar_t * wcp_End_Key, CString * csp_Useful_String )
{
	//��ȡATָ����Ӧ�У�����ʼ���ؼ��ʺ͡��������ؼ���֮����ַ�����

	wchar_t * wcpSrc = NULL;//��Դ���ݡ���buff��wcstok����ȡ����ʱ��ı�Դ�������ݣ������Ȱ�Դ���ݷ���buff������buff���ݣ����˺�����Դ��
	unsigned long ulBuff_Len = wcslen( wcp_Src_Data ) + 8;//���ڴ�š�Դ���ݡ���buff����
	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;
	int iLen, i;
	CString csData;

	try
	{

		//�����ڴ�
		this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

		//���롰Դ���ݡ�
		swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_Src_Data );

		//����Ƿ��С���ʼ���ؼ���
		wcpHead = wcsstr( wcpSrc, wcp_Start_Key );
		if( wcpHead == NULL )
		{
			this->m_csError_Info.Format( _T( "ERROR, GET_AT_RSP_MID_STRING, No start key!" ) );
			throw(0);
		}

		//ȥ������ʼ���ؼ���
		wcpHead += wcslen( wcp_Start_Key );


		//����Ƿ��С��������ؼ���
		wcpEnd = wcsstr( wcpHead, wcp_End_Key );
		if( wcpEnd == NULL )
		{
			this->m_csError_Info.Format( _T( "ERROR, GET_AT_RSP_MID_STRING, No end key!" ) );
			throw(0);
		}

		//���㡰�������ݡ�����
		iLen = wcpEnd - wcpHead;

		//��ȡ��������
		csData.Format( _T( "" ) );
		for( i = 0; i < iLen; i++ )
		{
			csData += *(wcpHead + i);
		}


		//����ֵ
		csp_Useful_String->Format( _T( "%s" ), csData );
	}
	catch( ... )
	{
		//�ͷ��ڴ�
		this->BUFF_WCHAR_FREE( &wcpSrc );
		return false;
	}

	//�ͷ��ڴ�
	this->BUFF_WCHAR_FREE( &wcpSrc );
	return true;
}

bool CGFC::Calcu_ARFCN_WCDMA( int i_W_Band_Index, double d_Center_Freq, int i_Up_Down_Link, int * ip_ARFCN )
{
	//��WCDMAƵ�㣨MHz�������ŵ���
	//WCDMA�¼�Ƶ�δ���λ��

	//
	try
	{
		switch( i_W_Band_Index )
		{
		case W_BAND_1:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 1922.6) && (d_Center_Freq <= 1977.4) )//BAND_1 ����
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1922600) / 200 + 9613);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 2112.6) && (d_Center_Freq <= 2167.4) )//BAND_1 ����
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 2112600) / 200 + 10563);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_2:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 1852.4) && (d_Center_Freq <= 1907.6) )//BAND_2 ����
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1852600) / 200 + 9263);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 1932.4) && (d_Center_Freq <= 1987.6) )//BAND_2 ����
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1932600) / 200 + 9663);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_3:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 1712.4) && (d_Center_Freq <= 1782.6) )//BAND_3 ����
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1712400) / 200 + 937);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 1807.4) && (d_Center_Freq <= 1877.6) )//BAND_3 ����
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1807400) / 200 + 1162);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_4:
			{
				if( RF_TX_UL_INPUT == i_Up_Down_Link )
				{
					if( (d_Center_Freq >= 1712.4) && (d_Center_Freq <= 1752.6) )//W_BAND_4 ����
					{
						*ip_ARFCN = (d_Center_Freq * 1000 - 1712400) / 200 + 1312;
					}
					else
					{
						throw(0);
					}
				}
				else if( RF_RX_DL_OUTPUT == i_Up_Down_Link )
				{
					if( (d_Center_Freq >= 2112.4) && (d_Center_Freq <= 2152.6) )//W_BAND_4 ����
					{
						*ip_ARFCN = (d_Center_Freq * 1000 - 2112400) / 200 + 1537;
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_5:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 826.4) && (d_Center_Freq <= 846.6) )//BAND_5 ����
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 826600) / 200 + 4133);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 871.4) && (d_Center_Freq <= 891.6) )//BAND_5 ����
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 871600) / 200 + 4358);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_6:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//W_BAND_6 ����
				{
					if( d_Center_Freq == 832.5 )
					{
						*ip_ARFCN = 812;
					}
					else if( d_Center_Freq == 837.5 )
					{
						*ip_ARFCN = 837;
					}
					else if( (d_Center_Freq >= 832.4) && (d_Center_Freq <= 837.6) )
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 832400) / 200 + 4162);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//W_BAND_6 ����
				{
					if( d_Center_Freq == 877.5 )
					{
						*ip_ARFCN = 1037;
					}
					else if( d_Center_Freq == 882.5 )
					{
						*ip_ARFCN = 1062;
					}
					else if( (d_Center_Freq >= 877.4) && (d_Center_Freq <= 882.6) )
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 877400) / 200 + 4387);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_8:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 882.4) && (d_Center_Freq <= 912.6) )//BAND_8 ����
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 882400) / 200 + 2712);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 927.4) && (d_Center_Freq <= 957.6) )//BAND_8 ����
					{
						*ip_ARFCN = (d_Center_Freq * 1000 - 927400) / 200 + 2937;
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_9:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 1752.4) && (d_Center_Freq <= 1782.4) )//W_BAND_9 ����
					{
						*ip_ARFCN = (d_Center_Freq * 1000.0 - 1752400) / 200 + 8762;
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 1847.4) && (d_Center_Freq <= 1877.4) )//W_BAND_9 ����
					{
						*ip_ARFCN = (d_Center_Freq * 1000.0 - 1847400) / 200 + 9237;
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_19:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//W_BAND_19 ����
				{
					if( d_Center_Freq == 832.5 )
					{
						*ip_ARFCN = 387;
					}
					else if( d_Center_Freq == 837.5 )
					{
						*ip_ARFCN = 412;
					}
					else if( d_Center_Freq == 842.5 )
					{
						*ip_ARFCN = 437;
					}
					else if( (d_Center_Freq >= 832.4) && (d_Center_Freq <= 842.6) )
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 832400) / 200 + 312);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//W_BAND_19 ����
				{
					if( d_Center_Freq == 877.5 )
					{
						*ip_ARFCN = 787;
					}
					else if( d_Center_Freq == 882.5 )
					{
						*ip_ARFCN = 812;
					}
					else if( d_Center_Freq == 887.5 )
					{
						*ip_ARFCN = 837;
					}
					else if( (d_Center_Freq >= 877.4) && (d_Center_Freq <= 887.6) )
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 877400) / 200 + 712);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		default:
			throw(0);
		}//switch( i_W_Band_Index
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "Calcu_ARFCN_WCDMA( W_Band_Index = %d, Center_Freq = %.4f, Up_Down_Link = %d ), Fail !" ), i_W_Band_Index, d_Center_Freq, i_Up_Down_Link );
		//fail
		return false;
	}

	//pass
	return true;
}

void CGFC::Empty_Directory( const wchar_t * wcp_Dir_Path )
{
	//����ļ���

	CString csDir;

	//
	csDir.Format( _T( "%s" ), wcp_Dir_Path );

	//
	if( csDir.IsEmpty() )//true���ǡ��ա��ִ�
	{
		//�ǡ��ա��ִ�
		//��������
		return;
	}

	// ����ɾ���ļ������ļ���
	CFileFind cla_Finder;
	BOOL bFound = cla_Finder.FindFile( csDir + _T( "\\*" ), 0 );

	while( bFound )
	{
		bFound = cla_Finder.FindNextFile();
		if( (cla_Finder.GetFileName() == _T( "." )) || (cla_Finder.GetFileName() == _T( ".." )) )
		{
			continue;
		}

		// ȥ���ļ�(��)ֻ��������
		SetFileAttributes( cla_Finder.GetFilePath(), FILE_ATTRIBUTE_NORMAL );

		if( cla_Finder.IsDirectory() )
		{
			// �ݹ�ɾ�����ļ���
			Empty_Directory( cla_Finder.GetFilePath() );
			RemoveDirectory( cla_Finder.GetFilePath() );
		}
		else
		{
			// ɾ���ļ�
			DeleteFile( cla_Finder.GetFilePath() );
		}
	}

	//
	cla_Finder.Close();

	//����ɾ����ǰ�ļ���
//	RemoveDirectory(csDir);
}

bool CGFC::Get_Reg_Usb_Dev_Friendly_Name( const wchar_t * wcp_USB_PID_VID, const wchar_t * wcp_USB_Dev_Name, CString * csp_Friendly_Name )
{
	//��USB�豸�ġ�FriendlyName��

	HKEY hKey_Root = HKEY_LOCAL_MACHINE;
	CString csSub_Key;
	HKEY hKey_Handle;
	wchar_t wcrValue_Data[128];
	DWORD dwData_Size;
	DWORD dwData_Type = REG_SZ;

	//
	try
	{
		//sub key
		csSub_Key.Format( _T( "SYSTEM\\CurrentControlSet\\Enum\\USB\\%s\\%s" ), wcp_USB_PID_VID, wcp_USB_Dev_Name );

		//open reg
		if( ::RegOpenKeyEx( hKey_Root, csSub_Key, 0, KEY_READ, &hKey_Handle ) != ERROR_SUCCESS )
		{
			throw(1);
		}

		//
		dwData_Size = 128;
		memset( wcrValue_Data, 0, sizeof( wcrValue_Data ) );

		//��ָ����Ŀ������
		if( ::RegQueryValueEx( hKey_Handle, _T( "FriendlyName" ), (LPDWORD)NULL, &dwData_Type, (BYTE *)wcrValue_Data, &dwData_Size ) != ERROR_SUCCESS )
		{
			throw(2);
		}

		//close reg
		::RegCloseKey( hKey_Handle );

		//
		csp_Friendly_Name->Format( _T( "%s" ), wcrValue_Data );
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

void CGFC::WCSTOK_STRING_T( const wchar_t * wcp_StrToken, CString * csp_Get_CString_Array, int * ip_Get_Quan )
{
	//��ͳ����ȡ���Ӵ���

	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//��Դ���ݡ���buff��wcstok����ȡ����ʱ��ı�Դ�������ݣ������Ȱ�Դ���ݷ���buff������buff���ݣ����˺�����Դ��
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//���ڴ�š�Դ���ݡ���buff����
	WCHAR * wcpResult = NULL;


	//�����ڴ�
	this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

	//���롰Դ���ݡ�
	swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

	//��ȡ��һ��ֵ(0)
	wcpEach = wcstok_s( wcpSrc, _T( " ,;\r\n" ), &wcpResult );

	while( wcpEach != NULL )
	{
		//��ֵ
		(csp_Get_CString_Array + i)->Format( _T( "%s" ), wcpEach );

		//��ȡ��һ��ֵ
		wcpEach = wcstok_s( NULL, _T( " ,;\r\n" ), &wcpResult );

		//����
		i++;
	}//while(wcpEach != NULL

	//��������
	*ip_Get_Quan = i;


	//�ͷ��ڴ�
	this->BUFF_WCHAR_FREE( &wcpSrc );
}

bool CGFC::Calcu_Center_Freq_LTE( int i_L_Band_Name, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq_MHz )
{
	//��LTE�ŵ��ż����ȡƵ�㣨MHz��
	//LTE�¼�Ƶ�δ���λ��

	//
	try
	{
		switch( i_L_Band_Name )
		{
		case 1://L_BAND_1
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1920.0 + 0.1 * (i_ARFCN - 18000);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 2110.0 + 0.1 * (i_ARFCN - 0);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 2://L_BAND_2
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1850.0 + 0.1 * (i_ARFCN - 18600);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 1930.0 + 0.1 * (i_ARFCN - 600);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 3://L_BAND_3
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1710.0 + 0.1 * (i_ARFCN - 19200);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 1805.0 + 0.1 * (i_ARFCN - 1200);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 4://L_BAND_4
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1710.0 + 0.1 * (i_ARFCN - 19950);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 2110.0 + 0.1 * (i_ARFCN - 1950);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 5://L_BAND_5
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 824.0 + 0.1 * (i_ARFCN - 20400);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 869.0 + 0.1 * (i_ARFCN - 2400);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 7://L_BAND_7
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 2500.0 + 0.1 * (i_ARFCN - 20750);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 2620.0 + 0.1 * (i_ARFCN - 2750);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 8://L_BAND_8
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 880.0 + 0.1 * (i_ARFCN - 21450);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 925.0 + 0.1 * (i_ARFCN - 3450);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 9://L_BAND_9
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1749.9 + 0.1 * (i_ARFCN - 21800);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 1844.9 + 0.1 * (i_ARFCN - 3800);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 10:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1710.0 + 0.1 * (i_ARFCN - 22150);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 2110.0 + 0.1 * (i_ARFCN - 4150);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 11://L_BAND_11
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1427.9 + 0.1 * (i_ARFCN - 22750);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 1475.9 + 0.1 * (i_ARFCN - 4750);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 12://L_BAND_12
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 699.0 + 0.1 * (i_ARFCN - 23010);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 729.0 + 0.1 * (i_ARFCN - 5010);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 13://L_BAND_13
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 777.0 + 0.1 * (i_ARFCN - 23180);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 746.0 + 0.1 * (i_ARFCN - 5180);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 14://L_BAND_14
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 788.0 + 0.1 * (i_ARFCN - 23280);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 758.0 + 0.1 * (i_ARFCN - 5280);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 17://L_BAND_17
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 704.0 + 0.1 * (i_ARFCN - 23730);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 734.0 + 0.1 * (i_ARFCN - 5730);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 18://L_BAND_18
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 815.0 + 0.1 * (i_ARFCN - 23850);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 860.0 + 0.1 * (i_ARFCN - 5850);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 19://L_BAND_19
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 830.0 + 0.1 * (i_ARFCN - 24000);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 875.0 + 0.1 * (i_ARFCN - 6000);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 20://L_BAND_20
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 832.0 + 0.1 * (i_ARFCN - 24150);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 791.0 + 0.1 * (i_ARFCN - 6150);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 21://L_BAND_21
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1447.9 + 0.1 * (i_ARFCN - 24450);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 1495.9 + 0.1 * (i_ARFCN - 6450);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 22:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 3410.0 + 0.1 * (i_ARFCN - 24600);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 3510.0 + 0.1 * (i_ARFCN - 6600);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 24:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1626.5 + 0.1 * (i_ARFCN - 25700);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 1525.0 + 0.1 * (i_ARFCN - 7700);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 25://L_BAND_25
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1850.0 + 0.1 * (i_ARFCN - 26040);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 1930.0 + 0.1 * (i_ARFCN - 8040);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 26://L_BAND_26
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 814.0 + 0.1 * (i_ARFCN - 26690);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 859.0 + 0.1 * (i_ARFCN - 8690);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 27://L_BAND_27
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 807.0 + 0.1 * (i_ARFCN - 27040);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 852.0 + 0.1 * (i_ARFCN - 9040);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 28://L_BAND_28
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 703.0 + 0.1 * (i_ARFCN - 27210);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 758.0 + 0.1 * (i_ARFCN - 9210);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 29://L_BAND_29
			{
				*dp_Center_Freq_MHz = 717.0 + 0.1 * (i_ARFCN - 9660);
			}
			//
			break;

		case 30://L_BAND_30
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 2305.0 + 0.1 * (i_ARFCN - 27660);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 2350.0 + 0.1 * (i_ARFCN - 9770);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 31://L_BAND_31 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 452.5 + 0.1 * (i_ARFCN - 27760);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 462.5 + 0.1 * (i_ARFCN - 9870);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 32://L_BAND_32
			{
				*dp_Center_Freq_MHz = 1452.0 + 0.1 * (i_ARFCN - 9920);
			}
			//
			break;

		case 34://L_BAND_34
			{
				*dp_Center_Freq_MHz = 2010.0 + 0.1 * (i_ARFCN - 36200);
			}
			//
			break;

		case 38://L_BAND_38
			{
				*dp_Center_Freq_MHz = 2570.0 + 0.1 * (i_ARFCN - 37750);
			}
			//
			break;

		case 39://L_BAND_39
			{
				*dp_Center_Freq_MHz = 1880.0 + 0.1 * (i_ARFCN - 38250);
			}
			//
			break;

		case 40://L_BAND_40
			{
				*dp_Center_Freq_MHz = 2300.0 + 0.1 * (i_ARFCN - 38650);
			}
			//
			break;

		case 41://L_BAND_41
			{
				*dp_Center_Freq_MHz = 2496.0 + 0.1 * (i_ARFCN - 39650);
			}
			//
			break;

		case 42://L_BAND_42
			{
				*dp_Center_Freq_MHz = 3400.0 + 0.1 * (i_ARFCN - 41590);
			}
			//
			break;

		case 43://L_BAND_43
			{
				*dp_Center_Freq_MHz = 3600.0 + 0.1 * (i_ARFCN - 43590);
			}
			//
			break;

		case 46://L_BAND_46
			{
				*dp_Center_Freq_MHz = 5150.0 + 0.1 * (i_ARFCN - 46790);
			}
			//
			break;

		case 47://L_BAND_47
			{
				*dp_Center_Freq_MHz = 5855.0 + 0.1 * (i_ARFCN - 54540);
			}
			//
			break;

		case 48://L_BAND_48
			{
				*dp_Center_Freq_MHz = 3550.0 + 0.1 * (i_ARFCN - 55240);
			}
			//
			break;

		case 66://L_BAND_66 FDD
			{
				//ע�⣺Band66��DL��2180-2200 MHz������CAʱ���õ�
				//The range 2180-2200 MHz of the DL operating band is restricted to E-UTRA operation when carrier aggregation is configured.
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1710.0 + 0.1 * (i_ARFCN - 131972);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 2110.0 + 0.1 * (i_ARFCN - 66436);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 68:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 698.0 + 0.1 * (i_ARFCN - 132672);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 753.0 + 0.1 * (i_ARFCN - 67536);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 70:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1695.0 + 0.1 * (i_ARFCN - 132972);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 1995.0 + 0.1 * (i_ARFCN - 68336);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 71://L_BAND_71 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 663.0 + 0.1 * (i_ARFCN - 133122);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 617.0 + 0.1 * (i_ARFCN - 68586);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 72://L_BAND_72 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 451.0 + 0.1 * (i_ARFCN - 133472);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 461.0 + 0.1 * (i_ARFCN - 68936);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 73://L_BAND_73 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 450.0 + 0.1 * (i_ARFCN - 133522);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 460.0 + 0.1 * (i_ARFCN - 68986);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 74:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 1427.0 + 0.1 * (i_ARFCN - 133572);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 1475.0 + 0.1 * (i_ARFCN - 69036);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 85://L_BAND_85 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 698.0 + 0.1 * (i_ARFCN - 134002);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 728.0 + 0.1 * (i_ARFCN - 70366);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 86://L_BAND_86 NB-B86 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*dp_Center_Freq_MHz = 787.0 + 0.1 * (i_ARFCN - 134182);//Uplink Channel:134182-134191, FREQ:787.0-787.9MHz
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*dp_Center_Freq_MHz = 757.0 + 0.1 * (i_ARFCN - 70546);//Downlink Channel:70546-70555, FREQ:757.0-757.9MHz
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 87://L_BAND_87 FDD
			{
				if( RF_TX_UL_INPUT == i_Up_Down_Link )//����
				{
					*dp_Center_Freq_MHz = 410.0 + 0.1 * (i_ARFCN - 134182);
				}
				else if( RF_RX_DL_OUTPUT == i_Up_Down_Link )//����
				{
					*dp_Center_Freq_MHz = 420.0 + 0.1 * (i_ARFCN - 70546);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 88://L_BAND_88 FDD
			{
				if( RF_TX_UL_INPUT == i_Up_Down_Link )//����
				{
					*dp_Center_Freq_MHz = 412.0 + 0.1 * (i_ARFCN - 134231);
				}
				else if( RF_RX_DL_OUTPUT == i_Up_Down_Link )//����
				{
					*dp_Center_Freq_MHz = 422.0 + 0.1 * (i_ARFCN - 70596);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		default:
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "ERROR, Calcu_Center_Freq_LTE( L_Band_Name = %d, ARFCN = %d, Up_Down_Link = %d )" ), i_L_Band_Name, i_ARFCN, i_Up_Down_Link );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Calcu_ARFCN_LTE( int i_L_Band_Name, double d_Center_Freq_MHz, int i_Up_Down_Link, int * ip_ARFCN )
{
	//��LTEƵ�㣨MHz�������ȡ�ŵ���
	//LTE�¼�Ƶ�δ���λ��

	try
	{
		switch( i_L_Band_Name )
		{
		case 1://L_BAND_1
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1920.0) + 18000);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 2110.0) + 0);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 2://L_BAND_2
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1850.0) + 18600);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1930.0) + 600);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 3://L_BAND_3
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1710.0) + 19200);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1805.0) + 1200);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 4://L_BAND_4
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1710.0) + 19950);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 2110.0) + 1950);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 5://L_BAND_5
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 824.0) + 20400);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 869.0) + 2400);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 7://L_BAND_7
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 2500.0) + 20750);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 2620.0) + 2750);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 8://L_BAND_8
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 880.0) + 21450);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 925.0) + 3450;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;
			
		case 9://L_BAND_9
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1749.9) + 21800;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1844.9) + 3800;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 11://L_BAND_11
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1427.9) + 22750;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1475.9) + 4750;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 12://L_BAND_12
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 699.0) + 23010;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 729.0) + 5010;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 13://L_BAND_13
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 777.0) + 23180;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 746.0) + 5180;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 14://L_BAND_14
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 788.0) + 23280;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 758.0) + 5280;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 17://L_BAND_17
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 704.0) + 23730;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 734.0) + 5730;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 18://L_BAND_18
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 815.0) + 23850;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 860.0) + 5850;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 19://L_BAND_19
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 830.0) + 24000;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 875.0) + 6000;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 20://L_BAND_20
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 832.0) + 24150;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 791.0) + 6150;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 21://L_BAND_21
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1447.9) + 24450;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1495.9) + 6450;
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 25://L_BAND_25
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1850.0) + 26040;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1930.0) + 8040;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 26://L_BAND_26
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 814.0) + 26690;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 859.0) + 8690;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 27://L_BAND_27
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 807.0) + 27040;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 852.0) + 9040;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 28://L_BAND_28
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 703.0) + 27210;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 758.0) + 9210;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 29://L_BAND_29
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 717.0) + 9660;
			}
			//
			break;

		case 30://L_BAND_30
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2305.0) + 27660;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2350.0) + 9770;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 31://L_BAND_31 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 452.5) + 27760;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 462.5) + 9870;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 32://L_BAND_32
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1452.0) + 9920;
			}
			//
			break;

		case 34://L_BAND_34
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2010.0) + 36200;
			}
			//
			break;

		case 38://L_BAND_38
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2570.0) + 37750;
			}
			//
			break;

		case 39://L_BAND_39
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1880.0) + 38250;
			}
			//
			break;

		case 40://L_BAND_40
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2300.0) + 38650;
			}
			//
			break;

		case 41://L_BAND_41
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2496.0) + 39650;
			}
			//
			break;

		case 42://L_BAND_42
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 3400.0) + 41590;
			}
			//
			break;

		case 43://L_BAND_43
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 3600.0) + 43590;
			}
			//
			break;

		case 46://L_BAND_46
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 5150.0) + 46790;
			}
			//
			break;

		case 47://L_BAND_47
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 5855.0) + 54540;
			}
			//
			break;

		case 48://L_BAND_48
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 3550.0) + 55240;
			}
			//
			break;

		case 66://L_BAND_66 FDD
			{
				//ע�⣺Band66��DL��2180-2200 MHz������CAʱ���õ�
				//The range 2180-2200 MHz of the DL operating band is restricted to E-UTRA operation when carrier aggregation is configured.
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1710.0) + 131972;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2110.0) + 66436;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 71://L_BAND_71 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 663.0) + 133122;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 617.0) + 68586;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 72://L_BAND_72 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 451.0) + 133472;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 461.0) + 68936;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 73://L_BAND_73 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 450.0) + 133522;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 460.0) + 68986;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 85://L_BAND_85 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 698.0) + 134002;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 728.0) + 70366;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 86://L_BAND_86 NB-B86 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 787.0) + 134182;//Uplink Channel:134182-134191, FREQ:787.0-787.9MHz
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 757.0) + 70546;//Downlink Channel:70546-70555, FREQ:757.0-757.9MHz
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 87://L_BAND_87, FDD, NB_BAND_87
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 410.0) + 134182;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 420.0) + 70546;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 88://L_BAND_88, FDD, NB_BAND_88
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 412.0) + 134231;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 422.0) + 70596;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		default:
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "Calcu_ARFCN_LTE( L_Band = %d, Center_Freq = %.4f, Up_Down_Link = %d ), Fail !" ), i_L_Band_Name, d_Center_Freq_MHz, i_Up_Down_Link );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_LTE_Band_Name( int i_LTE_Band_Index, int * ip_L_Band_Name )
{
	//LTEƵ��index�� ת�� LTEƵ����
	//LTE�¼�Ƶ�δ���λ��

	//
	switch( i_LTE_Band_Index )
	{
	case L_BAND_1:
		*ip_L_Band_Name = 1;
		break;
	case L_BAND_2:
		*ip_L_Band_Name = 2;
		break;
	case L_BAND_3:
		*ip_L_Band_Name = 3;
		break;
	case L_BAND_4:
		*ip_L_Band_Name = 4;
		break;
	case L_BAND_5:
		*ip_L_Band_Name = 5;
		break;
	case L_BAND_7:
		*ip_L_Band_Name = 7;
		break;
	case L_BAND_8:
		*ip_L_Band_Name = 8;
		break;
	case L_BAND_9:
		*ip_L_Band_Name = 9;
		break;
	case L_BAND_11:
		*ip_L_Band_Name = 11;
		break;
	case L_BAND_12:
		*ip_L_Band_Name = 12;
		break;
	case L_BAND_13:
		*ip_L_Band_Name = 13;
		break;
	case L_BAND_14:
		*ip_L_Band_Name = 14;
		break;
	case L_BAND_17:
		*ip_L_Band_Name = 17;
		break;
	case L_BAND_18:
		*ip_L_Band_Name = 18;
		break;
	case L_BAND_19:
		*ip_L_Band_Name = 19;
		break;
	case L_BAND_20:
		*ip_L_Band_Name = 20;
		break;
	case L_BAND_21:
		*ip_L_Band_Name = 21;
		break;
	case L_BAND_25:
		*ip_L_Band_Name = 25;
		break;
	case L_BAND_26:
		*ip_L_Band_Name = 26;
		break;
	case L_BAND_27:
		*ip_L_Band_Name = 27;
		break;
	case L_BAND_28:
		*ip_L_Band_Name = 28;
		break;
	case L_BAND_29:
		*ip_L_Band_Name = 29;
		break;
	case L_BAND_30:
		*ip_L_Band_Name = 30;
		break;
	case L_BAND_31:
		*ip_L_Band_Name = 31;
		break;
	case L_BAND_32:
		*ip_L_Band_Name = 32;
		break;
	case L_BAND_34:
		*ip_L_Band_Name = 34;
		break;
	case L_BAND_38:
		*ip_L_Band_Name = 38;
		break;
	case L_BAND_39:
		*ip_L_Band_Name = 39;
		break;
	case L_BAND_40:
		*ip_L_Band_Name = 40;
		break;
	case L_BAND_41:
		*ip_L_Band_Name = 41;
		break;
	case L_BAND_42:
		*ip_L_Band_Name = 42;
		break;
	case L_BAND_43:
		*ip_L_Band_Name = 43;
		break;
	case L_BAND_46:
		*ip_L_Band_Name = 46;
		break;
	case L_BAND_47:
		*ip_L_Band_Name = 47;
		break;
	case L_BAND_48:
		*ip_L_Band_Name = 48;
		break;
	case L_BAND_66:
		*ip_L_Band_Name = 66;
		break;
	case L_BAND_71:
		*ip_L_Band_Name = 71;
		break;
	case L_BAND_72:
		*ip_L_Band_Name = 72;
		break;
	case L_BAND_73:
		*ip_L_Band_Name = 73;
		break;
	case L_BAND_85:
		*ip_L_Band_Name = 85;
		break;
	case L_BAND_87:
		*ip_L_Band_Name = 87;
		break;
	case L_BAND_88:
		*ip_L_Band_Name = 88;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_LTE_Band_Name, LTE_Band_Index(%d) is wrong!" ), i_LTE_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_LTE_Band_Index

	//pass
	return true;
}

bool CGFC::Get_LTE_Band_Index( int i_LTE_Band_Name, int * ip_L_Band_Index )
{
	//LTEƵ���� ת�� LTEƵ��index��
	//LTE�¼�Ƶ�δ���λ��

	//
	switch( i_LTE_Band_Name )
	{
	case 1://L_BAND_1
		*ip_L_Band_Index = L_BAND_1;
		break;
	case 2://L_BAND_2
		*ip_L_Band_Index = L_BAND_2;
		break;
	case 3://L_BAND_3
		*ip_L_Band_Index = L_BAND_3;
		break;
	case 4://L_BAND_4
		*ip_L_Band_Index = L_BAND_4;
		break;
	case 5://L_BAND_5
		*ip_L_Band_Index = L_BAND_5;
		break;
	case 7://L_BAND_7
		*ip_L_Band_Index = L_BAND_7;
		break;
	case 8://L_BAND_8
		*ip_L_Band_Index = L_BAND_8;
		break;
	case 9://L_BAND_9
		*ip_L_Band_Index = L_BAND_9;
		break;
	case 11://L_BAND_11
		*ip_L_Band_Index = L_BAND_11;
		break;
	case 12://L_BAND_12
		*ip_L_Band_Index = L_BAND_12;
		break;
	case 13://L_BAND_13
		*ip_L_Band_Index = L_BAND_13;
		break;
	case 14://L_BAND_14
		*ip_L_Band_Index = L_BAND_14;
		break;
	case 17://L_BAND_17
		*ip_L_Band_Index = L_BAND_17;
		break;
	case 18://L_BAND_18
		*ip_L_Band_Index = L_BAND_18;
		break;
	case 19://L_BAND_19
		*ip_L_Band_Index = L_BAND_19;
		break;
	case 20://L_BAND_20
		*ip_L_Band_Index = L_BAND_20;
		break;
	case 21://L_BAND_21
		*ip_L_Band_Index = L_BAND_21;
		break;
	case 25://L_BAND_25
		*ip_L_Band_Index = L_BAND_25;
		break;
	case 26://L_BAND_26
		*ip_L_Band_Index = L_BAND_26;
		break;
	case 27://L_BAND_27
		*ip_L_Band_Index = L_BAND_27;
		break;
	case 28://L_BAND_28
		*ip_L_Band_Index = L_BAND_28;
		break;
	case 29://L_BAND_29
		*ip_L_Band_Index = L_BAND_29;
		break;
	case 30://L_BAND_30
		*ip_L_Band_Index = L_BAND_30;
		break;
	case 31://L_BAND_31
		*ip_L_Band_Index = L_BAND_31;
		break;
	case 32://L_BAND_32
		*ip_L_Band_Index = L_BAND_32;
		break;
	case 34://L_BAND_34
		*ip_L_Band_Index = L_BAND_34;
		break;
	case 38://L_BAND_38
		*ip_L_Band_Index = L_BAND_38;
		break;
	case 39://L_BAND_39
		*ip_L_Band_Index = L_BAND_39;
		break;
	case 40://L_BAND_40
		*ip_L_Band_Index = L_BAND_40;
		break;
	case 41://L_BAND_41
		*ip_L_Band_Index = L_BAND_41;
		break;
	case 42://L_BAND_42
		*ip_L_Band_Index = L_BAND_42;
		break;
	case 43://L_BAND_43
		*ip_L_Band_Index = L_BAND_43;
		break;
	case 46://L_BAND_46
		*ip_L_Band_Index = L_BAND_46;
		break;
	case 47://L_BAND_47
		*ip_L_Band_Index = L_BAND_47;
		break;
	case 48://L_BAND_48
		*ip_L_Band_Index = L_BAND_48;
		break;
	case 66://L_BAND_66
		*ip_L_Band_Index = L_BAND_66;
		break;
	case 71://L_BAND_71
		*ip_L_Band_Index = L_BAND_71;
		break;
	case 72://L_BAND_72
		*ip_L_Band_Index = L_BAND_72;
		break;
	case 73://L_BAND_73
		*ip_L_Band_Index = L_BAND_73;
		break;
	case 85://L_BAND_85
		*ip_L_Band_Index = L_BAND_85;
		break;
	case 87://L_BAND_87
		*ip_L_Band_Index = L_BAND_87;
		break;
	case 88://L_BAND_88
		*ip_L_Band_Index = L_BAND_88;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_LTE_Band_Index, LTE_Band_Name(%d) is wrong!" ), i_LTE_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_LTE_Band_Name

	//pass
	return true;
}

int CGFC::Trans_UL_ARFCN_LTE( int i_L_Band_Name, int i_Arfcn_DL )
{
	//LTE�������ŵ���ת�����ŵ���
	//LTE�¼�Ƶ�δ���λ��

	int iTx_Arfcn;

	//
	switch( i_L_Band_Name )
	{
		//FDD 1~28
	case 1://L_BAND_1
	case 2://L_BAND_2
	case 3://L_BAND_3
	case 4://L_BAND_4
	case 5://L_BAND_5
	case 7://L_BAND_7
	case 8://L_BAND_8
	case 9://L_BAND_9
	case 11://L_BAND_11
	case 12://L_BAND_12
	case 13://L_BAND_13
	case 14://L_BAND_14
	case 17://L_BAND_17
	case 18://L_BAND_18
	case 19://L_BAND_19
	case 20://L_BAND_20
	case 21://L_BAND_21
	case 25://L_BAND_25
	case 26://L_BAND_26
	case 28://L_BAND_28
	case 27://L_BAND_27
		iTx_Arfcn = i_Arfcn_DL + 18000;
		break;

		//TDD 33~51
	case 34://L_BAND_34
	case 38://L_BAND_38		
	case 39://L_BAND_39
	case 40://L_BAND_40
	case 41://L_BAND_41
	case 42://L_BAND_42
	case 43://L_BAND_43
	case 48://L_BAND_48
	case 47://L_BAND_47
	case 46://L_BAND_46
		iTx_Arfcn = i_Arfcn_DL;
		break;

		//LTE��B29��B32��ֻ��DL����FDD��ֻ����CA������
	case 29://L_BAND_29
	case 32://L_BAND_32
		iTx_Arfcn = i_Arfcn_DL;
		break;

	case 31://L_BAND_31
		iTx_Arfcn = i_Arfcn_DL + 17890;
		break;

		//FDD 30~31
	case 30://L_BAND_30
		iTx_Arfcn = i_Arfcn_DL + 17890;
		break;

	case 66://L_BAND_66
		//ע�⣺Band66��DL��2180-2200 MHz������CAʱ���õ�
		//The range 2180-2200 MHz of the DL operating band is restricted to E-UTRA operation when carrier aggregation is configured.
		iTx_Arfcn = i_Arfcn_DL + 65536;//66Ƶ���ŵ������п�ʼ�ŵ��Ĳ�ֵ����(131972-66436)
		break;

	case 71://L_BAND_71
		iTx_Arfcn = i_Arfcn_DL + 64536;
		break;

	case 72://L_BAND_72
		iTx_Arfcn = i_Arfcn_DL + 64536;
		break;

	case 73://L_BAND_73
		iTx_Arfcn = i_Arfcn_DL + 64536;
		break;

	case 85://L_BAND_85
		iTx_Arfcn = i_Arfcn_DL + 63636;
		break;

	case 86://L_BAND_86, NB_BAND_86
		iTx_Arfcn = i_Arfcn_DL + 63636;
		break;

	case 87://L_BAND_87, NB_BAND_87
		iTx_Arfcn = i_Arfcn_DL + 63636;
		break;

	case 88://L_BAND_88, NB_BAND_88
		iTx_Arfcn = i_Arfcn_DL + 63635;
		break;

	default:
		this->m_csError_Info.Format( _T( "ERROR, Trans_UL_ARFCN_LTE, LTE_Band_Name(%d) is wrong!" ), i_L_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		iTx_Arfcn = 0;
		//
		break;
	}//switch( i_L_Band_Name


	//����
	return iTx_Arfcn;
}

int CGFC::Trans_UL_ARFCN_WCDMA( int i_W_Band_Index, int i_Arfcn_DL )
{
	//��WCDMA�������ŵ�ת�����ŵ�
	//WCDMA�¼�Ƶ�δ���λ��

	int iTx_Arfcn;

	//WCDMAƵ�ε�index
	switch( i_W_Band_Index )
	{
	case W_BAND_1:
		iTx_Arfcn = i_Arfcn_DL - 950;
		break;
	case W_BAND_2:
		iTx_Arfcn = i_Arfcn_DL - 400;
		break;
	case W_BAND_3:
		iTx_Arfcn = i_Arfcn_DL - 225;
		break;
	case W_BAND_4:
		iTx_Arfcn = i_Arfcn_DL - 225;
		break;
	case W_BAND_5:
		iTx_Arfcn = i_Arfcn_DL - 225;
		break;
	case W_BAND_6:
		iTx_Arfcn = i_Arfcn_DL - 225;
		break;
	case W_BAND_8:
		iTx_Arfcn = i_Arfcn_DL - 225;
		break;
	case W_BAND_9:
		iTx_Arfcn = i_Arfcn_DL - 475;
		break;
	case W_BAND_19:
		iTx_Arfcn = i_Arfcn_DL - 400;
		break;
	default:
		iTx_Arfcn = 0;
		break;
	}

	//����
	return iTx_Arfcn;
}

bool CGFC::Calcu_Center_Freq_TD( int i_TD_Band_Name, int i_ARFCN, double * dp_Center_Freq_MHz )
{
	//�� TD �ŵ��ż����ȡƵ�㣨MHz��

	try
	{
		switch( i_TD_Band_Name )
		{
		case 34://AƵ�Σ�2010��2025 MHz��ԭBƵ�Σ�������15MHz
			{
				*dp_Center_Freq_MHz = i_ARFCN / 5.0;
			}
			break;
		case 39:
			{
				*dp_Center_Freq_MHz = i_ARFCN / 5.0;
			}
			break;
		default:
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "Calcu_Center_Freq_TD( TD_Band = %d, ARFCN = %d ), Fail !" ), i_TD_Band_Name, i_ARFCN );
		return false;
	}

	return true;
}

bool CGFC::Calcu_ARFCN_TD( int i_TD_Band, double d_Center_Freq_MHz, int * ip_ARFCN )
{
	//�� TD Ƶ�㣨MHz�������ȡ�ŵ���

	try
	{
		switch( i_TD_Band )
		{
		case 34://AƵ�Σ�2010��2025 MHz��ԭBƵ�Σ�������15MHz
			{
				*ip_ARFCN = d_Center_Freq_MHz * 5;
			}
			break;
		default:
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "Calcu_ARFCN_TD( TD_Band = %d, Center_Freq = %.4f ), Fail !" ), i_TD_Band, d_Center_Freq_MHz );
		return false;
	}

	return true;
}

bool CGFC::Get_TD_Band_Name( int i_TD_Band_Index, int * ip_TD_Band_Name )
{
	switch( i_TD_Band_Index )
	{
	case TD_BAND_34:
		*ip_TD_Band_Name = 34;
		break;
	case TD_BAND_39:
		*ip_TD_Band_Name = 39;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_TD_Band_Name, TD_Band_Index(%d) is wrong!" ), i_TD_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_TD_Band_Index( int i_TD_Band_Name, int * ip_TD_Band_Index )
{
	switch( i_TD_Band_Name )
	{
	case 34:
		*ip_TD_Band_Index = TD_BAND_34;
		break;
	case 39:
		*ip_TD_Band_Index = TD_BAND_39;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_TD_Band_Index, TD_Band_Name(%d) is wrong!" ), i_TD_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		return false;
	}

	return true;
}

bool CGFC::Encode_RF_Ref_Tx_Data_To_Str_V1( double d_Code_Key, int i_Band_And_Tx_Data_Quan, int * ip_Band_List, double * dp_RF_Tx_Data, CString * csp_Str_Data )
{
	//����Ƶ�κ�Tx��������ת�ɡ��ַ����������ڽ�����ݵ�д�롣
	//ÿ��Ƶ��ֻ��һ������

	CString csAll, csBand, csCh;
	int i, iCode;

	//���
	csAll.Format( _T( "" ) );

	//Ƶ��ѭ��
	for( i = 0; i < i_Band_And_Tx_Data_Quan; i++ )
	{
		//Ƶ����Ϣ�����롱
		if( (*(ip_Band_List + i) >= 0) && (*(ip_Band_List + i) <= 25) )
		{
			//a~z
			csBand.Format( _T( "%c" ), (*(ip_Band_List + i) + 'a') );
		}
		else if( (*(ip_Band_List + i) >= 26) && (*(ip_Band_List + i) <= 51) )
		{
			//A~Z
			csBand.Format( _T( "%c" ), (*(ip_Band_List + i) - 26 + 'A') );
		}
		else
		{
			this->m_csError_Info.Format( _T( "ERROR, Ƶ��index�����쳣����ֹ�����!" ) );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			return false;
		}


		//С������һλ����һλ��С�����ڶ�λ�������룻
		iCode =(int) ((*(dp_RF_Tx_Data + i) + 0.05 - d_Code_Key) * 10.0);

		//
		if( iCode < 0 )
		{
			this->m_csError_Info.Format( _T( "ERROR, Ref����ƫС�����ʺ������!" ) );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			return false;
		}

		//
		if( iCode > 99 )
		{
			this->m_csError_Info.Format( _T( "ERROR, Ref����ƫ�󣬲��ʺ������!" ) );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			return false;
		}


		csCh.Format( _T( "%02d" ), iCode );

		csBand += csCh;

		csAll += csBand;
	}//for(i=0; i<i_Data_And_Band_Quan; i++


	//���ص��ִ�
	csp_Str_Data->Format( _T( "%s" ), csAll );


	//pass
	return true;
}

bool CGFC::Decode_RF_Ref_Tx_Data_From_Str_V1( const wchar_t * wcp_Ref_Str_Data, double d_Code_Key, int * ip_Band_And_Tx_Data_Quan, int * ip_Band_List, double * dp_RF_Tx_Data )
{
	//�ӡ�����ַ���������ȡ����Ƶ�ζ�Ӧ�Ĺ������ݡ�
	//ÿ��Ƶ��ֻ��һ������

	int iLen, iBand_Quan, i;
	CString csData;

	//
	try
	{
		iLen = wcslen( wcp_Ref_Str_Data );

		//����Ƿ��ܱ�3������ÿ��Ƶ������������3����1+2��
		if( (iLen % 3) != 0 )
		{
			::AfxMessageBox( _T( "ERROR, ������ݳ��ȴ���!" ) );
			throw(0);
		}

		//Ƶ����
		iBand_Quan = iLen / 3;

		//
		for( i = 0; i < iBand_Quan; i++ )
		{
			//���Ƶ�α�ʶ�����ָ�����
			if( (*(wcp_Ref_Str_Data + i * 3) >= 'a') && (*(wcp_Ref_Str_Data + i * 3) <= 'z') )
			{
				//a~z
				//Ƶ�κŵ�index
				*(ip_Band_List + i) = *(wcp_Ref_Str_Data + i * 3) - 'a';
			}
			else if( (*(wcp_Ref_Str_Data + i * 3) >= 'A') && (*(wcp_Ref_Str_Data + i * 3) <= 'Z') )
			{
				//A~Z
				//Ƶ�κŵ�index
				*(ip_Band_List + i) = *(wcp_Ref_Str_Data + i * 3) - 'A' + 26;
			}
			else
			{
				::AfxMessageBox( _T( "ERROR, ���Ƶ�α�ʶ�����ݴ���!" ) );
				throw(0);//Ƶ�α�ʶ������
			}


			//������Ϣ
			csData.Format( _T( "%c%c" ), *(wcp_Ref_Str_Data + i * 3 + 1), *(wcp_Ref_Str_Data + i * 3 + 2) );
			*(dp_RF_Tx_Data + i) = _wtof( csData ) / 10.0 + d_Code_Key;
		}//for( i = 0; i < iBand_Quan; i++


		//��������
		*ip_Band_And_Tx_Data_Quan = iBand_Quan;
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

void CGFC::WCSTOK_STRING_T_C( const wchar_t * wcp_StrToken, const wchar_t * wcp_StrDelimit, CString * csp_Get_CString_Array, int * ip_Get_Quan )
{
	//�ԡ��ر�ָ���ķָ�����һ������ȡ��ȫ���Ӵ���

	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//��Դ���ݡ���buff��wcstok����ȡ����ʱ��ı�Դ�������ݣ������Ȱ�Դ���ݷ���buff������buff���ݣ����˺�����Դ��
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//���ڴ�š�Դ���ݡ���buff����
	WCHAR * wcpResult = NULL;


	//�����ڴ�
	this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

	//���롰Դ���ݡ�
	swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

	//��ȡ��һ��ֵ(0)
	wcpEach = wcstok_s( wcpSrc, wcp_StrDelimit, &wcpResult );

	while( wcpEach != NULL )
	{
		//��ֵ
		(csp_Get_CString_Array + i)->Format( _T( "%s" ), wcpEach );

		//��ȡ��һ��ֵ
		wcpEach = wcstok_s( NULL, wcp_StrDelimit, &wcpResult );

		//����
		i++;
	}//while(wcpEach != NULL

	//��������
	*ip_Get_Quan = i;


	//�ͷ��ڴ�
	this->BUFF_WCHAR_FREE( &wcpSrc );
}

int CGFC::Calcu_IMEI_Checksum( const wchar_t * wcp_IMEI_14_Bits )
{
	int iSum = 0;
	int irInt_Buff[16], i;
	wchar_t wcrTemp[2];

	memset( wcrTemp, 0, sizeof( wcrTemp ) );
	memset( irInt_Buff, 0, sizeof( irInt_Buff ) );

	for( i = 0; i < 14; i++ )
	{
		wcrTemp[0] = *(wcp_IMEI_14_Bits + i);
		irInt_Buff[i] = _wtoi( wcrTemp );
	}

	for( i = 0; i < 7; i++ )
	{
		iSum += irInt_Buff[2 * i + 1] * 2 / 10;
		iSum += irInt_Buff[2 * i + 1] * 2 % 10;
		iSum += irInt_Buff[2 * i];
	}

	iSum = iSum % 10;

	if( iSum != 0 )
	{
		iSum = 10 - iSum;
	}

	//����ֵ
	return iSum;
}

bool CGFC::Get_CDMA2000_Band_Name( int i_C2K_Band_Index, int * ip_C2K_Band_Name )
{
	switch( i_C2K_Band_Index )
	{
	case C2K_BC_0://band class 0
		*ip_C2K_Band_Name = 0;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_CDMA2000_Band_Name, cdma2000_Band_Index(%d) is wrong!" ), i_C2K_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_CDMA2000_Band_Index( int i_C2K_Band_Name, int * ip_C2K_Band_Index )
{
	switch( i_C2K_Band_Name )
	{
	case 0://band class 0
		*ip_C2K_Band_Index = C2K_BC_0;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_CDMA2000_Band_Index, cdma2000_Band_Name(%d) is wrong!" ), i_C2K_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		return false;
	}

	//pass
	return true;
}

bool CGFC::Calcu_Center_Freq_CDMA2000( int i_C2K_Band_Name, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq_MHz )
{
	try
	{
		switch( i_C2K_Band_Name )
		{
		case 0://band class 0
			{
				switch( i_ARFCN )
				{
				case 1013:
					{
						if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
						{
							*dp_Center_Freq_MHz = 824.7;
						}
						else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
						{
							*dp_Center_Freq_MHz = 869.7;
						}
						else
						{
							throw(0);
						}
					}
					break;
				case 384:
					{
						if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
						{
							*dp_Center_Freq_MHz = 836.52;
						}
						else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
						{
							*dp_Center_Freq_MHz = 881.52;
						}
						else
						{
							throw(0);
						}
					}
					break;
				case 777:
					{
						if( i_Up_Down_Link == RF_TX_UL_INPUT )//����
						{
							*dp_Center_Freq_MHz = 848.31;
						}
						else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//����
						{
							*dp_Center_Freq_MHz = 893.31;
						}
						else
						{
							throw(0);
						}
					}
					break;
				default:
					throw(0);
				}
			}
			break;
		default:
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "ERROR, Calcu_Center_Freq_CDMA2000( C2K_Band_Name = %d, ARFCN = %d, Up_Down_Link = %d )" ), i_C2K_Band_Name, i_ARFCN, i_Up_Down_Link );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_WCDMA_Band_Name( int i_W_Band_Index, int * ip_W_Band_Name )
{
	//WCDMAƵ��index�� ת�� WCDMAƵ����
	//WCDMA�¼�Ƶ�δ���λ��

	switch( i_W_Band_Index )//WCDMAƵ��index��
	{
	case W_BAND_1:
		*ip_W_Band_Name = 1;
		break;
	case W_BAND_2:
		*ip_W_Band_Name = 2;
		break;
	case W_BAND_3:
		*ip_W_Band_Name = 3;
		break;
	case W_BAND_4:
		*ip_W_Band_Name = 4;
		break;
	case W_BAND_5:
		*ip_W_Band_Name = 5;
		break;
	case W_BAND_6:
		*ip_W_Band_Name = 6;
		break;
	case W_BAND_7:
		*ip_W_Band_Name = 7;
		break;
	case W_BAND_8:
		*ip_W_Band_Name = 8;
		break;
	case W_BAND_9:
		*ip_W_Band_Name = 9;
		break;
	case W_BAND_10:
		*ip_W_Band_Name = 10;
		break;
	case W_BAND_19:
		*ip_W_Band_Name = 19;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_WCDMA_Band_Name, WCDMA_Band_Index(%d) is wrong!" ), i_W_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_WCDMA_Band_Index( int i_W_Band_Name, int * ip_W_Band_Index )
{
	//WCDMAƵ���� ת�� WCDMAƵ��index��
	//WCDMA�¼�Ƶ�δ���λ��

	switch( i_W_Band_Name )
	{
	case 1://W_BAND_1
		*ip_W_Band_Index = W_BAND_1;
		break;
	case 2://W_BAND_2
		*ip_W_Band_Index = W_BAND_2;
		break;
	case 3://W_BAND_3
		*ip_W_Band_Index = W_BAND_3;
		break;
	case 4://W_BAND_4
		*ip_W_Band_Index = W_BAND_4;
		break;
	case 5://W_BAND_5
		*ip_W_Band_Index = W_BAND_5;
		break;
	case 6://W_BAND_6
		*ip_W_Band_Index = W_BAND_6;
		break;
	case 7://W_BAND_7
		*ip_W_Band_Index = W_BAND_7;
		break;
	case 8://W_BAND_8
		*ip_W_Band_Index = W_BAND_8;
		break;
	case 9://W_BAND_9
		*ip_W_Band_Index = W_BAND_9;
		break;
	case 10://W_BAND_10
		*ip_W_Band_Index = W_BAND_10;
		break;
	case 19://W_BAND_19
		*ip_W_Band_Index = W_BAND_19;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_WCDMA_Band_Index, WCDMA_Band_Name(%d) is wrong!" ), i_W_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

void CGFC::Get_RF_Offset_Freq_Band_Seg( int i_Format, int i_Band_Index, double * dp_Freq_Tx_S, double * dp_Freq_Tx_E, double * dp_Freq_Rx_S, double * dp_Freq_Rx_E )
{
	//���𲹳������õ���ʼƵ��
	//WCDMA�¼�Ƶ�δ���λ��
	//LTE�¼�Ƶ�δ���λ��

	//
	try
	{
		switch( i_Format )
		{
		case FORMAT_GSM_S:
		case FORMAT_GSM_NS:
			{
				switch( i_Band_Index )//GSM Band
				{
				case G_BAND_GSM900:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 925.0;
					*dp_Freq_Rx_E = 960.0;
					break;
				case G_BAND_DCS1800:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1785.0;
					*dp_Freq_Rx_S = 1805.0;
					*dp_Freq_Rx_E = 1880.0;
					break;
				case G_BAND_PCS1900:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1910.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1990.0;
					break;
				case G_BAND_GSM850:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_WCDMA_S:
		case FORMAT_WCDMA_NS:
			{
				switch( i_Band_Index )//WCDMA Band
				{
				case W_BAND_1:
					*dp_Freq_Tx_S = 1920.0;
					*dp_Freq_Tx_E = 1980.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2170.0;
					break;
				case W_BAND_2:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1910.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1990.0;
					break;
				case W_BAND_3:
					*dp_Freq_Tx_S = 1712.4;
					*dp_Freq_Tx_E = 1782.6;
					*dp_Freq_Rx_S = 1807.4;
					*dp_Freq_Rx_E = 1877.6;
					break;
				case W_BAND_4:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1755.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2155.0;
					break;
				case W_BAND_5:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case W_BAND_6:
					*dp_Freq_Tx_S = 832.4;
					*dp_Freq_Tx_E = 837.6;
					*dp_Freq_Rx_S = 877.4;
					*dp_Freq_Rx_E = 882.6;
					break;
				case W_BAND_8:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 925.0;
					*dp_Freq_Rx_E = 960.0;
					break;
				case W_BAND_9:
					*dp_Freq_Tx_S = 1752.4;
					*dp_Freq_Tx_E = 1782.4;
					*dp_Freq_Rx_S = 1847.4;
					*dp_Freq_Rx_E = 1877.4;
					break;
				case W_BAND_19:
					*dp_Freq_Tx_S = 832.4;
					*dp_Freq_Tx_E = 842.6;
					*dp_Freq_Rx_S = 877.4;
					*dp_Freq_Rx_E = 887.6;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_TD_NS:
			{
				switch( i_Band_Index )//TD Band
				{
				case TD_BAND_34:
					*dp_Freq_Tx_S = 2000.0;
					*dp_Freq_Tx_E = 2030.0;
					*dp_Freq_Rx_S = 2000.0;
					*dp_Freq_Rx_E = 2030.0;
					break;
				case TD_BAND_39:
					*dp_Freq_Tx_S = 1875.0;
					*dp_Freq_Tx_E = 1925.0;
					*dp_Freq_Rx_S = 1875.0;
					*dp_Freq_Rx_E = 1925.0;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_C2K_S:
		case FORMAT_C2K_NS:
			{
				switch( i_Band_Index )//cdma2000 Band
				{
				case C2K_BC_0:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				default:
					//fail
					throw(0);
				}
			}
			//
			break;

		case FORMAT_LTE_NS:
			{
				switch( i_Band_Index )//LTE Band
				{
				case L_BAND_1:
					*dp_Freq_Tx_S = 1920.0;
					*dp_Freq_Tx_E = 1980.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2170.0;
					break;
				case L_BAND_2:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1910.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1990.0;
					break;
				case L_BAND_3:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1785.0;
					*dp_Freq_Rx_S = 1805.0;
					*dp_Freq_Rx_E = 1880.0;
					break;
				case L_BAND_4:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1755.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2155.0;
					break;
				case L_BAND_5:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case L_BAND_7:
					*dp_Freq_Tx_S = 2500.0;
					*dp_Freq_Tx_E = 2571.0;
					*dp_Freq_Rx_S = 2620.0;
					*dp_Freq_Rx_E = 2691.0;
					break;
				case L_BAND_8:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 925.0;
					*dp_Freq_Rx_E = 960.0;
					break;
				case L_BAND_12:
					*dp_Freq_Tx_S = 698.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 728.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case L_BAND_17:
					*dp_Freq_Tx_S = 704.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 734.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case L_BAND_20:
					*dp_Freq_Tx_S = 832.0;
					*dp_Freq_Tx_E = 862.0;
					*dp_Freq_Rx_S = 791.0;
					*dp_Freq_Rx_E = 821.0;
					break;
				case L_BAND_38:
					*dp_Freq_Tx_S = 2570.0;
					*dp_Freq_Tx_E = 2620.0;
					*dp_Freq_Rx_S = 2570.0;
					*dp_Freq_Rx_E = 2620.0;
					break;
				case L_BAND_39:
					*dp_Freq_Tx_S = 1880.0;
					*dp_Freq_Tx_E = 1920.0;
					*dp_Freq_Rx_S = 1880.0;
					*dp_Freq_Rx_E = 1920.0;
					break;
				case L_BAND_40:
					*dp_Freq_Tx_S = 2300.0;
					*dp_Freq_Tx_E = 2400.0;
					*dp_Freq_Rx_S = 2300.0;
					*dp_Freq_Rx_E = 2400.0;
					break;
				case L_BAND_41:
					*dp_Freq_Tx_S = 2496.0;
					*dp_Freq_Tx_E = 2690.0;
					*dp_Freq_Rx_S = 2496.0;
					*dp_Freq_Rx_E = 2690.0;
					break;
				case L_BAND_13:
					*dp_Freq_Tx_S = 777.0;
					*dp_Freq_Tx_E = 787.0;
					*dp_Freq_Rx_S = 746.0;
					*dp_Freq_Rx_E = 756.0;
					break;
				case L_BAND_18:
					*dp_Freq_Tx_S = 815.0;
					*dp_Freq_Tx_E = 830.0;
					*dp_Freq_Rx_S = 860.0;
					*dp_Freq_Rx_E = 875.0;
					break;
				case L_BAND_19:
					*dp_Freq_Tx_S = 830.0;
					*dp_Freq_Tx_E = 845.0;
					*dp_Freq_Rx_S = 875.0;
					*dp_Freq_Rx_E = 890.0;
					break;
				case L_BAND_25:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1915.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1995.0;
					break;
				case L_BAND_26:
					*dp_Freq_Tx_S = 814.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 859.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case L_BAND_28:
					*dp_Freq_Tx_S = 703.0;
					*dp_Freq_Tx_E = 748.0;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 803.0;
					break;
				case L_BAND_34:
					*dp_Freq_Tx_S = 2010.0;
					*dp_Freq_Tx_E = 2025.0;
					*dp_Freq_Rx_S = 2010.0;
					*dp_Freq_Rx_E = 2025.0;
					break;
				case L_BAND_14:
					*dp_Freq_Tx_S = 788.0;
					*dp_Freq_Tx_E = 797.9;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 767.9;
					break;
				case L_BAND_29:
					*dp_Freq_Tx_S = 717.0;
					*dp_Freq_Tx_E = 727.9;
					*dp_Freq_Rx_S = 717.0;
					*dp_Freq_Rx_E = 727.9;
					break;
				case L_BAND_30:
					*dp_Freq_Tx_S = 2305.0;
					*dp_Freq_Tx_E = 2314.9;
					*dp_Freq_Rx_S = 2350.0;
					*dp_Freq_Rx_E = 2359.9;
					break;
				case L_BAND_32:
					*dp_Freq_Tx_S = 1452.0;
					*dp_Freq_Tx_E = 1495.9;
					*dp_Freq_Rx_S = 1452.0;
					*dp_Freq_Rx_E = 1495.9;
					break;
				case L_BAND_66:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1779.9;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2199.9;
					break;
				case L_BAND_71:
					*dp_Freq_Tx_S = 663.0;
					*dp_Freq_Tx_E = 697.9;
					*dp_Freq_Rx_S = 617.0;
					*dp_Freq_Rx_E = 651.9;
					break;
				case L_BAND_42:
					*dp_Freq_Tx_S = 3400.0;
					*dp_Freq_Tx_E = 3599.9;
					*dp_Freq_Rx_S = 3400.0;
					*dp_Freq_Rx_E = 3599.9;
					break;
				case L_BAND_43:
					*dp_Freq_Tx_S = 3600.0;
					*dp_Freq_Tx_E = 3799.9;
					*dp_Freq_Rx_S = 3600.0;
					*dp_Freq_Rx_E = 3799.9;
					break;
				case L_BAND_48:
					*dp_Freq_Tx_S = 3550.0;
					*dp_Freq_Tx_E = 3699.9;
					*dp_Freq_Rx_S = 3550.0;
					*dp_Freq_Rx_E = 3699.9;
					break;
				case L_BAND_9:
					*dp_Freq_Tx_S = 1749.9;
					*dp_Freq_Tx_E = 1784.8;
					*dp_Freq_Rx_S = 1844.9;
					*dp_Freq_Rx_E = 1879.8;
					break;
				case L_BAND_11:
					*dp_Freq_Tx_S = 1427.9;
					*dp_Freq_Tx_E = 1447.8;
					*dp_Freq_Rx_S = 1475.9;
					*dp_Freq_Rx_E = 1495.8;
					break;
				case L_BAND_21:
					*dp_Freq_Tx_S = 1447.9;
					*dp_Freq_Tx_E = 1462.8;
					*dp_Freq_Rx_S = 1495.9;
					*dp_Freq_Rx_E = 1510.8;
					break;
				case L_BAND_47:
					*dp_Freq_Tx_S = 5855.0;
					*dp_Freq_Tx_E = 5924.9;
					*dp_Freq_Rx_S = 5855.0;
					*dp_Freq_Rx_E = 5924.9;
					break;
				case L_BAND_46:
					*dp_Freq_Tx_S = 5150.0;
					*dp_Freq_Tx_E = 5925.0;
					*dp_Freq_Rx_S = 5150.0;
					*dp_Freq_Rx_E = 5925.0;
					break;
				case L_BAND_27:
					*dp_Freq_Tx_S = 807.0;
					*dp_Freq_Tx_E = 824.0;
					*dp_Freq_Rx_S = 852.0;
					*dp_Freq_Rx_E = 869.0;
					break;
				case L_BAND_31:
					*dp_Freq_Tx_S = 452.5;
					*dp_Freq_Tx_E = 457.4;
					*dp_Freq_Rx_S = 462.5;
					*dp_Freq_Rx_E = 467.4;
					break;
				case L_BAND_72:
					*dp_Freq_Tx_S = 451.0;
					*dp_Freq_Tx_E = 456.0;
					*dp_Freq_Rx_S = 461.0;
					*dp_Freq_Rx_E = 466.0;
					break;
				case L_BAND_73:
					*dp_Freq_Tx_S = 450.0;
					*dp_Freq_Tx_E = 455.0;
					*dp_Freq_Rx_S = 460.0;
					*dp_Freq_Rx_E = 465.0;
					break;
				case L_BAND_85:
					*dp_Freq_Tx_S = 698.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 728.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case L_BAND_87:
					*dp_Freq_Tx_S = 410.0;
					*dp_Freq_Tx_E = 414.9;
					*dp_Freq_Rx_S = 420.0;
					*dp_Freq_Rx_E = 424.9;
					break;
				case L_BAND_88:
					*dp_Freq_Tx_S = 412.0;
					*dp_Freq_Tx_E = 416.9;
					*dp_Freq_Rx_S = 422.0;
					*dp_Freq_Rx_E = 426.9;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_CV2X_NS://C-V2X
			{
				switch( i_Band_Index )//C-V2X Band
				{
				case CV2X_BAND_46:
					*dp_Freq_Tx_S = 5150.0;
					*dp_Freq_Tx_E = 5925.0;
					*dp_Freq_Rx_S = 5150.0;
					*dp_Freq_Rx_E = 5925.0;
					break;
				case CV2X_BAND_47:
					*dp_Freq_Tx_S = 5855.0;
					*dp_Freq_Tx_E = 5924.9;
					*dp_Freq_Rx_S = 5855.0;
					*dp_Freq_Rx_E = 5924.9;
					break;
				default:
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_NBIOT_S:
		case FORMAT_NBIOT_NS:
			{
				switch( i_Band_Index )//NB-IoT Band
				{
				case NB_IDX_B_1:
					*dp_Freq_Tx_S = 1920.0;
					*dp_Freq_Tx_E = 1980.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2170.0;
					break;
				case NB_IDX_B_2:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1910.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1990.0;
					break;
				case NB_IDX_B_3:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1785.0;
					*dp_Freq_Rx_S = 1805.0;
					*dp_Freq_Rx_E = 1880.0;
					break;
				case NB_IDX_B_4:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1755.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2155.0;
					break;
				case NB_IDX_B_5:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case NB_IDX_B_7:
					*dp_Freq_Tx_S = 2500.0;
					*dp_Freq_Tx_E = 2571.0;
					*dp_Freq_Rx_S = 2620.0;
					*dp_Freq_Rx_E = 2691.0;
					break;
				case NB_IDX_B_8:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 925.0;
					*dp_Freq_Rx_E = 960.0;
					break;
				case NB_IDX_B_9:
					*dp_Freq_Tx_S = 1749.9;
					*dp_Freq_Tx_E = 1784.8;
					*dp_Freq_Rx_S = 1844.9;
					*dp_Freq_Rx_E = 1879.8;
					break;
				case NB_IDX_B_10:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1770.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2170.0;
					break;
				case NB_IDX_B_11:
					*dp_Freq_Tx_S = 1427.9;
					*dp_Freq_Tx_E = 1447.8;
					*dp_Freq_Rx_S = 1475.9;
					*dp_Freq_Rx_E = 1495.8;
					break;
				case NB_IDX_B_12:
					*dp_Freq_Tx_S = 698.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 728.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case NB_IDX_B_13:
					*dp_Freq_Tx_S = 777.0;
					*dp_Freq_Tx_E = 787.0;
					*dp_Freq_Rx_S = 746.0;
					*dp_Freq_Rx_E = 756.0;
					break;
				case NB_IDX_B_14:
					*dp_Freq_Tx_S = 788.0;
					*dp_Freq_Tx_E = 797.9;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 767.9;
					break;
				case NB_IDX_B_17:
					*dp_Freq_Tx_S = 704.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 734.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case NB_IDX_B_18:
					*dp_Freq_Tx_S = 815.0;
					*dp_Freq_Tx_E = 830.0;
					*dp_Freq_Rx_S = 860.0;
					*dp_Freq_Rx_E = 875.0;
					break;
				case NB_IDX_B_19:
					*dp_Freq_Tx_S = 830.0;
					*dp_Freq_Tx_E = 845.0;
					*dp_Freq_Rx_S = 875.0;
					*dp_Freq_Rx_E = 890.0;
					break;
				case NB_IDX_B_20:
					*dp_Freq_Tx_S = 832.0;
					*dp_Freq_Tx_E = 862.0;
					*dp_Freq_Rx_S = 791.0;
					*dp_Freq_Rx_E = 821.0;
					break;
				case NB_IDX_B_21:
					*dp_Freq_Tx_S = 1447.9;
					*dp_Freq_Tx_E = 1462.8;
					*dp_Freq_Rx_S = 1495.9;
					*dp_Freq_Rx_E = 1510.8;
					break;
				case NB_IDX_B_22:
					*dp_Freq_Tx_S = 3410.0;
					*dp_Freq_Tx_E = 3490.0;
					*dp_Freq_Rx_S = 3510.0;
					*dp_Freq_Rx_E = 3590.0;
					break;
				case NB_IDX_B_24:
					*dp_Freq_Tx_S = 1626.5;
					*dp_Freq_Tx_E = 1660.4;
					*dp_Freq_Rx_S = 1525.0;
					*dp_Freq_Rx_E = 1558.9;
					break;
				case NB_IDX_B_25:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1915.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1995.0;
					break;
				case NB_IDX_B_26:
					*dp_Freq_Tx_S = 814.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 859.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case NB_IDX_B_27:
					*dp_Freq_Tx_S = 807.0;
					*dp_Freq_Tx_E = 824.0;
					*dp_Freq_Rx_S = 852.0;
					*dp_Freq_Rx_E = 869.0;
					break;
				case NB_IDX_B_28:
					*dp_Freq_Tx_S = 703.0;
					*dp_Freq_Tx_E = 748.0;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 803.0;
					break;
				case NB_IDX_B_30:
					*dp_Freq_Tx_S = 2305.0;
					*dp_Freq_Tx_E = 2314.9;
					*dp_Freq_Rx_S = 2350.0;
					*dp_Freq_Rx_E = 2359.9;
					break;
				case NB_IDX_B_31:
					*dp_Freq_Tx_S = 452.5;
					*dp_Freq_Tx_E = 457.4;
					*dp_Freq_Rx_S = 462.5;
					*dp_Freq_Rx_E = 467.4;
					break;
				case NB_IDX_B_66:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1779.9;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2199.9;
					break;
				case NB_IDX_B_68:
					*dp_Freq_Tx_S = 698.0;
					*dp_Freq_Tx_E = 728.0;
					*dp_Freq_Rx_S = 753.0;
					*dp_Freq_Rx_E = 783.0;
					break;
				case NB_IDX_B_70:
					*dp_Freq_Tx_S = 1695.0;
					*dp_Freq_Tx_E = 1710.0;
					*dp_Freq_Rx_S = 1995.0;
					*dp_Freq_Rx_E = 2020.0;
					break;
				case NB_IDX_B_71:
					*dp_Freq_Tx_S = 663.0;
					*dp_Freq_Tx_E = 697.9;
					*dp_Freq_Rx_S = 617.0;
					*dp_Freq_Rx_E = 651.9;
					break;
				case NB_IDX_B_72:
					*dp_Freq_Tx_S = 451.0;
					*dp_Freq_Tx_E = 456.0;
					*dp_Freq_Rx_S = 461.0;
					*dp_Freq_Rx_E = 466.0;
					break;
				case NB_IDX_B_73:
					*dp_Freq_Tx_S = 450.0;
					*dp_Freq_Tx_E = 455.0;
					*dp_Freq_Rx_S = 460.0;
					*dp_Freq_Rx_E = 465.0;
					break;
				case NB_IDX_B_74:
					*dp_Freq_Tx_S = 1427.0;
					*dp_Freq_Tx_E = 1470.0;
					*dp_Freq_Rx_S = 1475.0;
					*dp_Freq_Rx_E = 1518.0;
					break;
				case NB_IDX_B_85:
					*dp_Freq_Tx_S = 698.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 728.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case NB_IDX_B_86:
					*dp_Freq_Tx_S = 787.0;
					*dp_Freq_Tx_E = 788.0;//Uplink Channel:134182-134191, FREQ:787.0-787.9MHz
					*dp_Freq_Rx_S = 757.0;
					*dp_Freq_Rx_E = 758.0;//Downlink Channel:70546-70555, FREQ:757.0-757.9MHz
					break;
				case NB_IDX_B_87:
					*dp_Freq_Tx_S = 410.0;
					*dp_Freq_Tx_E = 414.9;
					*dp_Freq_Rx_S = 420.0;
					*dp_Freq_Rx_E = 424.9;
					break;
				case NB_IDX_B_88:
					*dp_Freq_Tx_S = 412.0;
					*dp_Freq_Tx_E = 416.9;
					*dp_Freq_Rx_S = 422.0;
					*dp_Freq_Rx_E = 426.9;
					break;
				default:
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_NR5GSUB6_NS:
			{
				switch( i_Band_Index )
				{
				case NR5GSUB6_N_1:
					*dp_Freq_Tx_S = 1920.0;
					*dp_Freq_Tx_E = 1980.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2170.0;
					break;
				case NR5GSUB6_N_2:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1910.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1990.0;
					break;
				case NR5GSUB6_N_3:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1785.0;
					*dp_Freq_Rx_S = 1805.0;
					*dp_Freq_Rx_E = 1880.0;
					break;
				case NR5GSUB6_N_5:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case NR5GSUB6_N_7:
					*dp_Freq_Tx_S = 2500.0;
					*dp_Freq_Tx_E = 2570.0;
					*dp_Freq_Rx_S = 2620.0;
					*dp_Freq_Rx_E = 2690.0;
					break;
				case NR5GSUB6_N_8:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 925.0;
					*dp_Freq_Rx_E = 960.0;
					break;
				case NR5GSUB6_N_20:
					*dp_Freq_Tx_S = 832.0;
					*dp_Freq_Tx_E = 862.0;
					*dp_Freq_Rx_S = 791.0;
					*dp_Freq_Rx_E = 821.0;
					break;
				case NR5GSUB6_N_28:
					*dp_Freq_Tx_S = 703.0;
					*dp_Freq_Tx_E = 748.0;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 803.0;
					break;
				case NR5GSUB6_N_41:
					*dp_Freq_Tx_S = 2496.0;
					*dp_Freq_Tx_E = 2690.0;
					*dp_Freq_Rx_S = 2496.0;
					*dp_Freq_Rx_E = 2690.0;
					break;
				case NR5GSUB6_N_66:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1780.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2200.0;
					break;
				case NR5GSUB6_N_71:
					*dp_Freq_Tx_S = 663.0;
					*dp_Freq_Tx_E = 698.0;
					*dp_Freq_Rx_S = 617.0;
					*dp_Freq_Rx_E = 652.0;
					break;
				case NR5GSUB6_N_77:
					*dp_Freq_Tx_S = 3300.0;
					*dp_Freq_Tx_E = 4200.0;
					*dp_Freq_Rx_S = 3300.0;
					*dp_Freq_Rx_E = 4200.0;
					break;
				case NR5GSUB6_N_78:
					*dp_Freq_Tx_S = 3300.0;
					*dp_Freq_Tx_E = 3800.0;
					*dp_Freq_Rx_S = 3300.0;
					*dp_Freq_Rx_E = 3800.0;
					break;
				case NR5GSUB6_N_79:
					*dp_Freq_Tx_S = 4400.0;
					*dp_Freq_Tx_E = 5000.0;
					*dp_Freq_Rx_S = 4400.0;
					*dp_Freq_Rx_E = 5000.0;
					break;
				case NR5GSUB6_N_12:
					*dp_Freq_Tx_S = 699.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 729.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case NR5GSUB6_N_25:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1915.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1995.0;
					break;
				case NR5GSUB6_N_48:
					*dp_Freq_Tx_S = 3550.0;
					*dp_Freq_Tx_E = 3700.0;
					*dp_Freq_Rx_S = 3550.0;
					*dp_Freq_Rx_E = 3700.0;
					break;
				case NR5GSUB6_N_38:
					*dp_Freq_Tx_S = 2570.0;
					*dp_Freq_Tx_E = 2620.0;
					*dp_Freq_Rx_S = 2570.0;
					*dp_Freq_Rx_E = 2620.0;
					break;
				case NR5GSUB6_N_40:
					*dp_Freq_Tx_S = 2300.0;
					*dp_Freq_Tx_E = 2400.0;
					*dp_Freq_Rx_S = 2300.0;
					*dp_Freq_Rx_E = 2400.0;
					break;
				case NR5GSUB6_N_14:
					*dp_Freq_Tx_S = 788.0;
					*dp_Freq_Tx_E = 798.0;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 768.0;
					break;
				case NR5GSUB6_N_13:
					*dp_Freq_Tx_S = 777.0;
					*dp_Freq_Tx_E = 787.0;
					*dp_Freq_Rx_S = 746.0;
					*dp_Freq_Rx_E = 756.0;
					break;
				case NR5GSUB6_N_18:
					*dp_Freq_Tx_S = 815.0;
					*dp_Freq_Tx_E = 830.0;
					*dp_Freq_Rx_S = 860.0;
					*dp_Freq_Rx_E = 875.0;
					break;
				case NR5GSUB6_N_26:
					*dp_Freq_Tx_S = 814.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 859.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case NR5GSUB6_N_29:
					*dp_Freq_Tx_S = 717.0;
					*dp_Freq_Tx_E = 728.0;
					*dp_Freq_Rx_S = 717.0;
					*dp_Freq_Rx_E = 728.0;
					break;
				case NR5GSUB6_N_30:
					*dp_Freq_Tx_S = 2305.0;
					*dp_Freq_Tx_E = 2315.0;
					*dp_Freq_Rx_S = 2350.0;
					*dp_Freq_Rx_E = 2360.0;
					break;
				case NR5GSUB6_N_75:
					*dp_Freq_Tx_S = 1432.0;
					*dp_Freq_Tx_E = 1517.0;
					*dp_Freq_Rx_S = 1432.0;
					*dp_Freq_Rx_E = 1517.0;
					break;
				case NR5GSUB6_N_76:
					*dp_Freq_Tx_S = 1427.0;
					*dp_Freq_Tx_E = 1432.0;
					*dp_Freq_Rx_S = 1427.0;
					*dp_Freq_Rx_E = 1432.0;
					break;
				case NR5GSUB6_N_91:
					*dp_Freq_Tx_S = 832.0;
					*dp_Freq_Tx_E = 862.0;
					*dp_Freq_Rx_S = 1427.0;
					*dp_Freq_Rx_E = 1432.0;
					break;
				case NR5GSUB6_N_92:
					*dp_Freq_Tx_S = 832.0;
					*dp_Freq_Tx_E = 862.0;
					*dp_Freq_Rx_S = 1432.0;
					*dp_Freq_Rx_E = 1517.0;
					break;
				case NR5GSUB6_N_93:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 1427.0;
					*dp_Freq_Rx_E = 1432.0;
					break;
				case NR5GSUB6_N_94:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 1432.0;
					*dp_Freq_Rx_E = 1517.0;
					break;
				case NR5GSUB6_N_70:
					*dp_Freq_Tx_S = 1695.0;
					*dp_Freq_Tx_E = 1710.0;
					*dp_Freq_Rx_S = 1995.0;
					*dp_Freq_Rx_E = 2020.0;
					break;
				case NR5GSUB6_N_257:
					*dp_Freq_Tx_S = 26500.0;
					*dp_Freq_Tx_E = 29499.94;
					*dp_Freq_Rx_S = 26500.0;
					*dp_Freq_Rx_E = 29499.94;
					break;
				case NR5GSUB6_N_258:
					*dp_Freq_Tx_S = 24250.0;
					*dp_Freq_Tx_E = 27499.90;
					*dp_Freq_Rx_S = 24250.0;
					*dp_Freq_Rx_E = 27499.90;
					break;
				case NR5GSUB6_N_260:
					*dp_Freq_Tx_S = 37000.0;
					*dp_Freq_Tx_E = 39999.94;
					*dp_Freq_Rx_S = 37000.0;
					*dp_Freq_Rx_E = 39999.94;
					break;
				case NR5GSUB6_N_261:
					*dp_Freq_Tx_S = 27500.0;
					*dp_Freq_Tx_E = 28349.96;
					*dp_Freq_Rx_S = 27500.0;
					*dp_Freq_Rx_E = 28349.96;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_NR5GMMW_NS:
			{
				switch( i_Band_Index )
				{
				case NR5GMMW_N_257:
					*dp_Freq_Tx_S = 26500.0;
					*dp_Freq_Tx_E = 29500.0;
					*dp_Freq_Rx_S = 26500.0;
					*dp_Freq_Rx_E = 29500.0;
					break;
				case NR5GMMW_N_258:
					*dp_Freq_Tx_S = 24250.0;
					*dp_Freq_Tx_E = 27500.0;
					*dp_Freq_Rx_S = 24250.0;
					*dp_Freq_Rx_E = 27500.0;
					break;
				case NR5GMMW_N_260:
					*dp_Freq_Tx_S = 37000.0;
					*dp_Freq_Tx_E = 40000.0;
					*dp_Freq_Rx_S = 37000.0;
					*dp_Freq_Rx_E = 40000.0;
					break;
				case NR5GMMW_N_261:
					*dp_Freq_Tx_S = 27500.0;
					*dp_Freq_Tx_E = 28350.0;
					*dp_Freq_Rx_S = 27500.0;
					*dp_Freq_Rx_E = 28350.0;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		default:
			//fail
			throw(0);
		}//switch( i_Format
	}
	catch( ... )
	{
		//������
		::AfxMessageBox( _T( "ERROR, Get_RF_Offset_Freq_Band_Seg!" ) );
		//fail
		return;
	}

	//pass
	return;
}

bool CGFC::GET_ARFCN_L_M_H_TYPE( int i_Band, int i_Format_Network, int i_ARFCN, int * ip_LMH_TYPE )
{
	try
	{
		if( (i_Format_Network == FORMAT_GSM_S) || (i_Format_Network == FORMAT_GSM_NS) )
		{
			switch( i_Band )
			{
			case G_BAND_GSM900:
				{
					if( (975 <= i_ARFCN) && (i_ARFCN <= 1023) ) // EGSM
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (0 <= i_ARFCN) && (i_ARFCN <= 31) ) // PGSM
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (32 <= i_ARFCN) && (i_ARFCN <= 71) ) // PGSM
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (72 <= i_ARFCN) && (i_ARFCN <= 124) ) // PGSM
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;

			case G_BAND_DCS1800:
				{
					if( (512 <= i_ARFCN) && (i_ARFCN <= 636) )
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (637 <= i_ARFCN) && (i_ARFCN <= 760) )
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (761 <= i_ARFCN) && (i_ARFCN <= 885) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;

			case G_BAND_PCS1900:
				{
					if( (512 <= i_ARFCN) && (i_ARFCN <= 611) )
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (612 <= i_ARFCN) && (i_ARFCN <= 711) )
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (712 <= i_ARFCN) && (i_ARFCN <= 810) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;

			case G_BAND_GSM850:
				{
					if( (128 <= i_ARFCN) && (i_ARFCN <= 169) )
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (170 <= i_ARFCN) && (i_ARFCN <= 210) )
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (211 <= i_ARFCN) && (i_ARFCN <= 251) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;

			default:
				throw(0);
			}
		}
		else if( i_Format_Network == FORMAT_WCDMA_S )
		{
			throw(0);
		}
		else if( i_Format_Network == FORMAT_C2K_S )
		{
			switch( i_Band )
			{
			case C2K_BAND_800:
				{
					if( (1 <= i_ARFCN) && (i_ARFCN <= 341) )
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (342 <= i_ARFCN) && (i_ARFCN <= 682) )
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (683 <= i_ARFCN) && (i_ARFCN <= 1023) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else if( (1024 <= i_ARFCN) && (i_ARFCN <= 1323) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;
			case C2K_BAND_1900:
				{
					if( (0 <= i_ARFCN) && (i_ARFCN <= 400) )
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (401 <= i_ARFCN) && (i_ARFCN <= 801) )
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (802 <= i_ARFCN) && (i_ARFCN <= 1199) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;
			default:
				throw(0);
			}
		}
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

void CGFC::Adjust_From_Small_To_Large_Array_Int( int * ip_Data_List, int i_Quan )
{
	if( i_Quan <= 1 )
	{
		return;
	}


	//��С��������
	int a, b, c;

	for( a = 0; a < i_Quan; a++ )
	{
		for( b = a + 1; b < i_Quan; b++ )
		{
			if( *(ip_Data_List + a) > *(ip_Data_List + b) )
			{
				c = *(ip_Data_List + a);
				*(ip_Data_List + a) = *(ip_Data_List + b);
				*(ip_Data_List + b) = c;
			}
		}
	}
}

int CGFC::Rand_One_Number( int i_Range )
{
	//����i_Range����Χ����һ�������
	int i;

	//
	srand( (unsigned)time( NULL ) );

	//
	i = rand();
	i %= i_Range;

	//����
	return i;
}

bool CGFC::Calcu_MEID_Check_Digit( const wchar_t * wcp_MEID_Hex, wchar_t * wcp_Checksum )
{
	//����MEID��У��λ

	if( wcslen( wcp_MEID_Hex ) != 14 )
	{
		return false;
	}

	int i, iTemp;
	int iTotal = 0;
	CString csTemp;
	wchar_t wrBits[2];


	//
	wrBits[1] = '\0';


	//������λ������ӣ��ټ�����һ����õ�ֵ
	for( i = 0; i < 14; i += 2 )
	{
		wrBits[0] = *(wcp_MEID_Hex + i);
		iTotal += wcstoul( wrBits, NULL, 16 );
	}


	//��ż��λ���ֱַ����2���ֱ���㡰��λ�����͡�ʮλ����֮�͡���ע����16��������
	for( i = 1; i < 14; i += 2 )
	{
		//ż��λ���ֱַ����2
		wrBits[0] = *(wcp_MEID_Hex + i);
		iTemp = 2 * wcstoul( wrBits, NULL, 16 );

		csTemp.Format( _T( "%02X" ), iTemp );

		//�ֱ���㡰��λ�����͡�ʮλ����֮��
		iTotal += wcstoul( csTemp.Mid( 0, 1 ), NULL, 16 );
		iTotal += wcstoul( csTemp.Mid( 1, 1 ), NULL, 16 );
	}


	//����ó�������λ��0��У��λΪ0������Ϊ10(�����10��16����)��ȥ��λ��
	iTotal %= 16;//��ȡʮ�����Ƶ�ĩλ��
	iTotal = (iTotal == 0) ? 0 : (16 - iTotal);


	//����תΪ16����
	*wcp_Checksum = (iTotal >= 0 && iTotal <= 9) ? (iTotal + '0') : (iTotal + 'A' - 10);

	//
	return true;
}

void CGFC::WCSTOK_STRING_T_Q( const wchar_t * wcp_StrToken, int i_Take_Quan, CString * csp_Take_CString_Array )
{
	//�ӡ�����Դ����ȡ���ض��������ġ��Ӵ���
	//�ԡ�Ĭ�Ϸָ�������ȡ

	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//��Դ���ݡ���buff��wcstok����ȡ����ʱ��ı�Դ�������ݣ������Ȱ�Դ���ݷ���buff������buff���ݣ����˺�����Դ��
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//���ڴ�š�Դ���ݡ���buff����
	WCHAR * wcpResult = NULL;


	//���buff
	for( i = 0; i < i_Take_Quan; i++ )
	{
		(csp_Take_CString_Array + i)->Format( _T( "" ) );
	}


	//�����ڴ�
	this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );


	//���롰Դ���ݡ�
	swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

	//��ȡ��һ��ֵ(0)
	wcpEach = wcstok_s( wcpSrc, _T( " ,;\r\n" ), &wcpResult );

	//��������
	i = 0;

	//ѭ����ȡ
	while( wcpEach != NULL )
	{
		//��ֵ
		(csp_Take_CString_Array + i)->Format( _T( "%s" ), wcpEach );

		//��ȡ��һ��ֵ
		wcpEach = wcstok_s( NULL, _T( " ,;\r\n" ), &wcpResult );

		//����
		i++;

		//
		if( i == i_Take_Quan )
		{
			break;
		}
	}//while(wcpEach != NULL


	//�ͷ��ڴ�
	this->BUFF_WCHAR_FREE( &wcpSrc );
}

void CGFC::Adjust_From_Small_To_Large_Array_Double( double * dp_Data_List, int i_Quan )
{
	if( i_Quan <= 1 )
	{
		return;
	}


	//��С��������
	int a, b;
	double v;

	for( a = 0; a < i_Quan; a++ )
	{
		for( b = a + 1; b < i_Quan; b++ )
		{
			if( *(dp_Data_List + a) > *(dp_Data_List + b) )
			{
				v = *(dp_Data_List + a);
				*(dp_Data_List + a) = *(dp_Data_List + b);
				*(dp_Data_List + b) = v;
			}
		}
	}
}

bool CGFC::ReadPhysicalDriveInNTUsingSmart( CString * csp_HDD_SN )
{
	bool done = false;
	int drive = 0;
	HANDLE hPhysicalDriveIOCTL;
	wchar_t driveName[256];


	//
	try
	{
		for( drive = 0; drive < MAX_IDE_DRIVES; drive++ )
		{
			hPhysicalDriveIOCTL = INVALID_HANDLE_VALUE;

			//  Try to get a handle to PhysicalDrive IOCTL, report failure
			//  and exit if can't.
			swprintf_s( driveName, _T( "\\\\.\\PhysicalDrive%d" ), drive );


			//  Windows NT, Windows 2000, Windows Server 2003, Vista
			hPhysicalDriveIOCTL = CreateFile( driveName,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL, OPEN_EXISTING, 0, NULL );
			// if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
			//    printf ("Unable to open physical drive %d, error code: 0x%lX\n",
			//            drive, GetLastError ());

			//�ж�
			if( hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE )
			{
				/*
				#ifdef PRINTING_TO_CONSOLE_ALLOWED
				if (PRINT_DEBUG)
				printf ("\n%d ReadPhysicalDriveInNTUsingSmart ERROR"
				"\nCreateFile(%s) returned INVALID_HANDLE_VALUE\n"
				"Error Code %d\n",
				__LINE__, driveName, GetLastError ());
				#endif
				*/
			}
			else
			{
				GETVERSIONINPARAMS GetVersionParams;
				DWORD cbBytesReturned = 0;

				// Get the version, etc of PhysicalDrive IOCTL
				memset( (void*)& GetVersionParams, 0, sizeof( GetVersionParams ) );

				if( !DeviceIoControl( hPhysicalDriveIOCTL, SMART_GET_VERSION,
					NULL,
					0,
					&GetVersionParams,
					sizeof( GETVERSIONINPARAMS ),
					&cbBytesReturned,
					NULL ) )
				{
					/*
					#ifdef PRINTING_TO_CONSOLE_ALLOWED
					if (PRINT_DEBUG)
					{
					DWORD err = GetLastError ();
					printf ("\n%d ReadPhysicalDriveInNTUsingSmart ERROR"
					"\nDeviceIoControl(%d, SMART_GET_VERSION) returned 0, error is %d\n",
					__LINE__, (int) hPhysicalDriveIOCTL, (int) err);
					}
					#endif
					*/
				}
				else
				{
					// Print the SMART version
					// PrintVersion (& GetVersionParams);
					// Allocate the command buffer
					ULONG CommandSize = sizeof( SENDCMDINPARAMS ) + IDENTIFY_BUFFER_SIZE;
					PSENDCMDINPARAMS Command = (PSENDCMDINPARAMS)malloc( CommandSize );
					// Retrieve the IDENTIFY data
					// Prepare the command

					//
					Command->irDriveRegs.bCommandReg = ID_CMD;//#define ID_CMD 0xEC// Returns ID sector for ATA
					DWORD BytesReturned = 0;

					//
					if( !DeviceIoControl( hPhysicalDriveIOCTL,
						SMART_RCV_DRIVE_DATA, Command, sizeof( SENDCMDINPARAMS ),
						Command, CommandSize,
						&BytesReturned, NULL ) )
					{
						// Print the error
						//PrintError ("SMART_RCV_DRIVE_DATA IOCTL", GetLastError());
					}
					else
					{
						// Print the IDENTIFY data
						DWORD diskdata[256];

						USHORT *pIdSector = (USHORT *)
							(PIDENTIFY_DATA)((PSENDCMDOUTPARAMS)Command)->bBuffer;

						for( int ijk = 0; ijk < 256; ijk++ )
						{
							diskdata[ijk] = pIdSector[ijk];
						}

						//��ȡ������
						this->PrintIdeInfo( drive, diskdata, csp_HDD_SN );

						//ͳһ��д
						csp_HDD_SN->MakeUpper();

						//pass
						done = true;
					}


					//free
					CloseHandle( hPhysicalDriveIOCTL );
					free( Command );


					//
					if( done == true )
					{
						//pass
						break;
					}
				}
			}
		}//for(drive = 0; drive < MAX_IDE_DRIVES; drive++

	}
	catch( ... )
	{
		//fail
		return false;
	}

	//����
	return done;
}

void CGFC::PrintIdeInfo( int drive, DWORD * diskdata, CString * csp_HDD_SN )
{
	char serialNumber[1024];
	char modelNumber[1024];
	char revisionNumber[1024];
	char bufferSize[32];

	__int64 sectors = 0;
	__int64 bytes = 0;

	wchar_t wcrData[512];
	int iTemp;


	//  copy the hard drive serial number to the buffer
	this->ConvertToString( diskdata, 10, 19, serialNumber );
	this->ConvertToString( diskdata, 27, 46, modelNumber );
	this->ConvertToString( diskdata, 23, 26, revisionNumber );
	sprintf_s( bufferSize, "%u", diskdata[21] * 512 );


	//
	this->MultiByte_To_WideChar( serialNumber, wcrData, &iTemp );
	csp_HDD_SN->Format( _T( "%s" ), wcrData );


	//ȥ�����ո�
	csp_HDD_SN->Remove( _T( ' ' ) );
}

void CGFC::ConvertToString( DWORD * diskdata, int firstIndex, int lastIndex, char * buf )
{
	int index = 0;
	int position = 0;

	//  each integer has two characters stored in it backwards
	for( index = firstIndex; index <= lastIndex; index++ )
	{
		//  get high byte for 1st character
		buf[position++] = (char)(diskdata[index] / 256);

		//  get low byte for 2nd character
		buf[position++] = (char)(diskdata[index] % 256);
	}

	//  end the string 
	buf[position] = '\0';

	//  cut off the trailing blanks
	for( index = position - 1; index > 0 && isspace( buf[index] ); index-- )
		buf[index] = '\0';
}

int CGFC::HEX_WCHAR_TO_SIGNED_INT32( const wchar_t * wcp_HEX_WCHAR )
{
	//��8λ16���ƣ�32λ�����ƣ�4���ֽڣ�ת��Ϊ���з���int����
	//�����HEX�ִ����衰0x����ͷ��
	//ͬһ���������� ԭ�롱���Խ��Ϊ���з��š���Ҳ�ɽ��Ϊ���޷��š���
	//ʵ�ʾ��ǰ��������λ�ֽڣ�4λ������ߵ�һλ�ġ�1�����Ϊ�������ű�ʶ�����ǽ��Ϊ����ֵ1���Ĺ��̡�

	//�ȼ��HEX�ִ�
	if( wcslen( wcp_HEX_WCHAR ) <= 0 )
	{
		return 0;
	}


	//
	unsigned long i32;
	wchar_t wcrHex_Data[64];	//�š������齨�ġ�16��������
	wchar_t wcrBinary[128];		//�Ŷ������ַ���
	int iRtn_Signed_Int = 0;
	int iRatio = 1;
	int iLen, i;



	//16����ֵת�����޷��ų����͡�
	//ע�⣺Ϊ�˷�ֹ���ݶ�ʧ����ֹ��ʧ��һλ��־λ���ݡ�1����������תΪ���޷��ų����͡�������ֻ��ת��4λ16����
	i32 = wcstoul( wcp_HEX_WCHAR, NULL, 16 );


	//�����齨16�����ַ����������Զ�ȥ����16�����ַ���ǰ��0��
	swprintf_s( wcrHex_Data, _T( "%X" ), i32 );


	//�ж϶�������λ�Ƿ��ǡ�1����1��Ϊ����
	if( wcrHex_Data[0] > '8' )//8 = 1000
	{
		//����		

		//
		memset( wcrBinary, 0, sizeof( wcrBinary ) );

		//���Ϊ�������ַ���
		_ui64tow_s( i32, wcrBinary, 128, 2 );

		//������λ��
		iLen = wcslen( wcrBinary );


		//����һλ�⡰ȡ����
		for( i = 1; i < iLen; i++ )
		{
			wcrBinary[i] = '0' + !(wcrBinary[i] - '0');
		}


		//������תʮ���ƣ�Ȼ���һ��i=0�Ǳ�־λ����������㣩
		for( i = (iLen - 1); i > 0; i-- )
		{
			iRtn_Signed_Int = iRtn_Signed_Int + (wcrBinary[i] - '0') * iRatio;
			iRatio = iRatio * 2;
		}
		//
		iRtn_Signed_Int++;//��һ

		//����
		iRtn_Signed_Int = iRtn_Signed_Int * (-1);
	}
	else
	{
		//����
		iRtn_Signed_Int = i32;
	}


	//���ؽ��
	return iRtn_Signed_Int;



	/*��һ��ʵ�ֵĴ��룬�������⣬�и������޷�ת��
	CString csData;
	int iSigned_Int32_Value;


	//��ֹ��ʧ��һλ��־λ���ݡ�1��������Ҫ��ת��Ϊ���޷��ų����͡�
	unsigned long ul32bits = wcstoul(wcp_HEX_WCHAR, NULL, 16);

	//תΪ�����������ŵ�ʮ�����ַ�����
	csData.Format(_T("%d"), (signed long)ul32bits);

	//�ַ���תΪ���з�����������
	iSigned_Int32_Value = _wtoi(csData);

	//����
	return iSigned_Int32_Value;
	*/

}

unsigned long CGFC::HEX_WCHAR_TO_UNSIGNED_INT32( const wchar_t * wcp_HEX_WCHAR )
{
	//��8λ16���ƣ�32λ�����ƣ�4���ֽڣ�ת��Ϊ���޷���int��
	//Ĭ��hex����ȫ���������޷���

	unsigned long ul32bits = wcstoul( wcp_HEX_WCHAR, NULL, 16 );

	//����
	return ul32bits;
}

bool CGFC::Encode_RF_Ref_Tx_Data_To_Str_V2( double d_Code_Key, double * dp_RF_Tx_Data, int i_RF_Tx_Quan, int * ip_Band_List, int i_Band_Quan, CString * csp_Str_Data )
{
	//����Ƶ�κ�Tx��������ת�ɡ��ַ����������ڽ�����ݵ�д�롣
	//ÿ��Ƶ���С����и�3�����ʡ�

	CString csAll, csBand, csCh;
	int i, j, iCode;


	//
	try
	{

		//�������Ƶ����Ϣ
		csAll.Format( _T( "" ) );


		//Ƶ��ѭ��
		for( i = 0; i < i_Band_Quan; i++ )
		{
			//Ƶ����Ϣ�����롱
			if( (*(ip_Band_List + i) >= 0) && (*(ip_Band_List + i) <= 25) )
			{
				//a~z
				csBand.Format( _T( "%c" ), (*(ip_Band_List + i) + 'a') );
			}
			else if( (*(ip_Band_List + i) >= 26) && (*(ip_Band_List + i) <= 51) )
			{
				//A~Z
				csBand.Format( _T( "%c" ), (*(ip_Band_List + i) - 26 + 'A') );
			}
			else
			{
				this->m_csError_Info.Format( _T( "ERROR, Ƶ��index�����쳣����ֹ�����!" ) );
				::AfxMessageBox( this->m_csError_Info );
				//fail
				throw(0);
			}


			//���и�3������ѭ��
			for( j = 0; j < 3; j++ )
			{
				//С������һλ����һλ��С�����ڶ�λ�������룻
				iCode = (int)((*(dp_RF_Tx_Data + i * 3 + j) + 0.05 - d_Code_Key) * 10.0);

				//
				if( iCode < 0 )
				{
					this->m_csError_Info.Format( _T( "ERROR, Ref����ƫС�����ʺ������!" ) );
					::AfxMessageBox( this->m_csError_Info );
					throw(0);
				}

				if( iCode > 99 )
				{
					this->m_csError_Info.Format( _T( "ERROR, Ref����ƫ�󣬲��ʺ������!" ) );
					::AfxMessageBox( this->m_csError_Info );
					throw(0);
				}


				//����������Ϣ�����롱
				csCh.Format( _T( "%02d" ), iCode );

				//��ӵ���������Ϣ�����롱
				csBand += csCh;
			}//for(j=0; j<3; j++


			//��ӵ�������Ƶ����Ϣ�����롱
			csAll += csBand;
		}//for(i=0; i<i_Band_Quan; i++


		//���ص��ִ�
		csp_Str_Data->Format( _T( "%s" ), csAll );
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Decode_RF_Ref_Tx_Data_From_Str_V2( const wchar_t * wcp_Ref_Str_Data, double d_Code_Key, int * ip_Band_List, int * ip_Band_Quan, int * ip_RF_Tx_Data_Quan, double * dp_RF_Tx_Data )
{
	//�ӡ�����ַ���������ȡ����Ƶ�ζ�Ӧ�Ĺ������ݡ�
	//ÿ��Ƶ���С����и�3�����ʡ�

	int iLen, iBand_Quan, i, iPwr_Quan, j;
	CString csData;

	//
	try
	{
		iLen = wcslen( wcp_Ref_Str_Data );

		//��鳤�ȣ�Ƶ�κ� + 2λ���ŵ� + 2λ���ŵ� + 2λ���ŵ���
		if( (iLen % 7) != 0 )
		{
			::AfxMessageBox( _T( "ERROR, ������ݳ��ȴ���!" ) );
			throw(0);
		}

		//Ƶ������
		iBand_Quan = iLen / 7;

		//��������
		iPwr_Quan = 0;


		//
		for( i = 0; i < iBand_Quan; i++ )//Ƶ��ѭ��
		{
			//���Ƶ�α�ʶ�����ָ�����
			if( (*(wcp_Ref_Str_Data + i * 7) >= 'a') && (*(wcp_Ref_Str_Data + i * 7) <= 'z') )
			{
				//a~z
				//Ƶ�κŵ�index
				*(ip_Band_List + i) = *(wcp_Ref_Str_Data + i * 7) - 'a';
			}
			else if( (*(wcp_Ref_Str_Data + i * 7) >= 'A') && (*(wcp_Ref_Str_Data + i * 7) <= 'Z') )
			{
				//A~Z
				//Ƶ�κŵ�index
				*(ip_Band_List + i) = *(wcp_Ref_Str_Data + i * 7) - 'A' + 26;
			}
			else
			{
				::AfxMessageBox( _T( "ERROR, ���Ƶ�α�ʶ�����ݴ���!" ) );
				throw(0);//Ƶ�α�ʶ������
			}


			//������Ϣ
			for( j = 0; j < 3; j++ )//���и�3������ѭ��
			{
				csData.Format( _T( "%c%c" ), *(wcp_Ref_Str_Data + i * 7 + j * 2 + 1), *(wcp_Ref_Str_Data + i * 7 + j * 2 + 2) );
				*(dp_RF_Tx_Data + iPwr_Quan) = _wtof( csData ) / 10.0 + d_Code_Key;
				//
				iPwr_Quan++;
			}
		}//for( i = 0; i < iBand_Quan; i++


		//��������
		*ip_Band_Quan = iBand_Quan;
		*ip_RF_Tx_Data_Quan = iPwr_Quan;
	}
	catch( ... )
	{
		return false;
	}

	//
	return true;
}

bool CGFC::Get_Data_In_Quotation_Marks( const wchar_t * wcp_Src_Data, CString * csp_Get_Data )
{
	//�ڡ�����Դ������ȡ����������֮����ַ����ݡ�

	int iLen = wcslen( wcp_Src_Data );//Դ���ݳ���
	wchar_t * wcpSrc_Buff = NULL;
	CString csGet_Buff;
	int i;
	bool bGet_First = false;
	bool bGet_Second = false;
	unsigned long ulBuff_Len = wcslen( wcp_Src_Data ) + 8;//���ڴ�š�Դ���ݡ���buff����


	//
	try
	{
		//�����ȡBUFF
		csGet_Buff.Format( _T( "" ) );


		//�����ڴ�
		this->BUFF_WCHAR_CALLOC( &wcpSrc_Buff, ulBuff_Len );

		//����Դ����
		swprintf( wcpSrc_Buff, ulBuff_Len, _T( "%s" ), wcp_Src_Data );


		//ѭ�����������š�
		for( i = 0; i < iLen; i++ )
		{
			if( bGet_First == false )
			{
				//�ҵ�һ������
				if( *(wcpSrc_Buff + i) == '"' )
				{
					bGet_First = true;
				}
			}
			else
			{
				//�ҵڶ�������
				if( *(wcpSrc_Buff + i) == '"' )
				{
					//�ɹ��ҵ��˵ڶ������ţ����Խ�β��
					bGet_Second = true;
					//pass
					break;//�˳�for
				}
				else
				{
					//�������ż������
					csGet_Buff += *(wcpSrc_Buff + i);
				}
			}
		}//for(i=0; i<iLen; i++


		//�жϡ��������
		if( (bGet_First == true) && (bGet_Second == true) )
		{
			csp_Get_Data->Format( _T( "%s" ), csGet_Buff );
		}
		else
		{
			throw(0);
		}


		//�ͷ��ڴ�
		this->BUFF_WCHAR_FREE( &wcpSrc_Buff );
	}
	catch( ... )
	{
		//�ͷ��ڴ�
		this->BUFF_WCHAR_FREE( &wcpSrc_Buff );
		//
		return false;
	}

	//
	return true;
}

void CGFC::Get_PF_Name( int i_PF_Index, CString * csp_PF_Name )
{
	//оƬƽ̨��index�� ת��ΪоƬƽ̨��

	switch( i_PF_Index )
	{
	case PF_MTK:		//MTK
		csp_PF_Name->Format( _T( "MTK" ) );
		break;
	case PF_QUALCOMM:	//��ͨ
		csp_PF_Name->Format( _T( "QUALCOMM" ) );
		break;
	case PF_RDA:		//RDA
		csp_PF_Name->Format( _T( "RDA" ) );
		break;
	case PF_GCT:		//GCT
		csp_PF_Name->Format( _T( "GCT" ) );
		break;
	case PF_INTEL:		//Intel
		csp_PF_Name->Format( _T( "INTEL" ) );
		break;
	case PF_HUAWEI://��˼
		csp_PF_Name->Format( _T( "HISILICON" ) );
		break;
	case PF_ASR:		//ASR
		csp_PF_Name->Format( _T( "ASR" ) );
		break;
	case PF_REALTEK:		//Realtek
		csp_PF_Name->Format( _T( "Realtek" ) );
		break;
	case PF_SPREADTRUM:	//չ�� Spreadtrum
		csp_PF_Name->Format( _T( "Spreadtrum" ) );
		break;
	case PF_EIGENCOMM:	//��о Eigencomm
		csp_PF_Name->Format( _T( "Eigencomm" ) );
		break;
	case PF_TI:	//�������� Texas Instruments
		csp_PF_Name->Format( _T( "TI" ) );
		break;
	case PF_XINYI:		//о����Ϣ�Ƽ�(�Ϻ�)���޹�˾
		csp_PF_Name->Format( _T( "XINYI" ) );
		break;
	case PF_ALTAIR:		//Sony Altair
		csp_PF_Name->Format( _T( "Altair" ) );
		break;
	case PF_BEKEN:		//��ͨ���ɵ�·���Ϻ����ɷ����޹�˾
		csp_PF_Name->Format( _T( "BEKEN" ) );
		break;
	case PF_CYPRESS:		//������˹ Cypress �뵼�幫˾
		csp_PF_Name->Format( _T( "Cypress" ) );
		break;
	case PF_SILICON_LABS:	//о�ƿƼ�(Silicon Labs)
		csp_PF_Name->Format( _T( "Silicon_Labs" ) );
		break;
	case PF_AMLOGIC:		//Amlogic �����뵼��(�Ϻ�)�ɷ����޹�˾
		csp_PF_Name->Format( _T( "Amlogic" ) );
		break;
	default:
		csp_PF_Name->Format( _T( "NDF" ) );
		break;
	}//switch( i_PF_Index
}

void CGFC::Get_BB_Name( int i_BB_Index, CString * csp_BB_Name )
{
	//BBоƬ��index�� ת��ΪBBоƬ��

	switch( i_BB_Index )
	{
	case BB_MDM6200:	//��ͨ UC20 ϵ��
		csp_BB_Name->Format( _T( "MDM6200" ) );
		break;
	case BB_QSC6270:	//��ͨ UC15 ϵ��
		csp_BB_Name->Format( _T( "QSC6270" ) );
		break;
	case BB_XMM6255:	//Intel UG95 ϵ��
		csp_BB_Name->Format( _T( "XMM6255" ) );
		break;
	case BB_XMM6250:	//Intel UG96 ϵ��
		csp_BB_Name->Format( _T( "XMM6250" ) );
		break;
	case BB_MT6261:		//MTK
		csp_BB_Name->Format( _T( "MT6261" ) );
		break;
	case BB_MDM9x15:	//��ͨ
		csp_BB_Name->Format( _T( "MDM9x15" ) );
		break;
	case BB_MSM8909:	//��ͨ4GоƬ
		csp_BB_Name->Format( _T( "MSM8909" ) );
		break;
	case BB_MDM9x07:	//��ͨ
		csp_BB_Name->Format( _T( "MDM9x07" ) );
		break;
	case BB_RDA8851:	//RDA
		csp_BB_Name->Format( _T( "RDA8851" ) );
		break;
	case BB_MSM8937:	//��ͨ4GоƬ
		csp_BB_Name->Format( _T( "MSM8937" ) );
		break;
	case BB_MSM8953:	//��ͨ4GоƬ
		csp_BB_Name->Format( _T( "MSM8953" ) );
		break;
	case BB_AR6003G:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "AR6003G" ) );
		break;
	case BB_QCA1023:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "QCA1023" ) );
		break;
	case BB_MDM9206:	//��ͨ4GоƬ
		csp_BB_Name->Format( _T( "MDM9206" ) );
		break;
	case BB_MDM9628:	//��ͨ4G����оƬ
		csp_BB_Name->Format( _T( "MDM9628" ) );
		break;
	case BB_HI2110:		//��˼Hi2110 NB-IoTоƬ
		csp_BB_Name->Format( _T( "HI2110" ) );
		break;
	case BB_MSM8917:	//��ͨ4GоƬ
		csp_BB_Name->Format( _T( "MSM8917" ) );
		break;
	case BB_MDM9x40:	//��ͨ4GоƬ CAT-6
		csp_BB_Name->Format( _T( "MDM9x40" ) );
		break;
	case BB_SDM450:		//��ͨSDM450
		csp_BB_Name->Format( _T( "SDM450" ) );
		break;
	case BB_WCN3610:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN3610" ) );
		break;
	case BB_WCN3660:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN3660" ) );
		break;
	case BB_WCN3680:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN3680" ) );
		break;
	case BB_MSM8905:	//��ͨ4GоƬ
		csp_BB_Name->Format( _T( "MSM8905" ) );
		break;
	case BB_MT2625:		//MTK NB-IoTоƬ
		csp_BB_Name->Format( _T( "MT2625" ) );
		break;
	case BB_MT2503:		//MTK 2G+GPSоƬ
		csp_BB_Name->Format( _T( "MT2503" ) );
		break;
	case BB_SDX20:		//��ͨ4GоƬ
		csp_BB_Name->Format( _T( "SDX20" ) );
		break;
	case BB_RDA8955L:	//RDA��2GоƬ
		csp_BB_Name->Format( _T( "RDA8955L" ) );
		break;
	case BB_ASR1802S:	//ASR��4GоƬ
		csp_BB_Name->Format( _T( "ASR1802S" ) );
		break;
	case BB_MDM9150:	//��ͨоƬ ֻ��C-V2X
		csp_BB_Name->Format( _T( "MDM9150" ) );
		break;
	case BB_QCA6564:	//��ͨWiFiоƬ(����ƽ̨)
		csp_BB_Name->Format( _T( "QCA6564" ) );
		break;
	case BB_SDM660:		//��ͨ4GоƬ
		csp_BB_Name->Format( _T( "SDM660" ) );
		break;
	case BB_RDA8908A:	//RDA��NB-IoTоƬ
		csp_BB_Name->Format( _T( "RDA8908A" ) );
		break;
	case BB_RDA8909B:	//RDA��2G+NB-IoTоƬ
		csp_BB_Name->Format( _T( "RDA8909B" ) );
		break;
	case BB_MT2621:		//MTK��2G+NB-IoTоƬ
		csp_BB_Name->Format( _T( "MT2621" ) );
		break;
	case BB_SA415M:		//��ͨ����4G+V2X����
		csp_BB_Name->Format( _T( "SA415M" ) );
		break;
	case BB_SDX24:		//��ͨ4GоƬ, A7 at 1.5GHz, 256KB L2, Ӧ���ڱʵ�, LTE-A/����
		csp_BB_Name->Format( _T( "SDX24" ) );
		break;
	case BB_MDM9205:	//��ͨ4GоƬ, LTE Cat-M / Cat-NB
		csp_BB_Name->Format( _T( "MDM9205" ) );
		break;
	case BB_SDX55:		//��ͨ 5G оƬ, ����
		csp_BB_Name->Format( _T( "SDX55" ) );
		break;
	case BB_RTL8189:	//Realtekƽ̨WiFiоƬ
		csp_BB_Name->Format( _T( "RTL8189" ) );
		break;
	case BB_QM215:		//��ͨ4G����ģ��(���MSM8909)
		csp_BB_Name->Format( _T( "QM215" ) );
		break;
	case BB_QCM2150:	//��ͨ4G����ģ��(���MSM8909)
		csp_BB_Name->Format( _T( "QCM2150" ) );
		break;
	case BB_SA515M:		//��ͨ 5G NR оƬ, SA515M�ǳ���ƽ̨, ��Ӧ�Ĺ���SDX55ƽ̨
		csp_BB_Name->Format( _T( "SA515M" ) );
		break;
	case BB_WCN3990:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN3990" ) );
		break;
	case BB_ASR3601:	//ASR��ASR1802S�ͳɱ�4GоƬ
		csp_BB_Name->Format( _T( "ASR3601" ) );
		break;
	case BB_SL8541E:	//չ��(Spreadtrum)4G����ģ��оƬ
		csp_BB_Name->Format( _T( "SL8541E" ) );
		break;
	case BB_HI2152:		//��˼HI2152 4G������ģ��оƬ
		csp_BB_Name->Format( _T( "HI2152" ) );
		break;
	case BB_QCA6391:	//��ͨ WiFi ���� оƬ
		csp_BB_Name->Format( _T( "QCA6391" ) );
		break;
	case BB_EC616:		//��о ���͹���NB-IoT��ģоƬ
		csp_BB_Name->Format( _T( "EC616" ) );
		break;
	case BB_ASR1601:	//ASR��4GоƬ CAT1
		csp_BB_Name->Format( _T( "ASR1601" ) );
		break;
	case BB_CC1200:		//TI �͹��ĸ�������Ƶ�շ���
		csp_BB_Name->Format( _T( "CC1200" ) );
		break;
	case BB_HI9500:		//��˼ 5G оƬ
		csp_BB_Name->Format( _T( "HI9500" ) );
		break;
	case BB_ASR1802SL:	//ASR��4GоƬ��Linux�汾
		csp_BB_Name->Format( _T( "ASR1802SL" ) );
		break;
	case BB_ASR1826:	//ASR��4GоƬ, LTE CAT-6
		csp_BB_Name->Format( _T( "ASR1826" ) );
		break;
	case BB_HI2115:		//��˼Hi2115 NB-IoTоƬ
		csp_BB_Name->Format( _T( "HI2115" ) );
		break;
	case BB_QCA6696:	//��ͨ���漶WiFi/BTоƬ��֧��802.11 ax WiFi, ֧������ BT 5.1��
		csp_BB_Name->Format( _T( "QCA6696" ) );
		break;
	case BB_SDA845:		//��ͨϵͳ��оƬ(SoC: System-on-a-Chip), SDA����modem, ��û�з����ƶ�����
		csp_BB_Name->Format( _T( "SDA845" ) );
		break;
	case BB_UDX710:		//չ�� Spreadtrum 5Gƽ̨ ����
		csp_BB_Name->Format( _T( "UDX710" ) );
		break;
	case BB_QCX212:		//��ͨ NB-IoT SoC chipset, Category NB2, 2-HARQ
		csp_BB_Name->Format( _T( "QCX212" ) );
		break;
	case BB_SM6115:		//��ͨ 4G оƬ, Snapdragon 662
		csp_BB_Name->Format( _T( "SM6115" ) );
		break;
	case BB_QTM527:		//��ͨ 5G mmWave antenna module
		csp_BB_Name->Format( _T( "QTM527" ) );
		break;
	case BB_EC617:		//�Ϻ���о�ĳ��ͳɱ������͹��ĺͳ��߼��ɶ�NB-IoT SoC оƬ����ȫ֧��3GPP Rel14 NB-IoT ��׼
		csp_BB_Name->Format( _T( "EC617" ) );
		break;
	case BB_WCN3980:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN3980" ) );
		break;
	case BB_ESP8285:	//����WiFiоƬ
		csp_BB_Name->Format( _T( "ESP8285" ) );
		break;
	case BB_UIS8910DM:	//չ�� advanced system-on-chip solution integrated CAT1bis/GSM/GPRS modems
		csp_BB_Name->Format( _T( "UIS8910DM" ) );
		break;
	case BB_UIS8581:	//չ�� ����ģ��оƬ with embedded TD-LTE, LTE FDD, TDSCDMA/HSPA(+), WCDMA/HSPA(+) and GSM/GPRS/EDGE to supportDual SIM Dual Standbyup to LTE+LTE
		csp_BB_Name->Format( _T( "UIS8581" ) );
		break;
	case BB_XY1100:		//о�� �����ڵ��� IoT �����ĸ����ܣ��߼��ɶȵ� SOC оƬ����оƬ��ȫ���� 3GPP TS 36.211 Э��
		csp_BB_Name->Format( _T( "XY1100" ) );
		break;
	case BB_HI2120:		//��˼Hi2120 NB2 NB-IoTоƬ
		csp_BB_Name->Format( _T( "HI2120" ) );
		break;
	case BB_MT2731:		//MTK 2G/3G/4G ����ƽ̨оƬ
		csp_BB_Name->Format( _T( "MT2731" ) );
		break;
	case BB_WCN3988:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN3988" ) );
		break;
	case BB_ALT1250:	//Sony Altair eMTC (CAT M), NB IoT (CAT NB1 / NB2)
		csp_BB_Name->Format( _T( "ALT1250" ) );
		break;
	case BB_QCA6595:	//��ͨ MU-MIMO WiFi/BT 5.1 Automotive Dual-MAC Combo Chip for Automotive Infotainment and Telematics applications
		csp_BB_Name->Format( _T( "QCA6595" ) );
		break;
	case BB_ASR1603:	//ASR��4GоƬ CAT-1, 1601 + Flash ��װ
		csp_BB_Name->Format( _T( "ASR1603" ) );
		break;
	case BB_QCA1064:	//��ͨ WiFi ���� оƬ
		csp_BB_Name->Format( _T( "QCA1064" ) );
		break;
	case BB_MT6880:		//MTK 5G оƬ, T700 ��Ƭ
		csp_BB_Name->Format( _T( "MT6880" ) );
		break;
	case BB_MT6890:		//MTK 5G оƬ, T750 ��Ƭ
		csp_BB_Name->Format( _T( "MT6890" ) );
		break;
	case BB_WCN3615:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN3615" ) );
		break;
	case BB_EC616S:		//�Ϻ���о ���ͳɱ������͹��ĺͳ��߼��ɶ� NB-IoT SoC оƬ, ��ȫ֧��3GPP Rel14 NB-IoT ��׼
		csp_BB_Name->Format( _T( "EC616S" ) );
		break;
	case BB_QCM6125:	//��ͨ ����ģ�� Qualcomm Snapdragon 600 series processor and LTE modem. LTE Cat12/Cat13 (FDD and TDD)
		csp_BB_Name->Format( _T( "QCM6125" ) );
		break;
	case BB_UIS8811:	//չ�� advanced system-on-chip solution integrated NB-IoT modems. The UIS8811 integrates RF transceiver for low cost.
		csp_BB_Name->Format( _T( "UIS8811" ) );
		break;
	case BB_SDX12:		//��ͨ4GоƬ, ��ͨMDM9x50 cost down ƽ̨, Sixthgeneration LTE multimode modem, 14nm system on chip (SoC)
		csp_BB_Name->Format( _T( "SDX12" ) );
		break;
	case BB_QCM2290:	//��ͨ ����ģ�� 4GоƬ
		csp_BB_Name->Format( _T( "QCM2290" ) );
		break;
	case BB_BK7231:		//��������˫ģ5.1��WiFi802.11n�� WiFi SOC оƬ
		csp_BB_Name->Format( _T( "BK7231" ) );
		break;
	case BB_SDX62:		//��ͨ 5G оƬ, ����, SDX55����һ��, Provides Higher Performance mmWave and Features for Global Deployment
		csp_BB_Name->Format( _T( "SDX62" ) );
		break;
	case BB_QCA2064:	//��ͨ highly integrated System-On-Chip (SoC) supporting 802.11ax Wi-Fi and Bluetooth (BT) Milan. It is part of the QCA206x family of SoC��s supporting operation on 2.4 GHz and 5 GHz.
		csp_BB_Name->Format( _T( "QCA2064" ) );
		break;
	case BB_SM4350:		//��ͨ 5G ����оƬ, the new generation Qualcomm Snapdragon 400 series processor with integrated 5G modem.
		csp_BB_Name->Format( _T( "SM4350" ) );
		break;
	case BB_ASR1803S:	//ASR��4GоƬ CAT-4, ASR1802S Costdown оƬ, RTOS�汾
		csp_BB_Name->Format( _T( "ASR1803S" ) );
		break;
	case BB_MT2735:		//MTK 5G оƬ, supporting NR Sub6, LTE Cat19 4x4 MIMO
		csp_BB_Name->Format( _T( "MT2735" ) );
		break;
	case BB_WCN3991:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN3991" ) );
		break;
	case BB_SL8521E:	//չ��(Spreadtrum)4G����ģ��оƬ, ����ģ��ϵͳ��Linux, Android4.4/Linux
		csp_BB_Name->Format( _T( "SL8521E" ) );
		break;
	case BB_QCM6490:	//��ͨ 5G Smart ��������ģ��, ����5G�汾, ���ڳ�����Tbox���ϵ�һ���. including global multi-gigabit 5G mmW/Sub-6 GHz and WiFi 6E
		csp_BB_Name->Format( _T( "QCM6490" ) );
		break;
	case BB_QCA9377:	//��ͨ WLAN/Bluetooth оƬ, a single-die wireless local area network (WLAN) and Bluetooth combination solution to support 1 �� 1 802.11a/b/g/n/ac WLAN standards and Bluetooth 5.0 + HS, designed to deliver superior integration of WLAN/Bluetooth and low-energy technology.
		csp_BB_Name->Format( _T( "QCA9377" ) );
		break;
	case BB_CYW54591:	//The Cypress CYW54591 is a complete dual-band (2.4 GHz and 5 GHz) 5G Wi-Fi 2��2 MIMO MAC/PHY/Radio system-on-Chip.
		csp_BB_Name->Format( _T( "CYW54591" ) );
		break;
	case BB_ASR1803E:	//ASR��4GоƬ CAT-4, ASR1802S Costdown оƬ, Open Linux�汾
		csp_BB_Name->Format( _T( "ASR1803E" ) );
		break;
	case BB_ASR1803L:	//ASR��4GоƬ CAT-4, ASR1802S Costdown оƬ, ֧��"Open Linux�汾" or "RTOS�汾"
		csp_BB_Name->Format( _T( "ASR1803L" ) );
		break;
	case BB_WCN3998:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN3998" ) );
		break;
	case BB_QCM4290:	//��ͨ4Gƽ̨, The QCM4290 includes the next generation of the Qualcomm Snapdragon 600 series processor and LTE modem.
		csp_BB_Name->Format( _T( "QCM4290" ) );
		break;
	case BB_QCX315:		//��ͨ5Gƽ̨, The QCX315 device is a 5G modem from Qualcomm Technologies, Inc. (QTI). It implements the 5G NR standard for sub-6 GHz bands.
		csp_BB_Name->Format( _T( "QCX315" ) );
		break;
	case BB_QCS8250:	//��ͨ QCS8250 is the new generation Qualcomm? Snapdragon? premium-tier processor with robotic application.
		csp_BB_Name->Format( _T( "QCS8250" ) );
		break;
	case BB_CYW43455:	//The Cypress CYW43455 single-chip device provides the highest level of integration for Internet of Things applications and handheld wireless system with integrated single-stream IEEE 802.11ac MAC/baseband/radio and, Bluetooth 5.0.
		csp_BB_Name->Format( _T( "CYW43455" ) );
		break;
	case BB_UIS8310AM:	//չ�� UIS8310AM is a highly integrated application processor with embedded TDD-LTE, FDD-LTE, WCDMA and GSM/GPRS/EDGE modem and LPDDR2. The specially optimized architecture of UIS8310 can achieve high performance and low power for a lot of applications.
		csp_BB_Name->Format( _T( "UIS8310AM" ) );
		break;
	case BB_SDX65:		//��ͨ 5G оƬ, ����, The SDX65/SDX62 device is a multimode modem chip, supporting 2G/3G/4G/5G technologies. 4nm premium-tier 5G modem, 3G/4G/5G modem �CmmWave (mmW) and sub-6 GHz bands (3GPP Release 16 integrated modem).
		csp_BB_Name->Format( _T( "SDX65" ) );
		break;
	case BB_UWE5623DU:	//չ�� WiFi/���� оƬ, UWE5623DU is a high intergrated 2-in-1 connectivity single chip, this chip includes 2.4GHz and 5GHz WLAN IEEE 802.11 a/b/g/n/ac 2x2 MU-MIMO 20/40/80 MHz VHT R2 MAC/PHY/Radio, Bluetooth 5.1.
		csp_BB_Name->Format( _T( "UWE5623DU" ) );
		break;
	case BB_EC618:		//�Ϻ���о, Eigencomm, Cat1оƬ. EC618 is a Cat.1bis SoC chipset with ultra-low cost, ultra-low power and ultra-high integration, it is fully compliant to 3GPP Rel14 Cat.1bis standard��and compatible with 4G commercial networks.
		csp_BB_Name->Format( _T( "EC618" ) );
		break;
	case BB_EFR32BG21:	//о�ƿƼ�(Silicon Labs), ����+LoRa��оƬ, EFR32BG21 Blue Gecko Wireless SoC Family.
		csp_BB_Name->Format( _T( "EFR32BG21" ) );
		break;
	case BB_CYW43438:	//The Cypress CYW43438L is a highly integrated single-chip solution and offers the lowest RBOM in the industry for smartphones, tablets, and a wide range of other portable devices.
		csp_BB_Name->Format( _T( "CYW43438" ) );
		break;
	case BB_UIS8850DG:	//չ�� UIS8850DG is a highly integrated system on chip solution with embedded LTE Cat1bis modems, GNSS modem, RF transceiver, power management unit(PMU), Trust zone and Multi-Function Crypto Engine(CE).
		csp_BB_Name->Format( _T( "UIS8850DG" ) );
		break;
	case BB_CYW8X072:	//The Cypress CYW8X072 is a monolithic, single-chip, Bluetooth 4.2 compliant, stand-alone baseband processor with an integrated 2.4 GHz transceiver.
		csp_BB_Name->Format( _T( "CYW8X072" ) );
		break;
	case BB_WCN3950:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN3950" ) );
		break;
	case BB_W155S1:		//Amlogic W155S1 is an integrated Wi-Fi and Bluetooth combo chip. WiFi&BTоƬ, ֧��802.11_a/b/g/n/ac@WLAN2.4G&5G, BT5.0��BLE��BDR/EDR��.
		csp_BB_Name->Format( _T( "W155S1" ) );
		break;
	case BB_CYW43439:	//The Cypress CYW43439 is a highly integrated single-chip solution and offers the lowest RBOM in the industry for smartphones,tablets, and a wide range of other portable devices.
		csp_BB_Name->Format( _T( "CYW43439" ) );
		break;
	case BB_CB0201:		//��˼ CB0201 ��һ�˼ƽ̨�͹��ĵ�NB-IOTоƬ��֧��3GPP R13/R14/R15 NB-IoT��֧��Rel-16��������ͬʱ֧��BLE/GNSS��
		csp_BB_Name->Format( _T( "CB0201" ) );
		break;
	case BB_ASR1606C:	//ASR��LTE CAT1/MоƬ, ASR1606 is a highly cost-efficient System on Chip (SoC) device.��4M RAM+ 4M ROM��
		csp_BB_Name->Format( _T( "ASR1606C" ) );
		break;
	case BB_ASR1606S:	//ASR��LTE CAT1/MоƬ, ASR1606 is a highly cost-efficient System on Chip (SoC) device.��8M RAM+ 8M ROM��
		csp_BB_Name->Format( _T( "ASR1606S" ) );
		break;
	case BB_WCN6856:	//��ͨWiFiоƬ
		csp_BB_Name->Format( _T( "WCN6856" ) );
		break;
	case BB_QCX216:		//��ͨ compact, low power, low-cost LTE Cat.1bis chip, WiFi2.4GHz Rx.
		csp_BB_Name->Format( _T( "QCX216" ) );
		break;
	case BB_QCA206x:	//��ͨ The Qualcomm QCA206x family consist of System-On-Chips (SoC), Bluetooth Milan and Wi-Fi supported, supporting simultaneous operation on 2.4 GHz, 5 GHz, and 6GHz*, also known as Dual Band Simultaneous (DBS). Part Number(s): QCA2065, QCA2066, QCA2062, QCA2064.
		csp_BB_Name->Format( _T( "QCA206x" ) );
		break;
	case BB_ASR1828:	//ASR��4GоƬ, LTE CAT-6, 1826�ĵ�����Ʒ.
		csp_BB_Name->Format( _T( "ASR1828" ) );
		break;
	default:
		csp_BB_Name->Format( _T( "NDF" ) );
		break;
	}//switch( i_BB_Index
}

void CGFC::Get_Produ_Type_Name( int i_Produ_Type_Index, CString * csp_Produ_Type_Name )
{
	//��Ʒ���͵�index�� ת��Ϊ��Ʒ������

	switch( i_Produ_Type_Index )
	{
	case PRODU_TYPE_PCBA:		//PCBA
		csp_Produ_Type_Name->Format( _T( "PCBA" ) );
		break;
	case PRODU_TYPE_MINIPCIE:	//MINI-PCIE
		csp_Produ_Type_Name->Format( _T( "MINI-PCIE" ) );
		break;
	case PRODU_TYPE_TE_A:		//TE-A
		csp_Produ_Type_Name->Format( _T( "TE-A" ) );
		break;
	case PRODU_TYPE_TE_C:		//TE-C
		csp_Produ_Type_Name->Format( _T( "TE-C" ) );
		break;
	case PRODU_TYPE_TE_B:		//TE-B
		csp_Produ_Type_Name->Format( _T( "TE-B" ) );
		break;
	case PRODU_TYPE_KIT:		//KIT
		csp_Produ_Type_Name->Format( _T( "KIT" ) );
		break;
	case PRODU_TYPE_MOB:		//Module On Board
		csp_Produ_Type_Name->Format( _T( "MOB" ) );
		break;
	case PRODU_TYPE_COB:		//Chip On Board
		csp_Produ_Type_Name->Format( _T( "COB" ) );
		break;
	case PRODU_TYPE_ASM:		//Assembly ����
		csp_Produ_Type_Name->Format( _T( "ASM" ) );
		break;
	default:
		csp_Produ_Type_Name->Format( _T( "NDF" ) );
		break;
	}//switch( i_Produ_Type_Index
}

void CGFC::Get_Produ_Ver_Name( int i_Produ_Ver_Index, CString * csp_Produ_Ver_Name )
{
	//��Ʒ�汾��index�� ת��Ϊ��Ʒ�汾��

	switch( i_Produ_Ver_Index )
	{
	case PRODU_VER_STD://��׼��
		csp_Produ_Ver_Name->Format( _T( "STD" ) );
		break;
	case PRODU_VER_GW://������
		csp_Produ_Ver_Name->Format( _T( "GW" ) );
		break;
	case PRODU_VER_NW://
		csp_Produ_Ver_Name->Format( _T( "NW" ) );
		break;
	case PRODU_VER_ING://
		csp_Produ_Ver_Name->Format( _T( "ING" ) );
		break;
	case PRODU_VER_ZJXD://
		csp_Produ_Ver_Name->Format( _T( "ZJXD" ) );
		break;
	case PRODU_VER_ZJ01://
		csp_Produ_Ver_Name->Format( _T( "ZJ01" ) );
		break;
	case PRODU_VER_HIK://
		csp_Produ_Ver_Name->Format( _T( "HIK" ) );
		break;
	case PRODU_VER_KT://
		csp_Produ_Ver_Name->Format( _T( "KT" ) );
		break;
	case PRODU_VER_SKT://
		csp_Produ_Ver_Name->Format( _T( "SKT" ) );
		break;
	case PRODU_VER_CM://
		csp_Produ_Ver_Name->Format( _T( "CM" ) );
		break;
	default:
		csp_Produ_Ver_Name->Format( _T( "NDF" ) );
		break;
	}
}

long CGFC::HEX_WCHAR_TO_LONG_INT( const wchar_t * wcp_HEX_WCHAR )
{
	//16�����ַ�����ת�����Σ�long��

	wchar_t wcrData[128];
	wchar_t * d;
	long lInt_Data;

	//
	memset( wcrData, 0, sizeof( wcrData ) );

	//
	swprintf_s( wcrData, _T( "0X%s" ), wcp_HEX_WCHAR );

	//ת��ֵ
	lInt_Data = wcstol( wcrData, &d, 16 );

	//����
	return lInt_Data;
}

bool CGFC::Calcu_Center_Freq_WiFi( int i_WiFi_CH, double * dp_Center_Freq_MHz )
{
	if( i_WiFi_CH > 14 )//5G
	{
		*dp_Center_Freq_MHz = 5 * i_WiFi_CH + 5000.0;
	}
	else if( (i_WiFi_CH >= 1) && (i_WiFi_CH <= 14) )//2.4G
	{
		*dp_Center_Freq_MHz = 5 * (i_WiFi_CH - 1) + 2412.0;
	}
	else
	{
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_WiFi_FreqBand_Name( int i_WiFi_FB_Index, CString * csp_WiFi_FB_Name )
{
	switch( i_WiFi_FB_Index )
	{
	case WIFI_FB_B24G://2.4G
		csp_WiFi_FB_Name->Format( _T( "2.4G" ) );
		break;
	case WIFI_FB_B5G://5G
		csp_WiFi_FB_Name->Format( _T( "5G" ) );
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_WiFi_FreqBand_Name, WiFi_FreqBand_Index(%d) is wrong!" ), i_WiFi_FB_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_WiFi_Standard_Name( int i_WiFi_STAN_Index, CString * csp_WiFi_STAN_Name )
{
	//��ȡWiFi��׼��
	switch( i_WiFi_STAN_Index )
	{
	case WIFI_STAN_80211A:
		csp_WiFi_STAN_Name->Format( _T( "802.11a" ) );
		break;
	case WIFI_STAN_80211B:
		csp_WiFi_STAN_Name->Format( _T( "802.11b" ) );
		break;
	case WIFI_STAN_80211G:
		csp_WiFi_STAN_Name->Format( _T( "802.11g" ) );
		break;
	case WIFI_STAN_80211N_20M:
		csp_WiFi_STAN_Name->Format( _T( "802.11n.20M" ) );
		break;
	case WIFI_STAN_80211N_40M:
		csp_WiFi_STAN_Name->Format( _T( "802.11n.40M" ) );
		break;
	case WIFI_STAN_80211N_5G_20M:
		csp_WiFi_STAN_Name->Format( _T( "802.11n.5G.20M" ) );
		break;
	case WIFI_STAN_80211AC_20M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ac.20M" ) );
		break;
	case WIFI_STAN_80211AC_40M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ac.40M" ) );
		break;
	case WIFI_STAN_80211AC_80M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ac.80M" ) );
		break;
	case WIFI_STAN_80211AX_20M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ax.20M" ) );
		break;
	case WIFI_STAN_80211AX_40M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ax.40M" ) );
		break;
	case WIFI_STAN_80211AX_5G_20M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ax.5G.20M" ) );
		break;
	case WIFI_STAN_80211AX_5G_40M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ax.5G.40M" ) );
		break;
	case WIFI_STAN_80211AX_5G_80M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ax.5G.80M" ) );
		break;
	case WIFI_STAN_80211N_5G_40M:
		csp_WiFi_STAN_Name->Format( _T( "802.11n.5G.40M" ) );
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_WiFi_Standard_Name, WiFi_Standard_Index(%d) is wrong!" ), i_WiFi_STAN_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_WiFi_STAN_Index

	//pass
	return true;
}

bool CGFC::Calcu_Center_Freq_Bluetooth( int i_BLU_VER_Index, int i_BLU_CH, double * dp_Center_Freq_MHz )
{
	switch( i_BLU_VER_Index )
	{
	case BLU_VER_10_BR:
	case BLU_VER_20_EDR://����1M
		{
			if( (i_BLU_CH >= 0) && (i_BLU_CH <= 78) )
			{
				*dp_Center_Freq_MHz = 2402.0 + i_BLU_CH * 1.0;
			}
			else
			{
				this->m_csError_Info.Format( _T( "ERROR, Calcu_Center_Freq_Bluetooth, BLU_CH(%d) is wrong!" ), i_BLU_CH );
				::AfxMessageBox( this->m_csError_Info );
				//fail
				return false;
			}
		}
		break;

	case BLU_VER_40_BLE://����2M
		{
			if( (i_BLU_CH >= 0) && (i_BLU_CH <= 39) )
			{
				*dp_Center_Freq_MHz = 2402.0 + i_BLU_CH * 2.0;
			}
			else
			{
				this->m_csError_Info.Format( _T( "ERROR, Calcu_Center_Freq_Bluetooth, BLU_CH(%d) is wrong!" ), i_BLU_CH );
				::AfxMessageBox( this->m_csError_Info );
				//fail
				return false;
			}
		}
		break;

	default:
		this->m_csError_Info.Format( _T( "ERROR, Calcu_Center_Freq_Bluetooth, BLU_VER_Index(%d) is wrong!" ), i_BLU_VER_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_Bluetooth_Version_Name( int i_BLU_VER_Index, CString * csp_BLU_VER_Name )
{
	switch( i_BLU_VER_Index )
	{
	case BLU_VER_10_BR:
		csp_BLU_VER_Name->Format( _T( "BR" ) );
		break;
	case BLU_VER_20_EDR:
		csp_BLU_VER_Name->Format( _T( "EDR" ) );
		break;
	case BLU_VER_40_BLE:
		csp_BLU_VER_Name->Format( _T( "BLE" ) );
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_Bluetooth_Version_Name, BLU_VER_Index(%d) is wrong!" ), i_BLU_VER_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}
bool CGFC::Get_Local_PC_IP_Name(CString * csp_PC_IP)
{
	WORD wVersionRequested;
	WSADATA wsaData ;
	char crData[64];
	char *crIpData=NULL;
	wchar_t wcrTemp[64];

	memset(crData, 0, sizeof(crData));
	memset(wcrTemp, 0, sizeof(wcrTemp));

	//Get Computer Name
	wVersionRequested = MAKEWORD(2, 0);
	WSAStartup(wVersionRequested, &wsaData);

	if(::gethostname(crData, 60) != 0)
	{
		memset(crData, 0, sizeof(crData));
	}
	if(strlen(crData) > 0)
	{
		struct hostent *p=::gethostbyname(crData);
		if(p!=NULL)
		{
			struct in_addr in;
			for(int i=0;p->h_addr_list[i]!=0;i++)
			{
				memcpy(&in,p->h_addr_list[i],sizeof(struct in_addr));
				crIpData=inet_ntoa(in);
				this->MultiByte_To_WideChar(crIpData, wcrTemp, &i);
				csp_PC_IP->Format(_T("%s"), wcrTemp);
				WSACleanup();
				return true;
			}
		}
		else
		{
			WSACleanup();
			return false;
		}

	}
	else
	{
		WSACleanup();
		return false;
	}
	return false;
}

bool CGFC::Get_FileName_From_FilePath( const wchar_t * wcp_File_Path, bool b_Need_Extend_Name, CString * csp_File_Name )
{
	int iWhere;
	CString csFile_Path, csFileTitle;


	//
	try
	{
		//�����ļ�·��
		csFile_Path.Format( _T( "%s" ), wcp_File_Path );

		//
		iWhere = csFile_Path.ReverseFind( '\\' );
		//�ж�
		if( iWhere == -1 )//�C1 if the character is not found
		{
			//fail
			throw(0);
		}


		//��ȡ�ļ���
		csFileTitle = csFile_Path.Right( csFile_Path.GetLength() - 1 - iWhere );


		//ȥ����չ��
		if( b_Need_Extend_Name == false )
		{
			iWhere = csFileTitle.ReverseFind( '.' );
			//
			if( iWhere != -1 )
			{
				//��������չ����ȥ����չ��
				csFileTitle = csFileTitle.Left( iWhere );
			}
		}


		//��������
		csp_File_Name->Format( _T( "%s" ), csFileTitle );

	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}


void CGFC::WCSTOK_STRING_S_C_Ex( const wchar_t * wcp_StrToken, wchar_t wc_StrDelimit, CString * csp_Get_CString_Array, int * ip_Get_Quan )
{
	//�˺�����ԡ�ͷβ�ָ������͡����������ָ�������Ŀ�����Ҳ�����ͳ�ƣ���Ϊһ�����ա��ַ�������

	int i_Index;
	CString csStrToken;			//Դ������ʱ���
	CStringArray csaTempArray;	//Ŀ���ַ�����ʱ���
	CString csTemp, csStrDelimit;


	//1�����롰Դ���ݡ�
	csStrToken.Format( _T( "%s" ), wcp_StrToken );
	csStrDelimit.Format( _T( "%c" ), wc_StrDelimit );

	//2��ȷ���ж��ٸ�����
	*ip_Get_Quan = csStrToken.Replace( csStrDelimit, csStrDelimit ) + (csStrToken.IsEmpty() ? 0 : 1);

	//3�������ʱArray
	csaTempArray.RemoveAll();

	//4����������
	while( TRUE )//��Դ����Ϊ���ա�Ҳ�������˳�ѭ��
	{
		i_Index = csStrToken.Find( csStrDelimit );
		//
		if( i_Index == -1 )
		{
			//���һ��������
			csaTempArray.Add( csStrToken );
			//�˳�while
			break;
		}

		csTemp = csStrToken.Left( i_Index );

		csaTempArray.Add( csTemp );

		csStrToken = csStrToken.Right( csStrToken.GetLength() - i_Index - 1 );
	}

	//5����ʱ��ֵ��Ŀ��
	for( int i = 0; i < *ip_Get_Quan; i++ )
	{
		(csp_Get_CString_Array + i)->Format( _T( "%s" ), csaTempArray.GetAt( i ) );
	}
}


bool CGFC::Get_WiFi_Standard_FreqBand( int i_WiFi_STAN_Index, int * ip_WiFi_FB_Index )
{
	//��ȡWiFi��׼��WiFiƵ��
	switch( i_WiFi_STAN_Index )
	{
		//2.4G
	case WIFI_STAN_80211B:
	case WIFI_STAN_80211G:
	case WIFI_STAN_80211N_20M:
	case WIFI_STAN_80211N_40M:
	case WIFI_STAN_80211AX_20M:
	case WIFI_STAN_80211AX_40M:
		*ip_WiFi_FB_Index = WIFI_FB_B24G;
		//
		break;

		//5G
	case WIFI_STAN_80211A:
	case WIFI_STAN_80211N_5G_20M:
	case WIFI_STAN_80211N_5G_40M:
	case WIFI_STAN_80211AC_20M:
	case WIFI_STAN_80211AC_40M:
	case WIFI_STAN_80211AC_80M:
	case WIFI_STAN_80211AX_5G_20M:
	case WIFI_STAN_80211AX_5G_40M:
	case WIFI_STAN_80211AX_5G_80M:
		*ip_WiFi_FB_Index = WIFI_FB_B5G;
		//
		break;

	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_WiFi_Standard_FreqBand, WiFi_Standard_Index(%d) is wrong!" ), i_WiFi_STAN_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_LTE_Category_Name( int i_LTE_CAT_Index, CString * csp_LTE_CAT_Name )
{
	//��ȡ LTE_CAT �ַ�������
	switch( i_LTE_CAT_Index )
	{
	case LTE_CAT_NDF:
		csp_LTE_CAT_Name->Format( _T( "CAT-NDF" ) );
		break;
	case LTE_CAT_0:
		csp_LTE_CAT_Name->Format( _T( "CAT-0" ) );
		break;
	case LTE_CAT_1:
		csp_LTE_CAT_Name->Format( _T( "CAT-1" ) );
		break;
	case LTE_CAT_2:
		csp_LTE_CAT_Name->Format( _T( "CAT-2" ) );
		break;
	case LTE_CAT_3:
		csp_LTE_CAT_Name->Format( _T( "CAT-3" ) );
		break;
	case LTE_CAT_4:
		csp_LTE_CAT_Name->Format( _T( "CAT-4" ) );
		break;
	case LTE_CAT_5:
		csp_LTE_CAT_Name->Format( _T( "CAT-5" ) );
		break;
	case LTE_CAT_6:
		csp_LTE_CAT_Name->Format( _T( "CAT-6" ) );
		break;
	case LTE_CAT_7:
		csp_LTE_CAT_Name->Format( _T( "CAT-7" ) );
		break;
	case LTE_CAT_8:
		csp_LTE_CAT_Name->Format( _T( "CAT-8" ) );
		break;
	case LTE_CAT_9:
		csp_LTE_CAT_Name->Format( _T( "CAT-9" ) );
		break;
	case LTE_CAT_10:
		csp_LTE_CAT_Name->Format( _T( "CAT-10" ) );
		break;
	case LTE_CAT_M:
		csp_LTE_CAT_Name->Format( _T( "CAT-M" ) );
		break;
	case LTE_CAT_NB:
		csp_LTE_CAT_Name->Format( _T( "CAT-NB" ) );
		break;
	case LTE_CAT_11:
		csp_LTE_CAT_Name->Format( _T( "CAT-11" ) );
		break;
	case LTE_CAT_12:
		csp_LTE_CAT_Name->Format( _T( "CAT-12" ) );
		break;
	case LTE_CAT_18:
		csp_LTE_CAT_Name->Format( _T( "CAT-18" ) );
		break;
	case LTE_CAT_16:
		csp_LTE_CAT_Name->Format( _T( "CAT-16" ) );
		break;
	case LTE_CAT_20:
		csp_LTE_CAT_Name->Format( _T( "CAT-20" ) );
		break;
	case LTE_CAT_19:
		csp_LTE_CAT_Name->Format( _T( "CAT-19" ) );
		break;
	case LTE_CAT_15:
		csp_LTE_CAT_Name->Format( _T( "CAT-15" ) );
		break;
	case LTE_CAT_13:
		csp_LTE_CAT_Name->Format( _T( "CAT-13" ) );
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_LTE_Category_Name, LTE_CAT_Index(%d) is wrong!" ), i_LTE_CAT_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_LTE_CAT_Index

	//pass
	return true;
}


bool CGFC::Get_GSM_Band_Name( int i_G_Band_Index, CString * csp_G_Band_Name )
{
	switch( i_G_Band_Index )
	{
	case G_BAND_GSM900:
		csp_G_Band_Name->Format( _T( "GSM900" ) );
		break;
	case G_BAND_DCS1800:
		csp_G_Band_Name->Format( _T( "DCS1800" ) );
		break;
	case G_BAND_PCS1900:
		csp_G_Band_Name->Format( _T( "PCS1900" ) );
		break;
	case G_BAND_GSM850:
		csp_G_Band_Name->Format( _T( "GSM850" ) );
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_GSM_Band_Name, GSM_Band_Index(%d) is wrong!" ), i_G_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}


int CGFC::Get_Process_Attributes_Index( const CString cs_Process_Attributes_Name )
{
	CString csProcess_Attributes_Name;


	//����������������ͳһ��д
	csProcess_Attributes_Name.Format( _T( "%s" ), cs_Process_Attributes_Name );
	csProcess_Attributes_Name.MakeUpper();


	//ʶ��������������
	if( csProcess_Attributes_Name.Find( _T( "ALL_TEST" ) ) != -1 )//ȫ������
	{
		return PROCESS_ATT_ALL_TEST;
	}
	else if( csProcess_Attributes_Name.Find( _T( "ONLY_LABEL" ) ) != -1 )//ֻ���ǩ
	{
		return PROCESS_ATT_ONLY_LABEL;
	}
	else if( csProcess_Attributes_Name.Find( _T( "TO_DL" ) ) != -1 )//ֻ����DL
	{
		return PROCESS_ATT_TO_DL;
	}
	else if( csProcess_Attributes_Name.Find( _T( "TO_BT" ) ) != -1 )//ֻ����BT
	{
		return PROCESS_ATT_TO_BT;
	}
	else if( csProcess_Attributes_Name.Find( _T( "TO_FT" ) ) != -1 )//ֻ����FT
	{
		return PROCESS_ATT_TO_FT;
	}
	else if( csProcess_Attributes_Name.Find( _T( "TO_FCT" ) ) != -1 )//ֻ����FCT
	{
		return PROCESS_ATT_TO_FCT;
	}
	else
	{
	}

	//fail info
	this->m_csError_Info.Format( _T( "ERROR, Get_Process_Attributes_Index, \"%s\" is wrong!" ), csProcess_Attributes_Name );
	::AfxMessageBox( this->m_csError_Info );
	//fail
	return PROCESS_ATT_ERROR;
}


bool CGFC::Get_Process_Attributes_Name( const int i_Process_Attributes_Index, CString & csref_Process_Attributes )
{
	switch( i_Process_Attributes_Index )
	{
	case PROCESS_ATT_ERROR:
		csref_Process_Attributes.Format( _T( "ERROR" ) );
		break;
	case PROCESS_ATT_ALL_TEST:
		csref_Process_Attributes.Format( _T( "All_Test" ) );
		break;
	case PROCESS_ATT_ONLY_LABEL:
		csref_Process_Attributes.Format( _T( "Only_Label" ) );
		break;
	case PROCESS_ATT_TO_DL:
		csref_Process_Attributes.Format( _T( "To_DL" ) );
		break;
	case PROCESS_ATT_TO_BT:
		csref_Process_Attributes.Format( _T( "To_BT" ) );
		break;
	case PROCESS_ATT_TO_FT:
		csref_Process_Attributes.Format( _T( "To_FT" ) );
		break;
	case PROCESS_ATT_TO_FCT:
		csref_Process_Attributes.Format( _T( "To_FCT" ) );
		break;
	default:
		csref_Process_Attributes.Format( _T( "ERROR" ) );
		//fail info
		this->m_csError_Info.Format( _T( "ERROR, Get_Process_Attributes_Name, Process_Attributes_Index(%d) is wrong!" ), i_Process_Attributes_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NBIoT_Band_Index( int i_NBIoT_Band_Name, int * ip_NBIoT_Band_Index )
{
	//NB-IoTƵ���� ת�� NB-IoTƵ��index��
	//NB-IoT�¼�Ƶ�δ���λ��

	//
	switch( i_NBIoT_Band_Name )
	{
	case NB_BAND_1://NB_IDX_B_1
		*ip_NBIoT_Band_Index = NB_IDX_B_1;
		break;
	case NB_BAND_2://NB_IDX_B_2
		*ip_NBIoT_Band_Index = NB_IDX_B_2;
		break;
	case NB_BAND_3://NB_IDX_B_3
		*ip_NBIoT_Band_Index = NB_IDX_B_3;
		break;
	case NB_BAND_4://NB_IDX_B_4
		*ip_NBIoT_Band_Index = NB_IDX_B_4;
		break;
	case NB_BAND_5://NB_IDX_B_5
		*ip_NBIoT_Band_Index = NB_IDX_B_5;
		break;
	case NB_BAND_7://NB_IDX_B_7
		*ip_NBIoT_Band_Index = NB_IDX_B_7;
		break;
	case NB_BAND_8://NB_IDX_B_8
		*ip_NBIoT_Band_Index = NB_IDX_B_8;
		break;
	case NB_BAND_9://NB_IDX_B_9
		*ip_NBIoT_Band_Index = NB_IDX_B_9;
		break;
	case NB_BAND_10://NB_IDX_B_10
		*ip_NBIoT_Band_Index = NB_IDX_B_10;
		break;
	case NB_BAND_11://NB_IDX_B_11
		*ip_NBIoT_Band_Index = NB_IDX_B_11;
		break;
	case NB_BAND_12://NB_IDX_B_12
		*ip_NBIoT_Band_Index = NB_IDX_B_12;
		break;
	case NB_BAND_13://NB_IDX_B_13
		*ip_NBIoT_Band_Index = NB_IDX_B_13;
		break;
	case NB_BAND_14://NB_IDX_B_14
		*ip_NBIoT_Band_Index = NB_IDX_B_14;
		break;
	case NB_BAND_17://NB_IDX_B_17
		*ip_NBIoT_Band_Index = NB_IDX_B_17;
		break;
	case NB_BAND_18://NB_IDX_B_18
		*ip_NBIoT_Band_Index = NB_IDX_B_18;
		break;
	case NB_BAND_19://NB_IDX_B_19
		*ip_NBIoT_Band_Index = NB_IDX_B_19;
		break;
	case NB_BAND_20://NB_IDX_B_20
		*ip_NBIoT_Band_Index = NB_IDX_B_20;
		break;
	case NB_BAND_21://NB_IDX_B_21
		*ip_NBIoT_Band_Index = NB_IDX_B_21;
		break;
	case NB_BAND_22://NB_IDX_B_22
		*ip_NBIoT_Band_Index = NB_IDX_B_22;
		break;
	case NB_BAND_24://NB_IDX_B_24
		*ip_NBIoT_Band_Index = NB_IDX_B_24;
		break;
	case NB_BAND_25://NB_IDX_B_25
		*ip_NBIoT_Band_Index = NB_IDX_B_25;
		break;
	case NB_BAND_26://NB_IDX_B_26
		*ip_NBIoT_Band_Index = NB_IDX_B_26;
		break;
	case NB_BAND_27://NB_IDX_B_27
		*ip_NBIoT_Band_Index = NB_IDX_B_27;
		break;
	case NB_BAND_28://NB_IDX_B_28
		*ip_NBIoT_Band_Index = NB_IDX_B_28;
		break;
	case NB_BAND_30://NB_IDX_B_30
		*ip_NBIoT_Band_Index = NB_IDX_B_30;
		break;
	case NB_BAND_31://NB_IDX_B_31
		*ip_NBIoT_Band_Index = NB_IDX_B_31;
		break;
	case NB_BAND_66://NB_IDX_B_66
		*ip_NBIoT_Band_Index = NB_IDX_B_66;
		break;
	case NB_BAND_68://NB_IDX_B_68
		*ip_NBIoT_Band_Index = NB_IDX_B_68;
		break;
	case NB_BAND_70://NB_IDX_B_70
		*ip_NBIoT_Band_Index = NB_IDX_B_70;
		break;
	case NB_BAND_71://NB_IDX_B_71
		*ip_NBIoT_Band_Index = NB_IDX_B_71;
		break;
	case NB_BAND_72://NB_IDX_B_72
		*ip_NBIoT_Band_Index = NB_IDX_B_72;
		break;
	case NB_BAND_73://NB_IDX_B_73
		*ip_NBIoT_Band_Index = NB_IDX_B_73;
		break;
	case NB_BAND_74://NB_IDX_B_74
		*ip_NBIoT_Band_Index = NB_IDX_B_74;
		break;
	case NB_BAND_85://NB_IDX_B_85
		*ip_NBIoT_Band_Index = NB_IDX_B_85;
		break;
	case NB_BAND_86://NB_IDX_B_86
		*ip_NBIoT_Band_Index = NB_IDX_B_86;
		break;
	case NB_BAND_87://NB_IDX_B_87
		*ip_NBIoT_Band_Index = NB_IDX_B_87;
		break;
	case NB_BAND_88://NB_IDX_B_88
		*ip_NBIoT_Band_Index = NB_IDX_B_88;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_NBIoT_Band_Index, NB-IoT_Band_Name(%d) is wrong!" ), i_NBIoT_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_NBIoT_Band_Name

	//pass
	return true;
}


bool CGFC::Get_NBIoT_Band_Name( int i_NBIoT_Band_Index, int * ip_NBIoT_Band_Name )
{
	//NB-IoTƵ��index�� ת�� NB-IoTƵ����
	//NB-IoT�¼�Ƶ�δ���λ��

	//
	switch( i_NBIoT_Band_Index )
	{
	case NB_IDX_B_1:
		*ip_NBIoT_Band_Name = NB_BAND_1;
		break;
	case NB_IDX_B_2:
		*ip_NBIoT_Band_Name = NB_BAND_2;
		break;
	case NB_IDX_B_3:
		*ip_NBIoT_Band_Name = NB_BAND_3;
		break;
	case NB_IDX_B_4:
		*ip_NBIoT_Band_Name = NB_BAND_4;
		break;
	case NB_IDX_B_5:
		*ip_NBIoT_Band_Name = NB_BAND_5;
		break;
	case NB_IDX_B_7:
		*ip_NBIoT_Band_Name = NB_BAND_7;
		break;
	case NB_IDX_B_8:
		*ip_NBIoT_Band_Name = NB_BAND_8;
		break;
	case NB_IDX_B_9:
		*ip_NBIoT_Band_Name = NB_BAND_9;
		break;
	case NB_IDX_B_10:
		*ip_NBIoT_Band_Name = NB_BAND_10;
		break;
	case NB_IDX_B_11:
		*ip_NBIoT_Band_Name = NB_BAND_11;
		break;
	case NB_IDX_B_12:
		*ip_NBIoT_Band_Name = NB_BAND_12;
		break;
	case NB_IDX_B_13:
		*ip_NBIoT_Band_Name = NB_BAND_13;
		break;
	case NB_IDX_B_14:
		*ip_NBIoT_Band_Name = NB_BAND_14;
		break;
	case NB_IDX_B_17:
		*ip_NBIoT_Band_Name = NB_BAND_17;
		break;
	case NB_IDX_B_18:
		*ip_NBIoT_Band_Name = NB_BAND_18;
		break;
	case NB_IDX_B_19:
		*ip_NBIoT_Band_Name = NB_BAND_19;
		break;
	case NB_IDX_B_20:
		*ip_NBIoT_Band_Name = NB_BAND_20;
		break;
	case NB_IDX_B_21:
		*ip_NBIoT_Band_Name = NB_BAND_21;
		break;
	case NB_IDX_B_22:
		*ip_NBIoT_Band_Name = NB_BAND_22;
		break;
	case NB_IDX_B_24:
		*ip_NBIoT_Band_Name = NB_BAND_24;
		break;
	case NB_IDX_B_25:
		*ip_NBIoT_Band_Name = NB_BAND_25;
		break;
	case NB_IDX_B_26:
		*ip_NBIoT_Band_Name = NB_BAND_26;
		break;
	case NB_IDX_B_27:
		*ip_NBIoT_Band_Name = NB_BAND_27;
		break;
	case NB_IDX_B_28:
		*ip_NBIoT_Band_Name = NB_BAND_28;
		break;
	case NB_IDX_B_30:
		*ip_NBIoT_Band_Name = NB_BAND_30;
		break;
	case NB_IDX_B_31:
		*ip_NBIoT_Band_Name = NB_BAND_31;
		break;
	case NB_IDX_B_66:
		*ip_NBIoT_Band_Name = NB_BAND_66;
		break;
	case NB_IDX_B_68:
		*ip_NBIoT_Band_Name = NB_BAND_68;
		break;
	case NB_IDX_B_70:
		*ip_NBIoT_Band_Name = NB_BAND_70;
		break;
	case NB_IDX_B_71:
		*ip_NBIoT_Band_Name = NB_BAND_71;
		break;
	case NB_IDX_B_72:
		*ip_NBIoT_Band_Name = NB_BAND_72;
		break;
	case NB_IDX_B_73:
		*ip_NBIoT_Band_Name = NB_BAND_73;
		break;
	case NB_IDX_B_74:
		*ip_NBIoT_Band_Name = NB_BAND_74;
		break;
	case NB_IDX_B_85:
		*ip_NBIoT_Band_Name = NB_BAND_85;
		break;
	case NB_IDX_B_86:
		*ip_NBIoT_Band_Name = NB_BAND_86;
		break;
	case NB_IDX_B_87:
		*ip_NBIoT_Band_Name = NB_BAND_87;
		break;
	case NB_IDX_B_88:
		*ip_NBIoT_Band_Name = NB_BAND_88;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_NBIoT_Band_Name, NB-IoT_Band_Index(%d) is Wrong!" ), i_NBIoT_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_NBIoT_Band_Index

	//pass
	return true;
}


bool CGFC::Get_LTE_Band_DD_FDD_TDD( int i_LTE_Band_Name, int & oj_i_LTE_DD )
{
	//
	try
	{
		//FDD:1~32, 65~88 (B29��B32ֻ��DL��ֻ��CA������)��NB-B86��FDD
		if( ((i_LTE_Band_Name >= 1) && (i_LTE_Band_Name <= 32))
			|| ((i_LTE_Band_Name >= 65) && (i_LTE_Band_Name <= 88)) )//FDD:1~32, 65~88 (B29��B32ֻ��DL��ֻ��CA������)��NB-B86��FDD
		{
			oj_i_LTE_DD = DD_FDD;
		}
		else if( (i_LTE_Band_Name >= 33) && (i_LTE_Band_Name <= 53) )//TDD:33~53
		{
			oj_i_LTE_DD = DD_TDD;
		}
		else
		{
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_LTE_Band_DD_FDD_TDD, LTE_Band_Name(%d) is Wrong!" ), i_LTE_Band_Name );
		::AfxMessageBox( this->m_csError_Info );

		//Ĭ�Ϸ���ֵ
		oj_i_LTE_DD = DD_NDF;

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_DD_FDD_TDD_Name( int i_FDD_TDD, CString & oj_cs_DD_Name )
{
	try
	{
		switch( i_FDD_TDD )
		{
		case DD_FDD:
			oj_cs_DD_Name.Format( _T( "FDD" ) );
			break;
		case DD_TDD:
			oj_cs_DD_Name.Format( _T( "TDD" ) );
			break;
		default:
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_DD_FDD_TDD_Name, FDD_TDD(%d) is Wrong!" ), i_FDD_TDD );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NR_5G_SUB6_Band_Index( int i_Band_Name, int & oj_i_Band_Index )
{
	try
	{
		switch( i_Band_Name )
		{
		case 1://NR5GSUB6_N_1
			oj_i_Band_Index = NR5GSUB6_N_1;
			break;
		case 2://NR5GSUB6_N_2
			oj_i_Band_Index = NR5GSUB6_N_2;
			break;
		case 3://NR5GSUB6_N_3
			oj_i_Band_Index = NR5GSUB6_N_3;
			break;
		case 5://NR5GSUB6_N_5
			oj_i_Band_Index = NR5GSUB6_N_5;
			break;
		case 7://NR5GSUB6_N_7
			oj_i_Band_Index = NR5GSUB6_N_7;
			break;
		case 8://NR5GSUB6_N_8
			oj_i_Band_Index = NR5GSUB6_N_8;
			break;
		case 20://NR5GSUB6_N_20
			oj_i_Band_Index = NR5GSUB6_N_20;
			break;
		case 28://NR5GSUB6_N_28
			oj_i_Band_Index = NR5GSUB6_N_28;
			break;
		case 41://NR5GSUB6_N_41
			oj_i_Band_Index = NR5GSUB6_N_41;
			break;
		case 66://NR5GSUB6_N_66
			oj_i_Band_Index = NR5GSUB6_N_66;
			break;
		case 71://NR5GSUB6_N_71
			oj_i_Band_Index = NR5GSUB6_N_71;
			break;
		case 77://NR5GSUB6_N_77
			oj_i_Band_Index = NR5GSUB6_N_77;
			break;
		case 78://NR5GSUB6_N_78
			oj_i_Band_Index = NR5GSUB6_N_78;
			break;
		case 79://NR5GSUB6_N_79
			oj_i_Band_Index = NR5GSUB6_N_79;
			break;
		case 12://NR5GSUB6_N_12
			oj_i_Band_Index = NR5GSUB6_N_12;
			break;
		case 25://NR5GSUB6_N_25
			oj_i_Band_Index = NR5GSUB6_N_25;
			break;
		case 48://NR5GSUB6_N_48
			oj_i_Band_Index = NR5GSUB6_N_48;
			break;
		case 38://NR5GSUB6_N_38
			oj_i_Band_Index = NR5GSUB6_N_38;
			break;
		case 40://NR5GSUB6_N_40
			oj_i_Band_Index = NR5GSUB6_N_40;
			break;
		case 14://NR5GSUB6_N_14
			oj_i_Band_Index = NR5GSUB6_N_14;
			break;
		case 13://NR5GSUB6_N_13
			oj_i_Band_Index = NR5GSUB6_N_13;
			break;
		case 18://NR5GSUB6_N_18
			oj_i_Band_Index = NR5GSUB6_N_18;
			break;
		case 26://NR5GSUB6_N_26
			oj_i_Band_Index = NR5GSUB6_N_26;
			break;
		case 29://NR5GSUB6_N_29
			oj_i_Band_Index = NR5GSUB6_N_29;
			break;
		case 30://NR5GSUB6_N_30
			oj_i_Band_Index = NR5GSUB6_N_30;
			break;
		case 75://NR5GSUB6_N_75
			oj_i_Band_Index = NR5GSUB6_N_75;
			break;
		case 76://NR5GSUB6_N_76
			oj_i_Band_Index = NR5GSUB6_N_76;
			break;
		case 91://NR5GSUB6_N_91
			oj_i_Band_Index = NR5GSUB6_N_91;
			break;
		case 92://NR5GSUB6_N_92
			oj_i_Band_Index = NR5GSUB6_N_92;
			break;
		case 93://NR5GSUB6_N_93
			oj_i_Band_Index = NR5GSUB6_N_93;
			break;
		case 94://NR5GSUB6_N_94
			oj_i_Band_Index = NR5GSUB6_N_94;
			break;
		case 70://NR5GSUB6_N_70
			oj_i_Band_Index = NR5GSUB6_N_70;
			break;
		case 257://NR5GSUB6_N_257
			oj_i_Band_Index = NR5GSUB6_N_257;
			break;
		case 258://NR5GSUB6_N_258
			oj_i_Band_Index = NR5GSUB6_N_258;
			break;
		case 260://NR5GSUB6_N_260
			oj_i_Band_Index = NR5GSUB6_N_260;
			break;
		case 261://NR5GSUB6_N_261
			oj_i_Band_Index = NR5GSUB6_N_261;
			break;
		default:
			//fail
			throw(0);
		}//switch( i_Band_Name

	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_NR_5G_SUB6_Band_Index, Band_Name(%d) is Wrong!" ), i_Band_Name );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NR_5G_SUB6_Band_Name( int i_Band_Index, int & oj_i_Band_Name )
{
	try
	{
		switch( i_Band_Index )
		{
		case NR5GSUB6_N_1:
			oj_i_Band_Name = 1;
			break;
		case NR5GSUB6_N_2:
			oj_i_Band_Name = 2;
			break;
		case NR5GSUB6_N_3:
			oj_i_Band_Name = 3;
			break;
		case NR5GSUB6_N_5:
			oj_i_Band_Name = 5;
			break;
		case NR5GSUB6_N_7:
			oj_i_Band_Name = 7;
			break;
		case NR5GSUB6_N_8:
			oj_i_Band_Name = 8;
			break;
		case NR5GSUB6_N_20:
			oj_i_Band_Name = 20;
			break;
		case NR5GSUB6_N_28:
			oj_i_Band_Name = 28;
			break;
		case NR5GSUB6_N_41:
			oj_i_Band_Name = 41;
			break;
		case NR5GSUB6_N_66:
			oj_i_Band_Name = 66;
			break;
		case NR5GSUB6_N_71:
			oj_i_Band_Name = 71;
			break;
		case NR5GSUB6_N_77:
			oj_i_Band_Name = 77;
			break;
		case NR5GSUB6_N_78:
			oj_i_Band_Name = 78;
			break;
		case NR5GSUB6_N_79:
			oj_i_Band_Name = 79;
			break;
		case NR5GSUB6_N_12:
			oj_i_Band_Name = 12;
			break;
		case NR5GSUB6_N_25:
			oj_i_Band_Name = 25;
			break;
		case NR5GSUB6_N_48:
			oj_i_Band_Name = 48;
			break;
		case NR5GSUB6_N_38:
			oj_i_Band_Name = 38;
			break;
		case NR5GSUB6_N_40:
			oj_i_Band_Name = 40;
			break;
		case NR5GSUB6_N_14:
			oj_i_Band_Name = 14;
			break;
		case NR5GSUB6_N_13:
			oj_i_Band_Name = 13;
			break;
		case NR5GSUB6_N_18:
			oj_i_Band_Name = 18;
			break;
		case NR5GSUB6_N_26:
			oj_i_Band_Name = 26;
			break;
		case NR5GSUB6_N_29:
			oj_i_Band_Name = 29;
			break;
		case NR5GSUB6_N_30:
			oj_i_Band_Name = 30;
			break;
		case NR5GSUB6_N_75:
			oj_i_Band_Name = 75;
			break;
		case NR5GSUB6_N_76:
			oj_i_Band_Name = 76;
			break;
		case NR5GSUB6_N_91:
			oj_i_Band_Name = 91;
			break;
		case NR5GSUB6_N_92:
			oj_i_Band_Name = 92;
			break;
		case NR5GSUB6_N_93:
			oj_i_Band_Name = 93;
			break;
		case NR5GSUB6_N_94:
			oj_i_Band_Name = 94;
			break;
		case NR5GSUB6_N_70:
			oj_i_Band_Name = 70;
			break;
		case NR5GSUB6_N_257:
			oj_i_Band_Name = 257;
			break;
		case NR5GSUB6_N_258:
			oj_i_Band_Name = 258;
			break;
		case NR5GSUB6_N_260:
			oj_i_Band_Name = 260;
			break;
		case NR5GSUB6_N_261:
			oj_i_Band_Name = 261;
			break;
		default:
			//fail
			throw(0);
		}//switch( i_Band_Index

	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_NR_5G_SUB6_Band_Name, Band_Index(%d) is Wrong!" ), i_Band_Index );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_ANT_Type_Name( EU_ANT_TYPE_INDEX eu_Ant_Type_Index, CString & oj_cs_Ant_Type_Name )
{
	//�������� ת ������

	//
	try
	{
		switch( eu_Ant_Type_Index )
		{
		case ANT_TYPE_IDX_MAIN_1:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MAIN_1" ) );
			break;
		case ANT_TYPE_IDX_DIV_1:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_DIV_1" ) );
			break;
		case ANT_TYPE_IDX_GPS:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_GPS" ) );
			break;
		case ANT_TYPE_IDX_WIFI_BLU:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_WIFI_BLU" ) );
			break;
		case ANT_TYPE_IDX_DIV_GPS:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_DIV_GPS" ) );
			break;
		case ANT_TYPE_IDX_MULTI_DG:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_DG" ) );
			break;
		case ANT_TYPE_IDX_MULTI_M:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_M" ) );
			break;
		case ANT_TYPE_IDX_MULTI_M1:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_M1" ) );
			break;
		case ANT_TYPE_IDX_MULTI_M2:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_M2" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_1:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_1" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_2:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_2" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_3:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_3" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_4:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_4" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_5:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_5" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_6:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_6" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_7:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_7" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_8:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_8" ) );
			break;
		case ANT_TYPE_IDX_MAIN_2:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MAIN_2" ) );
			break;
		case ANT_TYPE_IDX_DIV_2:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_DIV_2" ) );
			break;
		case ANT_TYPE_IDX_MULTI_M3:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_M3" ) );
			break;
		case ANT_TYPE_IDX_MULTI_M4:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_M4" ) );
			break;
		case ANT_TYPE_IDX_WIFI_MIMO:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_WIFI_MIMO" ) );
			break;
		case ANT_TYPE_IDX_V2X_MAIN:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_V2X_MAIN" ) );
			break;
		case ANT_TYPE_IDX_V2X_DIV:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_V2X_DIV" ) );
			break;
		case ANT_TYPE_IDX_DSDA_MAIN:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_DSDA_MAIN" ) );
			break;
		case ANT_TYPE_IDX_DSDA_DIV:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_DSDA_DIV" ) );
			break;
		case ANT_TYPE_IDX_V2X_TRX0:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_V2X_TRX0" ) );
			break;
		case ANT_TYPE_IDX_V2X_TRX1:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_V2X_TRX1" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_9:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_9" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_10:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_10" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_11:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_11" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_12:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_12" ) );
			break;
		case ANT_TYPE_IDX_WIFI_BLU_GPS:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_WIFI_BLU_GPS" ) );
			break;
		default:
			//fail
			throw(0);
		}//switch( eu_Ant_Type_Index
	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_ANT_Type_Name, Ant_Type_Index(%d) is Wrong!" ), eu_Ant_Type_Index );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::IS_SMART_MODULE( int i_BB_Chip )
{
	//�ж��Ƿ��Ǹ�ͨ����ģ��ƽ̨

	bool bResult = false;

	//
	switch( i_BB_Chip )//��ͨ��MSMxxxx/SDMxxxx��������ģ��ƽ̨
	{
	case BB_MSM8909:
	case BB_MSM8937:
	case BB_MSM8953:
	case BB_MSM8917:
	case BB_SDM450:
	case BB_MSM8905:
	case BB_SDM660:
	case BB_QM215:
	case BB_QCM2150:
	case BB_SL8541E:
	case BB_SDA845:
	case BB_SM6115:
	case BB_UIS8581:
	case BB_QCM6125:
	case BB_QCM2290:
	case BB_SM4350:
	case BB_SL8521E:
	case BB_QCM6490:
	case BB_QCM4290:
	case BB_QCS8250:
		//������ģ��ƽ̨
		bResult = true;
		//
		break;

	default:
		//��������ģ��ƽ̨
		bResult = false;
		//
		break;
	}//switch( i_BB_Chip

	//���ؽ��
	return bResult;
}


void CGFC::Empty_Struct_TESTER_RF_OFFSET_S( TESTER_RF_OFFSET_S & oj_str_RF_Loss_V1 )
{
	int iBuff_Idx, iQuan;


	//GSM
	iQuan = G_BAND_SIZE * 6;
	//GSM ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_GSM_RF_Loss[iBuff_Idx] = 0.0;
	}

	//WCDMA
	iQuan = W_BAND_SIZE * 2;
	//WCDMA ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_W_RF_Loss[iBuff_Idx] = 0.0;
	}

	//TD-SCDMA
	iQuan = TD_BAND_SIZE * 2;
	//TD-SCDMA ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_TD_RF_Loss[iBuff_Idx] = 0.0;
	}

	//cdma2000
	iQuan = C2K_BC_SIZE * 2;
	//cdma2000 ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_C2K_RF_Loss[iBuff_Idx] = 0.0;
	}

	//LTE
	iQuan = L_BAND_SIZE * 2;
	//LTE ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_LTE_RF_Loss[iBuff_Idx] = 0.0;
	}

	//NB-IoT
	iQuan = NB_IDX_B_SIZE * 2;
	//NB-IoT ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_NBIoT_RF_Loss[iBuff_Idx] = 0.0;
	}
}


void CGFC::Empty_Struct_TESTER_RF_OFFSET_V2_S( TESTER_RF_OFFSET_V2_S & oj_str_RF_Loss_V2 )
{
	int iBuff_Idx, iQuan;


	//GSM
	iQuan = G_BAND_SIZE * 6;
	//GSM ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_GSM_RF_Loss[iBuff_Idx] = 0.0;
	}

	//WCDMA
	iQuan = W_BAND_SIZE * 6;
	//WCDMA ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_W_RF_Loss[iBuff_Idx] = 0.0;
	}

	//TD-SCDMA
	iQuan = TD_BAND_SIZE * 6;
	//TD-SCDMA ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_TD_RF_Loss[iBuff_Idx] = 0.0;
	}

	//cdma2000
	iQuan = C2K_BC_SIZE * 6;
	//cdma2000 ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_C2K_RF_Loss[iBuff_Idx] = 0.0;
	}

	//LTE
	iQuan = L_BAND_SIZE * 6;
	//LTE ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_LTE_RF_Loss[iBuff_Idx] = 0.0;
	}

	//NB-IoT
	iQuan = NB_IDX_B_SIZE * 6;
	//NB-IoT ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_NBIoT_RF_Loss[iBuff_Idx] = 0.0;
	}

	//NR 5G SUB6
	iQuan = NR5GSUB6_N_SIZE * 6;
	//NR 5G SUB6 ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_NR_5G_SUB6_RF_Loss[iBuff_Idx] = 0.0;
	}

	//GPS
	iQuan = 2;
	//GPS ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_GPS_RF_Loss[iBuff_Idx] = 0.0;
	}

	//WiFi
	iQuan = WIFI_FB_COUNT * 6;
	//WiFi ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_WiFi_RF_Loss[iBuff_Idx] = 0.0;
	}

	//Bluetooth
	iQuan = 2;
	//Bluetooth ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_BLU_RF_Loss[iBuff_Idx] = 0.0;
	}

	//CV2X
	iQuan = CV2X_BAND_SIZE * 6;
	//CV2X ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_CV2X_RF_Loss[iBuff_Idx] = 0.0;
	}
}


void CGFC::Empty_Struct_ANT_CFG_ALL_S( ANT_CFG_ALL_S & oj_str_ANT_Config_All )
{
	int iAnt_Idx;


	//��ա������б��ṹ��
	this->Empty_Struct_ANT_LIST_S( oj_str_ANT_Config_All.str_ANT_List );


	//��ա���������
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_IDX_SIZE; iAnt_Idx++ )
	{
		this->Empty_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_Config_All.strr_ANT_RF_Loss[iAnt_Idx] );
	}
}


void CGFC::Empty_Struct_ANT_LIST_S( ANT_LIST_S & oj_str_ANT_List )
{
	int iAnt_Idx;

	//��ա������б�������
	oj_str_ANT_List.i_ANT_Quan = 0;

	//��ա������б�����������
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++ )
	{
		oj_str_ANT_List.strr_ANT_Prop_List[iAnt_Idx].eu_ANT_HW_Type_Name = ANT_TYPE_IDX_NDF;
		oj_str_ANT_List.strr_ANT_Prop_List[iAnt_Idx].i_ANT_SW_Numb = -1;
	}
}


void CGFC::Empty_Struct_REF_DATA_ALL_S( REF_DATA_ALL_S & oj_str_Ref_Data_All )
{
	int iAnt_Idx;


	//��ա������б��Ľṹ��
	this->Empty_Struct_ANT_LIST_S( oj_str_Ref_Data_All.str_Ref_ANT_List );


	//��ա���塱����
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_IDX_SIZE; iAnt_Idx++ )
	{
		this->Empty_Struct_REF_DATA_BAND_S( oj_str_Ref_Data_All.strr_Ref_Band_Data[iAnt_Idx] );
	}
}


void CGFC::Empty_Struct_REF_DATA_BAND_S( REF_DATA_BAND_S & oj_str_Ref_Data_Band )
{
	int iBand_Idx, iLMH_Idx;


	//GSM Ƶ��
	for( iBand_Idx = 0; iBand_Idx < G_BAND_SIZE; iBand_Idx++ )
	{
		//���REFƵ������ʹ�ܱ�ʶ
		oj_str_Ref_Data_Band.strr_GSM_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//���REFֵ
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_GSM_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_GSM_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//WCDMA Ƶ��
	for( iBand_Idx = 0; iBand_Idx < W_BAND_SIZE; iBand_Idx++ )
	{
		//���REFƵ������ʹ�ܱ�ʶ
		oj_str_Ref_Data_Band.strr_WCDMA_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//���REFֵ
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_WCDMA_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_WCDMA_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//cdma2000 Ƶ��
	for( iBand_Idx = 0; iBand_Idx < C2K_BC_SIZE; iBand_Idx++ )
	{
		//���REFƵ������ʹ�ܱ�ʶ
		oj_str_Ref_Data_Band.strr_C2K_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//���REFֵ
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_C2K_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_C2K_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//TD-SCDMA Ƶ��
	for( iBand_Idx = 0; iBand_Idx < TD_BAND_SIZE; iBand_Idx++ )
	{
		//���REFƵ������ʹ�ܱ�ʶ
		oj_str_Ref_Data_Band.strr_TD_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//���REFֵ
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_TD_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_TD_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//LTE Ƶ��
	for( iBand_Idx = 0; iBand_Idx < L_BAND_SIZE; iBand_Idx++ )
	{
		//���REFƵ������ʹ�ܱ�ʶ
		oj_str_Ref_Data_Band.strr_LTE_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//���REFֵ
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_LTE_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_LTE_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//NB-IoT Ƶ��
	for( iBand_Idx = 0; iBand_Idx < NB_IDX_B_SIZE; iBand_Idx++ )
	{
		//���REFƵ������ʹ�ܱ�ʶ
		oj_str_Ref_Data_Band.strr_NBIoT_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//���REFֵ
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_NBIoT_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_NBIoT_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//NR_5G_SUB6 Ƶ��
	for( iBand_Idx = 0; iBand_Idx < NR5GSUB6_N_SIZE; iBand_Idx++ )
	{
		//���REFƵ������ʹ�ܱ�ʶ
		oj_str_Ref_Data_Band.strr_NR_5G_SUB6_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//���REFֵ
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_NR_5G_SUB6_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_NR_5G_SUB6_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//C-V2X
	for( iBand_Idx = 0; iBand_Idx < CV2X_BAND_SIZE; iBand_Idx++ )
	{
		//���REFƵ������ʹ�ܱ�ʶ
		oj_str_Ref_Data_Band.strr_CV2X_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//���REFֵ
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_CV2X_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_CV2X_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//GPS
	//���REFƵ������ʹ�ܱ�ʶ
	oj_str_Ref_Data_Band.str_GPS_REF_Data.i_Band_Name_And_Enable = -1;

	//���REFֵ
	for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
	{
		oj_str_Ref_Data_Band.str_GPS_REF_Data.dr_Tx_Power[iLMH_Idx] = -1.0;
		oj_str_Ref_Data_Band.str_GPS_REF_Data.dr_Rx_Level[iLMH_Idx] = -1.0;
	}


	//WiFi
	for( iBand_Idx = 0; iBand_Idx < WIFI_FB_COUNT; iBand_Idx++ )
	{
		//���REFƵ������ʹ�ܱ�ʶ
		oj_str_Ref_Data_Band.strr_WiFi_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//���REFֵ
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_WiFi_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_WiFi_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//����
	//���REFƵ������ʹ�ܱ�ʶ
	oj_str_Ref_Data_Band.str_Bluetooth_REF_Data.i_Band_Name_And_Enable = -1;

	//���REFֵ
	for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
	{
		oj_str_Ref_Data_Band.str_Bluetooth_REF_Data.dr_Tx_Power[iLMH_Idx] = -1.0;
		oj_str_Ref_Data_Band.str_Bluetooth_REF_Data.dr_Rx_Level[iLMH_Idx] = -1.0;
	}
}


void CGFC::Copy_Struct_ANT_CFG_ALL_S( ANT_CFG_ALL_S & oj_str_ANT_CFG_Dest, ANT_CFG_ALL_S str_ANT_CFG_Source )
{
	int iAnt_Idx;


	//Copy��ANT�����б��ṹ��
	this->Copy_Struct_ANT_LIST_S( oj_str_ANT_CFG_Dest.str_ANT_List, str_ANT_CFG_Source.str_ANT_List );


	//Copy����������
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_IDX_SIZE; iAnt_Idx++ )
	{
		//Copy������ V2���ṹ��
		this->Copy_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_CFG_Dest.strr_ANT_RF_Loss[iAnt_Idx], str_ANT_CFG_Source.strr_ANT_RF_Loss[iAnt_Idx] );
	}
}


void CGFC::Copy_Struct_ANT_LIST_S( ANT_LIST_S & oj_str_ANT_List_Dest, ANT_LIST_S str_ANT_List_Source )
{
	int iAnt_Idx;

	//��ֵ�������б�������
	oj_str_ANT_List_Dest.i_ANT_Quan = str_ANT_List_Source.i_ANT_Quan;

	//��ֵ�������б�����������
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++ )
	{
		//������������
		oj_str_ANT_List_Dest.strr_ANT_Prop_List[iAnt_Idx].eu_ANT_HW_Type_Name = str_ANT_List_Source.strr_ANT_Prop_List[iAnt_Idx].eu_ANT_HW_Type_Name;
		oj_str_ANT_List_Dest.strr_ANT_Prop_List[iAnt_Idx].i_ANT_SW_Numb = str_ANT_List_Source.strr_ANT_Prop_List[iAnt_Idx].i_ANT_SW_Numb;
	}
}


void CGFC::Copy_Struct_TESTER_RF_OFFSET_V2_S( TESTER_RF_OFFSET_V2_S & oj_str_RF_Loss_Dest, TESTER_RF_OFFSET_V2_S str_RF_Loss_Source )
{
	int iBuff_Idx, iQuan;


	//GSM
	iQuan = G_BAND_SIZE * 6;
	//GSM ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_GSM_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_GSM_RF_Loss[iBuff_Idx];
	}

	//WCDMA
	iQuan = W_BAND_SIZE * 6;
	//WCDMA ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_W_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_W_RF_Loss[iBuff_Idx];
	}

	//TD-SCDMA
	iQuan = TD_BAND_SIZE * 6;
	//TD-SCDMA ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_TD_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_TD_RF_Loss[iBuff_Idx];
	}

	//cdma2000
	iQuan = C2K_BC_SIZE * 6;
	//cdma2000 ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_C2K_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_C2K_RF_Loss[iBuff_Idx];
	}

	//LTE
	iQuan = L_BAND_SIZE * 6;
	//LTE ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_LTE_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_LTE_RF_Loss[iBuff_Idx];
	}

	//NB-IoT
	iQuan = NB_IDX_B_SIZE * 6;
	//NB-IoT ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_NBIoT_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_NBIoT_RF_Loss[iBuff_Idx];
	}

	//NR 5G SUB6
	iQuan = NR5GSUB6_N_SIZE * 6;
	//NR 5G SUB6 ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_NR_5G_SUB6_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_NR_5G_SUB6_RF_Loss[iBuff_Idx];
	}

	//GPS
	iQuan = 2;
	//GPS ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_GPS_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_GPS_RF_Loss[iBuff_Idx];
	}

	//WiFi
	iQuan = WIFI_FB_COUNT * 6;
	//WiFi ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_WiFi_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_WiFi_RF_Loss[iBuff_Idx];
	}

	//Bluetooth
	iQuan = 2;
	//Bluetooth ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_BLU_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_BLU_RF_Loss[iBuff_Idx];
	}

	//CV2X
	iQuan = CV2X_BAND_SIZE * 6;
	//CV2X ��ֵ
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_CV2X_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_CV2X_RF_Loss[iBuff_Idx];
	}
}


void CGFC::Copy_Struct_TESTER_RF_OFFSET_V2_S_To_TESTER_RF_OFFSET_S( TESTER_RF_OFFSET_S & oj_str_RF_Loss_V1_Dest, TESTER_RF_OFFSET_V2_S str_RF_Loss_V2_Source )
{
	int iBand_Idx, iQuan;


	//����
	//GSM ����
	iQuan = G_BAND_SIZE * 6;
	//GSM
	for( iBand_Idx = 0; iBand_Idx < iQuan; iBand_Idx++ )
	{
		oj_str_RF_Loss_V1_Dest.dr_GSM_RF_Loss[iBand_Idx] = str_RF_Loss_V2_Source.dr_GSM_RF_Loss[iBand_Idx];
	}

	//WCDMA ����
	for( iBand_Idx = 0; iBand_Idx < W_BAND_SIZE; iBand_Idx++ )
	{
		//TX
		oj_str_RF_Loss_V1_Dest.dr_W_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT] = str_RF_Loss_V2_Source.dr_W_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + CH_MID];
		//RX
		oj_str_RF_Loss_V1_Dest.dr_W_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT] = str_RF_Loss_V2_Source.dr_W_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + CH_MID];
	}

	//TD ����
	for( iBand_Idx = 0; iBand_Idx < TD_BAND_SIZE; iBand_Idx++ )
	{
		//TX
		oj_str_RF_Loss_V1_Dest.dr_TD_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT] = str_RF_Loss_V2_Source.dr_TD_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + CH_MID];
		//RX
		oj_str_RF_Loss_V1_Dest.dr_TD_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT] = str_RF_Loss_V2_Source.dr_TD_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + CH_MID];
	}

	//cdma2000 ����
	for( iBand_Idx = 0; iBand_Idx < C2K_BC_SIZE; iBand_Idx++ )
	{
		//TX
		oj_str_RF_Loss_V1_Dest.dr_C2K_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT] = str_RF_Loss_V2_Source.dr_C2K_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + CH_MID];
		//RX
		oj_str_RF_Loss_V1_Dest.dr_C2K_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT] = str_RF_Loss_V2_Source.dr_C2K_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + CH_MID];
	}

	//LTE ����
	for( iBand_Idx = 0; iBand_Idx < L_BAND_SIZE; iBand_Idx++ )
	{
		//TX
		oj_str_RF_Loss_V1_Dest.dr_LTE_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT] = str_RF_Loss_V2_Source.dr_LTE_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + CH_MID];
		//RX
		oj_str_RF_Loss_V1_Dest.dr_LTE_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT] = str_RF_Loss_V2_Source.dr_LTE_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + CH_MID];
	}

	//NB-IoT ����
	for( iBand_Idx = 0; iBand_Idx < NB_IDX_B_SIZE; iBand_Idx++ )
	{
		//TX
		oj_str_RF_Loss_V1_Dest.dr_NBIoT_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT] = str_RF_Loss_V2_Source.dr_NBIoT_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + CH_MID];
		//RX
		oj_str_RF_Loss_V1_Dest.dr_NBIoT_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT] = str_RF_Loss_V2_Source.dr_NBIoT_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + CH_MID];
	}
}


void CGFC::Copy_Struct_TESTER_RF_OFFSET_S_To_TESTER_RF_OFFSET_V2_S( TESTER_RF_OFFSET_V2_S & oj_str_RF_Loss_V2_Dest, TESTER_RF_OFFSET_S str_RF_Loss_V1_Source )
{
	int iBand_Idx, iQuan, iLMH_Idx;


	//����
	//GSM ����
	iQuan = G_BAND_SIZE * 6;
	//GSM
	for( iBand_Idx = 0; iBand_Idx < iQuan; iBand_Idx++ )
	{
		oj_str_RF_Loss_V2_Dest.dr_GSM_RF_Loss[iBand_Idx] = str_RF_Loss_V1_Source.dr_GSM_RF_Loss[iBand_Idx];
	}

	//WCDMA ����
	for( iBand_Idx = 0; iBand_Idx < W_BAND_SIZE; iBand_Idx++ )
	{
		for( iLMH_Idx = CH_LOW; iLMH_Idx <= CH_HIGH; iLMH_Idx++ )
		{
			//TX
			oj_str_RF_Loss_V2_Dest.dr_W_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_W_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT];
			//RX
			oj_str_RF_Loss_V2_Dest.dr_W_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_W_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT];
		}
	}

	//TD ����
	for( iBand_Idx = 0; iBand_Idx < TD_BAND_SIZE; iBand_Idx++ )
	{
		for( iLMH_Idx = CH_LOW; iLMH_Idx <= CH_HIGH; iLMH_Idx++ )
		{
			//TX
			oj_str_RF_Loss_V2_Dest.dr_TD_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_TD_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT];
			//RX
			oj_str_RF_Loss_V2_Dest.dr_TD_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_TD_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT];
		}
	}

	//cdma2000 ����
	for( iBand_Idx = 0; iBand_Idx < C2K_BC_SIZE; iBand_Idx++ )
	{
		for( iLMH_Idx = CH_LOW; iLMH_Idx <= CH_HIGH; iLMH_Idx++ )
		{
			//TX
			oj_str_RF_Loss_V2_Dest.dr_C2K_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_C2K_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT];
			//RX
			oj_str_RF_Loss_V2_Dest.dr_C2K_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_C2K_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT];
		}
	}

	//LTE ����
	for( iBand_Idx = 0; iBand_Idx < L_BAND_SIZE; iBand_Idx++ )
	{
		for( iLMH_Idx = CH_LOW; iLMH_Idx <= CH_HIGH; iLMH_Idx++ )
		{
			//TX
			oj_str_RF_Loss_V2_Dest.dr_LTE_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_LTE_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT];
			//RX
			oj_str_RF_Loss_V2_Dest.dr_LTE_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_LTE_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT];
		}
	}

	//NB-IoT ����
	for( iBand_Idx = 0; iBand_Idx < NB_IDX_B_SIZE; iBand_Idx++ )
	{
		for( iLMH_Idx = CH_LOW; iLMH_Idx <= CH_HIGH; iLMH_Idx++ )
		{
			//TX
			oj_str_RF_Loss_V2_Dest.dr_NBIoT_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_NBIoT_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT];
			//RX
			oj_str_RF_Loss_V2_Dest.dr_NBIoT_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_NBIoT_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT];
		}
	}
}


bool CGFC::Get_RF_Key_Type_Name( const EU_RF_KEY_TYPE_INDEX eu_RF_Key_Type_Index, CString & oj_cs_RF_Key_Type_Name )
{
	//RF Key Type index�� ת��Ϊ RF Key Type Name �ִ�

	//
	try
	{
		switch( eu_RF_Key_Type_Index )
		{
		case RF_KEY_TYPE_IDX_PXA1802S_RF858:	//ASRƽ̨RF Type:"1802S+RF858"
			oj_cs_RF_Key_Type_Name.Format( _T( "1802S+RF858" ) );
			break;
		case RF_KEY_TYPE_IDX_PAX1908_RF858:		//ASRƽ̨RF Type:"1802+RF858"
			oj_cs_RF_Key_Type_Name.Format( _T( "1802+RF858" ) );
			break;
		case RF_KEY_TYPE_IDX_RF866:				//ASRƽ̨RF Type:"1826+RF866"
			oj_cs_RF_Key_Type_Name.Format( _T( "1826+RF866" ) );
			break;
		case RF_KEY_TYPE_IDX_RF859C:			//ASRƽ̨RF Type:"1802S+RF859C"
			oj_cs_RF_Key_Type_Name.Format( _T( "1802S+RF859C" ) );
			break;
		case RF_KEY_TYPE_IDX_1601_3601:			//ASRƽ̨RF Type:"3601_1601"
			oj_cs_RF_Key_Type_Name.Format( _T( "3601_1601" ) );
			break;
		case RF_KEY_TYPE_IDX_1603:			    //ASRƽ̨RF Type:"1603"
			oj_cs_RF_Key_Type_Name.Format( _T( "1603" ) );
			break;
		case RF_KEY_TYPE_IDX_3603:			    //ASRƽ̨RF Type:"3603"
			oj_cs_RF_Key_Type_Name.Format( _T( "3603" ) );
			break;
		case RF_KEY_TYPE_IDX_1803_FAICON:		//ASRƽ̨RF Type:"1803"
			oj_cs_RF_Key_Type_Name.Format( _T( "1803" ) );
			break;
		case RF_KEY_TYPE_IDX_1606:				//ASRƽ̨RF Type:"1606"
			oj_cs_RF_Key_Type_Name.Format( _T( "1606" ) );
			break;
		case RF_KEY_TYPE_IDX_1828_RF866:		//ASRƽ̨RF Type:"1828+RF866"
			oj_cs_RF_Key_Type_Name.Format( _T( "1828+RF866" ) );
			break;
		default:
			oj_cs_RF_Key_Type_Name.Format( _T( "NDF" ) );
			//fail
			throw(0);
		}//switch( eu_RF_Key_Type_Index
	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_RF_Key_Type_Name, RF_Key_Type_Index(%d) is Wrong!" ), eu_RF_Key_Type_Index );

		//fail
		return false;
	}

	//pass
	return true;
}


void CGFC::Unicode_To_UTF8( const wchar_t * wcp_Unicode_Str, CStringA & oj_csa_UTF_8_Str )
{
	//Unicode��wchar_t* ---> utf8���룬char*

	int iLen = WideCharToMultiByte( CP_UTF8, 0, wcp_Unicode_Str, -1, NULL, 0, NULL, NULL );

	//�����ڴ�
	char * cpUtf8 = (char *)malloc( iLen + 8 );

	//���
	memset( cpUtf8, NULL, (iLen + 8) );

	//ת��
	WideCharToMultiByte( CP_UTF8, 0, wcp_Unicode_Str, -1, cpUtf8, iLen, NULL, NULL );

	//��ֵ
	oj_csa_UTF_8_Str.Format( "%s", cpUtf8 );

	//�ͷ��ڴ�
	free( cpUtf8 );
}


bool CGFC::Get_CV2X_Band_Index( int i_CV2X_Band_Name, int * ip_CV2X_Band_Index )
{
	//CV2XƵ���� ת�� CV2XƵ��index��
	//CV2X�¼�Ƶ�δ���λ��

	//
	switch( i_CV2X_Band_Name )
	{
	case 46://CV2X_BAND_46
		*ip_CV2X_Band_Index = CV2X_BAND_46;
		break;
	case 47://CV2X_BAND_47
		*ip_CV2X_Band_Index = CV2X_BAND_47;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_CV2X_Band_Index, C-V2X_Band_Name(%d) is wrong!" ), i_CV2X_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_CV2X_Band_Name( int i_CV2X_Band_Index, int * ip_CV2X_Band_Name )
{
	//CV2XƵ��index�� ת�� CV2XƵ����
	//CV2X�¼�Ƶ�δ���λ��

	//
	switch( i_CV2X_Band_Index )
	{
	case CV2X_BAND_46:
		*ip_CV2X_Band_Name = 46;
		break;
	case CV2X_BAND_47:
		*ip_CV2X_Band_Name = 47;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_CV2X_Band_Name, CV2X_Band_Index(%d) is wrong!" ), i_CV2X_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}


void CGFC::Copy_Struct_BAND_ENABLE_S( BAND_ENABLE_S & oj_str_Band_Enable_Dest, BAND_ENABLE_S str_Band_Enable_Source )
{
	int iBuff_Idx;


	//GSM
	for( iBuff_Idx = 0; iBuff_Idx < G_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_GSM_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_GSM_Band_Enable[iBuff_Idx];
	}

	//WCDMA
	for( iBuff_Idx = 0; iBuff_Idx < W_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_WCDMA_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_WCDMA_Band_Enable[iBuff_Idx];
	}

	//cdma2000
	for( iBuff_Idx = 0; iBuff_Idx < C2K_BC_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_C2K_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_C2K_Band_Enable[iBuff_Idx];
	}

	//TD
	for( iBuff_Idx = 0; iBuff_Idx < TD_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_TD_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_TD_Band_Enable[iBuff_Idx];
	}

	//LTE
	for( iBuff_Idx = 0; iBuff_Idx < L_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_LTE_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_LTE_Band_Enable[iBuff_Idx];
	}

	//NB-IoT
	for( iBuff_Idx = 0; iBuff_Idx < NB_IDX_B_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_NBIoT_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_NBIoT_Band_Enable[iBuff_Idx];
	}

	//NR 5G SUB6
	for( iBuff_Idx = 0; iBuff_Idx < NR5GSUB6_N_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_NR_5G_SUB6_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_NR_5G_SUB6_Band_Enable[iBuff_Idx];
	}

	//C-V2X
	for( iBuff_Idx = 0; iBuff_Idx < CV2X_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_CV2X_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_CV2X_Band_Enable[iBuff_Idx];
	}
}


void CGFC::Assign_Struct_BAND_ENABLE_S( BAND_ENABLE_S & oj_str_Band_Enable, int i_Value_Data )
{
	int iBuff_Idx;


	//GSM
	for( iBuff_Idx = 0; iBuff_Idx < G_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_GSM_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//WCDMA
	for( iBuff_Idx = 0; iBuff_Idx < W_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_WCDMA_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//cdma2000
	for( iBuff_Idx = 0; iBuff_Idx < C2K_BC_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_C2K_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//TD
	for( iBuff_Idx = 0; iBuff_Idx < TD_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_TD_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//LTE
	for( iBuff_Idx = 0; iBuff_Idx < L_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_LTE_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//NB-IoT
	for( iBuff_Idx = 0; iBuff_Idx < NB_IDX_B_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_NBIoT_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//NR 5G SUB6
	for( iBuff_Idx = 0; iBuff_Idx < NR5GSUB6_N_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_NR_5G_SUB6_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//C-V2X
	for( iBuff_Idx = 0; iBuff_Idx < CV2X_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_CV2X_Band_Enable[iBuff_Idx] = i_Value_Data;
	}
}


bool CGFC::Get_NB_SCSP_Name( int i_S3K75_S15K, CString & oj_cs_NB_SCSP_Name )
{
	try
	{
		switch( i_S3K75_S15K )
		{
		case NB_SCSP_S3K75://3.75 kHz
			oj_cs_NB_SCSP_Name.Format( _T( "S3K75" ) );
			break;
		case NB_SCSP_S15K://15 kHz
			oj_cs_NB_SCSP_Name.Format( _T( "S15K" ) );
			break;
		default:
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_NB_SCSP_Name, S3K75_S15K(%d) is Wrong!" ), i_S3K75_S15K );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NB_Mod_Scheme_Name( int i_BPSK_QPSK, CString & oj_cs_NB_Mod_Scheme_Name )
{
	try
	{
		switch( i_BPSK_QPSK )
		{
		case NB_MSCH_BPSK://0 = BPSK
			oj_cs_NB_Mod_Scheme_Name.Format( _T( "BPSK" ) );
			break;
		case NB_MSCH_QPSK://1 = QPSK
			oj_cs_NB_Mod_Scheme_Name.Format( _T( "QPSK" ) );
			break;
		default:
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_NB_Mod_Scheme_Name, BPSK_QPSK(%d) is Wrong!" ), i_BPSK_QPSK );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NR_5G_MMW_Band_Index( int i_Band_Name, int & oj_i_Band_Index )
{
	try
	{
		switch( i_Band_Name )//5G mmW: n257/n258/n260/n261
		{
		case 257://NR5GMMW_N_257
			oj_i_Band_Index = NR5GMMW_N_257;
			break;
		case 258://NR5GMMW_N_258
			oj_i_Band_Index = NR5GMMW_N_258;
			break;
		case 260://NR5GMMW_N_260
			oj_i_Band_Index = NR5GMMW_N_260;
			break;
		case 261://NR5GMMW_N_261
			oj_i_Band_Index = NR5GMMW_N_261;
			break;
		default:
			//fail
			throw(0);
		}//switch( i_Band_Name
	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_NR_5G_MMW_Band_Index, Band_Name(%d) is Wrong!" ), i_Band_Name );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NR_5G_MMW_Band_Name( int i_Band_Index, int & oj_i_Band_Name )
{
	try
	{
		switch( i_Band_Index )//5G mmW: n257/n258/n260/n261
		{
		case NR5GMMW_N_257:
			oj_i_Band_Name = 257;
			break;
		case NR5GMMW_N_258:
			oj_i_Band_Name = 258;
			break;
		case NR5GMMW_N_260:
			oj_i_Band_Name = 260;
			break;
		case NR5GMMW_N_261:
			oj_i_Band_Name = 261;
			break;
		default:
			//fail
			throw(0);
		}//switch( i_Band_Index

	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_NR_5G_MMW_Band_Name, Band_Index(%d) is Wrong!" ), i_Band_Index );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_ANT_Type_Index( const wchar_t * wcp_Ant_Type_Name, EU_ANT_TYPE_INDEX & oj_eu_Ant_Type_Idx )
{
	if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MAIN_1" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MAIN_1;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_DIV_1" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_DIV_1;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_GPS" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_GPS;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_WIFI_BLU" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_WIFI_BLU;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_DIV_GPS" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_DIV_GPS;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_DG" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_DG;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_M" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_M;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_M1" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_M1;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_M2" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_M2;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_1" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_1;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_2" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_2;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_3" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_3;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_4" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_4;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_5" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_5;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_6" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_6;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_7" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_7;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_8" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_8;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MAIN_2" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MAIN_2;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_DIV_2" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_DIV_2;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_M3" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_M3;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_M4" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_M4;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_WIFI_MIMO" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_WIFI_MIMO;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_V2X_MAIN" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_V2X_MAIN;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_V2X_DIV" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_V2X_DIV;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_DSDA_MAIN" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_DSDA_MAIN;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_DSDA_DIV" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_DSDA_DIV;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_V2X_TRX0" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_V2X_TRX0;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_V2X_TRX1" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_V2X_TRX1;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_9" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_9;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_10" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_10;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_11" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_11;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_12" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_12;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_WIFI_BLU_GPS" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_WIFI_BLU_GPS;
	}
	else
	{
		//��ʾ�쳣
		this->m_csError_Info.Format( _T( "ERROR, Get_ANT_Type_Index(), \"%s\" is Wrong!" ), wcp_Ant_Type_Name );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}

//�Ƚϵ�ǰʱ��͸���ʱ��Ƚ�
//_T("2018-05-18 18:32:44")
int CGFC::TimeCmp(CString strTimeSrc)
{
	CString strRep(_T("")),strDst(_T("")),csTr(_T(""));
	double dwSrc(0),dwDst(0);

	for (int i=0;i<strTimeSrc.GetLength();i++)
	{
		csTr = strTimeSrc.GetAt(i);
		if ((csTr >=_T("0")) && (csTr <=_T("9")))
		{
			strRep+=csTr;
		}
		else
		{
			continue;
		}

	}
	dwSrc= _wtof(strRep);
#if 0
	CTime aTime = CTime::GetCurrentTime();
	int y = aTime.GetYear();
	int m = aTime.GetMonth();
	int d = aTime.GetDay();
	int h = aTime.GetHour();
	int min = aTime.GetMinute();
	int s = aTime.GetSecond();

	strDst.Format(_T("%04d%02d%02d%02d%02d%02d"),y,m,d,h,min,s);
	dwDst= _wtof(strDst);
#endif
	dwDst=20180601000000;

	if (dwSrc>dwDst)
	{
		//��ǰ��ʱ��ȸ�����ʱ��Ҫ��
		return 1;
	} 
	else
	{
		return -1;
	}
	

}

void CGFC::Get_RF_Instru_Name( int i_Instru_Index, CString & oj_cs_Instru_Name )
{
	//�����궨��ת�����ִ���
	switch( i_Instru_Index )
	{
	case CMU200:
		oj_cs_Instru_Name.Format( _T( "CMU200" ) );
		break;

	case AG_E1960:
	case AG_E1968:
	case AG_E1987A:
	case AG_E1962B:
		oj_cs_Instru_Name.Format( _T( "8960" ) );
		break;

	case CMW500:
		oj_cs_Instru_Name.Format( _T( "CMW500" ) );
		break;

	case LP_IQXS:
		oj_cs_Instru_Name.Format( _T( "IQxstream" ) );
		break;

	case CMW100:
		oj_cs_Instru_Name.Format( _T( "CMW100" ) );
		break;

	case LP_IQXM:
		oj_cs_Instru_Name.Format( _T( "IQxstream-M" ) );
		break;

	case LP_5G_IQXS:
		oj_cs_Instru_Name.Format( _T( "IQxstream-5G" ) );
		break;

	case CMP180:
		oj_cs_Instru_Name.Format( _T( "CMP180" ) );
		break;

	default:
		oj_cs_Instru_Name.Format( _T( "NDF" ) );
		break;
	}//switch( i_Instru_Index
}

bool CGFC::Get_Locoal_IP_Mac(CString  cs_PC_IP, CString & cs_PC_Mac)
{
	//vector<MacIp> &macip;
	CString myIP, myMAC;
	bool bNetReady = false;
	ULONG outBufLen = 0;
	DWORD dwRetVal = 0;

	PIP_ADAPTER_INFO pAadpterInfo;
	PIP_ADAPTER_INFO pAadpterInfoTmp = NULL;

	//MacIp tmpmacip;

	pAadpterInfo = (IP_ADAPTER_INFO*)GlobalAlloc(GMEM_ZEROINIT, sizeof(IP_ADAPTER_INFO));

	if (GetAdaptersInfo(pAadpterInfo, &outBufLen) != ERROR_SUCCESS)
	{
		GlobalFree(pAadpterInfo);
		pAadpterInfo = (IP_ADAPTER_INFO*)GlobalAlloc(GMEM_ZEROINIT, outBufLen);
	}

	if ((dwRetVal = GetAdaptersInfo(pAadpterInfo, &outBufLen)) == NO_ERROR)
	{
		pAadpterInfoTmp = pAadpterInfo;
		myIP = "";

		while (pAadpterInfoTmp)
		{
			// 			if (pAadpterInfoTmp->Type == MIB_IF_TYPE_ETHERNET)
			// 			{
			if (pAadpterInfoTmp->CurrentIpAddress)
			{
				myIP = pAadpterInfoTmp->CurrentIpAddress->IpAddress.String;
			}
			else
			{
				myIP = pAadpterInfoTmp->IpAddressList.IpAddress.String;
			}
			//			}
			OutputDebugString(L"IP:" + myIP);
			if (!myIP.IsEmpty() && (myIP.CompareNoCase(_T("0.0.0.0")) != 0))
			{
				myMAC.Format(_T("%02X%02X%02X%02X%02X%02X"),
					pAadpterInfoTmp->Address[0],
					pAadpterInfoTmp->Address[1],
					pAadpterInfoTmp->Address[2],
					pAadpterInfoTmp->Address[3],
					pAadpterInfoTmp->Address[4],
					pAadpterInfoTmp->Address[5]);
				//tmpmacip.ip = myIP;
				//tmpmacip.mac = myMAC;
				//				macip.push_back(tmpmacip);
				OutputDebugString(L"MAC:" + myMAC);
				//break;
				if (myIP == cs_PC_IP)
				{
					bNetReady = true;
					cs_PC_Mac = myMAC;
					break;
				}
			}

			pAadpterInfoTmp = pAadpterInfoTmp->Next;
		}
	}
	GlobalFree(pAadpterInfo);

	return bNetReady;
}



void CGFC::Empty_Struct_REF_DATA_ALL_V2_S( REF_DATA_ALL_V2_S & oj_str_Ref_Data_All_V2 )
{
	int iAnt_Idx;

	//��ա������б��Ľṹ��
	this->Empty_Struct_ANT_LIST_S( oj_str_Ref_Data_All_V2.str_Ref_Ant_List );

	//��ա���塱����
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++ )
	{
		this->Empty_Struct_REF_DATA_BAND_S( oj_str_Ref_Data_All_V2.strr_Ref_Data_List[iAnt_Idx] );
	}
}


bool CGFC::Get_ANT_Type_List_Buff_Index( ANT_LIST_S str_Ant_List, EU_ANT_TYPE_INDEX eu_Ant_Type_Idx, int & oj_i_Ant_Buff_Index )
{
	//��ȡ��ǰ�����ANT���ߵĽṹ��洢Indexֵ

	int iBuff_Idx;

	//ѭ��ɨ��ANT�����б�
	for( iBuff_Idx = 0; iBuff_Idx < str_Ant_List.i_ANT_Quan; iBuff_Idx++ )
	{
		if( eu_Ant_Type_Idx == str_Ant_List.strr_ANT_Prop_List[iBuff_Idx].eu_ANT_HW_Type_Name )
		{
			//�ҵ���Ŀ��ANT�ˣ��˴�����pass
			//��ֵ��������
			oj_i_Ant_Buff_Index = iBuff_Idx;
			//pass
			return true;
		}
	}//for( iBuff_Idx = 0; iBuff_Idx < str_Ant_List.i_ANT_Quan; iBuff_Idx++


	//��ʾ�쳣
	this->m_csError_Info.Format( _T( "ERROR, Get_ANT_Type_List_Buff_Index(), No ANT(%d) In List!" ), eu_Ant_Type_Idx );
	::AfxMessageBox( this->m_csError_Info );

	//��������ƥ�����ߣ��㷵��fail
	return false;
}


void CGFC::Empty_Struct_ANT_CFG_ALL_V2_S( ANT_CFG_ALL_V2_S & oj_str_ANT_Config_All )
{
	int iAnt_Idx;


	//��ա������б��ṹ��
	this->Empty_Struct_ANT_LIST_S( oj_str_ANT_Config_All.str_ANT_List );


	//��ա���������
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++ )
	{
		this->Empty_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_Config_All.strr_ANT_RF_Loss_List[iAnt_Idx] );
	}
}


void CGFC::Copy_Struct_ANT_CFG_ALL_V2_S( ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_Dest, ANT_CFG_ALL_V2_S str_ANT_CFG_Source )
{
	int iAnt_Idx;


	//Copy��ANT�����б��ṹ��
	this->Copy_Struct_ANT_LIST_S( oj_str_ANT_CFG_Dest.str_ANT_List, str_ANT_CFG_Source.str_ANT_List );


	//Copy����������
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++ )
	{
		//Copy������ V2���ṹ��
		this->Copy_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_CFG_Dest.strr_ANT_RF_Loss_List[iAnt_Idx], str_ANT_CFG_Source.strr_ANT_RF_Loss_List[iAnt_Idx] );

	}//for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++
}


void CGFC::Copy_Struct_ANT_CFG_ALL_V2_S_To_ANT_CFG_ALL_S( ANT_CFG_ALL_S & oj_str_ANT_CFG_V1_Dest, const ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_V2_Source )
{
	//�ѡ�ANT_CFG_ALL_V2_S���ṹ�� Copy��ֵ ����ANT_CFG_ALL_S���ṹ����

	int iAnt_Idx;
	EU_ANT_TYPE_INDEX euANT_Type_Name;


	//Copy��ANT�����б��ṹ��
	this->Copy_Struct_ANT_LIST_S( oj_str_ANT_CFG_V1_Dest.str_ANT_List, oj_str_ANT_CFG_V2_Source.str_ANT_List );


	//Copy����������
	for( iAnt_Idx = 0; iAnt_Idx < oj_str_ANT_CFG_V2_Source.str_ANT_List.i_ANT_Quan; iAnt_Idx++ )
	{
		//��ȡANT������
		euANT_Type_Name = oj_str_ANT_CFG_V2_Source.str_ANT_List.strr_ANT_Prop_List[iAnt_Idx].eu_ANT_HW_Type_Name;

		//�ж�
		if( (euANT_Type_Name < 0) || (euANT_Type_Name >= ANT_TYPE_IDX_COUNT) )
		{
			CString csTemp;
			//��ʾ�쳣
			csTemp.Format( _T( "ERROR, Copy_Struct_ANT_CFG_ALL_V2_S_To_ANT_CFG_ALL_S, ANT_Type_Name(%d) is wrong!" ), euANT_Type_Name );
			::AfxMessageBox( csTemp );
			//fail
			return;
		}

		//Copy�����𡱽ṹ��
		//�ѡ�V2 ANT���ߵ�����List������ṹ�壨ANT�����б�Index������洢����Copy����V1 ANT���ߵ�����ṹ�壨ANT�������洢������
		this->Copy_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_CFG_V1_Dest.strr_ANT_RF_Loss[euANT_Type_Name], oj_str_ANT_CFG_V2_Source.strr_ANT_RF_Loss_List[iAnt_Idx] );

	}//for( iAnt_Idx = 0; iAnt_Idx < oj_str_ANT_CFG_V2_Source.str_ANT_List.i_ANT_Quan; iAnt_Idx++
}


void CGFC::Copy_Struct_ANT_CFG_ALL_S_To_ANT_CFG_ALL_V2_S( ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_V2_Dest, const ANT_CFG_ALL_S & oj_str_ANT_CFG_V1_Source )
{
	//�ѡ�ANT_CFG_ALL_S���ṹ�� Copy��ֵ ����ANT_CFG_ALL_V2_S���ṹ����

	int iAnt_Idx;
	EU_ANT_TYPE_INDEX euANT_Type_Name;


	//Copy��ANT�����б��ṹ��
	this->Copy_Struct_ANT_LIST_S( oj_str_ANT_CFG_V2_Dest.str_ANT_List, oj_str_ANT_CFG_V1_Source.str_ANT_List );


	//Copy����������
	for( iAnt_Idx = 0; iAnt_Idx < oj_str_ANT_CFG_V1_Source.str_ANT_List.i_ANT_Quan; iAnt_Idx++ )
	{
		//��ȡANT������
		euANT_Type_Name = oj_str_ANT_CFG_V1_Source.str_ANT_List.strr_ANT_Prop_List[iAnt_Idx].eu_ANT_HW_Type_Name;

		//�ж�
		if( (euANT_Type_Name < 0) || (euANT_Type_Name >= ANT_TYPE_IDX_COUNT) )
		{
			CString csTemp;
			//��ʾ�쳣
			csTemp.Format( _T( "ERROR, Copy_Struct_ANT_CFG_ALL_S_To_ANT_CFG_ALL_V2_S, ANT_Type_Name(%d) is wrong!" ), euANT_Type_Name );
			::AfxMessageBox( csTemp );
			//fail
			return;
		}

		//Copy�����𡱽ṹ��
		//�ѡ�V1 ANT���ߵ�����ṹ�壨ANT�������洢����Copy����V2 ANT���ߵ�����List������ṹ�壨ANT�����б�Index������洢������
		this->Copy_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_CFG_V2_Dest.strr_ANT_RF_Loss_List[iAnt_Idx], oj_str_ANT_CFG_V1_Source.strr_ANT_RF_Loss[euANT_Type_Name] );

	}//for( iAnt_Idx = 0; iAnt_Idx < oj_str_ANT_CFG_V1_Source.str_ANT_List.i_ANT_Quan; iAnt_Idx++
}


bool CGFC::Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S( const ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_V2, EU_ANT_TYPE_INDEX eu_Ant_Type_Idx, int i_Format_Network, int i_Band_Name, int i_L_M_H, double & oj_d_RF_Loss_Tx_Input, double & oj_d_RF_Loss_Rx_Output )
{
	int iANT_Buff_Index = 0;//ANT���ߵĽṹ��洢index
	int iBand_Index = 0;//Band Ƶ�ε� index ��

	//
	try
	{
		//���L/M/Hֵ
		if( (i_L_M_H < CH_LOW) || (i_L_M_H > CH_HIGH) )
		{
			this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, L_M_H(%d) is Wrong!" ), i_L_M_H );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			throw(0);
		}


		//��ȡ��ǰ�����ANT���ߵĽṹ��洢index
		if( this->Get_ANT_Type_List_Buff_Index( oj_str_ANT_CFG_V2.str_ANT_List, eu_Ant_Type_Idx, iANT_Buff_Index ) == false )
		{
			//��ʾ������Ϣ
			this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No_ANT_%d_Buff_Index!" ), eu_Ant_Type_Idx );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			throw(0);
		}


		//������ʽswitch
		switch( i_Format_Network )//������ʽswitch
		{
		case FORMAT_GSM_S:
		case FORMAT_GSM_NS:
			{
				//2G�� i_Band_Name ����index��
				iBand_Index = i_Band_Name;

				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_GSM_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_GSM_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_WCDMA_S:
		case FORMAT_WCDMA_NS:
			{
				//Ƶ���� ת Ƶ��index
				if( this->Get_WCDMA_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//��ʾ������Ϣ
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No WCDMA Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_W_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_W_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_TD_NS:
			{
				//Ƶ���� ת Ƶ��index
				if( this->Get_TD_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//��ʾ������Ϣ
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No TD-SCDMA Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_TD_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_TD_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_C2K_S:
		case FORMAT_C2K_NS:
			{
				//Ƶ���� ת Ƶ��index
				if( this->Get_CDMA2000_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//��ʾ������Ϣ
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No cdma2000 Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_C2K_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_C2K_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_LTE_NS:
			{
				//Ƶ���� ת Ƶ��index
				if( this->Get_LTE_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//��ʾ������Ϣ
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No LTE Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_LTE_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_LTE_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_GPS_NS:
			{
				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_GPS_RF_Loss[RF_TX_UL_INPUT];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_GPS_RF_Loss[RF_RX_DL_OUTPUT];
			}
			//
			break;

		case FORMAT_WIFI_NS:
			{
				//Ƶ���� ת Ƶ��index
				if( this->Get_WiFi_Standard_FreqBand( i_Band_Name, &iBand_Index ) == false )
				{
					//��ʾ������Ϣ
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No WiFi_Standard_FreqBand(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_WiFi_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_WiFi_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_BLUETOOTH_NS:
			{
				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_BLU_RF_Loss[RF_TX_UL_INPUT];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_BLU_RF_Loss[RF_RX_DL_OUTPUT];
			}
			//
			break;

		case FORMAT_NBIOT_S:
		case FORMAT_NBIOT_NS:
			{
				//Ƶ���� ת Ƶ��index
				if( this->Get_NBIoT_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//��ʾ������Ϣ
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No NB-IoT Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_NBIoT_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_NBIoT_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_CV2X_NS:
			{
				//Ƶ���� ת Ƶ��index
				if( this->Get_CV2X_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//��ʾ������Ϣ
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No C-V2X Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_CV2X_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_CV2X_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_NR5GSUB6_NS:
			{
				//Ƶ���� ת Ƶ��index
				if( this->Get_NR_5G_SUB6_Band_Index( i_Band_Name, iBand_Index ) == false )
				{
					//��ʾ������Ϣ
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No NR_5G_SUB6 Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//��ȡ����ֵ����Ҫ������ֵ
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_NR_5G_SUB6_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_NR_5G_SUB6_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		default:
			//��ʾ������Ϣ
			this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No Format_Network(%d)!" ), i_Format_Network );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			throw(0);
		}//switch( i_Format_Network

	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_Rx_Chain_Type_Name( int i_Rx_Chain_Type, CString & oj_cs_Rx_Chain_Type_Name )
{
	//Rx_Chain���� ת Rx_Chain��

	//
	try
	{
		switch( i_Rx_Chain_Type )
		{
		case RX_CHAIN_MAIN:
			oj_cs_Rx_Chain_Type_Name.Format( _T( "MAIN" ) );
			break;
		case RX_CHAIN_DIV:
			oj_cs_Rx_Chain_Type_Name.Format( _T( "DIV" ) );
			break;
		case RX_CHAIN_GPS:
			oj_cs_Rx_Chain_Type_Name.Format( _T( "GPS" ) );
			break;
		case RX_CHAIN_MAIN_DIV:
			oj_cs_Rx_Chain_Type_Name.Format( _T( "MAIN_DIV" ) );
			break;
		default:
			//fail
			throw(0);
		}//switch( i_Rx_Chain_Type
	}
	catch( ... )
	{
		//fail��Ϣ
		this->m_csError_Info.Format( _T( "ERROR, Get_Rx_Chain_Type_Name, Rx_Chain_Type(%d) is Wrong!" ), i_Rx_Chain_Type );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Determine_Type_Of_Plan_Connect_ANT_And_RF_Port( const ANT_CFG_ALL_V2_S str_ANT_CFG_ALL_V2, int & oj_i_Plan_Connect_ANT_And_RF_Port )
{
	//�ж���ANT���������������RF�ڡ����߷���

	//
	try
	{
		//Ĭ��RF�����ӷ���
		oj_i_Plan_Connect_ANT_And_RF_Port = PLAN_ANT4_MAIN1_DIV1_GPS1_WIFI1;//��ͳ�ĵ�Main�����ߵķ���


		//�ж��Ƿ�������RF�����ӷ���
		if( (4 == str_ANT_CFG_ALL_V2.str_ANT_List.i_ANT_Quan) &&
			(ANT_TYPE_IDX_MAIN_1 == str_ANT_CFG_ALL_V2.str_ANT_List.strr_ANT_Prop_List[0].eu_ANT_HW_Type_Name) &&
			(ANT_TYPE_IDX_MAIN_2 == str_ANT_CFG_ALL_V2.str_ANT_List.strr_ANT_Prop_List[1].eu_ANT_HW_Type_Name) &&
			(ANT_TYPE_IDX_DIV_1 == str_ANT_CFG_ALL_V2.str_ANT_List.strr_ANT_Prop_List[2].eu_ANT_HW_Type_Name) &&
			(ANT_TYPE_IDX_DIV_2 == str_ANT_CFG_ALL_V2.str_ANT_List.strr_ANT_Prop_List[3].eu_ANT_HW_Type_Name) )
		{
			//ȷ����4��ANT����, ��2��MAIN������ + 2��DIV�ּ����ߡ�
			oj_i_Plan_Connect_ANT_And_RF_Port = PLAN_ANT4_MAIN2_DIV2;//2��MAIN������ + 2��DIV�ּ�����
		}

	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

//����ĳ���̣�process��
bool CGFC::Find_Process( const wchar_t * wcp_Process_Name )
{
	//

	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

	PROCESSENTRY32 strProcess_Info;
	CString csCheck, csTgt_Name;
	BOOL bStart= FALSE;

	BOOL bFind = FALSE;

	g_del_exe_Ctrl.Lock(); //

	try
	{
		//��ϵͳ�����еĽ����ĸ�����
		hProcessSnap = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

		if( hProcessSnap == INVALID_HANDLE_VALUE )
		{
			throw(0);
		}


		//����Ŀ����
		csTgt_Name.Format( _T( "%s" ), wcp_Process_Name );
		csTgt_Name.MakeLower();


		//��ʹ������ṹǰ�����������Ĵ�С
		strProcess_Info.dwSize = sizeof( PROCESSENTRY32 );

		//������ǰ�Ľ����б�Ѱ���Ƿ����û�����Ľ���
		//�������̿��գ�������ʾÿ�����̵���Ϣ
		bStart = Process32First( hProcessSnap, &strProcess_Info );

		while( bStart )
		{
			//��ý����б��еĽ�����
			csCheck.Format( _T( "%s" ), strProcess_Info.szExeFile );
			csCheck.MakeLower();

			if( csCheck.Compare( csTgt_Name ) == 0 )//Zero if the strings are identical
			{
			   	bFind = TRUE;
				break;
			}

			//����һ��
			bStart = Process32Next( hProcessSnap, &strProcess_Info );
		}//while(bStart

		//ע�⣺�ر��������ķ�ʽ��������
		CloseHandle( hProcessSnap );

		if (TRUE == bFind)
		{
			g_del_exe_Ctrl.Unlock();
			return true;
		}
		else
		{
			g_del_exe_Ctrl.Unlock();
			return false;
		}
	}
	catch( ... )
	{
		g_del_exe_Ctrl.Unlock();
		return false;
	}

	g_del_exe_Ctrl.Unlock();
	return true;
}
