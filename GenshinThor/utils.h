#pragma once
#include <Windows.h>
#include <string>


void startApp(const char*);


LPCWSTR covert(std::string);


LPWSTR covert2(std::string);

BOOL openApp(std::string, PROCESS_INFORMATION*);
LPSTR covert3(std::string );

BOOL openApp2(std::string, PROCESS_INFORMATION*);

DWORD FindProcess(LPCWSTR);

BOOL injectDll(LPCSTR, const DWORD);