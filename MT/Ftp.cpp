#include "stdafx.h"
#include "Ftp.h"

CStringW CFtp::CStrA2CStrW(const CStringA &cstrSrcA)
{
	if (cstrSrcA.GetLength() <= 0)
	{
		return L"";
	}
	int len = MultiByteToWideChar(CP_UTF8, 0, LPCSTR(cstrSrcA), -1, NULL, 0);
	wchar_t *wstr = new wchar_t[len];
	memset(wstr, 0, len*sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, LPCSTR(cstrSrcA), -1, wstr, len);
	CStringW cstrDestW = wstr;
	delete[] wstr;
	wstr = NULL;
	return cstrDestW;
}

CFtp::CFtp()
{
	p_InternetSession = NULL;
	p_FtpConnection = NULL;
	m_b_FTP_Connect = false;
}


CFtp::~CFtp()
{
	DisConnect_FTP_Server();
}
/********************************************************************************************
void CFtp::DisConnect_FTP_Server()
Description:
�Ͽ�FTP����
Parameters:
Returns:
********************************************************************************************/
void CFtp::DisConnect_FTP_Server()
{
	try
	{
		//�Ͽ�FTP
		if (p_FtpConnection != NULL)
		{
			p_FtpConnection->Close();
			//delete
			delete p_FtpConnection;
			p_FtpConnection = NULL;
		}
		if (p_InternetSession != NULL)
		{
			delete p_InternetSession;
			p_InternetSession = NULL;
		}
	}
	catch (CInternetException * pEx)//error:can not connect to specific ftp
	{
		pEx->Delete();
	}
	m_b_FTP_Connect = false;
}
/********************************************************************************************
bool CFtp::Connect_FTP_Server(CString sServer, CString sUserName, CString sPassWord, int iPort)
Description:
����FTP����
Parameters:
1.sServer:��������ַ
2.sUserName:�û���
3.sPassWord:����
4.iPort:Ftp�˿�
Returns:
********************************************************************************************/
bool CFtp::Connect_FTP_Server()
{
	try
	{
		//����һ��CInternetSession����
		if (p_InternetSession != NULL)
		{
			DisConnect_FTP_Server();	
		}
		p_InternetSession = new CInternetSession();
		//����CFtpConnection���󲢷���һ��ָ��ö����ָ��
		p_FtpConnection = p_InternetSession->GetFtpConnection(
			m_FtpInfo.sServerName, m_FtpInfo.sUserName, m_FtpInfo.sPassword, m_FtpInfo.iPort, true);
	}
	catch (CInternetException * pEx)//error:can not connect to specific ftp
	{
		pEx->Delete();
		//
		DisConnect_FTP_Server();
	}
	if (p_FtpConnection == NULL)
	{
		return false;
	}
	m_b_FTP_Connect = true;
	return true;
}
/********************************************************************************************
bool CFtp::Create_FTP_Path(CString sPath)
Description:
����FTP·��
Parameters:
1.sPath:FTP·��
Returns:
********************************************************************************************/
bool CFtp::Create_FTP_Path(CString sFTPPath, CString &sError)
{
	int nDirIndex = -1;
	CString sDirectory = L"//", sPath = sFTPPath;
	if (sPath.GetLength()<3 || sPath.Left(2)!=L"//")
	{
		return false;
	}
	if (!Set_FTP_Path(sPath))
	{
		if (sPath.Right(1) != L"/")
		{
			sPath += L"/";
		}
		sPath = sPath.Right(sPath.GetLength() - 2);
		for (int i = 2;; i++)
		{
			nDirIndex = sPath.Find(L"/");
			if (nDirIndex < 0)
			{
				break;
			}
			sDirectory += sPath.Left(nDirIndex + 1);
			if (!Set_FTP_Path(sDirectory))
			{
				if (!p_FtpConnection->CreateDirectory(sDirectory))
				{
					sError.Format(L"CreateDirectory:(%s)Fail", sDirectory);
					return false;
				}
			}
			sPath = sPath.Right(sPath.GetLength() - nDirIndex - 1);
		}
	}
	return true;
}
/********************************************************************************************
bool CFtp::Delete_FTP_Path(CString sPath)
Description:
ɾ��FTP·��
Parameters:
1.sPath:FTP·��
Returns:
********************************************************************************************/
bool CFtp::Delete_FTP_Path(CString sPath)
{
	if (!p_FtpConnection->Remove(sPath))
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool CFtp::Get_Current_FTP_Path(CString& sPath)
Description:
��ȡ��ǰFTP��Ҫ������·��
Parameters:
1.sPath:FTP����·��
Returns:
********************************************************************************************/
bool CFtp::Get_Current_FTP_Path(CString& sPath)
{
	if (!p_FtpConnection->GetCurrentDirectory(sPath))
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool CFtp::Set_FTP_Path(CString& sPath)
Description:
���õ�ǰFTP��Ҫ������·��
Parameters:
1.sPath:FTP����·��
Returns:
********************************************************************************************/
bool CFtp::Set_FTP_Path(CString sPath)
{
	if (!p_FtpConnection->SetCurrentDirectory(sPath))
	{
		Connect_FTP_Server();
		if (!p_FtpConnection->SetCurrentDirectory(sPath))
		{
			return false;
		}
	}
	return true;
}
/********************************************************************************************
bool CFtp::Upload_File(CString sLocalFilePath, CString sFtpName)
Description:
�ϴ������ļ���FTP��������·��ΪSet_FTP_Path�������õ�·��
Parameters:
1.sLocalFilePath:�����ļ�ȫ·����
2.sFtpName:�������ļ���
Returns:
********************************************************************************************/
bool CFtp::Upload_File(CString sLocalFilePath, CString sFtpFileName)
{
	if (!p_FtpConnection->PutFile(sLocalFilePath, sFtpFileName))
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool CFtp::Download_File(CString sLocalFilePath, CString sFtpName)
Description:
��FTP�����������ļ������أ�·��ΪSet_FTP_Path�������õ�·��
Parameters:
1.sFtpFileName:�������ļ���
2.sLocalFilePath:�����ļ�ȫ·����
Returns:
********************************************************************************************/
bool CFtp::Download_File(CString sFtpFileName, CString sLocalFilePath)
{
	if (!p_FtpConnection->GetFile(sFtpFileName, sLocalFilePath, FALSE, FILE_ATTRIBUTE_NORMAL, INTERNET_FLAG_RELOAD, 1))
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool CFtp::Delete_File(CString sFileName)
Description:
ɾ���ļ�
Parameters:
1.sFtpName:�������ļ���
Returns:
********************************************************************************************/
bool CFtp::Delete_File(CString sFtpFileName)
{
	if (!p_FtpConnection->Remove(sFtpFileName))
	{
		return false;
	}
	return true;	
}
/********************************************************************************************
bool CFtp::Rename_File(CString sFileName, CString sFileNewName)
Description:
�������ļ�
Parameters:
1.sFtpFileName:�������ļ���
2.sFtpFileNewName:�µ��ļ���
Returns:
********************************************************************************************/
bool CFtp::Rename_File(CString sFtpFileName, CString sFtpFileNewName)
{
	if (!p_FtpConnection->Rename(sFtpFileName, sFtpFileNewName))
	{
		return false;
	}
	return true;
}
/********************************************************************************************
ULONG CFtp::Get_ServerFile_Size(CString sFtpFileName)
Description:
��ȡServer�ļ���С
Parameters:
1.sFtpFileName:�������ļ���
Returns:
�ļ���С
********************************************************************************************/
ULONGLONG CFtp::Get_ServerFile_Size(CString sFtpFileName)
{
	ULONGLONG ull_FileSize = 0;
	CFtpFileFind fFinder(p_FtpConnection);
	BOOL bFind = fFinder.FindFile(sFtpFileName);
	if (bFind)
	{
		fFinder.FindNextFile();
		ull_FileSize = fFinder.GetLength();
	}
	fFinder.Close();
	return ull_FileSize;
}
bool CFtp::Get_File_Size(const CString cs_Path, unsigned long& ulp_Size)
{
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;

	try
	{
		//��
		hFile_Handle = ::CreateFile(cs_Path,
			GENERIC_READ,				//�ļ��Ĳ������� Read only required
			FILE_SHARE_READ,			//�ļ��������� read share file
			NULL,						//�ļ���ȫ���� SecurityAttributes - none in Win95
			OPEN_EXISTING,				//�ļ����� how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//�ļ����� file attributes
			NULL	//�����Ϊ�㣬��ָ��һ���ļ���������ļ���������ļ��и�����չ���� handle of file with attributes to copy
			);

		if (hFile_Handle == INVALID_HANDLE_VALUE)
		{
			throw(0);
		}

		//��ȡ�ļ���BYTE��
		ulp_Size = ::GetFileSize(hFile_Handle, NULL);

		//�ر�
		::CloseHandle(hFile_Handle);
		hFile_Handle = INVALID_HANDLE_VALUE;
	}
	catch (...)
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool CFtp::Get_Files_From_Current_Path()
Description:
��ȡ��ǰĿ¼�µ������ļ���
Parameters:

Returns:
�ļ����洢��m_sFileList����
********************************************************************************************/
bool CFtp::Get_Files_From_Current_Path(CString sKey, CString sFileReject)
{
	CStringA sFileNameA = "";
	CString sFileName = L"", sFileLength = L"";
	//����յ�
	m_sFileList[0].RemoveAll();
	m_sFileList[1].RemoveAll();
	CFtpFileFind fFinder(p_FtpConnection);
	BOOL bFind = fFinder.FindFile(TEXT("*"));
	while (bFind)
	{
		bFind = fFinder.FindNextFile();
		if (fFinder.IsDots() || fFinder.IsDirectory())
		{
			continue;
		}
		sFileNameA = fFinder.GetFileName();
		sFileName = CStrA2CStrW(sFileNameA);
		if ((sFileName.Find(sKey) >= 0 || sKey.IsEmpty()) && sFileName != sFileReject)
		{
			m_sFileList[0].Add(sFileName);
			sFileLength.Format(L"%ld", fFinder.GetLength());
			m_sFileList[1].Add(sFileLength);
		}	
	}
	fFinder.Close();
	if (m_sFileList[0].GetCount() <= 0)
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool Find_File_In_Current_Path()
Description:��ѯ�ļ��Ƿ����
Parameters:

Returns:
TRUE/FAIL
********************************************************************************************/
bool CFtp::Find_File_In_Current_Path(CString sFileName)
{
	CFtpFileFind fFinder(p_FtpConnection);
	BOOL bFind = fFinder.FindFile(sFileName);
	if (bFind)
	{
		return true;
	}
	return false;
}