#pragma once

#include "gfc.h"
#include "Com_Port.h"
#include "PipeCommObj00.h"

typedef struct tagBIN_FILE_S_AG
{
	CString cs_Name;
	CString cs_Path;
	unsigned long ul_File_Size;
	bool b_Enable_DL;
}BIN_FILE_S_AG;

#define MAX_BIN_FILE  64

class CFlash_OP_AG
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
	CCom_Port m_Com_Port;

	CString m_csCurr_Exe_Path;
	CString m_csFlash_Path;

	CPipeCommObj00 m_PipeComm;			//�ܵ���	

public:
	CFlash_OP_AG(void);
	~CFlash_OP_AG(void);

	int Do_Execute();
	void Show_Status(const wchar_t * wcp_Status);

	void Show_Progress(int i_Pgr_Percent);
	void Show_Out(const wchar_t * wcp_Status);
	bool m_bPLC_Flag;//��PLC���Ƶļо�
	int m_iPLC_Port;

	CGFC m_cla_GFC;

	int   m_iComMethod;//0:UART  1:USB
	int   m_iResetFlag;//1:reset 0:no reset
	int   m_iClearNVFlag;//1:��NV, 0:�����NV

	int  m_iProdu_Type;//ͨ��OCP��ȡ�Ļ��ͣ���Ҫ�ǹ��������ж����������ǹ�����ʽ������

	bool  m_bSaveDebugLog;

	CString m_csLog_File_Path;
	CString m_csLog_File_Path_Full;

	BIN_FILE_S_AG m_strr_Bin_File_List[MAX_BIN_FILE];

	bool DL_ImageFile(const CString& strCmdExe);
	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);

};
