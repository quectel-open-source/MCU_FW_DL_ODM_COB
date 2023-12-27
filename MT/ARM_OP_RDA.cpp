#include "StdAfx.h"
#include "ARM_OP_RDA.h"


const unsigned char fcs_msb2lsb[256] =
{
 0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
 0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
 0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
 0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
 0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
 0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
 0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
 0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
 0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
 0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
 0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
 0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
 0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
 0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
 0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
 0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

const unsigned int fcs_table[256] =
{
   0x00000000,0x01BBA1B5,0x03CCE2DF,0x0277436A,0x0722640B,0x0699C5BE,0x04EE86D4,0x05552761,
   0x0E44C816,0x0FFF69A3,0x0D882AC9,0x0C338B7C,0x0966AC1D,0x08DD0DA8,0x0AAA4EC2,0x0B11EF77,
   0x1C89902C,0x1D323199,0x1F4572F3,0x1EFED346,0x1BABF427,0x1A105592,0x186716F8,0x19DCB74D,
   0x12CD583A,0x1376F98F,0x1101BAE5,0x10BA1B50,0x15EF3C31,0x14549D84,0x1623DEEE,0x17987F5B,
   0x39A881ED,0x38132058,0x3A646332,0x3BDFC287,0x3E8AE5E6,0x3F314453,0x3D460739,0x3CFDA68C,
   0x37EC49FB,0x3657E84E,0x3420AB24,0x359B0A91,0x30CE2DF0,0x31758C45,0x3302CF2F,0x32B96E9A,
   0x252111C1,0x249AB074,0x26EDF31E,0x275652AB,0x220375CA,0x23B8D47F,0x21CF9715,0x207436A0,
   0x2B65D9D7,0x2ADE7862,0x28A93B08,0x29129ABD,0x2C47BDDC,0x2DFC1C69,0x2F8B5F03,0x2E30FEB6,
   0x73EAA26F,0x725103DA,0x702640B0,0x719DE105,0x74C8C664,0x757367D1,0x770424BB,0x76BF850E,
   0x7DAE6A79,0x7C15CBCC,0x7E6288A6,0x7FD92913,0x7A8C0E72,0x7B37AFC7,0x7940ECAD,0x78FB4D18,
   0x6F633243,0x6ED893F6,0x6CAFD09C,0x6D147129,0x68415648,0x69FAF7FD,0x6B8DB497,0x6A361522,
   0x6127FA55,0x609C5BE0,0x62EB188A,0x6350B93F,0x66059E5E,0x67BE3FEB,0x65C97C81,0x6472DD34,
   0x4A422382,0x4BF98237,0x498EC15D,0x483560E8,0x4D604789,0x4CDBE63C,0x4EACA556,0x4F1704E3,
   0x4406EB94,0x45BD4A21,0x47CA094B,0x4671A8FE,0x43248F9F,0x429F2E2A,0x40E86D40,0x4153CCF5,
   0x56CBB3AE,0x5770121B,0x55075171,0x54BCF0C4,0x51E9D7A5,0x50527610,0x5225357A,0x539E94CF,
   0x588F7BB8,0x5934DA0D,0x5B439967,0x5AF838D2,0x5FAD1FB3,0x5E16BE06,0x5C61FD6C,0x5DDA5CD9,
   0xE76EE56B,0xE6D544DE,0xE4A207B4,0xE519A601,0xE04C8160,0xE1F720D5,0xE38063BF,0xE23BC20A,
   0xE92A2D7D,0xE8918CC8,0xEAE6CFA2,0xEB5D6E17,0xEE084976,0xEFB3E8C3,0xEDC4ABA9,0xEC7F0A1C,
   0xFBE77547,0xFA5CD4F2,0xF82B9798,0xF990362D,0xFCC5114C,0xFD7EB0F9,0xFF09F393,0xFEB25226,
   0xF5A3BD51,0xF4181CE4,0xF66F5F8E,0xF7D4FE3B,0xF281D95A,0xF33A78EF,0xF14D3B85,0xF0F69A30,
   0xDEC66486,0xDF7DC533,0xDD0A8659,0xDCB127EC,0xD9E4008D,0xD85FA138,0xDA28E252,0xDB9343E7,
   0xD082AC90,0xD1390D25,0xD34E4E4F,0xD2F5EFFA,0xD7A0C89B,0xD61B692E,0xD46C2A44,0xD5D78BF1,
   0xC24FF4AA,0xC3F4551F,0xC1831675,0xC038B7C0,0xC56D90A1,0xC4D63114,0xC6A1727E,0xC71AD3CB,
   0xCC0B3CBC,0xCDB09D09,0xCFC7DE63,0xCE7C7FD6,0xCB2958B7,0xCA92F902,0xC8E5BA68,0xC95E1BDD,
   0x94844704,0x953FE6B1,0x9748A5DB,0x96F3046E,0x93A6230F,0x921D82BA,0x906AC1D0,0x91D16065,
   0x9AC08F12,0x9B7B2EA7,0x990C6DCD,0x98B7CC78,0x9DE2EB19,0x9C594AAC,0x9E2E09C6,0x9F95A873,
   0x880DD728,0x89B6769D,0x8BC135F7,0x8A7A9442,0x8F2FB323,0x8E941296,0x8CE351FC,0x8D58F049,
   0x86491F3E,0x87F2BE8B,0x8585FDE1,0x843E5C54,0x816B7B35,0x80D0DA80,0x82A799EA,0x831C385F,
   0xAD2CC6E9,0xAC97675C,0xAEE02436,0xAF5B8583,0xAA0EA2E2,0xABB50357,0xA9C2403D,0xA879E188,
   0xA3680EFF,0xA2D3AF4A,0xA0A4EC20,0xA11F4D95,0xA44A6AF4,0xA5F1CB41,0xA786882B,0xA63D299E,
   0xB1A556C5,0xB01EF770,0xB269B41A,0xB3D215AF,0xB68732CE,0xB73C937B,0xB54BD011,0xB4F071A4,
   0xBFE19ED3,0xBE5A3F66,0xBC2D7C0C,0xBD96DDB9,0xB8C3FAD8,0xB9785B6D,0xBB0F1807,0xBAB4B9B2,
};

CARM_OP_RDA::CARM_OP_RDA(void)
: m_iCom_Port(0)
, m_iBaud_Rate(0)
, m_bRuning(false)
, m_iStop_Flag(0)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_csCurr_Exe_Path(_T(""))
, m_ulRead_Bytes(0)
, m_ulAll_Bytes(0)
, m_csLod_Product(_T(""))
, m_csLod_XCV_MODEL(_T(""))
, m_chipe_flag(0)
, m_bRDA_download(FALSE)
, m_bPLC_Flag(false)
, m_iPLC_Port(0)
, m_iFS_ROOT_BASE(0)
, m_iFS_ROOT_SIZE(0)
, m_iProdu_Type(0)
, m_bRdaReboot(FALSE)

{
	//��ʼ��PKT_BUFF
	m_str_Pkt_Send.ucp_Buff = NULL;
	m_str_Pkt_Send.ul_Total_Size = 0;
	m_str_Pkt_Send.ul_Data_Length = 0;

	m_str_Com_Rx_Buff.ucp_Buff = NULL;
	m_str_Com_Rx_Buff.ul_Total_Size = 0;
	m_str_Com_Rx_Buff.ul_Data_Length = 0;

	m_conn =NULL;

	m_iError_Code =0;

	this->m_iCfgFileNum =0;
}

CARM_OP_RDA::~CARM_OP_RDA(void)
{
	//�رմ���
	COM_PORT_CLOSE();

	//�ͷ�PKT_BUFF�ڴ�
	if(m_str_Pkt_Send.ucp_Buff != NULL)
	{
		free(m_str_Pkt_Send.ucp_Buff);

		m_str_Pkt_Send.ucp_Buff = NULL;
		m_str_Pkt_Send.ul_Total_Size = 0;
		m_str_Pkt_Send.ul_Data_Length = 0;
	}

	if(m_str_Com_Rx_Buff.ucp_Buff != NULL)
	{
		free(m_str_Com_Rx_Buff.ucp_Buff);

		m_str_Com_Rx_Buff.ucp_Buff = NULL;
		m_str_Com_Rx_Buff.ul_Total_Size = 0;
		m_str_Com_Rx_Buff.ul_Data_Length = 0;
	}
}

void CARM_OP_RDA::Show_Progress(double d_Pgr_Percent)
{
	//�˺�����Ӧ�ð����������ٷֱȵļ��㣬Ȼ������Ϣ��Ӧ��������ɾ��岽�������ļ���
	//Progress�ܳ�����1000
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_RDA::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

static void GetProcessPercent(int percent, void * p_usr_Arg)
{
	double percentOut = (double)percent;
	percentOut =1.0*percentOut/100;
	CString info;

	if (percentOut >0)
	{
		info.Format(_T("%d"),percentOut);
		// 
		CARM_OP_RDA * pSv = (CARM_OP_RDA *)p_usr_Arg;

		pSv->Show_Progress(percentOut);
	}
	
}

int CARM_OP_RDA::Do_Execute_8955(void)
{
	char * cpData = NULL;
	int iTemp=0;
	ChipInfo *chip =NULL;
	FlashProgram *programmer =NULL;

	try
	{
		//��ʼ��ʾ
		this->Show_Status(_T("Waiting......"));

		//��ʼ���������е���ز���
		this->m_iStop_Flag = -1;
		this->m_bRuning = true;
		this->m_csError_Code.Format(_T(""));


		//�����ڴ�
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpData, 512);

		//��Ҫ�����ļ�·��ת��Ϊ��char
		this->m_cla_GFC.WideChar_To_MultiByte(this->m_str_Lod_RamRun.cs_Path, cpData, &iTemp);

		if (false == Load_Com_Conn())
		//if (m_conn == NULL)
		{
			this->Show_Status(_T("Download Port Connect Fail"));
			this->Show_Out(_T("Download Port Connect Fail"));
			this->m_csError_Code.Format(_T("Download Port Connect Fail"));
			this->m_iError_Code  = RDA_CONNECTION_COM_FAIL;

			throw(1);
		}
		ChipInfo *chip = new ChipInfo(m_conn);
		//���˿ڵĶ�ȡ 20190424
		if (chip->probeDevice() != CH_STATUS_SUCCESS)
		{
			//��һ�����
			Sleep(3000);
			if (chip->probeDevice() != CH_STATUS_SUCCESS)
			{
				delete chip;
				delete m_conn;
				m_conn =NULL;
				this->Show_Status(_T("Unable To Upgrade, Please Check!"));
				this->Show_Out(_T("probeDevice Fail"));
				this->m_csError_Code.Format(_T("Module Flash Can\'t Write"));
				this->m_iError_Code  = RDA_ProbeDevice_FAIL;
				throw(1);
			}

		}

		this->Show_Out(_T("hostPrepareRamRun..."));
		if (0 != chip->hostPrepareRamRun(false, false))
		{
			delete chip;
			delete m_conn;
			m_conn =NULL;
			this->Show_Status(_T("hostPrepareRamRun Fail"));
			this->Show_Out(_T("hostPrepareRamRun Fail"));
			this->m_csError_Code.Format(_T("Prepare RamRun File Fail"));
			this->m_iError_Code  = RDA_HOSTPREPARERAMRUN_FAIL;
			throw(1);
		}
		this->Show_Status(_T("hostPrepareRamRun Pass"));
		this->Show_Out(_T("hostPrepareRamRun Pass"));

		programmer = new FlashProgram(m_conn, chip);
		LodData ramrunLod;
		this->Show_Out(_T("ramrunLod.load..."));

		if (!ramrunLod.load(cpData))
		{
			delete chip;
			delete m_conn;
			delete programmer;
			m_conn =NULL;
			this->Show_Status(_T("load ramrun Fail"));
			this->Show_Out(_T("ramrunLod.load Fail"));
			this->m_csError_Code.Format(_T("Load Ramrun Fail"));
			this->m_iError_Code  = RDA_LOAD_RAMRUN_FAIL;
			throw(1);
		}
		this->Show_Out(_T("ramrunLod.load Pass"));
		this->Show_Status(_T("load ramrun Pass"));

		//20190325
		if (FALSE == m_bRDA_download)//����
		{
			programmer->setWaitEventTimeout(10000);
		}
		else
		{
			programmer->setWaitEventTimeout(8000);
		}

		RamRun ramrun;
		ramrun.init(chip, &ramrunLod, m_conn);
		this->Show_Status(_T("Ramrun Run..."));
		this->Show_Out(_T("Ramrun Run..."));
		if (0 != ramrun.run())
		{
			delete chip;
			delete programmer;
			delete m_conn;
			m_conn =NULL;
			this->Show_Status(_T("Ramrun Run Fail"));
			this->Show_Out(_T("Ramrun Run Fail"));
			this->m_csError_Code.Format(_T("Ramrun Run Fail"));
			this->m_iError_Code  = RDA_RAMRUN_RUN_FAIL;
			throw(2);
		}
		this->Show_Status(_T("Ramrun Run Pass"));
		this->Show_Out(_T("Ramrun Run Pass"));

		this->Show_Status(_T("checkStart..."));
		this->Show_Out(_T("checkStart..."));
		if (0 != programmer->checkStart())
		{
			delete chip;
			delete programmer;
			delete m_conn;
			m_conn =NULL;
			this->Show_Status(_T("checkStart Fail"));
			this->Show_Out(_T("checkStart Fail"));
			this->m_csError_Code.Format(_T("checkStart Fail"));
			this->m_iError_Code  = RDA_CHECKSTEART_FAIL;
			throw(3);
		}
		this->Show_Out(_T("checkStart Pass"));
		this->Show_Status(_T("checkStart Pass"));

		this->Show_Status(_T("Program DL Start..."));
		this->Show_Out(_T("Program DL Start..."));
		if (0 != programmer->program(&m_Lod,GetProcessPercent,this))
		{
			delete chip;
			delete programmer;
			delete m_conn;
			m_conn =NULL;
			this->Show_Status(_T("Program Fail"));
			this->Show_Out(_T("Program DL Fail"));
			this->m_csError_Code.Format(_T("Program DL Fail"));
			this->m_iError_Code  = RDA_PROGRAM_DL_FAIL;
			throw(4);
		}
		this->Show_Out(_T("Program DL Pass"));

		this->Show_Out(_T("verify Start..."));
		if (0 != programmer->verify(&m_Lod, true))
		{
			delete chip;
			delete programmer;
			delete m_conn;
			m_conn =NULL;
			this->Show_Status(_T("verify Fail"));
			this->Show_Out(_T("verify Fail"));
			this->m_csError_Code.Format(_T("Verify Fail"));
			this->m_iError_Code  = RDA_VERIFY_FAIL;
			throw(5);
		}
		this->Show_Out(_T("verify Pass"));

		

		
		//20190227
		//�������������AT&W����Ĳ���
		if (FALSE == m_bRDA_download)//����
		{
			this->Show_Status(_T("Begin to erase user data ..."));
			this->Show_Out(_T("Begin to erase user data ..."));
			this->Save_Trace_Log(_T("Begin to erase user data ..."));
			//#$FS_ROOT_BASE=0x390000
			//#$FS_ROOT_SIZE=0x66000
			int  iXfbp = 0;
			CString strCause;
			//iXfbp = chip->xfbp();
			if (0 == iXfbp) //��֮ǰ���ϵ�Firmware����
			{
				if ((m_iFS_ROOT_BASE != 0) &&(m_iFS_ROOT_SIZE !=  0))
				{
					programmer->setWaitEventTimeout(20000);//��ʱ���õ�20��

					int iEraseFlag =0;
					iEraseFlag = programmer->erase(m_iFS_ROOT_BASE,m_iFS_ROOT_SIZE);
					if (0 != iEraseFlag)
					{
						delete chip;
						delete programmer;
						delete m_conn;
						m_conn =NULL;
						
						strCause.Format(_T("Erase FAIL(ROOT_BASE=0X%x,ROOT_SIZE=0X%x)-%d"),m_iFS_ROOT_BASE,m_iFS_ROOT_SIZE,iEraseFlag);

						this->Show_Status(strCause);
						this->Show_Out(strCause);
						this->Save_Trace_Log(strCause);

						this->m_csError_Code=strCause;
						this->m_iError_Code  = RDA_ERASE_USER_DATA_FAIL;
						throw(6);
					}
					else
					{
						this->Save_Trace_Log(_T("Erase Sucess"));
						this->Show_Status(_T("Erase user data Pass"));
					}
				} 
				else
				{
					this->Save_Trace_Log(_T("FS_ROOT_BASE-FS_ROOT_SIZE=0"));
				}
	
			}
			else
			{
				strCause.Format(_T("Xfbp=%d"),iXfbp);
				this->Save_Trace_Log(strCause);

				this->Save_Trace_Log(_T("Not Need Erase"));

			}
			//this->Show_Status(_T("Erase user data Pass"));
			//this->Show_Out(_T("Eraseuser data Pass"));

		}
		else
		{
			//20210508
			if (TRUE == this->m_bRdaReboot)
			{
				this->Show_Status(_T("Reboot..."));
				this->Show_Out(_T("Reboot..."));
				programmer->restartChip();
				Sleep(5000);
			}
		}

		this->Show_Out(_T("FW download success."));

		//this->Show_Status(_T("FW download success."));
	}
	catch(...)
	{
		//
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);
		this->m_iStop_Flag = -1;
		this->m_bRuning = false;

		return RTN_FAIL;
	}

	//
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);
	this->m_iStop_Flag = -1;
	this->m_bRuning = false;

	delete chip;
	delete m_conn;
	m_conn =NULL;
	delete programmer;

	return RTN_SUCCESS;


}
int CARM_OP_RDA::Do_Execute_8955_Sub(void)
{
	char * cpData = NULL;
	int iTemp=0;
	ChipInfo *chip =NULL;
	FlashProgram *programmer =NULL;

	CString strData;

	//��ʼ���������е���ز���
	this->m_iStop_Flag = -1;
	this->m_bRuning = true;
	this->m_csError_Code.Format(_T(""));

	int i =0;

	for (i=0;(i<m_Lod_Num) &&(i<MAX_LOD_FILE);i++ )
	{
		try
		{
			this->Show_Progress(0);

			//��ʼ��ʾ
			this->Show_Status(_T("Waiting......"));

			strData.Format(_T("DL Sub Lod-%d-%d"),i,m_Lod_Num);

			this->Show_Out(strData);

			//��ʼ���������е���ز���

			//�����ڴ�
			this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpData, 512);

			//��Ҫ�����ļ�·��ת��Ϊ��char
			this->m_cla_GFC.WideChar_To_MultiByte(this->m_str_Lod_RamRun.cs_Path, cpData, &iTemp);

			if (false == Load_Com_Conn())
			{
				this->Show_Status(_T("Connection Com Fail"));
				this->Show_Out(_T("Connection Com Fail"));
				this->m_csError_Code.Format(_T("Connection Com Fail"));
				this->m_iError_Code  = RDA_CONNECTION_COM_FAIL;

				throw(1);
			}

			ChipInfo *chip = new ChipInfo(m_conn);

			//���˿ڵĶ�ȡ 20190424
			if (chip->probeDevice() != CH_STATUS_SUCCESS)
			{
				//��һ�����
				Sleep(3000);
				if (chip->probeDevice() != CH_STATUS_SUCCESS)
				{
					delete chip;
					delete m_conn;
					m_conn =NULL;
					this->Show_Status(_T("Unable To Upgrade, Please Check!"));
					this->Show_Out(_T("probeDevice Fail"));
					this->m_csError_Code.Format(_T("Module Flash Can\'t Write"));
					this->m_iError_Code  = RDA_ProbeDevice_FAIL;
					throw(1);
				}

			}

			this->Show_Out(_T("hostPrepareRamRun..."));
			if (0 != chip->hostPrepareRamRun(false, false))
			{
				delete chip;
				delete m_conn;
				m_conn =NULL;
				this->Show_Status(_T("hostPrepareRamRun Fail"));
				this->Show_Out(_T("hostPrepareRamRun Fail"));
				this->m_csError_Code.Format(_T("hostPrepareRamRun Fail"));
				this->m_iError_Code  = RDA_HOSTPREPARERAMRUN_FAIL;
				throw(1);
			}
			this->Show_Status(_T("hostPrepareRamRun Pass"));
			this->Show_Out(_T("hostPrepareRamRun Pass"));

			programmer = new FlashProgram(m_conn, chip);
			LodData ramrunLod;
			this->Show_Out(_T("ramrunLod.load..."));

			if (!ramrunLod.load(cpData))
			{
				delete chip;
				delete m_conn;
				delete programmer;
				m_conn =NULL;
				this->Show_Status(_T("load ramrun Fail"));
				this->Show_Out(_T("ramrunLod.load Fail"));
				this->m_csError_Code.Format(_T("load ramrun Fail"));
				this->m_iError_Code  = RDA_LOAD_RAMRUN_FAIL;
				throw(1);
			}
			this->Show_Out(_T("ramrunLod.load Pass"));
			this->Show_Status(_T("load ramrun Pass"));


			programmer->setWaitEventTimeout(10000);


			RamRun ramrun;
			ramrun.init(chip, &ramrunLod, m_conn);
			this->Show_Status(_T("Ramrun Run..."));
			this->Show_Out(_T("Ramrun Run..."));
			if (0 != ramrun.run())
			{
				delete chip;
				delete programmer;
				delete m_conn;
				m_conn =NULL;
				this->Show_Status(_T("Ramrun Run Fail"));
				this->Show_Out(_T("Ramrun Run Fail"));
				this->m_csError_Code.Format(_T("Ramrun Run Fail"));
				this->m_iError_Code  = RDA_RAMRUN_RUN_FAIL;
				throw(2);
			}
			this->Show_Status(_T("Ramrun Run Pass"));
			this->Show_Out(_T("Ramrun Run Pass"));

			this->Show_Status(_T("checkStart..."));
			this->Show_Out(_T("checkStart..."));
			if (0 != programmer->checkStart())
			{
				delete chip;
				delete programmer;
				delete m_conn;
				m_conn =NULL;
				this->Show_Status(_T("checkStart Fail"));
				this->Show_Out(_T("checkStart Fail"));
				this->m_csError_Code.Format(_T("checkStart Fail"));
				this->m_iError_Code  = RDA_CHECKSTEART_FAIL;
				throw(3);
			}
			this->Show_Out(_T("checkStart Pass"));
			this->Show_Status(_T("checkStart Pass"));


			this->Show_Status(_T("Program DL Start..."));
			this->Show_Out(_T("Program DL Start..."));
			if (0 != programmer->program(&(m_Lod_Sub[i]),GetProcessPercent,this))
			{
				delete chip;
				delete programmer;
				delete m_conn;
				m_conn =NULL;
				this->Show_Status(_T("Program Fail"));
				this->Show_Out(_T("Program DL Fail"));
				this->m_csError_Code.Format(_T("Program DL Fail"));
				this->m_iError_Code  = RDA_PROGRAM_DL_FAIL;
				throw(4);
			}
			this->Show_Out(_T("Program DL Pass"));

			this->Show_Out(_T("verify Start..."));
			if (0 != programmer->verify(&(m_Lod_Sub[i]), true))
			{
				delete chip;
				delete programmer;
				delete m_conn;
				m_conn =NULL;
				this->Show_Status(_T("verify Fail"));
				this->Show_Out(_T("verify Fail"));
				this->m_csError_Code.Format(_T("Verify Fail"));
				this->m_iError_Code  = RDA_VERIFY_FAIL;
				throw(5);
			}
			this->Show_Out(_T("verify Pass"));

			//���������������һ�������� ��Ҫ���NV
			if (i+1 == m_Lod_Num)
			{
				//20190227
				//�������������AT&W����Ĳ���
				if (FALSE == m_bRDA_download)//����
				{
					this->Show_Status(_T("Begin to erase user data ..."));
					this->Show_Out(_T("Begin to erase user data ..."));
					this->Save_Trace_Log(_T("Begin to erase user data ..."));
					//#$FS_ROOT_BASE=0x390000
					//#$FS_ROOT_SIZE=0x66000
					int  iXfbp = 0;
					CString strCause;
					//iXfbp = chip->xfbp();
					if (0 == iXfbp) //��֮ǰ���ϵ�Firmware����
					{
						if ((m_iFS_ROOT_BASE != 0) &&(m_iFS_ROOT_SIZE !=  0))
						{
							programmer->setWaitEventTimeout(20000);//��ʱ���õ�20��

							int iEraseFlag =0;
							iEraseFlag = programmer->erase(m_iFS_ROOT_BASE,m_iFS_ROOT_SIZE);
							if (0 != iEraseFlag)
							{
								delete chip;
								delete programmer;
								delete m_conn;
								m_conn =NULL;

								strCause.Format(_T("Erase FAIL(ROOT_BASE=0X%x,ROOT_SIZE=0X%x)-%d"),m_iFS_ROOT_BASE,m_iFS_ROOT_SIZE,iEraseFlag);

								this->Show_Status(strCause);
								this->Show_Out(strCause);
								this->Save_Trace_Log(strCause);

								this->m_csError_Code=strCause;
								this->m_iError_Code  = RDA_ERASE_USER_DATA_FAIL;
								throw(6);
							}
							else
							{
								this->Save_Trace_Log(_T("Erase Sucess"));
								this->Show_Status(_T("Erase user data Pass"));
							}
						} 
						else
						{
							this->Save_Trace_Log(_T("FS_ROOT_BASE-FS_ROOT_SIZE=0"));
						}

					}
					else
					{
						strCause.Format(_T("Xfbp=%d"),iXfbp);
						this->Save_Trace_Log(strCause);

						this->Save_Trace_Log(_T("Not Need Erase"));

					}
					//this->Show_Status(_T("Erase user data Pass"));
					//this->Show_Out(_T("Eraseuser data Pass"));

				}
				else
				{
					//20210508
					if (TRUE == this->m_bRdaReboot)
					{
						this->Show_Status(_T("Reboot..."));
						this->Show_Out(_T("Reboot..."));
						programmer->restartChip();
						Sleep(5000);
					}
				}
			}


			this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);

			delete chip;
			delete m_conn;
			m_conn =NULL;
			delete programmer;

			this->Show_Out(_T("FW download success."));

			//this->Show_Status(_T("FW download success."));
		}
		catch(...)
		{
			//
			this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);
			this->m_iStop_Flag = -1;
			this->m_bRuning = false;

			return RTN_FAIL;
		}

	}

	//
	
	this->m_iStop_Flag = -1;
	this->m_bRuning = false;

	return RTN_SUCCESS;

}

int CARM_OP_RDA::Do_Execute(void)
{
	DWORD dwChip_ID;

	this->m_csError_Code = _T("");

	//20180930 MC25 8955оƬ
	if (1==m_chipe_flag)
	{
		int ret=RTN_FAIL;
		if (this->m_iCfgFileNum >0)
		{
			ret= Do_Execute_8955_Sub();	
		}
		else
		{
			ret= Do_Execute_8955();
		}
		
		return ret;
	}


	try
	{
		//��ʼ��ʾ
		this->Show_Status(_T("Waiting......"));

		//��ʼ���������е���ز���
		this->m_iStop_Flag = -1;
		this->m_bRuning = true;
		this->m_csError_Code.Format(_T(""));

		//
		this->m_ulRead_Bytes = 0;

		//------------------------------ ��鴮���Ƿ���� ------------------------------ 
		if(this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iCom_Port) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Confirm com port %d fail!"), this->m_iCom_Port);
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//�ͷ�PKT_BUFF�ڴ�
		this->m_cla_GFC.PKT_BUFF_FREE(&this->m_str_Pkt_Send);
		this->m_cla_GFC.PKT_BUFF_FREE(&this->m_str_Com_Rx_Buff);

		//�����Ŵ��ڽ��յ������ݵġ�����BUFF���ڴ�
		this->m_str_Com_Rx_Buff.ul_Total_Size = 4096;
		this->m_cla_GFC.PKT_BUFF_CALLOC(&this->m_str_Com_Rx_Buff, this->m_str_Com_Rx_Buff.ul_Total_Size);

		//------------------------------ �򿪴��� ------------------------------ 
		this->Show_Status(_T("Open com port......"));
		//���봮�ڲ���
		this->COM_PORT_INIT();
		//�򿪴���
		if(this->COM_PORT_OPEN() == false)
		{
			this->Show_Status(this->m_cla_Host_Uart.m_csError_Info);
			throw(0);
		}

		//------------------------------ ̽�⴮�����Ƿ���ģ�� ------------------------------ 
		this->Show_Status(_T("Probe port......"));
		if(this->Probe_Port() == false)
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//------------------------------ Lock XCPU ------------------------------ 
		this->Show_Status(_T("Lock XCPU......"));
		if(this->Lock_XCPU() == false)
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//------------------------------ ��ȡFlash��Ϣ ------------------------------
		/*
		int iFlash_Idx;
		CString csFlash_Name;

		this->Show_Status(_T("Get flash info......"));//��FWû����������ʱ��̽���Flash��Ϣ�᲻׼

		if(this->Read_Spi_Flash(&iFlash_Idx) == true)
		{
			if(this->Get_Spi_Flash_Name(iFlash_Idx, &csFlash_Name) == true)
			{
				this->Show_Status(csFlash_Name);
			}
			else
			{
				this->Show_Status(this->m_csError_Code);
				throw(0);
			}
		}
		else
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}
		*/

		//------------------------------ Enter_Host_Mode ------------------------------ 
		this->Show_Status(_T("Enter Host Mode......"));
		if(this->Enter_Host_Mode() == false)
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//------------------------------ Config_Ebc_Ram ------------------------------ 
		this->Show_Status(_T("Config Ebc Ram......"));
		if(this->Config_Ebc_Ram() == false)
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//------------------------------ Read_Chip_ID ------------------------------ 
		this->Show_Status(_T("Read Chip ID......"));
		if(this->Read_Chip_ID(&dwChip_ID) == false)
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		if(dwChip_ID != 0x8809)//20180929 MC25ADD
		{
			this->m_csError_Code.Format(_T("FAIL, Do not support HW %04X"), dwChip_ID);
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//------------------------------ ����ramrun.lod�ļ� ------------------------------ 
		this->Show_Status(_T("Download ramrun file......"));
		if(this->DL_RAMRUN_LOD_FILE(this->m_str_Lod_RamRun) == false)
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//------------------------------ ��FPC Access Info ------------------------------ 
		this->Show_Status(_T("Read FPC Access Info......"));
		if(this->Read_FPC_Access_Info(&this->m_str_Fpc_Access_Info) == false)
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//------------------------------ ����FW.lod�ļ� ------------------------------ 
		this->Show_Status(_T("Download fw file......"));
		if(this->DL_FW_LOD_FILE(this->m_str_Lod_FW) == false)
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//�رմ���
		this->COM_PORT_CLOSE();

		//�ͷ�PKT_BUFF�ڴ�
		this->m_cla_GFC.PKT_BUFF_FREE(&this->m_str_Pkt_Send);
		this->m_cla_GFC.PKT_BUFF_FREE(&this->m_str_Com_Rx_Buff);

		this->Show_Status(_T("FW upgrade success."));
	}
	catch(...)
	{
		//�رմ���
		this->COM_PORT_CLOSE();

		//�ͷ�PKT_BUFF�ڴ�
		this->m_cla_GFC.PKT_BUFF_FREE(&this->m_str_Pkt_Send);
		this->m_cla_GFC.PKT_BUFF_FREE(&this->m_str_Com_Rx_Buff);

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

void CARM_OP_RDA::COM_PORT_INIT(void)
{
	//ͨ�ſڳ�ʼ������ֵ��
	this->m_cla_Host_Uart.m_iCom_Port_Numb = this->m_iCom_Port;
	this->m_cla_Host_Uart.m_iCom_Baud_Rate = this->m_iBaud_Rate;
	this->m_cla_Host_Uart.m_iCom_Flow_Mode = CHFLOW_XONXOFF;
	this->m_cla_Host_Uart.m_cXonChar = ASYNC_HDLC_ESC_X_ON & 0xFF;
	this->m_cla_Host_Uart.m_cXoffChar = ASYNC_HDLC_ESC_X_OFF & 0xFF;
}

bool CARM_OP_RDA::Add_Byte_To_Packet(PKT_BUFF_S * strp_Pkt_Cmd, const unsigned char uc_Byte, bool b_Escape_Flag)
{
	unsigned char ucTemp;

	try
	{
		if((b_Escape_Flag == true) &&
			((uc_Byte == ASYNC_HDLC_ESC_X_ON) || (uc_Byte == ASYNC_HDLC_ESC_X_OFF) || (uc_Byte == ASYNC_HDLC_ESC_MASK_R)))
		{
			//��Flag�ַ�����Ҫ��ת�⡱����
			/*
			*(strp_Pkt_Cmd->ucp_Buff + strp_Pkt_Cmd->ul_Data_Length) = ASYNC_HDLC_ESC_MASK_R & 0xFF;
			*(strp_Pkt_Cmd->ucp_Buff + strp_Pkt_Cmd->ul_Data_Length + 1) = 0xFF - (uc_Byte & 0xFF);

			strp_Pkt_Cmd->ul_Data_Length += 2;
			*/

			ucTemp = ASYNC_HDLC_ESC_MASK_R & 0xFF;
			if(this->m_cla_GFC.PKT_BUFF_ADD_DATA(strp_Pkt_Cmd, &ucTemp, 1) == false)
			{
				throw(0);
			}

			ucTemp = 0xFF - (uc_Byte & 0xFF);
			if(this->m_cla_GFC.PKT_BUFF_ADD_DATA(strp_Pkt_Cmd, &ucTemp, 1) == false)
			{
				throw(0);
			}
		}
		else
		{
			//�����С�ת�⡱����
			/*
			*(strp_Pkt_Cmd->ucp_Buff + strp_Pkt_Cmd->ul_Data_Length) = uc_Byte & 0xFF;

			strp_Pkt_Cmd->ul_Data_Length++;
			*/

			ucTemp = uc_Byte & 0xFF;
			if(this->m_cla_GFC.PKT_BUFF_ADD_DATA(strp_Pkt_Cmd, &ucTemp, 1) == false)
			{
				throw(0);
			}
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CARM_OP_RDA::PKT_CMD_LOAD_BYTE(unsigned char * ucp_Data, unsigned long ul_Data_Size, unsigned long ul_Addr, unsigned char uc_Cmd, PKT_BUFF_S * strp_Pkt_Cmd)
{
	//�����Э�������������BYTE��ʽ
	unsigned long ulCRC_Data_Size = ul_Data_Size + 6;//����У��λ�������������
	unsigned long ulBuff_Size = 3 + ulCRC_Data_Size + 1 + (8 + ulCRC_Data_Size / 16);//��������װ������BUFF�ĳߴ磬���ڻ���ת��λ����ȡ��������1/16�Լ�8λ������������Ϊ����
	unsigned char ucCRC16;
	BYTE temp_byte;
	unsigned long j;

	try
	{
		//���ͷ��ڴ�
		this->m_cla_GFC.PKT_BUFF_FREE(strp_Pkt_Cmd);

		//�����ڴ�
		this->m_cla_GFC.PKT_BUFF_CALLOC(strp_Pkt_Cmd, ulBuff_Size);

		//�齨ADDR����֣���AD��׼������2λ���ݳ���λ������3��BYTE
		//addr���ݲ��֣�Ҫת��
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, ASYNC_HDLC_AD_FLAG, true) == false)
		{
			throw(1);
		}
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, ((ulCRC_Data_Size >> 8) & 0xFF), true) == false)
		{
			throw(2);
		}
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, (ulCRC_Data_Size & 0xFF), true) == false)
		{
			throw(3);
		}

		//�齨FF���ݲ��֣���FF��׼������1λ����λ������4λ����Ŀ���ַλ������ʵ�����ݡ�����1λУ��λ����
		//FFͷ
		ucCRC16 = ASYNC_HDLC_FF_FLAG & 0xFF;
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, ASYNC_HDLC_FF_FLAG, true) == false)
		{
			throw(4);
		}
		//����λ
		ucCRC16 = ucCRC16 ^ uc_Cmd;
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, uc_Cmd, true) == false)
		{
			throw(5);
		}

		//4λ����Ŀ���ַλ
		temp_byte = (BYTE)(ul_Addr & 0xff);
		ucCRC16 = ucCRC16 ^ temp_byte;
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, temp_byte, true) == false)
		{
			throw(6);
		}
		temp_byte = (BYTE)((ul_Addr >> 8) & 0xff);
		ucCRC16 = ucCRC16 ^ temp_byte;
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, temp_byte, true) == false)
		{
			throw(7);
		}
		temp_byte = (BYTE)((ul_Addr >> 16) & 0xff);
		ucCRC16 = ucCRC16 ^ temp_byte;
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, temp_byte, true) == false)
		{
			throw(8);
		}
		temp_byte = (BYTE)((ul_Addr >> 24) & 0xff);
		ucCRC16 = ucCRC16 ^ temp_byte;
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, temp_byte, true) == false)
		{
			throw(9);
		}

		//ʵ������
		for(j=0; j<ul_Data_Size; j++)
		{
			ucCRC16 = ucCRC16 ^ *(ucp_Data + j);
			if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, *(ucp_Data + j), true) == false)
			{
				throw(10);
			}
		}

		//���У��λ��1λ��
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, (ucCRC16 & 0xFF), true) == false)
		{
			throw(14);
		}
	}
	catch(int iErr)
	{
		this->m_csError_Code.Format(_T("PKT_CMD_LOAD_BYTE, Error:%d"), iErr);

		return false;
	}

	return true;
}

bool CARM_OP_RDA::PKT_CMD_LOAD_DWORD(DWORD * dwp_Data, unsigned long ul_Data_Size, unsigned long ul_Addr, unsigned char uc_Cmd, PKT_BUFF_S * strp_Pkt_Cmd)
{
	//�����Э�������������DWORD��ʽ
	unsigned long ulCRC_Data_Size = ul_Data_Size * 4 + 6;//1��DWORDռ4��BYTE������4���ʹӡ�FFͷ����ʼ��6λ���ݡ�����У��λ�������������
	unsigned long ulBuff_Size = 3 + ulCRC_Data_Size + 1 + (8 + ulCRC_Data_Size / 16);//��������װ������BUFF�ĳߴ磬���ڻ���ת��λ����ȡ��������1/16�Լ�8λ������������Ϊ����
	unsigned char ucCRC16;
	BYTE temp_byte;
	unsigned long j, a;

	try
	{
		//���ͷ��ڴ�
		this->m_cla_GFC.PKT_BUFF_FREE(strp_Pkt_Cmd);

		//�����ڴ�
		this->m_cla_GFC.PKT_BUFF_CALLOC(strp_Pkt_Cmd, ulBuff_Size);

		//�齨ADDR����֣���AD��׼������2λ���ݳ���λ������3��BYTE
		//addr���ݲ��֣�Ҫת��
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, ASYNC_HDLC_AD_FLAG, true) == false)
		{
			throw(1);
		}
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, ((ulCRC_Data_Size >> 8) & 0xFF), true) == false)
		{
			throw(2);
		}
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, (ulCRC_Data_Size & 0xFF), true) == false)
		{
			throw(3);
		}

		//�齨FF���ݲ��֣���FF��׼������1λ����λ������4λ����Ŀ���ַλ������ʵ�����ݡ�����1λУ��λ����
		//FFͷ���Ӵ����ݿ�ʼ����У��λ
		ucCRC16 = ASYNC_HDLC_FF_FLAG & 0xFF;
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, ASYNC_HDLC_FF_FLAG, true) == false)
		{
			throw(4);
		}

		//����λ
		ucCRC16 = ucCRC16 ^ uc_Cmd;
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, uc_Cmd, true) == false)
		{
			throw(5);
		}

		//4λBYTE����Ŀ���ַλ
		for(a=0; a<4; a++)
		{
			temp_byte = (BYTE)((ul_Addr >> (a * 8)) & 0xFF);

			ucCRC16 = ucCRC16 ^ temp_byte;

			if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, temp_byte, true) == false)
			{
				throw(6);
			}
		}

		//ʵ������
		for(j=0; j<ul_Data_Size; j++)//DWORD����
		{
			for(a=0; a<4; a++)//1��DWORDռ4��BYTE
			{
				temp_byte = (BYTE)((*(dwp_Data + j) >> (a * 8)) & 0xFF);

				ucCRC16 = ucCRC16 ^ temp_byte;

				if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, temp_byte, true) == false)
				{
					throw(10);
				}
			}
		}

		//���У��λ��1λ��
		if(this->Add_Byte_To_Packet(strp_Pkt_Cmd, (ucCRC16 & 0xFF), true) == false)
		{
			throw(14);
		}
	}
	catch(int iErr)
	{
		this->m_csError_Code.Format(_T("PKT_CMD_LOAD_DWORD, Error:%d"), iErr);

		return false;
	}

	return true;
}

bool CARM_OP_RDA::Read_DWORD(DWORD dw_Read_Addr, DWORD * dwp_Read_Data, int i_TimeOut)
{
	//���յ�ַ�����ж�ȡ1��DWORD����
	unsigned char ucRd_ID_Send = 0x03;//����д�ġ�������ʶ��ID��
	unsigned long ulRd_Cnt = 4;//��ظ�Ӧ����4λBYTE

	unsigned char ucRd_ID_Read;//���صġ�������ʶ��ID��
	unsigned char * ucpRd_Data = NULL;//��ȡ���������ݡ���BUFF
	unsigned long ulRd_Data_Quan;//���������ݡ�������

	bool bRtn = false;
	
	try
	{
		//�������DWORD������ָ��
		if(this->PKT_CMD_LOAD_BYTE(&ucRd_ID_Send, 1, dw_Read_Addr, CMD_RD_DWORD, &this->m_str_Pkt_Send) == false)
		{
			throw(0);
		}

		//--------------------------- ���͡���DWORD������ָ�����ȡ��Ӧ --------------------------- 
		if(this->QUERY_PKT(this->m_str_Pkt_Send, &this->m_str_Com_Rx_Buff, i_TimeOut) == false)
		{
			throw(0);
		}

		//--------------------------- ȷʵ�ж�����Ӧ���ݣ�����Է����ڴ棬׼���������� --------------------------- 
		//�����ڴ棬���������ݡ���BUFF�����С��m_str_Com_Rx_Buff�Ĵ�Сһ��
		this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpRd_Data, this->m_str_Com_Rx_Buff.ul_Data_Length);
		
		//--------------------------- �����յ������� --------------------------- 
		//ѭ������m_str_Com_Rx_Buff�е����ݣ�ֱ�������Ѿ�û���ˡ��Ϸ���Э�����ݰ�������˳�ѭ��
		while(this->GET_DATA_FROM_READ_PKT(&this->m_str_Com_Rx_Buff, &ucRd_ID_Read, ucpRd_Data, &ulRd_Data_Quan) == true)
		{
			//��ȡ��һ���Ϸ����ݣ�����ǲ���������ζ������ġ��Ϸ�Э�����ݰ�������Ӧ
			if((ucRd_ID_Read == ucRd_ID_Send) && (ulRd_Data_Quan == ulRd_Cnt))
			{
				//rd_id��ȷ������������Ҳ��ȷ�����Ƕ�����������ζ���������Ӧ
				//����һ��DWORD���ݣ�4��BYTE��
				*dwp_Read_Data = 0x0;
				*dwp_Read_Data = *dwp_Read_Data | (*(ucpRd_Data + 0)) | ((*(ucpRd_Data + 1)) << 8) | ((*(ucpRd_Data + 2)) << 16) | ((*(ucpRd_Data + 3)) << 24);
				

				/*
				this->m_csError_Code.Format(_T("[ %02X %02X %02X %02X ] 0x%08X"),
					*(ucpRd_Data + 0), *(ucpRd_Data + 1), *(ucpRd_Data + 2), *(ucpRd_Data + 3), *dwp_Read_Data);
				::AfxMessageBox(this->m_csError_Code);
				*/


				bRtn = true;//PASS
				break;//�˳�whileѭ��
			}
		}
	}
	catch(...)
	{
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRd_Data);
		return false;
	}

	this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRd_Data);
	return bRtn;
}

bool CARM_OP_RDA::WRITE_PKT(const PKT_BUFF_S str_Tx_Pkt_Buff)
{
	return this->m_cla_Host_Uart.Write_Byte(str_Tx_Pkt_Buff.ucp_Buff, str_Tx_Pkt_Buff.ul_Data_Length);
}

void CARM_OP_RDA::Clear_Com_Rx_Buffer(void)
{
	this->m_cla_Host_Uart.Clear_Rx_Buffer();
}

bool CARM_OP_RDA::COM_PORT_OPEN(void)
{
	return this->m_cla_Host_Uart.Port_Open();
}

void CARM_OP_RDA::COM_PORT_CLOSE(void)
{
	this->m_cla_Host_Uart.Port_Close();
}

bool CARM_OP_RDA::READ_PKT(PKT_BUFF_S * strp_Rx_Pkt_Buff)
{
	//ͬ��ģʽ�£���һ�ζ����ڵĲ�����������BUFF

	unsigned long ulExp_Read_Com, ulValid_Quan;
	unsigned char * ucpData = NULL;	//�����ݴӡ�����BUFF����ȡ���ŵġ���ʱ�ڴ桱

	try
	{
		ulExp_Read_Com = this->m_cla_Host_Uart.Get_Rx_Buff_Valid();

		if(ulExp_Read_Com <= 0)//����BUFF�������ݿɶ�
		{
			return true;
		}

		//�������������ݱ��ȡ
		//���䡰���������ݡ��ġ���ʱ�ڴ桱
		ulValid_Quan = ulExp_Read_Com + 8;
		ucpData = (unsigned char *)calloc(ulValid_Quan, sizeof(unsigned char));
		memset(ucpData, 0, ulValid_Quan);

		//����������
		//ע�⣬��������������С�����BUFF������ǰһ������һ��ʵ��������������������
		//�����еġ�������������ʱ���ڷ����仯����ǰ̽����������ܿ��ܹ��˼����벻׼��
		ulValid_Quan = 0;
		this->m_cla_Host_Uart.Read_Byte(ucpData, ulExp_Read_Com, &ulValid_Quan);

		if(ulExp_Read_Com != ulValid_Quan)
		{
			this->m_csError_Code.Format(_T("ERROR, READ_PKT, Read byte fail, E = %d, R = %d!"), ulExp_Read_Com, ulValid_Quan);
			throw(0);
		}

		if(ulValid_Quan <= 0)
		{
			this->m_csError_Code.Format(_T("FAIL, READ_PKT, COM port read byte error!"));
			throw(0);
		}

		//��ӵ�COM BUFF��
		if(this->m_cla_GFC.PKT_BUFF_ADD_DATA(strp_Rx_Pkt_Buff, ucpData, ulValid_Quan) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, READ_PKT, COM buff add byte error!"));
			throw(0);
		}
	}
	catch(...)
	{
		//�ͷ��ڴ�
		if(ucpData != NULL)
		{
			free(ucpData);
			ucpData = NULL;
		}

		return false;
	}

	//�ͷ��ڴ�
	if(ucpData != NULL)
	{
		free(ucpData);
		ucpData = NULL;
	}

	return true;
}

bool CARM_OP_RDA::QUERY_PKT(PKT_BUFF_S str_Send_Pkt, PKT_BUFF_S * strp_Read_Pkt, int i_TimeOut)
{
	//����ָ��󣬰ѳ�ʱ���������ݶ������ڡ�this->m_str_Com_Rx_Buff���У�������BUFF�ĵ�ַ��������
	try
	{
		//��մ�����
		this->m_csError_Code.Format(_T(""));

		//��ս���BUFF
		this->Clear_Com_Rx_Buffer();

		//����ָ��
		if(this->WRITE_PKT(str_Send_Pkt) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Query pkt, Write pkt, %s"), this->m_cla_Host_Uart.m_csError_Info);
			throw(0);
		}

		//��ս���COM�������ݵ�BUFF
		this->m_cla_GFC.PKT_BUFF_DEL_ALL(strp_Read_Pkt);

		//�ռ��ظ���Ӧ
		if(this->READ_RSP_PKT(strp_Read_Pkt, i_TimeOut) == false)
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

bool CARM_OP_RDA::GET_DATA_FROM_READ_PKT(PKT_BUFF_S * strp_Read_Pkt, unsigned char * ucp_Rd_ID, unsigned char * ucp_Read_Data, unsigned long * ulp_Read_Quan)
{
	//��PKT_BUFF_S��ȡ��һ���Ϸ���������Ӧ���ݰ����㷵��PASS
	//��������PKT_BUFF_S��û�кϷ�Э�����ݣ��㷵��FAIL
	int i, j, x, y, iTemp;
	unsigned char ucrData_Len[8];
	bool bFind_Esc_Mask, bGet_Error;
	unsigned long ulUseful_Data_Size;
	BYTE CRC;
	unsigned char * ucpTemp_Data = NULL;//��ʱ��������BUFF

	try
	{
		/*
		this->m_csError_Code.Format(_T("[ %d, %d ] 0x%X 0x%X"),
			strp_Read_Pkt->ul_Data_Length, strp_Read_Pkt->ul_Total_Size,
			*(strp_Read_Pkt->ucp_Buff), *(strp_Read_Pkt->ucp_Buff + 1));
		::AfxMessageBox(this->m_csError_Code);
		*/

		for(i=0; i<(int)(strp_Read_Pkt->ul_Data_Length); i++)
		{
			if(*(strp_Read_Pkt->ucp_Buff + i) == ASYNC_HDLC_AD_FLAG)//������ADͷ��Ȼ���ж����ADͷ�ǲ���������"ADͷ"
			{
				//���AD��һ����������ͷ���ݵ�AD
				//AD��FF֮����2~4λ����, AD xx xx xx xx FF
				//����������Ƿ���FF
				//���յ���λ��ȷ�ġ����ݳ��ȡ����ݺ󣬽�����һλ������FF������Ͳ�����

				//--------------------- ��ʼ������2λ�ϸ�ġ����ݳ��ȡ�λ --------------------- 
				x = 0;//�����ݳ��ȡ�λ��ͳ��
				memset(ucrData_Len, 0, sizeof(ucrData_Len));//����λBUFF
				bFind_Esc_Mask = false;//����ת���ʶ�����⴦����һ��ѭ��������
				bGet_Error = false;//�����з��ִ���

				for(j=1; j<=4; j++)//j��ƫ��index������λ��2λ��ת������4λ
				{
					if((i + j) < strp_Read_Pkt->ul_Data_Length)//����Ƿ񳬳���������BUFF����Χ
					{
						if(bFind_Esc_Mask == true)//��һλ��ת���ʶ�����Ҫ���⴦��ǰ���ݣ������������ַ�
						{
							switch(*(strp_Read_Pkt->ucp_Buff + i + j))
							{
							case 0xEE:
								ucrData_Len[x] = 0x11 & 0xFF;
								x++;
								break;
							case 0xEC:
								ucrData_Len[x] = 0x13 & 0xFF;
								x++;
								break;
							case 0xA3:
								ucrData_Len[x] = ASYNC_HDLC_ESC_MASK_R & 0xFF;//0x5c
								x++;
								break;
							default:
								bGet_Error = true;//���ִ����˳�forѭ��
								break;
								
							}

							bFind_Esc_Mask = false;//�ָ�״̬
						}
						else
						{
							if(*(strp_Read_Pkt->ucp_Buff + i + j) == ASYNC_HDLC_ESC_MASK_R)
							{
								//����ת���ַ�������������ݣ��ر�����һ������
								bFind_Esc_Mask = true;
							}
							else
							{
								//��������ֱ������
								ucrData_Len[x] = (*(strp_Read_Pkt->ucp_Buff + i + j)) & 0xFF;
								x++;
							}
						}

						//���ѭ�����Ƿ���������û��Ҫ��������
						if(bGet_Error == true)
						{
							break;//����for
						}

						//����Ƿ��Ѿ�����2λ�����ݳ��ȡ�
						if(x == 2)
						{
							break;//����for
						}
					}//����Ƿ񳬳���������BUFF����Χ
				}//for(j=1; j<=4;


				//--------------------- ��������λ���ݽ���������Ƿ��ճɹ� --------------------- 

				//���ѭ���г��������ݳ��ȡ�����2�����AD�϶����ǡ�ͷAD��
				if((bGet_Error == true) || (x != 2))
				{
					continue;//i�Ƶ���һ������
				}
				else
				{
					//2λ�����ݳ��ȡ����ͼ�顰���ݳ��ȡ�λ���ǲ���FF
					j++;//j������һλ��ָ��������FF��

					if((i + j) < strp_Read_Pkt->ul_Data_Length)//����Ƿ񳬳���������BUFF����Χ
					{
						if(*(strp_Read_Pkt->ucp_Buff + i + j) != ASYNC_HDLC_FF_FLAG)
						{
							//����FF, ���AD�϶����ǡ�ͷAD��
							continue;//i�Ƶ���һ������
						}
						else
						{
							//�ҵ�FF��ʶ��
							//���㡰���ݳ��ȡ�
							ulUseful_Data_Size = ucrData_Len[0];//�����ݳ��ȡ��ĸ�8λ
							ulUseful_Data_Size &= 0xFF;
							ulUseful_Data_Size = ulUseful_Data_Size << 8;
							ulUseful_Data_Size |= ucrData_Len[1] & 0xFF;//�����ݳ��ȡ��ĵ�8λ
							ulUseful_Data_Size &= 0xFFFF;

							/*
							this->m_csError_Code.Format(_T("���ݳ���:[ %X %X ] = 0x%X"),ucrData_Len[0], ucrData_Len[1], ulUseful_Data_Size);
							::AfxMessageBox(this->m_csError_Code);
							*/

							//����һλУ��λ
							ulUseful_Data_Size++;

							//��ȡ��FF��ʼ������
							//�����ڴ棬����ȡ���ݡ���BUFF
							this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpTemp_Data, strp_Read_Pkt->ul_Data_Length);

							//
							bFind_Esc_Mask = false;//����ת���ʶ�����⴦����һ��ѭ��������
							bGet_Error = false;//�����з��ִ���

							x = i + j;//x�ǡ�FF����index
							j = 0;//j�Ƿ�ת����ռ����ݵ�ͳ��

							for(; x<strp_Read_Pkt->ul_Data_Length; x++)
							{
								if(bFind_Esc_Mask == true)//��һλ��ת���ʶ�����Ҫ���⴦��ǰ���ݣ������������ַ�
								{
									switch(*(strp_Read_Pkt->ucp_Buff + x))
									{
									case 0xEE:
										*(ucpTemp_Data + j) = 0x11 & 0xFF;
										j++;
										break;
									case 0xEC:
										*(ucpTemp_Data + j) = 0x13 & 0xFF;
										j++;
										break;
									case 0xA3:
										*(ucpTemp_Data + j) = ASYNC_HDLC_ESC_MASK_R & 0xFF;//0x5c
										j++;
										break;
									default:
										bGet_Error = true;//���ִ����˳�forѭ��
										break;

									}

									bFind_Esc_Mask = false;//�ָ�״̬
								}
								else
								{
									if(*(strp_Read_Pkt->ucp_Buff + x) == ASYNC_HDLC_ESC_MASK_R)
									{
										//����ת���ַ�������������ݣ��ر�����һ������
										bFind_Esc_Mask = true;
									}
									else
									{
										//��������ֱ������
										*(ucpTemp_Data + j) = (*(strp_Read_Pkt->ucp_Buff + x)) & 0xFF;
										j++;
									}
								}

								//���ѭ�����Ƿ���������û��Ҫ��������
								if(bGet_Error == true)
								{
									break;//����for
								}

								//����Ƿ��Ѿ����㹻
								if(j == ulUseful_Data_Size)
								{
									break;//����for
								}
							}//for(; x<strp_Read_Pkt->ul_Data_Length;

							//�˴�ѭ��������x�������ʵ���RX_BUFF���ݵ�index��

							//���ѭ���г����ռ���������
							if((bGet_Error == true) || (j != ulUseful_Data_Size))
							{
								continue;//i�Ƶ���һ������
							}
							else
							{
								//�ռ���������ȷ��������Ǽ�����λ������Щ�����ǲ�����ȷ�����ݰ�
								//����У��λ
								CRC = *(ucpTemp_Data + 0);
								iTemp = (ulUseful_Data_Size - 2);//���һλ�Ƿ������ݵġ�У��ֵ�������Լ���У��λ�������ڶ�λ
								for(y=1; y<=iTemp; y++)
								{
									CRC = CRC ^ (*(ucpTemp_Data + y));
								}


								/*
								this->m_csError_Code.Format(_T("CRC = 0x%X, [ %X %X .... %X %X ]"),
									CRC, *(ucpTemp_Data + 0), *(ucpTemp_Data + 1),
									*(ucpTemp_Data + ulUseful_Data_Size - 2), *(ucpTemp_Data + ulUseful_Data_Size - 1));
								::AfxMessageBox(this->m_csError_Code);
								*/


								//���У��λ
								if(CRC == *(ucpTemp_Data + ulUseful_Data_Size - 1))
								{
									//У����ȷ, ������ȷֵ

									//����rd_id
									*ucp_Rd_ID = *(ucpTemp_Data + 1);

									//������������
									*ulp_Read_Quan = 0;

									ulUseful_Data_Size--;//ȥ��У��λ

									for(y=2; y<ulUseful_Data_Size; y++)
									{
										*(ucp_Read_Data + *ulp_Read_Quan) = *(ucpTemp_Data + y);
										
										*ulp_Read_Quan += 1;
									}


									/*
									this->m_csError_Code.Format(_T("rd_id = 0x%X, quan = %d, [ %X ... %X ]"),
										*ucp_Rd_ID, *ulp_Read_Quan, *(ucp_Read_Data + 0), *(ucp_Read_Data + *ulp_Read_Quan - 1));
									::AfxMessageBox(this->m_csError_Code);
									*/


									//ȥ���Ѿ��ɹ�����������
									this->m_cla_GFC.PKT_BUFF_DEL_DATA(strp_Read_Pkt, (x + 1));//i�ǡ����ܵ����ݡ��������������������ݺ�β���ؼ���


									/*
									this->m_csError_Code.Format(_T("ȥ���Ѿ��ɹ�����������, data len = %d, del = %d;"),
										strp_Read_Pkt->ul_Data_Length, (x + 1));
									::AfxMessageBox(this->m_csError_Code);
									*/


									//
									this->m_cla_GFC.BUFF_BYTE_FREE(&ucpTemp_Data);
									return true;
								}    
							}
						}//�ҵ�0xFF��
					}
				}//�ҵ���λ�����ݳ��ȡ�
			}//if(*(strp_Read_Pkt->ucp_Buff + i) == ASYNC_HDLC_AD_FLAG
		}//for(i=0; i<strp_Read_Pkt->ul_Data_Length;
	}
	catch(...)
	{
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpTemp_Data);
		return false;
	}

	//û�н����Э���ʶ����
	this->m_csError_Code.Format(_T("FAIL, Get data from read pkt, no flag!"));
	this->m_cla_GFC.BUFF_BYTE_FREE(&ucpTemp_Data);
	return false;
}

bool CARM_OP_RDA::Probe_Port(void)
{
	DWORD dwProbe_Addr = 0x01A24004;
	DWORD dwRead_Data = 0x0;

	for(int i=0; i<5; i++)
	{
		if(this->Read_DWORD(dwProbe_Addr, &dwRead_Data, 2) == true)
		{
			if((dwRead_Data == 0x09120711) || (dwRead_Data == 0xffffffff) || (dwRead_Data == 0x10090911))
			{
				return true;
			}
			else
			{
				this->m_csError_Code.Format(_T("FAIL, Probe port read data 0x%X;"), dwRead_Data);
			}
		}
	}

	return false;
}

bool CARM_OP_RDA::Write_REG(DWORD dw_Write_Addr, unsigned char uc_Reg_Data)
{
	//д�Ĵ�����1λBYTE
	if(this->PKT_CMD_LOAD_BYTE(&uc_Reg_Data, 1, dw_Write_Addr, CMD_WR_REG, &this->m_str_Pkt_Send) == false)
	{
		return false;
	}

	//����ָ��
	if(this->WRITE_PKT(this->m_str_Pkt_Send) == false)
	{
		this->m_csError_Code.Format(_T("Write_REG, WRITE_PKT, Fail!"));
		return false;
	}

	return true;
}

bool CARM_OP_RDA::Read_REG(DWORD dw_Read_Addr, unsigned char * ucp_Read_Reg_Data, int i_TimeOut)
{
	//���յ�ַ�����ж�ȡ1��BYTE����
	unsigned char ucRd_ID_Send = 0x03;//����д�ġ�������ʶ��ID��
	unsigned long ulRd_Cnt = 1;//��ظ�Ӧ����1λBYTE

	unsigned char ucRd_ID_Read;//���صġ�������ʶ��ID��
	unsigned char * ucpRd_Data = NULL;//��ȡ���������ݡ���BUFF
	unsigned long ulRd_Data_Quan;//���������ݡ�������

	bool bRtn = false;
	
	try
	{
		//��������Ĵ���reg������ָ��
		if(this->PKT_CMD_LOAD_BYTE(&ucRd_ID_Send, 1, dw_Read_Addr, CMD_RD_REG, &this->m_str_Pkt_Send) == false)
		{
			throw(0);
		}

		//--------------------------- ���͡����Ĵ���reg������ָ�����ȡ��Ӧ --------------------------- 
		if(this->QUERY_PKT(this->m_str_Pkt_Send, &this->m_str_Com_Rx_Buff, i_TimeOut) == false)
		{
			throw(0);
		}

		//--------------------------- ȷʵ�ж�����Ӧ���ݣ�����Է����ڴ棬׼���������� --------------------------- 
		//�����ڴ棬���������ݡ���BUFF�����С��m_str_Com_Rx_Buff�Ĵ�Сһ��
		this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpRd_Data, this->m_str_Com_Rx_Buff.ul_Data_Length);
		
		//--------------------------- �����յ������� --------------------------- 
		//ѭ������m_str_Com_Rx_Buff�е����ݣ�ֱ�������Ѿ�û���ˡ��Ϸ���Э�����ݰ�������˳�ѭ��
		while(this->GET_DATA_FROM_READ_PKT(&this->m_str_Com_Rx_Buff, &ucRd_ID_Read, ucpRd_Data, &ulRd_Data_Quan) == true)
		{
			//��ȡ��һ���Ϸ����ݣ�����ǲ���������ζ������ġ��Ϸ�Э�����ݰ�������Ӧ
			if((ucRd_ID_Read == ucRd_ID_Send) && (ulRd_Data_Quan == ulRd_Cnt))
			{
				//rd_id��ȷ������������Ҳ��ȷ�����Ƕ�����������ζ���������Ӧ
				//����1��BYTE
				*ucp_Read_Reg_Data = (*(ucpRd_Data + 0)) & 0xFF;
				
				bRtn = true;//PASS
				break;//�˳�whileѭ��
			}
		}
	}
	catch(...)
	{
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRd_Data);
		return false;
	}

	this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRd_Data);
	return bRtn;
}

bool CARM_OP_RDA::Lock_XCPU(void)
{
	DWORD dwReg_Addr;
	unsigned char ucReg_Data;

	try
	{
		//��CTRL_CFG_REG��д��0x80
		dwReg_Addr = CTRL_CFG_REG;
		ucReg_Data = 0x80;
		if(this->Write_REG(dwReg_Addr, ucReg_Data) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Lock XCPU, Write reg, 0x%X, 0x%X!"), dwReg_Addr, ucReg_Data);
			throw(0);
		}

		//��CTRL_SET_REG������
		dwReg_Addr = CTRL_SET_REG;
		if(this->Read_REG(dwReg_Addr, &ucReg_Data, 3) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Lock XCPU, Read reg, 0x%X!"), dwReg_Addr);
			throw(0);
		}

		//��CTRL_SET_REG��д��0x8
		dwReg_Addr = CTRL_SET_REG;
		ucReg_Data = 0x8;
		if(this->Write_REG(dwReg_Addr, ucReg_Data) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Lock XCPU, Write reg, 0x%X, 0x%X!"), dwReg_Addr, ucReg_Data);
			throw(0);
		}

		//��CTRL_CFG_REG��д��0x80
		dwReg_Addr = CTRL_CFG_REG;
		ucReg_Data = 0x80;
		if(this->Write_REG(dwReg_Addr, ucReg_Data) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Lock XCPU, Write reg, 0x%X, 0x%X!"), dwReg_Addr, ucReg_Data);
			throw(0);
		}

		Sleep(2000);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CARM_OP_RDA::Read_Spi_Flash(int * ip_Flash_Idx)
{
	//��FLASH ID�����ж�FLASH����
	DWORD dwFlash_ID, dwRead_Data;

	try
	{
		//��
		if(this->Read_DWORD(FLASH_ID_REG, &dwRead_Data, 2) == false)
		{
			throw(0);
		}

		//�������DWORD����
		dwFlash_ID = dwRead_Data & 0x00017FFF;

		switch(dwFlash_ID)
		{
		case 0x00002000:
			*ip_Flash_Idx = F_SPI_16M;
			break;
		default:
			*ip_Flash_Idx = F_SPI_16M;
			break;
			/*
			this->m_csError_Code.Format(_T("FAIL, Read spi flash, NDF, id = 0x%08X, data = 0x%08X"), dwFlash_ID, dwRead_Data);
			throw(0);
			*/
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CARM_OP_RDA::Get_Spi_Flash_Name(const int i_Flash_Idx, CString * csp_Flash_Name)
{
	switch(i_Flash_Idx)
	{
	case F_SPI_16M:
		csp_Flash_Name->Format(_T("Flash_SPI_16M"));
		break;
	default:
		this->m_csError_Code.Format(_T("FAIL, Get spi flash name, NDF, idx = %d"), i_Flash_Idx);
		return false;
	}

	return true;
}

bool CARM_OP_RDA::Write_DWORD(DWORD dw_Write_Addr, DWORD dw_Write_Data)
{
	//����һ��DWORD������
	if(this->PKT_CMD_LOAD_DWORD(&dw_Write_Data, 1, dw_Write_Addr, CMD_WR_DWORD, &this->m_str_Pkt_Send) == false)
	{
		return false;
	}

	//����ָ��
	if(this->WRITE_PKT(this->m_str_Pkt_Send) == false)
	{
		this->m_csError_Code.Format(_T("Write_DWORD, WRITE_PKT, Fail!"));
		return false;
	}

	return true;
}

bool CARM_OP_RDA::Enter_Host_Mode(void)
{
	DWORD dwAddr, dwData, dwBoot_Mode;
	unsigned char ucReg_Data, ucRd_ID_Read;
	unsigned char * ucpRd_Data = NULL;
	unsigned long ulRd_Data_Quan;
	bool bTemp;

	try
	{
		//
		dwAddr = 0xA1A25000;
		dwData = 0x66;
		if(this->Write_DWORD(dwAddr, dwData) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Enter host mode, Write dword, 0x%08X, 0x%08X!"), dwAddr, dwData);
			throw(0);
		}

		//
		dwAddr = 0xA1A25000;
		dwData = 0x99;
		if(this->Write_DWORD(dwAddr, dwData) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Enter host mode, Write dword, 0x%08X, 0x%08X!"), dwAddr, dwData);
			throw(0);
		}

		//��boot mode�����ء�AD 00 06 FF 03 00 00 60 00 9C��
		dwAddr = 0x01A000A0;
		if(this->Read_DWORD(dwAddr, &dwBoot_Mode, 2) == false)
		{
			throw(0);
		}

		//д������boot mode
		//AD 00 0A FF 82 A0 00 A0 01 00 00 2A 00 56
		dwAddr = 0x01A000A0;
		dwData = dwBoot_Mode | 0x0A0000;//LOG��boot mode�õ���0x00200000������д�����0x002A0000
		if(this->Write_DWORD(dwAddr, dwData) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Enter host mode, Write dword, 0x%08X, 0x%08X!"), dwAddr, dwData);
			throw(0);
		}

		Sleep(1000);

		//AD 00 07 FF 84 00 00 00 00 05 7E 
		dwAddr = 0x00;
		ucReg_Data = 0x05;
		if(this->Write_REG(dwAddr, ucReg_Data) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Enter host mode, Write reg, 0x%08X, 0x%02X!"), dwAddr, ucReg_Data);
			throw(0);
		}

		Sleep(1000);

		//AD 00 07 FF 84 03 00 00 00 80 F8 
		dwAddr = CTRL_CFG_REG;
		ucReg_Data = 0x80;
		if(this->Write_REG(dwAddr, ucReg_Data) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Enter host mode, Write reg, 0x%08X, 0x%02X!"), dwAddr, ucReg_Data);
			throw(0);
		}

		//AD 00 07 FF 84 01 00 00 00 02 78 
		dwAddr = 0x01;
		ucReg_Data = 0x02;
		if(this->Write_REG(dwAddr, ucReg_Data) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Enter host mode, Write reg, 0x%08X, 0x%02X!"), dwAddr, ucReg_Data);
			throw(0);
		}

		//AD 00 07 FF 84 05 00 00 00 FD 83
		dwAddr = 0x05;
		ucReg_Data = 0xFD;
		if(this->Write_REG(dwAddr, ucReg_Data) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Enter host mode, Write reg, 0x%08X, 0x%02X!"), dwAddr, ucReg_Data);
			throw(0);
		}


		//�ȴ�����0xff000001 (AD 00 06 FF 00 01 00 00 FF 01)
		if(this->READ_RSP_PKT(&this->m_str_Com_Rx_Buff, 5) == true)
		{
			//�����ڴ�
			this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpRd_Data, this->m_str_Com_Rx_Buff.ul_Data_Length);

			bTemp = false;

			//ѭ������m_str_Com_Rx_Buff�е����ݣ�ֱ�������Ѿ�û���ˡ��Ϸ���Э�����ݰ�������˳�ѭ��
			while(this->GET_DATA_FROM_READ_PKT(&this->m_str_Com_Rx_Buff, &ucRd_ID_Read, ucpRd_Data, &ulRd_Data_Quan) == true)
			{
				//��ȡ��һ���Ϸ�����
				//�ж��ǲ���������ζ��������������Ϸ�Э�����ݰ�������Ӧ
				//����������ݣ�1��rd_id��2��byte������
				if((ucRd_ID_Read == 0x00) && (ulRd_Data_Quan == 4))
				{
					dwData = 0x0;
					dwData = dwData | (*(ucpRd_Data + 0)) | ((*(ucpRd_Data + 1)) << 8) | ((*(ucpRd_Data + 2)) << 16) | ((*(ucpRd_Data + 3)) << 24);
				
					if(dwData == 0xFF000001)
					{
						bTemp = true;// �ȵ���Ŀ�����ݻظ���PASS
						break;
					}
				}
			}

			//�ͷ��ڴ�
			this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRd_Data);

			if(bTemp == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Enter host mode, no right data!"));
				throw(0);//�лظ����ݣ���û��Ŀ������
			}
		}
		else
		{
			this->m_csError_Code.Format(_T("FAIL, Enter host mode, no data!"));
			throw(0);//�޻ظ�����
		}

		//дboot mode
		//AD 00 0A FF 82 A0 00 A0 01 00 00 20 00 5C A3
		dwAddr = 0x01A000A0;
		dwData = dwBoot_Mode;//LOG��boot mode�õ���0x00200000;
		if(this->Write_DWORD(dwAddr, dwData) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Enter host mode, Write dword, 0x%08X, 0x%08X!"), dwAddr, dwData);
			throw(0);
		}

	}
	catch(...)
	{
		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRd_Data);

		return false;
	}

	return true;
}

bool CARM_OP_RDA::READ_RSP_PKT(PKT_BUFF_S * strp_Read_Pkt, int i_TimeOut)
{
	time_t t_Start, t_End;
	unsigned long ulLast_Read = 0;

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

			//��һ�ζ����ڵĲ����������ݷŵ�Rx_Buff��
			this->READ_PKT(strp_Read_Pkt);

			//����������
			if(strp_Read_Pkt->ul_Data_Length > 0)//����������
			{
				if(strp_Read_Pkt->ul_Data_Length == ulLast_Read)//��״̬������û�����ӣ��Ϳ��Է�����
				{
					//pass
					return true;
				}
				else//Ϊ��ֹ�����������ݣ���ֹ���ݶ��ѣ�������һ������������ȷ���Ƿ������
				{
					ulLast_Read = strp_Read_Pkt->ul_Data_Length;
				}
			}
			
			//��ʱ���
			time(&t_End);
			if(difftime(t_End, t_Start) >= i_TimeOut)
			{
				this->m_csError_Code.Format(_T("FAIL, Read rsp pkt, timeout, no data!"));
				throw(0);
			}

			//�´ζ�ȡ�ļ��ʱ��
			Sleep(100);
		}
		while(true);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CARM_OP_RDA::Config_Ebc_Ram(void)
{
	DWORD dwAddr, dwData, dwBootSectorStructPtr, dwRamTiming, dwRamMode;
	int iSwBootMode;

	try
	{
		//��AD 00 07 FF 02 A0 00 A0 01 03 FF�����ء�AD 00 06 FF 03 00 00 60 00 DC��
		dwAddr = 0x01A000A0;
		if(this->Read_DWORD(dwAddr, &dwData, 2) == false)
		{
			throw(0);
		}

		//
		iSwBootMode = (dwData & 0xfffffff) >> 22;

		if(iSwBootMode & (1 << 0) == 0x00)
		{
			this->m_csError_Code.Format(_T("FAIL, Config ebc ram, SwBootMode = 0x%X"), iSwBootMode);
			throw(0);
		}

		//��AD 00 07 FF 02 78 02 C0 81 03 C5�����ء�AD 00 06 FF 03 20 02 C0 81 9F��
		dwAddr = 0x81C00278;
		if(this->Read_DWORD(dwAddr, &dwBootSectorStructPtr, 2) == false)
		{
			throw(0);
		}

		//����ֵ��AD 00 06 FF 03 20 02 C0 81 9F (0x81C00220)
		if(((dwBootSectorStructPtr < 0x81c00000) || (dwBootSectorStructPtr >= 0x81c10000)) &&
			((dwBootSectorStructPtr < 0xa1c00000) || (dwBootSectorStructPtr >= 0xa1c10000)))
		{
			return true;
		}

		//�� +4��ebcTag����AD 00 07 FF 02 24 02 C0 81 03 99������AD 00 06 FF 03 C7 5E 07 B0 D2
		dwAddr = dwBootSectorStructPtr + 4;
		if(this->Read_DWORD(dwAddr, &dwData, 2) == false)
		{
			throw(0);
		}
		if(dwData != 0xB0075EC7)  //BOOT_SECTOR_EBC_VALID_TAG
		{
			return true;
		}

		//�� +8�� RamTiming, ��AD 00 07 FF 02 28 02 C0 81 03 95������AD 00 06 FF 03 0D 04 0F 10 EA
		dwAddr = dwBootSectorStructPtr + 8;
		if(this->Read_DWORD(dwAddr, &dwRamTiming, 2) == false)
		{
			throw(0);
		}

		//�� +12, RamMode, ��AD 00 07 FF 02 2C 02 C0 81 03 91������AD 00 06 FF 03 15 20 30 00 F9
		dwAddr = dwBootSectorStructPtr + 12;
		if(this->Read_DWORD(dwAddr, &dwRamMode, 2) == false)
		{
			throw(0);
		}

		//д RamTiming, ��AD 00 0A FF 82 0C 44 A0 01 0D 04 0F 10 82
		dwAddr = 0x01A0440C;
		dwData = dwRamTiming;
		if(this->Write_DWORD(dwAddr, dwData) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Config ebc ram, Write dword, 0x%08X, 0x%08X!"), dwAddr, dwData);
			throw(0);
		}

		//д RamMode, ��AD 00 0A FF 82 18 44 A0 01 15 20 30 00 85
		dwAddr = 0x01A04418;
		dwData = dwRamMode;
		if(this->Write_DWORD(dwAddr, dwData) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Config ebc ram, Write dword, 0x%08X, 0x%08X!"), dwAddr, dwData);
			throw(0);
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CARM_OP_RDA::Read_Chip_ID(DWORD * dwp_Chip_ID)
{
	DWORD dwAddr, dwData;

	try
	{
		dwAddr = CHIP_ID_REG;
		if(this->Read_DWORD(dwAddr, &dwData, 3) == false)//AD 00 07 FF 02 00 40 A2 01 03 1D
		{
			this->m_csError_Code.Format(_T("FAIL, Read chip id, Read dword, 0x%08X"), dwAddr);
			throw(0);
		}

		//����AD 00 06 FF 03 07 40 09 88 3A ( 88 09 40 07)
		*dwp_Chip_ID = (dwData >> 16) & 0x0000FFFF;
	}
	catch(...)
	{
		return false;
	}

	return true;
}
//8955 ƽ̨
bool CARM_OP_RDA::Load_Com_Conn(void)
{
	if (1!=m_chipe_flag)//�ж��Ƿ���8955оƬ
	{
		this->Show_Out(_T("Model Chipe not 8955,8909"));
		return false;
	}
	char * cpData = NULL;
	int iTemp=0;	
	CString str_portName;
	CString str_portName1;
	str_portName.Format(_T("COM%d"),m_iCom_Port);
	try
	{
		//�����ڴ�
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpData, 512);

		//��Ҫ�����ļ�·��ת��Ϊ��char
		this->m_cla_GFC.WideChar_To_MultiByte(str_portName, cpData, &iTemp);

		//------------------------------ ��鴮���Ƿ���� ------------------------------ 
		if(this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iCom_Port) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Confirm com port %d fail!"), this->m_iCom_Port);
			this->Show_Status(this->m_csError_Code);
			this->Show_Out(m_csError_Code);
			throw(0);
		}

		if (m_conn!=NULL)
		{
			delete(m_conn);
			m_conn = NULL;
		}	
		m_conn= CreateTargetConnection(cpData);//��RDA�޸ĵײ�˿�"COM155"
		if ( NULL == m_conn)
		{
			this->m_csError_Code.Format(_T("TargetConnection(%s) Fail"),str_portName);
			this->Show_Status(this->m_csError_Code);
			this->Show_Out(this->m_csError_Code);

			throw(1);
		}
		else
		{
			str_portName1.Format(_T("TargetConnection(%s) Pass"),str_portName);
			this->Show_Out(str_portName1);

		}
	}
	catch(...)
	{
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);
		return  false;
	}

	this->Show_Status(_T("TargetConnection Pass"));
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);
	return  true;

}
bool CARM_OP_RDA::MergeLod(const CString lodName1,const CString lodName2,const CString DestlodName)
{

	bool ret =false;
	char * cpData1 = NULL;
	char * cpData2 = NULL;

	char * cpDestcpData = NULL;

	LodData m_Lod_Ext;

	int iTemp=0;

	CString cstrlodFname1= lodName1;
	CString cstrlodFname2= lodName2;

	CString cstrDestlodFname= DestlodName;

	try
	{
		//�����ڴ�
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpData1, 512);
		//��Ҫ�����ļ�·��ת��Ϊ��char
		this->m_cla_GFC.WideChar_To_MultiByte(cstrlodFname1, cpData1, &iTemp);

		//�����ڴ�
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpData2, 512);
		//��Ҫ�����ļ�·��ת��Ϊ��char
		this->m_cla_GFC.WideChar_To_MultiByte(cstrlodFname2, cpData2, &iTemp);

		//�����ڴ�
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpDestcpData, 512);
		//��Ҫ�����ļ�·��ת��Ϊ��char
		this->m_cla_GFC.WideChar_To_MultiByte(cstrDestlodFname, cpDestcpData, &iTemp);

		

		ret = m_Lod.load(cpData1);
		if (false == ret)
		{
			this->m_csError_Code.Format(_T("MergeLod LodFile(%s) Fail"),cstrlodFname1);
			this->Show_Status(this->m_csError_Code);
			throw(1);
		}

		ret = m_Lod_Ext.load(cpData2);
		if (false == ret)
		{
			this->m_csError_Code.Format(_T("MergeLod LodFile(%s) Fail"),cstrlodFname2);
			this->Show_Status(this->m_csError_Code);
			throw(1);
		}

// 		foreach(LodPacket* p, m_Lod_Ext.packets())
// 		{
// 			m_Lod.addPacket(new LodPacket(*p));
// 		}
		std::list<LodPacket*> packetlist = m_Lod_Ext.packets();
		std::list<LodPacket*>::iterator  iterator;

		for (iterator = packetlist.begin(); iterator != packetlist.end();iterator ++)
		{
			m_Lod.addPacket( (*iterator) );
		}


		if (!m_Lod.store(cpDestcpData))
		{
			//
			this->m_csError_Code.Format(_T("store LodFile(%s) Fail"),cstrDestlodFname);
			this->Show_Status(this->m_csError_Code);
			throw(1);
		}
	}
	catch (...)
	{
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpData1);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpData2);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpDestcpData);
		return false;
	}

	this->Show_Status(_T("MergeLod Pass"));
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpData1);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpData2);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpDestcpData);
	return true;
}




bool CARM_OP_RDA::LOAD_LOD_FILE_Ext(LOD_FILE_S * strp_Lod_File)
{

	bool ret =false;
	char * cpData = NULL;
	int iTemp=0;
	//_T("F:\\MC25MAR01A01_BETA0914A.lod");
	CString cstrlodFname= strp_Lod_File->cs_Path;



	m_chipe_flag =1;//MC25��־

	try
	{
		//�����ڴ�
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpData, 512);
		
		//��Ҫ�����ļ�·��ת��Ϊ��char
		this->m_cla_GFC.WideChar_To_MultiByte(cstrlodFname, cpData, &iTemp);

		ret = m_Lod.load(cpData);

		if (false == ret)
		{
			this->m_csError_Code.Format(_T("load LodFile(%s) Fail"),cstrlodFname);
			this->Show_Status(this->m_csError_Code);
			throw(1);
		}
		m_Lodheader = m_Lod.getHeader();

	

		FlashInfo flashInfo;
		if (0 != flashInfo.loadFromLodHeader(*m_Lodheader))
		{
			this->m_csError_Code.Format(_T("LodFile(%s) getHeader Fail"),cstrlodFname);
			this->Show_Status(this->m_csError_Code);
			throw(2);
		}

		LodData  programLod;

		m_bRDA_download = FALSE;

#ifdef FW_MULTI
#ifndef FW_UPGRADE
		if (PRODU_TYPE_MOB != this->m_iProdu_Type)
		{
			m_bRDA_download = TRUE;
		}
#endif
#endif
//-----------------
		if (TRUE == m_bRDA_download) //������������У׼����һ���оƬ������
		{
			m_Lod.mergeForDownload(flashInfo.flashSize);
		}
		else
		{
			m_Lod.mergeForFlash();//����Ƿ�ֹlod�еĵ�ַ������С����˳��
		}

#if 0
		std::vector<int> vTemp;
		vTemp.push_back(1);
		vTemp.push_back(2);
		vTemp.push_back(3);

		std::vector<int>::iterator iter1;
		iter1 = vTemp.begin();

		CString csTemppp;
		csTemppp.Format(_T("%d"),*iter1);



		if (TRUE == m_bRDA_download) //������������У׼����һ���оƬ������
		{		
			LodPacket* p = new LodPacket(0x08000000);

			int flashSize = flashInfo.flashSize / sizeof(uint32_t);
			p->data.resize(flashSize);
			//uint8_t* pdata = (uint8_t*) (&(p->data.begin()));
			//memset(pdata, 0xFF, flashInfo.flashSize/ sizeof(uint32_t));
			uint8_t* pdata = NULL;
			std::vector<uint32_t>::iterator iter;

			p->data[0]= 0xffffffff;
			iter = p->data.begin();
			csTemppp.Format(_T("%d"),*iter);

			pdata =  (uint8_t*)(&(*iter));

			int size =  p->data.size();
			memset(pdata, 0xFF, size*4);//flashInfo.flashSize);

			//memset((uint8_t*) (&(p->data.begin())), 0xFF, flashInfo.flashSize);

			int i =1;
			i=2;

			foreach (LodPacket* packet, m_Lod.packets()) 
			{
				uint32_t base = packet->base & 0xffffff;
				if (base + packet->size() > flashInfo.flashSize)
				{
					return false;
				}
					
				if (packet->data.size() == 0)
				{
					continue;
				}

				memcpy(pdata + base, &packet->data.begin(), packet->size());
			}
			programLod.addPacket(p);
		
		}
		else 
		{
			std::list<LodPacket*> packets = m_Lod.packets();
			std::list<LodPacket *>::iterator pos = packets .begin();

			for(pos = packets .begin();pos != packets.end(); ++pos)
			{
				LodPacket* packet= new LodPacket(**pos);
				programLod.addPacket(packet);
			}
		}
		programLod.mergeForFlash();
#endif
		
	}
	catch (...)
	{
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);
		return false;
	}

	this->Show_Status(_T("LodFile Pass"));
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);

	//AfxMessageBox(_T("3"));

	return true;
}

//20191227
bool CARM_OP_RDA::LOAD_LOD_FILE_Ext_Sub(LOD_FILE_S * strp_Lod_File)
{

	bool ret =false;
	char * cpData = NULL;
	int iTemp=0;

	LodHeader* p_Lodheader = NULL;


	//_T("D:\4_Firmware\M56RMAR01A02_MFV03_TEMP\M56RMAR01A02_MFV03_TEMP\APPGS5MDM32A01.lod");
	//CString cstrlodFname= strp_Lod_File->cs_Path;

	int i=0,j=0;

    m_Lod_Num =0;

	CString cstrlodFname,strData1;
// 	//����lod�ļ�
// 	this->Find_lod_File(strp_Lod_File->cs_Path);

	m_chipe_flag =1;//MC25��־

	int iFS_ROOT_BASE = m_iFS_ROOT_BASE;
	int iFS_ROOT_SIZE = m_iFS_ROOT_SIZE;

	//------------------------------------------------------------------------
	try
	{
		//�����ڴ�
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpData, 512);

		cstrlodFname.Format(_T("%s"),this->m_strr_Lod_File_List[0].cs_Path);

		this->Show_Out(cstrlodFname);


		//��Ҫ�����ļ�·��ת��Ϊ��char
		this->m_cla_GFC.WideChar_To_MultiByte(cstrlodFname, cpData, &iTemp);

		ret = m_Lod_Sub[0].load(cpData);
		if (false == ret)
		{
			this->m_csError_Code.Format(_T("load LodFile(%s) Fail"),cstrlodFname);
			this->Show_Status(this->m_csError_Code);
			throw(1);
		}
		p_Lodheader = m_Lod_Sub[0].getHeader();
		FlashInfo flashInfo;
		if (0 != flashInfo.loadFromLodHeader(*p_Lodheader))
		{
			this->m_csError_Code.Format(_T("LodFile(%s) getHeader Fail"),cstrlodFname);
			this->Show_Status(this->m_csError_Code);
			throw(2);
		}

		m_bRDA_download = FALSE;

#ifdef FW_MULTI
#ifndef FW_UPGRADE
		m_bRDA_download = TRUE;
#endif
#endif
		//-----------------
		if (TRUE == m_bRDA_download) //������������У׼����һ���оƬ������
		{
			m_Lod_Sub[0].mergeForDownload(flashInfo.flashSize);
		}
		else
		{
			m_Lod_Sub[0].mergeForFlash();//����Ƿ�ֹlod�еĵ�ַ������С����˳��
		}

		i++;

	}
	catch (...)
	{
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);

		this->Show_Status(_T("Main Lod File Fail"));

		this->Show_Out(_T("Main Lod File Fail"));

		return false;
	}	
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);
	this->Show_Out(_T("Main Lod File Pass"));


	//------------------------------------------------------------------------
	for (j=0;j<MAX_LOD_FILE;j++)
	{
		if (false ==  this->m_strr_Lod_File_List[j].b_Enable_DL)
		{
			continue;
		}
		cstrlodFname.Format(_T("%s"),this->m_strr_Lod_File_List[j].cs_Path);

		this->Show_Out(cstrlodFname);


		//20190227
		//#$FS_ROOT_BASE=0x390000
		//#$FS_ROOT_SIZE=0x66000

		int iTempFS_ROOT_BASE =0;
		if(true ==  this->Get_Lod_File_FS_ROOT_BASE_Params(cstrlodFname,&iTempFS_ROOT_BASE))
		{
			if ((iFS_ROOT_BASE != m_iFS_ROOT_BASE) && (iFS_ROOT_BASE !=0) &&(m_iFS_ROOT_BASE!=0))
			{
				//����
				strData1.Format(_T("FS_ROOT_BASE is Invalid-%d,%d"),m_iFS_ROOT_BASE,iFS_ROOT_BASE);
				this->Show_Out(strData1);
				this->Show_Status(strData1);
			}
			else if (0!= m_iFS_ROOT_BASE)
			{
				iFS_ROOT_BASE = m_iFS_ROOT_BASE;
			} 
			else
			{
				//���账��
			}

		}
		if(true ==  this->Get_Lod_File_FS_ROOT_SIZE_Params(cstrlodFname))
		{
			if ((iFS_ROOT_SIZE != m_iFS_ROOT_SIZE) && (iFS_ROOT_SIZE !=0) &&(m_iFS_ROOT_SIZE !=0))
			{
				//����
				strData1.Format(_T("FS_ROOT_SIZE is Invalid-%d,%d"),m_iFS_ROOT_SIZE,iFS_ROOT_SIZE);
				this->Show_Out(strData1);
				this->Show_Status(strData1);

			}
			else if (0!= m_iFS_ROOT_SIZE)
			{
				iFS_ROOT_SIZE = m_iFS_ROOT_SIZE;
			} 
			else
			{
				//���账��
			}
		}

	}

	//------------------------------------------------------------------------
	for (j=1;j<MAX_LOD_FILE;j++)
	{
		 if (false ==  this->m_strr_Lod_File_List[j].b_Enable_DL)
		 {
			continue;
		 }
		 try
		 {
			 if (i >= MAX_LOD_FILE)
			 {
				 this->m_csError_Code.Format(_T("load LodFile(%s) Fail-1"),cstrlodFname);
				 this->Show_Status(this->m_csError_Code);
				 throw(1);
			 }
			 //�����ڴ�
			 this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpData, 512);

			 cstrlodFname.Format(_T("%s"),this->m_strr_Lod_File_List[j].cs_Path);

			 this->Show_Out(cstrlodFname);

			 //��Ҫ�����ļ�·��ת��Ϊ��char
			 this->m_cla_GFC.WideChar_To_MultiByte(cstrlodFname, cpData, &iTemp);

			 ret = m_Lod_Sub[i].load(cpData);
			 if (false == ret)
			 {
				 this->m_csError_Code.Format(_T("load LodFile(%s) Fail"),cstrlodFname);
				 this->Show_Status(this->m_csError_Code);
				 throw(1);
			 }
			 p_Lodheader = m_Lod_Sub[i].getHeader();
			 FlashInfo flashInfo;
			 if (0 != flashInfo.loadFromLodHeader(*p_Lodheader))
			 {
				 this->m_csError_Code.Format(_T("LodFile(%s) getHeader Fail"),cstrlodFname);
				 this->Show_Status(this->m_csError_Code);
				 throw(2);
			 }

			 m_Lod_Sub[i].mergeForFlash();//����Ƿ�ֹlod�еĵ�ַ������С����˳��

			 i++;

			 this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);

		 }
		 catch (...)
		 {
			 this->m_cla_GFC.BUFF_CHAR_FREE(&cpData);
			 return false;
		 }		 

	}

	m_iFS_ROOT_BASE = iFS_ROOT_BASE;
	m_iFS_ROOT_SIZE = iFS_ROOT_SIZE;

	m_Lod_Num =i;


	this->Show_Status(_T("LodFile Pass"));
	
	return true;
}

void CARM_OP_RDA::Find_lod_File(const wchar_t * wcp_FW_File_Path)
{
	//����FW�ļ����У����û�����½�FW�ļ���
	CString cs_FW_Dir, cs_Finder_Dir, cs_FW_Name,strFW_Name=_T("");
	CFileFind cla_Finder; //�ļ�������
	BOOL b_Found;
	bool bRtn = false;
	int j =0;

	RDA_FILE_S strr_Lod_File_List_Temp[MAX_LOD_FILE];


	for(j=0; j<MAX_LOD_FILE; j++)
	{
		this->m_strr_Lod_File_List[j].cs_Name.Format(_T(""));
		this->m_strr_Lod_File_List[j].cs_Path.Format(_T(""));
		this->m_strr_Lod_File_List[j].b_Enable_DL = false;
	}

	for(j=0; j<MAX_LOD_FILE; j++)
	{
		strr_Lod_File_List_Temp[j].cs_Name.Format(_T(""));
		strr_Lod_File_List_Temp[j].cs_Path.Format(_T(""));
		strr_Lod_File_List_Temp[j].b_Enable_DL = false;
	}
	j =0;

	this->m_iCfgFileNum =0;



	//��ȡ���·��
	if(this->m_cla_GFC.Get_File_Directory(wcp_FW_File_Path, &cs_FW_Dir) == false)
	{
		return;
	}
	//�ȶ�cfg�ļ�

	CString cs_Cfg_fir_path;
	bool bFindCfg = false;
	//-----1����������cfg�ļ�-------------------------------------------------------------

	try
	{
		//Ѱ���ļ���ʼ
		cs_Finder_Dir.Format(_T("%s\\*"), cs_FW_Dir);


		b_Found = cla_Finder.FindFile(cs_Finder_Dir);

		//
		while(b_Found) 
		{
			b_Found = cla_Finder.FindNextFile();

			cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//��ȡ�ҵ����ļ���

			cs_FW_Name.MakeUpper();//��д


			if(cla_Finder.IsDots())
			{
				continue;//�ҵ����ǵ�ǰĿ¼���ϼ�Ŀ¼������
			}

			if(!cla_Finder.IsDirectory())	//������ļ���
			{
				cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//��ȡ�ҵ����ļ���
				strFW_Name = cs_FW_Name;
				cs_FW_Name.MakeUpper();//��д
				if(wcsstr(cs_FW_Name, _T(".CFG")) != NULL)
				{
					//
					cs_Cfg_fir_path = cla_Finder.GetFilePath();
					bFindCfg = true;
					break;

				}
			}
		}
		//�ر�find
		cla_Finder.Close();

	}
	catch(...)
	{
		//
	}

	if (false == bFindCfg)
	{
		//AfxMessageBox(_T("Can not find cfg file"));
		this->Show_Status(_T("Can not find cfg file"));
		this->Show_Out(_T("Can not find cfg file"));
		return;
	}
	//-----,2��������CFG�ļ�------------------------------------------------------------
	//------------------------------------------------------------------
	// 		- file: M56RMAR01A02_MFV03.lod
	// # �������lod�ǽ����APP
	// 			- file: DCRE.lod
	// # �������lod��APP1
	// 			- file: APPGS5MDM32A01.lod
	// # �������lod��APP2
	// 			- file: APPGS5MDM32A02.lod
	// # ���������е�lod��ע���ˣ�������ϲ�
	// #    - file: APPGS5MDM32A03.lod
	//------------------------------------------------------------------
	CStdioFile cla_cfg_File;
	CString  csData,strD;
	int pos = -1,i=0;
	CString  strr_cfg_Lod_File_List[MAX_LOD_FILE];

	try
	{
		//��cfg�ļ�
		if(cla_cfg_File.Open(cs_Cfg_fir_path, (CFile::modeRead | CFile::modeNoTruncate | CFile::typeText)) == NULL)
		{
			//��Ŀ���ļ�ʧ��
			this->m_csError_Code.Format(_T("ERROR, Open cfg file fail!"));
			::AfxMessageBox(this->m_csError_Code);
			throw(0);
		}

		//��λ��ͷ
		cla_cfg_File.SeekToBegin();


		//ѭ����ȡ����
		while(cla_cfg_File.ReadString(csData) != NULL)
		{

			if(-1 != csData.Find(_T('#')))
			{
				continue;
			}

			pos  = csData.Find(_T("file\:"));

			if (-1 == pos)
			{
				continue;
			}
			if (i<MAX_LOD_FILE)
			{
				strr_cfg_Lod_File_List[i] = csData.Mid(pos+6);//- file: APPGS5MDM32A01.lod ע��ո�
				

				strD.Format(_T("Cfg Lod File: %s"),strr_cfg_Lod_File_List[i]);

				this->Show_Out(strD);

				i++;
			}
		}

		 this->m_iCfgFileNum = i;//20200318

		cla_cfg_File.Close();

	}
	catch(...)
	{
		//
	}
	//------------------------------------------------------------------
	try
	{
		//Ѱ���ļ���ʼ
		cs_Finder_Dir.Format(_T("%s\\*"), cs_FW_Dir);


		b_Found = cla_Finder.FindFile(cs_Finder_Dir);

		//
		while(b_Found) 
		{
			b_Found = cla_Finder.FindNextFile();

			cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//��ȡ�ҵ����ļ���

			cs_FW_Name.MakeUpper();//��д


			if(cla_Finder.IsDots())
			{
				continue;//�ҵ����ǵ�ǰĿ¼���ϼ�Ŀ¼������
			}

			if(!cla_Finder.IsDirectory())	//������ļ���
			{
				cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//��ȡ�ҵ����ļ���
				strFW_Name = cs_FW_Name;
				cs_FW_Name.MakeUpper();//��д
				if(wcsstr(cs_FW_Name, _T(".LOD")) != NULL)
				{
					CString cs_fir_path;
					cs_fir_path = cla_Finder.GetFilePath();
					if (j<MAX_LOD_FILE)
					{

						csData.Format(_T("%s"),cla_Finder.GetFileName());
						
						//20200103
						for (i=0;i<MAX_LOD_FILE;i++)
						{
							if (csData  == strr_cfg_Lod_File_List[i])
							{
								strr_Lod_File_List_Temp[j].cs_Name.Format(_T("%s"),csData);
								strr_Lod_File_List_Temp[j].cs_Path.Format(_T("%s"),cs_fir_path);
								strr_Lod_File_List_Temp[j].b_Enable_DL = true;
								j++;
								break;
							}	
						}
						//20200505 �Ҳ�������
						if (MAX_LOD_FILE == i)
						{
							strD.Format(_T("Lod_File-%s Not Find,Pleae Check cfg File"),csData);
							this->Show_Out(strD);

// 							this->m_csError_Code = strD;
// 							::AfxMessageBox(this->m_csError_Code);
// 							throw(0);
						}
					}
					
				}
			}
			else
			{
				//....
			}
		}//while(b_Found

		//�ر�find
		cla_Finder.Close();

	}
	catch(...)
	{
		return ;
	}

	//20200103
	for (i=0;i<MAX_LOD_FILE;i++)
	{
        if (true ==   strr_cfg_Lod_File_List[i].IsEmpty())
        {
			continue;
        }
            

		for (j=0;j<MAX_LOD_FILE;j++)
		{
			if (strr_cfg_Lod_File_List[i] ==  strr_Lod_File_List_Temp[j].cs_Name)
			{
				m_strr_Lod_File_List[i].cs_Name.Format(_T("%s"),strr_cfg_Lod_File_List[i]);
				m_strr_Lod_File_List[i].cs_Path.Format(_T("%s"),strr_Lod_File_List_Temp[j].cs_Path);
				m_strr_Lod_File_List[i].b_Enable_DL = true;
				break;
			}		  
		}	
	}	

	//
	return ;
}




bool CARM_OP_RDA::LOAD_LOD_FILE(LOD_FILE_S * strp_Lod_File)
{
	//����LOD�ļ�����Ҫ��Ϣ������������LODʱ����Ч��λaddr��phase data��λ�ú�����
	CString csData;
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;
	unsigned char ucrData[16];
	unsigned long i, ulRec_Quan, ulData_In_Phase_Idx, ulBytes_To_Read, ulStart_Byte;
	DWORD * dwpPhase_Data = NULL;
	int iTemp;
	bool bBreak_Flag;

	try
	{
		//------------------------------ �趨Ĭ�ϳ�ʼֵ ------------------------------ 
		memset(strp_Lod_File->ulr_Start_Index, 0, sizeof(strp_Lod_File->ulr_Start_Index));
		//------------------------------ 


		//------------------------------ ��LOD�ļ� ------------------------------ 
		hFile_Handle = ::CreateFile(strp_Lod_File->cs_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			this->m_csError_Code.Format(_T("FAIL, Open %s.lod file failed!"), strp_Lod_File->cs_Name);
			throw(0);
		}

		//��ȡlod�ļ���BYTE����lod�ļ�Size��
		strp_Lod_File->ul_File_Size = ::GetFileSize(hFile_Handle, NULL);
		//------------------------------ 


		//------------------------------ ȥ��ǰ��ġ�#$����ͷ����Ϣ��������һ����@�� ------------------------------ 
		for(i=0; i<strp_Lod_File->ul_File_Size; i++)
		{
			if(this->READ_FILE(hFile_Handle, ucrData, 1, &ulRec_Quan, false) != RTN_SUCCESS)
			{
				this->m_csError_Code += _T("Err 1 load lod file;");
				throw(0);
			}

			if(ucrData[0] == '@')
			{
				ulStart_Byte = i;
				break;//�ҵ���ַ��Ϣ��
			}
		}
		if(i >= strp_Lod_File->ul_File_Size)
		{
			//û�ҵ���@��
			this->m_csError_Code.Format(_T("FAIL, Load lod file, does not find \'@\'"));
			throw(0);
		}
		//------------------------------ 

		/*
		#$PA_MODEL=pasw_rda6625 
		#$FLSH_MODEL=flsh_spi16m 
		#$FLASH_SIZE=0x00200000 
		#$RAM_SIZE=0x00200000 
		#$RAM_PHY_SIZE=0x00200000 
		#$CALIB_BASE=0x001FC000 
		#$FACT_SETTINGS_BASE=0x001FE000 
		#$CODE_BASE=0x00000000 
		#$USER_DATA_BASE=0x001E0000 
		#$USER_DATA_SIZE=0x0001C000 
		#$PM_MODEL=pmu_8809 
		@08000000
		ffffffff
		00000000
		00000000
		00000000
		3c0581c0

		ffffffff
		ffffffff
		#checksum=c4d05e66
		*/

		//�ҵ���@����
		//��ȡ��1��addr���ݡ���10λBYTE��ַ���ݣ�08000000 + 0x0D + 0x0A��Ҳ�п�����9λBYTE��ַ����(01c000a0 + 0x0A)
		memset(ucrData, 0, sizeof(ucrData));
		//�ȶ�8λaddr���ݣ����ж���LOD�ļ�������
		iTemp = this->READ_FILE(hFile_Handle, ucrData, 8, &ulRec_Quan, false);
		switch(iTemp)
		{
		case RTN_SUCCESS://pass
			break;
		default:
			this->m_csError_Code += _T("Err 2 load lod file;");
			throw(0);
		}

		//��ʼ�����һ����ַ
		strp_Lod_File->ul_Phase_Quan = 0;//���LOD�ļ���phase������
		strp_Lod_File->dwr_Phase_Addr[strp_Lod_File->ul_Phase_Quan] = this->m_cla_GFC.HEX_BYTE_TO_DWORD(ucrData, 8);
		strp_Lod_File->ulr_Start_Index[strp_Lod_File->ul_Phase_Quan] = ulStart_Byte;//��һ����ַ����ǰ��������@�������ַ�����������������ַ���ݺ�@
		//------------------------------ 


		//------------------------------ �������������ݣ������LOD�ļ���ʲô���Ļ��з� ------------------------------ 
		iTemp = this->READ_FILE(hFile_Handle, ucrData, 1, &ulRec_Quan, false);
		switch(iTemp)
		{
		case RTN_SUCCESS://pass
			break;
		default:
			this->m_csError_Code += _T("Err 3 load lod file;");
			throw(0);
		}

		if(ucrData[0] == 0x0A)
		{
			strp_Lod_File->i_Lod_Type = 1;//��0x0A����
		}
		else if(ucrData[0] == 0x0D)
		{
			//��һ��byte����
			iTemp = this->READ_FILE(hFile_Handle, ucrData, 1, &ulRec_Quan, false);
			switch(iTemp)
			{
			case RTN_SUCCESS://pass
				break;
			default:
				this->m_csError_Code += _T("Err 4 load lod file;");
				throw(0);
			}

			if(ucrData[0] == 0x0A)
			{
				strp_Lod_File->i_Lod_Type = 2;//��0x0D + 0x0A����
			}
			else
			{
				this->m_csError_Code.Format(_T("FAIL, Load lod file, Lod type 1 is wrong!"));
				throw(0);
			}
		}
		else
		{
			this->m_csError_Code.Format(_T("FAIL, Load lod file, Lod type 2 is wrong!"));
			throw(0);
		}
		//------------------------------ 


		//------------------------------ ��ʼ����ÿ��Phase��������� ------------------------------ 
		//------------------------------ ��ʼ�����һ����ַ�����ݺ�������ַ������ ------------------------------ 
		//�������
		ulData_In_Phase_Idx = 0;//ÿ��phase�еľ���ʵ��dword���ݵ�index
		
		strp_Lod_File->ulr_Data_Quan_In_Phase[strp_Lod_File->ul_Phase_Quan] = 0;//ÿ��phase�еľ���ʵ��dword���ݵ�����ͳ��

		strp_Lod_File->dwr_FCS[0] = 0;
		strp_Lod_File->ul_FCS_Quan = 0;

		//����Phase_Data�ڴ档ÿ��phase�еľ���ʵ��dword����
		this->m_cla_GFC.BUFF_DWORD_CALLOC(&dwpPhase_Data, 0x10000);

		//ʵ��ÿ�ζ���BYTE����
		ulBytes_To_Read = 8 + strp_Lod_File->i_Lod_Type;

		do
		{
			//��10 BYTE��ַ���ݣ�08000000 + 0x0D + 0x0A�������ܶ���phase����Ҳ������addr
			memset(ucrData, 0, sizeof(ucrData));

			//������
			iTemp = this->READ_FILE(hFile_Handle, ucrData, ulBytes_To_Read, &ulRec_Quan, false);
			switch(iTemp)
			{
			case RTN_SUCCESS://pass
				break;
			case NO_ENOUGH_RESPONSE://û�ж���Ԥ�ڵ������������ܶ����ļ�������
				break;
			default:
				this->m_csError_Code += _T("Err 5 load lod file;");
				throw(0);
			}

			//�ж϶�LOD�Ƿ������
			if(ucrData[0] == '#')
			{
				//��飨���ң���#��֮���Ƿ���һ��FW��
				/*
				#checksum=b13da2b9
				#$mode=flsh_spi32m
				*/

				//ͳ����һ��FWǰ��byte������
				ulStart_Byte = ulRec_Quan;

				//
				bBreak_Flag = false;

				do
				{
					memset(ucrData, 0, sizeof(ucrData));
					//��һ�����ݵĲ���
					iTemp = this->READ_FILE(hFile_Handle, ucrData, 1, &ulRec_Quan, false);

					ulStart_Byte += ulRec_Quan;//�ۼƶ�������

					switch(iTemp)
					{
					case RTN_SUCCESS://������
						{
							if(ucrData[0] == '@')
							{
								bBreak_Flag = true;//���ҵ���ַ��Ϣ�ˣ�������ѭ������
								ulStart_Byte--;//��������@��
							}
						}
						break;
					case NO_RESPONSE://һ�����ݶ�û�ж��������ܶ����ļ�������
						{
							bBreak_Flag = true;
						}
						break;
					default:
						this->m_csError_Code += _T("Err 7 load lod file;");
						throw(0);
					}

					//ѭ�������н�����˳�whileѭ��
					if(bBreak_Flag == true)
					{
						break;//�˳�while
					}
				}
				while(true);

				//�ж��������
				if(iTemp == RTN_SUCCESS)
				{
					//LOD�ļ����ݡ�û�С�����
					//����@��������������ݲ���
					iTemp = this->READ_FILE(hFile_Handle, &ucrData[1], (ulBytes_To_Read - 1), &ulRec_Quan, false);
					switch(iTemp)
					{
					case RTN_SUCCESS://�������桰if(ucrData[0] == '@')������������
						break;
					default:
						this->m_csError_Code += _T("Err 8 load lod file;");
						throw(0);
					}

					//��һ��FWǰ��byte����ͳ��
					strp_Lod_File->ulr_Start_Index[strp_Lod_File->ul_Phase_Quan + 1] = ulStart_Byte;

					/*
					CString b;
					for(int aa=0; aa<200; aa++)
					{
						if(strp_Lod_File->ulr_Start_Index[aa] > 0)
						{
							b.Format(_T("index = %d, head = %d;"), (aa), strp_Lod_File->ulr_Start_Index[aa]);
							::AfxMessageBox(b);
						}
					}
					*/
				}
				else
				{
					//LOD�ļ����ݡ������ˡ�
					//������һ��phase����
					//����FCS
					if(this->Calcu_FCS(
						(unsigned char *)dwpPhase_Data,
						(strp_Lod_File->ulr_Data_Quan_In_Phase[strp_Lod_File->ul_Phase_Quan] * 4),
						strp_Lod_File) == false)
					{
						throw(0);
					}

					//��һ��phase�����ˣ�phase��������һ��ͳ���Ѿ������phase������
					strp_Lod_File->ul_Phase_Quan++;
					if(strp_Lod_File->ul_Phase_Quan > 200)
					{
						this->m_csError_Code.Format(_T("FAIL, Load lod file, %s phase %d is wrong!"), strp_Lod_File->cs_Name, strp_Lod_File->ul_Phase_Quan);
						throw(0);
					}

					break;//�����ļ�������, �˳���LOD whileѭ��
				}
			}//if(ucrData[0] == '#'

			//�жϡ������Ѷ�����LOD����
			if(ucrData[0] == '@')//����addr�����ˣ���������һ��phase�����ˣ��µ����ݿ�ʼ��
			{
				//������һ��phase����
				//����FCS
				if(this->Calcu_FCS(
					(unsigned char *)dwpPhase_Data,
					(strp_Lod_File->ulr_Data_Quan_In_Phase[strp_Lod_File->ul_Phase_Quan] * 4),
					strp_Lod_File) == false)
				{
					throw(0);
				}

				//��һ��phase�����ˣ�phase��������һ��ͳ���Ѿ������phase������
				strp_Lod_File->ul_Phase_Quan++;
				if(strp_Lod_File->ul_Phase_Quan > 200)
				{
					this->m_csError_Code.Format(_T("FAIL, Load lod file, %s phase %d is wrong!"), strp_Lod_File->cs_Name, strp_Lod_File->ul_Phase_Quan);
					throw(0);
				}

				//��ʼ��һ��phase
				ulData_In_Phase_Idx = 0;//��һ��phase�еľ���ʵ��dword���ݵ�index���㣬���phase�����������ݼ����ˡ���phsea���ݼ���
				//��phase��ַ
				strp_Lod_File->dwr_Phase_Addr[strp_Lod_File->ul_Phase_Quan] = this->m_cla_GFC.HEX_BYTE_TO_DWORD(&ucrData[1], 8);
				
				//��phase�����������
				strp_Lod_File->ulr_Data_Quan_In_Phase[strp_Lod_File->ul_Phase_Quan] = 0;//ÿ��phase�еľ���ʵ��dword���ݵ�����ͳ��

				//ǰ�����ˡ�@���������ļ����¶�һλ������addr�����е�0x0A
				iTemp = this->READ_FILE(hFile_Handle, ucrData, 1, &ulRec_Quan, false);
				switch(iTemp)
				{
				case RTN_SUCCESS://pass
					break;
				default:
					this->m_csError_Code += _T("Err 6 load lod file;");
					throw(0);
				}
			}//if(ucrData[0] == '@'
			else//��phase�е�data����
			{
				//��phase��������dwpPhase_Data��
				*(dwpPhase_Data + ulData_In_Phase_Idx) = this->m_cla_GFC.HEX_BYTE_TO_DWORD(ucrData, 8);

				//ÿ��phase��dword�������ۼ�
				ulData_In_Phase_Idx++;//dword������ͳ��

				//�����phase dword���ݣ�ͳ�Ƶ��ṹ��ulr_Data_Quan_In_Phase��
				strp_Lod_File->ulr_Data_Quan_In_Phase[strp_Lod_File->ul_Phase_Quan]++;
			}
		}
		while(true);
	

		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//�ͷ�Phase_Data�ڴ�
		this->m_cla_GFC.BUFF_DWORD_FREE(&dwpPhase_Data);
	}
	catch(...)
	{
		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//�ͷ�Phase_Data�ڴ�
		this->m_cla_GFC.BUFF_DWORD_FREE(&dwpPhase_Data);

		return false;
	}

//	this->m_csError_Code.Format(_T("%s: %d, %d"), strp_Lod_File->cs_Name, strp_Lod_File->ul_Phase_Quan, strp_Lod_File->ul_FCS_Quan);
//	::AfxMessageBox(this->m_csError_Code);

	return true;
}

bool CARM_OP_RDA::Calcu_FCS(unsigned char * ucp_Data_Buff, unsigned long ul_Data_Quan, LOD_FILE_S * strp_Lod_File)
{
	DWORD dwFinal = 0;
	unsigned long data_done = 0;
	unsigned long chunksize = FLASH_PACKET_SIZE_MAX;
	int data_len = 0;
	unsigned char transData[16], digest[3];
	int offset, mini_len, j;
	FCSContext ctx;

	while(data_done < ul_Data_Quan)
	{
		offset = 0;
		mini_len = 0;

		FCSInit(&ctx);

		memset(transData, 0, sizeof(transData));
		memset(digest, 0, sizeof(digest));

		data_len = ( ul_Data_Quan - data_done > chunksize ) ? (chunksize):(ul_Data_Quan - data_done);

		while(offset < data_len)
		{
			mini_len = ((data_len - offset) > 16) ? 16 : (data_len - offset);

			for(j=0; j<mini_len; j++)
			{
				transData[j] = *(ucp_Data_Buff + data_done + offset + j);
			}

			FCSUpdate(&ctx, transData, mini_len);
			offset += mini_len;
		}

		FCSFinal(&ctx, digest);

		dwFinal = 0x00000000 | (digest[0]<<16) | (digest[1]<<8) | digest[2];

		data_done += data_len;//�ۼ�

		//�����FCS
		strp_Lod_File->dwr_FCS[strp_Lod_File->ul_FCS_Quan] = dwFinal;
		strp_Lod_File->ul_FCS_Quan++;
		if(strp_Lod_File->ul_FCS_Quan > 300)
		{
			this->m_csError_Code.Format(_T("FAIL, Load lod file, %s fcs quan %d is wrong!"), strp_Lod_File->cs_Name, strp_Lod_File->ul_FCS_Quan);
			return false;
		}

		/*
		int i = 0;
		CString csData;
		i++;
		csData.Format(_T("[ %d ]: 0x%08X; [ data_done=%d; data_len=%d ]"), i, dwFinal, data_done, data_len);
		::AfxMessageBox(csData);
		*/
	}

	return true;
}

void CARM_OP_RDA::FCSInit(struct FCSContext * context)
{
	context->remainder = 0xFFFFFFFF;
}

void CARM_OP_RDA::FCSUpdate(struct FCSContext * context, unsigned char * buffer, int len)
{
	int i;
    unsigned int R = context->remainder;

    for(i = 0; i<len; i++)
	{
        R = (R<<8) ^ fcs_table[(unsigned char)((R>>16)^(fcs_msb2lsb[*buffer++]))];
	}

    context->remainder = R;
}

void CARM_OP_RDA::FCSFinal(struct FCSContext * context, unsigned char * fcs)
{
	unsigned int R = context->remainder;

    R = ~R;  // ones complement of the remainder

	*(fcs + 0) = fcs_msb2lsb[(unsigned char)(R >> 16)];
	*(fcs + 1) = fcs_msb2lsb[(unsigned char)(R >> 8)];
	*(fcs + 2) = fcs_msb2lsb[(unsigned char)(R)];
}

bool CARM_OP_RDA::Write_DWORD_BLOCK(DWORD dw_Write_Addr, DWORD * dwp_Write_Data, unsigned long ul_Write_Quan)
{
	//���Ͷ��DWORD������
	if(this->PKT_CMD_LOAD_DWORD(dwp_Write_Data, ul_Write_Quan, dw_Write_Addr, CMD_WR_BLOCK, &this->m_str_Pkt_Send) == false)
	{
		return false;
	}

	//����ָ��
	if(this->WRITE_PKT(this->m_str_Pkt_Send) == false)
	{
		this->m_csError_Code.Format(_T("Write_DWORD_BLOCK, WRITE_PKT, Fail!"));
		return false;
	}

	return true;
}

bool CARM_OP_RDA::DL_RAMRUN_LOD_FILE(LOD_FILE_S str_Ramrun_Lod)
{
	CString csData;
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;
	int i, j, iSend_Times, iSend_Rem, iTemp;
	unsigned long ulRec_Quan, ulCount_DW, ulCount_BYTE, ulTemp;
	DWORD * dwpPhase_Data = NULL;//�������ÿblock��dword����
	BYTE * ucpRead_Buff = NULL;//��LOD�ļ���BUFF
	DWORD dwAddr;
	unsigned char ucByte;
	unsigned long ulBytes_To_Read = 8 + str_Ramrun_Lod.i_Lod_Type;//ʵ��ÿ�ζ���BYTE����

	try
	{
		//��LOD�ļ�
		hFile_Handle = ::CreateFile(str_Ramrun_Lod.cs_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl ramrun lod file, Open %s.lod file failed!"), str_Ramrun_Lod.cs_Name);
			throw(0);
		}


		//�����ڴ�
		this->m_cla_GFC.BUFF_DWORD_CALLOC(&dwpPhase_Data, (HOST_PACKET_MAX + 8));
		this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpRead_Buff, (HOST_PACKET_MAX * ulBytes_To_Read + 8));


		//��LOD�ļ���λ��ָ������@����addr��Ϣ
		if(str_Ramrun_Lod.ulr_Start_Index[0] > 0)
		{
			//��һ����ַ����ǰ���������ݣ���Ҫ��һ�ζ��Ĳ���
			iTemp = this->READ_FILE(hFile_Handle, ucpRead_Buff, str_Ramrun_Lod.ulr_Start_Index[0], &ulRec_Quan, true);
			switch(iTemp)
			{
			case RTN_SUCCESS://pass
				break;
			default:
				this->m_csError_Code += _T("Err 1; DL ramrun lod file;");
				throw(0);
			}
		}

		//
		for(i=0; i<str_Ramrun_Lod.ul_Phase_Quan; i++)//phaseѭ��
		{
			//------------------------------- ����phase��addr��Ϣ�����˶�addr ------------------------------- 
			//addrǰ�и���@��������Ҫ���һ��byte
			iTemp = this->READ_FILE(hFile_Handle, ucpRead_Buff, (ulBytes_To_Read + 1), &ulRec_Quan, true);
			switch(iTemp)
			{
			case RTN_SUCCESS://pass
				break;
			default:
				this->m_csError_Code += _T("Err 2; DL ramrun lod file;");
				throw(0);
			}

			//�ж��Ƿ���addr�ı�ʶ��@��
			if(*(ucpRead_Buff + 0) != '@')
			{
				this->m_csError_Code.Format(_T("FAIL, Dl ramrun lod file, Addr head (%c) is wrong!"), *(ucpRead_Buff + 0));
				throw(0);
			}

			//���������ݣ�������ֹ���������������
			*(ucpRead_Buff + ulRec_Quan) = '\0';

			//�����ַ
			dwAddr = this->m_cla_GFC.HEX_BYTE_TO_DWORD((ucpRead_Buff + 1), 8);

			if(dwAddr != str_Ramrun_Lod.dwr_Phase_Addr[i])
			{
				this->m_csError_Code.Format(_T("FAIL, Dl ramrun lod file, Addr data (0x%08X) is wrong, not 0x%08X!"), dwAddr, str_Ramrun_Lod.dwr_Phase_Addr[i]);
				throw(0);
			}

			//------------------------------- ��ַ��ȷ����LOD�������� ------------------------------- 
			//�����phase�У���Ҫ�����ٴ�block�������һ�ε�������
			iSend_Times = str_Ramrun_Lod.ulr_Data_Quan_In_Phase[i] / HOST_PACKET_MAX;//ÿ��block�е�DWORD��������
			iSend_Rem = str_Ramrun_Lod.ulr_Data_Quan_In_Phase[i] % HOST_PACKET_MAX;//���һ��block�е�DWORD��������

			for(j=0; j<=iSend_Times; j++)
			{
				//����ÿ�δ����block��dword������
				if(j == iSend_Times)
				{
					if(iSend_Rem > 0)
					{
						ulCount_DW = iSend_Rem;
					}
					else
					{
						break;//û��ʣ��������
					}
				}
				else
				{
					ulCount_DW = HOST_PACKET_MAX;//1024��DWORD �� 4096��BYTE = 0x1000
				}

				ulCount_BYTE = ulCount_DW * ulBytes_To_Read;//8λdword���ݺ�����λ��һλ���س����С�����08000000 + 0x0D + 0x0A��

				iTemp = this->READ_FILE(hFile_Handle, ucpRead_Buff, ulCount_BYTE, &ulRec_Quan, true);
				switch(iTemp)
				{
				case RTN_SUCCESS://pass
					break;
				default:
					this->m_csError_Code += _T("Err 3; DL ramrun lod file;");
					throw(0);
				}

				//���������ݣ�������ֹ���������������
				*(ucpRead_Buff + ulRec_Quan) = NULL;

				//��LOD�ļ��е�byte����ת��Ϊdword����
				this->TRANS_DWORD_DATA(ucpRead_Buff, dwpPhase_Data, &ulTemp);

				if(ulTemp != ulCount_DW)
				{
					this->m_csError_Code.Format(_T("FAIL, Dl ramrun lod file, Trans dword data, %d != %d;"), ulTemp, ulCount_DW);
					throw(0);
				}

				//дblock����
				if(this->Write_DWORD_BLOCK(dwAddr, dwpPhase_Data, ulCount_DW) == false)
				{
					this->m_csError_Code.Format(_T("FAIL, Dl ramrun lod file, Write dword block fail!"));
					throw(0);
				}

				//addr�������ţ���byteΪ��λ������һ��block���ݵĵ�ַ
				ulTemp = dwAddr;
				dwAddr = ulTemp + ulCount_DW * 4;
			}//for(j=0; j<=iSend_Times;
		}//for(i=0; i<str_Ramrun_Lod.ul_Phase_Quan;

		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_DWORD_FREE(&dwpPhase_Data);
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRead_Buff);


		//AD 00 0A FF 82 A0 00 C0 81 FF 00 00 00 63
		dwAddr = HOST_MONITOR_CTRL;
		ulTemp = 0xFF;
		if(this->Write_DWORD(dwAddr, ulTemp) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl ramrun lod file, Write dword, 0x%08X, 0x%08X!"), dwAddr, ulTemp);
			throw(0);
		}

		//AD 00 07 FF 84 05 00 00 00 FF 81
		dwAddr = H2P_STATUS_REG;
		ucByte = 0xFF & 0xFF;
		if(this->Write_REG(dwAddr, ucByte) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl ramrun lod file, Write reg, 0x%08X, 0x%02X!"), dwAddr, ucByte);
			throw(0);
		}


		//��ս���COM�������ݵ�BUFF
		this->m_cla_GFC.PKT_BUFF_DEL_ALL(&this->m_str_Com_Rx_Buff);


		//�ȴ�Ԥ�ڻظ�
		//AD 00 06 FF 00 04 00 00 FF 04
		if(this->WAIT_DWORD(0x00, 0xFF000004, 5) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl ramrun lod file, Wait dword, 0xFF000004;"));
			throw(0);
		}

		//AD 00 06 FF 00 57 00 00 00 A8
		if(this->WAIT_DWORD(0x00, 0x00000057, 5) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl ramrun lod file, Wait dword, 0x00000057;"));
			throw(0);
		}

		//AD 00 06 FF 00 F0 00 00 00 0F
		if(this->WAIT_DWORD(0x00, EVENT_FLASH_PROG_READY, 5) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl ramrun lod file, Wait dword, EVENT_FLASH_PROG_READY;"));
			throw(0);
		}
	}
	catch(...)
	{
		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_DWORD_FREE(&dwpPhase_Data);
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRead_Buff);

		return false;
	}

	return true;
}

void CARM_OP_RDA::TRANS_DWORD_DATA(unsigned char * ucp_Hex_Data, DWORD * dwp_Buff, DWORD * dwp_Quan)
{
	unsigned long i = 0;
	unsigned long j = 0;

	while(*(ucp_Hex_Data + i) != NULL)
	{
		if(((*(ucp_Hex_Data + i) >= '0') && (*(ucp_Hex_Data + i) <= '9')) ||
			((*(ucp_Hex_Data + i) >= 'a') && (*(ucp_Hex_Data + i) <= 'f')) ||
			((*(ucp_Hex_Data + i) >= 'A') && (*(ucp_Hex_Data + i) <= 'F')))
		{
			*(dwp_Buff + j) = this->m_cla_GFC.HEX_BYTE_TO_DWORD((ucp_Hex_Data + i), 8);
			j++;//ת������dword���ݵ�ͳ��
			i += 8;
		}
		else
		{
			i++;
		}
	}

	*dwp_Quan = j;
}

bool CARM_OP_RDA::WAIT_DWORD(unsigned char uc_Tgt_ID, DWORD dw_Tgt_Data, int i_TimeOut)
{
	//���COM��BUFF������û�С��������ء�
	unsigned char * ucpRd_Data = NULL;//��ȡ���������ݡ���BUFF
	unsigned long ulRd_Data_Quan;//���������ݡ�������
	unsigned char ucRd_ID_Read;
	DWORD dwTemp_Data;
	time_t t_Start, t_End;
	bool bRtn = false;

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

			//��һ�ζ����ڵĲ����������ݷŵ�Rx_Buff��
			this->READ_PKT(&this->m_str_Com_Rx_Buff);

			//����������
			if(this->m_str_Com_Rx_Buff.ul_Data_Length > 0)//����������
			{
				//--------------------------- ȷʵ�ж�����Ӧ���ݣ�����Է����ڴ棬׼���������� --------------------------- 
				//�����ڴ棬���������ݡ���BUFF�����С��m_str_Com_Rx_Buff�Ĵ�Сһ��
				this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpRd_Data, this->m_str_Com_Rx_Buff.ul_Data_Length);

				//--------------------------- �����յ������� --------------------------- 
				//ѭ������m_str_Com_Rx_Buff�е����ݣ�ֱ�������Ѿ�û���ˡ��Ϸ���Э�����ݰ�������˳�ѭ��
				while(this->GET_DATA_FROM_READ_PKT(&this->m_str_Com_Rx_Buff, &ucRd_ID_Read, ucpRd_Data, &ulRd_Data_Quan) == true)
				{
					//��ȡ��һ���Ϸ����ݣ�����ǲ���������ζ������ġ��Ϸ�Э�����ݰ�������Ӧ
					if((ucRd_ID_Read == uc_Tgt_ID) && (ulRd_Data_Quan == 4))
					{
						//rd_id��ȷ������������Ҳ��ȷ�����Ƕ�����������ζ���������Ӧ

						dwTemp_Data = 0x0;

						dwTemp_Data = dwTemp_Data | (*(ucpRd_Data + 0)) | ((*(ucpRd_Data + 1)) << 8) | ((*(ucpRd_Data + 2)) << 16) | ((*(ucpRd_Data + 3)) << 24);

						if(dwTemp_Data == dw_Tgt_Data)
						{
							bRtn = true;//PASS
							break;//�˳�whileѭ��
						}				
					}
				}//while(

				//�ͷ��ڴ�
				this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRd_Data);

				//�ж��Ƿ��Ѿ��ҵ�Ŀ��ظ���
				if(bRtn == true)
				{
					break;//pass
				}

			}//if(this->m_str_Com_Rx_Buff.ul_Data_Length > 0

			//��ʱ���
			time(&t_End);
			if(difftime(t_End, t_Start) >= i_TimeOut)
			{
				this->m_csError_Code.Format(_T("FAIL, Wait dword, timeout, no data!"));
				throw(0);
			}

			//�´ζ�ȡ�ļ��ʱ��
			Sleep(10);
		}
		while(true);
	}
	catch(...)
	{
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRd_Data);
		return false;
	}

	return bRtn;
}

int CARM_OP_RDA::READ_FILE(HANDLE h_File, unsigned char * ucp_Read_Buff, unsigned long ul_Bytes_To_Read, unsigned long * ulp_Bytes_Real_Rec, bool b_Show_Progress)
{
	try
	{
		if(h_File == INVALID_HANDLE_VALUE)
		{
			//��Ч�ļ����
			this->m_csError_Code.Format(_T("FAIL, Read file, Handle is invalid; "));
			throw(RTN_ERROR);
		}

		if(::ReadFile(h_File, ucp_Read_Buff, ul_Bytes_To_Read, ulp_Bytes_Real_Rec, NULL) != TRUE)
		{
			//��ʧ��
			this->m_csError_Code.Format(_T("FAIL, Read file, Read fail; "));
			throw(RTN_FAIL);
		}

		if(*ulp_Bytes_Real_Rec == 0)
		{
			//û����ֵ���п����Ƕ����ļ�������
			this->m_csError_Code.Format(_T("FAIL, Read file, Nothing response; "));
			throw(NO_RESPONSE);
		}

		if(*ulp_Bytes_Real_Rec != ul_Bytes_To_Read)
		{
			//û�����㹻��ֵ
			this->m_csError_Code.Format(_T("FAIL, Read file, Not enough response; "));
			throw(NO_ENOUGH_RESPONSE);
		}

		//��������ʾ
		if(b_Show_Progress == true)
		{
			//�Ѷ����ļ�byte��ͳ��
			this->m_ulRead_Bytes += *ulp_Bytes_Real_Rec;
			
			//����״̬��Ϣ��ʾ
			this->m_csError_Code.Format(_T("Download file ( %d / %d ) ...... %.1f%%"),
				this->m_ulRead_Bytes, this->m_ulAll_Bytes, (100.0 * this->m_ulRead_Bytes / this->m_ulAll_Bytes));
			this->Show_Status(this->m_csError_Code);
			this->m_csError_Code.Format(_T(""));

			//��ʾ������
			this->Show_Progress((1.0 * this->m_ulRead_Bytes / this->m_ulAll_Bytes));
		}
	}
	catch(int err)
	{
		return err;
	}

	return RTN_SUCCESS;
}

bool CARM_OP_RDA::DL_FW_LOD_FILE(LOD_FILE_S str_FW_Lod)
{
	//����FW��LOD�ļ�
	CString csData;
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;
	int i, j, iSend_Times, iSend_Rem, iTemp;
	unsigned long ulRec_Quan, ulCount_DW, ulCount_BYTE, ulTemp;
	DWORD * dwpPhase_Data = NULL;//�������ÿblock��dword����
	BYTE * ucpRead_Buff = NULL;//��LOD�ļ���BUFF
	DWORD dwAddr;
	unsigned long ulBytes_To_Read = 8 + str_FW_Lod.i_Lod_Type;//ʵ��ÿ�ζ���BYTE����
	FPC_CMD_INFO fpc_cmd[2];
	bool wait_event_flag = false;
	DWORD x, y, curr, prev;
	int iCnt = 0;//�������˶��ٸ�block

	try
	{
		//��LOD�ļ�
		hFile_Handle = ::CreateFile(str_FW_Lod.cs_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Open %s.lod file failed!"), str_FW_Lod.cs_Name);
			throw(0);
		}


		//�����ڴ�
		this->m_cla_GFC.BUFF_DWORD_CALLOC(&dwpPhase_Data, (this->m_str_Fpc_Access_Info.buf_size + 8));
		this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpRead_Buff, (this->m_str_Fpc_Access_Info.buf_size * ulBytes_To_Read + 8));


		//��տ��Ʋ���
		x    = 0;//����cmd_base
		y    = 0;//����ram_buf_base
		curr = 0;
		prev = -1;
		wait_event_flag = false;

		//��ս���COM�������ݵ�BUFF
		this->m_cla_GFC.PKT_BUFF_DEL_ALL(&this->m_str_Com_Rx_Buff);
		this->Clear_Com_Rx_Buffer();

		//phaseѭ��
		for(i=0; i<str_FW_Lod.ul_Phase_Quan; i++)
		{
			//------------------------------- ��LOD�ļ���λ��ָ������@����addr��Ϣ ------------------------------- 
			if(str_FW_Lod.ulr_Start_Index[i] > 0)
			{
				//��һ����ַ����ǰ���������ݣ���Ҫ��һ�ζ��Ĳ���
				iTemp = this->READ_FILE(hFile_Handle, ucpRead_Buff, str_FW_Lod.ulr_Start_Index[i], &ulRec_Quan, true);
				switch(iTemp)
				{
				case RTN_SUCCESS://pass
					break;
				default:
					this->m_csError_Code += _T("Err 1; DL fw lod file;");
					throw(0);
				}
			}
			//------------------------------- 


			//------------------------------- ����phase��addr��Ϣ�����˶�addr ------------------------------- 
			//addrǰ�и���@��������Ҫ���һ��byte
			iTemp = this->READ_FILE(hFile_Handle, ucpRead_Buff, (ulBytes_To_Read + 1), &ulRec_Quan, true);
			switch(iTemp)
			{
			case RTN_SUCCESS://pass
				break;
			default:
				this->m_csError_Code += _T("Err 2; DL fw lod file;");
				throw(0);
			}

			//�ж��Ƿ���addr�ı�ʶ��@��
			if(*(ucpRead_Buff + 0) != '@')
			{
				this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Addr head (%c) is wrong!"), *(ucpRead_Buff + 0));
				throw(0);
			}

			//���������ݣ�������ֹ���������������
			*(ucpRead_Buff + ulRec_Quan) = '\0';

			//�����ַ
			dwAddr = this->m_cla_GFC.HEX_BYTE_TO_DWORD((ucpRead_Buff + 1), 8);

			if(dwAddr != str_FW_Lod.dwr_Phase_Addr[i])
			{
				this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Addr data (0x%08X) is wrong, not 0x%08X!"), dwAddr, str_FW_Lod.dwr_Phase_Addr[i]);
				throw(0);
			}
			//------------------------------- 


			//���addr�Ƿ�����
			if((str_FW_Lod.dwr_Phase_Addr[i] == 0x02000000) && (str_FW_Lod.ulr_Data_Quan_In_Phase[i] > 8))
			{
				this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, dwr_Phase_Addr == 0x02000000, error!"));
				::AfxMessageBox(this->m_csError_Code);
				//bChkMagic = TRUE;
				throw(0);
			}


			//����ϴ�ѭ����Ӧ��
			if(wait_event_flag)
			{
				if(this->WAIT_DWORD(0x00, (EVENT_FLASH_PROG_READY + x), 5) == false)
				{
					this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Wait for rsp-1 0x%08X timeout!"), (EVENT_FLASH_PROG_READY + x));
					throw(0);
				}
			}
			
			//------------------ �� x��0, ��FPC_CMD_INFO, FPC_NONE
			fpc_cmd[x].cmd        = FPC_NONE;
			fpc_cmd[x].flash_addr = str_FW_Lod.dwr_Phase_Addr[i] & 0xffffff;//ֻȡ��6λ
			fpc_cmd[x].ram_addr   = this->m_str_Fpc_Access_Info.ram_buf_base[x];//fastpf_ini��
			fpc_cmd[x].data_size  = 0x10000;
			fpc_cmd[x].fcs        = 0x0;
			//д
			//AD 00 1A FF 83 D8 56 C0 81 00 00 00 00 00 00 02 00 00 80 00 A2 00 00 01 00 00 00 00 00 92 
			if(this->Write_DWORD_BLOCK(this->m_str_Fpc_Access_Info.cmd_base[x], (DWORD *)(&fpc_cmd[x]), 5) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Write fpc cmd info fpc none 0 fail!"));
				throw(0);
			}
			//------------------ �� x��0, ��FPC_CMD_INFO, FPC_ERASE_SECTOR
			fpc_cmd[x].cmd = FPC_ERASE_SECTOR;
			//д
			//AD 00 0A FF 83 D8 56 C0 81 02 00 00 00 B1
			if(this->Write_DWORD_BLOCK(this->m_str_Fpc_Access_Info.cmd_base[x], (DWORD *)(&fpc_cmd[x]), 1) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Write fpc cmd info fpc erase sector 0 fail!"));
				throw(0);
			}

			//
			if(++curr == 2)
			{
				wait_event_flag = true;
			}


			x = x ^ 0x1;//������㣬��ͬʱΪ1��i��0��1֮���л�


			//�����ַ
			dwAddr = str_FW_Lod.dwr_Phase_Addr[i] & 0xffffff;//ȡ��6λ


			//------------------------------- ��ַ��ȷ����LOD�������ݣ�ת����DWORD���� ------------------------------- 
			//�����phase�У���Ҫ�����ٴ�block�������һ�ε�������
			iSend_Times = str_FW_Lod.ulr_Data_Quan_In_Phase[i] / this->m_str_Fpc_Access_Info.buf_size;//ÿ��block�е�DWORD��������
			iSend_Rem = str_FW_Lod.ulr_Data_Quan_In_Phase[i] % this->m_str_Fpc_Access_Info.buf_size;//���һ��block�е�DWORD��������


			for(j=0; j<=iSend_Times; j++)
			{
				//------------------------------- ��DWORD���ݴ�LOD�ļ��ж��� ------------------------------- 
				//����ÿ�δ����block��dword������
				if(j == iSend_Times)
				{
					if(iSend_Rem > 0)
					{
						ulCount_DW = iSend_Rem;
					}
					else
					{
						break;//û��ʣ��������
					}
				}
				else
				{
					ulCount_DW = this->m_str_Fpc_Access_Info.buf_size;//0x2000��DWORD �� 0x8000��BYTE
				}


				//8λdword���ݺ�����λ��һλ���س����С�����08000000 + 0x0D + 0x0A��
				ulCount_BYTE = ulCount_DW * ulBytes_To_Read;


				iTemp = this->READ_FILE(hFile_Handle, ucpRead_Buff, ulCount_BYTE, &ulRec_Quan, true);
				switch(iTemp)
				{
				case RTN_SUCCESS://pass
					break;
				default:
					this->m_csError_Code += _T("Err 3; DL fw lod file;");
					throw(0);
				}

				//���������ݣ�������ֹ���������������
				*(ucpRead_Buff + ulRec_Quan) = NULL;

				//��LOD�ļ��е�byte����ת��Ϊdword����
				this->TRANS_DWORD_DATA(ucpRead_Buff, dwpPhase_Data, &ulTemp);

				if(ulTemp != ulCount_DW)
				{
					this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Trans dword data [%d], 0x%X != 0x%X;"), (i + 1), ulTemp, ulCount_DW);
					throw(0);
				}

				//------------------------------- ��ʼ����DWORD���� -------------------------------
				//AD 80 06 FF 83 00 00 00 A2
				//AD 80 06 FF 83 00 80 00 A2
				//дFW���ݣ�block����
				if(this->Write_DWORD_BLOCK(this->m_str_Fpc_Access_Info.ram_buf_base[y % 3], dwpPhase_Data, ulCount_DW) == false)
				{
					csData.Format(_T("%s"), this->m_csError_Code);
					this->m_csError_Code.Format(_T("FAIL, DL_FW_LOD_FILE, %s"), csData);
					throw(0);
				}


				//�ѷ��͵�block����
				iCnt++;


				//������LOD���ݣ�����F1 00 00 00
				if(wait_event_flag)
				{
					if(this->WAIT_DWORD(0x00, (EVENT_FLASH_PROG_READY + x), 5) == false)
					{
						this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Wait for rsp-2 0x%08X timeout!"), (EVENT_FLASH_PROG_READY + x));
						throw(0);
					}
				}


				//------------------ �� x��1, ��FPC_CMD_INFO, FPC_NONE
				fpc_cmd[x].cmd        = FPC_NONE;
				fpc_cmd[x].flash_addr = dwAddr;
				fpc_cmd[x].ram_addr   = this->m_str_Fpc_Access_Info.ram_buf_base[y % 3];
				fpc_cmd[x].data_size  = ulCount_DW * 4;
				fpc_cmd[x].fcs        = 0x0;
				//д
				//AD 00 1A FF 83 C4 56 C0 81 00 00 00 00 00 00 02 00 00 00 00 A2 00 80 00 00 00 00 00 00 8F 
				//AD 00 1A FF 83 D8 56 C0 81 00 00 00 00 00 80 02 00 00 80 00 A2 00 80 00 00 00 00 00 00 93 	
				if(this->Write_DWORD_BLOCK(this->m_str_Fpc_Access_Info.cmd_base[x], (DWORD *)(&fpc_cmd[x]), 5) == false)
				{
					this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Write fpc cmd info fpc none 0 fail!"));
					throw(0);
				}
				//------------------ �� x��1, ��FPC_CMD_INFO, FPC_PROGRAM
				fpc_cmd[x].cmd = FPC_PROGRAM;
				//д
				//AD 00 0A FF 83 C4 56 C0 81 01 00 00 00 AE 
				//AD 00 0A FF 83 D8 56 C0 81 01 00 00 00 B2
				if(this->Write_DWORD_BLOCK(this->m_str_Fpc_Access_Info.cmd_base[x], (DWORD *)(&fpc_cmd[x]), 1) == false)
				{
					this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Write fpc cmd info fpc erase sector 0 fail!"));
					throw(0);
				}

				wait_event_flag = true;//����i=1��FPC_CMD_INFO�󣬱������Ӧ

				if(++curr==2)
				{
					wait_event_flag = true;//������2��FPC_CMD_INFO���󣬵ȴ�һ�λظ�
				}

				x = x ^ 0x1;//x�л�״̬

				//addr�������ţ���byteΪ��λ������һ��block���ݵĵ�ַ
				ulTemp = dwAddr;
				dwAddr = ulTemp + ulCount_DW * 4;

				y++;//
			}//for(j=0; j<=iSend_Times;
		}//for(i=0; i<str_FW_Lod.ul_Phase_Quan;

		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_DWORD_FREE(&dwpPhase_Data);
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRead_Buff);


		//���block����Ŀ�Բ���
		if(iCnt != str_FW_Lod.ul_FCS_Quan)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Block quan is %d, FCS quan is %d, It is error."), iCnt, str_FW_Lod.ul_FCS_Quan);
			throw(0);
		}

		//��ȡ���һ��block����Ӧ
		if(this->WAIT_DWORD(0x00, (EVENT_FLASH_PROG_READY + x), 5) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Wait for rsp-3 0x%08X timeout!"), (EVENT_FLASH_PROG_READY + x));
			throw(0);
		}


		//------------------------------- ����FCS���� ------------------------------- 
		//�����ڴ�
		this->m_cla_GFC.BUFF_DWORD_CALLOC(&dwpPhase_Data, (str_FW_Lod.ul_FCS_Quan * 3 + 16));
		//AD 04 9E FF 83
		//00 00 01 A2 (ram_buf_base)
		//00 00 00 00 00 10 00 00 58 78 23 00
		//00 10 00 00 00 10 00 00 B7 40 6D 00
		//00 20 00 00 00 10 00 00 A2 31 35 00 

		//00 00 02 00 00 80 00 00 96 CC AF 00 (0x00AFCC96)
		//00 80 02 00 00 80 00 00 29 89 91 00 (0x00918929)
		//00 00 03 00 00 80 00 00 26 6F 54 00 (0x00546F26)
		//00 80 03 00 00 80 00 00 76 CB A0 00 (0x00A0CB76)

		//phaseѭ��
		iCnt = 0;//FCS����ͳ��
		j = 0;//addr + len + FCS ��dword����ͳ��

		for(i=0; i<str_FW_Lod.ul_Phase_Quan; i++)
		{
			int datadone = 0;
			int datalen  = 0;
			int pack_size = str_FW_Lod.ulr_Data_Quan_In_Phase[i];//0x4000

			dwAddr = str_FW_Lod.dwr_Phase_Addr[i] & 0xffffff;//ȥ����ַλ��ǰ���0x08

			while(datadone < pack_size)//0x8000 < 0x10000 
			{
				datalen = ( pack_size - datadone > FPC_BUFFER_SIZE / 4 )?(FPC_BUFFER_SIZE/4):(pack_size - datadone);//FPC_BUFFER_SIZE/4 = 0x2000 = 8192 


				/*
				fcsHash.push_back(dwAddr + datadone * 4);
				fcsHash.push_back(datalen * 4);
				fcsHash.push_back( p_flash_packet->fcsHash[index] );
				*/

				*(dwpPhase_Data + j) = dwAddr + datadone * 4;
				j++;
				*(dwpPhase_Data + j) = datalen * 4;
				j++;
				*(dwpPhase_Data + j) = str_FW_Lod.dwr_FCS[iCnt];
				j++;
				iCnt++;


				datadone += datalen;
			}
		}//for(i=0; i<str_FW_Lod.ul_Phase_Quan;

		//
		if(j > 0)
		{
			//AD 04 9E FF 83, 98 * 3 * 4 + 6 = 0x49E
			//дFCS���ݣ�block����
			if(this->Write_DWORD_BLOCK(this->m_str_Fpc_Access_Info.ram_buf_base[y % 3], dwpPhase_Data, j) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Write dword fcs block data fail!"));
				throw(0);
			}

			//------------------ FPC_CMD_INFO, FPC_NONE
			fpc_cmd[x].cmd        = FPC_NONE;
			fpc_cmd[x].flash_addr = 0x0;
			fpc_cmd[x].ram_addr   = this->m_str_Fpc_Access_Info.ram_buf_base[y % 3];
			fpc_cmd[x].data_size  = j / 3;//98 = 0x62
			fpc_cmd[x].fcs        = 0x0;
			//д
			//AD 00 1A FF 83 D8 56 C0 81 00 00 00 00 00 00 00 00 00 00 01 A2 62 00 00 00 00 00 00 00 72	
			if(this->Write_DWORD_BLOCK(this->m_str_Fpc_Access_Info.cmd_base[x], (DWORD *)(&fpc_cmd[x]), 5) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Write fpc cmd info, fpc_none, fail!"));
				throw(0);
			}
			//------------------ FPC_CMD_INFO, FPC_CHECK_FCS
			fpc_cmd[x].cmd = FPC_CHECK_FCS;
			//д
			//AD 00 0A FF 83 D8 56 C0 81 05 00 00 00 B6
			if(this->Write_DWORD_BLOCK(this->m_str_Fpc_Access_Info.cmd_base[x], (DWORD *)(&fpc_cmd[x]), 1) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Write fpc cmd info, fpc_check_fcs, fail!"));
				throw(0);
			}

			x = x ^ 0x1;//FCS�л�

			//�ȴ�������
			if(this->WAIT_DWORD(0x00, (EVENT_FLASH_PROG_READY + x), 5) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Wait for rsp-4 0x%08X timeout!"), (EVENT_FLASH_PROG_READY + x));
				throw(0);
			}

			x = x ^ 0x1;//wait FCS�л� 1

			if(this->WAIT_DWORD(0x00, (EVENT_FLASH_PROG_READY + x), 5) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Wait for rsp-5 0x%08X timeout!"), (EVENT_FLASH_PROG_READY + x));
				throw(0);
			}

			x = x ^ 0x1;//wait FCS�л� 2
		}//if(j > 0

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_DWORD_FREE(&dwpPhase_Data);


		//------------------------------- ���ؽ��� ------------------------------- 
		//------------------ FPC_CMD_INFO, FPC_END
		fpc_cmd[x].cmd        = FPC_END;
		fpc_cmd[x].flash_addr = 0x0;
		fpc_cmd[x].ram_addr   = 0x0;
		fpc_cmd[x].data_size  = 0x0;
		fpc_cmd[x].fcs        = 0x0;
		//д
		//AD 00 1A FF 83 C4 56 C0 81 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 AB
		if(this->Write_DWORD_BLOCK(this->m_str_Fpc_Access_Info.cmd_base[x], (DWORD *)(&fpc_cmd[x]), 5) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Write fpc cmd info, fpc_end, fail!"));
			throw(0);
		}
		
		//�ȴ�������
		if(this->WAIT_DWORD(0x00, (EVENT_FLASH_PROG_READY + x), 5) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Dl fw lod file, Wait for rsp-6 0x%08X timeout!"), (EVENT_FLASH_PROG_READY + x));
			throw(0);
		}

		x = x ^ 0x1;//wait FPC_END ����

		Sleep(1000);

		/*
		if( bChkMagic )
		{
		DWORD addr_tmp;
		DWORD data_tmp;

		data_tmp = 0xffffffff;
		addr_tmp = 0x82000000;
		call_flag = read_dword(&data_tmp, addr_tmp, SERIALREAD_TIMEOUT);
		if (!call_flag | (data_tmp != 0xD9EF0045))
		{
		return -7;
		}
		}
		*/
	}
	catch(...)
	{
		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_DWORD_FREE(&dwpPhase_Data);
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpRead_Buff);

		return false;
	}

	return true;
}

bool CARM_OP_RDA::Read_FPC_Access_Info(FPC_ACCESS_INFO * strp_Fpc_Access_Info)
{
	DWORD dwAddr, dwData;
	int i;

	try
	{
		//��AD 00 07 FF 02 7C 02 C0 81 03 C1�����ء�AD 00 06 FF 03 44 58 C0 81 A1��
		dwAddr = GALLITE_HALFIXED_ADDR;
		if(this->Read_DWORD(dwAddr, &dwData, 2) == false)
		{
			throw(0);
		}

		//��fpc_access_base��cmd_base
		//��AD 00 07 FF 02 E8 58 C0 81 03 0F�����ء�AD 00 06 FF 03 C0 56 C0 81 2B��
		dwAddr = dwData + FPC_ACCESS_OFFSET;
		if(this->Read_DWORD(dwAddr, &dwData, 2) == false)
		{
			throw(0);
		}
		//��ֵ
		strp_Fpc_Access_Info->fpc_access_base = dwData;//0x81C056C0
		strp_Fpc_Access_Info->cmd_base[0] = dwData + 4;//0x81C056C4
		strp_Fpc_Access_Info->cmd_base[1] = dwData + 24;//0x81C056D8

		//��AD 00 07 FF 02 C0 56 C0 81 03 29�����ء�AD 00 06 FF 03 01 FA 04 FB F8��
		dwAddr = dwData;
		if(this->Read_DWORD(dwAddr, &dwData, 2) == false)
		{
			throw(0);
		}

		//��ram_buf_base
		dwAddr = strp_Fpc_Access_Info->fpc_access_base + 44;// (0x81C056C0 + 44 = 0xEC)
		for(i=0; i<3; i++)
		{
			//AD 00 07 FF 02 EC 56 C0 81 03 05
			//AD 00 07 FF 02 F0 56 C0 81 03 19
			//AD 00 07 FF 02 F4 56 C0 81 03 1D
			if(this->Read_DWORD(dwAddr, &dwData, 2) == false)
			{
				throw(0);
			}
			else
			{
				//AD 00 06 FF 03 00 00 00 A2 5E
				//AD 00 06 FF 03 00 80 00 A2 DE
				//AD 00 06 FF 03 00 00 01 A2 5F
				strp_Fpc_Access_Info->ram_buf_base[i] = dwData;//0xA2000000
				dwAddr = dwAddr + 4;
			}
		}

		//��buf_size
		//��AD 00 07 FF 02 F8 56 C0 81 03 5C EE�����ء�AD 00 06 FF 03 00 80 00 00 7C��
		//dwAddr = ��һ����ַ + 4 = 0x81C056F8
		if(this->Read_DWORD(dwAddr, &dwData, 2) == false)
		{
			throw(0);
		}

		strp_Fpc_Access_Info->buf_size = dwData>>2;//0x8000>>2 = 0x8000 / 4 = 0x2000
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void CARM_OP_RDA::Get_Lod_File_Name(const wchar_t * wcp_File_Path, wchar_t * wcp_File_Name)
{
	int i, j;
	int iLong = wcslen(wcp_File_Path);

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

	return;
}

bool CARM_OP_RDA::Get_Lod_File_Info(wchar_t * wcp_Lod_File_Name, CString * csp_Product_Name)
{
	//��ȡLod�ļ����еĻ�����
	//GC10RAR01A07��GC65RAR01A04_BETA1017
	wchar_t * wcpKey_1 = NULL;
	wchar_t * wcpKey_2 = NULL;
	wchar_t * wcpKey_3 = NULL;
	bool bGet_R_A_Key = false;
	int iLen, i;
	wchar_t wcrData[64];

	try
	{
		csp_Product_Name->Format(_T(""));

		//��R
		wcpKey_1 = wcsstr(wcp_Lod_File_Name, _T("R"));

		while(wcpKey_1 != NULL)
		{
			//��R���A
			wcpKey_2 = wcsstr(wcpKey_1, _T("A"));

			if(wcpKey_2 != NULL)
			{
				//"R"��"A"֮�������ַ���"R"�Ժ�������6���ַ�
				if((wcpKey_2 - wcpKey_1) == 3)
				{
					//"R"��"A"�󶼸�����λ����
					if((*(wcpKey_1 + 1) >= '0') && (*(wcpKey_1 + 1) <= '9') &&
						(*(wcpKey_1 + 2) >= '0') && (*(wcpKey_1 + 2) <= '9') &&
						(*(wcpKey_2 + 1) >= '0') && (*(wcpKey_2 + 1) <= '9') &&
						(*(wcpKey_2 + 2) >= '0') && (*(wcpKey_2 + 2) <= '9'))
					{
						//�ҵ���RxxAxx���汾��Ϣ��λ�ã�wcpKey_1�����׵�ַ
						bGet_R_A_Key = true;
						break;
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

		//û�ж�λ����RxxAxx������
		if(bGet_R_A_Key == false)
		{
			throw(0);
		}

		//�������������
		iLen = wcpKey_1 - wcp_Lod_File_Name - 2;//ȥ����������ġ�RA��order code��Ϣ
		if((iLen <= 0) || (iLen >= 64))
		{
			throw(0);
		}

		//���������
		memset(wcrData, 0, sizeof(wcrData));
		for(i=0; i<iLen; i++)
		{
			wcrData[i] = *(wcp_Lod_File_Name + i);
		}
		csp_Product_Name->Format(_T("%s"), wcrData);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CARM_OP_RDA::Load_Lod_File_Info(void)
{
	wchar_t * wcpPath = NULL;
	wchar_t * wcpName = NULL;

	try
	{
		//�����ڴ�
		wcpPath = (wchar_t *)calloc(MAX_PATH, sizeof(wchar_t));
		wmemset(wcpPath, 0, MAX_PATH);

		wcpName = (wchar_t *)calloc(128, sizeof(wchar_t));
		wmemset(wcpName, 0, 128);

		//
		swprintf(wcpPath, _T("%s"), this->m_str_Lod_FW.cs_Path);

		//��lod�ļ����л�ȡ������
		this->Get_Lod_File_Name(wcpPath, wcpName);
		this->Get_Lod_File_Info(wcpName, &this->m_csLod_Product);

		//�޻�����Ϣ�ͱ�Fail
		if(this->m_csLod_Product.GetLength() <= 0)
		{
			this->m_csError_Code.Format(_T("ERROR, Lod file does not have module name!"));
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

bool CARM_OP_RDA::Load_Lod_File_Params(const wchar_t * wcp_File_Path)
{
	//����LOD�ļ����ԡ�#$MODE����ͷ�Ĳ�����Ϣ

	CString csData;
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;
	unsigned long ulFile_Size, i, ulRec_Quan;
	unsigned char ucrData[2];
	wchar_t wcrTemp[4];
	bool bFind_Key_Word, bGet_Params_Head;
	int iTemp;
	wchar_t wcrParams_Head[] = {_T("#$MODE=")};//����Ҫ��ȡ�Ĳ����Ĺؼ���

	/*
	#$mode=flsh_spi32m (#$mode=flsh_spi16m)
	#$sectormap=(16 x 4k, 57 x 64k, 96 x 4k)
	#$base=0x08000000
	#$spacesize=0x08000000
	#$XCV_MODEL=xcv_8809 
	#$PA_MODEL=pasw_rda6625 
	#$FLSH_MODEL=flsh_spi32m 
	#$FLASH_SIZE=0x00400000 
	#$RAM_SIZE=0x00200000 
	#$RAM_PHY_SIZE=0x00200000 
	#$CALIB_BASE=0x003FC000 
	#$FACT_SETTINGS_BASE=0x003FE000 
	#$CODE_BASE=0x00000000 
	#$USER_DATA_BASE=0x003C0000 
	#$USER_DATA_SIZE=0x0003C000 
	#$PM_MODEL=pmu_8809 
	*/

	try
	{
		//��LOD�ļ�
		hFile_Handle = ::CreateFile(wcp_File_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			this->m_csError_Code.Format(_T("FAIL, Load_Lod_File_Params, Open lod file failed, %s!"), wcp_File_Path);
			throw(0);
		}


		//��ȡlod�ļ���BYTE����lod�ļ�Size��
		ulFile_Size = ::GetFileSize(hFile_Handle, NULL);


		//������#��
		csData.Format(_T(""));
		bFind_Key_Word = false;
		bGet_Params_Head = false;

		for(i=0; i<ulFile_Size; i++)
		{
			memset(ucrData, 0, sizeof(ucrData));
			ulRec_Quan = 0;

			//���ļ�����
			if(this->READ_FILE(hFile_Handle, ucrData, 1, &ulRec_Quan, false) != RTN_SUCCESS)
			{
				this->m_csError_Code += _T("Err 1 load lod file params;");
				throw(0);
			}


			//û�ж�������
			if(ulRec_Quan < 1)
			{
				break;
			}


			if(ucrData[0] == '#')//�ǡ�#���ؼ���
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ������#������һ�������Ŀ�ʼ��������ȡ����������
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
						break;
					}
					else//����Ŀ������У����������
					{
						csData.Format(_T("#"));
					}
				}
				else//֮ǰû�з���#�ؼ��ʣ���ʼ�������
				{
					//��Ӹ��ַ�
					csData.Format(_T("#"));
					//�ҵ��˵�һ��#
					bFind_Key_Word = true;
				}
			}
			else if((ucrData[0] == '\r') || (ucrData[0] == '\n'))//�ǡ�\r����\n���ؼ��ʣ����������н���
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ���3���ؼ��ʴ��������������ȡ����������
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
						break;
					}
					else//����Ŀ������У����������
					{
						csData.Format(_T(""));
						bFind_Key_Word = false;
					}
				}
			}
			else if(ucrData[0] == '@')
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ���@������������ݶν������˳�
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
					}
				}

				break;//���֡�@�����˳�
			}
			else//�ǹؼ����ַ�����
			{
				if(bFind_Key_Word == true)//����������
				{
					//��Ӹ��ַ�
					this->m_cla_GFC.MultiByte_To_WideChar((char *)ucrData, wcrTemp, &iTemp);

					if(iTemp >= 1)
					{
						csData += wcrTemp[0];
					}
				}
			}
		}//for(i=0; i<ulFile_Size; i++

		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//����Ƿ�����������
		if(bGet_Params_Head == true)
		{
			wchar_t * wcpParams_Line = NULL;
			wchar_t * wcpHead = NULL;

			//�����ڴ�
			wcpParams_Line = (wchar_t *)calloc((csData.GetLength() + 8), sizeof(wchar_t));
			//�������������
			swprintf(wcpParams_Line, _T("%s"), csData);
			//��λ����������
			wcpHead = wcsstr(wcpParams_Line, wcrParams_Head);
			wcpHead += wcslen(wcrParams_Head);

			//���뷵�ص�����
			this->m_csLod_Product.Format(_T("%s"), wcpHead);

			//�ͷ��ڴ�
			if(wcpParams_Line != NULL)
			{
				free(wcpParams_Line);
				wcpParams_Line = NULL;
			}
		}
		else
		{
			this->m_csError_Code.Format(_T("FAIL, Load_Lod_File_Params, No flash mode data!"));
			throw(0);
		}
	}
	catch(...)
	{
		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		return false;
	}

	return true;
}
bool CARM_OP_RDA::Get_Lod_File_XCV_MODEL_Params(const wchar_t * wcp_File_Path)
{
	//����LOD�ļ����ԡ�#$MODE����ͷ�Ĳ�����Ϣ

	CString csData;
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;
	unsigned long ulFile_Size, i, ulRec_Quan;
	unsigned char ucrData[2];
	wchar_t wcrTemp[4];
	bool bFind_Key_Word, bGet_Params_Head;
	int iTemp;
	wchar_t wcrParams_Head[] = {_T("#$XCV_MODEL=")};//����Ҫ��ȡ�Ĳ����Ĺؼ���

	/*
	#$mode=flsh_spi32m (#$mode=flsh_spi16m)
	#$sectormap=(16 x 4k, 57 x 64k, 96 x 4k)
	#$base=0x08000000
	#$spacesize=0x08000000
	#$XCV_MODEL=xcv_8809 
	#$PA_MODEL=pasw_rda6625 
	#$FLSH_MODEL=flsh_spi32m 
	#$FLASH_SIZE=0x00400000 
	#$RAM_SIZE=0x00200000 
	#$RAM_PHY_SIZE=0x00200000 
	#$CALIB_BASE=0x003FC000 
	#$FACT_SETTINGS_BASE=0x003FE000 
	#$CODE_BASE=0x00000000 
	#$USER_DATA_BASE=0x003C0000 
	#$USER_DATA_SIZE=0x0003C000 
	#$PM_MODEL=pmu_8809 
	*/

	try
	{
		//��LOD�ļ�
		hFile_Handle = ::CreateFile(wcp_File_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			this->m_csError_Code.Format(_T("FAIL, Get_Lod_File_XCV_MODEL_Params, Open lod file failed, %s!"), wcp_File_Path);
			throw(0);
		}


		//��ȡlod�ļ���BYTE����lod�ļ�Size��
		ulFile_Size = ::GetFileSize(hFile_Handle, NULL);


		//������#��
		csData.Format(_T(""));
		bFind_Key_Word = false;
		bGet_Params_Head = false;

		for(i=0; i<ulFile_Size; i++)
		{
			memset(ucrData, 0, sizeof(ucrData));
			ulRec_Quan = 0;

			//���ļ�����
			if(this->READ_FILE(hFile_Handle, ucrData, 1, &ulRec_Quan, false) != RTN_SUCCESS)
			{
				this->m_csError_Code += _T("Err 2 load lod file params;");
				throw(0);
			}


			//û�ж�������
			if(ulRec_Quan < 1)
			{
				break;
			}


			if(ucrData[0] == '#')//�ǡ�#���ؼ���
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ������#������һ�������Ŀ�ʼ��������ȡ����������
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
						break;
					}
					else//����Ŀ������У����������
					{
						csData.Format(_T("#"));
					}
				}
				else//֮ǰû�з���#�ؼ��ʣ���ʼ�������
				{
					//��Ӹ��ַ�
					csData.Format(_T("#"));
					//�ҵ��˵�һ��#
					bFind_Key_Word = true;
				}
			}
			else if((ucrData[0] == '\r') || (ucrData[0] == '\n'))//�ǡ�\r����\n���ؼ��ʣ����������н���
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ���3���ؼ��ʴ��������������ȡ����������
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
						break;
					}
					else//����Ŀ������У����������
					{
						csData.Format(_T(""));
						bFind_Key_Word = false;
					}
				}
			}
			else if(ucrData[0] == '@')
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ���@������������ݶν������˳�
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
					}
				}

				break;//���֡�@�����˳�
			}
			else//�ǹؼ����ַ�����
			{
				if(bFind_Key_Word == true)//����������
				{
					//��Ӹ��ַ�
					this->m_cla_GFC.MultiByte_To_WideChar((char *)ucrData, wcrTemp, &iTemp);

					if(iTemp >= 1)
					{
						csData += wcrTemp[0];
					}
				}
			}
		}//for(i=0; i<ulFile_Size; i++

		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//����Ƿ�����������
		if(bGet_Params_Head == true)
		{
			wchar_t * wcpParams_Line = NULL;
			wchar_t * wcpHead = NULL;

			//�����ڴ�
			wcpParams_Line = (wchar_t *)calloc((csData.GetLength() + 8), sizeof(wchar_t));
			//�������������
			swprintf(wcpParams_Line, _T("%s"), csData);
			//��λ����������
			wcpHead = wcsstr(wcpParams_Line, wcrParams_Head);
			wcpHead += wcslen(wcrParams_Head);

			//���뷵�ص�����
			this->m_csLod_XCV_MODEL.Format(_T("%s"), wcpHead);

			//�ͷ��ڴ�
			if(wcpParams_Line != NULL)
			{
				free(wcpParams_Line);
				wcpParams_Line = NULL;
			}
		}
		else
		{
			this->m_csError_Code.Format(_T("FAIL, Get_Lod_File_XCV_MODEL_Params, No flash mode data!"));
			throw(0);
		}
	}
	catch(...)
	{
		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		return false;
	}

	return true;
}
void CARM_OP_RDA::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_RDA::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

bool CARM_OP_RDA::Get_Lod_File_FS_ROOT_BASE_Params(const wchar_t * wcp_File_Path,int* p_iFS_ROOT_BASE)
{
	//����LOD�ļ����ԡ�#$FS_ROOT_BASE����ͷ�Ĳ�����Ϣ

	CString csData;
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;
	unsigned long ulFile_Size, i, ulRec_Quan;
	unsigned char ucrData[2];
	wchar_t wcrTemp[4];
	bool bFind_Key_Word, bGet_Params_Head;
	int iTemp;
	wchar_t wcrParams_Head[] = {_T("#$FS_ROOT_BASE")};//����Ҫ��ȡ�Ĳ����Ĺؼ���

	/*
	#$FS_ROOT_BASE=0x390000
	#$FS_ROOT_SIZE=0x66000
	*/
	CString strFS_ROOT_BASE;

	try
	{
		//��LOD�ļ�
		hFile_Handle = ::CreateFile(wcp_File_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			this->m_csError_Code.Format(_T("FAIL, Get_Lod_File_FS_ROOT_BASE_Params, Open lod file failed, %s!"), wcp_File_Path);
			throw(0);
		}


		//��ȡlod�ļ���BYTE����lod�ļ�Size��
		ulFile_Size = ::GetFileSize(hFile_Handle, NULL);


		//������#��
		csData.Format(_T(""));
		bFind_Key_Word = false;
		bGet_Params_Head = false;

		for(i=0; i<ulFile_Size; i++)
		{
			memset(ucrData, 0, sizeof(ucrData));
			ulRec_Quan = 0;

			//���ļ�����
			if(this->READ_FILE(hFile_Handle, ucrData, 1, &ulRec_Quan, false) != RTN_SUCCESS)
			{
				this->m_csError_Code += _T("Err 2 load lod file params;");
				throw(0);
			}


			//û�ж�������
			if(ulRec_Quan < 1)
			{
				break;
			}


			if(ucrData[0] == '#')//�ǡ�#���ؼ���
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ������#������һ�������Ŀ�ʼ��������ȡ����������
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
						break;
					}
					else//����Ŀ������У����������
					{
						csData.Format(_T("#"));
					}
				}
				else//֮ǰû�з���#�ؼ��ʣ���ʼ�������
				{
					//��Ӹ��ַ�
					csData.Format(_T("#"));
					//�ҵ��˵�һ��#
					bFind_Key_Word = true;
				}
			}
			else if((ucrData[0] == '\r') || (ucrData[0] == '\n'))//�ǡ�\r����\n���ؼ��ʣ����������н���
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ���3���ؼ��ʴ��������������ȡ����������
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
						break;
					}
					else//����Ŀ������У����������
					{
						csData.Format(_T(""));
						bFind_Key_Word = false;
					}
				}
			}
			else if(ucrData[0] == '@')
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ���@������������ݶν������˳�
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
					}
				}

				break;//���֡�@�����˳�
			}
			else//�ǹؼ����ַ�����
			{
				if(bFind_Key_Word == true)//����������
				{
					//��Ӹ��ַ�
					this->m_cla_GFC.MultiByte_To_WideChar((char *)ucrData, wcrTemp, &iTemp);

					if(iTemp >= 1)
					{
						csData += wcrTemp[0];
					}
				}
			}
		}//for(i=0; i<ulFile_Size; i++

		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
			hFile_Handle = INVALID_HANDLE_VALUE;
		}

		//����Ƿ�����������
		if(bGet_Params_Head == true)
		{
			wchar_t * wcpParams_Line = NULL;
			wchar_t * wcpHead = NULL;

			//�����ڴ�
			wcpParams_Line = (wchar_t *)calloc((csData.GetLength() + 8), sizeof(wchar_t));
			//�������������
			swprintf(wcpParams_Line, _T("%s"), csData);
			//��λ����������
			wcpHead = wcsstr(wcpParams_Line, wcrParams_Head);
			wcpHead += wcslen(wcrParams_Head);

			//���뷵�ص�����
			strFS_ROOT_BASE.Format(_T("%s"), wcpHead);
			strFS_ROOT_BASE.MakeUpper();
			strFS_ROOT_BASE.Replace(_T("=0X"),_T(""));
			ULONG lValue;//4 BYTE
			char hExData[16]={0};
			int iiTemp=0;
			this->m_cla_GFC.WideChar_To_MultiByte(strFS_ROOT_BASE, hExData, &iiTemp);
			sscanf(hExData,"%X",&lValue);
			m_iFS_ROOT_BASE = (UINT32)lValue;

			*p_iFS_ROOT_BASE = m_iFS_ROOT_BASE;



			//�ͷ��ڴ�
			if(wcpParams_Line != NULL)
			{
				free(wcpParams_Line);
				wcpParams_Line = NULL;
			}
		}
		else
		{

			m_iFS_ROOT_BASE = 0;
			this->m_csError_Code.Format(_T("FAIL, Get_Lod_File_FS_ROOT_BASE_Params, No flash mode data!"));
			throw(0);
		}
	}
	catch(...)
	{
		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		return true;
	}

	return true;
}
bool CARM_OP_RDA::Get_Lod_File_BOOT_SECTOR_Params(const wchar_t * wcp_File_Path,int* p_iBOOT_SECTOR)
{
	//����LOD�ļ����ԡ�#$BOOT_SECTOR����ͷ�Ĳ�����Ϣ

	CString csData;
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;
	unsigned long ulFile_Size, i, ulRec_Quan;
	unsigned char ucrData[2];
	wchar_t wcrTemp[4];
	bool bFind_Key_Word, bGet_Params_Head;
	int iTemp;
	wchar_t wcrParams_Head[] = {_T("#$BOOT_SECTOR")};//����Ҫ��ȡ�Ĳ����Ĺؼ���

	
	CString strBOOT_SECTOR;

	try
	{
		//��LOD�ļ�
		hFile_Handle = ::CreateFile(wcp_File_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			this->m_csError_Code.Format(_T("FAIL, Get_Lod_File_BOOT_SECTOR_Params, Open lod file failed, %s!"), wcp_File_Path);
			throw(0);
		}


		//��ȡlod�ļ���BYTE����lod�ļ�Size��
		ulFile_Size = ::GetFileSize(hFile_Handle, NULL);


		//������#��
		csData.Format(_T(""));
		bFind_Key_Word = false;
		bGet_Params_Head = false;

		for(i=0; i<ulFile_Size; i++)
		{
			memset(ucrData, 0, sizeof(ucrData));
			ulRec_Quan = 0;

			//���ļ�����
			if(this->READ_FILE(hFile_Handle, ucrData, 1, &ulRec_Quan, false) != RTN_SUCCESS)
			{
				this->m_csError_Code += _T("Err 2 load lod file params;");
				throw(0);
			}


			//û�ж�������
			if(ulRec_Quan < 1)
			{
				break;
			}


			if(ucrData[0] == '#')//�ǡ�#���ؼ���
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ������#������һ�������Ŀ�ʼ��������ȡ����������
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
						break;
					}
					else//����Ŀ������У����������
					{
						csData.Format(_T("#"));
					}
				}
				else//֮ǰû�з���#�ؼ��ʣ���ʼ�������
				{
					//��Ӹ��ַ�
					csData.Format(_T("#"));
					//�ҵ��˵�һ��#
					bFind_Key_Word = true;
				}
			}
			else if((ucrData[0] == '\r') || (ucrData[0] == '\n'))//�ǡ�\r����\n���ؼ��ʣ����������н���
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ���3���ؼ��ʴ��������������ȡ����������
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
						break;
					}
					else//����Ŀ������У����������
					{
						csData.Format(_T(""));
						bFind_Key_Word = false;
					}
				}
			}
			else if(ucrData[0] == '@')
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ���@������������ݶν������˳�
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
					}
				}

				break;//���֡�@�����˳�
			}
			else//�ǹؼ����ַ�����
			{
				if(bFind_Key_Word == true)//����������
				{
					//��Ӹ��ַ�
					this->m_cla_GFC.MultiByte_To_WideChar((char *)ucrData, wcrTemp, &iTemp);

					if(iTemp >= 1)
					{
						csData += wcrTemp[0];
					}
				}
			}
		}//for(i=0; i<ulFile_Size; i++

		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
			hFile_Handle = INVALID_HANDLE_VALUE;
		}

		//����Ƿ�����������
		if(bGet_Params_Head == true)
		{
			wchar_t * wcpParams_Line = NULL;
			wchar_t * wcpHead = NULL;

			//�����ڴ�
			wcpParams_Line = (wchar_t *)calloc((csData.GetLength() + 8), sizeof(wchar_t));
			//�������������
			swprintf(wcpParams_Line, _T("%s"), csData);
			//��λ����������
			wcpHead = wcsstr(wcpParams_Line, wcrParams_Head);
			wcpHead += wcslen(wcrParams_Head);

			//���뷵�ص�����
			strBOOT_SECTOR.Format(_T("%s"), wcpHead);
			strBOOT_SECTOR.MakeUpper();
			strBOOT_SECTOR.Replace(_T("=0X"),_T(""));
			ULONG lValue;//4 BYTE
			char hExData[16]={0};
			int iiTemp=0;
			this->m_cla_GFC.WideChar_To_MultiByte(strBOOT_SECTOR, hExData, &iiTemp);
			sscanf(hExData,"%X",&lValue);
			m_iBOOT_SECTOR = (UINT32)lValue;

			*p_iBOOT_SECTOR= m_iBOOT_SECTOR;



			//�ͷ��ڴ�
			if(wcpParams_Line != NULL)
			{
				free(wcpParams_Line);
				wcpParams_Line = NULL;
			}
		}
		else
		{

			m_iFS_ROOT_BASE = 0;
			this->m_csError_Code.Format(_T("FAIL, Get_Lod_File_BOOT_SECTOR_Params, No flash mode data!"));
			throw(0);
		}
	}
	catch(...)
	{
		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		return true;
	}

	return true;
}



bool CARM_OP_RDA::Get_Lod_File_FS_ROOT_SIZE_Params(const wchar_t * wcp_File_Path)
{
	//����LOD�ļ����ԡ�#$FS_ROOT_SIZE����ͷ�Ĳ�����Ϣ

	CString csData;
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;
	unsigned long ulFile_Size, i, ulRec_Quan;
	unsigned char ucrData[2];
	wchar_t wcrTemp[4];
	bool bFind_Key_Word, bGet_Params_Head;
	int iTemp;
	wchar_t wcrParams_Head[] = {_T("#$FS_ROOT_SIZE")};//����Ҫ��ȡ�Ĳ����Ĺؼ���

	/*
	#$FS_ROOT_BASE=0x390000
	#$FS_ROOT_SIZE=0x66000
	*/
	CString strFS_ROOT_SIZE;

	try
	{
		//��LOD�ļ�
		hFile_Handle = ::CreateFile(wcp_File_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			this->m_csError_Code.Format(_T("FAIL, Get_Lod_File_FS_ROOT_SIZE_Params, Open lod file failed, %s!"), wcp_File_Path);
			throw(0);
		}


		//��ȡlod�ļ���BYTE����lod�ļ�Size��
		ulFile_Size = ::GetFileSize(hFile_Handle, NULL);


		//������#��
		csData.Format(_T(""));
		bFind_Key_Word = false;
		bGet_Params_Head = false;

		for(i=0; i<ulFile_Size; i++)
		{
			memset(ucrData, 0, sizeof(ucrData));
			ulRec_Quan = 0;

			//���ļ�����
			if(this->READ_FILE(hFile_Handle, ucrData, 1, &ulRec_Quan, false) != RTN_SUCCESS)
			{
				this->m_csError_Code += _T("Err 2 load lod file params;");
				throw(0);
			}


			//û�ж�������
			if(ulRec_Quan < 1)
			{
				break;
			}


			if(ucrData[0] == '#')//�ǡ�#���ؼ���
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ������#������һ�������Ŀ�ʼ��������ȡ����������
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
						break;
					}
					else//����Ŀ������У����������
					{
						csData.Format(_T("#"));
					}
				}
				else//֮ǰû�з���#�ؼ��ʣ���ʼ�������
				{
					//��Ӹ��ַ�
					csData.Format(_T("#"));
					//�ҵ��˵�һ��#
					bFind_Key_Word = true;
				}
			}
			else if((ucrData[0] == '\r') || (ucrData[0] == '\n'))//�ǡ�\r����\n���ؼ��ʣ����������н���
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ���3���ؼ��ʴ��������������ȡ����������
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
						break;
					}
					else//����Ŀ������У����������
					{
						csData.Format(_T(""));
						bFind_Key_Word = false;
					}
				}
			}
			else if(ucrData[0] == '@')
			{
				if(bFind_Key_Word == true)//֮ǰ�ѷ���һ��#�ؼ��ʣ���@������������ݶν������˳�
				{
					csData.MakeUpper();

					if(wcsstr(csData, wcrParams_Head) != NULL)//�ҵ�Ŀ�������
					{
						bGet_Params_Head = true;
					}
				}

				break;//���֡�@�����˳�
			}
			else//�ǹؼ����ַ�����
			{
				if(bFind_Key_Word == true)//����������
				{
					//��Ӹ��ַ�
					this->m_cla_GFC.MultiByte_To_WideChar((char *)ucrData, wcrTemp, &iTemp);

					if(iTemp >= 1)
					{
						csData += wcrTemp[0];
					}
				}
			}
		}//for(i=0; i<ulFile_Size; i++

		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
			hFile_Handle = INVALID_HANDLE_VALUE;


		}

		//����Ƿ�����������
		if(bGet_Params_Head == true)
		{
			wchar_t * wcpParams_Line = NULL;
			wchar_t * wcpHead = NULL;

			//�����ڴ�
			wcpParams_Line = (wchar_t *)calloc((csData.GetLength() + 8), sizeof(wchar_t));
			//�������������
			swprintf(wcpParams_Line, _T("%s"), csData);
			//��λ����������
			wcpHead = wcsstr(wcpParams_Line, wcrParams_Head);
			wcpHead += wcslen(wcrParams_Head);

			//���뷵�ص�����
			strFS_ROOT_SIZE.Format(_T("%s"), wcpHead);
			//���뷵�ص�����
			strFS_ROOT_SIZE.Format(_T("%s"), wcpHead);
			strFS_ROOT_SIZE.MakeUpper();
			strFS_ROOT_SIZE.Replace(_T("=0X"),_T(""));
			ULONG lValue;//4 BYTE
			char hExData[16]={0};
			int iiTemp=0;
			this->m_cla_GFC.WideChar_To_MultiByte(strFS_ROOT_SIZE, hExData, &iiTemp);
			sscanf(hExData,"%X",&lValue);
			m_iFS_ROOT_SIZE = (UINT32)lValue;

			//�ͷ��ڴ�
			if(wcpParams_Line != NULL)
			{
				free(wcpParams_Line);
				wcpParams_Line = NULL;
			}
		}
		else
		{
			this->m_csError_Code.Format(_T("FAIL, Get_Lod_File_FS_ROOT_SIZE_Params, No flash mode data!"));
			throw(0);
		}
	}
	catch(...)
	{
		//�ر�lod�ļ�
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		return true;
	}

	return true;
}
