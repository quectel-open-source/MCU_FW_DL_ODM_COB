#include "StdAfx.h"
#include "Com_Thr_Port.h"

CCom_Thr_Port::CCom_Thr_Port(void)
: m_csError_Code(_T(""))
, m_iRING(0)
, m_iCTS(1)
, m_iRLSD(0)
{
	memset(&m_str_COM, 0, sizeof(m_str_COM));

	m_str_COM.hCom = INVALID_HANDLE_VALUE;
	m_str_COM.pfnCommitRecvData = NULL;
	m_str_COM.pfnRequestSendData = NULL;
	m_str_COM.pfnReportError = NULL;
	m_str_COM.pfnReportUartState = NULL;
	m_str_COM.pfnReportUartError = NULL;
}

CCom_Thr_Port::~CCom_Thr_Port(void)
{
}

static DWORD WINAPI ReadUartThread(LPVOID lpThreadParameter)
{
	//����COM�ڵġ����̡߳�����
	CCom_Thr_Port * port = (CCom_Thr_Port *)(lpThreadParameter);

	OVERLAPPED AsyncReadInfo = { 0 };
	OVERLAPPED AsyncStatusInfo = { 0 };

	AsyncReadInfo.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	AsyncStatusInfo.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	//ASSERT()����Debugģʽ�£���releaseģʽ�±�������Ϊ����䲻����ᡣ
	//ASSERT()�ڵ�����Ϊ��falseʱ,�������ִֹ�У��������״̬��
	//����Ҫ�Ǽ��ӳ����ڵ������еĹ����е��������
	ASSERT( AsyncReadInfo.hEvent );
	ASSERT( AsyncStatusInfo.hEvent );

	//
	DWORD dwCommEvent;
	BOOL waiting_on_status = FALSE;
	int bytes_to_read = 0;
	char read_buffer[READ_BUFF_SIZE + 1];
	DWORD dwBytesRead;

	port->m_str_COM.bInputThreadReading = FALSE;

	HANDLE handles[ 4 ] = {
		port->m_str_COM.hKillReadThreadEvent,
		AsyncReadInfo.hEvent,
		AsyncStatusInfo.hEvent,
		port->m_str_COM.hReadRequestEvent
	};

	SetCommMask(port->m_str_COM.hCom,
		EV_BREAK | EV_CTS  | EV_DSR   | EV_RXCHAR |
		EV_ERR   | EV_RING | EV_RLSD );

//	port->CheckModemStatus(&port->m_str_COM, 0);

	//ѭ��������ʼ
	for(BOOL done = FALSE; !done; ) 
	{
		if(!port->m_str_COM.bInputThreadReading)
		{
			//���COM״̬����RX_BUFF���Ƿ�������
			bytes_to_read = port->Get_Rx_Buff_Valid();

			if(bytes_to_read <= 0)//�������ݣ���ǿ��һ�Σ����������ܳ�ʱ������ʱ�������ݣ�
			{
				//���BUFF���ж���Free�ռ�
//				bytes_to_read = port->Buffer_Get_Space_Free(&port->m_str_COM.recvBuffer);
				bytes_to_read = 0;
			}

			if(bytes_to_read > READ_BUFF_SIZE)
			{
				bytes_to_read = READ_BUFF_SIZE;
			}

			if(bytes_to_read > 0)
			{
				memset(read_buffer, 0, sizeof(read_buffer));

				if(!ReadFile(port->m_str_COM.hCom, read_buffer, bytes_to_read, &dwBytesRead, &AsyncReadInfo))
				{
					//��ʧ��
					port->m_str_COM.bInputThreadReading = TRUE;
				} 
				else 
				{
					if( dwBytesRead ) 
					{
						/*
						#ifdef LOG_COM_READDATA_READFFILE
						{
						if(!gFileLog_Open_ReadFile)
						{
						if(gFileLogReadFile.Open("C:\\comuart.log",CFile::modeWrite|CFile::modeCreate|CFile::typeBinary,NULL))
						{
						gFileLog_Open_ReadFile = TRUE;
						}
						}
						if(gFileLog_Open_ReadFile)
						{
						gFileLogReadFile.Write(read_buffer, dwBytesRead);
						}
						}
						#endif
						*/
						port->Buffer_Add_Data(&port->m_str_COM.recvBuffer, read_buffer, dwBytesRead );
						port->Process_Rec_Data(&port->m_str_COM, dwBytesRead);//����������ݵĽ���ص�����
					}
				}
			}
			else 
			{
				ResetEvent( AsyncReadInfo.hEvent );
			}
		}//if(!port->m_str_COM.bInputThreadReading)

		if(!waiting_on_status) 
		{
//			dwCommEvent = 0x0;

			if(!WaitCommEvent(port->m_str_COM.hCom, &dwCommEvent, &AsyncStatusInfo)) 
			{
				waiting_on_status = TRUE;
			} 
			else 
			{
//				port->CheckModemStatus(&port->m_str_COM, dwCommEvent);
//				port->CheckModemError(&port->m_str_COM);
			}
		}

		if(waiting_on_status && (port->m_str_COM.bInputThreadReading || (bytes_to_read == 0))) 
		{
			DWORD result = WaitForMultipleObjects(4, handles, FALSE, INFINITE);

			switch( result ) 
			{
			case 0 : 
				done = TRUE;
				break;
			case 1 :
				if(GetOverlappedResult(port->m_str_COM.hCom, &AsyncReadInfo, &dwBytesRead, FALSE))
				{     
					if(dwBytesRead) 
					{
						/*
						#ifdef LOG_COM_READDATA_READFFILE
						{
						if(!gFileLog_Open_ReadFile)
						{
						if(gFileLogReadFile.Open("C:\\comuart.log",CFile::modeWrite|CFile::modeCreate|CFile::typeBinary,NULL))
						{
						gFileLog_Open_ReadFile = TRUE;
						}
						}
						if(gFileLog_Open_ReadFile)
						{
						gFileLogReadFile.Write(read_buffer, dwBytesRead);
						}
						}
						#endif
						*/
						port->Buffer_Add_Data(&port->m_str_COM.recvBuffer, read_buffer, dwBytesRead );
						port->Process_Rec_Data(&port->m_str_COM, dwBytesRead);
					}
				}
				port->m_str_COM.bInputThreadReading = FALSE;
				break;
			case 2 : 
				{ 
					DWORD dwOverlappedResult;

					if(GetOverlappedResult(port->m_str_COM.hCom, &AsyncStatusInfo, &dwOverlappedResult, FALSE))
					{
						//���ﾭ�����е�
//						port->CheckModemStatus(&port->m_str_COM, dwCommEvent);
//						port->CheckModemError(&port->m_str_COM);
					}
					waiting_on_status = FALSE;
					break;
				}
			case 3 :
				break;
			default :
				ASSERT(FALSE);
			}
		}//if(waiting_on_status && (port->m_str_COM.bInputThreadReading || (bytes_to_read == 0))
	}//for(BOOL done = FALSE; !done;

	CloseHandle( AsyncReadInfo.hEvent );
	CloseHandle( AsyncStatusInfo.hEvent );

	return 0;
}

static DWORD WINAPI WriteUartThread(LPVOID lpThreadParameter)
{
	//����COM�ڵġ�д�̡߳�����
	CCom_Thr_Port * port = (CCom_Thr_Port *)(lpThreadParameter);

	HANDLE handles[3] = {
		port->m_str_COM.hKillWriteThreadEvent,
		port->m_str_COM.hWriteRequestEvent,
		port->m_str_COM.hBreakRequestEvent
	};

	port->Process_Send_Data(&port->m_str_COM, 0, NULL);

	for(bool done = false; !done; ) 
	{
		switch(WaitForMultipleObjects(3, handles, FALSE, INFINITE)) 
		{
		case 0: //hKillWriteThreadEvent
			done = true;
			break;

		case 1: //hWriteRequestEvent
			done = port->Request_Write(&port->m_str_COM);
			break;

		case 2: //hBreakRequestEvent
			SetCommBreak(port->m_str_COM.hCom);
			//SleepEx( port->iBreakDuration, FALSE );
			ClearCommBreak(port->m_str_COM.hCom);
			break;

		default :
			ASSERT(FALSE);
			break;
		}
	}	

	return 0;
}

bool CCom_Thr_Port::Port_Open(void)
{
	if(this->m_str_COM.hCom != INVALID_HANDLE_VALUE)
	{
		return true;
	}

	DWORD dwError;
	CString strCom;

	try
	{
		strCom.Format(_T("\\\\.\\COM%d"), this->m_str_COM.dwCom);

		this->m_str_COM.hCom = CreateFile(
			strCom, GENERIC_READ | GENERIC_WRITE, 0,
			NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			NULL);

		if(this->m_str_COM.hCom == INVALID_HANDLE_VALUE)
		{
			dwError = GetLastError();

			this->m_csError_Code.Format(_T("FAIL, Open thr com %d failed, error:0x%X!"), this->m_str_COM.dwCom, dwError);

			throw(FALSE);
		}

		//�趨����ʱ 
		this->m_str_TimeOuts.ReadIntervalTimeout = 1;			//�������ʱ
		this->m_str_TimeOuts.ReadTotalTimeoutMultiplier = 0;	//��ʱ��ϵ��
		this->m_str_TimeOuts.ReadTotalTimeoutConstant = 0;		//��ʱ�䳣��
		//�趨д��ʱ
		this->m_str_TimeOuts.WriteTotalTimeoutMultiplier = 0;	//дʱ��ϵ��
		this->m_str_TimeOuts.WriteTotalTimeoutConstant = 0;		//дʱ�䳣��
		//���ó�ʱ
		SetCommTimeouts(this->m_str_COM.hCom, &this->m_str_TimeOuts);
		//���뻺����������������Ĵ�С����
		SetupComm(this->m_str_COM.hCom, UART_RCV_BUFFERSIZE, UART_SND_BUFFERSIZE);

		PurgeComm(this->m_str_COM.hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

		SetCommMask(this->m_str_COM.hCom, EV_RXCHAR | EV_TXEMPTY | EV_CTS | EV_BREAK | EV_DSR | EV_ERR | EV_RLSD  | EV_RING);

		PurgeComm(this->m_str_COM.hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

		//���ô��ڲ���
		if(!this->Config_Comm_State(this->m_str_COM))
		{
			this->m_csError_Code.Format(_T("FAIL, Config com %d state failed!"), this->m_str_COM.dwCom);
			throw(FALSE);
		}

		//�����¼�
		if((this->m_str_COM.hKillWriteThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, COM:%d, CreateEvent hKillWriteThreadEvent failed!"), this->m_str_COM.dwCom);
			throw(FALSE);
		}
		if((this->m_str_COM.hKillReadThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, COM:%d, CreateEvent hKillReadThreadEvent failed!"), this->m_str_COM.dwCom);
			throw(FALSE);
		}
		if((this->m_str_COM.hWriteRequestEvent = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, COM:%d, CreateEvent hWriteRequestEvent failed!"), this->m_str_COM.dwCom);
			throw(FALSE);
		}
		if((this->m_str_COM.hReadRequestEvent = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, COM:%d, CreateEvent hReadRequestEvent failed!"), this->m_str_COM.dwCom);
			throw(FALSE);
		}
		if((this->m_str_COM.hBreakRequestEvent = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, COM:%d, CreateEvent hBreakRequestEvent failed!"), this->m_str_COM.dwCom);
			throw(FALSE);
		}
		
		//��ʼ�����͡�����BUFF
		this->m_str_COM.sendBuffer.i_Type = 1;
		if(!Buffer_Init(&this->m_str_COM.sendBuffer))
		{
			this->m_csError_Code.Format(_T("FAIL, COM:%d, CreateEvent Buffer_Init sendBuffer failed!"), this->m_str_COM.dwCom);
			throw(FALSE);
		}
		
		this->m_str_COM.recvBuffer.i_Type = 2;
		if(!Buffer_Init(&this->m_str_COM.recvBuffer))
		{
			this->m_csError_Code.Format(_T("FAIL, COM:%d, CreateEvent Buffer_Init recvBuffer failed!"), this->m_str_COM.dwCom);
			throw(FALSE);
		}
		
		//�������������߳�
		DWORD id;

		//�������߳�
		this->m_str_COM.hReadThread = CreateThread(
			NULL, 0,
			(LPTHREAD_START_ROUTINE)ReadUartThread,
			(void *)this,
			0, &id
			);
		
		if(this->m_str_COM.hReadThread == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, COM:%d, CreateEvent hReadThread failed!"), this->m_str_COM.dwCom);
			throw(FALSE);
		}
		
		//��д���߳�
		this->m_str_COM.hWriteThread = CreateThread(
			NULL, 0,
			(LPTHREAD_START_ROUTINE)WriteUartThread,
			(void*)this,//ptagcom,
			0, &id);
		
		if(this->m_str_COM.hWriteThread == NULL)
		{
			this->m_csError_Code.Format(_T("FAIL, COM:%d, CreateEvent hWriteThread failed!"), this->m_str_COM.dwCom);
			throw(FALSE);
		}
	}
	catch(...)
	{
		this->Port_Close();
		return false;
	}
	
	return true;
}

bool CCom_Thr_Port::Config_Comm_State(const COM_THR_S str_COM)
{
	DCB dcb;

	if(!GetCommState(str_COM.hCom, &dcb))
	{
		return false;
	}

	dcb.fBinary  = TRUE;
	dcb.BaudRate = str_COM.dwBaudRate; 

	dcb.ByteSize = str_COM.iByteSize;
	dcb.fParity  = FALSE;
   
	dcb.Parity = str_COM.iParity;
	/*
	switch(0)
	{
	case 0: dcb.Parity = NOPARITY;
		break;
	case 1: dcb.Parity = EVENPARITY;
		break;
	case 2: dcb.Parity = ODDPARITY;
		break;
	default:;
	}
	*/
   
	dcb.StopBits = str_COM.iStopBits;
	/*
	switch(0)
	{
	case 0: dcb.StopBits = ONESTOPBIT;
		break;
	case 1: dcb.StopBits = ONE5STOPBITS;
		break;
	case 2: dcb.StopBits = TWOSTOPBITS;
		break;
	default:;
	}
	*/

    dcb.fOutxDsrFlow = FALSE; //�رմ��ж˿ڵ�DSR����

	if(str_COM.iDtrControl)
	{
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
	}
	else
	{
		dcb.fDtrControl = DTR_CONTROL_DISABLE;
	}
	
    dcb.fDsrSensitivity   = FALSE; //�����ΪTRUE�������κ�������ֽڣ�����DSR�߱����� 


	/*��Ӳ������*/
    dcb.fOutxCtsFlow = FALSE; //���ж˿ڵ������CTS�߿���

	if(str_COM.iRtsControl)
	{
		//RTS��������RTS��Ӳ����������Ӳ����������
		dcb.fRtsControl = RTS_CONTROL_ENABLE;
	}
	else
	{
		//RTS�ر�
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
	}

    //dcb.fRtsControl   = RTS_CONTROL_ENABLE; //����RTS�� 
	//EscapeCommFunction(m_hCom, SETRTS);

    dcb.fTXContinueOnXoff = TRUE;//��ΪTRUEʱ��������ջ��������������������Ѵ���XOFF�ַ�����ʹ��������ֹͣ�����ַ�
    dcb.fOutX             = FALSE;//��ΪTRUEָ��XON/XOFF���Ʊ����ڿ��ƴ������ 
    dcb.fInX              = FALSE;


	if(str_COM.iFlowType == 2)
	{
		/*��Ӳ������*/
		dcb.fOutxCtsFlow = TRUE; //���ж˿ڵ������CTS�߿���

		if(str_COM.iRtsControl == TRUE)
		{
			//RTS��������RTS��Ӳ����������Ӳ����������
			dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
		}
		else
		{
			//RTS�ر�
			dcb.fRtsControl = RTS_CONTROL_DISABLE;
		}
		
		//dcb.fRtsControl   = RTS_CONTROL_ENABLE; //����RTS�� 
		//EscapeCommFunction(m_hCom, SETRTS);
	}
	else if(str_COM.iFlowType == 1)
	{
		/*
		ָ�������뻺���������յ�XoffChar�ַ�ʱ�������Ƿ���ֹ��
		�ò�������ΪTRUE�������뻺�������ֽ���С��XoffLim������ֽ���ʱ���佫������������XoffChar�ֽں�ֹͣ�����ֽڡ�
		����ò�������ΪFALSE����ôֻ�е����뻺�������ֽ�����XonLim������ֽ����ڣ������������͹�XonChar�ַ��Իָ����չ���ʱ������Ż������
		*/
		dcb.fTXContinueOnXoff = TRUE;
		dcb.fOutX             = TRUE;//��ΪTRUEָ��XON/XOFF���Ʊ����ڿ��ƴ������ 
		dcb.fInX              = TRUE;
		//dcb.fTXContinueOnXoff = FALSE;  /* Continue TX when Xoff sent */
		//dcb.fOutX = FALSE;       /* Enable output X-ON/X-OFF        */
		//dcb.fInX = FALSE;        /* Enable input X-ON/X-OFF         */
		dcb.XonLim = UART_XONLIM_THRESHOLD;          /* Transmit X-ON threshold         */
		dcb.XoffLim = UART_XOFFLIM_THRESHOLD;         /* Transmit X-OFF threshold        */
		dcb.XonChar = 0x11;         /* Tx and Rx X-ON character        */
		dcb.XoffChar = 0x13;        /* Tx and Rx X-OFF character       */
	}
	else
	{
	}

    dcb.fErrorChar        = FALSE;
    dcb.fNull             = FALSE;
    dcb.fAbortOnError     = FALSE;
	
	return (bool)SetCommState(str_COM.hCom, &dcb);
}

bool CCom_Thr_Port::Buffer_Init(BUFF_THR_S * strp_Buff)
{
	strp_Buff->hMutex = CreateMutex(NULL, FALSE, NULL);

	strp_Buff->iBufferSize = BUFFER_DATASIZE;
	strp_Buff->iBufferValidLength = 0;

	memset(strp_Buff->szBuffer, 0, sizeof(strp_Buff->szBuffer));

	ASSERT(strp_Buff->hMutex);

	return true;
}

void CCom_Thr_Port::CheckModemStatus(COM_THR_S * strp_Port, DWORD dwCommEvent)
{
	DWORD dwModemStatus;

	if(!GetCommModemStatus(strp_Port->hCom, &dwModemStatus))
	{
		ASSERT(FALSE);
	}

	//dwCommEvent & EV_CTS
	//dwCommEvent & EV_DSR

	//dwModemStatus & MS_CTS_ON
	//dwModemStatus & MS_DSR_ON

	this->ReportUartState(strp_Port, dwCommEvent, dwModemStatus);

//	TRACE(_T("EVENT %x, ModemStatus %x\r\n"), dwCommEvent, dwModemStatus);
}

void CCom_Thr_Port::ReportUartState(COM_THR_S * strp_Port, DWORD dwMask, DWORD ModemStat)
{
	bool bCTS = false;
	bool bRLSD = false;
	bool bRING =  false;

	if(dwMask & EV_CTS)
	{
		if(ModemStat & MS_CTS_ON)
		{
			if(this->m_iCTS != 1)
			{
				this->m_iCTS = 1;
				bCTS = true;
			}

			this->ReportError(strp_Port, _T("EV_CTS MS_CTS_ON"));
		}
		else
		{
			if(this->m_iCTS != 0)
			{
				this->m_iCTS = 0;
				bCTS = true;
			}

			this->ReportError(strp_Port, _T("EV_CTS MS_CTS_OFF"));
		}
	}

	//RLSD
	if(dwMask & EV_RLSD)
	{
		if(ModemStat & MS_RLSD_ON)
		{
			if(this->m_iRLSD != 1)
			{
				this->m_iRLSD = 1;
				bRLSD = true;
			}
			this->ReportError(strp_Port, _T("EV_RLSD MS_RLSD_ON"));
		}
		else
		{
			if(this->m_iRLSD != 0)
			{
				this->m_iRLSD = 0;
				bRLSD = true;
			}
			this->ReportError(strp_Port, _T("EV_RLSD MS_RLSD_OFF"));
		}
	}

	//RING
	if(dwMask & EV_RING)
	{
		if(ModemStat & MS_RING_ON)
		{
			if(this->m_iRING != 1)
			{
				this->m_iRING = 1;
				bRING = true;
			}
			this->ReportError(strp_Port, _T("EV_RING MS_RING_ON"));
		}
		else
		{
			if(this->m_iRING != 0)
			{
				this->m_iRING = 0;
				bRING = true;
			}
			this->ReportError(strp_Port, _T("EV_RING MS_RING_OFF"));
		}
	}

	if(bCTS || bRLSD || bRING)
	{
		/*
		char crData[64];
		sprintf(crData, " DCD:%d  CTS:%d  RI:%d", this->m_iRLSD, this->m_iCTS, this->m_iRING);
		this->Show_REdit_State(crData, 0);
		*/
	}

	//DSR
	if((dwMask & EV_DSR) && (( ModemStat & MS_DSR_ON)))
	{
		this->ReportError(strp_Port, _T("EV_CTS MS_DSR_ON"));
	}
	if((dwMask & EV_DSR) && (!( ModemStat & MS_DSR_ON)))
	{
		this->ReportError(strp_Port, _T("EV_CTS MS_DSR_OFF"));
	}

	if(dwMask & EV_TXEMPTY)
	{
		this->ReportError(strp_Port, _T("EV_TXEMPTY"));
	}
	if(dwMask & EV_BREAK)
	{
		this->ReportError(strp_Port, _T("EV_BREAK"));
	}
	if(dwMask & EV_ERR)
	{
		this->ReportError(strp_Port, _T("EV_ERR"));
	}
	if(dwMask & EV_PERR)
	{
		this->ReportError(strp_Port, _T("EV_PERR"));
	}
	if(dwMask & EV_RX80FULL)
	{
		this->ReportError(strp_Port, _T("EV_RX80FULL"));
	}

	if(strp_Port->pfnReportUartState)
	{
		strp_Port->pfnReportUartState(strp_Port, dwMask, ModemStat);
	}
}

void CCom_Thr_Port::ReportError(COM_THR_S * strp_Port, LPCTSTR lpszInfo)
{
	if(strp_Port->pfnReportError)
	{
		strp_Port->pfnReportError(strp_Port, lpszInfo);
	}
}

int CCom_Thr_Port::Buffer_Get_Space_Free(BUFF_THR_S * strp_Buff)
{
	int iSpaceFree;

	ASSERT(strp_Buff->iBufferSize >= strp_Buff->iBufferValidLength);

	WaitForSingleObject(strp_Buff->hMutex, INFINITE);

	iSpaceFree = (strp_Buff->iBufferSize - strp_Buff->iBufferValidLength);

	ReleaseMutex(strp_Buff->hMutex);

	return iSpaceFree;
}

int CCom_Thr_Port::Buffer_Get_Space_Valid(BUFF_THR_S * strp_Buff)
{
	int iValid;

	ASSERT(strp_Buff->iBufferSize >= strp_Buff->iBufferValidLength);

	WaitForSingleObject(strp_Buff->hMutex, INFINITE);

	iValid = strp_Buff->iBufferValidLength;

	ReleaseMutex(strp_Buff->hMutex);

	return iValid;
}

bool CCom_Thr_Port::Buffer_Add_Data(BUFF_THR_S * strp_Buff, char * cp_Input_Data, DWORD dw_Data_Len)
{
	int iSpaceFree;
	
	ASSERT(strp_Buff->iBufferSize >= strp_Buff->iBufferValidLength);

	WaitForSingleObject(strp_Buff->hMutex, INFINITE);

	iSpaceFree = (strp_Buff->iBufferSize - strp_Buff->iBufferValidLength);//�ܿռ�-���ÿռ�=���ÿռ�
	
	if((DWORD)iSpaceFree > dw_Data_Len)//���ÿռ��㹻��������
	{
		iSpaceFree = (int)dw_Data_Len;

		memcpy((strp_Buff->szBuffer + strp_Buff->iBufferValidLength), cp_Input_Data, iSpaceFree);

		strp_Buff->iBufferValidLength += iSpaceFree;

		//���BUFF����������������������
		ReleaseMutex(strp_Buff->hMutex);

		return true;
	}
	else
	{
		CString a;
		a.Format(_T("FAIL, Buffer %d thr add data failed!"), strp_Buff->i_Type);
		::AfxMessageBox(a);
		ReleaseMutex(strp_Buff->hMutex);//�ͷſռ��ռ�ı�ʶ
		return false;
	}
}

void CCom_Thr_Port::CheckModemError(COM_THR_S * strp_Port)
{
	COMSTAT comstat;
    DWORD errors;

    ClearCommError(strp_Port->hCom, &errors, &comstat);

	this->ReportUartError(strp_Port, errors);
}

void CCom_Thr_Port::ReportUartError(COM_THR_S * strp_Port, DWORD errorvalue)
{
	if(errorvalue & CE_RXOVER)
	{
		ReportError(strp_Port, _T("CE_RXOVER"));
	}
	if(errorvalue & CE_OVERRUN)
	{
		ReportError(strp_Port, _T("CE_OVERRUN"));
	}
	if(errorvalue & CE_RXPARITY)
	{
		ReportError(strp_Port, _T("CE_RXPARITY"));
	}
	if(errorvalue & CE_FRAME)
	{
		ReportError(strp_Port, _T("CE_FRAME"));
	}
	if(errorvalue & CE_BREAK)
	{
		ReportError(strp_Port, _T("CE_BREAK"));
	}
	if(errorvalue & CE_TXFULL)
	{
		ReportError(strp_Port, _T("CE_TXFULL"));
	}
	if(errorvalue & CE_PTO)
	{
		ReportError(strp_Port, _T("CE_PTO"));
	}
	if(errorvalue & CE_IOE)
	{
		ReportError(strp_Port, _T("CE_IOE"));
	}
	if(errorvalue & CE_DNS)
	{
		ReportError(strp_Port, _T("CE_DNS"));
	}
	if(errorvalue & CE_OOP)
	{
		ReportError(strp_Port, _T("CE_OOP"));
	}
	if(errorvalue & CE_MODE)
	{
		ReportError(strp_Port, _T("CE_MODE"));
	}

	if(strp_Port->pfnReportUartError)
	{
		strp_Port->pfnReportUartError(strp_Port, errorvalue);
	}
}

void CCom_Thr_Port::Process_Rec_Data(COM_THR_S * strp_COM, DWORD dw_Read)
{
	//������յ����ݺ󣬽��н��洦��Ļص�����

	if(strp_COM->pfnCommitRecvData)
	{
		strp_COM->pfnCommitRecvData(strp_COM);//(strp_COM, dw_Read);
	}
}

void CCom_Thr_Port::Process_Send_Data(COM_THR_S * strp_COM, int i_Quan, char * cp_Data)
{
	//׼�����͵����ݣ����н��洦��Ļص�����

	if(strp_COM->pfnRequestSendData)
	{
		strp_COM->pfnRequestSendData(strp_COM, i_Quan, cp_Data);
	}
}

bool CCom_Thr_Port::Request_Write(COM_THR_S * strp_COM)
{
	OVERLAPPED AsyncWriteInfo = { 0 };

	AsyncWriteInfo.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	ASSERT( AsyncWriteInfo.hEvent );

	HANDLE handles[ 2 ] = { strp_COM->hKillWriteThreadEvent,
		AsyncWriteInfo.hEvent };

	bool killed = false;
	char data[WRITE_BUFF_SIZE + 8];
	int count;

	for(BOOL done = FALSE; !done; ) 
	{
		memset(data, 0, sizeof(data));
		count = Buffer_Extract_Data(&strp_COM->sendBuffer, data, WRITE_BUFF_SIZE);

		if(count == 0) 
		{
			//			RequestSendData(port, count, data);
			//          RequestSendData(port);
			break;
		}
		else if(count > 0)
		{
			this->Process_Send_Data(strp_COM, count, data);
		}

		DWORD result_count;

		if(!WriteFile(strp_COM->hCom, data, count, &result_count, &AsyncWriteInfo)) 
		{
			if(GetLastError() == ERROR_IO_PENDING) 
			{
				switch(WaitForMultipleObjects(2, handles, FALSE, INFINITE)) 
				{

				case 0 : 
					done = TRUE;
					killed = true;
					PurgeComm( strp_COM->hCom, PURGE_TXABORT );
					break;
				case 1 :
					if (!GetOverlappedResult(strp_COM->hCom, &AsyncWriteInfo, &result_count, FALSE) ||
						(result_count != (DWORD)count))
					{
						if(GetLastError() == ERROR_IO_PENDING)
						{
							this->CheckModemError(strp_COM);
						}
						else
						{
							this->m_csError_Code.Format(_T("FAIL, WriteFile GetOverlappedResult error"));
							TRACE("WriteFile GetOverlappedResult error");
							ASSERT(0);
						}
						done = TRUE;
					}
					break;
				default :
					ASSERT(FALSE);
				}
			}
			else 
			{
				this->m_csError_Code.Format(_T("FAIL, WriteFile else error"));
				TRACE("WriteFile error");
				ASSERT(0);
				done = TRUE;
			}
		} 
		else 
		{
			if(result_count != (DWORD)count) 
			{
				this->m_csError_Code.Format(_T("FAIL, WriteFile result_count != count"));
				TRACE("WriteFile result_count != count ");
				ASSERT(0);
				done = TRUE;
			}
		}
	}

	CloseHandle( AsyncWriteInfo.hEvent );
	return killed;
}

int CCom_Thr_Port::Buffer_Extract_Data(BUFF_THR_S * strp_Buff, char * cp_Buff, DWORD dw_Buff_Size)
{
	int iExtractData = (int)dw_Buff_Size;

	ASSERT(strp_Buff->iBufferSize >= strp_Buff->iBufferValidLength);

	WaitForSingleObject(strp_Buff->hMutex, INFINITE);

	if(iExtractData > strp_Buff->iBufferValidLength)
	{
		iExtractData = strp_Buff->iBufferValidLength;
	}

	//��ȡ
	memcpy(cp_Buff, strp_Buff->szBuffer, iExtractData);

	strp_Buff->iBufferValidLength -= iExtractData;

	memcpy(strp_Buff->szBuffer, (strp_Buff->szBuffer + iExtractData), strp_Buff->iBufferValidLength);

	ReleaseMutex(strp_Buff->hMutex);

	return iExtractData;
}

void CCom_Thr_Port::Port_Close(void)
{
	if(this->m_str_COM.hCom == INVALID_HANDLE_VALUE)
	{
		return;
	}

	SetEvent(this->m_str_COM.hKillWriteThreadEvent);
	SetEvent(this->m_str_COM.hKillReadThreadEvent);

	HANDLE handles[2] = {this->m_str_COM.hReadThread, this->m_str_COM.hWriteThread};

	//�������⣬�����ڵȴ������߳��˳������߳�ͬʱ�ڴ������Ĳ���������ʱ�����ڵȴ��߳��˳��������ա���ʾ���񡱣������������
	WaitForMultipleObjects(2, handles, TRUE, 8000);//INFINITE);

	CloseHandle(this->m_str_COM.hReadThread);
	this->m_str_COM.hReadThread = NULL;

	CloseHandle(this->m_str_COM.hWriteThread);
	this->m_str_COM.hWriteThread = NULL;

	CloseHandle(this->m_str_COM.hCom);
	this->m_str_COM.hCom = INVALID_HANDLE_VALUE;

	CloseHandle(this->m_str_COM.hKillWriteThreadEvent);
	CloseHandle(this->m_str_COM.hKillReadThreadEvent);
	CloseHandle(this->m_str_COM.hWriteRequestEvent);
	CloseHandle(this->m_str_COM.hReadRequestEvent);
	CloseHandle(this->m_str_COM.hBreakRequestEvent);

	this->m_str_COM.hKillWriteThreadEvent = NULL;
	this->m_str_COM.hKillReadThreadEvent = NULL;
	this->m_str_COM.hWriteRequestEvent = NULL;
	this->m_str_COM.hReadRequestEvent = NULL;
	this->m_str_COM.hBreakRequestEvent = NULL;

	this->Buffer_UnInit(&this->m_str_COM.recvBuffer);
	this->Buffer_UnInit(&this->m_str_COM.sendBuffer);
}

void CCom_Thr_Port::Buffer_Clear_All(BUFF_THR_S * strp_Buff)
{
	ASSERT(strp_Buff->iBufferSize >= strp_Buff->iBufferValidLength);

	WaitForSingleObject(strp_Buff->hMutex, INFINITE);

	//���
	strp_Buff->iBufferSize = BUFFER_DATASIZE;
	strp_Buff->iBufferValidLength = 0;
	memset(strp_Buff->szBuffer, 0, sizeof(strp_Buff->szBuffer));

	ReleaseMutex(strp_Buff->hMutex);
}

void CCom_Thr_Port::Buffer_UnInit(BUFF_THR_S * strp_Buff)
{
	CloseHandle(strp_Buff->hMutex);
}

bool CCom_Thr_Port::Write_Byte(char * cp_Send_Data, unsigned long ul_Data_Quan)
{
	
	OVERLAPPED AsyncWriteInfo = { 0 };

	AsyncWriteInfo.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	ASSERT( AsyncWriteInfo.hEvent );

	HANDLE handles[ 1 ] = { AsyncWriteInfo.hEvent };

	BOOL bWriteState;
	DWORD dwBytesWritten;

	bWriteState = WriteFile(this->m_str_COM.hCom, cp_Send_Data, ul_Data_Quan, &dwBytesWritten, &AsyncWriteInfo);   
	
	if(!bWriteState)   
	{   
		if(GetLastError() == ERROR_IO_PENDING)   
		{   
			WaitForMultipleObjects(1, handles, FALSE, INFINITE);//INFINITE

			CloseHandle( AsyncWriteInfo.hEvent );
			return true;   
		}

		CloseHandle( AsyncWriteInfo.hEvent );
		return false;   
	}

	CloseHandle( AsyncWriteInfo.hEvent );
	return true;
	
/*
	if(this->m_str_COM.hCom == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	if(this->Buffer_Add_Data(&this->m_str_COM.sendBuffer, cp_Send_Data, dw_Data_Quan) == false)
	{
		this->m_csError_Code.Format(_T("FAIL, Write byte add buff failed!"));
		return false;
	}

	::SetEvent(this->m_str_COM.hWriteRequestEvent);

	return true;*/
}

int CCom_Thr_Port::Read_Byte(char * cp_Read_Data, const int i_Data_Quan)
{
	int iByteCount = 0;

	if(this->m_str_COM.hCom == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	iByteCount = this->Buffer_Extract_Data(&this->m_str_COM.recvBuffer, cp_Read_Data, i_Data_Quan);

	if(!this->m_str_COM.bInputThreadReading)
	{
		SetEvent(this->m_str_COM.hReadRequestEvent);
	}

	return iByteCount;
}


unsigned long CCom_Thr_Port::Get_Rx_Buff_Valid(void)
{
	COMSTAT ComStat;
	DWORD dwErrorFlags;

	ComStat.cbInQue = 0;

	if(!ClearCommError(this->m_str_COM.hCom, &dwErrorFlags, &ComStat))
	{
		return 0;
	}

	if(dwErrorFlags != 0)//�б����ComStat.cbInQue��ֵ�ܻ���
	{
		return 0;
	}

	return ComStat.cbInQue;
}

void CCom_Thr_Port::Buffer_Del_Data(BUFF_THR_S * strp_Buff, DWORD dw_Del_Quan)
{
	int iExtractData = (int)dw_Del_Quan;

	ASSERT(strp_Buff->iBufferSize >= strp_Buff->iBufferValidLength);

	WaitForSingleObject(strp_Buff->hMutex, INFINITE);

	if(iExtractData > strp_Buff->iBufferValidLength)
	{
		iExtractData = strp_Buff->iBufferValidLength;
	}

	strp_Buff->iBufferValidLength -= iExtractData;

	memcpy(strp_Buff->szBuffer, (strp_Buff->szBuffer + iExtractData), strp_Buff->iBufferValidLength);

	ReleaseMutex(strp_Buff->hMutex);
}

void CCom_Thr_Port::GET_THR_PORT_RX_BUFF(char ** cp2_Buff_Pointer, unsigned long * ulp_Data_Length)
{
	//���ش���BUFF�ĵ�ַ
	*cp2_Buff_Pointer = this->m_str_COM.recvBuffer.szBuffer;
	//������BUFF�е�������������
	*ulp_Data_Length = this->Buffer_Get_Space_Valid(&this->m_str_COM.recvBuffer);
}
