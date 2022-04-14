#include "Sys.hpp"

#include <utils/memory/memory.hpp>

LRESULT Sys_ShowConsole()
{
    HMODULE v0;
#ifdef MATCHING
    

    if (!*(HWND*)(0x0642D9D0))
    {
        v0 = GetModuleHandleA(0);
        Sys_CreateConsole(v0);
    }
#else
    v0 = GetModuleHandleA(0);
    Sys_CreateConsole(v0);
#endif
    ShowWindow(*(HWND*)(0x0642D9D0), 1);
    return SendMessageA(*(HWND*)(0x00642D9D4), 0xB6u, 0, 0xFFFF);
}

ATOM Sys_CreateConsole(HINSTANCE hInstance)
{
    return memory::call<ATOM(HINSTANCE)>(0x005CB1D0)(hInstance);
}