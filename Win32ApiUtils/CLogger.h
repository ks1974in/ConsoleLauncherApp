#pragma once
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wchar.h>
//#pragma comment(lib, "LIBCMT.LIB")
//#pragma comment(lib, "MSVCRT.LIB")
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1  
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1
using namespace std;
// CProcessManager



class CLogger
{
private:
	size_t len = 260;
	wchar_t* buffer = new wchar_t[len];
	wstring logPath;
	wofstream file;
	CRITICAL_SECTION CriticalSectionLog;
public:
	CLogger(wstring fileName);
	~CLogger();
	void ClearLog();
	void Log(const wchar_t * msg, const wchar_t * bufferErr);
	void Log(wstring strMsg, long data);
	void Log(wstring strMsg, wstring strData);
	void Log(wstring strMsg);
	void Log(const wchar_t * msg);


	void Log(const char * msg);
	void Log(const char * msg, LONG value);

};

