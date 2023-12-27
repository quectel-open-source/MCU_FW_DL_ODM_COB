#pragma once

#define WRITE_BUFF_SIZE				1100	//ÿ��ʵ�ʡ�WriteFile����ʵ��bytes
#define READ_BUFF_SIZE				512		//ÿ��ʵ�ʡ�ReadFile����ʵ��bytes
#define BUFFER_DATASIZE				3072	//1024 * 3
#define UART_RCV_BUFFERSIZE			8192//1024
#define UART_SND_BUFFERSIZE			8192//1024
#define UART_XONLIM_THRESHOLD		100	//��Buffer�е���100ʱ����XON
#define UART_XOFFLIM_THRESHOLD		600 //��Buffer�и���600ʱ����XOFF

typedef struct tagBUFF_THR_S
{
	HANDLE hMutex;
	char szBuffer[BUFFER_DATASIZE + 1];
	int iBufferSize;		//�ܿռ�
	int iBufferValidLength;	//���ÿռ�
	int i_Type;
}BUFF_THR_S;

typedef void (*Fn_ReportError)(struct tagCOM_THR_S * pCom, LPCTSTR lpszInfo);
typedef void (*Fn_CommitRecvData)(struct tagCOM_THR_S * pCom);
typedef void (*Fn_RequestSendData)(struct tagCOM_THR_S * pCom, int iNum, char * cpData);
typedef void (*Fn_ReportUartState)(struct tagCOM_THR_S * pCom, DWORD dwMask, DWORD ModemStat);
typedef void (*Fn_ReportUartError)(struct tagCOM_THR_S * pCom, DWORD errorvalue);

typedef struct tagCOM_THR_S
{
	//
	//�û����ô��ڲ���
	DWORD dwCom; //���ں�
	DWORD dwBaudRate; //������
/*
flowtype=0; ��������
flowtype=1; �������
flowtype=2; Ӳ������
*/
	int iFlowType;
	int iByteSize;
	int iStopBits;
	int iParity;
	int iDtrControl;
	int iRtsControl;
	
	//
	//�û����ýӿڣ��ص�������
	Fn_CommitRecvData pfnCommitRecvData;
	Fn_RequestSendData pfnRequestSendData;

	Fn_ReportError pfnReportError;
	Fn_ReportUartState pfnReportUartState;
	Fn_ReportUartError pfnReportUartError;
	DWORD userparam1;//(DWORD)this; �û�����thisָ�룬�û��������õ�ֵ
	DWORD userparam2;

	//
	//�ڲ����в���
	HANDLE hCom;
	HANDLE hReadThread;
	HANDLE hWriteThread;
	HANDLE hKillWriteThreadEvent;
	HANDLE hKillReadThreadEvent;
	HANDLE hWriteRequestEvent;
	HANDLE hReadRequestEvent;
	HANDLE hBreakRequestEvent;

	BOOL bInputThreadReading;

	struct tagBUFF_THR_S recvBuffer;
	struct tagBUFF_THR_S sendBuffer;

}COM_THR_S;

class CCom_Thr_Port
{
public:
	CCom_Thr_Port(void);
	~CCom_Thr_Port(void);
	COM_THR_S m_str_COM;
	CString m_csError_Code;
	COMMTIMEOUTS m_str_TimeOuts;
	bool Port_Open(void);
	void Port_Close(void);
	bool Config_Comm_State(const COM_THR_S str_COM);
	void ReportUartState(COM_THR_S * strp_Port, DWORD dwMask, DWORD ModemStat);
	void ReportError(COM_THR_S * strp_Port, LPCTSTR lpszInfo);
	void ReportUartError(COM_THR_S * strp_Port, DWORD errorvalue);
	bool Buffer_Add_Data(BUFF_THR_S * strp_Buff, char * cp_Input_Data, DWORD dw_Data_Len);
	int Buffer_Extract_Data(BUFF_THR_S * strp_Buff, char * cp_Buff, DWORD dw_Buff_Size);
	int Buffer_Get_Space_Free(BUFF_THR_S * strp_Buff);
	int Buffer_Get_Space_Valid(BUFF_THR_S * strp_Buff);
	bool Buffer_Init(BUFF_THR_S * strp_Buff);
	void Buffer_UnInit(BUFF_THR_S * strp_Buff);
	void Buffer_Clear_All(BUFF_THR_S * strp_Buff);
	void CheckModemError(COM_THR_S * strp_Port);
	void CheckModemStatus(COM_THR_S * strp_Port, DWORD dwCommEvent);
	void Process_Rec_Data(COM_THR_S * strp_COM, DWORD dw_Read);
	void Process_Send_Data(COM_THR_S * strp_COM, int i_Quan, char * cp_Data);
	bool Request_Write(COM_THR_S * strp_COM);
	int m_iRING;
	int m_iCTS;
	int m_iRLSD;
	bool Write_Byte(char * cp_Send_Data, unsigned long ul_Data_Quan);
	int Read_Byte(char * cp_Read_Data, const int i_Data_Quan);
	unsigned long Get_Rx_Buff_Valid(void);
	void Buffer_Del_Data(BUFF_THR_S * strp_Buff, DWORD dw_Del_Quan);
	void GET_THR_PORT_RX_BUFF(char ** cp2_Buff_Pointer, unsigned long * ulp_Data_Length);
};
