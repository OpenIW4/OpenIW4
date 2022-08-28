#include "main.hpp"
#include "loader/loader.hpp"

#include "Sys/Sys.hpp"
#include "Com/Com.hpp"
#include "Win/Win.hpp"
#include "Dvar/Dvar.hpp"
#include "Cmd/Cmd.hpp"
#include "Cbuf/Cbuf.hpp"
#include "DB/DB.hpp"
#include "LSP/LSP.hpp"

#include "MSG/MSG.hpp"
#include "Huffman/Huffman.hpp"

#include "defs.hpp"

#include <utils/memory/memory.hpp>

#define NOUPNP

//TODO : 0x0064AE50
double SecondsPerTick()
{
	//return memory::call<double()>(0x0064AE50)();

    memory::call<void(std::int32_t)>(0x4A5E00)(2);

    std::uint64_t time, time2;
    long long v8;

    HANDLE currentThread = GetCurrentThread();
    std::int32_t nPriority = GetThreadPriority(currentThread);
    SetThreadPriority(currentThread, nPriority);
    Sleep(0); //why is this called, IW?

    LARGE_INTEGER performanceCount;
    performanceCount.QuadPart = 0;
    LARGE_INTEGER v4;
    v4.QuadPart = 0;
    LARGE_INTEGER frequency;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&performanceCount);

    time = __rdtsc();

    do
    {
        QueryPerformanceCounter(&v4);
        v8 = v4.QuadPart - performanceCount.QuadPart;
    } while ((std::double_t)(v4.QuadPart - performanceCount.QuadPart) / (std::double_t)frequency.QuadPart <= 0.25);

    time2 = __rdtsc();

    QueryPerformanceCounter(&v4);

    *(std::double_t*)&v8 = (std::double_t)(v4.QuadPart - performanceCount.QuadPart)
        /
        (((std::double_t)(std::int64_t)time2 - time) * (std::double_t)frequency.QuadPart);
    SetThreadPriority(currentThread, nPriority);

    memory::call<void(std::int32_t)>(0x4A5E00)(0);

    return *(std::double_t*)&v8;
}

//DONE : 0x47ADF0
void InitTiming()
{
	*(std::double_t*)(0x47ADF0) /*msecPerRawTimerTick*/ = SecondsPerTick() * 1000.0;
}

//DONE : 0x437EB0
void* Field_Clear(field_t* edit)
{
    void* result = memset(edit->buffer, 0, sizeof(edit->buffer));

    edit->cursor = 0;
    edit->scroll = 0;
    edit->drawWidth = 256;

    return result;
}

//This function is blank in 159
//DONE : 0x4BB9B0
void Session_InitDvars()
{
    
}

//Temp fix until further reimp

void* ReallocateAssetPool(XAssetType type, std::size_t newSize)
{
    auto DB_XAssetPool = (void**)0x7998A8;
    auto g_poolSize = (std::uint32_t*)0x7995E8;

    auto size = DB_GetXAssetTypeSize(type);
    auto poolEntry = malloc(newSize * size);
    DB_XAssetPool[type] = poolEntry;
    g_poolSize[type] = newSize;

    return poolEntry;
}

void killCeg()
{
    std::initializer_list<std::int32_t> ceg = {0x402FD0, 0x4044E0,
    0x42BEB0, 0x438620,
    0x446740, 0x4A81D0,
    0x4CA590, 0x4CC180,
    0x4F3BF0, 0x4FAC40, 0x471B20, 0x4A76F0 };

    for (const std::int32_t& address : ceg)
    {
        memory::kill(address);
    }
}

void patches()
{
#if defined(NOUPNP) && defined(DEBUG)
    memory::kill(0x4797F0);
#endif
    killCeg();
    Sys_ShowConsole();
    ReallocateAssetPool(ASSET_TYPE_WEAPON, 2400);
    static const dvar_t* cg_fov = *reinterpret_cast<dvar_t**>(0x9FBE24);
    cg_fov = Dvar_RegisterFloat("cg_fov", 90.0f, 0.0f, FLT_MAX, 68, "The field of view angle in degrees");
}

//DONE : 0x004513D0
std::int32_t main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
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
                Com_Init((char*)(0x649F760) /*sys_cmdline*/);
                Cbuf_AddText(0, "readStats\n");
                Sys_getcwd();
                SetFocus(*(HWND*)(0x64A3AD0) /*g_wv*/);

                while (1)
                {
                    if (*(int*)(0x64A3ADC))
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

void replace_funcs()
{
    memory::replace(0x4513D0, main);

    memory::replace(0x4305E0, Sys_ShowConsole);
    memory::replace(0x43D570, Sys_Error);
    memory::replace(0x4CF7F0, DB_DirtyDiscError);
}

std::int32_t __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, std::int32_t nShowCmd)
{
    loader::load("iw4mp.exe");
    replace_funcs();
    return memory::call<std::int32_t()>(0x6BAC0F)();
}
