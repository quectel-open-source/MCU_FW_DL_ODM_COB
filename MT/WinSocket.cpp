#include "StdAfx.h"
#include "WinSocket.h"

CWinSocket::CWinSocket( void )
	: m_csError_Info( _T( "" ) )
	, m_bDebug_Log_File( false )
	, m_bConn_Running_Enable( false )
{
	m_uiSocket_Main = INVALID_SOCKET;
	m_bConn_Running_Enable = false;
}

CWinSocket::~CWinSocket(void)
{
	UNINIT_WIN_SOCK_API();//�ͷ�Ϊ�ó���������Դ����ֹ��winsock��̬���ʹ��
}

void CWinSocket::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
//	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)1);
}

bool CWinSocket::INIT_WIN_SOCK_API(void)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int iRet;

	//
	try
	{
		//��һ���������׽��֡�WinSock��ʼ��
		wVersionRequested = MAKEWORD(2, 2);//ϣ��ʹ�õ�WinSock DLL�İ汾

		//�����׽��ֿ�
		iRet = WSAStartup(wVersionRequested, &wsaData);

		//�ж�����ֵ
		if(iRet != 0)
		{
			this->m_csError_Info.Format(_T("FAIL, INIT_WIN_SOCK_API, WSAStartup(), iRet = %d;"), iRet);
			throw(0);
		}


		//ȷ��WinSock DLL֧�ְ汾2.2
		if((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2))
		{
			this->m_csError_Info.Format(_T("FAIL, INIT_WIN_SOCK_API, Invalid_WinSock_Version(%d.%d)!"),
				LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
			throw(0);
		}
	}
	catch(...)
	{
		this->UNINIT_WIN_SOCK_API();//�ͷ�Ϊ�ó���������Դ����ֹ��winsock��̬���ʹ��

		//fail
		return false;
	}

	//pass
	return true;
}

void CWinSocket::UNINIT_WIN_SOCK_API(void)
{
	WSACleanup();//�ͷ�Ϊ�ó���������Դ����ֹ��winsock��̬���ʹ��
}

bool CWinSocket::Create_Socket(int i_Mode_TCP_UDP)
{
	int iType, iProtocol;

	try
	{
		//ȷ������
		switch(i_Mode_TCP_UDP)
		{
		case MODE_TCP:
			iType = SOCK_STREAM;
			iProtocol = IPPROTO_TCP;
			break;
		default:
			this->m_csError_Info.Format(_T("ERROR, Create_Socket, Mode(%d) is wrong!"), i_Mode_TCP_UDP);
			throw(0);
		}


		//�ڶ����������׽��֡�����Socket,ʹ��TCPЭ��
		this->m_uiSocket_Main = socket(AF_INET, iType, iProtocol);

		//�ж�����ֵ
		if(this->m_uiSocket_Main == INVALID_SOCKET)
		{
			this->m_csError_Info.Format(_T("FAIL, Create_Socket, uiSocket_HD = %d;"), this->m_uiSocket_Main);
			throw(0);
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CWinSocket::Create_Client(const wchar_t * wcp_IP_Addr, int i_Port)
{
	struct sockaddr_in strSock_Addr_In;//��������ַ��Ϣ
	int iRet;
	char * cpIP_Addr = NULL;


	try
	{
		//����char�ڴ�
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpIP_Addr, (wcslen(wcp_IP_Addr) * 2 + 8));

		//IP��ַת��
		this->m_cla_GFC.WideChar_To_MultiByte(wcp_IP_Addr, cpIP_Addr, &iRet);


		//
		memset(&strSock_Addr_In, 0, sizeof(strSock_Addr_In));

		//������������ַ��Ϣ
		strSock_Addr_In.sin_family = AF_INET; //��ַ����
		strSock_Addr_In.sin_port = htons((unsigned short)i_Port);//htons(SERVER_PORT);//ע��ת��Ϊ�������
		strSock_Addr_In.sin_addr.S_un.S_addr = htonl(ntohl(inet_addr(cpIP_Addr)));//inet_addr("192.168.1.10");

/*		//�ڶ������첽��Ϣ�������
		iRet = WSAAsyncSelect(this->m_uiSocket_Main, this->m_hWnd_View, WM_SOCKET, FD_READ | FD_CLOSE);
		if (iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, WSAAsyncSelect(), iRet = %d, Error_code = %d;"), iRet, WSAGetLastError());
			throw(0);
		}*/
		

		//�����������ӷ�������
		iRet = connect(this->m_uiSocket_Main, (struct sockaddr *)&strSock_Addr_In, sizeof(strSock_Addr_In));
		//
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, Create_Client(IP_Addr = %s, Port = %d), iRet = %d;"), wcp_IP_Addr, i_Port, iRet);
			throw(0);
		}

		iRet = WSAAsyncSelect(this->m_uiSocket_Main, this->m_hWnd_View, WM_SOCKET, FD_READ | FD_CLOSE);
		//�ͷ�char�ڴ�
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpIP_Addr);
	}
	catch(...)
	{
		//�ر��׽���
		this->CLOSE_SOCKET_MAIN();

		//�ͷ�char�ڴ�
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpIP_Addr);

		//
		return false;
	}

	return true;
}

void CWinSocket::CLOSE_SOCKET_MAIN(void)
{
	//�رա����ء������׽���
	this->CLOSE_SOCKET_SEL( &this->m_uiSocket_Main );
}

bool CWinSocket::SEND_CHAR_MAIN(const char * cp_Send_Byte, int i_Byte_Quan)
{
	return this->SEND_CHAR_SEL(this->m_uiSocket_Main, cp_Send_Byte, i_Byte_Quan);
}

bool CWinSocket::READ_CHAR_MAIN(char * cp_Read_Byte, int * ip_Recv_Byte_Size, int i_Byte_Max_Limit)
{
	return this->READ_CHAR_SEL(this->m_uiSocket_Main, cp_Read_Byte, ip_Recv_Byte_Size, i_Byte_Max_Limit);
}

bool CWinSocket::CONN_Client(int i_Mode_TCP_UDP, const wchar_t * wcp_IP_Addr, int i_Port)
{
	try
	{
		//���Client�Ƿ�������
		if( this->m_bConn_Running_Enable == true )
		{
			//Client������
			return true;
		}

		//�ڶ����������׽��֡�����Socket,ʹ��TCP��UDPЭ��
		if(this->Create_Socket(i_Mode_TCP_UDP) == false)
		{
			throw(0);
		}

		//�����������ӷ�������
		if(this->Create_Client(wcp_IP_Addr, i_Port) == false)
		{
			throw(0);
		}

		//�趨Client�����ѽ�����ʶ
		this->m_bConn_Running_Enable = true;
	}
	catch(...)
	{
		//fail
		return false;
	}

	//pass
	return true;
}

void CWinSocket::DISCONN_Client(void)
{
	if( this->m_bConn_Running_Enable == true )
	{
		this->CLOSE_SOCKET_MAIN();//�ر����׽���
	}
}

bool CWinSocket::START_Server(int i_Mode_TCP_UDP, const wchar_t * wcp_IP_Addr, int i_Port)
{
	try
	{
		//�ڶ��������������׽��֣�sListen=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);��Ȼʹ�õ���socket������
		if( this->Create_Socket( i_Mode_TCP_UDP ) == false )
		{
			throw(0);
		}

		//���������󶨡�ʹ��bind�������ú����������ǽ�һ�������õ��׽��ְ󶨵����ص�ĳ����ַ�Ͷ˿��ϡ�
		//���Ĳ����������ӡ�ʹ��listen�������ú����ǽ�ָ�����׽�������Ϊ����ģʽ����ret = listen(sListen, 5)��
		if( this->Create_Server( wcp_IP_Addr, i_Port ) == false )
		{
			throw(0);
		}

		//�趨Server�����ѽ�����ʶ
		this->m_bConn_Running_Enable = true;
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

void CWinSocket::STOP_Server(void)
{
	//�ر����׽���
	this->CLOSE_SOCKET_MAIN();
	//
	WSACleanup();
}

bool CWinSocket::Create_Server(const wchar_t * wcp_IP_Addr, int i_Port)
{
	struct sockaddr_in strSock_Addr_In_Server;
	int iRet;
	char * cpIP_Addr = NULL;

	try
	{
		//����char�ڴ�
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpIP_Addr, (wcslen(wcp_IP_Addr) * 2 + 8));

		//IP��ַת��
		this->m_cla_GFC.WideChar_To_MultiByte(wcp_IP_Addr, cpIP_Addr, &iRet);

		//
		memset(&strSock_Addr_In_Server, 0, sizeof(strSock_Addr_In_Server));

		//�������ص�ַ��Ϣ
		strSock_Addr_In_Server.sin_family = AF_INET; //��ַ����
		strSock_Addr_In_Server.sin_port = htons((unsigned short)i_Port);//htons(SERVER_PORT); //ע��ת��Ϊ�����ֽ���
		strSock_Addr_In_Server.sin_addr.S_un.S_addr = htonl(ADDR_ANY);//htonl(ntohl(inet_addr(cpIP_Addr)));//htonl(INADDR_ANY);ʹ��INADDR_ANY ָʾ�����ַ


		//���������󶨡�ʹ��bind�������ú����������ǽ�һ�������õ��׽��ְ󶨵����ص�ĳ����ַ�Ͷ˿��ϡ�
		iRet = bind(this->m_uiSocket_Main, (struct sockaddr *)&strSock_Addr_In_Server, sizeof(strSock_Addr_In_Server));
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, bind(IP_Addr = %s, Port = %d), iRet = %d, Error_code = %d;"), wcp_IP_Addr, i_Port, iRet, WSAGetLastError());
			throw(0);
		}


		//���Ĳ����������ӡ�ʹ��listen�������ú����ǽ�ָ�����׽�������Ϊ����ģʽ����ret = listen(sListen, 5)��
		iRet = listen(this->m_uiSocket_Main, 12);//�ڶ����������ǵȴ����Ӷ��е����ĳ��ȡ�
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, listen(), iRet = %d, Error_code = %d;"), iRet, WSAGetLastError());
			throw(0);
		}


		//TCP���ӽ���, ��Ϣ�������
		iRet = WSAAsyncSelect(this->m_uiSocket_Main, this->m_hWnd_View, WM_SOCKET_SERVER, FD_ACCEPT | FD_READ| FD_WRITE | FD_CLOSE );
		if(iRet == SOCKET_ERROR) 
		{
			this->m_csError_Info.Format(_T("FAIL, WSAAsyncSelect(), iRet = %d, Error_code = %d;"), iRet, WSAGetLastError());
			throw(0);
		}


		//�ͷ�char�ڴ�
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpIP_Addr);
	}
	catch(...)
	{
		//�ر��׽���
		this->CLOSE_SOCKET_MAIN();
		//
		WSACleanup();

		//�ͷ�char�ڴ�
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpIP_Addr);

		//
		return false;
	}

	return true;
}

SOCKET CWinSocket::ACCEPT(sockaddr_in * strp_Addr_In)
{
	int iLength, iRet;
	SOCKET uiRec_Socket = INVALID_SOCKET;//�����׽���

	try
	{
		iLength = sizeof(*strp_Addr_In);//������������Ҳ��һ������ֵ��ָ�����͵ı����������˷��صĵ�ַ��Ϣ�ĳ��ȡ�

		//���岽�����ܿͻ��˵���������ʹ��accept�������ܿͻ��˷��͵���������
		//accept��������ֵ��һ���ͻ��˺ͷ��������ӵ�SOCKET���͵����������ڷ������˱�ʶ������ͻ��ˡ�
		uiRec_Socket = accept(this->m_uiSocket_Main, (struct sockaddr *)strp_Addr_In, &iLength);
		//
		if(uiRec_Socket == INVALID_SOCKET)
		{
			this->m_csError_Info.Format(_T("FAIL, accept(), strRec_Socket = %d, Error_code = %d;"), uiRec_Socket, WSAGetLastError());
			throw(0);
		}



		//����������д���رա���Ϣ����
		iRet = WSAAsyncSelect(uiRec_Socket, this->m_hWnd_View, WM_SOCKET_SERVER, (FD_READ | FD_WRITE | FD_CLOSE));
		//
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, WSAAsyncSelect( FD_READ FD_WRITE FD_CLOSE ), iRet = %d, Error_code = %d;"), iRet, WSAGetLastError());
			closesocket(uiRec_Socket);
			throw(0);
		}
	}
	catch(...)
	{
		return INVALID_SOCKET;
	}

	return uiRec_Socket;
}

CString CWinSocket::IP_NetworkByteOrder_To_CString(in_addr str_sin_addr)
{
	CString csIP;
	int iTemp;
	char crIP[128];
	wchar_t wcrIP[128];

	sprintf_s(crIP, "%s", inet_ntoa(str_sin_addr));

	this->m_cla_GFC.MultiByte_To_WideChar(crIP, wcrIP, &iTemp);

	csIP.Format(_T("%s"), wcrIP);

	return csIP;
}

CString CWinSocket::Port_NetworkByteOrder_To_CString(unsigned short us_sin_port)
{
	CString csPort;

	csPort.Format(_T("%d"), ntohs(us_sin_port));

	return csPort;
}

void CWinSocket::CLOSE_SOCKET_SEL(SOCKET * strp_Sel_Socket)
{
	//�رա�ѡ�еġ������׽���

	if( *strp_Sel_Socket != INVALID_SOCKET )
	{
		int iRet;

		iRet = closesocket( *strp_Sel_Socket );
		//
		if( iRet == SOCKET_ERROR )
		{
			this->m_csError_Info.Format( _T( "FAIL, closesocket( %d ), iRet = %d, Error_code = %d;" ), *strp_Sel_Socket, iRet, WSAGetLastError() );
			::AfxMessageBox( this->m_csError_Info );
		}

		//����ָ�����Ч��
		*strp_Sel_Socket = INVALID_SOCKET;

		//�����ѶϿ���ʶ
		this->m_bConn_Running_Enable = false;
	}
}

bool CWinSocket::SEND_CHAR_SEL(SOCKET ui_Sel_Socket, const char * cp_Send_Byte, int i_Byte_Quan)
{
	int iRet;

	try
	{
		//���Ĳ������͡�
		iRet = send(ui_Sel_Socket, cp_Send_Byte, i_Byte_Quan, 0);//send(sClient, (char *)&sendMessage, sizeof(sendMessage), 0);

		//
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, SEND_CHAR, iRet = %d;"), iRet);
			throw(0);
		}
	}
	catch(...)
	{
		//
		return false;
	}

	return true;
}

bool CWinSocket::READ_CHAR_SEL(SOCKET ui_Sel_Socket, char * cp_Read_Byte, int * ip_Recv_Byte_Size, int i_Byte_Max_Limit)
{
	int iRet;

	try
	{
		//���岽�����ա�
		iRet = recv(ui_Sel_Socket, cp_Read_Byte, i_Byte_Max_Limit, 0);

		//�ж�+�����ȡ�����ͽ��
		switch(iRet)
		{
		case SOCKET_ERROR://TCP���ӵĽ���BUFF���������ˣ����Բ��ٶ���
			*ip_Recv_Byte_Size = 0;
			break;
		case 0://�������Ͽ����򷵻ص��ǡ�0����If the connection has been gracefully closed, the return value is zero
			*ip_Recv_Byte_Size = 0;//�ͻ����Ѿ��ر�����
			throw(0);
		default:
			{
				if(iRet < i_Byte_Max_Limit)
				{
					//��ʵ�ʶ�����������С�ڡ�������ȡ������ȫ�����ˣ�������������
				}
				else
				{
					//��ʵ�ʶ��������������ڵ��ڡ�������ȡ������û�ж����ˣ�������ܻ�������
					//��Ҫ������ȡ+����
				}

				//
				*ip_Recv_Byte_Size = iRet;
			}
			break;
		}
	}
	catch(...)
	{
		//
		return false;
	}

	return true;
}

bool CWinSocket::Set_Send_And_Recv_Timeout(int i_Send_Timeout_mS, int i_Recv_Timeout_mS)
{
	int iRet;


	try
	{
		//����ʱ��
		iRet = setsockopt(this->m_uiSocket_Main, SOL_SOCKET, SO_SNDTIMEO, (char *)&i_Send_Timeout_mS, sizeof(i_Send_Timeout_mS));
		//
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, setsockopt, SO_SNDTIMEO, iRet = %d;"), iRet);
			throw(0);
		}


		//����ʱ��
		iRet = setsockopt(this->m_uiSocket_Main, SOL_SOCKET, SO_RCVTIMEO, (char *)&i_Recv_Timeout_mS, sizeof(i_Recv_Timeout_mS));
		//
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, setsockopt, SO_RCVTIMEO, iRet = %d;"), iRet);
			throw(0);
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}
