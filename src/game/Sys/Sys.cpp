#include "Sys.hpp"
#include "../Com/Com.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x004305E0
LRESULT Sys_ShowConsole()
{
    HMODULE v0;
    if (!*(HWND*)(0x064A3288))
    {
        v0 = GetModuleHandleA(0);
        Sys_CreateConsole(v0);
    }
    ShowWindow(*(HWND*)(0x064A3288), 1);
    return SendMessageA(*(HWND*)(0x0064A328C), 0xB6u, 0, 0xFFFF);
}

//THUNK : 0x004288A0
ATOM Sys_CreateConsole(HINSTANCE hInstance)
{
    return memory::call<ATOM(HINSTANCE)>(0x004288A0)(hInstance);
}

//THUNK : 0x0042F0A0
void Sys_InitializeCriticalSections()
{
    memory::call<void()>(0x0042F0A0)();
}

//THUNK : 0x004301B0
void Sys_InitMainThread()
{
    memory::call<void()>(0x004301B0)();
}

//DONE : 0x004169C0
void Sys_Sleep(DWORD dwMilliseconds)
{
    Sleep(dwMilliseconds);
}

//THUNK : 0x0043EBB0
void Sys_CheckQuitRequest()
{
    memory::call<void()>(0x0043EBB0)();
}

//THUNK : 0x004C8E30
void Sys_RecordAccessibilityShortcutSettings()
{
    memory::call<void()>(0x004C8E30)();
}

//THUNK : 0x0040BC60
void Sys_AllowAccessibilityShortcutKeys(bool a1)
{
    memory::call<void(bool)>(0x0040BC60)(a1);
}

//THUNK : 0x00475F00
void Sys_CreateSplashWindow()
{
    memory::call<void()>(0x00475F00)();
}

//DONE : 0x004A7B10
void Sys_ShowSplashWindow()
{
    if (*(HWND*)(0x064A3050)/*g_splash_wv*/)
    {
        ShowWindow(*(HWND*)(0x064A3050)/*g_splash_wv*/, 5);
        UpdateWindow(*(HWND*)(0x064A3050)/*g_splash_wv*/);
    }
}

//THUNK : 0x0064D270
void Sys_RegisterClass()
{
    memory::call<void()>(0x0064D270)();
}

//DONE : 0x0042A660
int Sys_Milliseconds()
{
    if (!*(int*)(0x064A304C)/*Sys_Milliseconds(void)::initialized*/)
    {
        *(int*)(0x064A3034) /*sys_timeBase*/ = timeGetTime();
        *(int*)(0x064A304C)/*Sys_Milliseconds(void)::initialized*/ = 1;
    }
    return timeGetTime() - *(int*)(0x064A3034) /*sys_timeBase*/;
}

//THUNK : 0x004EC730
int Sys_GetValue(int a1)
{
    return memory::call<int(int)>(0x004EC730)(a1);
}

//THUNK : 0x0064D300
void Sys_getcwd()
{
    memory::call<void()>(0x0064D300)();
}