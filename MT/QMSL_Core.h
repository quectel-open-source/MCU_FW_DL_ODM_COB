/******************************************************************************/
/**
Program: QMSL

$Id: //depot/HTE/QDART/QMSL6/inc/QMSL_Core.h#3 $

\code

- Defines the "C" interface for all Factory Library functions.

- Table of contents (text search can be done for the following sections)

- General Information
- Related Documents
- Version History
- Callback function definitions

- Connection/Disconnection Functions
- Text LoggingQ
- Diagnostic subsystem
- LTE Diag
- HDR Diag
- GSM Diag
- Streaming Download Diag
- Diagnostic commands
- Phone async Logging
- Phone sync (command) Logging
- Keypad Diag
- Handset Diag
- MediaFLO Diag
- CGPS Diag
- ISDB-T Diag
- MBP Diag

- Commands for multiple FTM modes
- Dual RX Chain FTM RF Commands
- FTM CDMA RF Calibration API V2
- GSM FTM
- GSM Polar Tx Cal FTM
- GSM Linear Tx FTM
- PMIC FTM
- cdma2000 FTM
- cdma2000 FTM non-signaling
- EVDO Non-signaling FTM
- Camera FTM
- FTM Log

- WCDMA BER FTM
- HSDPA BER FTM

- GSM BER FTM
- EGPRS BER FTM
- UMTS SINGLE ENDED BER FTM
- GSDI Diag
- AGPS FTM
- Common RF FTM
- LTE Non-signaling FTM
- MediaFLO FTM
- MediaFLO FTM UBM
- MediaFLO NS FTM
- DVB-H Diag Commands
- WLAN FTM
- QFUSE FTM
- GPS FTM


- Software Download
- EFS Functions

- Calibration Data Manager
- NVTool

- FTM Sequencer
- Band Class Helper
- WP7 PVK

------------------------------------------------------


----------------------
General Information
----------------------

- All data types used in this interface are ANSI C compatible.

- All returns that indicate "true if succeed, false if fail" are defined
as:  1 = true = Success , 0 = false = failure

Functions that are not entirely upper case do not correspond directly to
an FTM or diagnostic command.

For example, QLIB_ConnectServer() is not mapped directly to a diagnostic
command.


- Variable Names
Variable names are prefixed in lower case to indicate the data type.
The following are defined:

p = Pointer
i = Integer type, either char, short, or long
d = Double or floating point
e = Enumeration (possibly stored as a short, but representing
a fixed list of possibilities)
b = unsigned char, 0=false, 1=true

----------------------
Related Documents
----------------------

- The function name convention used in this document is as follows:

QLIB_ indicates that this function corresponds to library function
in the QLib project.

From there, all diagnostic and FTM functions are named exactly as
the command identifiers are defined in the QLib_Defines.h.

- For a list of all document numbers referred to by QMSL, please see following section in the QDART Help file:
1) QDART -> QDART Related Documents
or  2) QDART -> QMSL -> Getting Started -> References.


\endcode


-----------------------
Version History
-----------------------

\b QUALCOMM  \b PROPRIETARY

This document contains propriety information, and except with written
permission of Qualcomm Technologies INC, such information shall not be
published, or disclosed to others, or used for any purpose, and the
document shall not be duplicated in whole or in part.


Copyright (c) 2004-2012 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

\version
Version information is stored in the file "QMSL_Release_Notes.txt"

\note
Compiler:  Microsoft Visual C++ v6.0 SP4
*******************************************************************************/

#if !defined(_QLIB_CORE_H)	//为了避免同一个文件被include多次
#define _QLIB_CORE_H

#if !defined(_WINDOWS_)
#include "windows.h"
#endif

/*
Establish whether the library is being compiled into a DLL (exporting),
or being included from a client (importing)

When the DLL is built, then QLIB_EXPORTS should be defined
*/
#ifdef QLIB_API
#undef QLIB_API
#endif

#ifdef QLIB_EXPORTS
#define QLIB_API __declspec(dllexport)	//将该函数作为导出函数导出动态库
#else
#define QLIB_API __declspec(dllimport)	//从一个动态库中导入该函数
#endif

#if defined(QLIB_STATIC)
#undef QLIB_API
#define QLIB_API
//#define QLIB_API __cdecl
#endif

/*
__cplusplus是cpp中的自定义宏，那么定义了这个宏的话表示这是一段cpp的代码。
也就是说，上面的代码的含义是:如果这是一段cpp的代码，那么加入extern "C"{和}处理其中的代码，对编译器提示使用C的方式来处理函数。
*/
#ifdef __cplusplus
extern "C" {
#endif


	/******************************************************************************
	Callback function definitions
	*******************************************************************************/
#if !defined(DIAG_FS_MAX_FILENAME_LEN)
#define DIAG_FS_MAX_FILENAME_LEN		80 /* Specified by EFS2              */
#define DIAG_FS_MAX_PATHNAME_LEN		128 /* Specified by EFS2              */
#endif
#define QLIB_EFS_MAX_FILENAME_LEN		DIAG_FS_MAX_PATHNAME_LEN
	
	typedef unsigned char( *EfsDirCB )
		(
		char name[QLIB_EFS_MAX_FILENAME_LEN],  //defined in DiagEfsPkt.h
		unsigned char isFile,

		//attributes follow below
		unsigned short iAttributeMask,
		unsigned char iBufferingOption,
		unsigned char iCleanupOption,
		unsigned long iCreateDate,
		unsigned long iFileSize,
		HANDLE hContextID
		);

	/**
	Call back for EFS functions
	*/
	typedef unsigned char (*EfsFileTransferCB)
		(
		char srcname[512],
		char dstname[512],
		int oper,
		int suboper,
		unsigned long bytestransferred,
		unsigned long filesize,
		HANDLE hContextID
		);


	/**
	Call back for general SW Download event( replaces the other non-EFS call backs)

	Optional for NV backup/restore and normal download.  Must be used for boot loader and
	multimage download
	*/
	typedef unsigned char( *generalSWDownloadCB )
		(
		unsigned char* pEventStructure	// type union generalSwDownloadEvent_union, as defined in SoftwareDownload.h
		// Not declared explicitly here because general users do not need to
		// include the SWDownload.h file.
		);

	/**
	Call back for receipt of async messages

	Optional for NV backup/restore and normal download.  Must be used for boot loader and
	multimage download

	\param iMessageSize = number of bytes in the message
	\param pMessageBuffer = pointer to a buffer of memory with the message.  This must be copied by the
	user because after the call back is called, the buffer can be changed at
	any time
	\param hContextID = the same context ID used for function calls on a certain COM port.


	*/
	typedef void( *asyncMessageCB )
		(
		unsigned short iMessageSize,
		unsigned char* iMessageBuffer,
		HANDLE hContextID
		);


	/**
	Call back for user-defined send (PC to mobile) function.
	To be used with QLIB_ConnectServer_UserDefinedTransport()


	\param hQMSL_ContextID = the QMSL that is associated with the connection.
	\param hUserContextID = the context ID that was passed to QLIB_ConnectServer_UserDefinedTransport()
	\param iRequestSize = Number of bytes to be sent in the request packet.
	\param piRequestBytes = Pointer to a buffer of request packet contents.
	\param piActualWriteSize = to be updated by the user function, the actual # of bytes that were written

	\return 0 if successful (ERROR_SUCCESS) otherwise, an error code as defined by WINERR.H error codes,
	usually the value of ::GetLastError

	http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/system_error_codes__0-499_.asp
	*/
	typedef unsigned long( *userDefinedSend )
		(
		HANDLE hQMSL_ContextID,
		HANDLE hUserContextID,
		unsigned long iRequestSize,
		unsigned char* piRequestBytes,
		unsigned long* piActualWriteSize
		);

	/**
	Call back for user-defined receive (from mobile to PC) function.
	To be used with QLIB_ConnectServer_UserDefinedTransport()

	\param hQMSL_ContextID = the QMSL that is associated with the connection.
	\param hUserContextID = the context ID that was passed to QLIB_ConnectServer_UserDefinedTransport()
	\param iRequestSize = Number of bytes to be sent in the request packet.
	\param piRequestBytes = Pointer to a buffer of request packet contents.
	\param piResponseSize = Pointer to number of bytes received in the response packet
	NOTE: when calling this function, the value must be equal to the
	maximum size of the receive buffer.  When the function returns, the
	value will be the actual number of bytes filled into the receive buffer.
	\param iResponseBytes = Pointer to a buffer to store the response packet contents.

	\return 0 if successful (ERROR_SUCCESS) otherwise, an error code as defined by WINERR.H error codes,
	usually the value of ::GetLastError

	http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/system_error_codes__0-499_.asp

	*/
	typedef unsigned long( *userDefinedReceive )
		(
		HANDLE hQMSL_ContextID,
		HANDLE hUserContextID,
		unsigned long* piResponseSize,
		unsigned char* piResponseBytes
		);

	/**
	Call back for user-defined function to flush Tx and Rx buffers.

	\param hQMSL_ContextID = the QMSL that is associated with the connection.
	\param hUserContextID = the context ID that was passed to QLIB_ConnectServer_UserDefinedTransport()
	\return 0 if successful (ERROR_SUCCESS) otherwise, an error code as defined by WINERR.H error codes,
	usually the value of ::GetLastError

	http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/system_error_codes__0-499_.asp

	*/
	typedef unsigned long( *userDefinedFlushTxRx )
		(
		HANDLE hQMSL_ContextID,
		HANDLE hUserContextID

		);

	/**
	Call back for NV Tool functions

	\param hQMSLContext = the QMSL context that is associated with the connection.
	\param iNVid		= NV item ID
	\param iSourceFunc	= The NV Tool function call, See QMSL_NVTool_CallBack_Source_Enum in QLib_Defines.h
	\param iEvent       = The status from loading or writing the NV item.  See QMSL_NVTool_CallBack_Event_Enum in QLib_Defines.h
	\param iProgress    = Progress, 0 to 100 percent
	*/
	typedef void ( *nvToolCB )
		(
		HANDLE hQMSLContext,
		unsigned short iNVid,
		unsigned short iSourceFunc,
		unsigned short iEvent,
		unsigned short iProgress
		);

	/**
	Call back for QLIB_QPHONEMS_UploadSBMultiImage

	\param hQMSLContext = the QMSL context that is associated with the connection.
	\param sMsg			= NULL terminerated character string message
	\param iMsgSize		= The lenght of the sMsg
	*/
	typedef void ( *swdlQPHONEMSCB )
		(
		HANDLE hQMSLContext,
		char	*sMsg,
		unsigned short iMsgSize
		);

	/******************************************************************************
	Connection/Disconnection Functions
	*******************************************************************************/

	/******************************************************************************/
	/**

	Connect the server to a certain COM port number

	\param iComPort = COM port number to use for communication with phone.
	This does not open the COM port at the operating system
	level until a call is made to the embedded target.


	For auto detection, the COM port passed in can be
	QLIB_COM_AUTO_DETECT, and the first attached phone will
	be used automatically.

	For a NULL connection, the COM port passed in should be
	QLIB_COM_NULL.

	\return HANDLE to use for all subsequent QLIB calls that require a handle.
	If NULL, then no valid phone was found.

	*******************************************************************************/
	QLIB_API HANDLE QLIB_ConnectServer( unsigned int iComPort );

	/******************************************************************************/
	/**

	Connect the server to a certain COM port number

	\param iComPort = COM port number to use for communication with phone.
	This does not open the COM port at the operating system
	level until a call is made to the embedded target.

	For auto detection, the COM port passed in can be
	QLIB_COM_AUTO_DETECT, and the first attached phone will
	be used automatically.

	For a NULL connection, the COM port passed in should be
	QLIB_COM_NULL.


	\param iWait_ms = # of milliseconds to wait before an error is considered.

	\return HANDLE to use for all subsequent QLIB calls that require a handle.
	If NULL, then no valid phone was found.

	*******************************************************************************/
	QLIB_API HANDLE QLIB_ConnectServerWithWait( unsigned int iComPort, unsigned long iWait_ms );

	/******************************************************************************/
	/**

	Connect to a specified logical port number.  Before this function is called,
	a logical<-->physical mapping must be created using the function: QLIB_AddPortAliasEntry()


	\param iLogicalPort = Logical number to create a connection for.

	\return void

	*******************************************************************************/
	QLIB_API HANDLE QLIB_ConnectServer_LogicalPort( unsigned int iLogicalPort );

	/******************************************************************************/
	/**

	Connect the server to a certain COM port number with diagnostic command ECHO(0x7B)
	as handshake for QPHONEMS mode

	In QPST mode, this function is same as QLIB_ConnectServer

	In QPHONEMS mode, the function will send two diagnostic command, ECHO(0x7B), as handshake during port opening.

	Use this function to connect to a Windows Mobile target booting directly into FTM mode. (NV_FTM_I(453) == 1)

	\param iComPort = COM port number to use for communication with phone.
	This does not open the COM port at the operating system
	level until a call is made to the embedded target.

	For auto detection, the COM port passed in can be
	QLIB_COM_AUTO_DETECT, and the first attached phone will
	be used automatically.

	For a NULL connection, the COM port passed in should be
	QLIB_COM_NULL.

	\return HANDLE to use for all subsequent QLIB calls that require a handle.
	If NULL, then no valid phone was found.

	*******************************************************************************/
	QLIB_API HANDLE QLIB_ConnectServerWithHandShake( unsigned int iComPort );
	/******************************************************************************/
	/**

	Set the state of a flag which determines whether the DATA 2 DIAG switch
	should be attempted when there is a IsPhoneConnected() failure.

	DATA 2 DIAG is a feature of the "AT" data mode, which allows the "diagnostic"
	and "data" channels to the MSM to be multiplexed on the same serial path.
	This is used only for RS-232 communication, not USB.

	The default setting of the library will be to not have DATA 2 DIAG mode enabled.
	This will make it faster to detect whether or not a phone is connected to a certain
	COM port.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bSwitchData2Daig = true to attempt AT command to change from data mode to diag mode.

	\return True if the library is not in QPST mode.

	\warning This function does not apply when the library is in QPST mode.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetData2DiagSearch( HANDLE hResourceContext, unsigned char bSwitchData2Daig );

	/******************************************************************************/
	/**

	Setup a connection using a user-defined transport layer.  The user defined transport
	consists of three functions--one to send data, another to receive data, and the last
	to flush the Rx buffer.


	These functions	will be called by QMSL when QMSL needs to access the physical layer.

	The user also has the option of having QMSL to handle the HDLC formatting (including CRC)
	seperately for the send and receive packets.

	Before calling this function, the communication channel should be already established
	so that QMSL can make calls to the user functions immediately.

	Until DisconnectServer() is called, a background thread will be running to continually
	read the COM port and check for received data.

	This function will automatically set the library into 'QPHONEMS' mode by calling
	QLIB_SetLibraryMode() with a parameter of FALSE.  The user-defined mode is a sub
	mode of the the internal engine, QPHONEMS.

	\param hUserHandle = User defined handle, to be passed when the send or receive functions are called.
	This value can be any number except for -1 (0xFFFF).  When QLIB_GetComPortNumber()
	is called, this value will be returned.

	\param pUserDefinedSend = User send function, to be called when QMSL will send data to the mobile

	\param pUserDefinedReceive = User defined receive function, to be called when QMSL will receive data
	from the mobile

	\param pUserDefinedFlushRx = user defined function to flush the rx buffer

	\param bQMSL_HandlesTxHDLC = TRUE if QMSL is to handle the HDLC tasks when sending a packet,
	including escape sequences, CRC, and trailing flag.

	This flag should always be set to TRUE because this is the only mode that
	QMSL is tested in.

	If this flag is set to FALSE then only synchronous messages can be recieved.

	\param bQMSL_HandlesRxHDLC = TRUE if QMSL is to handle the HDLC tasks when receiving a packet,
	including escape sequences, CRC, and trailing flag.

	This flag should always be set to TRUE because this is the only mode that
	QMSL is tested in.

	If this flag is set to FALSE then only synchronous messages can be recieved.

	\return void

	*******************************************************************************/
	QLIB_API HANDLE QLIB_ConnectServer_UserDefinedTransport(
		HANDLE hUserHandle,
		userDefinedSend pUserDefinedSend,
		userDefinedReceive pUserDefinedReceive,
		userDefinedFlushTxRx pUserDefinedFlushRx,
		unsigned char bQMSL_HandlesTxHDLC,
		unsigned char bQMSL_HandlesRxHDLC
		);

	/******************************************************************************/
	/**

	Pause the receive thread for a specific device context.  This can only be used
	when the library is in the QPHONEMS mode, and the sub-mode of user
	defined transport layer mode.

	This function can be used to prevent QMSL from accessing the COM port during the
	time that the operation is paused.

	Call QLIB_ResumeDataReceive() to resume operation

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if failure.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_PauseDataReceive( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Resumes receive operations that were paused by calling QLIB_PauseDataReceive().

	This can only be used when the library is in the QPHONEMS mode, and the sub-mode,
	of user defined transport layer mode.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if failure.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_ResumeDataReceive( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Sets the maximum size of how much data will be written to a serial device in one
	call to the Windows serial device call, ::WriteFile().

	In order words, if a packet is larger than this value, then it will be broken
	into smaller sections, with the size of "iSendDataWriteSizeLimit"

	This function is needed for some USB->RS232 converters, which allow only 1k of
	data to be written at a time.  Most USB and PC hardware can allow larger
	transfer sizes.

	This function is only useful for very large packets, such as EFS Write transfers.

	This can only be used when the library is in the QPHONEMS mode. It is ignored
	in the QPST mode and the "user defined transport layer" mode.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSendDataWriteSizeLimit = maximum size of data written to the serial device per call to
	the Windows serial device driver.  Default size is 1000 at
	the time the library starts

	\return true if successful, false if failure.

	\warning WARNING: If this number is set higher that 1500, then some phones will have diagnostic
	buffer overflows.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetSendDataSerialWriteSizeLimit(
		HANDLE hResourceContext,
		unsigned long iSendDataWriteSizeLimit );

	/******************************************************************************

	In QPHONEMS or UserDefinedMode, QLibWorkerThreadProc is responsible for poll data from serial port or user defined
	receive function.

	This function sets the sleep counter frequency used in QPHONEMS or User Defined Transport Mode in the QLiWorkerThreadProc function

	This function has no effect when QPST mode is used.

	/param iSleepCounter, Sleep counter. Default value is 1 if the function is not called.
	Valid value is 1 or above.

	Use this function with caution.

	Increasing the value may improve the QMSL throughput of receiving data from the transport layer.
	However, this will also increase the CPU usage.

	The performance gain can be realized in situation where large amount of
	data are streamed from mobile to PC.  The sample scneario are NV items back up or GPS I/Q capture.

	The following code snipplet shows the worker thread for QMSL to receive data in QPHONEMS or Userdefined transport mode.

	// Simplied version of QlibWorerThreadProc
	// iSleepCounter is default to 1. i.e. Sleep(1) during each iteration;

	dword WINAPI QLibWorkerThreadProc( LPVOID pParam )
	{
	unsigned long = count;

	while( thread is running )
	{
	if(( count % iSleepCounter) == 0)
	{
	Sleep(1);
	count++
	}

	// Ask user defined receive or QPHONEMS for data
	GetDataFromQhoneMSorUserDefined();
	}
	}

	The following data can be used as reference for the performance gain

	On a MDM9600 dongle, backing up 6200 NV items will take

	1) 28.5 seconds with QPHONEMS and default value of 1
	2) 22.5 seconds with QPHONEMS and default value of 2
	4) 23.5 seconds with QPST as trasnport layer


	*******************************************************************************/
	QLIB_API unsigned char QLIB_Set_QPHONEMS_Userdefined_Sleep_Counter(HANDLE hResourceContext, unsigned long iSleepCounter);
	/******************************************************************************/
	/**

	Disconnect the server and close the COM port associated with the server.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return void

	*******************************************************************************/
	QLIB_API void QLIB_DisconnectServer( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Disconnects all servers and closes the COM ports associated with those server.

	Normally, this function does not need to be called, because a call to
	QLIB_DisconnectServer() can be done for each reasource that is opened.

	This function is to be used for cases when multiple connections are made to different
	COM ports during the lifetime of the application, or in the case of emergency/exception
	shut down.

	\return void

	\warning All handles will be invalid after this function is called.

	*******************************************************************************/
	QLIB_API void QLIB_DisconnectAllServers( void );

	/******************************************************************************/
	/**

	Uses the port list in the system registry to determine which COM ports are available
	on the PC and returns the information via iNumPorts and pPortList.  This function
	is only tested on Windows XP.

	\param iNumPorts = input/output, The input value is the maximum number of entries that
	can be added to the pPortList array.  The output value is updated to
	reflect how many valid ports were found on the system.

	\param pPortList = output, byte array.  Each byte indicates a COM port number that is
	available on the system.

	\return true if operation was able to complete successfully and at least one
	valid port is found.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GetAllPortList( unsigned short* iNumPorts, unsigned short* pPortList  );

	/******************************************************************************/
	/**

	Uses GetAllPortList() to determine which ports have phones available, then uses
	attempts a connection on each of the ports returned.

	\param iNumPorts = input/output, The input value is the maximum number of entries that
	can be added to the pPortList array.  The output value is updated to
	reflect how many valid ports were found on the system.

	\param pPortList = output, unsigned char array.  Each unsigned char indicates a COM port number that is
	available on the system.

	\param iNumIgnorePorts = # of ports in the "ignore port list."  Zero to check all ports
	\param pIgnorePortList = a list of port numbers that are to be ignored.  This can speed up
	the auto detection of ports.

	\return true if operation was able to complete successfully and at least one
	valid port is found.

	\warning 1) this function will take about 1 second for each port that is on the system

	2) All connections will be closed before searching begins.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GetAvailablePhonesPortList(
		unsigned short* iNumPorts, unsigned short* pPortList,
		unsigned short iNumIgnorePorts, unsigned short* pIgnorePortList );

	/******************************************************************************/
	/**

	Returns the COM port number associated with a specific resource context.  This
	can be used to determine the COM port number for a phone that was connected
	to automatically.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piPhysicalPort = output, physical port number, such as a 6 for COM6

	\return true if COM port could be determined successfully

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GetComPortNumber(HANDLE hResourceContext , unsigned short* piPhysicalPort );


	/******************************************************************************/
	/**

	Clears the port alias list.

	The port alias list is a method for mapping a logical port number to a physical
	port number.  This allows users to respond to a situation in which USB ports
	are mapped to unique numbers, depending on which devices have been installed in
	the past.

	An example of a port alias list:

	Logical    Physical
	Port       Port
	-------    --------
	1         COM7
	2         COM19
	3         COM6
	4         COM30

	The port mapping process can be facilitated by the GetAvailablePhonesPortList()
	function, which returns a list of which ports have a phone installed.  A "calibration"
	procedure could be setup, which tells the user to plug one phone into a port, then
	records which port the phone is in, then instructs them to plug the phone into the
	next USB port, and so on.

	During this procedure, the user software would create a logical list of ports
	(like 1,2,3,4) and remember which physical COM port number is associated with
	each logical port.  It is the user's (user of QLIB) responsibility to store this
	association in a persistant way (such as the registry or INI file), then when
	their software starts up the software would first call ClearPortAliasList(),
	then call AddPortAliasEntry() for each logical/physical port association.

	NOTE: It is not necessary to ClearPortAliasList(), if not alias have been created
	in the current run-time session.  In other words, the alias list is empty
	by default, and is only filled in by users of QLIB.

	After that, they could call the QLIB_ConnectServer_LogicalPort() function, to
	connect to a logical port number that has been configured.

	\return true if port alias list is successfully cleared.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_ClearPortAliasList( void );

	/******************************************************************************/
	/**

	Add an entry to the port alias list.  This function should be called once for
	each logical/physical port association.

	See ClearPortAliasList() for more information about port aliasing

	\param iLogicalPort = logical port number, user defined number
	\param iPhysicalPort = physical port number, such as a 6 for COM6

	\return true if port alias is successfully updated.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_AddPortAliasEntry(unsigned short iLogicalPort, unsigned short iPhysicalPort );

	/******************************************************************************/
	/**

	Configures call back functions.

	If one of the pointers is NULL, then the call back will be disabled for
	that call back type.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param pEfsDirCallback   = Callback for the EfsDirectory() operation
	\param pGeneralSwDownloadCB = Callback for download events
	\param pAsyncMessageCB = Callback for filtering async messages

	\return None

	*******************************************************************************/
	QLIB_API void QLIB_ConfigureCallBacks
		(
		HANDLE hResourceContext,
		EfsDirCB pEfsDirCallback,
		generalSWDownloadCB pGeneralSwDownloadCB,
		asyncMessageCB pAsyncMessageCB
		);

	/******************************************************************************/
	/**

	Configures EFS2 call back functions.

	If one of the pointers is NULL, then the call back will be disabled for
	that call back type.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param pEfsCallback = Callback for general EFS2 operations
	\param EfsDirCB = Callback for EFS Directory operations

	\return None

	*******************************************************************************/
	QLIB_API void QLIB_ConfigureEfs2CallBacks
		(
		HANDLE hResourceContext,
		EfsFileTransferCB pEfsCallback,
		EfsDirCB pEfsDirCB
		);


	/******************************************************************************/
	/**

	Configure Library Text Message call back function.

	The library text message call back is called each time a text message is printed
	to the log file.  The call back structure will contain the message level (e.g. LOG_IO
	or LOG_FN), and the NULL-terminated string containing the message text.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param pAsyncMessageCB = Callback for library generated text messages. The "unsigned char*" pointer
	will be of the structure type: QMSL_TextLog_struct, defined in QLib_Defines.h
	Specify a NULL value to disable this callback.

	\return None

	*******************************************************************************/
	QLIB_API void QLIB_ConfigureLibraryTextLogCallBack
		(
		HANDLE hResourceContext,
		asyncMessageCB pAsyncMessageCB
		);

	/******************************************************************************/
	/**

	Configures a specific timeout value.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eTimeOutId = Identifier index of timeout to change, index defined by
	QMSL_TimeOutType_Enum in QLibDefines.h
	\param iNewValue_ms = number of millseconds for timeout value

	\return None

	*******************************************************************************/
	QLIB_API unsigned char QLIB_ConfigureTimeOut
		(
		HANDLE hResourceContext,
		unsigned long eTimeOutId,
		unsigned long iNewValue_ms
		);

	/******************************************************************************/
	/**

	Get a specific timeout value.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eTimeOutId = Identifier index of timeout to change, index defined by
	QMSL_TimeOutType_Enum

	\return timeout value in milliseconds

	\return None

	*******************************************************************************/
	QLIB_API unsigned long QLIB_GetTimeOut( HANDLE hResourceContext, unsigned long eTimeOutId );

	/******************************************************************************/
	/**

	CDMA ICD, 3.4.123 Diagnostic Protocol Loopback Request/Response

	Pings the phone using the diagnostic command DIAG_PROTOCOL_LOOPBACK_F.

	Additionally verifies the connection using the dignostic version command DIAG_VERNO_F.

	This command has a timeout value that can be checked or modified using the identifier
	QMSL_Timeout_IsPhoneConnected when calling QLIB_ConfigureTimeOut() or QLIB_GetTimeOut()

	The default timeout is 200ms, so that as little time as possible will be spent to wait
	for a phone request in the event that a phone is not present.  Other diagnostic functions
	use a longer timeout, so this function is the only one that should be called until a
	phone is verified to be on a certain COM port.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\return Returns true if the phone is connected, false if it is not connected

	*******************************************************************************/
	QLIB_API unsigned char QLIB_IsPhoneConnected(HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Checks if the device is connected using the specified method (iDeviceCheckType).

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iDeviceCheckType = The check type that should be used to confirm device connection
	Format: QMSL_Device_Check_Enum (Defined in Qlib_Defines.h)
	\return Returns true if the device is connected, false if it is not connected

	*******************************************************************************/
	QLIB_API unsigned char QLIB_IsDeviceConnected(HANDLE hResourceContext, unsigned char iDeviceCheckType );

	/******************************************************************************/
	/**

	CDMA ICD, 2.1.2 Asynchronous data protocol

	Sends a request/response packet.  This allows the user to send any diagnostic
	or FTM command that is available in the protocol documents.

	The request and response packets will be formatted properly for HDLC transmission.

	This function can also be used to send packets to access new FTM functionality
	that is added before the PC library is updated to support that new function
	directly.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iRequestSize = Number of bytes to be sent in the request packet.
	\param piRequestBytes = Pointer to a buffer of request packet contents.
	\param piResponseSize = Pointer to number of bytes received in the response packet
	NOTE: when calling this function, the value must be equal to the
	maximum size of the receive buffer.  When the function returns, the
	value will be the actual number of bytes filled into the receive buffer.
	\param iResponseBytes = Pointer to a buffer to store the response packet contents.
	\param iTimeout = Number of milliseconds to wait for a timeout.

	\return Returns true if the first byte of the response packet matches
	the first byte of the request packet, false if it the phone is
	not connected or the first bytes of request and response packets
	do not match.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SendSync
		(
		HANDLE hResourceContext,
		short iRequestSize,
		unsigned char* piRequestBytes,
		short* piResponseSize,
		unsigned char* piResponseBytes,
		unsigned long iTimeout
		);

	/******************************************************************************/
	/**

	Sends a asynchronous packet.  Unlike SendSync, this command does not wait for a
	response packet from the phone.

	This command is only usable when the phone is in QPHONEMS mode, not QPST mode

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iRequestSize = Number of bytes to be sent in the request packet.
	\param piRequestBytes = Pointer to a buffer of request packet contents.
	\param iTimeout = Number of milliseconds to wait for a timeout.
	\param bStartFlag = 0 - SendASync will send data as it is, 1 - SendASync will add Start and Stop bytes

	\return Returns true if the send operation completes successfully.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SendASync
		(
		HANDLE hResourceContext,
		unsigned short iRequestSize,
		unsigned char* piRequestBytes,
		unsigned long iTimeout,
		unsigned char bStartFlag
		);

	/******************************************************************************/
	/**
	Sets mode for Send
	This command is only usable when the phone is in QPHONEMS mode, not QPST mode

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bStartFlag = 0 - Send will send data as it is, 1 - Send will add Start and Stop bytes

	\return Returns last status of StartFlag

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetStartFlag
		(
		HANDLE hResourceContext,
		unsigned char bStartFlag
		);

	/******************************************************************************/
	/**

	Sends RAW data.
	This allows the user to send an AT command and check for an "OK" response.

	The only valid response that is checked for is "OK". No other response message
	will work.

	In other words, this command has very limited use and cannot be used
	for general ASCII communication. Instead a separate communications library
	should be used for ACII.

	The message will not be formatted in any way. The user should format message correctly.
	(i.e. AT command finished with "\r" and close string with "\0")

	This command is only usable when the phone is in QPHONEMS mode, not QPST mode.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iRequestSize = Number of bytes to be sent in the request packet.
	\param piRequestBytes = Pointer to a buffer of request packet contents.
	\param piResponseSize = Pointer to number of bytes received in the response packet
	NOTE: when calling this function, the value must be equal to the
	maximum size of the receive buffer.  When the function returns, the
	value will be the actual number of bytes filled into the receive buffer.
	\param iResponseBytes = Pointer to a buffer to store the response packet contents.
	\param iTimeout = Number of milliseconds to wait for a timeout.

	\return Returns true response message received, false if it the phone is
	not connected or the time delay has run out.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SendRAW
		(
		HANDLE hResourceContext,
		short iRequestSize,
		unsigned char* piRequestBytes,
		short* piResponseSize,
		unsigned char* piResponseBytes,
		unsigned long iTimeout
		);

	/******************************************************************************/
	/**
	Sets packet mode for sending Diagnostic, FTM or RAW data.

	This function sets both phone and Qlib to send data either in Diagnostic or RAW mode.

	This command is only usable when the phone is in QPHONEMS mode, not QPST mode

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iPacketMode =	QLIB_PacketMode_BothDiag = 0 - both Qlib and phone in Diagnostic/FTM command mode
	(sending Diagnostic/FTM commands via Diagnostic COM port to phones Diagnostic port),
	used to send diagnostic commands via serial cable or diagnostic COM port on USB cable
	QLIB_PacketMode_BothAT = 1 - both Qlib and phone in Data/AT command mode
	(sending AT commands via Diagnostic COM port to phones Diagnostic port),
	used to send AT comands via serial cable
	QLIB_PacketMode_LibAT = 2 - Qlib in Data/At command mode and phone as it is
	(sending AT commands via Diagnostic COM port to phones modem port)
	used to send AT commands via modem COM port on USB cable
	QLIB_PacketMode_OBEX = 3 - QLib in OBEX protocol mode

	\return Returns true if mode switched, false if phone was not connectedor failed switching modes

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetPacketMode
		(
		HANDLE hResourceContext,
		unsigned char iPacketMode
		);

	/******************************************************************************/
	/**

	Set the state of DTR.
	This implementation is only defined with the non-QPST server version of the library

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bSetDTR_High = true to set DTR High, false to set DTR Low
	\return Returns true if the call succeeded, false if it does not

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetDTR_State( HANDLE hResourceContext, unsigned char bSetDTR_High);

	/******************************************************************************/
	/**

	Set the state of RTS.
	This implementation is only defined with the non-QPST server version of the library

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bSetRTS_High = true to set RTS High, false to set RTS Low
	\return Returns true if the call succeeded, false if it does not

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetRTS_State( HANDLE hResourceContext, unsigned char bSetRTS_High);

	/******************************************************************************/
	/**

	Clears the Rx buffer.

	This implementation is only defined with the non-QPST server version of the library

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return Returns true if the call succeeded, false if it does not

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FlushRxBuffer( HANDLE hResourceContext  );


	/******************************************************************************/
	/**

	Returns a NULL terminated string with the version information in the format:
	"QLIB VXX.yy.xx, <MODE>" where: XX is the major version #, yy is the sub-
	minor version, xx is the minor version number and <MODE> is either QPST or QPHONEMS,
	depending upon the library mode

	Examples:
	"QLIB V04.0.11,QPHONEMS"
	"QLIB V04.0.11,QPST"

	\param psVersion = Buffer to store the version number, 25 bytes should be reserved
	for this buffer.
	\return void

	*******************************************************************************/
	QLIB_API void QLIB_GetLibraryVersion( char * psVersion );

	/******************************************************************************/
	/**

	Set the library mode--QPST or QPHONEMS.  The default state of the library is to
	use QPHONEMS.  In order to use QPST, this function must be called with
	bUseQPST set to TRUE.

	\param bUseQPST = true to use QPST, false to use QPHONEMS
	\return void

	*******************************************************************************/
	QLIB_API void QLIB_SetLibraryMode( unsigned char bUseQPST );

	/******************************************************************************/
	/**

	Set the target type of any QMSL handle created after the call.   See QLIB_TargetType_Enum in QLib_Defines.h
	for possible value

	By default (or no call), QMSL set the QMSL handle target type to be MSM_MDM.  With MSM_MDM type, QLIB_IsPhoneConnected() will use
	modem diagnostic loop back command (0x7B) to check whether the diag service health

	If QMSL handle target type is APQ,  QLIB_IsPhoneConntected() will not use modem diagnostic loop back command.
	Instead, it will use {0x75, 0x32, 0x3, 0x0} to check for diagnostic response.  It is because APQ diagnostic service
	does not support modem diagnostic loopback command

	By setting target type to APQ, QMSL also uses different logic for
	1) QLIB_GetOperatingMode()
	2) QLIB_DIAG_READ_ESN_F()

	*******************************************************************************/

	QLIB_API void QLIB_SetTargetType ( unsigned char iTargetType );
	/******************************************************************************/
	/**

	This function will kill "QPSTServer.exe" process.  It has the same effect as killing
	"QPSTServer.exe" from Windows Task Manager.

	The function will not clean up handles created by QLIB_ConnectServer calls.

	Calling this function will have unknown side effects to handles already opened by
	QLIB_ConnectServer in QPST mode.  User should clean up those handles by calling
	QLIB_DisonnectServer and/or QLIB_DisconnectAllServers();

	This function can be used prior to calling QLIB_ConnectServer in QPHONEMS/UserDefinedTransport
	to ensure that the COMPORT resource is not owned by QPST Server

	\return void

	*******************************************************************************/
	QLIB_API void QLIB_TerminateQPSTServer();
	/******************************************************************************/
	/**

	Returns flags indicating the capabilities of the library

	\param pbSupportsDiag         = true if diag/ftm commands are supported
	\param pbSupportsEFS          = true if EFS commands are supported
	\param pbSupportsSwDownload   = true if software download commands are supported
	\param pbUsingQPST            = true if QPST is being used, false if QPHONEMS is used

	\return void

	*******************************************************************************/
	QLIB_API void QLIB_GetLibraryCapabilities(
		unsigned char* pbSupportsDiag,
		unsigned char* pbSupportsEFS,
		unsigned char* pbSupportsSwDownload,
		unsigned char* pbUsingQPST );


	/******************************************************************************/
	/**

	The function stores a 32-bit number that is to be associated with the QMSL resource
	context.  The purpose is so that client software callback functions can identify
	the client context from in the QMSL callback has been generated.

	\param iToken = input, a 32-bit number that is meaningful to the client software.
	This value is not evaluated by QMSL.

	\return true if the function is successful

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetClientToken(HANDLE hResourceContext, unsigned long iToken );

	/******************************************************************************/
	/**

	The function returns the token stored by QLIB_GetClientToken().

	The token is a 32-bit number that is to be associated with the QMSL resource
	context.  The purpose is so that client software callback functions can identify
	the client context from in the QMSL callback has been generated.

	\param piToken = output, a pointer to a 32-bit number that is meaningful to the client software.
	This value is not evaluated by QMSL.  If QLIB_SetClientToken() has not been
	called the value will be zero

	\return true if the function is successful.  Returns false if unsuccessful or
	if the token value==0

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GetClientToken(HANDLE hResourceContext, unsigned long *piToken );

	/******************************************************************************
	Text Logging
	*******************************************************************************/

	/******************************************************************************/
	/**

	Function to start logging send/receive data information to a text file.  After calling this
	function, the log file will remain open until the ClosePort() or DisconnectServer()
	operations are called.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sLogfile = path to log file that will be created

	\return True if file is opened successfully.


	\warning This function will reset any DLF files that are currently being logged.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StartLogging( HANDLE hResourceContext, char* sLogFile );

	/******************************************************************************/
	/**

	Function to Stop text logging

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return True if file is opened successfully.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StopLogging( HANDLE hResourceContext );


	/******************************************************************************/
	/**

	Function to check if text logging is started. Text logging may stop if ClosePort() or DisconnectServer
	is called. Use this function to check the logging status, restart if needed.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return True if logging is started

	*******************************************************************************/
	QLIB_API unsigned char QLIB_IsLoggingStarted( HANDLE hResourceContext  );

	/******************************************************************************/
	/**

	Function to start logging send/receive data information to a DLF file.  After calling this
	function, the log file will remain open until the ClosePort() or DisconnectServer()
	operations are called.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sLogfile = path to log file that will be created.  Extension .DLF should be specified
	\param bAlwaysUsePC_Time = TRUE to always use PC time in the log stamps for the DLF entries.

	If FALSE then time will be determined from the mobile's time stamp
	after an async messasge (log, event, debug message) has been received.
	Currently this behavior is disabled and DLF entries are always made
	with the the PC time.

	\return True if file is opened successfully.

	\warning

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StartDLF_Logging(
		HANDLE hResourceContext,
		char* sLogFile,
		unsigned char bAlwaysUsePC_Time );


	/******************************************************************************/
	/**

	Function to Stop DLF logging

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return True if file is opened successfully.

	\warning Function is not implement

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StopDLF_Logging( HANDLE hResourceContext );


	/******************************************************************************/
	/**

	Function to annotate DLF file, which adds a log entry into the DLF file that
	will show up in the DLF Viewer as an "Annotation". The contents of the
	annotation are a user defined string.  This can be very useful for
	putting algorithm markers into the log file, which can later be compared with the
	mobile messages.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sAnnotation = string to add into the DLF log. This will be added to the DLF as an "ANNOTATION" type of log
	and can be filtered by the DLF viewer

	\return True if file is opened successfully.

	\warning

	*******************************************************************************/
	QLIB_API unsigned char QLIB_AnnotateDLF_Log(
		HANDLE hResourceContext,
		char* sAnnotation
		);

	/******************************************************************************/
	/**

	Load the asynchronous messages from a DLF into the QMSL "Log queue", which holds
	asynchronous messages from the mobile.  After this command is called then other
	functions for managing the async queue can be called, such as QLIB_DIAG_ClearPhoneLogQueue(),
	QLIB_DIAG_GetPhoneLogQueueSize(), and QLIB_DIAG_GetNextPhoneLog().


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sDLF_File = File name of the ".DLF "log file to parse

	\return true if file is found and response data is added to the playback queue

	\warning the async queues should be cleared before this command is called, using QLIB_DIAG_ClearPhoneLogQueue()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_Playback_DLF(
		HANDLE hResourceContext,
		const char* sLogFile
		);

	/******************************************************************************/
	/**

	Function to set log level for text logging (not the same as log records from the phone).

	Log levels are treated as flags and can be added:
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param uiLogFlags = Mask for text log
	\code
	LOG_NOTHING		0x0000	= log nothing

	LOG_IO          0x0001	= data IO (data bytes)
	LOG_FN          0x0002	= function calls with parameters
	LOG_RET         0x0004	= function return data
	LOG_INF         0x0008	= general information (nice to know)
	LOG_ERR         0x0010	= critcial error information

	LOG_C_HIGH_LEVEL_START  0x0200 = High level C function start, indicates the begining of a high level C function, which
	calls other low level C functions internal to the library

	LOG_C_HIGH_LEVEL_STOP   0x4000 = High level C function stop

	LOG_IO_AHDLC    0x0020	= HDLC IO tracing (data bytes)
	LOG_FN_AHDLC    0x0040	= HDLC layer function calls
	LOG_RET_AHDLC   0x0080	= HDLC function return data
	LOG_INF_AHDLC   0x0100	= HDLC general information

	LOG_IO_DEV      0x0400	= device IO tracing (data bytes)
	LOG_FN_DEV      0x0800	= device layer function calls
	LOG_RET_DEV     0x1000	= device function return data
	LOG_INF_DEV     0x2000	= device general information

	LOG_DEFAULT     ( LOG_IO | LOG_ERR | LOG_ERR_AHDLC | LOG_ERR_DEV )

	LOG_ALL			0xFFFF	// everything
	\endcode

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param uiLogFlags = log flags as listed above

	\return True if file is opened successfully.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetLogFlags( HANDLE hResourceContext, unsigned int uiLogFlags );

	/******************************************************************************/
	/**

	Function to actvate/deactivate an extended text message category.

	This is used to enable verbose text messages, which can be used for troubleshooting.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eMessageCategory = enumeration value of the message category to be enabled.
	Uses the QMSL_Extended_Text_Message_Category_Enum, in QLib_Defines.h.


	\code
	QMSL_ExtTextMsgCat_MediaFLO_Parse         = 0,  //!< ' MediaFLO Layer one log parsing
	QMSL_ExtTextMsgCat_cdma2000_RDA_Parse     = 1   //!< ' cdma2000 RDA log parsing for SER/FER
	QMSL_ExtTextMsgCat_NV_Manager_Debug		 = 2,  //!< ' NV Manager Debug Message
	QMSL_ExtTextMsgCat_NV_Tool_Debug		     = 3,   //!< ' NV Tool Debug Message
	QMSL_ExtTextMsgCat_QPHONEMS_SW_Download   = 4    //!< ' QPHONEMS Software Download

	\endcode
	\param bEnableCategory = TRUE to enable the message category, FALSE to disable it

	\return True if file is opened successfully.


	*******************************************************************************/
	QLIB_API unsigned char QLIB_ExtendedTextMessage_SetCategoryEnable(
		HANDLE hResourceContext,
		unsigned short eMessageCategory,
		unsigned char bEnableCategory
		);

	/******************************************************************************/
	/**

	Function to get the size of the extended text message category list.  This can be used before
	calling QLIB_GetExtetendedTextMessageCategoryListItem(), in order to determine the
	list size, in case the memory must be allocated in the client.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piListSize = Output, stores the number of extended text message categories

	\return True if file is opened successfully.


	*******************************************************************************/
	QLIB_API unsigned char QLIB_ExtendedTextMessage_GetCategoryListSize(
		HANDLE hResourceContext,
		unsigned short* piListSize
		);

	/******************************************************************************/
	/**

	Function retreive one element in the extended text message category list.
	The element values are a string with the name of the item and an enable/disable status

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eMessageCategory = Input, enumeration value of the message category to be enabled.
	Uses the QMSL_Extended_Text_Message_Category_Enum, in QLib_Defines.h.
	\param sCategoryName = Output, stores the number of extended text message category indexed
	by eMessageCategory.  Must be QMSL_EXTENDED_TEXT_MESSAGE_CATEGORY_NAME_SIZE
	bytes reserved, including NULL termination.

	If sCategoryName is NULL, then no action is taken and only the
	bEnabled value will be updated.
	\param pbEnabled = Output, TRUE if the category is enabled, FALSE if it is not.

	\return True if file is opened successfully.


	*******************************************************************************/
	QLIB_API unsigned char QLIB_ExtendedTextMessage_GetCategoryListItem(
		HANDLE hResourceContext,
		unsigned short eMessageCategory,
		char* sCategoryName,
		unsigned char* pbEnabled
		);

	/******************************************************************************/
	/**

	Starts a millisecond-resolution timer and returns a handle to the timer so the
	user can query the timer value later.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return output, the unique ID that will be used to identify the timer later

	*******************************************************************************/
	QLIB_API unsigned long QLIB_StartTimer( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Gets the elapsed time of a timer

	\param iTimerHandle = input, the unique ID that was returned from StartTimer()

	\return the elapsed time

	*******************************************************************************/
	QLIB_API unsigned long QLIB_GetTimerElapsed( HANDLE hResourceContext, unsigned long iTimerHandle );

	/******************************************************************************/
	/**

	Prints  a millisecond-resolution timer to the text log.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iTimerHandle = input, the unique ID that was returned from StartTimer()
	\param sLogText = input, Text printed log file, preceding the the timer value.

	\return the elapsed time

	*******************************************************************************/
	QLIB_API unsigned long QLIB_PrintTimer( HANDLE hResourceContext, unsigned long iTimerHandle, char* sLogText );

	/******************************************************************************/
	/**

	Stops and deletes the millisecond-resolution timer associated with the user
	provided timer handle

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iTimerHandle = input, the unique ID that was returned from StartTimer()

	\return the elapsed time

	*******************************************************************************/
	QLIB_API unsigned long QLIB_StopTimer( HANDLE hResourceContext, unsigned long iTimerHandle );

	/******************************************************************************/
	/**

	Adds a user defined message to the text log

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iLogType = Log type bit mask, defined in QLib_Defines.h, for example, LOG_IO
	\param sUserMessage = message to send to the log
	\param bUpdateConsole = true to update the console, false for no update

	\return unsigned, integer value of the hex string

	*******************************************************************************/
	QLIB_API unsigned char QLIB_AddUserMessageToTextLog
		(
		HANDLE hResourceContext,
		unsigned int iLogType,
		char* sUserMessage,
		unsigned char bUpdateConsole
		);

	/******************************************************************************/
	/**

	Adds the text version of user binary async message to the text log

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iLogType = Log type bit mask, defined in QLib_Defines.h, for example, LOG_IO
	\param pMessage = pointer to the binary data containing the message
	\param bUpdateConsole = true to update the console, false for no update

	\return unsigned, integer value of the hex string

	*******************************************************************************/
	QLIB_API unsigned char QLIB_AddUserAsyncLogMessageToTextLog(
		HANDLE hResourceContext, unsigned int iLogType, unsigned char*pMessage,
		unsigned char bSendToQlibLogFile, unsigned char bUpdateConsole   );

	/******************************************************************************/
	/**


	Parse a user defined file and add all of the async and sync response messages to the
	command/async queues.  This will put the library into a "playback" mode.

	When commands are called in the future, they will not actually be sent to the phone,
	instead the responses will be retrieved from the queues that were populated from
	the log files.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sLogFile = File name of the the QLIB log file to parse
	\param bLogIsQpstMode = true to indicate the log was taken in QPST mode.
	In this mode, messages do not have CRC's.  If the file is
	not in QPST mode, then CRC's are checked and processed.

	\return true if file is found and response data is added to the playback queue

	\warning the command and async queues will be cleared before messages are added.

	\warning this function behavior is being removed in QMSL 5.2.60 and the function will
	return FALSE.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StartLogFilePlaybackMode(
		HANDLE hResourceContext, const char* sLogFile, unsigned char bLogIsQpstMode  );


	/******************************************************************************/
	/**

	Indicates that log file playback should be stopped.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return always true

	\warning the command and async queues will be cleared.

	\warning this function behavior is being removed in QMSL 5.2.60 and the function will
	return FALSE.
	*******************************************************************************/
	QLIB_API unsigned char QLIB_StopLogFilePlaybackMode( HANDLE hResourceContext   );


	/******************************************************************************
	Generate the NV items associated with GSM linear Tx DA calibration. The NV items include QMSL_GSM_RGI_Fn_I, QMSL_GSM_PMEAS_Fn_I, NV_GSM_AMAM_ARFCN_Fn_I
	Parameters:
	hResourceContext  Resource context that was returned from the call to QLIB_ConnectServer(). 
	aGSM_DA_Cal  [input] pointer to QMSL_GSM_DA_Cal_Result struct containing DA calibration sweep raw measurement data 
	aGSM_DA_Cal_NV  [output] pointer to QMSL_GSM_DA_Cal_NV_Chan_struct containing NV items generated. 
	bCachedByNVManager  [input] Store generated NV items in NV Manager. NV items stored in NV Manager will be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. bCachedByNVManager == 1: Store NV items bCachedByNVManager == 0: Do not store NV items. Generated NV items will not be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. 

	Returns:
	true if operation is successful, false otherwise
	Warning:
	NV items are not written to the mobile station until QLIB_RFCAL_NV_Manager_WriteToPhone() is called 
	******************************************************************************/
	QLIB_API unsigned char QLIB_RFCAL_GSM_Tx_DA_Cal_Results(HANDLE hResourceContext, unsigned short iNvModeId, void * aGSM_DA_Cal, void * aGSM_DA_Cal_NV, unsigned char bCachedByNVManager);


	/******************************************************************************
	Generate the NV items associated with GSM linear Tx pre-distortion calibration. The NV items include NV_GSM_<band>_AMAM_LINEAR_PA_RANGEA_F<n>_SEG<n>_I, NV_GSM_<band>_AMPM_LINEAR_PA_RANGEA_F<n>_SEG<n>_I, NV_GSM_AMAM_DYNAMIC_RANGE_I, NV_GSM_<band>_TX_AGC_SETTING_FOR_PRED_I

	NV_GSM_AMAM_ARFCN_Fn_I is populated as part of DA calibration (QLIB_RFCAL_GSM_Tx_DA_Cal_Results())

	Parameters:
	hResourceContext  Resource context that was returned from the call to QLIB_ConnectServer(). 
	aGSM_PreDist_Cal  [input] Pointer to QMSL_GSM_PreDist_Cal_Result struct containing raw pre-distortion calibration data. Phase (in degrees) and Amplitude (in dBm). 
	aGSM_PreDist_Cal_NV  [output] Pointer to QMSL_GSM_PreDist_Cal_NV_Chan_struct containing NV items generated. 
	bCachedByNVManager  [input] Store generated NV items in NV Manager. NV items stored in NV Manager will be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. bCachedByNVManager == 1: Store NV items bCachedByNVManager == 0: Do not store NV items. Generated NV items will not be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. 

	Returns:
	true if operation is successful, false otherwise
	Warning:
	NV items are not written to the mobile station until QLIB_RFCAL_NV_Manager_WriteToPhone() is called 
	******************************************************************************/
	QLIB_API unsigned char QLIB_RFCAL_GSM_Tx_Linear_Process_PreDist_Cal_Results(HANDLE hResourceContext,  
		unsigned short iNvModeId, void * aGSM_PreDist_Cal, void * aGSM_PreDist_Cal_NV, unsigned char bCachedByNVManager);


	/******************************************************************************
	Generate the NV items associated with GSM LNA gain range calibration. The NV items includes NV_<band>_RX_GAIN_RANGE_<n>_FREQ_COMP

	Parameters:
	hResourceContext  Resource context that was returned from the call to QLIB_ConnectServer(). 
	iNvModeId  [input] Enumeration of nv_mode_id_type. The NV mode ID determines the set of NV items to be generated. See QLib_Defines.h for definition 
	aGSM_Rx_Gain_Range_Result  [input] Pointer to QMSL_GSM_Rx_Gain_Range_struct, GSM LNA range gain calibration values. See QLib_Defines.h for details 
	aGSM_Rx_Gain_Range_NV  [output] Pointer to QMSL_GSM_Rx_Gain_Range_NV_struct. See QLib_Defines.h for details 
	bCachedByNVManager  [input] Store generated NV items in NV Manager. NV items stored in NV Manager will be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. bCachedByNVManager == 1: Store NV items bCachedByNVManager == 0: Do not store NV items. Generated NV items will not be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. 

	Returns:
	true if operation is successful, false otherwise
	Warning:
	NV items are not written to the mobile station until QLIB_RFCAL_NV_Manager_WriteToPhone() is called 
	******************************************************************************/
	QLIB_API unsigned char QLIB_RFCAL_GSM_Rx_Gain_Range_Results(HANDLE hResourceContext,
		unsigned short iNvModeId, void * aGSM_Rx_Gain_Range_Result, void * aGSM_Rx_Gain_Range_NV, unsigned char bCachedByNVManager);


	/******************************************************************************
	Generate GSM calibrationa channel list item (NV_GSM_CAL_ARFCN NV) items.

	Parameters:
	hResourceContext  Resource context that was returned from the call to QLIB_ConnectServer(). 
	iNvModeId  [input] Enumeration of nv_mode_id_type. The NV mode ID determines the set of NV items to be generated. See QLib_Defines.h for definition 
	aGSM_Cal_ARFCN_List  [input] Pointer to an array of GSM calbrtation channel 
	iNumOfChannel  [input] Number of channel in aGSM_Cal_ARFCN_List 
	pGSM_Cal_ARFCN_NV  [output] Pointer to QMSL_RFCAL_NV_Item. See QLib_Defines.h for details 
	bCachedByNVManager  [input] Store generated NV items in NV Manager. NV items stored in NV Manager will be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. bCachedByNVManager == 1: Store NV items bCachedByNVManager == 0: Do not store NV items. Generated NV items will not be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. 

	Returns:
	true if operation is successful, false otherwise
	Warning:
	NV items are not written to the mobile station until QLIB_RFCAL_NV_Manager_WriteToPhone() is called 
	******************************************************************************/
	QLIB_API unsigned char QLIB_RFCAL_GSM_CAL_ARFCN(HANDLE hResourceContext,
		unsigned short iNvModeId, unsigned short * aGSM_Cal_ARFCN_List,
		unsigned char iNumOfChannel, void * pGSM_Cal_ARFCN_NV, unsigned char bCachedByNVManager);


	/******************************************************************************
	Write NV items stored in NV Manager to mobile station. The NV items are generated by previous QLIB_RFCAL_xxx_Results calls By default, the API will use AUTO_DETECT method to use NV_WRITE or FTM_RFNV_WRITE to write data

	Call QLIB_RFCAL_NV_Manager_WriteToPhoneMethod to override the AUTO_DETECT mode

	Parameters:
	hResourceContext  Resource context that was returned from the call to QLIB_ConnectServer(). 
	pWriteToPhoneStatus  Pointer to QMSL_RFCal_WriteToPhone_Status. This structure return the ID and status of NV items written to mobile. This pointer can be NULL. 

	Returns:
	true if operation is successful, false otherwise
	Warning:
	******************************************************************************/
	QLIB_API unsigned char QLIB_RFCAL_NV_Manager_WriteToPhone(HANDLE  hResourceContext, void * pWriteToPhoneStatus);


	/******************************************************************************
	Generate Tx Linearizer (Beta Scaling), Tx AGC vs HDET, HDET offset and span NV items

	Parameters:
	hResourceContext  Resource context that was returned from the call to QLIB_ConnectServer(). 
	iNvModeID  [input] Enumeration of nv_mode_id_type. The NV mode ID determines the set of NV items to be generated. See QLib_Defines.h for definition 
	aTx_Measured  [input] Pointer to QMSL_CDMA_TxLin_BetaScaling_struct. The data structure stores Tx linearizer calibration results. See QLib_Defines.h for definition 
	aTx_Result  [output] Pointer to QMSL_CDMA_TxLin_BetaScaling_Result_struct. The data structure stores the generated Tx Linearizer related NV items. See QLib_Defines.h for definition 
	bCachedByNVManager  [input] Store generated NV items in NV Manager. NV items stored in NV Manager will be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. bCachedByNVManager == 1: Store NV items bCachedByNVManager == 0: Do not store NV items. Generated NV items will not be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. 

	Returns:
	true if successful, false otherwise
	Warning:
	NV items are not written to the mobile station until QLIB_RFCAL_NV_Manager_WriteToPhone() is called 
	******************************************************************************/
	QLIB_API unsigned char QLIB_RFCAL_CDMA_TxLinCal_BetaScaling_Results(HANDLE hResourceContext, unsigned short iNvModeId,  
		void * aTx_Measured, void * aTx_Result, unsigned char bCachedByNVManager);


	/******************************************************************************
	Generate DVGA and LNA related NV item values

	Parameters:
	hResourceContext  Resource context that was returned from the call to QLIB_ConnectServer(). 
	iNv_Mode_ID  [input] Enumeration of nv_mode_id_type. The NV mode ID determines the set of NV items to be generated. See QLib_Defines.h for definition 
	iIntelliceiver  [input] Is it Intelliceiver? 1 == Yes, 0 == No 
	dDVGAOffsetScalingFactor  [input] The scaling factor to calculate the DVGAvsFreq offset. DVGAvsFreq[_ch] = (DVGA[_ch] - refDVGA) / dDVGAOffsetScalingFactor 

	For CDMA targets(MSM6K/7K,QSC6010/20/30), the scaling factor is 12
	For WCDMA targets, the scaling factor is 1
	For QSC6055/65/75/85 targets, the scaling factor is 1
	For QTR86xx targets, the scaling factor is 1
	Consult the RF NV document for the proper scaling factor.

	Parameters:
	aRX_Measured  [input] Pointer to QMSL_CDMA_RxFreq_Measurement_struct. The data structure that stores receiver calibration results. See QLib_Defines.h for definition 
	aRX_MeasuredChannels  [input] Pointer to QMSL_CDMA_MeasuredChannels_struct. The list of RF channels which the receiver calibration runs. See QLib_Defines.h for definition 
	aRX_NV_Channels  [input] Pointer to QMSL_CDMA_NVChannels_struct. The list of RF channels which NV generation is used. This list must always contain 16 values. If fewer channels are desired to be used then the last valid channel (highest frequency) is to be filled into the end of the list, such that there are 16 items in the list. 
	aRX_DVGA_LNA_Cal_NV  [output] Pointer to QMSL_CDMA_Rx_DVGA_LNA_Cal_NV_Result. The data structure stores the NV item values generated. 
	bWriteNVChList  [input] Generate calibration channel list NV_Cx_BCy_RX_CAL_CHAN_I NV item based on channesl in aRX_NV_Channels. Note that not all targets supports this NV items. Please refer to the target RF NV items document 
	bCachedByNVManager  [input] Store generated NV items in NV Manager. NV items stored in NV Manager will be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. 
	bCachedByNVManager == 1: Store NV items 
	bCachedByNVManager == 0: Do not store NV items. Generated NV items will not be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called.  

	Returns:
	Warning:
	******************************************************************************/
	QLIB_API unsigned char QLIB_RFCAL_CDMA_RxCal_Results(HANDLE hResourceContext,  
		unsigned short  iNV_Mode_ID,  
		unsigned char  iIsIntelliceiver,  
		double  dDVGAOffsetScalingFactor,  
		void *  aRX_Measured,  
		void *  aRX_MeasuredChannels,  
		void *  aRX_NV_Channels,  
		void *  aRX_DVGA_LNA_Cal_NV,  
		unsigned char  bWriteNVChList,  
		unsigned char  bCachedByNVManager);

	
	/******************************************************************************
	Generate NV items related to internal thermistor calibration. The NV Items include (NV_THERM, NV_THERM_TEMP_MAX, NV_THERM_TEMP_MIN)

	Parameters:
	hResourceContext  Resource context that was returned from the call to QLIB_ConnectServer(). 
	pInt_Therm_Cal_Meas  [input] Pointer to QMSL_Internal_Thermistor_Cal_Meas_struct. See QLib_Defines.h for details 
	pInt_Therm_Cal_NV  [input] Pointer to QMSL_Internal_Thermistor_Cal_NV_struct. See QLib_Defines.h for details 
	bCachedByNVManager  [input] Store generated NV items in NV Manager. NV items stored in NV Manager will be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. bCachedByNVManager == 1: Store NV items bCachedByNVManager == 0: Do not store NV items. Generated NV items will not be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. 

	Returns:
	true if operation is successful, false otherwise
	Warning:
	NV items are not written to the mobile station until QLIB_RFCAL_NV_Manager_WriteToPhone() is called 
	******************************************************************************/
	QLIB_API unsigned char QLIB_RFCAL_Internal_Thermistor_Calibration_Results(HANDLE hResourceContext,  
		void * pInt_Therm_Cal_Meas, void * pInt_Therm_Cal_NV, unsigned char bCachedByNVManager);


	/******************************************************************************
	Generate Tx Frequency Compensation (TX_COMP_n) NV items

	Parameters:
	hResourceContext  Resource context that was returned from the call to QLIB_ConnectServer(). 
	iNvModeID  [input] Enumeration of nv_mode_id_type. The NV mode ID determines the set of NV items to be generated. See QLib_Defines.h for definition 
	aTx_Measured_Power  [input] Pointer to QMSL_CDMA_TxLinVsFreq_struct. The data structure stores Tx linearizer vs frequency results. See QLib_Defines.h for definition 
	aTx_MeasuredChannels  [input] Pointer to QMSL_CDMA_MeasuredChannels_struct. The data structure stores the list of RF channels, which the measurement is made. See QLib_Defines.h for definition This list must always contain 16 values. If fewer channels are desired to be used then the last valid channel (highest frequency) is to be filled into the end of the list, such that there are 16 items in the list. 
	aTx_NV_Channels  [input] Pointer to QMSL_CDMA_NVChannels_struct. The list of RF channels which NV generation is used. See QLib_Defines.h for definition. 
	aTx_LinVsFreq_NV_Result  [output] Pointer to QMSL_CDMA_Tx_LinVsFreq_Cal_NV_Result_struct. The data structure that stores the NV item values generated. See QLib_Defines.h for definition 

	Note, User can use bC2K_Not_Overwrite_Tx_Pwr_Comp_Static_Component_With_Zero to NOT overwrite the static component by reading NV See bC2K_Not_Overwrite_Tx_Pwr_Comp_Static_Component_With_Zero in Qlib_Defines.h for details

	Parameters:
	iNV_Resolution  [input] The number of NV value counts per dB NV[chan] = ( measured-pwr[chan] - refChannelPwr ) * iNV_Resolution For targets with 102.4 dB dynamic range, use 10 (ie 1/10dB per count). For targets with 85.3 dB dynamic range, use 12 (ie 1/12dB per count). 
	iNV_Tx_Comp_Type  [input] 0 = QMSL_NV_TX_COMP, (NV_<bandclass>_TX_COMP_x NV items are generated) 1 = QMSL_NV_TX_PWR_COMP, (NV_<bandclass>_TX_PWR_COMP_x NV items are generated) 
	bWriteNVChList  [input] Generate calibration channel list NV_Cx_BCy_TX_CAL_CHAN_I NV item based on channesl in aRX_NV_Channels. Note that not all target supports this NV items. Please refer to the target RF NV items document 
	bCachedByNVManager  [input] Store generated NV items in NV Manager. NV items stored in NV Manager will be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. bCachedByNVManager == 1: Store NV items bCachedByNVManager == 0: Do not store NV items. Generated NV items will not be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. 

	Returns:
	true if successful, false otherwise
	Warning:
	NV items are not written to the mobile station until QLIB_RFCAL_NV_Manager_WriteToPhone() is called 
	******************************************************************************/
	QLIB_API unsigned char QLIB_RFCAL_CDMA_Tx_LinVsFreq_Results(HANDLE hResourceContext,  
		unsigned short iNvModId,  
		void * aTx_Measured_Power,  
		void * aTx_MeasuredChannels,  
		void * aTx_NV_Channels,  
		int iNV_Resolution_Type,  
		int iNV_Tx_Comp_Type,  
		void * aTx_LinVsFreq_NV_Result,  
		unsigned char bWriteNVChList,  
		unsigned char bCachedByNVManager);


	/******************************************************************************
	Generate Tx Lim vs freq (TX_LIM_VS_FREQ) NV items. Calcuations are based on the WCDMA NV Document 80-V6968-12 Rev H, section 4.3.3.11.

	The same calculations are used for CDMA2000 and WCDMA, this document is just a reference point.

	As for Feb 2008, certain CDMA2000 documents had a different formula. The formula will be changed to match 80-V6968-12 after the following versions: 

	80-V8891-11 Rev C for MSM6800
	80-VC467-11 Rev C for RTR6500

	Parameters:
	hResourceContext  Resource context that was returned from the call to QLIB_ConnectServer(). 
	iNvModeId  [input] Enumeration of nv_mode_id_type. The NV mode ID determines the set of NV items to be generated. See QLib_Defines.h for definition 
	dDesiredMaxPower  [input] Desired maximum output power in dBm 
	aTx_LimVsFreqMeasured  [input] Pointer to QMSL_CDMA_TxLimVsFreq_struct. The data structure stores Tx Lim vs Freq measurement. See QLib_Defines.h for definition 
	aTx_MeasuredChannels  [input] Pointer to QMSL_CDMA_MeasuredChannels_struct. The data structure stores the list of RF channels, which the measurement is made. See QLib_Defines.h for definition 
	aTx_NV_Channels  [input] Pointer to QMSL_CDMA_NVChannels_struct. The list of RF channels which NV generation is used. See QLib_Defines.h for definition. This list must always contain 16 values. If fewer channels are desired to be used then the last valid channel (highest frequency) is to be filled into the end of the list, such that there are 16 items in the list. 
	aTx_LimVsFreq_NV_Result  [output] Pointer to QMSL_CDMA_Tx_LimVsFreq_NV_struct. The data structure that stores the NV item values generated. See QLib_Defines.h for definition. This list must always contain 16 values. If fewer channels are desired to be used then the last valid channel (highest frequency) is to be filled into the end of the list, such that there are 16 items in the list. 
	bCachedByNVManager  [input] Store generated NV items in NV Manager. NV items stored in NV Manager will be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. bCachedByNVManager == 1: Store NV items bCachedByNVManager == 0: Do not store NV items. Generated NV items will not be written to mobile when QLIB_RFCAL_NV_Manager_WriteToPhone() is called. 

	Returns:
	true if successful, false otherwise
	Warning:
	NV items are not written to the mobile station until QLIB_RFCAL_NV_Manager_WriteToPhone() is called 
	******************************************************************************/
	QLIB_API unsigned char QLIB_RFCAL_CDMA_Tx_LimVsFreq_Results(HANDLE hResourceContext,  
		unsigned short iNvModeId,  
		double dDesiredMaxPower,  
		void * aTx_LimVsFreqMeasured,  
		void * aTx_MeasuredChannels,  
		void * aTx_NV_Channels,  
		void * aTx_LimVsFreq_NV_Result,  
		unsigned char bCachedByNVManager);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif	// defined(_QLIB_H)
