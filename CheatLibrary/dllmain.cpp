// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "native.h"
#include "log.h"
#include <string>
#include <iostream>

void init();


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)init, new HMODULE(hModule), 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


void init(){
    AllocConsole();
    freopen("CONOUT$", "a+", stdout);
    LOGI("DLL注入成功,开始加载...");
    while (GetModuleHandle(L"UserAssembly.dll") == nullptr) {
        LOGI("等待加载UserAssembly.dll...");
        Sleep(2000);
    }
    LOGI("等待15秒使游戏加载完毕...");
    Sleep(15000);
    LOGI("尝试关闭保护");
    if (CloseHandleByName(L"\\Device\\mhyprot2"))
    {
        LOGI("保护关闭成功~");
    }
    else {
        LOGE("保护关闭失败~");
    }

}