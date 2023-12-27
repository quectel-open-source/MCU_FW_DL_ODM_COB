#pragma once

#include "gfc.h"
#include "mnbdl_lib.h"
#include "Com_Port.h"



class CFlash_OP_NB
{
public:
	HWND m_hWnd_View;
	int m_iCom_Port;
	int m_iBaud_Rate;

	bool m_bRuning;
	int m_iClass_Idx;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iStop_Flag;

	QTI_PROG_ARG m_str_Qti_Arg_Main;

	CCom_Port m_Com_Port;
	
public:
	CFlash_OP_NB(void);
	~CFlash_OP_NB(void);

	int Do_Execute();
	void Show_Status(const wchar_t * wcp_Status);

	void LoadFirst(void);
	void LoadInit(int i_TSCH_Numb , CProgressCtrl* pCtrProgress, CString csDA_File_Path,CString Scat_Cfg_File_Path,CString csLog_File_Path);

	int OpenMainPort();

	void Show_Progress(int i_Pgr_Percent);
	void Show_Out(const wchar_t * wcp_Status);
	bool m_bPLC_Flag;//��PLC���Ƶļо�
	int m_iPLC_Port;

	CGFC m_cla_GFC;

	//��ģ��������ȥ����ģʽ
	int RestModule();

	int   m_iComMethod;//0:UART  1:USB
	int   m_iResetFlag;//1:reset 0:no reset
	int   m_iClearNVFlag;//1:��NV, 0:�����NV

	int  m_iProdu_Type;//ͨ��OCP��ȡ�Ļ��ͣ���Ҫ�ǹ��������ж����������ǹ�����ʽ������

	bool  m_bSaveDebugLog;

	CString m_csLog_File_Path;
	CString m_csLog_File_Path_Full;

	bool Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size);

};
