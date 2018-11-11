#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <wchar.h>
#include "CLogger.h"


std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

std::wstring s2ws(const char * cMsg)
{
	const std::string& str(cMsg);
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}


bool DoesFileExist(wstring  filePath) {
	struct _stat32 buf;
	if (_wstat32(filePath.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}
CLogger::CLogger(wstring logPath)
{
	if (!InitializeCriticalSectionAndSpinCount(&CriticalSectionLog, 0x00000400)) {
		MessageBoxA(0, "Failed to Initialize Log Critical Section", "FAILURE", MB_OK);
		
	}
	this->logPath = logPath;
	
}


CLogger::~CLogger()
{
	DeleteCriticalSection(&CriticalSectionLog);
}

void CLogger::ClearLog() {
	EnterCriticalSection(&CriticalSectionLog);
	if (DoesFileExist(logPath))
		_wremove(logPath.c_str());
	LeaveCriticalSection(&CriticalSectionLog);
}

void CLogger::Log(const wchar_t * msg, const wchar_t * bufferErr)
{
	EnterCriticalSection(&CriticalSectionLog);

	//can't enable exception now because of gcc bug that raises ios_base::failure with useless message
	//file.exceptions(file.exceptions() | std::ios::failbit);
	file.open(logPath, std::ios::out | std::ios::app);
	/*if (file.fail())
	throw std::ios_base::failure(std::strerror(errno));
	*/
	//make sure write fails with exception if something is wrong
	file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);
	wstring strMsg(msg);
	wstring strErr(bufferErr);
	file << strMsg << ":" << strErr << std::endl;
	file.close();
	LeaveCriticalSection(&CriticalSectionLog);
}
void CLogger::Log(wstring strMsg, long data)
{
	EnterCriticalSection(&CriticalSectionLog);
	_ltow_s(data, buffer, len, 10);
	wstring strData = wstring(buffer);
	//can't enable exception now because of gcc bug that raises ios_base::failure with useless message
	//file.exceptions(file.exceptions() | std::ios::failbit);
	file.open(logPath, std::ios::out | std::ios::app);
	/*if (file.fail())
	throw std::ios_base::failure(std::strerror(errno));
	*/
	//make sure write fails with exception if something is wrong
	file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);


	file << strMsg << ":" << strData << std::endl;
	file.close();
	LeaveCriticalSection(&CriticalSectionLog);
}
void CLogger::Log(wstring strMsg, wstring strData)
{

	EnterCriticalSection(&CriticalSectionLog);
	//can't enable exception now because of gcc bug that raises ios_base::failure with useless message
	//file.exceptions(file.exceptions() | std::ios::failbit);
	file.open(logPath, std::ios::out | std::ios::app);
	/*if (file.fail())
	throw std::ios_base::failure(std::strerror(errno));
	*/
	//make sure write fails with exception if something is wrong
	file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);


	file << strMsg << ":" << strData << std::endl;
	file.close();

}




void CLogger::Log(wstring strMsg)
{

	EnterCriticalSection(&CriticalSectionLog);
	//can't enable exception now because of gcc bug that raises ios_base::failure with useless message
	//file.exceptions(file.exceptions() | std::ios::failbit);
	file.open(logPath, std::ios::out | std::ios::app);
	/*if (file.fail())
	throw std::ios_base::failure(std::strerror(errno));
	*/
	//make sure write fails with exception if something is wrong
	file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);


	file << strMsg << std::endl;
	file.close();
	LeaveCriticalSection(&CriticalSectionLog);
}


void CLogger::Log(const wchar_t * msg)
{

	EnterCriticalSection(&CriticalSectionLog);
	//can't enable exception now because of gcc bug that raises ios_base::failure with useless message
	//file.exceptions(file.exceptions() | std::ios::failbit);
	file.open(logPath, std::ios::out | std::ios::app);
	/*if (file.fail())
	throw std::ios_base::failure(std::strerror(errno));
	*/
	//make sure write fails with exception if something is wrong
	file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);
	wstring strMsg(msg);

	file << strMsg << std::endl;
	file.close();
	LeaveCriticalSection(&CriticalSectionLog);
}



void CLogger::Log(const char * msg)
{
	Log(s2ws(msg));
}
void CLogger::Log(const char * msg, LONG value) {
	Log(s2ws(msg), value);
}


