#include "stdafx.h"
#include "MS_IMbnDevice.h"


CMS_IMbnDevice::CMS_IMbnDevice ()
{

}


CMS_IMbnDevice::~CMS_IMbnDevice ()
{

}

bool CMS_IMbnDevice::Open_IMbn_Device(BOOL bIsSleep)
{
	CComPtr<IMbnInterface>  pInf = NULL;
	MBN_INTERFACE_CAPS interfaceCaps;
	SAFEARRAY *psa = NULL;
	HRESULT hr = E_FAIL;
	LONG lLower;
	LONG lUpper;
	BSTR InterfaceID;
	CString csDeviceID;
	unsigned long _req_id;


	try
	{
		//���
		this->_mbn_dev_svc_mgr = NULL;
		this->_mnb_dev_svc_cxt = NULL;
		this->_mbn_dev_svc = NULL;
		this->g_InterfaceMgr = NULL;


		//INIT COM���
		hr = CoInitializeEx ( NULL , COINIT_MULTITHREADED );
		if ( FAILED ( hr ) )
		{
			TRACE(_T("INIT COM��� fail\n"));
			throw(0);
		}


		//����IMbnDeviceServicesManager
		hr = CoCreateInstance ( CLSID_MbnDeviceServicesManager ,
			NULL ,
			CLSCTX_ALL ,
			IID_IMbnDeviceServicesManager ,
			( void** ) &_mbn_dev_svc_mgr );
		if ( FAILED ( hr ) )
		{
			TRACE(_T("����IMbnDeviceServicesManager fail\n"));
			throw(0);
		}

		//����IMBN Interface Manager
		hr = CoCreateInstance ( CLSID_MbnInterfaceManager ,
			NULL ,
			CLSCTX_ALL ,
			IID_IMbnInterfaceManager ,
			( void** ) &g_InterfaceMgr );
		if ( FAILED ( hr ) )
		{
			TRACE(_T("����IMBN Interface Manager fail\n"));
			throw(0);
		}

		if (bIsSleep)
		{
			Sleep(3000);
		}

		//ͨ��IMBN Interface Manager��ȡ�豸��Ϣ
		hr = g_InterfaceMgr->GetInterfaces ( &psa );
		if ( FAILED ( hr ) )
		{
			TRACE(_T("ͨ��IMBN Interface Manager��ȡ�豸��Ϣ fail\n"));
			throw(0);
		}

		//��ȡ�豸LBound
		hr = SafeArrayGetLBound ( psa , 1 , &lLower );
		if ( FAILED ( hr ) )
		{

			TRACE(_T("��ȡ�豸LBound fail\n"));
			throw(0);
		}

		//��ȡ�豸UBound
		hr = SafeArrayGetUBound ( psa , 1 , &lUpper );
		if ( FAILED ( hr ) )
		{
			TRACE(_T("��ȡ�豸UBound fail\n"));
			throw(0);
		}


		if ( lLower <= lLower )
		{
			//�����豸lLower��lUpper��ȣ�
			hr = SafeArrayGetElement ( psa , &lLower , ( void* ) ( &pInf ) );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("�����豸lLower��lUpper��ȣ� fail\n"));
				throw(0);
			}

			//ͨ��IMbnInterface��ȡID
			hr = pInf->get_InterfaceID ( &InterfaceID );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("ͨ��IMbnInterface��ȡID fail\n"));
				throw(0);
			}

			//ͨ��InterfaceID��ȡMBN_INTERFACE_CAPS
			hr = pInf->GetInterfaceCapability ( &interfaceCaps );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("ͨ��InterfaceID��ȡMBN_INTERFACE_CAPS fail\n"));
				throw(0);
			}

			//ͨ��MBN_INTERFACE_CAPS��ȡDeviceID
			csDeviceID = interfaceCaps.deviceID;


			//ͨ��DeviceServicesManager + InterfaceID��ȡDeviceServicesContext
			hr = _mbn_dev_svc_mgr->GetDeviceServicesContext ( InterfaceID , &_mnb_dev_svc_cxt );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("ͨ��DeviceServicesManager + InterfaceID��ȡDeviceServicesContext fail\n"));
				throw(0);
			}


			//ͨ��ID����IMbnDeviceService
			// "{6427015F-579D-48F5-8C54-F43ED1E76F83}" ==> EM06-J
			hr = _mnb_dev_svc_cxt->GetDeviceService ( _T ( "{6427015F-579D-48F5-8C54-F43ED1E76F83}" ) , &_mbn_dev_svc );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("ͨ��ID����IMbnDeviceService fail\n"));
				throw(0);
			}

			//��DeviceService���ݴ���
			hr = _mbn_dev_svc->OpenCommandSession ( &_req_id );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("��DeviceService���ݴ��� fail\n"));
				throw(0);
			}

			//�ȴ��ȶ�(����)
			Sleep ( 2000 );
		}
		else
		{
			//û���豸
			TRACE(_T("û���豸 fail\n"));
			throw(0);
		}

	}
	catch ( ... )
	{
		return false;
	}

	return true;
}

bool CMS_IMbnDevice::Open_Debug_Port ()
{
	unsigned char data[2] = { 0x11 , 0x00 };
	unsigned char cid = 0x05;

	try
	{
		//��IMbn�豸
		if ( false == Open_IMbn_Device () )
		{
			throw( 0 );
		}

		//����ָ����Ϣ
		if ( false == Send_specific_cid_command ( cid , data ) )
		{
			throw( 0 );
		}

	}
	catch ( ... )
	{
		//Uninit COM���
		CoUninitialize ();
		return false;
	}

	//Uninit COM���
	CoUninitialize ();
	return true;
}

bool CMS_IMbnDevice::Close_Debug_Port ()
{
	unsigned char data[2] = { 0x00 , 0x11 };
	unsigned char cid = 0x05;
	try
	{
		//��IMbn�豸
		if ( false == Open_IMbn_Device (TRUE) )
		{
			TRACE(_T("pen_IMbn_Device fail\n"));
			throw( 0 );
		}

		//����ָ����Ϣ
		if ( false == Send_specific_cid_command ( cid , data ) )
		{
			TRACE(_T("Send_specific_cid_command fail\n"));
			throw(0);
		}

	}
	catch ( ... )
	{
		//Uninit COM���
		CoUninitialize ();
		return false;
	}

	//Uninit COM���
	CoUninitialize ();
	return true;
}

bool CMS_IMbnDevice::Open_QDloader_Port ()
{
	unsigned char data[2] = { 0x00 , 0x00 };
	unsigned char cid = 0x04;
	try
	{
		//��IMbn�豸
		if ( false == Open_IMbn_Device () )
		{
			throw( 0 );
		}

		//����ָ����Ϣ
		if ( false == Send_specific_cid_command ( cid , data ) )
		{
			throw( 0 );
		}

	}
	catch ( ... )
	{
		//Uninit COM���
		CoUninitialize ();
		return false;
	}

	//Uninit COM���
	CoUninitialize ();
	return true;
}

bool CMS_IMbnDevice::Send_specific_cid_command ( unsigned char cid , unsigned char data[] )
{
	HRESULT hr = E_FAIL;
	unsigned long _req_id_Ex;

	//����CComSafeArray ATLģ���
	CComSafeArray<BYTE> csa ( 2 , 0 );

	try
	{
		//��������CComSafeArray ATLģ���
		csa.SetAt ( 0 , data[0] );
		csa.SetAt ( 1 , data[1] );

		//CComSafeArrayת��LPSAFEARRAY
		LPSAFEARRAY *psa = csa.GetSafeArrayPtr ();

		//ִ�з���
		hr = _mbn_dev_svc->SetCommand ( cid , *psa , &_req_id_Ex );
		if ( FAILED ( hr ) )
		{
			throw( 0 );
		}
	}
	catch ( ... )
	{
		return false;
	}
	return true;
}