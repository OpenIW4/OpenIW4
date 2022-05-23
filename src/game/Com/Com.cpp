#include "Com.hpp"
#include "../main.hpp"
#include "../Sys/Sys.hpp"
#include "../Render/Render.hpp"
#include "../DB/DB.hpp"

#include <memory/memory.hpp>

// None of these do anything to my knowledge
// but for sake of completion they are here
//DONE : 0x004FF220
void Com_InitParse()
{
    *(int*)(0x06466A28) = 1;
    *(bool*)(0x06466A2C) = 0;
    *(bool*)(0x06466A2D) = 1;
    *(bool*)(0x06466A2E) = 0;
    *(bool*)(0x06466A2F) = 0;
    *(bool*)(0x06466A30) = 0;
    *(int*)(0x06466A34) = 0;
    *(int*)(0x06466A38) = 0;
    *(int*)(0x06466A3C) = 0;
    *(int*)(0x06466A40) = 0;
    *(int*)(0x0646B034) = 1;
    *(bool*)(0x0646B038) = 0;
    *(bool*)(0x0646B039) = 1;
    *(bool*)(0x0646B03A) = 0;
    *(bool*)(0x0646B03B) = 0;
    *(bool*)(0x0646B03C) = 0;
    *(int*)(0x0646B040) = 0;
    *(int*)(0x0646B044) = 0;
    *(int*)(0x0646B048) = 0;
    *(int*)(0x0646B04C) = 0;
    *(int*)(0x0646F640) = 1;
    *(bool*)(0x0646F644) = 0;
    *(bool*)(0x0646F645) = 1;
    *(bool*)(0x0646F646) = 0;
    *(bool*)(0x0646F647) = 0;
    *(bool*)(0x0646F648) = 0;
    *(int*)(0x0646F64C) = 0;
    *(int*)(0x0646F650) = 0;
    *(int*)(0x0646F654) = 0;
    *(int*)(0x0646F658) = 0;
    *(int*)(0x06473C4C) = 1;
    *(bool*)(0x06473C50) = 0;
    *(bool*)(0x06473C51) = 1;
    *(bool*)(0x06473C52) = 0;
    *(bool*)(0x06473C53) = 0;
    *(bool*)(0x06473C54) = 0;
    *(int*)(0x06473C58) = 0;
    *(int*)(0x06473C5C) = 0;
    *(int*)(0x06473C60) = 0;
    *(int*)(0x06473C64) = 0;
}

//DONE : 0x004B7230
void Com_Frame()
{
    int* Value; // eax
    int v1; // esi

    Value = Sys_GetValue(2);

    if (!_setjmp(Value))
    {
        DB_Update();
        Com_Frame_Try_Block_Function();
        *(int*)(0x01AD8F80) = *(int*)(0x01AD8F80) + 1 /*com_frameNumber*/;
    }

    if (*(int*)(0x01AD8F1C)/*com_errorEnteredCount*/)
    {
        if (Sys_IsMainThread())
        {
            *(int*)(0x001AD8F2C) = R_PopRemoteScreenUpdate();
        }

        Sys_EnterCriticalSection(CRITSECT_COM_ERROR);

        if (Sys_IsMainThread())
        {
            Com_EnterError();
        }

        v1 = *(int*)(0x01AD8F1C)/*com_errorEnteredCount*/;
        Sys_LeaveCriticalSection(CRITSECT_COM_ERROR);

        if (Sys_IsMainThread())
        {
            R_PushRemoteScreenUpdate(*(int*)(0x001AD8F2C));
        }

        if (v1)
        {
            Com_ErrorCleanup();
            Com_StartHunkUsers();
        }
    }
}

//DONE : 0x004D9640
void Com_Init(char* src)
{
    int* Value; // eax
    char* v2; // eax

    Value = Sys_GetValue(2);
    if (_setjmp(Value))
    {
        v2 = va("Error during initialization:\n%s\n", (char*)(0x01AD7EC0)/*com_errorMessage*/);
        Sys_Error(v2);
    }
    Com_Init_Cont();
    Com_AddStartupCommands();
    Com_StartHunkUsers();
}

//THUNK : 0x0060BAE0
void Com_Init_Cont()
{
    memory::call<void()>(0x0060BAE0)();
}

//THUNK : 0x0047DCA0
void Com_Frame_Try_Block_Function()
{
    memory::call<void()>(0x0047DCA0)();
}

//DONE : 0x005091E0
bool Com_EnterError()
{
    bool result; // eax

    if (*(bool*)(0x066DAD5D))
    {
        if (*(int*)(0x066DAD5C) /*r_glob_0*/)
        {
            result = Sys_ReleaseThreadOwnership();
            *(int*)(0x066DAD5C) /*r_glob_0*/ = 0;
        }
    }
    return result;
}

//THUNK : 0x0060BFD0
int Com_ErrorCleanup()
{
    return memory::call<int()>(0x0060BFD0)();
}

//THUNK : 0x0060BF40
int Com_StartHunkUsers()
{
    return memory::call<int()>(0x60BF40)();
}

//THUNK : 0x0060C3D0
void Com_AddStartupCommands()
{
    memory::call<void()>(0x60C3D0)();
}

//THUNK : 0x402500
void Com_Printf(std::uint32_t channel, const char* Format, ...)
{
    char Buffer[0x1000];
    va_list ArgList;

    va_start(ArgList, Format);
    vsnprintf(Buffer, sizeof(Buffer), Format, ArgList);
    Buffer[sizeof(Buffer) - 1] = '\0';
    memory::call<void(std::uint32_t, char*, int /*error*/)>(0x4AA830)(channel, Buffer, 0);
}

//DONE : 0x00413DE0
int Com_sprintf(char* buf, size_t bufCount, const char* fmt, ...)
{
    va_list va;

    va_start(va, fmt);
    std::int32_t result = _vsnprintf(buf, bufCount, fmt, va);
    buf[bufCount - 1] = 0;
    va_end(va);

    return result;
}

//DONE : 0x416E40
void* Com_Memcpy(void* dest, const void* src, int size)
{
    return memcpy(dest, src, size); //seems like Com_Memcpy appears to be a wrapper across a few engines
}

//DONE : 0x4C9AD0
void* Com_Memset(void* dest, std::int32_t value, std::size_t size)
{
    return memset(dest, value, size); //also seems like a wrapper for memset kinda like above
}

// DONE : Inlined
int I_strlen(const char* s)
{
    const char *str;

    for (str = s; *str; ++str)
        ;
    return (s - str);
}

//DONE : 0x4EF820
char I_CleanChar(char c)
{
    if (c == 0x92)
    {
        c = '\'';
    }

    return c;
}

//DONE : 0x4E71E0
bool I_isdigit(int c)
{
    return (c >= 0x30 && c <= 0x39);
}

//DONE : 0x437C80
bool I_islower(int c)
{
    return (c >= 0x61 && c <= 0x7A);
}

//DONE : 0x4D6F80
void I_strncpyz(char* dest, const char* src, int destsize)
{
    strncpy(dest, src, destsize - 1);
    dest[destsize - 1] = '\0';
}

// DONE : 0x42F2A0
int I_strncmp(const char* s0, const char* s1, int n)
{
    int c1, c2;

    do
    {
        c1 = *s0++;
        c2 = *s1++;

        if (!n--)
        {
            return 0;
        }

        if (c1 != c2)
        {
            return c1 < c2 ? -1 : 1;
        }

    } while (c1 != 0);

    return 0;
}

// DONE : 0x4B0100
int I_strcmp(const char* s0, const char* s1)
{
    return I_strncmp(s0, s1, INT_MAX);
}

// DONE : 0x426080
int I_strnicmp(const char* s0, const char* s1, int n)
{
    return memory::call<int(const char*, const char*, int)>(0x426080)(s0, s1, n);
}

//DONE : 0x4DCF20
int I_stricmp(const char* s0, const char* s1)
{
    return I_strnicmp(s0, s1, INT_MAX);
}

//DONE : 0x466BE0
void I_strncat(char* dest, int size, const char* src)
{
    int l1;

    l1 = I_strlen(dest);
    if (l1 >= size)
    {
        //Com_Error(ERR_FATAL, "\x15I_strncat: already overflowed");
    }

    I_strncpyz(&dest[l1], src, size - l1);
}

//DONE : 0x409F60
char* I_strlwr(char* s)
{
    char value;

    for (char* i = s; *i; ++i)
    {
        value = *i;

        // If char is upper convert to lower
        if (value >= 0x41 && value <= 0x5A)
        {
            *i = value + 0x20;
        }
    }

    return s;
}

#define I_COLOR_ESCAPE '^'
#define I_IsColorString(p) (p && *(p) == I_COLOR_ESCAPE && *((p)+1) && *((p)+1) != I_COLOR_ESCAPE)

//DONE : 0x4AD470
char* I_CleanStr(char* string)
{
    char* d;
    char* s;
    int c;

    s = string;
    d = string;

    while ((c = *s) != 0)
    {
        if (I_IsColorString(s))
        {
            s++;
        }
        else if (c >= 0x20 && c <= 0x7E)
        {
            *d++ = c;
        }

        s++;
    }

    *d = '\0';
    return string;
}

//DONE : 0x49A580
int I_DrawStrlen(const char* str)
{
    const char* s;
    const char* d;
    int c;

    s = str;
    d = str;

    int len = 0;
    while ((c = *s) != 0)
    {
        if (I_IsColorString(s))
        {
            s++;
        }
        else
        {
            len++;
        }

        s++;
    }
    
    return len;
}
