#include "Sys.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x005CB500
LRESULT Sys_ShowConsole()
{
    HMODULE v0;
    if (!*(HWND*)(0x0642D9D0))
    {
        v0 = GetModuleHandleA(0);
        Sys_CreateConsole(v0);
    }
    ShowWindow(*(HWND*)(0x0642D9D0), 1);
    return SendMessageA(*(HWND*)(0x00642D9D4), 0xB6u, 0, 0xFFFF);
}

//THUNK : 0x005CB1D0
ATOM Sys_CreateConsole(HINSTANCE hInstance)
{
    return memory::call<ATOM(HINSTANCE)>(0x005CB1D0)(hInstance);
}

//THUNK : 0x005C3CD0
void Sys_InitializeCriticalSections()
{
    memory::call<void()>(0x005C3CD0)();
}

//THUNK : 0x00576130
void Sys_InitMainThread()
{
    memory::call<void()>(0x00576130)();
}

//DONE : 0x00576000
void Sys_Sleep(DWORD dwMilliseconds)
{
    Sleep(dwMilliseconds);
}

//THUNK : 0x005C9290
void Sys_CheckQuitRequest()
{
    memory::call<void()>(0x005C9290)();
}

//THUNK : 0x005CC790
void Sys_RecordAccessibilityShortcutSettings()
{
    memory::call<void()>(0x005CC790)();
}

//THUNK : 0x005CC7E0
void Sys_AllowAccessibilityShortcutKeys(bool a1)
{
    memory::call<void(bool)>(0x005CC7E0)(a1);
}