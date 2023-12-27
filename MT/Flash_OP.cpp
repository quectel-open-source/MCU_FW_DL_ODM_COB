#include "StdAfx.h"
#include "Flash_OP.h"

//struct DL_HANDLE * g_DL_HANDLE = NULL;
//struct DA_HANDLE * g_DA_HANDLE = NULL;
CCriticalSection g_Get_Dynamic_COM;


//cb Show_State
static void __stdcall cb_show_state_r(const wchar_t * wcp_State, void * p_usr_Arg)
{
	CFlash_OP * pSv = (CFlash_OP *)p_usr_Arg;

	pSv->Show_Status( wcp_State );
	pSv->Show_Out(wcp_State);
}



//���ڶ� ocp �����Ļص�
//cb Load_produ_params_gen
static bool __stdcall cb_load_produ_params_gen_r(const wchar_t * wcp_Produ_Name, int i_Produ_Type, int i_Produ_Ver, void * p_usr_Arg)
{
	bool bRtn;

	//
#ifdef FW_MULTI
	bRtn = ::OCP_LOAD_PRODU_PARAMS_GEN( wcp_Produ_Name, i_Produ_Type, i_Produ_Ver );
#endif
	//
	return bRtn;
}

//cb ocp_get_error_info
static void __stdcall cb_ocp_get_error_info_r(CString * csp_Error_Info, void * p_usr_Arg)
{
#ifdef FW_MULTI
	::OCP_GET_ERROR_INFO( csp_Error_Info );
#endif
}

//cb Read_Produ_GSM_Band_List
static bool __stdcall cb_read_produ_gsm_band_list_r(int * ip_GSM_Band_List, int * ip_GSM_Band_Quan, void * p_usr_Arg)
{
	bool bRtn;

	//
#ifdef FW_MULTI
	bRtn = ::OCP_READ_PRODU_GSM_BAND_LIST( ip_GSM_Band_List, ip_GSM_Band_Quan );
#endif
	//
	return bRtn;
}

//cb Read_Produ_WCDMA_Band_List
static bool __stdcall cb_read_produ_wcdma_band_list_r(int * ip_WCDMA_Band_List, int * ip_WCDMA_Band_Quan, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_WCDMA_BAND_LIST( ip_WCDMA_Band_List, ip_WCDMA_Band_Quan );
#endif
	//
	return bRtn;
}

//cb Read_Produ_C2K_Band_List
static bool __stdcall cb_read_produ_c2k_band_list_r(int * ip_C2K_Band_List, int * ip_C2K_Band_Quan, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_C2K_BAND_LIST( ip_C2K_Band_List, ip_C2K_Band_Quan );
#endif
	//
	return bRtn;
}

//cb Read_Produ_TD_Band_List
static bool __stdcall cb_read_produ_td_band_list_r(int * ip_TD_Band_List, int * ip_TD_Band_Quan, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_TD_BAND_LIST( ip_TD_Band_List, ip_TD_Band_Quan );
#endif
	//
	return bRtn;
}

//cb Read_Produ_LTE_Band_List
static bool __stdcall cb_read_produ_lte_band_list_r(int * ip_LTE_Band_List, int * ip_LTE_Band_Quan, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_LTE_BAND_LIST( ip_LTE_Band_List, ip_LTE_Band_Quan );
#endif
	//
	return bRtn;
}

//cb Read_produ_pf_and_bb_chip
static bool __stdcall cb_read_produ_pf_and_bb_chip_r(int * ip_Platform_Solution, int * ip_BB_Chip, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_PF_AND_BB_CHIP( ip_Platform_Solution, ip_BB_Chip );
#endif
	//
	return bRtn;
}

//cb Read_Produ_TS_Type_Seq
static bool __stdcall cb_read_produ_ts_type_seq_r(int * ip_TS_Type_Seq, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_TS_TYPE_SEQ( ip_TS_Type_Seq );
#endif
	//
	return bRtn;
}

//cb Read_Produ_Init_Time
static bool __stdcall cb_read_produ_init_time_r(int * ip_Init_Time, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_INIT_TIME( ip_Init_Time );
#endif
	//
	return bRtn;
}

//cb Read_Produ_Cal_Tree_Name
static bool __stdcall cb_read_produ_cal_tree_name_r(CString * csp_Cal_Tree_Name, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_CAL_TREE_NAME( csp_Cal_Tree_Name );
#endif
	//
	return bRtn;
}

//cb Read_produ_volt_and_curr
static bool __stdcall cb_read_produ_volt_and_curr_r(double * dp_Voltage, double * dp_Current, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_VOLT_AND_CURR( dp_Voltage, dp_Current );
#endif
	//
	return bRtn;
}

//cb read_produ_lte_cat_r
static bool __stdcall cb_read_produ_lte_cat_r(int * ip_LTE_CAT, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_LTE_CAT( ip_LTE_CAT );
#endif
	//
	return bRtn;
}

//cb read_produ_landi_enable_r
static bool __stdcall cb_read_produ_landi_enable_r(bool * bp_Landi_Enable, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_LANDI_ENABLE( bp_Landi_Enable );
#endif
	//
	return bRtn;
}

//cb read_produ_wifi_enable_r
static bool __stdcall cb_read_produ_wifi_enable_r(bool * bp_WiFi_Enable, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_WIFI_ENABLE( bp_WiFi_Enable );
#endif
	//
	return bRtn;
}

//cb read_produ_nw_scan_mode_r 
static bool __stdcall cb_read_produ_nw_scan_mode_r(int * ip_Nw_Scan_Mode, void * p_usr_Arg)
{
	bool bRtn;
#ifdef FW_MULTI
	//
	bRtn = ::OCP_READ_PRODU_NW_SCAN_MODE( ip_Nw_Scan_Mode );
#endif
	//
	return bRtn;
}






//
CFlash_OP::CFlash_OP(void)
: m_iCom_Port(0)
, m_bRuning(false)
, m_iClass_Idx(0)
, m_csError_Code(_T(""))
, m_usROMcount(0)
, m_csBin_Product(_T(""))
, m_iBin_Flash(0)
, m_iComm_Method(0)
, m_iOP_TYPE(0)
, m_iStop_Flag(0)
, m_iNV_Retain_File_Quan(0)
, m_iMeta_Handle(0)
, m_ui_nand_acccon(0)
, m_iFw_Type(0)
, m_csCurr_Exe_Path(_T(""))
, m_csScat_Cfg_File_Path(_T(""))
, m_strNewFlashID(_T(""))
, m_iError_Code(0)
, m_MC_flag(0)
, m_bMCXX_Old(FALSE)
, m_iQFLASH_CMD_MTK(0)
, m_bPLC_Flag(false)
, m_iPLC_Port(0)
{
	for(int num=0;num<FLASH_ID_NUM;num++)
	{
		m_strFLASH_ID[num].Format(_T(""));
	}
	m_iFLASH_NUM =0;

	m_DL_HANDLE=NULL;
	m_DA_HANDLE=NULL;

	//
	m_uiFLASH_START_ADDR=0;  
	m_uiFLASH_ADDR_LENGTH=0x00300000; 
	
}

CFlash_OP::~CFlash_OP(void)
{
	UN_INIT_DA_HANDLE();
	UN_INIT_DL_HANDLE();
	UN_INIT_META_DLL();
}


//---------------------------------------------------------------------------
static int __stdcall cb_format_conn_report_init(unsigned int begin_addr, unsigned int length, void *usr_arg)
{

	CString strInfo;
	strInfo.Format(_T("Format Connectivity Moudle flash... ADDR[0x%08X], LEN[0x%08X]."),begin_addr,length);


	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	pSv->Show_Status(strInfo);
	pSv->Show_Out(strInfo);

	return 0;
}

static int __stdcall cb_download_da_init(void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	pSv->Show_Status(_T("Download DA: 0%"));
	pSv->Show_Out(_T("Download DA: 0%"));

	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(_T("Download DA: 0%"));
	}

    return 0;
}

static int __stdcall cb_download_da(unsigned char percent, unsigned int sent_bytes, unsigned int total_bytes, void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;
	CString csData;
	int iPercent = percent;

	csData.Format(_T("Download DA: %d%%"), iPercent);
	pSv->Show_Status(csData);
	//pSv->Show_Out(csData);
	pSv->Show_Progress(iPercent);

	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(csData);
	}
	
	return 0;
}

static int __stdcall cb_da_report(const DA_REPORT_T * p_da_report, void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	char * cpHW_DetectionReport = NULL;//[2048];
	char * cpNor_FlashHint = NULL;//[512];
	char * cpNand_FlashHint = NULL;//[512];
	char * cpData = NULL;//[256];
	wchar_t * wcpTemp = NULL;//[256];

	CString strData;
	
	int iTemp;
	int iResult = 0;
	
	try
	{
		//�����ڴ�
		cpHW_DetectionReport = (char *)calloc(2050, sizeof(char));
		cpNor_FlashHint = (char *)calloc(520, sizeof(char));
		cpNand_FlashHint = (char *)calloc(520, sizeof(char));
		cpData = (char *)calloc(256, sizeof(char));

		//����DA Report
		iTemp = ConvertDAReportToString(p_da_report,
			cpNor_FlashHint, 512, cpNand_FlashHint, 512, cpHW_DetectionReport, 2048);

		//--------------------------------------------------------------	
		if(iTemp == 0)
		{
			/*
			sprintf(cpData, "[%s] %s", p_da_report->m_bbchip_name, cpNor_FlashHint);
			t->ShowFlashDetails(cpData);
			
			char crTemp[3000];
			sprintf(crTemp,
				"m_bbchip_type[0x%X]\nm_bbchip_name[%s]\nm_bbchip_hw_ver[0x%X]\nm_bbchip_sw_ver[0x%X]\nm_bbchip_hw_code[0x%X]\nm_nor_flash_id[0x%X]\nnor_flash_dev_code_1[0x%04X]\nnor_flash_dev_code_2[0x%04X]\nnor_flash_dev_code_3[0x%04X]\nnor_flash_dev_code_4[0x%04X]\n%s\nm_strNorFlashHint[%s]",
				p_da_report->m_bbchip_type, p_da_report->m_bbchip_name,
				p_da_report->m_bbchip_hw_ver, p_da_report->m_bbchip_sw_ver, p_da_report->m_bbchip_hw_code,
				p_da_report->m_nor_flash_id,
				p_da_report->m_nor_flash_dev_code_1, p_da_report->m_nor_flash_dev_code_2,
				p_da_report->m_nor_flash_dev_code_3, p_da_report->m_nor_flash_dev_code_4,
				m_strHWDetectionReport, m_strNorFlashHint);
			::AfxMessageBox(crTemp);
			g_stopflag = BOOT_STOP;
			return iResult;
			*/
			// ɸѡ�Ϸ���FLASH ID
			UINT32 uFLASH_ID = (UINT32)p_da_report->m_nor_flash_dev_code_3;
			uFLASH_ID = (uFLASH_ID<<8)|(UINT32)p_da_report->m_nor_flash_dev_code_2;
			uFLASH_ID = (uFLASH_ID<<8)|(UINT32)p_da_report->m_nor_flash_dev_code_1;

			CString strFLASH_ID;
			strFLASH_ID.Format(_T("0x%08X"),uFLASH_ID);

			pSv->m_strFLASH_ID_Current = strFLASH_ID;
			//0922

			BOOL  bHAS_Flag =FALSE;

			//pSv->m_MC_flag =1;//���عر�
			//20180604
			if(1 == pSv->m_MC_flag)
			{
				bHAS_Flag = FALSE;
				if (TRUE == pSv->m_bMCXX_Old) //�ϰ汾
				{
					if(uFLASH_ID != FLASH_GD25LQ32_ID)
					{
						int num=0;
						for (num=0;(num<pSv->m_iFLASH_NUM) && (num<FLASH_ID_NUM);num++)
						{
							if (strFLASH_ID == pSv->m_strFLASH_ID[num])
							{
								bHAS_Flag = TRUE;
								break;
							}
						}
					}
					else //uFLASH_ID == FLASH_GD25LQ32_ID �Ϸ�
					{
						bHAS_Flag = TRUE;
					}
				} 
				else //�°汾
				{
					if ((uFLASH_ID == FLASH_GD25LQ32_ID)
						|| (uFLASH_ID == FLASH_W25Q32BV_ID)
						|| (uFLASH_ID == FLASH_W25Q32JV_ID) //��ʱ��Ϊ�Ƿ�I
						|| (uFLASH_ID == FLASH_MX25L3291FWJI_09_ID)
						)
					{
						bHAS_Flag = TRUE;
					}
					else //��ͨ�������ļ�Params_Main.ini����һ��ȷ��
					{
						int num=0;
						for (num=0;(num<pSv->m_iFLASH_NUM) && (num<FLASH_ID_NUM);num++)
						{
							if (strFLASH_ID == pSv->m_strFLASH_ID[num])
							{
								bHAS_Flag = TRUE;
								break;
							}
						}
					}
				}		
				
			}
			else
			{
				if ((uFLASH_ID == FLASH_GD25LQ32_ID)
					|| (uFLASH_ID == FLASH_W25Q32BV_ID)
					|| (uFLASH_ID == FLASH_W25Q32JV_ID) //��ʱ��Ϊ�Ƿ�I
					|| (uFLASH_ID == FLASH_W25Q64DW_ID)
					|| (uFLASH_ID == FLASH_W25Q128FW_ID)
					|| (uFLASH_ID == FLASH_MX25L3291FWJI_09_ID)
					|| (uFLASH_ID == FLASH_GD25LQ128CWIG)//M36 FC
					|| (uFLASH_ID == FLASH_W25Q128FW_EXT_ID)//20190411
					)
				{
					bHAS_Flag = TRUE;
				}
				else //��ͨ�������ļ�Params_Main.ini����һ��ȷ��
				{
					int num=0;
					for (num=0;(num<pSv->m_iFLASH_NUM) && (num<FLASH_ID_NUM);num++)
					{
						if (strFLASH_ID == pSv->m_strFLASH_ID[num])
						{
							bHAS_Flag = TRUE;
							break;
						}
					}
				}


			}

			if(FALSE == bHAS_Flag)
			{
				//�Ƿ���FLASH ID
				pSv->m_strNewFlashID = strFLASH_ID;
				iResult = -5;
				throw(iResult);

			}
		
			
			//Flash���Ҽ�飨ͨ��BIN�ļ�����ȡFlash����������ģ��ʵ�ʼ��Flash���Һ˶ԣ�
			switch(pSv->m_iBin_Flash)
			{
			case NDF_FLASH://BIN�ļ��޹�˾�����Ͳ��˶���
				break;
			case MX_FLASH:
				if(p_da_report->m_nor_flash_dev_code_1 != 0x00C2)//���깫˾
				{
					iResult = -2;
				}
				break;
			case WINBOND_FLASH:
				if(p_da_report->m_nor_flash_dev_code_1 != 0x00EF)//WINBOND��˾
				{
					iResult = -2;
				}
				break;
			case NUMONYX_FLASH:
				if(p_da_report->m_nor_flash_dev_code_1 != 0x0020)//NUMONYX��˾
				{
					iResult = -2;
				}
				break;
			case MICRON_FLASH:
				if(p_da_report->m_nand_flash_dev_code_1 != 0x002C)//Micron���⹫˾0x002C��Nand Flash
				{
					iResult = -4;
				}
				break;
			case SST_FLASH:
				if(p_da_report->m_nor_flash_dev_code_1 != 0x00BF)//SST��˾
				{
					iResult = -2;
				}
				break;
			case TOSHIBA_FLASH:
				if(p_da_report->m_nor_flash_dev_code_1 != 0x0098)//TOSHIBA��˾
				{
					iResult = -2;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			//����DA Reportʧ��
			iResult = -1;	
		}
		//�ж���ǰ�����Ƿ���Fail
		if(iResult != 0)
		{
			throw(iResult);
		}
	}
	catch(...)
	{
		//�ó�����ֹ����
		pSv->m_iStop_Flag = BOOT_STOP;

		strData.Format(_T("Flash ID[0x%04X 0x%04X 0x%04X 0x%04X]"),
			p_da_report->m_nor_flash_dev_code_1, p_da_report->m_nor_flash_dev_code_2,
			p_da_report->m_nor_flash_dev_code_3, p_da_report->m_nor_flash_dev_code_4);
		pSv->Show_Out(strData);


		//ʧ����Ϣ
		if(iResult == -1)
		{
			sprintf(cpData, "Decode DA Report Fail [0x%04X 0x%04X 0x%04X 0x%04X]",
				p_da_report->m_nor_flash_dev_code_1, p_da_report->m_nor_flash_dev_code_2,
				p_da_report->m_nor_flash_dev_code_3, p_da_report->m_nor_flash_dev_code_4);
		}
		else if(iResult == -2)
		{
			sprintf(cpData, "Flash Match Fail [0x%04X 0x%04X 0x%04X 0x%04X] %s",
				p_da_report->m_nor_flash_dev_code_1, p_da_report->m_nor_flash_dev_code_2,
				p_da_report->m_nor_flash_dev_code_3, p_da_report->m_nor_flash_dev_code_4, 
				cpNor_FlashHint);
		}
		else if(iResult == -4)//Nand Flash
		{
			sprintf(cpData, "Flash Match Fail [0x%04X 0x%04X 0x%04X 0x%04X] %s",
				p_da_report->m_nand_flash_dev_code_1, p_da_report->m_nand_flash_dev_code_2,
				p_da_report->m_nand_flash_dev_code_3, p_da_report->m_nand_flash_dev_code_4, 
				cpNand_FlashHint);
		}
		else if(iResult == -3)
		{
			sprintf(cpData, "Flash LIC Fail [0x%04X 0x%04X 0x%04X 0x%04X] %s",
				p_da_report->m_nor_flash_dev_code_1, p_da_report->m_nor_flash_dev_code_2,
				p_da_report->m_nor_flash_dev_code_3, p_da_report->m_nor_flash_dev_code_4, 
				cpNor_FlashHint);
		}
		else if(iResult == -5)//�Ƿ�ID
		{

			sprintf(cpData, "Flash ID illegal [0x%04X 0x%04X 0x%04X 0x%04X]",
				p_da_report->m_nor_flash_dev_code_1, p_da_report->m_nor_flash_dev_code_2,
				p_da_report->m_nor_flash_dev_code_3, p_da_report->m_nor_flash_dev_code_4);

			strData.Format(_T("�µ�Flash ID[0x%04X 0x%04X 0x%04X 0x%04X] ����ϵפ��������Ա����!"),
				p_da_report->m_nor_flash_dev_code_1, p_da_report->m_nor_flash_dev_code_2,
				p_da_report->m_nor_flash_dev_code_3, p_da_report->m_nor_flash_dev_code_4);

			::AfxMessageBox(strData);	

		}


		//�����ڴ�
		wcpTemp = (wchar_t *)calloc(256, sizeof(wchar_t));
		wmemset(wcpTemp, 0, 256);
		//ת����ʽ
		pSv->m_cla_GFC.MultiByte_To_WideChar(cpData, wcpTemp, &iTemp);
		if(iTemp >= 256)
		{
			::AfxMessageBox(_T("ERROR, MultiByte_To_WideChar, Buff is over!"));
		}

		//��ʾ
		pSv->m_csError_Code.Format(_T("%s"), wcpTemp);
		pSv->Show_Status(pSv->m_csError_Code);
		pSv->Show_Out(pSv->m_csError_Code);

		if(1== pSv->m_iQFLASH_CMD_MTK) 
		{
			pSv->Show_Dos(pSv->m_csError_Code);
		}

	
		Sleep(300);
		
		//�ͷ��ڴ�
		if(cpHW_DetectionReport != NULL)
		{
			free(cpHW_DetectionReport);
		}
		if(cpNor_FlashHint != NULL)
		{
			free(cpNor_FlashHint);
		}
		if(cpNand_FlashHint != NULL)
		{
			free(cpNand_FlashHint);
		}
		if(cpData != NULL)
		{
			free(cpData);
		}
		if(wcpTemp != NULL)
		{
			free(wcpTemp);
		}
		
		return iResult;
	}

// 	strData.Format(_T("Flash ID[0x%04X 0x%04X 0x%04X 0x%04X]"),
// 		p_da_report->m_nor_flash_dev_code_1, p_da_report->m_nor_flash_dev_code_2,
// 		p_da_report->m_nor_flash_dev_code_3, p_da_report->m_nor_flash_dev_code_4);
// 	pSv->Show_Out(strData);


	//�ͷ��ڴ�
	if(cpHW_DetectionReport != NULL)
	{
		free(cpHW_DetectionReport);
	}
	if(cpNor_FlashHint != NULL)
	{
		free(cpNor_FlashHint);
	}
	if(cpNand_FlashHint != NULL)
	{
		free(cpNand_FlashHint);
	}
	if(cpData != NULL)
	{
		free(cpData);
	}
	if(wcpTemp != NULL)
	{
		free(wcpTemp);
	}

	return iResult;
}

static int __stdcall cb_download_flash_init(void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	pSv->Show_Status(_T("Download Bin: 0%"));
	pSv->Show_Out(_T("Download Bin: 0%"));
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(_T("Download Bin: 0%"));
	}
    return 0;
}

static int __stdcall cb_download_flash(unsigned char percent, unsigned int sent_bytes, unsigned int total_bytes, void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;
	CString csData;
	int iPercent = percent;

	csData.Format(_T("Download Bin: %d%%"), iPercent);
	pSv->Show_Status(csData);
	pSv->Show_Progress(iPercent);
	//pSv->Show_Out(csData);
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(csData);
	}
    return 0;
}

static int __stdcall cb_format_report_init(HW_StorageType_E storage_type, unsigned int begin_addr, unsigned int length, void * usr_arg)
{
    CFlash_OP * pSv = (CFlash_OP *)usr_arg;
	CString csData;

	switch(storage_type)
	{
	case HW_STORAGE_NOR:
		csData.Format(_T("Format NOR flash... ADDR[0x%08X], LEN[0x%08X]. "), begin_addr, length);
		break;
	case HW_STORAGE_NAND:
		csData.Format(_T("Format NAND flash... ADDR[0x%08X], LEN[0x%08X]. "), begin_addr, length);
		break;
	case HW_STORAGE_EMMC:
		unsigned long long format_size = length * 512; //unit 512 bytes
		csData.Format(_T("Format eMMC flash...SIZE[0x%011X]. "), format_size);
		break;
	}

	pSv->Show_Status(csData);
	pSv->Show_Out(csData);
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(csData);
	}
    return 0;
}

static int __stdcall cb_format_report(unsigned char percent, void * usr_arg)
{
    CFlash_OP * pSv = (CFlash_OP *)usr_arg;
	CString csData;
	int iPercent = percent;

	csData.Format(_T("Format Flash: %d%%"), iPercent);
	pSv->Show_Status(csData);
	//pSv->Show_Out(csData);
	pSv->Show_Progress(iPercent);
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(csData);
	}
    return 0;
}

static int __stdcall cb_download_bloader_init(void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	pSv->Show_Status(_T("Download Bootloader: 0%"));
	pSv->Show_Out(_T("Download Bootloader: 0%"));
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(_T("Download Bootloader: 0%"));
	}
    return 0;
}

static int __stdcall cb_download_bloader(unsigned char currProgress, unsigned int finished_bytes, unsigned int total_bytes, void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;
	CString csData;

	csData.Format(_T("Download Bootloader: %d%c"), currProgress, '%');
	pSv->Show_Status(csData);
	//pSv->Show_Out(csData);
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(csData);
	}
    return 0;
}

static int __stdcall cb_boot_meta_in_brom_stage(unsigned int brom_handle, HANDLE hCOM, void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	pSv->Show_Status(_T("Boot meta in brom stage......"));
	pSv->Show_Out(_T("Boot meta in brom stage......"));
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(_T("Boot meta in brom stage......"));
	}
	Sleep(1000);

    return 0;
}

static int __stdcall cb_pre_format_flash_init(void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	pSv->Show_Status(_T("Pre-Format: 0%"));
	pSv->Show_Out(_T("Pre-Format: 0%"));
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(_T("Pre-Format: 0%"));
	}
    return 0;
}

static int __stdcall cb_pre_format_flash(unsigned char percent, void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;
	CString csData;

	csData.Format(_T("Pre-Format: %d%%"), percent);
	pSv->Show_Status(csData);
	//pSv->Show_Out(csData);
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(csData);
	}
    return 0;
}

static int __stdcall cb_update_param_init(void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	pSv->Show_Status(_T("Update Params: 0%"));
	pSv->Show_Out(_T("Update Params: 0%"));
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(_T("Update Params: 0%"));
	}
    return 0;
}

static int __stdcall cb_update_param(unsigned char percent, void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;
	CString csData;

	csData.Format(_T("Update Params: %d%%"), percent);
	pSv->Show_Status(csData);
	//pSv->Show_Out(csData);
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(csData);
	}
    return 0;
}

static int __stdcall cb_post_process_init(void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	pSv->Show_Status(_T("Post Process: 0%"));
	pSv->Show_Out(_T("Post Process: 0%"));
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(_T("Post Process: 0%"));
	}
    return 0;
}

static int __stdcall cb_post_process(unsigned char percent, void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;
	CString csData;

	csData.Format(_T("Post Process: %d%%"), percent);
	pSv->Show_Status(csData);
	//pSv->Show_Out(csData);
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(csData);
	}
    return 0;
}

static int __stdcall cb_download_conn_init(void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	pSv->Show_Status(_T("Connectivity module image to flash now: 0%"));
	pSv->Show_Out(_T("Connectivity module image to flash now: 0%"));
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(_T("Connectivity module image to flash now: 0%"));
	}
    return 0;
}

static int __stdcall cb_download_conn(unsigned char finished_percentage, unsigned int finished_bytes, unsigned int total_bytes, void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;
	CString csData;

	csData.Format(_T("Connectivity module image to flash now: %d%%"), finished_percentage);
	pSv->Show_Status(csData);
	//pSv->Show_Out(csData);
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(csData);
	}
    return 0;
}

static int __stdcall cb_download_conn_da_init(void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;

	pSv->Show_Status(_T("Download Connectivity Module DA now: 0%"));
	pSv->Show_Out(_T("Download Connectivity Module DA now: 0%"));
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(_T("Download Connectivity Module DA now: 0%"));
	}
    return 0;
}

static int __stdcall cb_download_conn_da(unsigned char finished_percentage, unsigned int finished_bytes, unsigned int total_bytes, void * usr_arg)
{
	CFlash_OP * pSv = (CFlash_OP *)usr_arg;
	CString csData;

	csData.Format(_T("Download Connectivity Module DA now: %d%%"), finished_percentage);
	pSv->Show_Status(csData);
	//pSv->Show_Out(csData);
	if(1== pSv->m_iQFLASH_CMD_MTK) 
	{
		pSv->Show_Dos(csData);
	}
    return 0;
}




bool CFlash_OP::LOAD_DA_FILE(const wchar_t * wcp_DA_File_Path)
{
	int iRtn, iTemp;
	char * cpPath = NULL;

	try
	{
		//�����ڴ�
		//����˫charת��char���ڴ棨ת��ʱ����char�Ŀռ������˫char��������
		iTemp = wcslen(wcp_DA_File_Path);
		cpPath = (char *)calloc((iTemp * 2), sizeof(char));
		memset(cpPath, 0, (iTemp * 2));

		//·������
		//˫char����תΪ��char����
		if(this->m_cla_GFC.WideChar_To_MultiByte(wcp_DA_File_Path, cpPath, &iTemp) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, WideChar_To_MultiByte"));
			throw(0);
		}

		//�������DA�������������
		if(m_DA_HANDLE != NULL)
		{
			this->UN_INIT_DA_HANDLE();
		}
		
		//����DA���
		iRtn = DA_Create(&m_DA_HANDLE);
		if(iRtn != S_DONE)
		{
			this->m_csError_Code.Format(_T("FAIL, DA_Create, Rtn = %d"), iRtn);
			throw(0);
		}

		//����DA�ļ�
		iRtn = DA_Load(m_DA_HANDLE, cpPath, _FALSE);	
		if(iRtn != S_DONE)
		{
			this->m_csError_Code.Format(_T("FAIL, DA_Load, Rtn = %d"), iRtn);
			throw(0);
		}
		
	}
	catch(...)
	{
		this->UN_INIT_DA_HANDLE();

		//�ͷ��ڴ�
		if(cpPath != NULL)
		{
			free(cpPath);
			cpPath = NULL;
		}
		return false;
	}

	//�ͷ��ڴ�
	if(cpPath != NULL)
	{
		free(cpPath);
		cpPath = NULL;
	}
	return true;
}

bool CFlash_OP::LOAD_SCAT_FILE(const wchar_t * wcp_Scat_File_Path)
{
	int iRtn, iTemp, i;

	try
	{
		//20180604
		CString strScatPath;
		strScatPath.Format(_T("%s"),wcp_Scat_File_Path);

		strScatPath.MakeUpper();

		if (wcsstr(strScatPath,_T("MC20")) != NULL)
		{
			m_MC_flag=1;
		}
		else 
		if (wcsstr(strScatPath,_T("MC60")) != NULL)
		{
			m_MC_flag=1;
		}	
		else
		{
			m_MC_flag=0;
		}


		//·������
		memset(this->m_crScat_File_Path, 0, sizeof(this->m_crScat_File_Path));
		//˫char����תΪ��char����
		if(this->m_cla_GFC.WideChar_To_MultiByte(wcp_Scat_File_Path, this->m_crScat_File_Path, &iTemp) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, WideChar_To_MultiByte"));
			throw(0);
		}

		//----------------------------------- ��ȡROMSetting��Ϣ
		//��սṹ��
		memset(&this->m_str_RomSetting, 0, sizeof(this->m_str_RomSetting));
		//
		iRtn = GetROMSetting(this->m_crScat_File_Path, &this->m_str_RomSetting);
		if((iRtn != S_DONE) || (this->m_str_RomSetting.version <= 0))
		{
			this->m_csError_Code.Format(_T("FAIL, GetROMSetting, Rtn = %d, romSetting version = %d"), iRtn, this->m_str_RomSetting.version);
			throw(0);
		}
		//----------------------------------- 

		//�������DL_HANDLE����������
		if(m_DL_HANDLE != NULL)
		{
			this->UN_INIT_DL_HANDLE();
		}

		//����DL_HANDLE
		DL_Create(&m_DL_HANDLE);

		//Create ROM Entries
		iRtn = DL_CreateROMEntries(m_DL_HANDLE, &this->m_str_RomSetting);
		if(iRtn != S_DONE)
		{
			this->m_csError_Code.Format(_T("FAIL, DL_CreateROMEntries, Rtn = %d"), iRtn);
			throw(0);
		}

		//�ж�RomSetting.version
		if(this->m_str_RomSetting.version == 1)
		{
			// For legacy SW load����ͨ��.txt��ʽScat�ļ����壩
			iRtn = DL_AutoLoadByScatRegionName(m_DL_HANDLE, this->m_crScat_File_Path, NORMAL_ROM | RESOURCE_BIN | JUMPTABLE_BIN, _TRUE);
			
			if(iRtn != S_DONE)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_AutoLoadByScatRegionName, Rtn = %d"), iRtn);
				throw(0);
			}
		}
		else//.CFG�ļ������ this->m_str_RomSetting.version = 2
		{
			//U10 "m_str_RomSetting.version != 1"
			iRtn = DL_AutoLoadROMFiles(m_DL_HANDLE);
			
			if(iRtn != S_DONE)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_AutoLoadROMFiles, Rtn = %d"), iRtn);
				throw(0);
			}
		}

		//
		iRtn = DL_SetPacketLength(m_DL_HANDLE, 4096);
		if(iRtn != S_DONE)
		{
			this->m_csError_Code.Format(_T("FAIL, DL_SetPacketLength, Rtn = %d"), iRtn);
			throw(0);
		}

		//���ROM�İ汾����ͨ�Ļ���Ex��
		if(this->m_str_RomSetting.version == 1)
		{
			//���DL_INFO�ṹ��
			memset(&this->m_str_DL_Info, 0, sizeof(this->m_str_DL_Info));
			
			//��ȡDL_INFO��Ϣ
			DL_GetInfo(m_DL_HANDLE, &this->m_str_DL_Info);

			//�����RomSetting.version == 1�����ж��Ƿ���NFB Flash
			if(this->m_str_DL_Info.m_config.m_is_nfb)
			{
//				::AfxMessageBox(_T("NFB Flash"));
				/*
				const vector<wstring>::iterator result = find(downloadList.begin(),
					downloadList.end(),
					wstring(L"BOOTLOADER"));
				const bool bootloaderEnabled = (result != downloadList.end());
				*/
				
				iRtn = DL_BL_SetEnableAttr(m_DL_HANDLE, _TRUE);//(bootloaderEnabled ? _TRUE : _FALSE));
				
				if(iRtn != S_DONE)
				{
					this->m_csError_Code.Format(_T("FAIL, DL_BL_SetEnableAttr, Rtn = %d"), iRtn);
					throw(0);
				}
			}

			//��BIN�ļ�����Ŀ
			iRtn = DL_GetCount(m_DL_HANDLE, &this->m_usROMcount);

			//����BIN�ļ�
			if(iRtn == S_DONE)
			{
				//������BIN�ļ���·��
				if((DL_Rom_GetInfoAll(m_DL_HANDLE, this->m_strr_Rom_Info, MAX_LOAD_SECTIONS) == S_DONE) &&
					(this->m_usROMcount < MAX_LOAD_SECTIONS) &&
					(this->m_usROMcount > 0))
				{
					for(i=0; i<this->m_usROMcount; i++)
					{
						//����
						iRtn = DL_Rom_SetEnableAttr(m_DL_HANDLE, i, _TRUE);

						if(iRtn != S_DONE)
						{
							this->m_csError_Code.Format(_T("FAIL, DL_Rom_SetEnableAttr [%d]"), i);
							throw(0);
						}

						/*
						iRtn = DL_Rom_Load(::g_DL_HANDLE, i, this->m_strr_Rom_Info[i].filepath);

						if(iRtn != S_DONE)
						{
							this->m_csError_Code.Format(_T("FAIL, DL_Rom_Load [%d]"), i);
							throw(0);
						}
						*/
					}
				}
			}
			else
			{
				this->m_csError_Code.Format(_T("FAIL, DL_GetCount, Rtn = %d"), iRtn);
				throw(0);
			}
		}//if(this->m_str_RomSetting.version == 1)
		else//this->m_str_RomSetting.version != 1
		{
			//��Ex BIN������
			iRtn = DL_GetCount_Ex(m_DL_HANDLE, &this->m_usROMcount);

			if(iRtn == S_DONE)
			{
				if((this->m_usROMcount < MAX_LOAD_SECTIONS) && (this->m_usROMcount > 0))
				{
					//������BIN�ļ���·����ȫ����Ϣ
					iRtn = DL_Rom_GetInfoAll_Ex(m_DL_HANDLE, this->m_strr_Rom_Info_Ex, MAX_LOAD_SECTIONS);

					if(iRtn == S_DONE)
					{
						for(i=0; i<this->m_usROMcount; i++)
						{
							//����
							iRtn = DL_Rom_SetEnableAttr_Ex(m_DL_HANDLE, i, _TRUE);

							if(iRtn != S_DONE)
							{
								this->m_csError_Code.Format(_T("FAIL, DL_Rom_SetEnableAttr_Ex [%d]"), i);
								throw(0);
							}
						}
					}
					else
					{
						this->m_csError_Code.Format(_T("FAIL, DL_Rom_GetInfoAll_Ex, Rtn = %d"), iRtn);
						throw(0);
					}
				}
			}
			else
			{
				this->m_csError_Code.Format(_T("FAIL, DL_GetCount_Ex, Rtn = %d"), iRtn);
				throw(0);
			}
		}//this->m_str_RomSetting.version != 1


		//----------------------------------- ��ȡDL_INFO_EX��Ϣ
		//���DL_INFO_EX�ṹ��
		memset(&this->m_str_DL_Info_Ex, 0, sizeof(this->m_str_DL_Info_Ex));
		//��ȡDL_INFO_EX��Ϣ
		iRtn = DL_GetInfo_Ex(m_DL_HANDLE, &this->m_str_DL_Info_Ex);
		if(iRtn != S_DONE)
		{
			this->m_csError_Code.Format(_T("FAIL, DL_GetInfo_Ex, Rtn = %d"), iRtn);
			throw(0);
		}
		//-----------------------------------

		//����һ����������BIN�ļ���Ϣ
		iRtn = DL_GetCount_Ex(m_DL_HANDLE, &this->m_usROMcount);
		if(iRtn == S_DONE)
		{
			if((this->m_usROMcount < MAX_LOAD_SECTIONS) && (this->m_usROMcount > 0))
			{
				//������BIN�ļ���·����ȫ����Ϣ
				iRtn = DL_Rom_GetInfoAll_Ex(m_DL_HANDLE, this->m_strr_Rom_Info_Ex, MAX_LOAD_SECTIONS);

				if(iRtn != S_DONE)
				{
					this->m_csError_Code.Format(_T("FAIL, DL_Rom_GetInfoAll_Ex-Load_Scat, Rtn = %d"), iRtn);
					throw(0);
				}

				//���
				for(i=0; i<this->m_usROMcount; i++)
				{
					//���rom_type�����Ƿ�����
					if((V_ERROR_ROM <= this->m_strr_Rom_Info_Ex[i].rom_type) && (this->m_strr_Rom_Info_Ex[i].rom_type <= V_ERROR_ROM_END))
					{
						this->m_csError_Code.Format(_T("FAIL, Error ROM %d, 0x%08X"), i, this->m_strr_Rom_Info_Ex[i].rom_type);
						throw(0);
					}
				}//for(i=0; i<this->m_usROMcount;
			}//if((this->m_usROMcount
		}
		else
		{
			this->m_csError_Code.Format(_T("FAIL, DL_GetCount_Ex-Load_Scat, Rtn = %d"), iRtn);
			throw(0);
		}
	}
	catch(...)
	{
		this->UN_INIT_DL_HANDLE();

		return false;
	}

	return true;
}

bool CFlash_OP::UN_INIT_DL_HANDLE(void)
{
	int iRtn;
	
	try
	{
		if(m_DL_HANDLE != NULL)
		{
			iRtn = DL_Rom_UnloadAll(m_DL_HANDLE);
			if(iRtn != S_DONE)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Rom_UnloadAll, Rtn = %d"), iRtn);
				throw(0);
			}

			iRtn = DL_Destroy(&m_DL_HANDLE);
			if(iRtn != S_DONE)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Destroy, Rtn = %d"), iRtn);
				throw(0);
			}

			m_DL_HANDLE = NULL;
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CFlash_OP::UN_INIT_DA_HANDLE(void)
{
	int iRtn;
	
	try
	{
		if(m_DA_HANDLE != NULL)
		{
			//
			iRtn = DA_Unload(m_DA_HANDLE);
			if(iRtn != S_DONE)
			{
				this->m_csError_Code.Format(_T("FAIL, DA_Unload, Rtn = %d"), iRtn);
				throw(0);
			}

			//
			iRtn = DA_Destroy(&m_DA_HANDLE);
			if(iRtn != S_DONE)
			{
				this->m_csError_Code.Format(_T("FAIL, DA_Destroy, Rtn = %d"), iRtn);
				throw(0);
			}

			m_DA_HANDLE = NULL;
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void CFlash_OP::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CFlash_OP::Show_Dos(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_DOS, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}
int CFlash_OP::Do_Execute()
{
	int iRtn;

	this->m_iStop_Flag = -1;
	this->m_bRuning = true;

	this->m_csError_Code.Format(_T(""));

	if(this->m_iComm_Method == IN_UART)
	{
		//20190904

		//ȫ������
		//iRtn = this->Format_Internal();

		iRtn = this->DL_IN_UART();

	}
	else if(this->m_iComm_Method == IN_USB)
	{
		iRtn = this->DL_IN_USB();
	}

	this->m_iStop_Flag = -1;
	this->m_bRuning = false;

	return iRtn;
}

int CFlash_OP::DL_IN_UART(void)
{
	COM_PORT_SETTING str_Com_Setting;
	FLASHTOOL_ARG str_FT_ARG;
	FLASHTOOL_RESULT str_FT_Result;
	FLASHTOOL_DOWNLOAD_ARG str_FT_DL_ARG;
	FLASHTOOL_DOWNLOAD_RESULT str_FT_DL_Result;
	int iRtn;
	CString csData;

	try
	{
		//------------------------------------- ���ԡ�ͨ�ſڡ��Ƿ����� -------------------------------------
		//��ʾ��ǰ������Ϣ
// 		csData.Format(_T("Check COM%d port is enabled......"), this->m_iCom_Port);
// 		this->Show_Status(csData);
		//
		this->m_cla_Com_Port.m_iPort = this->m_iCom_Port;
		this->m_cla_Com_Port.m_iBaudRate = 115200;
// 		//��ͨ�ſ�
// 		if(this->m_cla_Com_Port.Port_Open( 400 ) == false)
// 		{
// 			//
// 			this->m_csError_Code.Format(_T("COM%d port disabled"), this->m_iCom_Port);
// 			//
// 			csData.Format(_T("ERROR, %s, port disabled!"), this->m_csError_Code);
// 			//
// 			this->Show_Status(csData);
// 
// 			m_iError_Code = S_COM_PORT_OPEN_FAIL;
// 			//
// 			throw(0);
// 		}
// 
// 		//�ӳ�
// 		Sleep(300);
// 
// 		//���ԡ��ر�ͨ�ſڡ�
// 		this->m_cla_Com_Port.Port_Close();
		//-------------------------------------

		//��ʼ��ʾ
		this->Show_Status(_T("Waiting......"));
		this->Show_Out(_T("Waiting..."));

		if(1== m_iQFLASH_CMD_MTK) 
		{
			Show_Dos(_T("Waiting(Module Power On or Reset)......"));
		}


		//------------------------------ Com Port Setting ------------------------------
		this->Arg_Com_Port_Setting(&str_Com_Setting);

		//------------------------------ FLASHTOOL_ARG, FLASHTOOL_RESULT ------------------------------
		this->Arg_Flash_Tool(&str_FT_ARG, &str_FT_Result, false);

		//------------------------------ FLASHTOOL_DOWNLOAD_ARG, FLASHTOOL_DOWNLOAD_RESULT ------------------------------
		this->Arg_Flash_Tool_Download(&str_FT_DL_ARG, &str_FT_DL_Result);

		//���ؿ�ʼ
		iRtn = FlashDownload(&str_Com_Setting, &str_FT_ARG, &str_FT_Result, &str_FT_DL_ARG, &str_FT_DL_Result, &this->m_iStop_Flag);

		//�ж����ؽ��
		m_iError_Code = iRtn;
		if(!this->Process_FlashTool_Result(iRtn))
		{
			if((FALSE == this->m_csError_Code.IsEmpty())&&(FALSE == this->m_strFLASH_ID_Current.IsEmpty()))
			{
				this->m_csError_Code.Format(_T("%s-(%s)"),this->m_csError_Code,m_strFLASH_ID_Current);
			}
			else //20190406 add
			{
				this->m_csError_Code.Format(_T("%s"),this->m_csError_Code);
			}
			
			csData.Format(_T("FAIL, %s"), this->m_csError_Code);
			this->Show_Status(csData);
			this->Show_Out(csData);
			if(1== m_iQFLASH_CMD_MTK) 
			{
				Show_Dos(csData);
			}
			//
			throw(0);
		}
		if(this->m_iOP_TYPE == MTK_OPTION_DL_CLEAR_NV)
		{
#if 0
			//����NV�ļ�����
			Sleep(600);
			//20181011 app_version: APPGS3MDM32A02_THZ
			this->Show_Status(_T("Check_APP_VER..."));
			if (false == Check_APP_VER())
			{
				this->Show_Progress(300.0);
				throw(0);
			} 
			else
			{
				this->Show_Status(_T("Check_APP_VER Pass"));
			}
#endif			


#if 1
			if(!this->Enter_META_Mode(str_FT_ARG.m_boot_arg.m_bbchip_type, str_FT_ARG.m_boot_arg.m_ext_clock))
			{
				this->Show_Status(_T("FW upgrade success."));//�岻�ɹ���Ҳ��Ϊ��PASS
				this->Show_Status(csData);
				if(1== m_iQFLASH_CMD_MTK) 
				{
					Show_Dos(_T("FW upgrade success."));
				}

				this->Show_Progress((int)300.0);
				//throw(0);
			}
			else
			{
				if(!this->Clear_Nv_File())
				{
					this->Show_Status(_T("FW upgrade success."));//�岻�ɹ���Ҳ��Ϊ��PASS
					if(1== m_iQFLASH_CMD_MTK) 
					{
						Show_Dos(_T("FW upgrade success."));
					}

					this->Show_Progress((int)300.0);
					//throw(0);
				}
				else
				{
					this->Show_Status(_T("FW upgrade success."));
					if(1== m_iQFLASH_CMD_MTK) 
					{
						Show_Dos(_T("FW upgrade success."));
					}

					this->Show_Progress((int)300.0);
				}
			}
#endif 
		this->Show_Progress((int)300.0);
			
		}
		else
		{
			CString csData;

			//delay for download OK
			for(int a=10; a<=100;)
			{
				csData.Format(_T("Confirm FW: %d%%"), a);
				this->Show_Status(csData);
				if(1== m_iQFLASH_CMD_MTK) 
				{
					Show_Dos(csData);
				}

				this->Show_Progress(a);

				a += 10;

				Sleep(400);
			}
			this->Show_Status(_T("FW download success."));
			if(1== m_iQFLASH_CMD_MTK) 
			{
				Show_Dos(_T("FW download success."));
			}
			this->Show_Progress((int)400.0);
		}

		this->Show_Status(_T("FW download success."));
		this->Show_Out(_T("FW download success."));
		if(1== m_iQFLASH_CMD_MTK) 
		{
			Show_Dos(_T("FW download success."));
		}
	}
	catch(...)
	{
		return RTN_FAIL;
	}

	return RTN_SUCCESS;
}

bool CFlash_OP::INIT_META_DLL(void)
{
	META_RESULT eu_meta_ret;

	try
	{
		//�������
		eu_meta_ret = META_GetAvailableHandle(&this->m_iMeta_Handle);
		if(!this->Process_Meta_Result(eu_meta_ret, _T("META_GetAvailableHandle")))
		{
			throw(0);
		}

		//��ʼ�����
		eu_meta_ret = META_Init_r(this->m_iMeta_Handle, NULL);
		if(!this->Process_Meta_Result(eu_meta_ret, _T("META_Init_r")))
		{
			throw(0);
		}

	}
	catch(...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}

//	this->Show_Status(_T("INIT_META_DLL SUCCESS"));
	return true;
}

void CFlash_OP::UN_INIT_META_DLL(void)
{
	META_Deinit_r(&this->m_iMeta_Handle);
}

bool CFlash_OP::Enter_META_Mode(const BBCHIP_TYPE eu_BB_Chip, const EXT_CLOCK eu_Ext_Clock)
{
	if(this->m_iComm_Method == IN_UART)
	{
		return this->Enter_META_Mode_In_Uart(eu_BB_Chip, eu_Ext_Clock);
	}
	else if(this->m_iComm_Method == IN_USB)
	{
		return this->Enter_META_Mode_In_USB(eu_BB_Chip, eu_Ext_Clock);
	}
	else if(this->m_iComm_Method == IN_UART_USB)
	{
		return this->Enter_META_Mode_In_USB(eu_BB_Chip, eu_Ext_Clock);
	}
	else
	{
		return false;
	}
}

bool CFlash_OP::Enter_META_Mode_In_Uart(const BBCHIP_TYPE eu_BB_Chip, const EXT_CLOCK eu_Ext_Clock)
{
	if(this->m_iStop_Flag == BOOT_STOP)
	{
		throw(0);//��ͣ
	}

	META_Connect_Req str_Meta_Conn_Req;
	META_Connect_Report str_Meta_Conn_Report;

	META_RESULT eu_meta_ret;

	static META_COMM_BAUDRATE  meta_com_baudrate[11] =
	{
		META_BAUD115200,
		META_BAUD230400,
		META_BAUD460800,
		META_BAUD921600,
		META_BAUD_END,
		META_BAUD_END,
		META_BAUD_END,
		META_BAUD_END,
		META_BAUD_END,
		META_BAUD_END,
		META_BAUD_END
	}; 

	try
	{
		this->Show_Status(_T("Enter meta mode in uart......"));

		//------------------------------ META_Connect_Req ------------------------------
		memset(&str_Meta_Conn_Req, 0, sizeof(str_Meta_Conn_Req));
		//COM��
		str_Meta_Conn_Req.com_port = this->m_iCom_Port;
		//������
		memcpy(str_Meta_Conn_Req.baudrate, meta_com_baudrate, sizeof(str_Meta_Conn_Req.baudrate));

		//
		str_Meta_Conn_Req.flowctrl = META_SW_FLOWCTRL;
		str_Meta_Conn_Req.ms_connect_timeout = 40000;
		str_Meta_Conn_Req.boot_meta_arg.m_ms_boot_timeout = 40000;

		str_Meta_Conn_Req.boot_meta_arg.m_bbchip_type = eu_BB_Chip;
		str_Meta_Conn_Req.boot_meta_arg.m_ext_clock = eu_Ext_Clock;

		str_Meta_Conn_Req.boot_meta_arg.m_ms_boot_timeout = BOOT_INFINITE;
		str_Meta_Conn_Req.boot_meta_arg.m_max_start_cmd_retry_count = DEFAULT_BROM_START_CMD_RETRY_COUNT;

		str_Meta_Conn_Req.boot_meta_arg.m_cb_in_brom_stage      = ::cb_boot_meta_in_brom_stage;
		str_Meta_Conn_Req.boot_meta_arg.m_cb_in_brom_stage_arg  = this;

		// speed-up BootROM stage baudrate
		str_Meta_Conn_Req.boot_meta_arg.m_speedup_brom_baudrate = _TRUE;

		// WM_BROM_READY_TO_POWER_ON_TGT
		str_Meta_Conn_Req.boot_meta_arg.m_ready_power_on_wnd_handle	= NULL;
		str_Meta_Conn_Req.boot_meta_arg.m_ready_power_on_wparam		= NULL;
		str_Meta_Conn_Req.boot_meta_arg.m_ready_power_on_lparam		= NULL;

		// serial link authentication arg
		str_Meta_Conn_Req.boot_meta_arg.m_auth_handle = NULL;//g_AUTH_HANDLE;
		str_Meta_Conn_Req.boot_meta_arg.m_cb_sla_challenge         = SLA_Challenge;
		str_Meta_Conn_Req.boot_meta_arg.m_cb_sla_challenge_arg     = this;
		str_Meta_Conn_Req.boot_meta_arg.m_cb_sla_challenge_end     = SLA_Challenge_END;
		str_Meta_Conn_Req.boot_meta_arg.m_cb_sla_challenge_end_arg = this;

		// EDGE card
		str_Meta_Conn_Req.boot_meta_arg.m_cb_com_init_stage = NULL;//::cb_PowerOnOxford;
		str_Meta_Conn_Req.boot_meta_arg.m_cb_com_init_stage_arg = this;

		// Boot META, usb flag, must to set false, since call the META_ConnectWithTarget() API
		str_Meta_Conn_Req.boot_meta_arg.m_usb_enable = _FALSE;

		// META Without Battery,add by wuxiong in 2012/02/20
		str_Meta_Conn_Req.boot_meta_arg.m_enable_without_battery = _FALSE;//MainForm->USBDownloadWOBattery->Checked;

		//------------------------------ META_Connect_Report ------------------------------
		memset(&str_Meta_Conn_Report, 0, sizeof(str_Meta_Conn_Report));

		//��ʼ����
		eu_meta_ret = META_ConnectWithTarget_r(this->m_iMeta_Handle, &str_Meta_Conn_Req, &this->m_iStop_Flag, &str_Meta_Conn_Report);
		if(!this->Process_Meta_Result(eu_meta_ret, _T("META_ConnectWithTarget_r")))
		{
			this->Show_Status(this->m_csError_Code);
			if(1== m_iQFLASH_CMD_MTK) 
			{
				Show_Dos(this->m_csError_Code);
			}
			throw(0);
		}
	}
	catch(...)
	{
		return false;
	}

	this->Show_Status(_T("Enter meta mode in uart success."));
	if(1== m_iQFLASH_CMD_MTK) 
	{
		Show_Dos(_T("Enter meta mode in uart success."));
	}
	return true;
}

bool CFlash_OP::Enter_META_Mode_In_USB(const BBCHIP_TYPE eu_BB_Chip, const EXT_CLOCK eu_Ext_Clock)
{
	if(this->m_iStop_Flag == BOOT_STOP)
	{
		throw(0);//��ͣ
	}

	META_ConnectByUSB_Req str_Meta_Conn_Req;
	META_ConnectByUSB_Report str_Meta_Conn_Report;

	META_RESULT eu_meta_ret;

	try
	{
		this->Show_Status(_T("Enter meta mode in usb......"));
		if(1== m_iQFLASH_CMD_MTK) 
		{
			Show_Dos(_T("Enter meta mode in usb......"));
		}
		//------------------------------ ����������USB�� ------------------------------ 
		if(!this->Search_Dynamic_Com_Port(&this->m_iCom_Port))
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		if(this->m_iStop_Flag == BOOT_STOP)
		{
			throw(0);//��ͣ
		}

		//------------------------------ META_Connect_Req ------------------------------
		this->Arg_META_Conn_Req_In_USB(&str_Meta_Conn_Req, &str_Meta_Conn_Report);

		//��ʼ����
		eu_meta_ret = META_ConnectWithTargetByUSB_r(this->m_iMeta_Handle, &str_Meta_Conn_Req, &this->m_iStop_Flag, &str_Meta_Conn_Report);

		if(!this->Process_Meta_Result(eu_meta_ret, _T("META_ConnectWithTargetByUSB")))
		{
			this->Show_Status(this->m_csError_Code);
			if(1== m_iQFLASH_CMD_MTK) 
			{
				Show_Dos(this->m_csError_Code);
			}
			throw(0);
		}
	}
	catch(...)
	{
		return false;
	}

	this->Show_Status(_T("Enter meta mode in usb success."));
	if(1== m_iQFLASH_CMD_MTK) 
	{
		Show_Dos(_T("Enter meta mode in usb success."));
	}
	return true;
}

bool CFlash_OP::Process_Meta_Result(const META_RESULT eu_Meta_Result, const wchar_t * wcpMETA_FUNC)
{
	//��ͣ
	if(this->m_iStop_Flag == BOOT_STOP)
	{
		this->m_csError_Code.Format(_T("Operation has been stopped!"));
		return false;
	}

	if(eu_Meta_Result == META_SUCCESS)
	{
		return true;
	}
	else
	{
		char * cpTemp = NULL;
		wchar_t * wcpData = NULL;
		int iTemp;

		//�����ڴ�
		cpTemp = (char *)calloc(256, sizeof(char));
		memset(cpTemp, 0, 256);
		wcpData = (wchar_t *)calloc(256, sizeof(wchar_t));
		wmemset(wcpData, 0, 256);

		sprintf(cpTemp, "%s", META_GetErrorString(eu_Meta_Result));

		//ת����ʽ
		this->m_cla_GFC.MultiByte_To_WideChar(cpTemp, wcpData, &iTemp);

		this->m_csError_Code.Format(_T("FAIL, %s, [%d], %s"), wcpMETA_FUNC, (int)eu_Meta_Result, wcpData);

		//�ͷ��ڴ�
		if(cpTemp != NULL)
		{
			free(cpTemp);
			cpTemp = NULL;
		}
		if(wcpData != NULL)
		{
			free(wcpData);
			wcpData = NULL;
		}

		return false;
	}
}

bool CFlash_OP::Clear_Nv_File(void)
{
	META_RESULT eu_meta_ret;
	int iMatched_File, i, k;
	short stToken;
	int i_fat_file_count = 0;//Ŀ¼��FAT�ļ�����Ŀ
	char cr2_FAT_File_List[500][MAX_PATH];
	bool b_Un_Del;
	CString csData;
	double dTemp;

	try
	{
		this->Show_Status(_T("Init clear nv files......"));
		if(1== m_iQFLASH_CMD_MTK) 
		{
			Show_Dos(_T("Init clear nv files......"));
		}
		//-------------------------------- ��ʼ����ϵͳ�� -------------------------------- 
		//MT6252�Ƕ�D(Z)�̣�D(Z)����ϵͳ�̣�C���Ǹ��ͻ��õġ�MT6223��ϵͳ����Z(C)�̡�Z:\\NVRAM\\NVD_DATA
		//Z:\NVRAM\NVD_DATA\MT00A000; Z:\NVRAM\NVD_DATA\PACKALID
		i_fat_file_count = 0;

		if(this->Read_Nv_FAT_File_List("Z:\\", cr2_FAT_File_List, &i_fat_file_count) != RTN_SUCCESS)
		{
			this->Show_Status(this->m_csError_Code);
			if(1== m_iQFLASH_CMD_MTK) 
			{
				Show_Dos(this->m_csError_Code);
			}
			throw(0);
		}

		if(i_fat_file_count <= 0)
		{
			this->Show_Status(_T("FAIL, Do not find any nv files."));
			if(1== m_iQFLASH_CMD_MTK) 
			{
				Show_Dos(_T("FAIL, Do not find any nv files."));
			}
			throw(0);
		}

		//��ʼ�����ļ�
		iMatched_File = 0;//ͳ����Ҫ�������ļ���Ŀ

		for(i = 0; i<i_fat_file_count;)//�����ȡ��FAT�ļ������ƺ˶�ȷ��
		{
			//��ͣ
			if(this->m_iStop_Flag == BOOT_STOP)
			{
				throw(0);
			}

			//��ʼ���趨��־Ϊ��ɾ��
			b_Un_Del = false;

			for(k = 0; k<this->m_iNV_Retain_File_Quan; k++)//��txt�ļ��еĸ����ļ�����FAT�˶�
			{
				//��ͣ
				if(this->m_iStop_Flag == BOOT_STOP)
				{
					throw(0);
				}

				if(strstr(cr2_FAT_File_List[i], this->m_cr2NV_Retain_File[k]) != NULL)
				{
					/*
					char cr[600];
					wchar_t wcTep[1200];
					int p;
					sprintf(cr, "%s <%s>", cr2_FAT_File_List[i], m_cr2NV_Retain_File[k]);
					this->MultiByte_To_WideChar(cr, wcTep, &p);
					::AfxMessageBox(wcTep);
					*/

					b_Un_Del = true;//���FAT�ļ��Ǹ���������ɾ�������ļ�
					iMatched_File++;
					break;
				}
			}

			//��ʼɾ����ɾ���ļ�
			if(b_Un_Del == false)
			{
				//��ͣ
				if(this->m_iStop_Flag == BOOT_STOP)
				{
					throw(0);
				}

				//ɾ������
				eu_meta_ret = META_FAT_Delete_r(this->m_iMeta_Handle, cr2_FAT_File_List[i], &stToken);

				if((eu_meta_ret != META_SUCCESS) && (eu_meta_ret != META_MAUI_DB_INCONSISTENT))
				{
					this->Process_Meta_Result(eu_meta_ret, _T("META_FAT_Delete_r, Sys"));
					this->Show_Status(this->m_csError_Code);
					if(1== m_iQFLASH_CMD_MTK) 
					{
						Show_Dos(this->m_csError_Code);
					}
					throw(0);
				}

				Sleep(120);
			}

			//����
			i++;

			dTemp = 90.0 * i / i_fat_file_count;//ϵͳ������ռ��Clear Nv����90������
			csData.Format(_T("Clear Nv Sys Files [%d/%d]: %.2f%%"), iMatched_File, i_fat_file_count, dTemp);
			this->Show_Status(csData);
			this->Show_Progress((int)dTemp);
			if(1== m_iQFLASH_CMD_MTK) 
			{
				Show_Dos(csData);
			}
		}//for(i = 0; i<i_fat_file_count

		if(iMatched_File == 0)
		{
			this->Show_Status(_T("FAIL, There are not matched files in NV!"));
			if(1== m_iQFLASH_CMD_MTK) 
			{
				Show_Dos(_T("FAIL, There are not matched files in NV!"));
			}
			throw(1);
		}

		//-------------------------------- ��ʼ�����û��̣�����ϵͳ�У� -------------------------------- 
		i_fat_file_count = 0;

		if(this->Read_Nv_FAT_File_List("C:\\UFS", cr2_FAT_File_List, &i_fat_file_count) == RTN_SUCCESS)
		{
			if(i_fat_file_count > 0)
			{
				//��ʼ�����ļ���ȫ�������
				for(i = 0; i<i_fat_file_count; i++)
				{
					//��ͣ
					if(this->m_iStop_Flag == BOOT_STOP)
					{
						throw(0);
					}

					/*
					char cr[600];
					wchar_t wcTep[1200];
					int p;
					sprintf(cr, "%d/%d: %s", i, i_fat_file_count, cr2_FAT_File_List[i]);
					this->MultiByte_To_WideChar(cr, wcTep, &p);
					::AfxMessageBox(wcTep);
					*/

					//ɾ������
					eu_meta_ret = META_FAT_Delete_r(this->m_iMeta_Handle, cr2_FAT_File_List[i], &stToken);

					if((eu_meta_ret != META_SUCCESS) && (eu_meta_ret != META_MAUI_DB_INCONSISTENT))
					{
						this->Process_Meta_Result(eu_meta_ret, _T("META_FAT_Delete_r, User"));
						if(1== m_iQFLASH_CMD_MTK) 
						{
							Show_Dos(this->m_csError_Code);
						}
						this->Show_Status(this->m_csError_Code);
						throw(0);
					}

					Sleep(100);

					dTemp = 90.0 + 10.0 * i / i_fat_file_count;//�û�������ռ��Clear Nv����10������
					csData.Format(_T("Clear Nv User Files [%d/%d]: %.2f%%"), (i + 1), i_fat_file_count, dTemp);
					this->Show_Status(csData);
					if(1== m_iQFLASH_CMD_MTK) 
					{
						Show_Dos(csData);
					}
					this->Show_Progress((int)dTemp);
				}//for(i = 0; i<i_fat_file_count;
			}//if(i_fat_file_count > 0
		}//if(this->Read_Nv_FAT_File_List("C:\\UFS\\",
		
		//��Clear Nv��������100������
		this->Show_Progress(100);
	}
	catch(...)
	{
		this->Exit_META_Mode();
		return false;
	}

	//�˳�META
	this->Exit_META_Mode();
	return true;
}

bool CFlash_OP::Exit_META_Mode(void)
{
	META_RESULT eu_meta_ret;

	eu_meta_ret = META_DisconnectInMetaMode_r(this->m_iMeta_Handle);

	if(!this->Process_Meta_Result(eu_meta_ret, _T("META_DisconnectInMetaMode_r")))
	{
		this->Show_Status(this->m_csError_Code);
		if(1== m_iQFLASH_CMD_MTK) 
		{
			Show_Dos(this->m_csError_Code);
		}
		return false;
	}

	return true;
}

bool CFlash_OP::Process_FlashTool_Result(int i_Status_ID)
{
	//��ͣ
	if(this->m_iStop_Flag == BOOT_STOP)
	{
		if(wcsstr(this->m_csError_Code, _T("[0x")) == NULL)//�ֶ���ͣ
		{
			this->m_csError_Code.Format(_T("DL_has_been_stopped!"));
			this->m_iError_Code = BOOT_STOP;
		}

		return false;
	}

	if( i_Status_ID == S_DONE)
	{
		return true;
	}
	else
	{
		this->m_iError_Code = i_Status_ID;

		if (i_Status_ID >= S_SV5X_ERROR_END)
		{
			this->m_csError_Code.Format(_T("ERR_%d_Unknown_Cause"), i_Status_ID);
		}
		else
		{
			char * cpTemp = NULL;
			wchar_t * wcpData = NULL;
			int iTemp;

			//�����ڴ�
			cpTemp = (char *)calloc(256, sizeof(char));
			memset(cpTemp, 0, 256);
			wcpData = (wchar_t *)calloc(256, sizeof(wchar_t));
			wmemset(wcpData, 0, 256);

			sprintf(cpTemp, "%s", StatusToString(i_Status_ID));

			//ת����ʽ
			this->m_cla_GFC.MultiByte_To_WideChar( cpTemp, wcpData, &iTemp );

			this->m_csError_Code.Format(_T("ERR_%d_%s"), i_Status_ID, wcpData);

			//�ͷ��ڴ�
			if(cpTemp != NULL)
			{
				free(cpTemp);
				cpTemp = NULL;
			}
			if(wcpData != NULL)
			{
				free(wcpData);
				wcpData = NULL;
			}

		}
		   



		return false;
	}
}

void CFlash_OP::Arg_Com_Port_Setting(COM_PORT_SETTING * strp_Com_Setting)
{
	memset(strp_Com_Setting, 0 , sizeof(*strp_Com_Setting));

	strp_Com_Setting->com.number		= this->m_iCom_Port;
	strp_Com_Setting->baudrate			= this->m_eu_BR_UART;
	strp_Com_Setting->ms_read_timeout	= COM_DEFAULT_TIMEOUT;
	strp_Com_Setting->ms_write_timeout	= COM_DEFAULT_TIMEOUT;
}

void CFlash_OP::Arg_Flash_Tool(FLASHTOOL_ARG * strp_FT_Arg, FLASHTOOL_RESULT * strp_FT_Result, bool b_In_USB)
{
	//------------------------------ FLASHTOOL_ARG ------------------------------
	memset(strp_FT_Arg, 0, sizeof(*strp_FT_Arg));

	strp_FT_Arg->m_boot_arg.m_bbchip_type = this->m_eu_BB_Chip;
	strp_FT_Arg->m_boot_arg.m_ext_clock = this->m_eu_Ext_Clock;
	strp_FT_Arg->m_boot_arg.m_ms_boot_timeout = 30000;


	strp_FT_Arg->m_boot_arg.m_max_start_cmd_retry_count = DEFAULT_BROM_START_CMD_RETRY_COUNT;

	strp_FT_Arg->m_boot_arg.m_cb_com_init_stage     = NULL;//COMPortOpenCallback;
	strp_FT_Arg->m_boot_arg.m_cb_com_init_stage_arg = NULL;
	strp_FT_Arg->m_boot_arg.m_cb_in_brom_stage      = NULL;
	strp_FT_Arg->m_boot_arg.m_cb_in_brom_stage_arg  = NULL;

	// speed-up BootROM stage baudrate
	strp_FT_Arg->m_boot_arg.m_speedup_brom_baudrate = _TRUE;

	// Application's window handle to send WM_BROM_READY_TO_POWER_ON_TGT message
	strp_FT_Arg->m_boot_arg.m_ready_power_on_wnd_handle = NULL;
	strp_FT_Arg->m_boot_arg.m_ready_power_on_wparam     = NULL;
	strp_FT_Arg->m_boot_arg.m_ready_power_on_lparam     = NULL;

	// serial link authentication arg
	strp_FT_Arg->m_boot_arg.m_auth_handle               = NULL;//authHandle;
	strp_FT_Arg->m_boot_arg.m_cb_sla_challenge          = SLA_Challenge;
	strp_FT_Arg->m_boot_arg.m_cb_sla_challenge_arg      = NULL;
	strp_FT_Arg->m_boot_arg.m_cb_sla_challenge_end      = SLA_Challenge_END;
	strp_FT_Arg->m_boot_arg.m_cb_sla_challenge_end_arg  = NULL;

	// security certificate
	// Maggie ToDo: export the parameter to set secure control certificate
	strp_FT_Arg->m_boot_arg.m_scert_handle = NULL;

	// EMI Control for bank0 and bank1
	strp_FT_Arg->m_boot_arg.m_p_bank0_mem_cfg = NULL;
	strp_FT_Arg->m_boot_arg.m_p_bank1_mem_cfg = NULL;

	// Maggie ToDo: in GUI mode, daHandle could be zero, see g_bSkipBootRom variable
	strp_FT_Arg->m_boot_arg.m_da_handle = m_DA_HANDLE;

	// da download progress callack
	strp_FT_Arg->m_boot_arg.m_cb_download_da_init     = ::cb_download_da_init;
	strp_FT_Arg->m_boot_arg.m_cb_download_da_init_arg = this;
	strp_FT_Arg->m_boot_arg.m_cb_download_da          = ::cb_download_da;
	strp_FT_Arg->m_boot_arg.m_cb_download_da_arg      = this;

	// download by USB
	strp_FT_Arg->m_boot_arg.m_usb_enable = (_BOOL)b_In_USB;//(_BOOL) g_bUsbDLRB;
	strp_FT_Arg->m_boot_arg.m_usb_com_port_control = USB_COM_PORT_CONTROL_NO_OPERATION;
	strp_FT_Arg->m_boot_arg.m_enable_usb_2_0 = _TRUE;

	//TODO:: fix memory setting for Multi-load on SV5(MT6250��MT6276)
	GetExternalMemorySetting(this->m_crScat_File_Path, &strp_FT_Arg->m_boot_arg.m_external_memory_setting[0]);

	// root certificate
	strp_FT_Arg->m_boot_arg.m_rootcert_handle       = NULL;
	strp_FT_Arg->m_boot_arg.m_rootcert_force_assign = _FALSE;
	strp_FT_Arg->m_boot_arg.m_disable_long_press_power_key = _TRUE;//g_bDisableLongPressPWKEY;


	// fill FLASHTOOL_ARG
	strp_FT_Arg->m_cb_da_report     = ::cb_da_report;
	strp_FT_Arg->m_cb_da_report_arg = this;

	strp_FT_Arg->m_cb_security_pre_process_notify     = NULL;
	strp_FT_Arg->m_cb_security_pre_process_notify_arg = NULL;
	strp_FT_Arg->m_baudrate_full_sync_count           = 1;
	strp_FT_Arg->m_nor_chip_select[0]                 = CS0;
	strp_FT_Arg->m_nor_chip_select[1]                 = CS_WITH_DECODER;
	strp_FT_Arg->m_nand_chip_select                   = CS0;//g_bNFI_SelectCS1?CS1:CS0;
	
	if (true ==  b_In_USB) //20191126
	{
		strp_FT_Arg->m_enable_download_without_battery    = _TRUE;
	} 
	else
	{
		strp_FT_Arg->m_enable_download_without_battery    = _FALSE;
	}
	
	

	strp_FT_Arg->m_disable_flashID_comparison		  = _FALSE;

	this->m_ui_nand_acccon = 0x20022124;
	strp_FT_Arg->m_p_nand_acccon = NULL;//&this->m_ui_nand_acccon;//g_EMI_NFI_Setting.m_bAuto?NULL:&(g_EMI_NFI_Setting.m_nand_acccon);

	// get the DRAM EMI Setting from UI
	// EMI������������ʼ��Ext-Ram�ģ���ΪDA�����ص�Ext-Ram����������ǰ��Ҫ��ʼ��Ext-Ram
	// EMI���ù�ȥҪ�ڽ������õģ����ڿ�����CFG�ļ�ͨ����GetExternalMemorySetting����������ʹ�����������ڿ��Բ�������
	strp_FT_Arg->m_enable_ui_dram_cfg = _FALSE;//ext_mem1Form->m_EnableDRAMSetting->Checked 
//	memcpy(&arg.m_dram_cfg, &g_EMI_NFI_Setting.m_dram_cfg, sizeof(DRAM_SETTING));

	strp_FT_Arg->m_cb_set_high_speed_baudrate = NULL;
	strp_FT_Arg->m_cb_set_high_speed_baudrate_arg = NULL;

	//------------------------------ FLASHTOOL_RESULT ------------------------------
	memset(strp_FT_Result, 0, sizeof(*strp_FT_Result));
}

void CFlash_OP::Arg_Flash_Tool_Download(FLASHTOOL_DOWNLOAD_ARG * strp_FT_DL_Arg, FLASHTOOL_DOWNLOAD_RESULT * strp_FT_DL_Result)
{
	//------------------------------ FLASHTOOL_DOWNLOAD_ARG ------------------------------
	memset(strp_FT_DL_Arg, 0, sizeof(*strp_FT_DL_Arg));

	//DL���
	strp_FT_DL_Arg->m_dl_handle_list	= NULL;
	strp_FT_DL_Arg->m_dl_handle			= m_DL_HANDLE;



	// download progress callback
	strp_FT_DL_Arg->m_download_accuracy						= ACCURACY_AUTO;
	strp_FT_DL_Arg->m_cb_download_flash_init				= ::cb_download_flash_init;
	strp_FT_DL_Arg->m_cb_download_flash_init_arg			= this;
	strp_FT_DL_Arg->m_cb_download_flash						= ::cb_download_flash;
	strp_FT_DL_Arg->m_cb_download_flash_arg					= this;
	strp_FT_DL_Arg->m_cb_download_fs_init					= NULL;
    strp_FT_DL_Arg->m_cb_download_fs_init_arg				= NULL;
    strp_FT_DL_Arg->m_cb_download_fs						= NULL;
    strp_FT_DL_Arg->m_cb_download_fs_arg					= NULL;
    strp_FT_DL_Arg->m_cb_download_partition_init			= NULL;
    strp_FT_DL_Arg->m_cb_download_partition_init_arg		= NULL;
    strp_FT_DL_Arg->m_cb_download_partition					= NULL;
    strp_FT_DL_Arg->m_cb_download_partition_arg				= NULL;
    strp_FT_DL_Arg->m_cb_nvram_backup_init					= NULL;
    strp_FT_DL_Arg->m_cb_nvram_backup_init_arg				= NULL;
    strp_FT_DL_Arg->m_cb_nvram_backup						= NULL;
    strp_FT_DL_Arg->m_cb_nvram_backup_arg					= NULL;
	strp_FT_DL_Arg->m_cb_security_post_process_notify		= NULL;
	strp_FT_DL_Arg->m_cb_security_post_process_notify_arg	= NULL;
	strp_FT_DL_Arg->m_cb_download_fat_change_notify			= NULL;
	strp_FT_DL_Arg->m_cb_download_fat_change_notify_arg		= NULL;
	strp_FT_DL_Arg->m_cb_pre_format_flash_init				= ::cb_pre_format_flash_init;
    strp_FT_DL_Arg->m_cb_pre_format_flash_init_arg			= this;
    strp_FT_DL_Arg->m_cb_pre_format_flash					= ::cb_pre_format_flash;
    strp_FT_DL_Arg->m_cb_pre_format_flash_arg				= this;


	// Connectivity Module
	strp_FT_DL_Arg->m_cb_download_conn_init					= ::cb_download_conn_init;
	strp_FT_DL_Arg->m_cb_download_conn_init_arg				= this;
	strp_FT_DL_Arg->m_cb_download_conn						= ::cb_download_conn;
	strp_FT_DL_Arg->m_cb_download_conn_arg					= this;
	strp_FT_DL_Arg->m_cb_download_conn_da_init				= ::cb_download_conn_da_init;
	strp_FT_DL_Arg->m_cb_download_conn_da_init_arg			= this;
	strp_FT_DL_Arg->m_cb_download_conn_da					= ::cb_download_conn_da;
	strp_FT_DL_Arg->m_cb_download_conn_da_arg				= this;



	// format option
	// [EMMC_FT]
	strp_FT_DL_Arg->m_format_cfg.m_EMMC_format_begin_addr_BP1	= NULL;
	strp_FT_DL_Arg->m_format_cfg.m_EMMC_format_length_BP1		= NULL;
	strp_FT_DL_Arg->m_format_cfg.m_EMMC_format_begin_addr_GP1	= NULL;
	strp_FT_DL_Arg->m_format_cfg.m_EMMC_format_length_GP1		= NULL;
	strp_FT_DL_Arg->m_format_cfg.m_EMMC_format_begin_addr_UA	= NULL;
	strp_FT_DL_Arg->m_format_cfg.m_EMMC_format_length_UA		= NULL;
	//�ֶ���ʽ��
	strp_FT_DL_Arg->m_format_cfg.m_format_begin_addr	= 0;
	strp_FT_DL_Arg->m_format_cfg.m_format_length		= 0;

	strp_FT_DL_Arg->m_format_cfg.m_validation			= _FALSE;

	// format option
	if(this->m_iOP_TYPE == MTK_OPTION_DL_FORMAT)
	{
		strp_FT_DL_Arg->m_format_enable                  = _TRUE;
		strp_FT_DL_Arg->m_format_cfg.m_auto_format_fat   = _TRUE;

	}
	else
	{
		strp_FT_DL_Arg->m_format_enable                  = _FALSE;
		strp_FT_DL_Arg->m_format_cfg.m_auto_format_fat   = _FALSE;
	}

	strp_FT_DL_Arg->m_cb_format_report_init			= ::cb_format_report_init;
	strp_FT_DL_Arg->m_cb_format_report_init_arg		= this;
	strp_FT_DL_Arg->m_cb_format_report				= ::cb_format_report;
	strp_FT_DL_Arg->m_cb_format_report_arg			= this;
	strp_FT_DL_Arg->m_enable_tgt_res_layout_check	= _TRUE;
	strp_FT_DL_Arg->m_enable_bbchip_ver_check		= _FALSE;//checkBasebandVersion ? _TRUE : _FALSE;
	strp_FT_DL_Arg->m_downloadstyle_sequential		= _FALSE;
	strp_FT_DL_Arg->m_cb_format_statistics			= NULL;
	strp_FT_DL_Arg->m_cb_format_statistics_arg		= NULL;

	strp_FT_DL_Arg->m_enable_fat_range_check_before_dl = _FALSE;

	strp_FT_DL_Arg->m_keep_cbr_region_intact = _FALSE;

	// parameter
	strp_FT_DL_Arg->m_otp_enable                = _FALSE;
	strp_FT_DL_Arg->m_ft_cfg_enable             = _FALSE;
	strp_FT_DL_Arg->m_sec_ro_enable             = _FALSE;
	strp_FT_DL_Arg->m_cust_para_enable          = _FALSE;

	// update param
	strp_FT_DL_Arg->m_cb_param_process_init		= ::cb_update_param_init;
    strp_FT_DL_Arg->m_cb_param_process_init_arg	= this;
    strp_FT_DL_Arg->m_cb_param_process			= ::cb_update_param;
    strp_FT_DL_Arg->m_cb_param_process_arg		= this;

	// post-process callback
    strp_FT_DL_Arg->m_cb_post_process_init		= ::cb_post_process_init;
    strp_FT_DL_Arg->m_cb_post_process_init_arg	= this;
    strp_FT_DL_Arg->m_cb_post_process			= ::cb_post_process;
    strp_FT_DL_Arg->m_cb_post_process_arg		= this;


	// Boot-Loader download progress callback
	strp_FT_DL_Arg->m_cb_download_bloader_init     = ::cb_download_bloader_init;
	strp_FT_DL_Arg->m_cb_download_bloader_init_arg = this;
	strp_FT_DL_Arg->m_cb_download_bloader          = ::cb_download_bloader;
	strp_FT_DL_Arg->m_cb_download_bloader_arg      = this;

	//���ؽ������Ƿ���Ҫ����ģ��
	if(this->m_iOP_TYPE == MTK_OPTION_DL_CLEAR_NV)
	{
		strp_FT_DL_Arg->m_enable_wdt_timeout		= _TRUE;
		strp_FT_DL_Arg->m_ms_wdt_timeout_interval	= 5000;//ms
		// set download mode for datacard project��MTK���ݿ���Ʒר�ã�ģ��������ó�_FALSE��
		strp_FT_DL_Arg->m_enable_reset_to_download_mode = _FALSE;
		strp_FT_DL_Arg->m_download_mode_op = RESET_DOWNLOAD_OPERATION_BY_BOOTLOADER; //for usbdl 2.0 feature
	}
	else
	{
		strp_FT_DL_Arg->m_enable_wdt_timeout      = _FALSE;
		strp_FT_DL_Arg->m_ms_wdt_timeout_interval = 0;
		// set normal mode for datacard project��MTK���ݿ���Ʒר�ã�ģ��������ó�_FALSE��
		strp_FT_DL_Arg->m_enable_reset_to_download_mode = _FALSE;
		strp_FT_DL_Arg->m_download_mode_op = RESET_DOWNLOAD_OPERATION_NO_CHANGE; //for usbdl 2.0 feature


	}



	// for GPS project
	strp_FT_DL_Arg->m_relay_da_enable = _FALSE;

	/*
	strp_FT_DL_Arg->m_relay_da_enable           = _TRUE;//g_DL_HANDLE_NODE.m_param.m_gps_enable;
	strp_FT_DL_Arg->m_gpio_gps.m_power_pin      = 1;//g_DL_HANDLE_NODE.m_param.m_gps_power_pin;
	strp_FT_DL_Arg->m_gpio_gps.m_reset_pin      = 25;//g_DL_HANDLE_NODE.m_param.m_gps_reset_pin;
	strp_FT_DL_Arg->m_gpio_gps.m_standby_pin    = 42;//g_DL_HANDLE_NODE.m_param.m_gps_standby_pin;
	strp_FT_DL_Arg->m_gpio_gps.m_32k_pin        = 36;//g_DL_HANDLE_NODE.m_param.m_gps_32k_pin;
	strp_FT_DL_Arg->m_enable_reset_to_normal_mode = _FALSE;
	*/


	//------------------------------ FLASHTOOL_DOWNLOAD_RESULT ------------------------------
	memset(strp_FT_DL_Result, 0, sizeof(*strp_FT_DL_Result));
}

void CFlash_OP::Show_Progress(int i_Pgr_Percent)
{
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_MSG, (WPARAM)i_Pgr_Percent, (LPARAM)this->m_iClass_Idx);
}

void CFlash_OP::Enable_Debug_Log(bool b_Enable)
{
	//����LOG�ļ�
	if(b_Enable)
	{
		Brom_DebugOn();
		META_DebugOn();
	}
	else
	{
		Brom_DebugOff();
		META_DebugOff();
	}
}

int CFlash_OP::Read_Nv_FAT_File_List(const char * cp_FAT_Base_Dir, char cr2_FAT_File_List[][MAX_PATH], int * ip_FAT_Quan)
{
	META_RESULT eu_meta_ret;
	int fs_handle, i_fat_file_size, i_fat_file_count;
	short s_Token;
	char * cp_fat_file_path = NULL;//FAT�ļ�·��BUFF

	try
	{
		//�����ڴ�
		cp_fat_file_path = (char *)calloc(MAX_PATH, sizeof(char));

		//MT6252�Ƕ�D(Z)�̣�D(Z)����ϵͳ�̣�C���Ǹ��ͻ��õġ�MT6223��ϵͳ����Z(C)�̡�Z:\\NVRAM\\NVD_DATA
		eu_meta_ret = META_FAT_Find_Start_r(this->m_iMeta_Handle, cp_FAT_Base_Dir, "*.*", FAT_FIND_FILE_RECURSIVE, &fs_handle, &s_Token);
		//����ʧ��
		if((eu_meta_ret != META_SUCCESS) && (eu_meta_ret != META_MAUI_DB_INCONSISTENT))
		{
			this->Process_Meta_Result(eu_meta_ret, _T("META_FAT_Find_Start_r"));
			throw(0);
		}

		//��ͣ
		if(this->m_iStop_Flag == BOOT_STOP)
		{
			throw(0);
		}

		//��ȡϵͳ������FAT�ļ�����
		i_fat_file_count = 0;

		do
		{
			memset(cp_fat_file_path, 0, MAX_PATH);

			eu_meta_ret = META_FAT_Find_GetFileInfo(fs_handle, cp_fat_file_path, MAX_PATH, &i_fat_file_size);

			if((eu_meta_ret != META_SUCCESS) && (eu_meta_ret != META_MAUI_DB_INCONSISTENT))
			{
				this->Process_Meta_Result(eu_meta_ret, _T("META_FAT_Find_GetFileInfo"));
				throw(0);
			}
			else
			{
				//�����ļ�·��������BUFF
				sprintf(cr2_FAT_File_List[i_fat_file_count], cp_fat_file_path);
				i_fat_file_count++;
			}

			//����һ��
			eu_meta_ret = META_FAT_Find_Next(fs_handle);	

			//��ͣ
			if(this->m_iStop_Flag == BOOT_STOP)
			{
				throw(0);
			}
		}
		while((eu_meta_ret == META_SUCCESS) || (eu_meta_ret == META_MAUI_DB_INCONSISTENT));

		//�ر�
		eu_meta_ret = META_FAT_Find_Close(&fs_handle);
		if((eu_meta_ret != META_SUCCESS) && (eu_meta_ret != META_MAUI_DB_INCONSISTENT))
		{
			this->Process_Meta_Result(eu_meta_ret, _T("META_FAT_Find_Close"));
			throw(0);
		}

		//
		*ip_FAT_Quan = i_fat_file_count;
	}
	catch(...)
	{
		//�ͷ��ڴ�
		if(cp_fat_file_path != NULL)
		{
			free(cp_fat_file_path);
			cp_fat_file_path = NULL;
		}

		return RTN_FAIL;
	}

	//�ͷ��ڴ�
	if(cp_fat_file_path != NULL)
	{
		free(cp_fat_file_path);
		cp_fat_file_path = NULL;
	}

	return RTN_SUCCESS;
}

bool CFlash_OP::LOAD_BIN_FILE(void)
{
	//����һ����������BIN�ļ�����������.bin�Ƿ�Ҫ����

	int iRtn, i;

	try
	{
		//------------------------------ �ж�FW�����Ƿ�֧��USB���� ------------------------------ 
		if((this->m_iComm_Method == IN_USB) || (this->m_iComm_Method == IN_UART_USB))
		{
// 			if(this->m_str_DL_Info_Ex.brom_usb_supported)
// 			{
// 				this->m_csError_Code.Format(_T("FAIL, Brom_usb_supported = %d"), this->m_str_DL_Info_Ex.brom_usb_supported);
// 				throw(0);
// 			}
		}
		//------------------------------ 


		//------------------------------ ����һ����������BIN�ļ���Ϣ(if(!dl_info.brom_usb_supported)) ------------------------------ 
		iRtn = DL_GetCount_Ex(m_DL_HANDLE, &this->m_usROMcount);
		if(iRtn == S_DONE)
		{
			if(this->m_iFw_Type == FW_STD)//��׼�����
			{
//20180822 
#if 0
				//����USBģʽ���أ�ĳЩBIN�ļ��ǲ���Ҫ���ص�
				//if(this->m_iComm_Method == IN_USB)
				{
					if((this->m_usROMcount < MAX_LOAD_SECTIONS) && (this->m_usROMcount > 0))
					{
						//�������.bin�ļ�������
						for(i=0; i<this->m_usROMcount; i++)
						{
							
// 							if(ARM_BL_ROM == this->m_strr_Rom_Info_Ex[i].rom_type)
// 							{
// 								DL_Rom_SetEnableAttr_Ex(m_DL_HANDLE, i, _FALSE);
// 							}
// 							else if((EXT_BL_ROM == this->m_strr_Rom_Info_Ex[i].rom_type) && !this->m_str_DL_Info_Ex.bootloader_self_update_supported)
// 							{
// 								DL_Rom_SetEnableAttr_Ex(m_DL_HANDLE, i, _FALSE);
// 							}
							//20180922 �ε�MC20-E���������� ȥ����Щ����ص�����
							if(strstr(this->m_strr_Rom_Info_Ex[i].name,"PRIMARY_MAUI")!=NULL)
							{
								DL_Rom_SetEnableAttr_Ex(m_DL_HANDLE, i, _FALSE);
							}
							else if(strstr(this->m_strr_Rom_Info_Ex[i].name,"VIVA")!=NULL)
							{
								DL_Rom_SetEnableAttr_Ex(m_DL_HANDLE, i, _FALSE);
							}
							else if(strstr(this->m_strr_Rom_Info_Ex[i].name,"GPS")!=NULL)
							{
								DL_Rom_SetEnableAttr_Ex(m_DL_HANDLE, i, _FALSE);
							}

						}//for(i=0; i<this->m_usROMcount;
					}//if((this->m_usROMcount
				}//if(this->m_iComm_Method == IN_USB
#endif
			}
			else if(this->m_iFw_Type == FW_APP)//��������app���
			{
				//app.bin���ǡ�V_CUST_ROM�����ԣ���app�⣬����bin�趨Ϊfalse

				//�������.bin�ļ�������
				for(i=0; i<this->m_usROMcount; i++)
				{
					if(V_CUST_ROM != this->m_strr_Rom_Info_Ex[i].rom_type)
					{
						DL_Rom_SetEnableAttr_Ex(m_DL_HANDLE, i, _FALSE);//��app��bin���ó�false
					}
				}//for(i=0; i<this->m_usROMcount;

				/*
				char crName[128];
				int iTemp;
				CStdioFile cla_sf_App_File;
				CString csApp_Name;


				//------------------------------ ��app�ļ�������app�ļ�����Ϣ ------------------------------ 
				if(cla_sf_App_File.Open(this->m_csScat_Cfg_File_Path, (CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead)) == NULL)
				{
					this->m_csError_Code.Format(_T("FAIL, LOAD_BIN_FILE, Open app file fail!"));
					throw(0);
				}
				//
				cla_sf_App_File.SeekToBegin();
				//------------------------------ 


				//ѭ����ȡapp�ļ���д��scat.cfg�ļ�
				while(cla_sf_App_File.ReadString(csApp_Name))
				{
					if(csApp_Name.GetLength() > 0)
					{
						//
						memset(crName, 0, sizeof(crName));
						this->m_cla_GFC.WideChar_To_MultiByte(csApp_Name, crName, &iTemp);

						//
						if((this->m_usROMcount < MAX_LOAD_SECTIONS) && (this->m_usROMcount > 0))
						{
							//�������.bin�ļ�������
							for(i=0; i<this->m_usROMcount; i++)
							{

								wchar_t wcrA[128], wcrB[128];
								CString aa;
								this->m_cla_GFC.MultiByte_To_WideChar(this->m_strr_Rom_Info_Ex[i].name, wcrA, &iTemp);
								this->m_cla_GFC.MultiByte_To_WideChar(crName, wcrB, &iTemp);
								aa.Format(_T("[%s]  [%d]"), wcrA, this->m_strr_Rom_Info_Ex[i].rom_type);
								::AfxMessageBox(aa);

								if(strcmp(this->m_strr_Rom_Info_Ex[i].name, crName) != 0)
								{
									::AfxMessageBox(_T("3"));

									DL_Rom_SetEnableAttr_Ex(::g_DL_HANDLE, i, _FALSE);//��app��bin���ó�false
								}
							}//for(i=0; i<this->m_usROMcount;
						}//if((this->m_usROMcount
					}
				}//while(cla_sf_App_File.ReadString(csApp_Name

				//app�ļ��ر�
				cla_sf_App_File.Close();
				*/
			}
			else
			{
				this->m_csError_Code.Format(_T("FAIL, LOAD_BIN_FILE, Fw type %d is wrong!"), this->m_iFw_Type);
				throw(0);
			}



			//------------------------------ ������BIN�ļ���·����ȫ����Ϣ������rom info��------------------------------ 
			iRtn = DL_Rom_GetInfoAll_Ex(m_DL_HANDLE, this->m_strr_Rom_Info_Ex, MAX_LOAD_SECTIONS);

			if(iRtn != S_DONE)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Rom_GetInfoAll_Ex-Load_Bin, Rtn = %d"), iRtn);
				throw(0);
			}
			//------------------------------ 
		}
		else
		{
			this->m_csError_Code.Format(_T("FAIL, DL_GetCount_Ex-Load_Bin, Rtn = %d"), iRtn);
			throw(0);
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CFlash_OP::Search_Dynamic_Com_Port(int * ip_New_Com_Port)
{
	::g_Get_Dynamic_COM.Lock();

	time_t t_Start, t_End;
	int irList_1[64], irList_2[64], iQuan_1, iQuan_2, i, j;
	int iTimeOut = 25;

	try
	{
		time(&t_Start);

		if(this->m_cla_GFC.Get_Reg_Com_Port_List(irList_1, &iQuan_1) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Get_Com_Port_List_1"));
			throw(0);
		}

		if(iQuan_1 >= 64)
		{
			this->m_csError_Code.Format(_T("FAIL, Get_Com_Port_List_1, com buff 1 overload!"));
			throw(0);
		}

		do
		{
			Sleep(400);

			if(this->m_cla_GFC.Get_Reg_Com_Port_List(irList_2, &iQuan_2) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Get_Com_Port_List_2"));
				throw(0);
			}

			if(iQuan_2 >= 64)
			{
				this->m_csError_Code.Format(_T("FAIL, Get_Com_Port_List_2, com buff 2 overload!"));
				throw(0);
			}

			if(iQuan_2 > iQuan_1)
			{
				break;//����COM�ڳ���
			}
			else//ˢ�¡�List_1���б������µ�irList_2��ֵ��irList_1
			{
				iQuan_1 = iQuan_2;

				for(i=0; i<iQuan_2; i++)
				{
					irList_1[i] = irList_2[i];
				}

				irList_1[i] = 0;
			}

			time(&t_End);
			if((int)difftime(t_End, t_Start) >= iTimeOut)
			{
				this->m_csError_Code.Format(_T("FAIL, Search_Dynamic_Com_Port_Timeout"));
				throw(0);
			}

			//ϵͳ��ͣ
			if(this->m_iStop_Flag == BOOT_STOP)
			{
				throw(0);//��ͣ
			}
		}
		while(true);

		//
		for(i=0; i<iQuan_2; i++)
		{
			for(j=0; j<iQuan_1; j++)
			{
				if(irList_2[i] == irList_1[j])
				{
					break;
				}
			}//for(j=0; j<iQuan_1;

			//irList_2[i]���ֵirList_1����û��
			if(j >= iQuan_1)
			{
				*ip_New_Com_Port = irList_2[i];
				break;
			}
		}//for(i=0; i<iQuan_2;

		//
		if(i >= iQuan_2)
		{
			this->m_csError_Code.Format(_T("FAIL, Search_Dynamic_Com_Port_Error"));
			throw(0);
		}
	}
	catch(...)
	{
		::g_Get_Dynamic_COM.Unlock();
		return false;
	}

	::g_Get_Dynamic_COM.Unlock();
	return true;
}

int CFlash_OP::DL_IN_USB(void)
{
	COM_PORT_SETTING str_Com_Setting;
	FLASHTOOL_ARG str_FT_ARG;
	FLASHTOOL_RESULT str_FT_Result;
	FLASHTOOL_DOWNLOAD_ARG str_FT_DL_ARG;
	FLASHTOOL_DOWNLOAD_RESULT str_FT_DL_Result;
	int iRtn;

	try
	{
		//��ʼ��ʾ
		this->Show_Status(_T("Waiting......"));

		//------------------------------ ����������USB�� ------------------------------ 
		if(!this->Search_Dynamic_Com_Port(&this->m_iCom_Port))
		{
			this->Show_Status(this->m_csError_Code);
			m_iError_Code = S_COM_PORT_OPEN_FAIL;
			throw(0);
		}

		//------------------------------ Com Port Setting ------------------------------
		this->Arg_Com_Port_Setting(&str_Com_Setting);

		//------------------------------ FLASHTOOL_ARG, FLASHTOOL_RESULT ------------------------------
		this->Arg_Flash_Tool(&str_FT_ARG, &str_FT_Result, true);

		//------------------------------ FLASHTOOL_DOWNLOAD_ARG, FLASHTOOL_DOWNLOAD_RESULT ------------------------------
		this->Arg_Flash_Tool_Download(&str_FT_DL_ARG, &str_FT_DL_Result);

		//���ؿ�ʼ
		iRtn = FlashDownload(&str_Com_Setting, &str_FT_ARG, &str_FT_Result, &str_FT_DL_ARG, &str_FT_DL_Result, &this->m_iStop_Flag);

		//�ж����ؽ��
		if(!this->Process_FlashTool_Result(iRtn))
		{
			this->m_csError_Code.Format(_T("%s-(%s)"),this->m_csError_Code,m_strFLASH_ID_Current);

			this->Show_Status(this->m_csError_Code);
			throw(0);
		}
#if 1
		if(this->m_iOP_TYPE == MTK_OPTION_DL_CLEAR_NV)
		{
			//����NV�ļ�����
			Sleep(600);

			if(!this->Enter_META_Mode(str_FT_ARG.m_boot_arg.m_bbchip_type, str_FT_ARG.m_boot_arg.m_ext_clock))
			{
				this->Show_Status(_T("FW upgrade success."));//�岻�ɹ���Ҳ��Ϊ��PASS
				this->Show_Progress((int)100.0);
			}
			else
			{
				if(!this->Clear_Nv_File())
				{
					this->Show_Status(_T("FW upgrade success.")); //�岻�ɹ���Ҳ��Ϊ��PASS
					this->Show_Progress((int)100.0);
				}
				else
				{
					this->Show_Status(_T("FW upgrade success."));
					this->Show_Progress((int)100.0);
				}
			}
		
			
		}
		else
		{
			CString csData;

			//delay for download OK
			for(int a=10; a<=100;)
			{
				csData.Format(_T("Confirm FW: %d%%"), a);
				this->Show_Status(csData);
				this->Show_Progress(a);

				a += 10;

				Sleep(400);
			}
			this->Show_Status(_T("FW download success."));
		}
#endif
		this->Show_Status(_T("FW download success."));
		this->Show_Progress((int)100.0);

	}
	catch(...)
	{
		return RTN_FAIL;
	}

	return RTN_SUCCESS;
}


void CFlash_OP::Arg_META_Conn_Req_In_USB(META_ConnectByUSB_Req * strp_META_Conn_Req, META_ConnectByUSB_Report * strp_META_Conn_Report)
{
	memset(strp_META_Conn_Req, 0, sizeof(*strp_META_Conn_Req));
	memset(strp_META_Conn_Report, 0, sizeof(*strp_META_Conn_Report));

	strp_META_Conn_Req->com_port = this->m_iCom_Port;
	strp_META_Conn_Req->ms_connect_timeout = 20000;

	strp_META_Conn_Req->boot_meta_arg.m_bbchip_type = this->m_eu_BB_Chip;
	strp_META_Conn_Req->boot_meta_arg.m_ext_clock = this->m_eu_Ext_Clock;


	strp_META_Conn_Req->boot_meta_arg.m_ms_boot_timeout           = BOOT_INFINITE;
	strp_META_Conn_Req->boot_meta_arg.m_max_start_cmd_retry_count = DEFAULT_BROM_START_CMD_RETRY_COUNT;

	// This callback function will be invoke after COM port is opened
	// You can do some initialization by using this callback function.
	strp_META_Conn_Req->boot_meta_arg.m_cb_com_init_stage     = NULL;//cb_com_init_stage;
	strp_META_Conn_Req->boot_meta_arg.m_cb_com_init_stage_arg = this;

	// This callback function will be invoke after BootROM start cmd is passed.
	// You can issue other BootROM command by brom_handle and hCOM which provides callback arguments,
	// or do whatever you want otherwise.
	strp_META_Conn_Req->boot_meta_arg.m_cb_in_brom_stage      = ::cb_boot_meta_in_brom_stage;
	strp_META_Conn_Req->boot_meta_arg.m_cb_in_brom_stage_arg  = this;

	// speed-up BootROM stage baudrate
	strp_META_Conn_Req->boot_meta_arg.m_speedup_brom_baudrate = _TRUE;//g_SpeedUpBootROMBaudrate?_TRUE:_FALSE;

	// WM_BROM_READY_TO_POWER_ON_TGT
	strp_META_Conn_Req->boot_meta_arg.m_ready_power_on_wnd_handle = NULL;
	strp_META_Conn_Req->boot_meta_arg.m_ready_power_on_wparam     = NULL;
	strp_META_Conn_Req->boot_meta_arg.m_ready_power_on_lparam     = NULL;

	// serial link authentication arg
	strp_META_Conn_Req->boot_meta_arg.m_auth_handle              = NULL;
	strp_META_Conn_Req->boot_meta_arg.m_cb_sla_challenge         = SLA_Challenge;
	strp_META_Conn_Req->boot_meta_arg.m_cb_sla_challenge_arg     = this;
	strp_META_Conn_Req->boot_meta_arg.m_cb_sla_challenge_end     = SLA_Challenge_END;
	strp_META_Conn_Req->boot_meta_arg.m_cb_sla_challenge_end_arg = this;

	// security certificate
	strp_META_Conn_Req->boot_meta_arg.m_scert_handle = NULL;//g_SCERT_HANDLE_Loaded ? g_SCERT_HANDLE : NULL;

	strp_META_Conn_Req->boot_meta_arg.m_usb_enable = _TRUE;

	// root certificate
	strp_META_Conn_Req->boot_meta_arg.m_rootcert_handle            = NULL;//g_ROOTCERT_HANDLE_Loaded ? g_ROOTCERT_HANDLE : (g_ROOTCERT_HANDLE_DL_Loaded ? g_ROOTCERT_HANDLE_DL: NULL);
	strp_META_Conn_Req->boot_meta_arg.m_rootcert_force_assign      = _FALSE;//(_BOOL) g_bForceSendRootCert;

	// META Without Battery
	//20191130---------
	strp_META_Conn_Req->boot_meta_arg.m_enable_without_battery     = _TRUE;//(g_DownloadWithoutBattery != DISABLE_DL_WO_BATT)? _TRUE:_FALSE;
}

bool CFlash_OP::Get_Bin_File_Info(wchar_t * wcp_Bin_File_Name, CString * csp_Product_Name, int * ip_Flash)
{
	CString csUseful_Data;

	try
	{
		//�ȼ�顢��ȡProduct_Name�������������Ǳ����еġ�
		if(this->Get_Bin_File_Product_Name(wcp_Bin_File_Name, csp_Product_Name, &csUseful_Data) == false)
		{
			throw(0);
		}

		//�ڻ��flash_index��������NDF
		this->Get_Bin_File_Flash_Index(csUseful_Data, ip_Flash);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CFlash_OP::Get_Bin_File_Name(const wchar_t * wcp_File_Path, wchar_t * wcp_File_Name)
{
	int i, j;
	int iLong = wcslen(wcp_File_Path);

	try
	{
		for(i=(iLong - 1); i>=0; i--)
		{
			if(*(wcp_File_Path + i) == '\\')
			{
				break;
			}
		}

		i++;
		j = 0;

		while((*(wcp_File_Path + i) != '.') && (*(wcp_File_Path + i) != '\0'))
		{
			*(wcp_File_Name + j) = *(wcp_File_Path + i);
			i++;
			j++;
		}

		*(wcp_File_Name + j) = '\0';
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CFlash_OP::Load_Bin_File_Product_Flash_Name(void)
{
	wchar_t * wcpPath = NULL;
	wchar_t * wcpName = NULL;
	int i, iTemp;

	try
	{
		//�����ڴ�
		wcpPath = (wchar_t *)calloc(MAX_PATH, sizeof(wchar_t));
		wmemset(wcpPath, 0, MAX_PATH);

		wcpName = (wchar_t *)calloc(128, sizeof(wchar_t));
		wmemset(wcpName, 0, 128);

		for(i=0; i<this->m_usROMcount; i++)
		{
			//BIN�ļ�·��
			this->m_cla_GFC.MultiByte_To_WideChar(this->m_strr_Rom_Info_Ex[i].filepath, wcpPath, &iTemp);

			//��·���н������ļ���
			if(this->Get_Bin_File_Name(wcpPath, wcpName))
			{
				//���ļ����н��������ͺ�Flash
				this->m_csBin_Product.Format(_T(""));
				this->m_iBin_Flash = NDF_FLASH;

				if(this->Get_Bin_File_Info(wcpName, &this->m_csBin_Product, &this->m_iBin_Flash))
				{
					break;
				}
			}
		}

		if(i >= this->m_usROMcount)
		{
			this->m_csError_Code.Format(_T("Error, Bin file does not have module name and flash name!"));
			throw(0);
		}

		//�޻�����Ϣ�ͱ�Fail
		if(this->m_csBin_Product.GetLength() <= 0)
		{
			this->m_csError_Code.Format(_T("Error, Bin file does not have module name!"));
			throw(0);
		}
	}
	catch(...)
	{
		//�ͷ��ڴ�
		if(wcpPath != NULL)
		{
			free(wcpPath);
			wcpPath = NULL;
		}
		if(wcpName != NULL)
		{
			free(wcpName);
			wcpName = NULL;
		}

		return false;
	}

	//�ͷ��ڴ�
	if(wcpPath != NULL)
	{
		free(wcpPath);
		wcpPath = NULL;
	}
	if(wcpName != NULL)
	{
		free(wcpName);
		wcpName = NULL;
	}

	return true;
}

bool CFlash_OP::Load_NV_Retain_File_List(const int i_NV_TYPE, const wchar_t * wcp_Curr_Exe_Path)
{
	CStdioFile cla_File;
	CString csFile_Path, csData_Buff;
	int iTemp;
	int iQuan = 0;

	try
	{
		//����·��
		csFile_Path.Format(_T("%s\\TYPE_%d.txt"), wcp_Curr_Exe_Path, i_NV_TYPE);

		//���ļ�
		if(cla_File.Open(csFile_Path, CFile::modeRead))
		{
			cla_File.SeekToBegin();

			while(cla_File.ReadString(csData_Buff) != NULL)
			{
				//
				memset(this->m_cr2NV_Retain_File[iQuan], 0, sizeof(this->m_cr2NV_Retain_File[iQuan]));

				//˫char����תΪ��char����
				if(this->m_cla_GFC.WideChar_To_MultiByte(csData_Buff, this->m_cr2NV_Retain_File[iQuan], &iTemp) == false)
				{
					cla_File.Close();

					this->m_csError_Code.Format(_T("FAIL, Wchar to multi byte in nv type %d file!"), i_NV_TYPE);
					throw(0);
				}

				//����
				iQuan++;
			}

			cla_File.Close();
		}
		else
		{
			this->m_csError_Code.Format(_T("FAIL, Open Nv Type %d File!"), i_NV_TYPE);
			throw(0);
		}
	}
	catch(...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}

	this->m_iNV_Retain_File_Quan = iQuan;
	return true;
}

bool CFlash_OP::Get_Bin_File_Product_Name(const wchar_t * wcp_Bin_File_Name, CString * csp_Product_Name, CString * csp_Useful_Data)
{
	//����ļ������Ƿ��С��������������л�����������ȡ�����ء����������͡��������ݶΡ���

	int i = 0;
	wchar_t * wcpKey_1 = NULL;
	wchar_t * wcpKey_2 = NULL;
	wchar_t * wcpKey_3 = NULL;
	wchar_t wcrUseful[64];

	try
	{
		//ѭ����ȡ��_����������ݴ�
		while(this->m_cla_GFC.WCSTOK_STRING_C(wcp_Bin_File_Name, _T("_"), i, csp_Useful_Data) == true)
		{
			//��ʼ�ҹؼ��ʣ���ȡӲ�����ͣ����ҡ�RxxAxx��
			swprintf_s(wcrUseful, _T("%s"), *csp_Useful_Data);
			wcpKey_1 = wcsstr(wcrUseful, _T("R"));

			while(wcpKey_1 != NULL)//�ҡ�RxxAxx��
			{
				wcpKey_2 = wcsstr(wcpKey_1, _T("A"));

				if(wcpKey_2 != NULL)
				{
					//"R"��"A"֮�������ַ�����"R"Ϊͷ����������6���ַ�
					if(((wcpKey_2 - wcpKey_1) == 3) && (wcslen(wcpKey_1) >= 6))
					{
						//"R"��"A"�󶼸�����λ����
						if((*(wcpKey_1 + 1) >= '0') && (*(wcpKey_1 + 1) <= '9') &&
							(*(wcpKey_1 + 2) >= '0') && (*(wcpKey_1 + 2) <= '9') &&
							(*(wcpKey_2 + 1) >= '0') && (*(wcpKey_2 + 1) <= '9') &&
							(*(wcpKey_2 + 2) >= '0') && (*(wcpKey_2 + 2) <= '9'))
						{
							//"A"��ĸ�������ΪFlash��Ϣ�����������Flashʹ�ã����磺U10AR02A01V06M1024��M12ER01A01W64_OCPU
							//"R"��ĸǰ�������Ϊ������Ϣ
							*(wcpKey_1 + 0) = '\0';
							csp_Product_Name->Format(_T("%s"), wcrUseful);

							//�ɹ�����������
							return true;
						}
						else
						{
							wcpKey_3 = wcpKey_1 + 1;
							wcpKey_1 = wcsstr(wcpKey_3, _T("R"));
							continue;
						}
					}
					else//"R"��"A"֮�䲻��2���ַ�
					{
						wcpKey_3 = wcpKey_1 + 1;
						wcpKey_1 = wcsstr(wcpKey_3, _T("R"));
						continue;
					}
				}
				else//û��"A"���Ͳ���������
				{
					break;
				}
			}//û��"R"���Ͳ���������


			//��������һ�ֿ��ܵĹؼ��ʣ��ҡ�SxxVx�����硰M6310S01V1_0����_M6310S02_��
			swprintf_s(wcrUseful, _T("%s"), *csp_Useful_Data);
			wcpKey_1 = wcsstr(wcrUseful, _T("S"));

			if(wcpKey_1 != NULL)
			{
				if(wcslen(wcpKey_1) >= 3)
				{
					//"S"��2λ���ǰ���������
					if((*(wcpKey_1 + 1) >= '0') && (*(wcpKey_1 + 1) <= '9') && (*(wcpKey_1 + 2) >= '0') && (*(wcpKey_1 + 2) <= '9'))
					{
						//"S"��ĸǰ�������Ϊ������Ϣ
						*(wcpKey_1 + 0) = '\0';
						csp_Product_Name->Format(_T("%s"), wcrUseful);

						//�ɹ�����������
						return true;
					}
				}
			}

			/*
			while(wcpKey_1 != NULL)//�ҡ�SxxVx��
			{
				wcpKey_2 = wcsstr(wcpKey_1, _T("V"));

				if(wcpKey_2 != NULL)
				{
					//"S"��"V"֮�������ַ�����"S"Ϊͷ����������5���ַ�
					if(((wcpKey_2 - wcpKey_1) == 3) && (wcslen(wcpKey_1) >= 5))
					{
						//"S"��"V"���ǰ���������
						if((*(wcpKey_1 + 1) >= '0') && (*(wcpKey_1 + 1) <= '9') &&
							(*(wcpKey_1 + 2) >= '0') && (*(wcpKey_1 + 2) <= '9') &&
							(*(wcpKey_2 + 1) >= '0') && (*(wcpKey_2 + 1) <= '9'))
						{
							//"S"��ĸǰ�������Ϊ������Ϣ
							*(wcpKey_1 + 0) = '\0';
							csp_Product_Name->Format(_T("%s"), wcrUseful);

							//�ɹ�����������
							return true;
						}
						else
						{
							wcpKey_3 = wcpKey_1 + 1;
							wcpKey_1 = wcsstr(wcpKey_3, _T("S"));
							continue;
						}
					}
					else//"S"��"V"֮��������2������������
					{
						wcpKey_3 = wcpKey_1 + 1;
						wcpKey_1 = wcsstr(wcpKey_3, _T("R"));
						continue;
					}
				}
				else//û��"S"���Ͳ���������
				{
					break;
				}
			}//û��"S"���Ͳ���������
			*/

			//���ţ�����һ��
			i++;
		}//while(this->m_cla_GFC.WCSTOK_STRING_C(wcp_Bin_File_Name, _T(" "), i, &csUseful) == true
	}
	catch(...)
	{
		csp_Product_Name->Format(_T(""));
		csp_Useful_Data->Format(_T(""));
		return false;
	}

	csp_Product_Name->Format(_T(""));
	csp_Useful_Data->Format(_T(""));
	return false;
}

bool CFlash_OP::Get_Bin_File_Flash_Index(const wchar_t * wcp_Bin_File_Name, int * ip_Flash_Index)
{
	//��BIN�ļ�������ȡflash�ͺ���Ϣ

	wchar_t * wcprFlash_Key[] = {
		_T("_SST"), _T("S32"), _T("S64"), _T("S128"),//0 ~ 3
		_T("_TSB"), _T("_TOSB"), _T("T32"), _T("T64"), _T("T128"),//4 ~ 8
		_T("_NMX"), _T("N32"), _T("N64"), _T("N128"), _T("NM128"),//9 ~ 13
		_T("SPANSION"), _T("SP32"), _T("SP64"), _T("SP128"),//14 ~ 17
		_T("W32"), _T("W64"),//18 ~ 19
		_T("MX128"),//20
		_T("M1024"),//21
		_T("KH"),//22
		_T("W128")//23
	};

	int iFlash_Key_Quan = 24;

	int i;
	int iFlash_Index = -99;//Ĭ��ֵ
	CString csBuff;

	try
	{
		//����buff
		csBuff.Format(_T("%s"), wcp_Bin_File_Name);

		//�˶�Flash
		if(csBuff.GetLength() > 0)//��Flash����
		{
			for(i=0; i<iFlash_Key_Quan; i++)
			{
				if(csBuff.Find(wcprFlash_Key[i]) >= 0)
				{
					//����ƥ���flash�ؼ���
					iFlash_Index = i;
					break;
				}
			}
		}

		//Flash���͸�ֵ
		switch(iFlash_Index)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			*ip_Flash_Index = SST_FLASH;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			*ip_Flash_Index = TOSHIBA_FLASH;
			break;
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
			*ip_Flash_Index = NUMONYX_FLASH;
			break;
		case 14:
		case 15:
		case 16:
		case 17:
			*ip_Flash_Index = SPANSION_FLASH;
			break;
		case 18:
		case 19:
		case 23:
			*ip_Flash_Index = WINBOND_FLASH;
			break;
		case 20:
			*ip_Flash_Index = MX_FLASH;
			break;
		case 21:
			*ip_Flash_Index = MICRON_FLASH;
			break;
		case 22:
			*ip_Flash_Index = MX_FLASH;
			break;
		default:
			*ip_Flash_Index = NDF_FLASH;
			break;
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CFlash_OP::JUDGE_SCAT_FILE(const wchar_t * wcp_Scat_File_Path, int * ip_Fw_Type)
{
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;
	BYTE ucrData[8];
	unsigned long ulActual_Read;			//ÿ��ʵ�ʶ���ֵ

	try
	{
		//��mbn�ļ�
		hFile_Handle = ::CreateFile(wcp_Scat_File_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			this->m_csError_Code.Format(_T("FAIL, JUDGE_SCAT_FILE, Open %s file failed!"), wcp_Scat_File_Path);
			throw(0);
		}

		//
		memset(ucrData, 0, sizeof(ucrData));

		//���ļ�
		if(::ReadFile(hFile_Handle, ucrData, 1, &ulActual_Read, NULL) != TRUE)
		{
			this->m_csError_Code.Format(_T("FAIL, JUDGE_SCAT_FILE, Read scat file failed!"));
			throw(0);
		}

		//����scat�ļ��ĵ�һ���ַ����ж�MTK���������
		if(((ucrData[0] >= '0') && (ucrData[0] <= '9')) ||
			((ucrData[0] >= 'A') && (ucrData[0] <= 'Z')) ||
			((ucrData[0] >= 'a') && (ucrData[0] <= 'z')))
		{
			*ip_Fw_Type = FW_APP;//ocpu app�ļ�ֻ��.bin�ļ���
		}
		else
		{
			*ip_Fw_Type = FW_STD;
		}

		//�ر�scat�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//�ѽ������õ�scat�ļ�������mtk class
		this->m_csScat_Cfg_File_Path.Format(_T("%s"), wcp_Scat_File_Path);
	}
	catch(...)
	{
		//�ر�scat�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		return false;
	}

	return true;
}

bool CFlash_OP::SETUP_APP_FULL_FW_FILE(const wchar_t * wcp_App_File_Path, const wchar_t * wcp_Model_Name, CString * csp_App_Scat_Cfg_File_Path)
{
	CString csTemp_Download_Dir;	//��ʱ����齨��app�������ذ����ļ���
	CString csDL_Src_Dir;			//��Ÿ������͡���ԴFW���ļ���

	try
	{
		//�齨 app�������ذ����ļ���
		csTemp_Download_Dir.Format(_T("%s\\dl_src\\temp_download"), this->m_csCurr_Exe_Path);
		//�齨 ��Ÿ������͡���ԴFW���ļ���
		csDL_Src_Dir.Format(_T("%s\\dl_src\\%s"), this->m_csCurr_Exe_Path, wcp_Model_Name);

		//�����app�������ذ����ļ���
		this->m_cla_GFC.Empty_Directory(csTemp_Download_Dir);


		//�ٰѻ��Ͷ�Ӧ��FWԴ�ļ�������app����ʱ�ļ�����
		if(this->COPY_BIN_FILE(csDL_Src_Dir, csTemp_Download_Dir) == false)
		{
			throw(0);
		}

		//����app����ļ���app����ʱ�ļ�����
		if(this->COPY_APP_FILE(wcp_App_File_Path, csTemp_Download_Dir) == false)
		{
			throw(0);
		}

		//����app��������õ�scat�ļ�
		if(this->SETUP_APP_SCAT_FILE(wcp_App_File_Path, csDL_Src_Dir, csTemp_Download_Dir, csp_App_Scat_Cfg_File_Path) == false)
		{
			throw(0);
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CFlash_OP::COPY_BIN_FILE(const wchar_t * wcp_Src_Dir, const wchar_t * wcp_Tgt_Dir)
{
	CFileFind cla_Finder; //�ļ�������
	CString csFinder_Dir, csFW_Name, csSrc_Path, csTgt_Path;
	BOOL b_Found;

	try
	{
		//Ѱ���ļ���ʼ
		csFinder_Dir.Format(_T("%s\\*"), wcp_Src_Dir);

		b_Found = cla_Finder.FindFile(csFinder_Dir);

		while(b_Found) 
		{
			b_Found = cla_Finder.FindNextFile();

			if(cla_Finder.IsDots())
			{
				continue;//�ҵ����ǵ�ǰĿ¼���ϼ�Ŀ¼������
			}

			if(!cla_Finder.IsDirectory())//��������ļ��У������ļ�
			{
				csFW_Name.Format(_T("%s"), cla_Finder.GetFileName());//��ȡ�ҵ����ļ���

				csFW_Name.MakeUpper();

				//�ж��ļ�����
				if((wcsstr(csFW_Name, _T(".TXT")) != NULL) || (wcsstr(csFW_Name, _T(".CFG")) != NULL))
				{
					//.txt��.cfg�ļ����ÿ�����
					continue;
				}
				else
				{
					//�����ļ��򿽱�
					csSrc_Path.Format(_T("%s\\%s"), wcp_Src_Dir, cla_Finder.GetFileName());
					csTgt_Path.Format(_T("%s\\%s"), wcp_Tgt_Dir, cla_Finder.GetFileName());

					//���ƣ������Ѿ����ڵ��ļ���
					if(::CopyFile(csSrc_Path, csTgt_Path, FALSE) == NULL)
                    {
						this->m_csError_Code.Format(_T("ERROR, Copy file %s fail!"), cla_Finder.GetFileName());
						throw(0);
                    }
				}
			}//���ļ�
		}//while(b_Found

		//
		cla_Finder.Close();
	}
	catch(...)
	{
		cla_Finder.Close();

		return false;
	}

	return true;
}

bool CFlash_OP::SETUP_APP_SCAT_FILE(const wchar_t * wcp_App_File_Path, const wchar_t * wcp_Src_Dir, const wchar_t * wcp_Tgt_Dir, CString * csp_App_Scat_Cfg_File_Path)
{
	CStdioFile cla_sf_Src_File, cla_sf_Tgt_File;
	CString csSrc_File, csTgt_File, csFile_Extension, csValue, csData;

	try
	{
		//------------------------------ ��1���ļ� ------------------------------ 
		csSrc_File.Format(_T("%s\\1.cfg"), wcp_Src_Dir);
		//
		if(cla_sf_Src_File.Open(csSrc_File, (CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead)) == NULL)
		{
			//1.txt
			csSrc_File.Format(_T("%s\\1.txt"), wcp_Src_Dir);
			//��1���ļ�
			if(cla_sf_Src_File.Open(csSrc_File, (CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead)) == NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, SETUP_APP_SCAT_FILE, Open 1 file fail!"));
				throw(0);
			}
			else
			{
				csFile_Extension.Format(_T("txt"));
			}
		}
		else
		{
			csFile_Extension.Format(_T("cfg"));
		}
		//
		cla_sf_Src_File.SeekToBegin();
		//------------------------------ 



		//------------------------------ Ŀ��scat�ļ������棩------------------------------ 
		csTgt_File.Format(_T("%s\\scat.%s"), wcp_Tgt_Dir, csFile_Extension);
		//��scat�ļ�
		if(cla_sf_Tgt_File.Open(csTgt_File, (CFile::modeCreate |CFile::modeNoTruncate | CFile::modeWrite)) == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, SETUP_APP_SCAT_FILE, Open new scat file fail!"));
			throw(0);
		}
		//
		cla_sf_Tgt_File.SeekToEnd();
		//------------------------------ 


		//ѭ����ȡ1.cfg�ļ���д��scat.cfg�ļ�
		while(cla_sf_Src_File.ReadString(csValue))
		{
			//���벢�����ַ�
			cla_sf_Tgt_File.WriteString(csValue);
			cla_sf_Tgt_File.WriteString(_T("\r\n"));
		}

		//1.cfg�ļ��ر�
		cla_sf_Src_File.Close();


		//------------------------------ ��app�ļ�������app�ļ�����Ϣ ------------------------------ 
		if(cla_sf_Src_File.Open(wcp_App_File_Path, (CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead)) == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, SETUP_APP_SCAT_FILE, Open app file fail!"));
			throw(0);
		}
		//
		cla_sf_Src_File.SeekToBegin();
		//------------------------------ 


		//ѭ����ȡapp�ļ���д��scat.cfg�ļ�
		while(cla_sf_Src_File.ReadString(csValue))
		{
			if(csValue.GetLength() > 0)
			{
				csData.Format(_T("    - file: %s\r\n"), csValue);
				cla_sf_Tgt_File.WriteString(csData);
			}
		}
		//app�ļ��ر�
		cla_sf_Src_File.Close();


		//------------------------------ ��2.cfg�ļ� ------------------------------ 
		csSrc_File.Format(_T("%s\\2.%s"), wcp_Src_Dir, csFile_Extension);
		//
		if(cla_sf_Src_File.Open(csSrc_File, (CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead)) == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, SETUP_APP_SCAT_FILE, Open 2 file fail!"));
			throw(0);
		}
		//
		cla_sf_Src_File.SeekToBegin();
		//------------------------------ 


		//ѭ����ȡ2.cfg�ļ���д��scat.cfg�ļ�
		while(cla_sf_Src_File.ReadString(csValue))
		{
			//���벢�����ַ�
			cla_sf_Tgt_File.WriteString(csValue);
			cla_sf_Tgt_File.WriteString(_T("\r\n"));
		}


		//�ر������ļ�
		cla_sf_Src_File.Close();
		cla_sf_Tgt_File.Close();


		//������scat�ļ���·��
		csp_App_Scat_Cfg_File_Path->Format(_T("%s"), csTgt_File);
	}
	catch(...)
	{
		//
		cla_sf_Src_File.Close();
		cla_sf_Tgt_File.Close();

		return false;
	}

	return true;
}

bool CFlash_OP::COPY_APP_FILE(const wchar_t * wcp_App_File_Path, const wchar_t * wcp_Tgt_Dir)
{
	CStdioFile cla_sf_App_File;
	CString csApp_Name, csApp_Dir, csSrc_File, csTgt_File;

	try
	{
		//------------------------------ ��app�ļ�������app�ļ�����Ϣ ------------------------------ 
		if(cla_sf_App_File.Open(wcp_App_File_Path, (CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead)) == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, COPY_APP_FILE, Open app file fail!"));
			throw(0);
		}
		//
		cla_sf_App_File.SeekToBegin();
		//------------------------------ 


		//��ȡapp.bin�ļ����ڵ��ļ���
		if(this->m_cla_GFC.Get_File_Directory(wcp_App_File_Path, &csApp_Dir) == false)
		{
			this->m_csError_Code.Format(_T("ERROR, COPY_APP_FILE, Get app file directory fail!"));
			throw(0);
		}


		//ѭ����ȡapp�ļ���д��scat.cfg�ļ�
		while(cla_sf_App_File.ReadString(csApp_Name))
		{
			if(csApp_Name.GetLength() > 0)
			{
				csSrc_File.Format(_T("%s\\%s"), csApp_Dir, csApp_Name);
				csTgt_File.Format(_T("%s\\%s"), wcp_Tgt_Dir, csApp_Name);

				//���ƣ������Ѿ����ڵ��ļ���
				if(::CopyFile(csSrc_File, csTgt_File, FALSE) == NULL)
				{
					this->m_csError_Code.Format(_T("ERROR, COPY_APP_FILE, Copy app file %s fail!"), csApp_Name);
					throw(0);
				}
			}
		}

		//app�ļ��ر�
		cla_sf_App_File.Close();
	}
	catch(...)
	{
		//app�ļ��ر�
		cla_sf_App_File.Close();

		return false;
	}

	return true;
}

void CFlash_OP::Load_CALLBACK_ARG(void)
{
	//ARG ����
	this->m_str_QTI_ARG.p_usr_Arguments = this;
	//
	this->m_str_QTI_ARG.cb_Show_REdit_Info = ::cb_show_state_r;//
	this->m_str_QTI_ARG.cb_Show_State = ::cb_show_state_r;
	this->m_str_QTI_ARG.cb_Show_Test_Result = NULL;
	this->m_str_QTI_ARG.cb_Save_Trace_Log = NULL;
	this->m_str_QTI_ARG.cb_Show_Title = NULL;



	//��OCP����ARG�Ĳ���
	this->m_str_READ_OCP_PAR_ARG.p_usr_Arguments = this;
	//
	this->m_str_READ_OCP_PAR_ARG.cb_Load_Produ_Params_Gen = ::cb_load_produ_params_gen_r;
	this->m_str_READ_OCP_PAR_ARG.cb_OCP_Get_Error_Info = ::cb_ocp_get_error_info_r;
	//
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_GSM_Band_List = ::cb_read_produ_gsm_band_list_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_WCDMA_Band_List = ::cb_read_produ_wcdma_band_list_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_C2K_Band_List = ::cb_read_produ_c2k_band_list_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_TD_Band_List = ::cb_read_produ_td_band_list_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_LTE_Band_List = ::cb_read_produ_lte_band_list_r;
	//
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_PF_And_BB_Chip = ::cb_read_produ_pf_and_bb_chip_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_TS_Type_Seq = ::cb_read_produ_ts_type_seq_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_Init_Time = ::cb_read_produ_init_time_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_Cal_Tree_Name = ::cb_read_produ_cal_tree_name_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_Volt_And_Curr = ::cb_read_produ_volt_and_curr_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_LTE_CAT = ::cb_read_produ_lte_cat_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_Landi_Enable = ::cb_read_produ_landi_enable_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_WiFi_Enable = ::cb_read_produ_wifi_enable_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_Nw_Scan_Mode = ::cb_read_produ_nw_scan_mode_r;
	this->m_str_READ_OCP_PAR_ARG.cb_Read_Produ_HW_SIM_Enable = NULL;//::cb_read_produ_hw_sim_enable_r;

#ifdef FW_MULTI

	//���ػص���Դ
	//flash_test
	::FLASH_TEST_LOAD_ARG_QTI( this->m_iClass_Idx, &this->m_str_QTI_ARG );

	//write_imei
	::WRITE_IMEI_LOAD_ARG_QTI( this->m_iClass_Idx, &this->m_str_QTI_ARG );
	::WRITE_IMEI_LOAD_ARG_READ_OCP_PARAMS( this->m_iClass_Idx, &this->m_str_READ_OCP_PAR_ARG );
#endif

}

//app_version: APPGS3MDM32A02_THZ
bool CFlash_OP::Check_APP_VER(void)
{
	CString csAT_Cmd,csData;
	int iTimeOut = 15;
	time_t start,finish;
	wchar_t wcrData[128];
	int iRec_Bits;
	csAT_Cmd.Format( _T( "app_version" ) );
	try
	{
		this->m_cla_Com_Port.m_iPort = this->m_iCom_Port;
		this->m_cla_Com_Port.m_iBaudRate = 115200;
		//��ͨ�ſ�
		if(this->m_cla_Com_Port.Port_Open( 400 ) == false)
		{
			//
			this->m_csError_Code.Format(_T("COM%d��ʧЧ"), this->m_iCom_Port);
			//
			csData.Format(_T("ERROR, %s, �޷�ʹ��!"), this->m_csError_Code);
			//
			this->Show_Status(csData);
			//
			return false;
		}
		time( &start );
		do
		{
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			this->m_cla_Com_Port.Query( csAT_Cmd, wcrData, &iRec_Bits, 100 );

			//
			if(wcsstr( wcrData, _T( "APPGS3MDM32A02_THZ" ) ) != NULL)
			{
				//���
				break;
			}

			//
			time( &finish );

			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				this->Show_Status(_T("Check_APP_VER Time Out"));
				throw(0);
			}
		}
		while( true );
	}

	catch( ... )
	{
		this->m_cla_Com_Port.Port_Close();
		return false;
	}

	this->m_cla_Com_Port.Port_Close();
	return true;

}

void CFlash_OP::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}



int CFlash_OP::Arg_Flash_Tool_FormatEx( FLASHTOOL_FORMAT_ARG_EX       *fmt_arg_ex, FLASHTOOL_FORMAT_RESULT    *fmt_result)
{
	memset( fmt_arg_ex,    0, sizeof(*fmt_arg_ex));
	memset( fmt_result, 0, sizeof(*fmt_result));

	
	fmt_arg_ex->m_format_cfg.storageType = HW_STORAGE_NOR;
	fmt_arg_ex->m_format_cfg.formatAction.uArg.manual.startAddr = m_uiFLASH_START_ADDR;//0;  
	fmt_arg_ex->m_format_cfg.formatAction.uArg.manual.length = m_uiFLASH_ADDR_LENGTH;//0x00300000; 


	fmt_arg_ex->m_cb_format_report_init        = cb_format_report_init;
	fmt_arg_ex->m_cb_format_report_init_arg    = this;
	fmt_arg_ex->m_cb_format_report             = cb_format_report;
	fmt_arg_ex->m_cb_format_report_arg         = this;
	fmt_arg_ex->m_cb_format_statistics         = NULL;//cb_format_statistics;
	fmt_arg_ex->m_cb_format_statistics_arg     = this;
	fmt_arg_ex->m_format_cfg.validation        = _FALSE;//_TRUE:_FALSE;
	fmt_arg_ex->m_nor_flash_erase_mode         =  NOR_FLASH_ERASE_SAFE_MODE; //NOR_FLASH_ERASE_SAFE_MODE : NOR_FLASH_ERASE_FAST_MODE;

	fmt_arg_ex->m_cb_format_conn_report_init        = cb_format_conn_report_init;
	fmt_arg_ex->m_cb_format_conn_report_init_arg    = this;
	fmt_arg_ex->m_cb_format_conn_report             = cb_format_report;
	fmt_arg_ex->m_cb_format_conn_report_arg         = this;

	fmt_arg_ex->m_cb_download_conn_da_init        = cb_download_conn_da_init;
	fmt_arg_ex->m_cb_download_conn_da_init_arg    = this;
	fmt_arg_ex->m_cb_download_conn_da             = cb_download_conn_da;
	fmt_arg_ex->m_cb_download_conn_da_arg         = this;

	fmt_arg_ex->m_format_cfg.formatAction.type = TOTAL_FORMAT;


	// disable watchdog
	fmt_arg_ex->m_enable_wdt_timeout      = _FALSE;
	fmt_arg_ex->m_ms_wdt_timeout_interval = 0;
	// set download mode for datacard project
	fmt_arg_ex->m_enable_reset_to_download_mode = _FALSE;
	fmt_arg_ex->m_download_mode_op = RESET_DOWNLOAD_OPERATION_NO_CHANGE; //for usbdl 2.0 feature

	return 0;
}

//20190904 ȫ��ʽ��
int CFlash_OP::Format_Internal(void)
{
	COM_PORT_SETTING str_Com_Setting;
	FLASHTOOL_ARG str_FT_ARG;
	FLASHTOOL_RESULT str_FT_Result;


	FLASHTOOL_FORMAT_ARG_EX		fmt_arg_ex;
	FLASHTOOL_FORMAT_RESULT		fmt_result;

	int iRtn;
	CString csData;

	try
	{
		//------------------------------------- ���ԡ�ͨ�ſڡ��Ƿ����� -------------------------------------
		//��ʾ��ǰ������Ϣ
		csData.Format(_T("Check COM%d port is enabled......"), this->m_iCom_Port);
		this->Show_Status(csData);
		//
		this->m_cla_Com_Port.m_iPort = this->m_iCom_Port;
		this->m_cla_Com_Port.m_iBaudRate = 115200;
		//��ͨ�ſ�
		if(this->m_cla_Com_Port.Port_Open( 400 ) == false)
		{
			//
			this->m_csError_Code.Format(_T("COM%d port disabled"), this->m_iCom_Port);
			//
			csData.Format(_T("ERROR, %s, port disabled!"), this->m_csError_Code);
			//
			this->Show_Status(csData);

			m_iError_Code = S_COM_PORT_OPEN_FAIL;
			//
			throw(0);
		}

		//�ӳ�
		Sleep(300);

		//���ԡ��ر�ͨ�ſڡ�
		this->m_cla_Com_Port.Port_Close();
		//-------------------------------------

		//��ʼ��ʾ
		this->Show_Status(_T("Waiting......"));
		this->Show_Out(_T("Waiting..."));


		//------------------------------ Com Port Setting ------------------------------
		this->Arg_Com_Port_Setting(&str_Com_Setting);

		//------------------------------ FLASHTOOL_ARG, FLASHTOOL_RESULT ------------------------------
		this->Arg_Flash_Tool(&str_FT_ARG, &str_FT_Result, false);

		// fill FLASHTOOL_FORMAT_ARG
		this->Arg_Flash_Tool_FormatEx( &fmt_arg_ex, &fmt_result );

		//��ʼ��ʽ
		iRtn =  FlashFormat_Ex(&str_Com_Setting, &str_FT_ARG, &str_FT_Result, &fmt_arg_ex, &fmt_result, &this->m_iStop_Flag);

		if (iRtn != 0)
		{
			throw(1);
		}

	}
	catch(...)
	{
		return RTN_FAIL;
	}

	return RTN_SUCCESS;
}
