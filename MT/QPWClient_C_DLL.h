
#if !defined(_QPWCLIENT_C_DLL_H_)
#define _QPWCLIENT_C_DLL_H_


#ifdef QPWCLIENT_C_DLL_EXPORTS
#define QPWCLIENT_C_DLL_API __declspec(dllexport)
#else
#define QPWCLIENT_C_DLL_API __declspec(dllimport)
#endif


extern "C"
{

	/***********************************************************************************************
	*������       �� Query_Connect_Keyword
	*������������ �� ����ָ��������ѯ��ӦIP,PORT,USER,PASSWORD
	*��������     �� i_Input_Query_Type			:<input> ��ѯ����< 0 =  MAIN_SQL,1 = LOG_SQL ,2 = LOG_FTP>
	*                cp_Input_IP_Addr		    :<input> �����ʵ�IP��ַ(e.g.192.168.10.38)
	*                i_Input_Port			    :<input> �����ʵ�Port(e.g.8015)
	*                i_Input_Send_TimeOut_mS    :<input> ���ͳ�ʱ����(e.g.1000)
	*                i_Input_Recv_TimeOut_mS    :<input> ���ճ�ʱ����(e.g.1000)
	*		         cp_Output_IP_Addr          :<output> Target IP
	*                i_Output_Port              :<output> Target PORT
	*                cp_Output_User             :<output> Target USER
	*                cp_Output_Password         :<output> Target PASSWORD
	*��������ֵ   �� TRUE = �ɹ�;FALSE = ʧ��
	***********************************************************************************************/
	QPWCLIENT_C_DLL_API bool Query_Connect_Keyword(int i_Input_Query_Type,const char * cp_Input_IP_Addr, int i_Input_Port,int i_Input_Send_TimeOut_mS,int i_Input_Recv_TimeOut_mS,char * cp_Output_IP_Addr,int * i_Output_Port,char * cp_Output_User,char * cp_Output_Password);

	/***********************************************************************************************
	*������       �� Query_Connect_Keyword_Ex
	*������������ �� ����ָ��������ѯIP,PORT,USER,PASSWORD��Ӧ�ַ���
	*��������     �� i_Input_Query_Type			:<input> ��ѯ����< 0 =  MAIN_SQL,1 = LOG_SQL ,2 = LOG_FTP>
	*                cp_Input_IP_Addr		    :<input> �����ʵ�IP��ַ(e.g.192.168.10.38)
	*                i_Input_Port			    :<input> �����ʵ�Port(e.g.8015)
	*                i_Input_Send_TimeOut_mS    :<input> ���ͳ�ʱ����(e.g.1000)
	*                i_Input_Recv_TimeOut_mS    :<input> ���ճ�ʱ����(e.g.1000)
	*		         cp_Output_Target_Keyword   :<output> Target Keyword(e.g. "192.168.10.38,1433,ate,a@psd2*7e")
	*��������ֵ   �� TRUE = �ɹ�;FALSE = ʧ��
	***********************************************************************************************/
	QPWCLIENT_C_DLL_API bool Query_Connect_Keyword_Ex(int i_Input_Query_Type,const char * cp_Input_IP_Addr, int i_Input_Port,int i_Input_Send_TimeOut_mS,int i_Input_Recv_TimeOut_mS,char * cp_Output_Target_Keyword);

	/***********************************************************************************************
	*������       �� Get_Last_Error_Info
	*������������ �� ���ڵ�����Query_Connect_Keyword����FALSEʱ��ȡʧ����Ϣ
	*��������     �� cpErrorInfo               :<output> ����ʧ��log
	*��������ֵ   �� ��
	***********************************************************************************************/
	QPWCLIENT_C_DLL_API void Get_Last_Error_Info(char* cpErrorInfo);//��ȡʧ����Ϣ
}



#endif // #if !defined(_QPWCLIENT_C_DLL_H_)

