#include "StdAfx.h"
#include "Com_Port.h"

/*
Ver1.04 (2014-5-12)

2014-3-12��xjli
1.�����һЩRead�����ġ�����ע�͡����Ա��Read����֮��Ĺ�������

2014-3-17��xjli
1.����ˡ�Query_Tgt_CHAR���͡�GET_RX_PKT_BUFF_DATA��������

2014-3-29��xjli
1.����ˡ�Clear_Tx_Buffer������GET_BUFF_BYTE������DEL_BUFF_BYTE���͡�Empty_Com_And_Rx_Buff��������

2014-5-12, xjli
1.����AT�ġ�Write�������ɡ�\r����β��Ϊ��\r\n����β��

2014-6-11, xjli
1.������DTR��RTS�ܽŵı������ơ�
*/

CCom_Port::CCom_Port(void)
: m_bOpen(false)
, m_iPort(0)
, m_iBaudRate(0)
, m_csError_Info(_T(""))
, m_dwDTR_CTRL(DTR_CONTROL_ENABLE)
, m_dwRTS_CTRL(RTS_CONTROL_ENABLE)
{
	m_bOpen = false;
	m_hCommHandle = INVALID_HANDLE_VALUE;
	m_dwDTR_CTRL = DTR_CONTROL_ENABLE;
	m_dwRTS_CTRL = RTS_CONTROL_ENABLE;

	m_str_Com_Rx_Buff.ucp_Buff = NULL;
	m_str_Com_Rx_Buff.ul_Total_Size = 0;
	m_str_Com_Rx_Buff.ul_Data_Length = 0;
}

CCom_Port::~CCom_Port(void)
{
	if(m_str_Com_Rx_Buff.ucp_Buff != NULL)
	{
		free(m_str_Com_Rx_Buff.ucp_Buff);

		m_str_Com_Rx_Buff.ucp_Buff = NULL;
		m_str_Com_Rx_Buff.ul_Total_Size = 0;
		m_str_Com_Rx_Buff.ul_Data_Length = 0;
	}
}

bool CCom_Port::Port_Open(int i_Read_TimeOut)
{
	if(this->m_bOpen == true)
	{
		return true;
	}

	CString port;
	port.Format(_T("\\\\.\\COM%d"), this->m_iPort);

	this->m_hCommHandle = CreateFile(port,
		GENERIC_READ | GENERIC_WRITE,	//�������д
		NULL,							//��ռ��ʽ don't share port
		NULL,							//SecurityAttributes - none in Win95
		OPEN_EXISTING,					//�򿪶����Ǵ��� how to create
		0,								//ͬ����ʽ file attributes
		NULL							// handle of file with attributes to copy
		);

	if(this->m_hCommHandle == INVALID_HANDLE_VALUE)//��Ч���
	{
		this->m_csError_Info.Format(_T("FAIL, Open com port %d failed!"), this->m_iPort);
		return false;
	}

	//��Ч���
	this->CommProps.wPacketLength = sizeof(COMMPROP);
	GetCommProperties(this->m_hCommHandle, &this->CommProps);

	//SERIAL_SET_QUEUE_SIZE
	SetupComm(this->m_hCommHandle, 4096, 4096);


	//����Mask
	SetCommMask(this->m_hCommHandle, EV_RXCHAR|EV_ERR|EV_BREAK);


	//DCB
	GetCommState(this->m_hCommHandle, &this->CommDcb);

	/*
	this->CommDcb.DCBlength = sizeof(DCB);
	this->CommDcb.fBinary = TRUE;
	this->CommDcb.fParity = TRUE;
	this->CommDcb.fOutxCtsFlow = TRUE;
	this->CommDcb.fOutxDsrFlow = FALSE;
	this->CommDcb.fDtrControl = DTR_CONTROL_ENABLE;
	this->CommDcb.fDsrSensitivity = FALSE;
	this->CommDcb.fTXContinueOnXoff = TRUE;
	this->CommDcb.fOutX = FALSE;
	this->CommDcb.fInX = FALSE;
	this->CommDcb.fErrorChar = FALSE;
	this->CommDcb.fNull = FALSE;
	this->CommDcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	this->CommDcb.fAbortOnError = FALSE;
	this->CommDcb.ByteSize	= 8;
	this->CommDcb.Parity	= NOPARITY;
	this->CommDcb.StopBits	= ONESTOPBIT;
	this->CommDcb.BaudRate = this->m_iBaudRate;
	this->CommDcb.EvtChar = 0x7E;
	*/

	this->CommDcb.DCBlength = sizeof(DCB);
	this->CommDcb.BaudRate = this->m_iBaudRate;// commDeviceInfo.clockRates[1].baudSetting[COMM_9600_BAUD_INDEX]; // 9600

	this->CommDcb.ByteSize = 8;
	this->CommDcb.fParity  = FALSE;
	this->CommDcb.StopBits = ONESTOPBIT;

	this->CommDcb.fBinary           = TRUE;
	this->CommDcb.fOutxCtsFlow      = FALSE;
	this->CommDcb.fOutxDsrFlow      = FALSE;

	this->CommDcb.fDtrControl = this->m_dwDTR_CTRL;

	EscapeCommFunction(this->m_hCommHandle, SETDTR);

	this->CommDcb.fRtsControl = this->m_dwRTS_CTRL;

	EscapeCommFunction(this->m_hCommHandle, SETRTS);

	this->CommDcb.fDsrSensitivity   = FALSE;
	this->CommDcb.fTXContinueOnXoff = FALSE;
	this->CommDcb.fOutX             = FALSE;
	this->CommDcb.fInX              = FALSE;
	this->CommDcb.fErrorChar        = FALSE;
	this->CommDcb.fNull             = FALSE;
	this->CommDcb.fAbortOnError     = FALSE;

	//Ϊ����ͨƽ̨�����
//	this->CommDcb.EvtChar = 0x7e;//�¼��ַ��������յ����ַ���ʱ�򣬻����һ���¼���
	this->CommDcb.XonLim = 0;
	this->CommDcb.XoffLim = 0;


	//SERIAL_SET_RTS
	//SERIAL_SET_DTR
	//SERIAL_SET_LINE_CONTROL
	//SERIAL_SET_CHAR
	//SERIAL_SET_HANDFLOW
	if(SetCommState(this->m_hCommHandle, &this->CommDcb) == FALSE)
	{
		this->Port_Close();
		this->m_csError_Info.Format(_T("FAIL, Could not set comm state!"));
		return false;
	}



	//�������ʱ
	this->commTimeOuts.ReadIntervalTimeout         = -1;
	//��ʱ��ϵ��
	this->commTimeOuts.ReadTotalTimeoutMultiplier  = -1;
	//��ʱ�䳣��
	this->commTimeOuts.ReadTotalTimeoutConstant    = i_Read_TimeOut;//500;
	//дʱ��ϵ��
	this->commTimeOuts.WriteTotalTimeoutMultiplier = 1;
	//дʱ�䳣��
	this->commTimeOuts.WriteTotalTimeoutConstant   = 500;

	//SERIAL_SET_TIMEOUTS
	if(SetCommTimeouts(this->m_hCommHandle, &this->commTimeOuts) == FALSE)
	{
		this->Port_Close();
		this->m_csError_Info.Format(_T("FAIL, Could not set comm timeout!"));
		return false;
	}


	this->Clear_Tx_Buffer();
	this->Clear_Rx_Buffer();


	//�����Ŵ��ڽ��յ������ݵġ�����BUFF���ڴ�
	this->m_cla_GFC.PKT_BUFF_CALLOC(&this->m_str_Com_Rx_Buff, 6144);

	this->m_bOpen = true;
	return true;
}

void CCom_Port::Port_Close(void)
{
	if((this->m_hCommHandle != INVALID_HANDLE_VALUE) || (this->m_bOpen == true))
	{
		CloseHandle(this->m_hCommHandle);
		this->m_hCommHandle = INVALID_HANDLE_VALUE;

		this->m_cla_GFC.PKT_BUFF_FREE(&this->m_str_Com_Rx_Buff);
		this->m_bOpen = false;
	}
}

bool CCom_Port::Write(const wchar_t * wcpSend_Data)
{
	//����wchar_t���͵ġ�ָ�����ݡ���������ͨ��ATָ��

	char * cpWrite_Buff = NULL;
	DWORD dwdTotalBytesSent = 0;
	int iTemp;
	CString csWrite;

	//���ַ�����ָ���Ҫ�á�\r\n����β
	csWrite.Format(_T("%s\r\n"), wcpSend_Data);

	//�����ڴ�
	iTemp = csWrite.GetLength() * 2 + 8;
	this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpWrite_Buff, iTemp);

	//��������
	this->m_cla_GFC.WideChar_To_MultiByte(csWrite, cpWrite_Buff, &iTemp);

	//д
	iTemp = WriteFile(this->m_hCommHandle, cpWrite_Buff, strlen(cpWrite_Buff), &dwdTotalBytesSent, NULL);

	//�ͷ��ڴ�
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpWrite_Buff);

	if(iTemp == 0)
	{
		this->m_csError_Info.Format(_T("WriteFile Error = %d"), GetLastError());
		return false;
	}
	else
	{
		return true;
	}
}

bool CCom_Port::Read(wchar_t * wcpRead_Data, int * ipRec_Numb, const int iRec_TimeOut)
{
	//��ȡwchar_t���͵ġ�ָ�����ݡ���������ͨ��ATָ��
	//�����COM Rx Buff�е�������������ʱ������һ�Ρ���COM�������ŵ�wcpRead_Data�С�

	char * cpRead_Buff = NULL;
	DWORD dw_Bytes_Rec = 0;
	DWORD dw_Numb_Read = 0;
	DWORD dw_Last_Read = 0;
	int iResult;
	time_t start, end;
	double dTimeOut = 2.0;

	try
	{
		//��ʼ��ʱ
		time(&start);

		do
		{
			time(&end);
			if(difftime(end, start) >= dTimeOut)
			{
				throw(0);//��ʱ
			}

			//���COM��BUFF���ж�������
			dw_Numb_Read = this->Get_Com_Rx_Buff_Valid();

			if(dw_Numb_Read > 0)//������
			{
				if(dw_Numb_Read == dw_Last_Read)//��״̬������û�����ӣ��Ϳ��Զ�������
				{
					//��ʽ�ɶ�����������BUFF�ߴ�
					if(dw_Numb_Read > iRec_TimeOut)
					{
						dw_Numb_Read = iRec_TimeOut;
					}

					//�����ڴ�
					this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, (dw_Numb_Read + 8));

					memset(cpRead_Buff, 0, (dw_Numb_Read + 8));

					//
					iResult = ReadFile(this->m_hCommHandle, cpRead_Buff, dw_Numb_Read, &dw_Bytes_Rec, NULL);

					if((dw_Bytes_Rec <= 0) || (iResult == FALSE))
					{
						throw(0);//��ʧ��
					}
					else
					{
						*(cpRead_Buff + dw_Bytes_Rec) = '\0';

						//���ڶ�����������Ҫת��Ϊwchar_t��˫�ַ��������ڽ�����ʾ
						this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpRead_Data, ipRec_Numb);

						//�ͷ��ڴ�
						this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

						*ipRec_Numb = *ipRec_Numb - 1;//ȥ����ֹλ
						return true;//�ɹ�
					}
				}//if(dw_Numb_Read == dw_Last_Read
				else//Ϊ��ֹ�����������ݣ���ֹ���ݶ��ѣ�������һ������������ȷ���Ƿ������
				{
					dw_Last_Read = dw_Numb_Read;
					Sleep(50);
					continue;
				}
			}//if(dw_Numb_Read > 0
			else
			{
				Sleep(50);
				continue;
			}
		}
		while(true);
	}
	catch(...)
	{
		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	return false;
}

bool CCom_Port::Query(const wchar_t * wcpSend_Data, wchar_t * wcpRead_Data, int * ipRec_Bits, const int iRec_TimeOut)
{
	if(this->Write(wcpSend_Data))
	{
		return this->Read(wcpRead_Data, ipRec_Bits, iRec_TimeOut);
	}
	else
	{
		return false;
	}
}

bool CCom_Port::Write_Byte(char * cp_Send_Data, unsigned long ul_Data_Quan)
{
	//���͵�char���ݣ������ͨFW����

	DWORD dwdTotalBytesSent = 0;
	int iRtn;

	//д
	iRtn = WriteFile(this->m_hCommHandle, cp_Send_Data, ul_Data_Quan, &dwdTotalBytesSent, NULL);

	if(iRtn == 0)//If the function fails, the return value is zero
	{
		this->m_csError_Info.Format(_T("FATL, WriteFile Error = %d"), GetLastError());
		return false;
	}
	else
	{
		return true;
	}
}

bool CCom_Port::Read_Byte(char * cp_Read_Buff, unsigned long ul_Expect_Read_Bytes, unsigned long * ulp_Real_Rec_Bytes)
{
	//��ȡ��char���ݣ������ͨFW����
	//���������������� Expect_Read_Bytes����Ҫ������һ�Ρ���COM������

	DWORD dw_Bytes_Rec = 0;
	int iResult;

	//������ǰ���ټ��BUFF�����������������պ��������й涨����������ȡ��
	//��ΪBUFF���������᲻�������п��ܽ���ȡBUFF�ű�������ڴ������
//	DWORD dw_Numb_Read = this->Get_Rx_Buff_Valid();

	//��
	iResult = ReadFile(this->m_hCommHandle, cp_Read_Buff, ul_Expect_Read_Bytes, &dw_Bytes_Rec, NULL);

	if(iResult == 0)
	{
		*ulp_Real_Rec_Bytes = 0;
		this->m_csError_Info.Format(_T("FAIL, ReadFile Error = %d"), GetLastError());
		return false;
	}

	*ulp_Real_Rec_Bytes = dw_Bytes_Rec;//ʵ�ʶ���λ��
	return true;
}

void CCom_Port::Clear_Rx_Buffer(void)
{
	//���Ӳ��COM�ڵ�RX_BUFF����
	PurgeComm(this->m_hCommHandle, (PURGE_RXABORT | PURGE_RXCLEAR));
}

void CCom_Port::Clear_Tx_Buffer(void)
{
	//���Ӳ��COM�ڵ�TX_BUFF����
	PurgeComm(this->m_hCommHandle, (PURGE_TXCLEAR | PURGE_TXABORT));
}

unsigned long CCom_Port::Get_Com_Rx_Buff_Valid(void)
{
	//���COM�ڵ�RX�������ж������ݣ����Զ�ȡ��

	COMSTAT ComStat;
	DWORD dwErrorFlags;

	ComStat.cbInQue = 0;

	if(!ClearCommError(this->m_hCommHandle, &dwErrorFlags, &ComStat))
	{
		return 0;
	}

	if(dwErrorFlags != 0)//�б����ComStat.cbInQue��ֵ�ܻ���
	{
		return 0;
	}

	return ComStat.cbInQue;
}

bool CCom_Port::Send_Byte_Data(const wchar_t * wcp_Send_Data, int i_Numb)
{
	//���ڷ���wchar_t���͵ĵ�byte��0x00FF�����ݣ�������ֱ���ǿ���ָ��
	char * cpWrite_Buff = NULL;
	DWORD dwdTotalBytesSent = 0;
	int i;

	PurgeComm(this->m_hCommHandle, (PURGE_TXCLEAR | PURGE_RXCLEAR));

	//�����ڴ�
	this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpWrite_Buff, (i_Numb + 8));

	//�������ݣ���������һ�㲻������ʾ��ֻ����ֵ����
	for(i=0; i<i_Numb; i++)
	{
		*(cpWrite_Buff + i) = *(wcp_Send_Data + i) & 0x00FF;
	}
	*(cpWrite_Buff + i) = '\0';//��ֹλ

	//д
	i = WriteFile(this->m_hCommHandle, cpWrite_Buff, i_Numb, &dwdTotalBytesSent, NULL);

	//�ͷ��ڴ�
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpWrite_Buff);

	if(i == 0)
	{
		this->m_csError_Info.Format(_T("WriteFile Error = %d"), GetLastError());
		return false;
	}
	else
	{
		return true;
	}
}

bool CCom_Port::Read_Byte_Data(wchar_t * wcp_Read_Data, int * ip_Rec_Bytes, const int i_Rec_TimeOut)
{
	//���ڶ�ȡwchar_t���͵ĵ�byte��0x00FF�����ݣ�������ֱ���ǿ���ָ��

	char * cpRead_Buff = NULL;
	DWORD dwdBytesReceived = 0;
	int iResult, i;

	//�����ڴ�
	this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, (i_Rec_TimeOut + 8));

	memset(cpRead_Buff, 0, (i_Rec_TimeOut + 8));

	//��
	iResult = ReadFile(this->m_hCommHandle, cpRead_Buff, (DWORD)i_Rec_TimeOut, &dwdBytesReceived, NULL);

	//�������ݣ��Ѷ����ĵ��ֽ����ݼ���˫�ֽ�BUFF�С���������һ�㲻������ʾ��ֻ����ֵ����
	for(i=0; i<dwdBytesReceived; i++)
	{
		*(wcp_Read_Data + i) = *(cpRead_Buff + i) & 0x00FF;
	}
	*(wcp_Read_Data + i) = '\0';//��ֹλ

	//�ͷ��ڴ�
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	if(iResult == 0)
	{
		*ip_Rec_Bytes = 0;
		this->m_csError_Info.Format(_T("FAIL, Read_Byte_Data, ReadFile Error = %d"), GetLastError());
		return false;
	}

	*ip_Rec_Bytes = dwdBytesReceived;//ʵ�ʶ���λ��
	return true;
}

bool CCom_Port::Read_Byte_Sync_V1(void)
{
	//ͬ��ģʽ�£�����һ�Ρ���COM�Ĳ���������COM�ڣ�����һ�ζ��������ݲ�����PKT_BUFF��ʽ�ġ���COM Rx Buff���С�
	//�Ȼ���COM_RX���ж������ݣ�������������һ�ζ�������

	unsigned long ulExp_Read_Com, ulValid_Quan;
	unsigned char * ucpData = NULL;	//�����ݴӡ�����BUFF����ȡ���ŵġ���ʱ�ڴ桱

	try
	{
		//���COM Rx Buff�е�������
		ulExp_Read_Com = this->Get_Com_Rx_Buff_Valid();

		if(ulExp_Read_Com <= 0)//����BUFF�������ݿɶ�
		{
			return true;
		}

		//�������������ݱ��ȡ
		//���䡰���������ݡ��ġ���ʱ�ڴ桱
		this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpData, (ulExp_Read_Com + 8));

		//����������
		//ע�⣬��������������С�����BUFF������ǰһ������һ��ʵ��������������������
		//�����еġ�������������ʱ���ڷ����仯����ǰ̽����������ܿ��ܹ��˼����벻׼��
		ulValid_Quan = 0;
		this->Read_Byte((char *)ucpData, ulExp_Read_Com, &ulValid_Quan);

		if(ulExp_Read_Com != ulValid_Quan)
		{
			this->m_csError_Info.Format(_T("ERROR, Read_Byte_Sync, Read byte fail, E = %d, R = %d!"), ulExp_Read_Com, ulValid_Quan);
			throw(0);
		}

		if(ulValid_Quan <= 0)
		{
			this->m_csError_Info.Format(_T("FAIL, Read_Byte_Sync, COM port read byte error!"));
			throw(0);
		}

		//��ӵ�COM BUFF��
		if(this->m_cla_GFC.PKT_BUFF_ADD_DATA(&this->m_str_Com_Rx_Buff, ucpData, ulValid_Quan) == false)
		{
			this->m_csError_Info.Format(_T("FAIL, Read_Byte_Sync, COM buff add byte error!"));
			throw(0);
		}
	}
	catch(...)
	{
		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpData);
		return false;
	}

	//�ͷ��ڴ�
	this->m_cla_GFC.BUFF_BYTE_FREE(&ucpData);
	return true;
}

bool CCom_Port::Read_Byte_Sync_V2(unsigned long ul_Exp_Rec_Byte_Quan, int i_TimeOut)
{
	//ͬ��ģʽ�£��ڳ�ʱʱ���ڣ���ȡ������byte���������ݡ���
	//���ȼ��COM_RX���ж������ݡ�

	time_t t_Start, t_End;
	unsigned long ulActual_Valid_Quan;	//����ʵ�ʶ���byte����
	unsigned long ulExp_Read_Quan;		//������������byte����
	unsigned long ulRec_Total_Quan = 0;	//����˵��byte����ͳ��
	unsigned char * ucpData = NULL;		//�����ݴӡ�����BUFF����ȡ���ŵġ���ʱ�ڴ桱


	try
	{
		//��ʱ��ʼ
		time(&t_Start);

		//ѭ������ָ�����ȡ��Ӧ
		do
		{
			//Ԥ��
			ulExp_Read_Quan = this->Get_Com_Rx_Buff_Valid();
			//
			if(ulExp_Read_Quan <= 0)
			{
				ulExp_Read_Quan = 1;
			}


			//���ͷ��ڴ�
			this->m_cla_GFC.BUFF_BYTE_FREE(&ucpData);
			//���䡰���������ݡ��ġ���ʱ�ڴ桱
			this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpData, (ulExp_Read_Quan + 8));


			//������
			ulActual_Valid_Quan = 0;
			//
			if(this->Read_Byte((char *)ucpData, ulExp_Read_Quan, &ulActual_Valid_Quan) == true)
			{
				//��������ͳ��
				ulRec_Total_Quan += ulActual_Valid_Quan;

				//�������ݣ������Rx buff��
				if(ulActual_Valid_Quan > 0)
				{
					//��ӵ�COM BUFF��
					if(this->m_cla_GFC.PKT_BUFF_ADD_DATA(&this->m_str_Com_Rx_Buff, ucpData, ulActual_Valid_Quan) == false)
					{
						this->m_csError_Info.Format(_T("FAIL, Read_Byte_Sync_V2, COM buff add byte error!"));
						throw(0);
					}
				}
			}


			//��ʱ���
			time(&t_End);
			//
			if(difftime(t_End, t_Start) >= i_TimeOut)
			{
				this->m_csError_Info.Format(_T("FAIL, Read_Byte_Sync_V2 timeout"));
				throw(0);
			}
		}
		while(ulRec_Total_Quan < ul_Exp_Rec_Byte_Quan);



		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpData);
	}
	catch(...)
	{
		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpData);

		return false;
	}

	return true;
}

bool CCom_Port::Query_Tgt_CHAR(const wchar_t * wcp_Send_Data, const wchar_t * wcp_Tgt_Data, int i_TimeOut)
{
	//��ԡ������ַ�����ͨ�ţ�����ָ��󣬼���Ƿ���������Ӧ�𣬳�ʱ�㱨fail
	//��Ҫ�ѽ���ʹ�õ�wchar_tת��Ϊ����ͨ���õ�char���˶�

	time_t t_Start, t_End;
	char * cpTgt = NULL;
	int iTemp;

	 wchar_t wcp_Data[500];

	try
	{
		//�ڿ�ʼ������ǰ�������Rx Buff
		this->Empty_Com_And_Rx_Buff();

		//�����ڴ�
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpTgt, (wcslen(wcp_Tgt_Data) * 2 + 16));

		//ת��Ŀ���ַ�
		this->m_cla_GFC.WideChar_To_MultiByte(wcp_Tgt_Data, cpTgt, &iTemp);

		//��ʼ��ʱ
		time(&t_Start);

		//��ָ��
		this->Write(wcp_Send_Data);

		do
		{
			//��ָ��
//			this->Write(wcp_Send_Data);

			Sleep(5);

			//��������buff����
			this->Read_Byte_Sync_V1();

			//�ӡ���buff���к˶�Ŀ������
			if(strstr((char *)this->m_str_Com_Rx_Buff.ucp_Buff, cpTgt) != NULL)
			{
				break;
			}
			else
			{
				int len;
				this->m_cla_GFC.MultiByte_To_WideChar((char *)this->m_str_Com_Rx_Buff.ucp_Buff,wcp_Data,&len);
				//AfxMessageBox(wcp_Data);

			}

			//��ʱ���
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)//��ʱ����fail
			{
				this->m_csError_Info.Format(_T("FAIL, Query_Tgt_CHAR, Time out!"));
				throw(0);
			}
		}
		while(true);
	}
	catch(...)
	{
		//�ͷ��ڴ�
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpTgt);

		return false;
	}

	//�ͷ��ڴ�
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpTgt);

	return true;
}

void CCom_Port::GET_RX_PKT_BUFF_DATA(CString * csp_Rx_Data)
{
	//��ԡ������ַ�����ͨ�ţ��ѡ���Rx Buff���е�����ת��������CString

	wchar_t * wcpTemp_Data = NULL;
	int iLen;

	this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpTemp_Data, (this->m_str_Com_Rx_Buff.ul_Data_Length + 16));

	this->m_cla_GFC.MultiByte_To_WideChar((char *)this->m_str_Com_Rx_Buff.ucp_Buff, wcpTemp_Data, &iLen);

	csp_Rx_Data->Format(_T("%s"), wcpTemp_Data);

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpTemp_Data);
}

bool CCom_Port::GET_BUFF_BYTE(unsigned char ** ucp2_Buff_Pointer, unsigned long * ulp_Data_Length)
{
	//�����ڣ����ش���ʵ�ʽ��յ����ݵ�BUFF�ĵ�ַ��BUFF�е�byte������
	bool bRtn = false;

	bRtn = this->Read_Byte_Sync_V1();

	*ucp2_Buff_Pointer = this->m_str_Com_Rx_Buff.ucp_Buff;
	*ulp_Data_Length = this->m_str_Com_Rx_Buff.ul_Data_Length;

	return bRtn;
}

void CCom_Port::DEL_BUFF_BYTE(unsigned long ul_Del_Data_Len)
{
	this->m_cla_GFC.PKT_BUFF_DEL_DATA(&this->m_str_Com_Rx_Buff, ul_Del_Data_Len);
}

bool CCom_Port::Empty_Com_And_Rx_Buff(void)
{
	bool ret =false;
	//���COM����
	this->Clear_Rx_Buffer();
	//��ս���COM�������ݵ�BUFF
	ret = this->m_cla_GFC.PKT_BUFF_DEL_ALL(&this->m_str_Com_Rx_Buff);
	return ret;
}
