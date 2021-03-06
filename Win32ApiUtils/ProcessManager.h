// ProcessManager.h : Declaration of the CProcessManager

#pragma once
#include "resource.h"       // main symbols

#include <Winnt.h>
#include <Winbase.h>
#include <Wtsapi32.h>
#include <Processthreadsapi.h>
#include <Userenv.h>
#include <TlHelp32.h>
#include  <Windows.h>
#include <comdef.h>
#include <Wtsapi32.h>
#include <Userenv.h>
#include <Windows.h>
#include <winternl.h>
#include <Strsafe.h>
#include <atlbase.h>
#include "CLogger.h"
#include "Win32ApiUtils_i.h"


#pragma comment(lib,"Wtsapi32.lib")

#pragma comment(lib,"Userenv.lib")
#pragma comment(lib,"ntdll.lib") // Need to link with ntdll.lib import library. You can find the ntdll.lib from the Windows DDK.
#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

typedef struct _SYSTEM_PROCESS_INFO
{
	ULONG                   NextEntryOffset;
	ULONG                   NumberOfThreads;
	LARGE_INTEGER           Reserved[3];
	LARGE_INTEGER           CreateTime;
	LARGE_INTEGER           UserTime;
	LARGE_INTEGER           KernelTime;
	UNICODE_STRING          ImageName;
	ULONG                   BasePriority;
	HANDLE                  ProcessId;
	HANDLE                  InheritedFromProcessId;
}SYSTEM_PROCESS_INFO, *PSYSTEM_PROCESS_INFO;
using namespace ATL;




#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CProcessManager

class ATL_NO_VTABLE CProcessManager :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProcessManager, &CLSID_ProcessManager>,
	public IDispatchImpl<IProcessManager, &IID_IProcessManager, &LIBID_Win32ApiUtilsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	CLogger* logger;
	size_t len = 260;
	wchar_t* buffer = new wchar_t[len];


private:
	HANDLE GetTokenForProcessName(wchar_t * lpzzProcessName);
	HANDLE GetTokenForProcessId(UINT ProcessId);
	BOOL CopyPrivileges(HANDLE hTokenSrc, HANDLE hTokenDst);
	HANDLE GetImpersonationToken(HANDLE hToken);
	BOOL CreateProcessWithToken(HANDLE hToken, wchar_t * lpszCommandLine, DWORD * dwProcessId);
public:
	CProcessManager()
	{
		wchar_t * lpFileName = (wchar_t*)LocalAlloc(LMEM_ZEROINIT, sizeof(wchar_t) * 256);
		GetModuleFileName(GetModuleHandle(NULL), lpFileName, 256);
		PathRemoveFileSpec(lpFileName);
		//wchar_t *lpBuffer = (wchar_t*)LocalAlloc(LMEM_ZEROINIT, sizeof(wchar_t) * 256);
		//GetCurrentDirectoryW(256, lpBuffer);
		wstring path = wstring(lpFileName);
		path.append(L"\\Logs\\Win32ApiUtilsLog.log");
		logger = new CLogger(path);
	}
	~CProcessManager() {
		delete logger;
	}

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(CProcessManager)
	COM_INTERFACE_ENTRY(IProcessManager)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(LaunchProcessAsActiveUser)(BSTR processName, LONG* dwProcessId);


};

OBJECT_ENTRY_AUTO(__uuidof(ProcessManager), CProcessManager)
