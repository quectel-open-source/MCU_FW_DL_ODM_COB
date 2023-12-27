#pragma once
#include "gfc.h"
#include "IniFile.h"
#include "Com_Port.h"


class CARM_OP_ALTAIR
{
public:
	CARM_OP_ALTAIR(void);
	~CARM_OP_ALTAIR(void);

	HWND m_hWnd_View;
	bool m_bRuning;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iClass_Idx;
	int m_iCom_Port;
	CString m_csCurr_Exe_Path;
	int m_iBaud_Rate;
	CGFC m_cla_GFC;
	CString m_strFactoryImagePath; // ������golden image·��
	CString m_strFWFolderPath;     // RK_03_00�ļ���Ŀ¼
	CString m_strAppFW;            // burns Application FW
	CString m_strModemFW;          // burns Modem FW
	CString m_strModemFW2;         // burns Modem FW2
	CString m_strChipType;         // оƬ����
	bool m_bIsBaudHighSpeed;       // �ǵ��ٻ��Ǹ������أ�Ĭ�ϸ��٣�1
	int m_iFlowControl;            // Flow Control��None,0��RTS_CTS:1
	int m_iDLMode;                 // оƬ״̬��0�ǿ�Ƭģʽrecovery mode and burn golden image��1�Ƿǿ�Ƭģʽburn golden image��Ĭ����0,��������ʽ����
	int m_iProdu_Type;//ͨ��OCP��ȡ�Ļ��ͣ���Ҫ�ǹ��������ж����������ǹ�����ʽ������
	CCom_Port m_cla_Com_Port;


	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	void Show_Progress(double d_Pgr_Percent);
	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);
	void Show_Status(const wchar_t * wcp_Status);
	void Show_Out(const wchar_t * wcp_Status);
	bool LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path);
	int Do_Execute(void);
	bool CheckModuleMode();
	bool DL_FactoryRecoveryModeFW();
	bool DL_FactoryFW();  // ǰ��ȷ���ǿ�Ƭ�������ø�ģʽ��ʽ������
};


