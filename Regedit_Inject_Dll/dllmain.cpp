// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "windows.h"
#include "tchar.h"

//因为反单斜杠\在C/C++/C# 中是转义前导字符，例如 \n 代表换行。为了避免歧义，路径中的\必须用//。
#define DEF_CMD L"c:\\Program Files\\Internet Explorer\\iexplore.exe"
#define DEF_ADDR L"https://blog.csdn.net/liu1152239/article/details/72728262"
#define DEF_DST_PROC L"notepad.exe"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    //sz代表以0结尾的字符串，匈牙利表示法
    TCHAR szCmd[MAX_PATH] = { 0, };
    TCHAR szPath[MAX_PATH] = { 0, };
    TCHAR* p = NULL;
    STARTUPINFO si = { 0, };        //用于指定新进程的主窗口特性的一个结构。
    PROCESS_INFORMATION pi = { 0, };            //在创建进程时相关的数据结构之一，该结构返回有关新进程及其主线程的信息

    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;             //https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfow
    si.wShowWindow = SW_HIDE;                   //Hides the window and activates another window

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (!GetModuleFileName(NULL, szPath, MAX_PATH))
            break;
        
        if (!(p = _tcsrchr(szPath, '\\')))   // 最后一次出现的位置
            break;

        if (_tcsicmp(p + 1, DEF_DST_PROC)) //相等返回0
            break;

        wsprintf(szCmd, L"%s %s", DEF_CMD, DEF_ADDR); //第一个参数是字符缓冲区，后面是格式字符串，将其写入缓冲区中，该函数返回该字符串的长度。

        if (!CreateProcess(NULL, (LPTSTR)(LPCTSTR)szCmd, //执行的命令行参数
            NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,     //NORMAL_PRIORITY_CLASS 线程优先级，正常优先级
            NULL, NULL, 
            &si,   //指向 STARTUPINFO或STARTUPINFOEX结构的指针
            &pi))  //指向 PROCESS_INFORMATION结构的指针，该结构接收有关新进程的标识信息。
            break;
        if (pi.hProcess != NULL)
        {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

