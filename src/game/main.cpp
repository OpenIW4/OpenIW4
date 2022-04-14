#include "loader/loader.hpp"

#include "Sys/Sys.hpp"
#include "Com/Com.hpp"
#include "Win/Win.hpp"
#include "Dvar/Dvar.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x005C87E0
void inlined_1()
{
	memory::call<void()>(0x005C87E0)();
}

//THUNK : 0x005C3560
double inlined_2()
{
	return memory::call<double()>(0x005C3560)();
}

//DONE : 0x005C3680
void InitTiming()
{
	*(double*)(0x006424348) = inlined_2() * 1000.0;
}

void main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    const char* v4; // eax
    char* v5; // eax
    int result; // eax
    char DstBuf[256]; // [esp+0h] [ebp-100h] BYREF

    Sys_InitializeCriticalSections();
    Sys_InitMainThread();
    if (Win_InitLocalization(0))
    {
#ifdef MATCHING
        if (!sub_5C1C50(lpCmdLine, "allowdupe", 9) && lpCmdLine[9] <= 32 || (sub_5C90E0(), sub_5C9150()))
        {
#endif
            if (!hPrevInstance)
            {
                Com_InitParse();
                Dvar_Init();
                InitTiming();
                inlined_1();
                Sys_RecordAccessibilityShortcutSettings();
                Sys_AllowAccessibilityShortcutKeys(0);
                *(HINSTANCE*)(0x0642E21C) = hInstance;  //Global hinstance
                I_strncpyz(sys_cmdline, lpCmdLine, 1024);
                Sys_CreateSplashWindow();
                Sys_ShowSplashWindow();
                Sys_RegisterClass();
                SetErrorMode(1u);
                Sys_Milliseconds();
                Session_InitDvars();
                //nullsub_3();                                    // __Z20Sys_InitStreamThreadv
                Com_Init();
                Cbuf_AddText(0, "readStats\n");
                _getcwd(DstBuf, 256);
                SetFocus(*(HWND*)(0x0642E218) /*g_wv*/);
                while (1)
                {
                    if (*(int*)(0x00642E224))
                        Sys_Sleep(5u);
                    Sys_CheckQuitRequest();
                    Com_Frame();
                }
            }
#ifdef MATCHING
        }
#endif
        Win_ShutdownLocalization();
        result = 0;
    }
    else
    {
        v4 = Win_GetLocalizationFilename();
        v5 = va("Could not load %s.\n\nPlease make sure Modern Warfare 2 is run from the correct folder.", v4);
        MessageBoxA(0, v5, "Modern Warfare 2 - Fatal Error", 0x10u);
        result = 0;
    }
    return result;
}
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	loader::load("iw4mp.exe");  //211
	memory::replace(0x005C93C0, main);
	return memory::call<int()>(0x0066250F)();
}