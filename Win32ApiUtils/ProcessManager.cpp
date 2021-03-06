// ProcessManager.cpp : Implementation of CProcessManager

#include "stdafx.h"
#include "ProcessManager.h"

wchar_t *CharToWideChar(const char* input)
{
	int lenA = lstrlenA(input);
	int lenW = ::MultiByteToWideChar(CP_ACP, 0, input, lenA, NULL, 0);
	if (lenW > 0)
	{		/* Create empty wchar_t buffer to hold Unicode form of above string, and initialize (zero) it */
		wchar_t *output = (wchar_t*)LocalAlloc(LMEM_ZEROINIT, sizeof(wchar_t) * strlen(input));
		MultiByteToWideChar(CP_ACP, 0, input, lenA, output, lenW);
		return output;
	}

}

char * WideCharToChar(const wchar_t * input)
{
	int length = ::WideCharToMultiByte(CP_ACP, 0, input, -1, 0, 0, NULL, NULL);
	char* output = (char*)LocalAlloc(LMEM_ZEROINIT, sizeof(char) * length);
	WideCharToMultiByte(CP_ACP, 0, input, -1, output, length, NULL, NULL);
	return output;
}




HANDLE CProcessManager::GetTokenForProcessName(wchar_t *lpszProcessName) {
	UINT dwSessionId = WTSGetActiveConsoleSessionId();
	//dwSessionId = 1;
	UINT Pid = 0;
	logger->Log(L"WTSGetActiveConsoleSessionId", dwSessionId);
	logger->Log(L"GetTokenForProcessName", lpszProcessName);
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	ZeroMemory(&pe32, sizeof(pe32));
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_ltow_s((DWORD)GetLastError(), buffer, len, 10);
		//MessageBoxA(0,"CreateToolhelp32Snapshot  returned Invalid Handle",buffer,MB_OK);
		logger->Log(L"CreateToolhelp32Snapshot  returned Invalid Handle", buffer);
		return(0);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		_ltow_s((DWORD)GetLastError(), buffer, len, 10);
		//MessageBoxA(0,"Process32First Error",buffer,MB_OK); // show cause of failure
		logger->Log(L"Process32First Error", buffer);
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(0);
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		//size_t converted = 0;

		//wcstombs_s(&converted, buffer, len, pe32.szExeFile, len);
		logger->Log(L"Process Name", buffer);
		wstring entry = wstring(buffer);
		entry += L":" + wstring(lpszProcessName);

		int result = wcscmp(lpszProcessName, buffer);
		_itow_s(result, buffer, len, 10);
		//logger->Log(entry.c_str(),buffer);
		if (result == 0) {

			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(pe32.th32ProcessID, &winlogonSessId) &&
				winlogonSessId == dwSessionId) {
				Pid = pe32.th32ProcessID;
				_ltow_s(Pid, buffer, len, 10);
				//MessageBoxA(0, buffer, "ProcessId", MB_OK);
				logger->Log(L"ProcessId", buffer);
				return GetTokenForProcessId(Pid);
			}

		}

	} while (Process32Next(hProcessSnap, &pe32));
	logger->Log(L"Failed to Find Process", lpszProcessName);
	CloseHandle(hProcessSnap);
	return 0;


}

HANDLE CProcessManager::GetTokenForProcessId(UINT dwProcessId)
{

	_ltow_s(dwProcessId, buffer, len, 10);
	logger->Log(L"GetTokenForProcessId::", buffer);
	HANDLE hProcess = 0;
	HANDLE hToken = 0;
	hProcess = OpenProcess(MAXIMUM_ALLOWED, 0, dwProcessId);
	if (hProcess == NULL) {
		_ltow_s((DWORD)GetLastError(), buffer, len, 10);
		//MessageBoxA(0, "OpenProcess Returned Null", buffer, MB_OK);
		logger->Log(L"OpenProcess Returned Null", buffer);
	}
	_ltow_s((long)hProcess, buffer, len, 10);
	logger->Log(L"OpenProcess reurned Handle:", buffer);
	// obtain a handle to the access token of the winlogon process
	if (!OpenProcessToken(hProcess, TOKEN_ALL_ACCESS | TOKEN_READ | TOKEN_WRITE | TOKEN_EXECUTE, &hToken))
	{
		_ltow_s((DWORD)GetLastError(), buffer, len, 10);
		//MessageBoxA(0, "OpenProcesToken Returned Null", buffer, MB_OK);
		logger->Log(L"OpenProcesToken Returned Null", buffer);
		CloseHandle(hProcess);

		return 0;
	}
	_ltow_s((long)hToken, buffer, len, 10);
	logger->Log(L"OpenProcessToken reurned Handle:", buffer);



	DWORD TokenInfLength = 0;

	//-http://www.microsoft-questions.com/microsoft/Platform-SDK-Security/35984508/how-to-run-a-process-with-elevated-privileges-run-as-administrat.aspx
	// first call gets lenght of TokenInformation
	GetTokenInformation(hToken, TokenUser, 0, TokenInfLength, &TokenInfLength);
	_ltow_s((long)TokenInfLength, buffer, len, 10);
	logger->Log(L"Token Inf Length:", buffer);

	HANDLE * TokenInformation = (HANDLE*)LocalAlloc(LMEM_ZEROINIT, TokenInfLength);

	if (!GetTokenInformation(hToken, TokenElevation, TokenInformation, TokenInfLength, &TokenInfLength)) {


		if (!GetTokenInformation(hToken, TokenLinkedToken, TokenInformation, TokenInfLength, &TokenInfLength))
		{
			_ltow_s((long)GetLastError(), buffer, len, 10);
			logger->Log(L"Failed to Get Primary Token:", buffer);

		}

	}
	HANDLE hPrimaryToken = *TokenInformation;
	_ltow_s((long)hPrimaryToken, buffer, len, 10);
	logger->Log(L"Primary Token:", buffer);


	return hPrimaryToken;
}

HANDLE CProcessManager::GetImpersonationToken(HANDLE hToken)
{

	_ltow_s((long)hToken, buffer, len, 10);
	logger->Log(L"GetImpersonationToken reurned Handle:", buffer);

	// Security attibute structure used in DuplicateTokenEx and CreateProcessAsUser
	// I would prefer to not have to use a security attribute variable and to just 
	// simply pass null and inherit (by default) the security attributes
	// of the existing token. However, in C# structures are value types and therefore
	// cannot be assigned the null value.
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(sa));

	sa.nLength = sizeof(sa);
	HANDLE hTokenDup = 0;
	// copy the access token of the winlogon process; the newly created token will be a primary token
	if (!DuplicateTokenEx(hToken, TOKEN_ALL_ACCESS | TOKEN_READ | TOKEN_WRITE | TOKEN_EXECUTE, &sa, SecurityIdentification, TokenPrimary, &hTokenDup))
	{
		_ltow_s((DWORD)GetLastError(), buffer, len, 10);
		//MessageBoxA(0, "DuplicateToken Failed",buffer, MB_OK);
		logger->Log(L"DuplicateToken Failed", buffer);

		CloseHandle(hToken);
		return 0;
	}
	_ltow_s((long)hTokenDup, buffer, len, 10);
	logger->Log(L"DuplicateToken reurned Handle:", buffer);

	return hTokenDup;
}




BOOL CProcessManager::CopyPrivileges(HANDLE hTokenSrc, HANDLE hTokenDst)
{

	_ltow_s((long)hTokenSrc, buffer, len, 10);
	wstring entry = wstring(buffer);
	_ltow_s((long)hTokenDst, buffer, len, 10);
	entry += wstring(L":") + wstring(buffer);

	logger->Log(L"CopyPrivileges: Original Tokens", entry);

	hTokenSrc = GetImpersonationToken(hTokenSrc);
	hTokenDst = GetImpersonationToken(hTokenDst);


	_ltow_s((long)hTokenSrc, buffer, len, 10);
	entry = wstring(buffer);
	_ltow_s((long)hTokenDst, buffer, len, 10);
	entry += wstring(L":") + wstring(buffer);

	logger->Log(wstring(L"CopyPrivileges: Impersonated Tokens"), entry);


	TOKEN_PRIVILEGES Priv;
	ZeroMemory(&Priv, sizeof(Priv));
	DWORD dwLen = 0;
	GetTokenInformation(hTokenSrc, TokenPrivileges, &Priv, sizeof(TOKEN_PRIVILEGES), &dwLen);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
		/**///)
		_ltow_s(GetLastError(), buffer, len, 10);
		logger->Log(L"GetTokenPrivileges First Call Failed with Error:", buffer);

		return(0);
	}

	_ltow_s(dwLen, buffer, len, 10);
	logger->Log(L"GetTokenPrivileges Buffer size:", buffer);

	PTOKEN_PRIVILEGES pPriv = (PTOKEN_PRIVILEGES)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwLen);
	if (!GetTokenInformation(hTokenDst, TokenPrivileges, (LPVOID)pPriv, dwLen, &dwLen)) {
		_ltow_s(GetLastError(), buffer, len, 10);
		logger->Log(L"GetTokenPrivileges Failed with Error:", buffer);
		return(0);
	}
	_ltow_s(pPriv->PrivilegeCount, buffer, len, 10);
	logger->Log(L"Privilege Count", buffer);

	if (!AdjustTokenPrivileges(hTokenDst, FALSE, pPriv, dwLen, NULL, NULL)) {
		_ltow_s(GetLastError(), buffer, len, 10);
		logger->Log(L"AdjustTokenPrivileges Failed with Error:", buffer);
		return false;
	}

	LUID luid;
	ZeroMemory(&luid, sizeof(luid));
	if (!LookupPrivilegeValue(0, SE_TCB_NAME, &luid))
	{
		_ltow_s(GetLastError(), buffer, len, 10);
		logger->Log(L"LookupPrivilegeValue Failed with Error:", buffer);
		return false;
	}
	TOKEN_PRIVILEGES tp;
	ZeroMemory(&tp, sizeof(tp));
	tp.PrivilegeCount = 1;
	tp.Privileges->Luid.HighPart = luid.HighPart;
	tp.Privileges->Luid.LowPart = luid.LowPart;

	DWORD dwReturn = 0;
	if (!AdjustTokenPrivileges(hTokenDst, SE_PRIVILEGE_ENABLED, &tp, sizeof(tp), 0, &dwReturn))
	{
		_ltow_s(GetLastError(), buffer, len, 10);
		logger->Log(L"AdjustTokenPrivileges for TCB_NAME Failed with Error:", buffer);
		return false;

	}

	logger->Log(L"Adjust Privileges Succeeded");
	DWORD dwSessionId = WTSGetActiveConsoleSessionId();
	if (!SetTokenInformation(hTokenDst, TokenSessionId, &dwSessionId, sizeof(dwSessionId)))
	{
		_ltow_s(GetLastError(), buffer, len, 10);
		logger->Log(L"SetTokenInformation Failed with Error:", buffer);
		return false;;
	}
	logger->Log(L"SetTokenInformation succeded");
	return true;
}


//https://stackoverflow.com/questions/14315013/how-to-get-the-active-user-when-multiple-users-are-logged-on-in-windows


STDMETHODIMP CProcessManager::LaunchProcessAsActiveUser(BSTR processName, LONG* dwProcessId)
{

	//char *lpszPath = _com_util::ConvertBSTRToString(processName);
	wchar_t* path = (wchar_t*)processName;//CharToWideChar(lpszPath);

	DWORD session_id = -1;
	DWORD session_count = 0;

	WTS_SESSION_INFOA *pSession = NULL;


	if (WTSEnumerateSessionsA(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSession, &session_count))
	{
		//log success
	}
	else
	{
		//log error
		return S_OK;
	}
	logger->Log(L"Session Count", session_count);
	logger->Log(L"Begin Enumerating Sesions");
	for (int i = 0; i < session_count; i++)
	{
		session_id = pSession[i].SessionId;
		logger->Log(L"SessionId", session_id);

		WTS_CONNECTSTATE_CLASS wts_connect_state = WTSDisconnected;
		WTS_CONNECTSTATE_CLASS* ptr_wts_connect_state = NULL;

		DWORD bytes_returned = 0;
		if (::WTSQuerySessionInformation(
			WTS_CURRENT_SERVER_HANDLE,
			session_id,
			WTSConnectState,
			reinterpret_cast<LPTSTR*>(&ptr_wts_connect_state),
			&bytes_returned))
		{
			wts_connect_state = *ptr_wts_connect_state;
			::WTSFreeMemory(ptr_wts_connect_state);
			if (wts_connect_state != WTSActive) continue;
		}
		else
		{
			//log error
			continue;
		}
		logger->Log(L"End Enumerating Sesions");
		logger->Log(L"Selected Session Id", session_id);
		HANDLE hImpersonationToken;

		if (!WTSQueryUserToken(session_id, &hImpersonationToken))
		{
			//log error
			logger->Log(L"Exception in WTSQueryUserToken", GetLastError());
			continue;
		}


		//Get real token from impersonation token
		DWORD neededSize1 = 0;
		HANDLE *realToken = new HANDLE;
		if (GetTokenInformation(hImpersonationToken, (::TOKEN_INFORMATION_CLASS) TokenLinkedToken, realToken, sizeof(HANDLE), &neededSize1))
		{
			CloseHandle(hImpersonationToken);
			hImpersonationToken = *realToken;
		}
		else
		{
			//log error
			logger->Log(L"Exception in GetTokenInformation", GetLastError());
			continue;
		}


		HANDLE hUserToken;

		if (!DuplicateTokenEx(hImpersonationToken,
			//0,
			//MAXIMUM_ALLOWED,
			TOKEN_ASSIGN_PRIMARY | TOKEN_ALL_ACCESS | MAXIMUM_ALLOWED,
			NULL,
			SecurityImpersonation,
			TokenPrimary,
			&hUserToken))
		{
			//log error
			logger->Log(L"Exception in DuplicateTokenEx", GetLastError());
			continue;
		}

		// Get user name of this process
		//LPTSTR pUserName = NULL;
		WCHAR* pUserName;
		DWORD user_name_len = 0;

		if (WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, session_id, WTSUserName, &pUserName, &user_name_len))
		{
			//log username contained in pUserName WCHAR string
			// char * lpszUserName = WideCharToChar(pUserName);
			logger->Log(pUserName);
			//LocalFree(lpszUserName);
		}
		else
		{
			logger->Log(L"Exception in WTSQuerySessionInformation", GetLastError());
		}

		//Free memory                         
		if (pUserName) WTSFreeMemory(pUserName);

		ImpersonateLoggedOnUser(hUserToken);

		STARTUPINFOW StartupInfo;

		StartupInfo.cb = sizeof(STARTUPINFOW);




		//GetStartupInfoW(&StartupInfo);
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		//Uncommented  by Sagar 20th January 20118 1612
		StartupInfo.lpDesktop = CharToWideChar("winsta0\\default");

		//to Hide Console Process 03-10-2018
		StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartupInfo.wShowWindow = SW_SHOW;//SW_HIDE;



		PROCESS_INFORMATION processInfo;



		SECURITY_ATTRIBUTES Security1;
		ZeroMemory(&Security1, sizeof(Security1));

		Security1.nLength = sizeof SECURITY_ATTRIBUTES;


		SECURITY_ATTRIBUTES Security2;
		ZeroMemory(&Security2, sizeof(Security2));
		Security2.nLength = sizeof SECURITY_ATTRIBUTES;

		void* lpEnvironment = NULL;

		// Get all necessary environment variables of logged in user
		// to pass them to the new process
		BOOL resultEnv = CreateEnvironmentBlock(&lpEnvironment, hUserToken, FALSE);

		if (!resultEnv)
		{
			//log error
			DWORD err = GetLastError();
			logger->Log(L"Exception in CreateEnvironmentBlock", err);

			continue;
		}

		WCHAR PP[1024]; //path and parameters
		ZeroMemory(PP, 1024 * sizeof WCHAR);
		wcscpy_s(PP, path);
		wcscat_s(PP, L" ");
		//wcscat(PP, args);

		// Start the process on behalf of the current user 
		BOOL result = CreateProcessAsUserW(hUserToken,
			PP,
			NULL,
			&Security1,
			&Security2,
			FALSE,
			NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE/*| CREATE_NO_WINDOW*/,//CREATE_NO_WINDOW to Hide Console Process 03-10-2018
			/*lpEnvironment*/NULL,

			//"C:\\ProgramData\\some_dir",
			NULL,
			/*NULL,*/
			&StartupInfo,
			&processInfo);

		if (!result)
		{
			//log error
			//char * lpszPath = WideCharToChar(PP);
			logger->Log(L"Failed to create process", PP);
			//LocalFree(lpszPath);
			DWORD err = GetLastError();
			logger->Log(L"GetLastError returned", err);

		}
		else
		{
			wstring text = L"Created Process ";
			text += PP;
			
			*dwProcessId = processInfo.dwProcessId;
			logger->Log(text, *dwProcessId);
			//log success
		}

		DestroyEnvironmentBlock(lpEnvironment);

		CloseHandle(hImpersonationToken);
		CloseHandle(hUserToken);
		CloseHandle(realToken);

		RevertToSelf();
	}

	WTSFreeMemory(pSession);


	return S_OK;
}


