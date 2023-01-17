#include <iostream>
#include <windows.h>
#include <string>
#include <TlHelp32.h>
#include "utils.h"


void startApp(const char* path) {

}

LPCWSTR covert(std::string src) {
	size_t length = src.length();
	size_t covertLength = 0;
	wchar_t *target = (wchar_t *)malloc(length * sizeof(wchar_t));
	mbstowcs_s(&covertLength, target, length + 1, src.c_str(),_TRUNCATE);
	return target;
}

LPWSTR covert2(std::string src) {
	const char* c = src.c_str();
	DWORD size = MultiByteToWideChar(CP_ACP,0,c,-1,NULL,0);
	wchar_t* target = (wchar_t*)malloc(size*sizeof(wchar_t*));
	MultiByteToWideChar(CP_ACP, 0, c, -1, target, size);
	return target;

}
LPSTR covert3(std::string src) {
	const char* c = src.c_str();
	size_t s = strlen(c);
	char* target = new char[s];
	strcpy(target,c);
	return target;

}

BOOL openApp(std::string app, PROCESS_INFORMATION *pi) {
	STARTUPINFO si = { sizeof(si) };  //startupinfo 结构体初始化
	si.dwFlags = STARTF_USESHOWWINDOW; // 指定wShowWindow成员有效
	si.wShowWindow = TRUE;   // 此成员设为TRUE的话则显示新建进程的主窗口，

	BOOL ret = ::CreateProcess(NULL, covert2(app), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, pi);
	
	if (ret)
	{
		::CloseHandle(pi->hThread);     //结束新创建主线程句柄
		::CloseHandle(pi->hProcess);    //结束新创建主进程句柄
	}
	
	return ret;
}

BOOL openApp2(std::string app, PROCESS_INFORMATION *pi) {
	HANDLE appToken;
	BOOL ret = OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&appToken);
	if (!ret)
	{
		std::cout << "获取进程token失败" << std::endl;
		return false;
	}

	STARTUPINFOEXA si = {  };  //startupinfo 结构体初始化
	si.StartupInfo.cb = sizeof(si); // 指定wShowWindow成员有效
	//si.lpAttributeList = TRUE;   // 此成员设为TRUE的话则显示新建进程的主窗口，
	ret = CreateProcessAsUserA(appToken,0,covert3(app),0,0,0, EXTENDED_STARTUPINFO_PRESENT,0,0,(LPSTARTUPINFOA) & si, pi);
	if (ret)
	{
		::CloseHandle(pi->hThread);     //结束新创建主线程句柄
		::CloseHandle(pi->hProcess);    //结束新创建主进程句柄
	}
	return ret;
}

DWORD FindProcess(LPCWSTR  ProcessName) {
	DWORD ProcessId = 0;
	HANDLE process = NULL;
	process = CreateToolhelp32Snapshot(TH32CS_SNAPALL, ::GetCurrentProcessId());
	PROCESSENTRY32 pe = { sizeof(pe) };
	while (::Process32Next(process, &pe))
	{
		if (::lstrcmpi(pe.szExeFile, ProcessName) == 0)
		{
			ProcessId = pe.th32ProcessID;
			std::cout << "找到进程: " << ProcessId << std::endl;
		}
	}
	return ProcessId;
}

BOOL injectDll(LPCSTR dll_path, const DWORD remote_pro_id) {
	HANDLE h_token;
	HANDLE h_remote_process;
	TCHAR msg[MAX_PATH];
	DWORD path_len = strlen(dll_path) + 1;
	SIZE_T write_len;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &h_token))
	{
		TOKEN_PRIVILEGES tkp;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid); tkp.PrivilegeCount = 1; tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(h_token, FALSE, &tkp, sizeof(tkp), NULL, NULL);
		std::cout << "OpenSuccess" << std::endl;
	}
	if ((h_remote_process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, remote_pro_id)) == NULL)
	{
		std::cout << "OpenError" << std::endl;
		return FALSE;
	}
	char* lib_func_buf;
	lib_func_buf = (char*)VirtualAllocEx(h_remote_process, NULL, path_len, MEM_COMMIT, PAGE_READWRITE);
	if (lib_func_buf == NULL) {
		std::cout << "BufReadError" << std::endl;
		return FALSE;
	}

	if (WriteProcessMemory(h_remote_process, lib_func_buf, (void*)dll_path, path_len, &write_len) == 0) {
		std::cout << "WriteError!" << std::endl;
		return FALSE;
	}

	if (path_len != write_len)
	{
		std::cout << "WriteNotFull!" << std::endl;
		return FALSE;
	}

	PTHREAD_START_ROUTINE load_start_addr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");
	if (load_start_addr == NULL)
	{
		std::cout << "LoadError" << std::endl;
		return FALSE;
	}
	HANDLE h_remote_thread;
	if ((h_remote_thread = CreateRemoteThread(h_remote_process, NULL, 0, load_start_addr, lib_func_buf, 0, NULL)) == NULL) {
		std::cout << "InjectError" << std::endl;
		return FALSE;
	}
	return TRUE;
}