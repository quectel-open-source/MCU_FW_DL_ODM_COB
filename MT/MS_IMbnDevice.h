#pragma once

#include <atlbase.h>
#include "mbnapi.h"
#include "atlsafe.h"

/*
V1.0.0
{
	Author��Summer
	Date��2018-9-13
	Version��1.0.0
	Dependency Library��mbnapi_uuid.lib
	Remark��Create Version
	OS:WIN8/WIN10
	MSDN��https://docs.microsoft.com/en-us/windows/desktop/api/_mbn/
}
*/


class CMS_IMbnDevice
{
public:
	CMS_IMbnDevice ();
	~CMS_IMbnDevice ();

public:
	CComPtr<IMbnDeviceServicesManager> _mbn_dev_svc_mgr;
	CComPtr<IMbnDeviceServicesContext> _mnb_dev_svc_cxt;
	CComPtr<IMbnDeviceService> _mbn_dev_svc;
	CComPtr<IMbnInterfaceManager>  g_InterfaceMgr;

public:
	bool Open_IMbn_Device (BOOL bIsSleep = FALSE);
	bool Open_Debug_Port ();
	bool Close_Debug_Port ();
	bool Open_QDloader_Port ();
	bool Send_specific_cid_command ( unsigned char cid , unsigned char data[] );
};

