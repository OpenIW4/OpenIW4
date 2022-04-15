#include "loader/loader.hpp"

#include "Sys/Sys.hpp"
#include "Com/Com.hpp"
#include "Win/Win.hpp"
#include "Dvar/Dvar.hpp"
#include "Cmd/Cmd.hpp"

#include "defs.hpp"

#include <utils/memory/memory.hpp>

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

//THUNK : 0x004D6F80
char* __cdecl I_strncpyz(char* Destination, char* Source, int a3)
{
    return memory::call<char* (char*, char*, int)>(0x004D6F80)(Destination, Source, a3);
}

//THUNK : 0x00404B20
int __cdecl Cbuf_AddText(int a1, const char* a2)
{
    return memory::call<int(int, const char*)>(0x00404B20)(a1, a2);
}

//THUNK : 0x000x437EB0
void* Field_Clear(std::uint32_t* a1)
{
    return memory::call<void*(std::uint32_t*)>(0x00437EB0)(a1);
}

//DONE : 0x004785B0
char* va(char* Format, ...)
{
    int Value; // eax
    int v2; // ecx
    char* v3; // esi
    int v4; // eax
    va_list ArgList; // [esp+Ch] [ebp+8h] BYREF

    va_start(ArgList, Format);
    Value = *Sys_GetValue(1);
    v2 = *(DWORD*)(Value + 2048);
    *(DWORD*)(Value + 2048) = (v2 + 1) % 2;
    v3 = (char*)(Value + (v2 << 10));
    v4 = _vsnprintf(v3, 0x400u, Format, ArgList);
    v3[1023] = 0;
    if (v4 < 0 || v4 >= 1024)
        //Com_Error(1, (char*)&byte_70924C);
    return v3;
}

//This function is blank in 177
//DONE : 0x004BB9B0
void Session_InitDvars()
{
    
}

//DONE : 0x004513D0
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    const char* LocalizationFilename; // eax
    char* error_msg; // eax

    Sys_InitializeCriticalSections();
    Sys_InitMainThread();
    if (Win_InitLocalization(0))
    {
#ifdef MATCHING
        if (!sub_426080(lpCmdLine, "allowdupe", 9) && lpCmdLine[9] <= 32 || (sub_64D200(), sub_411350()))
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

//Temp fix until further reimp
static std::uint32_t GetXAssetHandlerSize(XAssetType type)
{
    typedef std::uint32_t(__cdecl* DB_GetXAssetSizeHandler_t)();
    auto DB_GetXAssetSizeHandler = (DB_GetXAssetSizeHandler_t*)0x799488;
    return DB_GetXAssetSizeHandler[type]();
}

void* ReallocateAssetPool(XAssetType type, std::size_t newSize)
{
    auto DB_XAssetPool = (void**)0x007998A8;
    auto g_poolSize = (std::uint32_t*)0x007995E8;

    auto size = GetXAssetHandlerSize(type);
    auto poolEntry = malloc(newSize * size);
    DB_XAssetPool[type] = poolEntry;
    g_poolSize[type] = newSize;

    return poolEntry;
}

void commands()
{
    *(int*)(0x009FBE24) /*cg_fov*/ = Dvar_RegisterFloat("cg_fov", 90.0f, 0.0f, FLT_MAX, 68, "The field of view angle in degrees");
}

void patches()
{
    Sys_ShowConsole();
    ReallocateAssetPool(XAssetType::ASSET_TYPE_WEAPON, 3000);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    ::AllocConsole();

	loader::load("iw4mp.exe");  //177
    commands();
    patches();
	memory::replace(0x004513D0, main);
	return memory::call<int()>(0x006BAC0F)();
}