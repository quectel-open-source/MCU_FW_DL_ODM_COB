#pragma once

#include "IniFile.h"

class UpdateConfig
{
public:
	UpdateConfig();
	~UpdateConfig();

public:
public:
	CIniFile m_iniFile;
	wchar_t m_wcrCurr_Exe_Path[MAX_PATH];//���ߵ�ǰ·��

	void InitConfigPath(CString csFileName);
	void SaveConfig();
	CString	m_csServerName_Fac;//������
	bool m_bOCP; //BOOLתbool����warning
	bool m_bWrite_Check;
	CString m_csSave_Path;
	CString m_csFactory_Info_Name;//�������ƣ�HF/JSD/WCL/XT)
};

