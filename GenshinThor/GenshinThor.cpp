// GenshinThor.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <string>
#include "utils.h"

int main()
{
    std::cout << "Hello World!\n" << std::endl;
    
    PROCESS_INFORMATION pi;     //process_infomation

    
    std::string gamePath = "D:\\Program Files\\Genshin Impact\\Genshin Impact Game\\YuanShen.exe";
    //std::string gamePath = "cmd.exe";
    std::cout << "尝试打开原神" << std::endl;
   
   if (openApp(gamePath,&pi))
   {
       // 既然我们不使用两个句柄，最好是立刻将它们关闭
      
       std::cout << "打开成功" << std::endl;
       std::cout << "进程的进程ID号:" << pi.dwProcessId << std::endl;   //dwprocessid：新建进程ID
       std::cout << "进程的主线程ID号：" << pi.dwThreadId << std::endl;   //dwthreadid: 新建主线程ID
       DWORD processId = pi.dwProcessId;
       DWORD exploerId = FindProcess(L"YuanShen.exe");
       std::cout << "原神的进程ID号:" << exploerId << std::endl;   //dwprocessid：新建进程ID
       BOOL ret = injectDll("D:\\VSProjects\\GenshinThor\\GenshinThor\\x64\\Debug\\CheatLibrary.dll", exploerId);
       if (ret)
       {
           std::cout << "注入成功~" << std::endl;
       }
       else {
           std::cout << "注入失败." << std::endl;
           system("pause");
       }
       
   }
   else {
       std::cout << "打开失败" << std::endl;
   }

   return 0;

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
