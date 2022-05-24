#include "Sys/Sys.hpp"
#include "Com/Com.hpp"
#include "Win/Win.hpp"
#include "Dvar/Dvar.hpp"
#include "Cmd/Cmd.hpp"
#include "Cbuf/Cbuf.hpp"
#include "DB/DB.hpp"
#include "LSP/LSP.hpp"

#include "defs.hpp"

#include <memory/memory.hpp>

//THUNK : 0x0064AE50
double inlined_2()
{
	return memory::call<double()>(0x0064AE50)();
}

//DONE : 0x0047ADF0
void InitTiming()
{
	*(double*)(0x0047ADF0) /*msecPerRawTimerTick*/ = inlined_2() * 1000.0;
}

//THUNK : 0x000x437EB0
void* Field_Clear(std::uint32_t* a1)
{
    return memory::call<void*(std::uint32_t*)>(0x00437EB0)(a1);
}

//This function is blank in 159
//DONE : 0x4BB9B0
void Session_InitDvars()
{
    
}

//Temp fix until further reimp

void* ReallocateAssetPool(XAssetType type, std::size_t newSize)
{
    auto DB_XAssetPool = (void**)0x007998A8;
    auto g_poolSize = (std::uint32_t*)0x007995E8;

    auto size = DB_GetXAssetTypeSize(type);
    auto poolEntry = malloc(newSize * size);
    DB_XAssetPool[type] = poolEntry;
    g_poolSize[type] = newSize;

    return poolEntry;
}

void killCeg()
{
    auto ceg = {0x402FD0, 0x4044E0,
    0x42BEB0, 0x438620,
    0x446740, 0x4A81D0,
    0x4CA590, 0x4CC180,
    0x4F3BF0, 0x4FAC40, 0x471B20, 0x4A76F0 };

    for (const auto& address : ceg)
    {
        memory::kill(address);
    }
}

void patches()
{
    Sys_ShowConsole();
    ReallocateAssetPool(ASSET_TYPE_WEAPON, 2400);
    *(std::float_t*)(0x9FBE24) = Dvar_RegisterFloat("cg_fov", 90.0f, 0.0f, FLT_MAX, 68, "The field of view angle in degrees");
}

//DONE : 0x004513D0
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    patches();

    const char* LocalizationFilename; // eax
    char* error_msg; // eax

    Sys_InitializeCriticalSections();
    Sys_InitMainThread();
    if (Win_InitLocalization(0))
    {
#ifdef MATCHING
        if (!I_strnicmp(lpCmdLine, "allowdupe", 9) && lpCmdLine[9] <= 32 || (sub_64D200(), sub_411350()))
        {
#endif
            if (!hPrevInstance)
            {
                Com_InitParse();
                Dvar_Init();
                InitTiming();
                Sys_EnumerateHw();
                Sys_RecordAccessibilityShortcutSettings();
                Sys_AllowAccessibilityShortcutKeys(0);
                *(HINSTANCE*)(0x064A3AD4) = 0;
                I_strncpyz((char*)(0x0649F760) /*sys_cmdline*/, lpCmdLine, 1024);
                Sys_CreateSplashWindow();
                Sys_ShowSplashWindow();
                Sys_RegisterClass();
                SetErrorMode(1u);
                Sys_Milliseconds();
                Session_InitDvars();
                Com_Init((char*)(0x0649F760) /*sys_cmdline*/);
                Cbuf_AddText(0, "readStats\n");
                Sys_getcwd();
                SetFocus(*(HWND*)(0x064A3AD0) /*g_wv*/);

                while (1)
                {
                    if (*(int*)(0x0064A3ADC))
                    {
                        Sys_Sleep(5u);
                    }

                    Sys_CheckQuitRequest();
                    Com_Frame();
                }
            }
#ifdef MATCHING
        }
#endif
        Win_ShutdownLocalization();
        return 0;
    }
    else
    {
        LocalizationFilename = Win_GetLocalizationFilename();
        error_msg = va(
            "Could not load %s.\n\nPlease make sure Modern Warfare 2 is run from the correct folder.",
            LocalizationFilename);
        MessageBoxA(0, error_msg, "Modern Warfare 2 - Fatal Error", MB_ICONHAND);
        return 0;
    }
}

void ZynamicPatch()
{
    memory::replace(0x06BABA1, WinMain);
}
