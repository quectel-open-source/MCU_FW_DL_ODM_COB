#pragma once

#include "IniFile.h"

class Tool_Update_Config
{
public:
	Tool_Update_Config ();
	~Tool_Update_Config ();


public:
	CIniFile m_iniFile;

	CString GetExePath ( void );
	void InitConfigPath ( CString csFileName );
	void SaveConfig ();
	CString	m_csServerName_Fac;//������
	int m_iPort;//�˿�
	CString m_csFactory_Info_Name;//�������ƣ�HF/JSD/WCL/XT/KD)

	CString m_csExe_Path;//   /* D:\Run\ASR_RF_CAL_V1.10.1\ */
	CString m_csExe_Name;//      ASR_RF_CAL_V1.10.1
	CString m_csSetting;// ��Ҫ���Ϲ���copy���¹��ߵ��ļ����ļ���,�ԷֺŸ���  Config.ini;Param

	CString m_csTool_Name;// ASR_RF_CAL

};

