#include "Com.hpp"
#include "../Sys/Sys.hpp"
#include "../Render/Render.hpp"
#include "../DB/DB.hpp"
#include "../LSP/LSP.hpp"

#include <utils/memory/memory.hpp>

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
void Com_Memcpy(void* dest, const void* src, int size)
{
    // In Quake III this is all inline assembly
    memcpy(dest, src, size);
}

//DONE : 0x4C9AD0
void Com_Memset(void* dest, const int val, int count)
{
    // In Quake III this is all inline assembly
    memset(dest, val, count);
}

static int* com_errorPrintsCount = reinterpret_cast<int*>(0x1AD7910);
static int* com_fixedConsolePosition = reinterpret_cast<int*>(0x1AD8EC8);

//DONE : 0x4AA830
void Com_PrintMessage_t(int channel, const char* msg, int error)
{
    memory::call<void(int, const char*, int)>(0x4AA830)(channel, msg, error);
}

//DONE : 0x4F8C70
void Com_PrintError(int channel, const char* fmt, ...)
{
    char error[0x1000];

    const char* foundError = I_stristr(fmt, "error");
    const char* prefix = (foundError == NULL) ? "^1Error: " : "^1";

    I_strncpyz(error, prefix, sizeof(error));
    int len = I_strlen(error);
    
    va_list va;
    va_start(va, fmt);
    vsnprintf(&error[len], sizeof(error) - len, fmt, va);
    error[sizeof(error) - 1] = '\0';
    va_end(va);

    ++(*com_errorPrintsCount);
    LSP_LogStringEvenIfControllerIsInactive(error);
    Com_PrintMessage_t(channel, error, 3);

    int* cls_uiStarted = (int*)(0x0A7FFA0);
    if (*cls_uiStarted)
    {
        if (!*com_fixedConsolePosition)
        {
            memory::call<void()>(0x44A430); // CL_ConsoleFixPosition
        }
    }

}

// DONE : Inlined
int I_strlen(const char* s)
{
    const char *end;

    for (end = s; *end; ++end)
        ;
    return (s - end);
}

//DONE : 0x4EF820
char I_CleanChar(char c)
{
    if (static_cast<unsigned char>(c) == 0x92)
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

//DONE : 0x4A9DB0
const char* I_stristr(const char* s0, const char* substr)
{
    return memory::call<const char*(const char*, const char*)>(0x4A9DB0)(s0, substr);
}

//DONE : 0x4785B0
char* va(const char* format, ...)
{
    va_list ap;

    va_info_t* info = (va_info_t*)Sys_GetValue(THREAD_VALUE_VA);
    int index = info->index;

    info->index = (info->index + 1) % 2;
    char* buf = info->va_string[index];

    va_start(ap, format);
    int len = vsnprintf(buf, 1024, format, ap);
    va_end(ap);

    buf[1023] = '\0';
    if (len < 0 || len >= 1024)
    {
        //Com_Error(ERR_DROP, (char*)&byte_70924C);
    }

    return buf;
}

//DONE : inlined
static ParseThreadInfo* g_parse = reinterpret_cast<ParseThreadInfo*>(0x6466628);
ParseThreadInfo* Com_GetParseThreadInfo()
{
    if (Sys_IsMainThread())
    {
        return g_parse;
    }
    else if (Sys_IsRenderThread())
    {
        return &g_parse[1];
    }
    else if (Sys_IsServerThread())
    {
        return &g_parse[2];
    }
    else if (Sys_IsDatabaseThread())
    {
        return &g_parse[3];
    }
    else
    {
        return 0;
    }
}

//DONE : inlined
void Com_InitParseInfo(parseInfo_t* pi)
{
    pi->lines = 1;
    pi->spaceDelimited = 1;
    pi->ungetToken = 0;
    pi->keepStringQuotes = 0;
    pi->csv = 0;
    pi->negativeNumbers = 0;
    pi->backup_lines = 0;
    pi->backup_text = 0;
    pi->errorPrefix = *(const char**)0x6FAC0D;
    pi->warningPrefix = *(const char**)0x6FAC0D;
}

//DONE : 0x4AAB80
void Com_BeginParseSession(const char* filename)
{
    parseInfo_t* pi;
    ParseThreadInfo* parse = Com_GetParseThreadInfo();
    std::int32_t v2 = 0;
    const char** p_parseFile;

    if (parse->parseInfoNum == 15)
    {
        Com_Printf(23, "Already parsing:\n");

        if (parse->parseInfoNum > 0)
        {
            p_parseFile = &parse->parseInfo[0].parseFile;

            do
            {
                Com_Printf(23, "%i. %s\n", v2++, *p_parseFile);
                p_parseFile += 264;
            }
            while (v2 < parse->parseInfoNum);
        }
        //com_error
        memory::call<void(std::int32_t, char* Format, const char*)>(0x4B22D0)(0, (char*)0x730450, filename);
    }

    pi = &parse->parseInfo[++parse->parseInfoNum];
    Com_InitParseInfo(pi);
    pi->parseFile = filename;
}

//DONE : 0x4B80B0
void Com_EndParseSession()
{
    ParseThreadInfo* parse = Com_GetParseThreadInfo();

    if (!parse->parseInfoNum)
    {
        //com_error
        memory::call<void(std::int32_t, char* Format)>(0x4B22D0)(0, (char*)0x72C7C0);
    }

    --parse->parseInfoNum;
}

//DONE 0x64A190
parseInfo_t* Com_ParseCSV(const char** data_p, std::int32_t allowLineBreaks)
{
    //T5 code, used for readability, *should* act the same as IW4
    //keyword: should
    ParseThreadInfo* parse = Com_GetParseThreadInfo();
    parseInfo_t* pi = &parse->parseInfo[parse->parseInfoNum];
    const char* data = *data_p;
    std::uint32_t len = 0;
    pi->token[0] = 0;

    const char* v3;

    const char* dataa; //lol 3arc

    if (allowLineBreaks)
    {
        while (*data == 13 || *data == 10)
        {
            ++data;
        }
    }
    else if (*data == 13 || *data == 10)
    {
        return pi;
    }

    parse->prevTokenPos = parse->tokenPos;
    parse->tokenPos = data;

    while (*data && *data != 44 && *data != 10)
    {
        if (!data == 13)
        {
            ++data;
        }
        else if (*data == 34)
        {
            for (dataa = data + 1; ; dataa += 2)
            {
                while (*dataa != 34)
                {
                    if (len < 1023)
                    {
                        pi->token[len++] = *dataa;
                    }
                    ++dataa;
                }

                if (dataa[1] != 34)
                {
                    break;
                }

                if (len < 1023)
                {
                    pi->token[len++] = 34;
                }
            }
            data = dataa + 1;
        }
        else
        {
            if (len < 1023)
            {
                pi->token[len++] = *data;
            }
            ++data;
        }
    }

    if (*data && *data != 10)
    {
        ++data;
    }

    if (!data || len)
    {
        v3 = data;
    }
    else
    {
        v3 = 0;
    }

    *data_p = v3;
    pi->token[len] = 0;
    return pi;
}

//DONE 0x64A100
const char* SkipWhitespace(const char* data, std::int32_t* newLines)
{
    ParseThreadInfo* parse = Com_GetParseThreadInfo();
    parseInfo_t* pi = &parse->parseInfo[parse->parseInfoNum];

    std::int32_t v5 = *data;

    if (v5 > 32)
    {
        return data;
    }

    while (v5)
    {
        if (v5 == 10)
        {
            ++pi->lines;
            *newLines = 1;
        }
        v5 = *++data;

        if (v5 > 32)
        {
            return data;
        }
    }

    return 0;
}

//DONE : 0x64A2D0
static const char** puncuation = reinterpret_cast<const char**>(0x79D208);
parseInfo_t* Com_ParseExt(const char** data_p, std::int32_t allowLineBreaks)
{
    //T5 code, used for readability, *should* act the same as IW4
    //keyword: should

    ParseThreadInfo* parse = Com_GetParseThreadInfo();
    parseInfo_t* pi = &parse->parseInfo[parse->parseInfoNum];

    const char* data = *data_p;
    std::int32_t len = 0;
    std::int32_t newLines, j;

    char c, ca, cb, cc;

    pi->token[0] = 0;
    if (!data)
    {
        *data_p = 0;
        return pi;
    }

    pi->backup_lines = pi->lines;
    pi->backup_text = *data_p;

    if (pi->csv)
    {
        return Com_ParseCSV(data_p, allowLineBreaks);
    }

    while (1)
    {
        while (1)
        {
            data = SkipWhitespace(data, &newLines);

            if (!data)
            {
                *data_p = 0;
                return pi;
            }

            if (newLines > 0 && !allowLineBreaks)
            {
                return pi;
            }
            pi->lines += newLines;
            c = *data;

            if (*data != 47 || data[1] != 47)
            {
                break;
            }
            while (*data && *data != 10)
            {
                ++data;
            }
        }

        if (c != 47 || data[1] != 42)
        {
            break;
        }

        while (*data && (*data != 42 || data[1] != 47))
        {
            if (*data == 10)
            {
                ++pi->lines;
            }
            ++data;
        }

        if (*data)
        {
            data += 2;
        }
    }

    parse->prevTokenPos = parse->tokenPos;
    parse->tokenPos = data;

    if (c == 34)
    {
        if (pi->keepStringQuotes)
        {
            pi->token[len++] = 34;
        }
        ++data;

        while (1)
        {
            ca = *data++;
            if (ca == 92 && (*data == 34 || *data == 92))
            {
                ca = *data++;
            }
            else
            {
                if (ca == 34 || !ca)
                {
                    if (pi->keepStringQuotes)
                    {
                        pi->token[len++] = 34;
                    }
                    pi->token[len] = 0;
                    *data_p = data;
                    return pi;
                }
                if (*data == 10)
                {
                    ++pi->lines;
                }
            }

            if (len < 1023)
            {
                pi->token[len++] = ca;
            }
        }
    }

    if (c == -1)
    {
        ++data;
        while (1)
        {
            cb = *data++;
            if (cb == -1 || !cb)
            {
                break;
            }

            if (len < 1023)
            {
                pi->token[len++] = cb;
            }
        }

        pi->token[len] = 0;
        *data_p = data;
        return pi;
    }
    else if (pi->spaceDelimited)
    {
        do
        {
            if (len < 1023)
            {
                pi->token[len++] = c;
            }
            c = *++data;
        }
        while (*data > 32);

        if (len == 1024)
        {
            len = 0;
        }
        pi->token[len] = 0;
        *data_p = data;
        return pi;
    }
    else if (c >= 48 && c <= 57 || pi->negativeNumbers && c == 45 && data[1] >= 48 && data[1] <= 57 || c == 46 && data[1] >= 48 && data[1] <= 57)
    {
        do
        {
            if (len < 1023)
            {
                pi->token[len++] = c;
            }
            c = *++data;
        } while (*data >= 48 && c <= 57 || c == 46);

        if (c == 101 || c == 69)
        {
            if (len < 1023)
            {
                pi->token[len++] = c;
            }
            cc = *++data;

            if (*data == 45 || cc == 43)
            {
                if (len < 1023)
                {
                    pi->token[len++] = cc;
                }
                cc = *++data;
            }

            do
            {
                if (len < 1023)
                {
                    pi->token[len++] = cc;
                }
                cc = *++data;
            }
            while (*data >= 48 && cc <= 57);
        }

        if (len == 1024)
        {
            len = 0;
        }
        pi->token[len] = 0;
        *data_p = data;
        return pi;
    }
    else if (c >= 97 && c <= 122 || c >= 65 && c <= 90 || c == 95 || c == 47 || c == 92)
    {
        do
        {
            if (len < 1023)
            {
                pi->token[len++] = c;
            }
            c = *++data;
        }
        while (*data >= 97 && c <= 122 || c >= 65 && c <= 90 || c == 95 || c >= 48 && c <= 57);

        if (len == 1024)
        {
            len = 0;
        }
        pi->token[len] = 0;
        *data_p = data;
        return pi;
    }
    else
    {
        //this might be wrong
        for (const char** punc = puncuation; *punc; ++punc)
        {
            std::size_t l = strlen(*punc);
            for (j = 0; j < l && data[j] == (*punc)[j]; ++j)
            {
                ;
            }

            if (j == l)
            {
                memcpy((std::uint8_t*)pi, (std::uint8_t*)*punc, l);
                pi->token[l] = 0;
                data += l;
                *data_p = data;
                return pi;
            }
        }

        pi->token[0] = *data;
        pi->token[1] = 0;
        return pi;
    }
    
}

//DONE : 0x474D60
char* Com_Parse(const char** data_p)
{
    ParseThreadInfo* parse = Com_GetParseThreadInfo();
    const char* backupText;
    parseInfo_t* pi = &parse->parseInfo[parse->parseInfoNum];

    if (pi->ungetToken)
    {
        backupText = pi->backup_text;
        pi->ungetToken = 0;
        *data_p = backupText;
        pi->lines = pi->backup_lines;
    }
    //dont be surprised if this breaks
    return (char*)Com_ParseExt(data_p, 1);
}

//DONE : 0x48C550
static hunkUsed_t* hunk_low = reinterpret_cast<hunkUsed_t*>(0x63D97AC);
static hunkUsed_t* hunk_high = reinterpret_cast<hunkUsed_t*>(0x63D97A4);
static unsigned char* s_hunkData = reinterpret_cast<unsigned char*>(0x63E2638);
static std::size_t s_hunkTotal = *reinterpret_cast<std::size_t*>(0x63D978C);
void Com_TouchMemory()
{
    std::int32_t start = Sys_Milliseconds();
    std::int32_t sum = 0;

    for (std::int32_t i = 0; i < hunk_low->permanent >> 2; i += 64)
    {
        sum += *(long*)&s_hunkData[4 * i];
    }

    for (std::int32_t j = (std::int32_t)(s_hunkTotal - hunk_high->permanent) >> 2; j < hunk_high->permanent >> 2; j += 64)
    {
        sum += *(long*)&s_hunkData[4 * j];
    }

    std::int32_t end = Sys_Milliseconds();
    Com_Printf(16, "Com_TouchMemory: %i msec. Using sum: %d\n", end - start, sum);
}
