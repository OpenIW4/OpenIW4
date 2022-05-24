#include "game/defs.hpp"

#include "Memory.hpp"
#include "Com.hpp"

#include "../Sys/Sys.hpp"
#include "../Script/StringList.hpp"

#include "utils/memory/memory.hpp"

static std::uint8_t* g_largeLocalBuf;
static int g_largeLocalPos;
static int g_largeLocalRightPos;

static int g_maxLargeLocalPos;
static int g_minLargeLocalRightPos;

//DONE : 0x4A62A0
void LargeLocalInit()
{
    g_largeLocalBuf = (std::uint8_t*)VirtualAlloc(nullptr, 0x180000, MEM_RESERVE, PAGE_READWRITE);

    g_maxLargeLocalPos = 0;
    g_minLargeLocalRightPos = 0x180000;

    g_largeLocalPos = 0;
    g_largeLocalRightPos = 0x180000;

    LargeLocalReset();
}

//DONE : Inlined
void LargeLocalEnd(int startPos)
{
    g_largeLocalPos = startPos;
}

//DONE : Inlined
void LargeLocalEndRight(int startPos)
{
    g_largeLocalRightPos = startPos;
}

//DONE : 0x430630
void LargeLocalReset()
{
    auto context = memory::call<int()>(0x50B0D0)(); // R_PopRemoteScreenUpdate
    memory::call<void()>(0x5091B0)(); //  R_SyncRenderThread

    auto unk1 = (g_largeLocalPos + 0xFFF) & 0xFFFFF000;
    auto unk2 = (g_maxLargeLocalPos + 0xFFF) & 0xFFFFF000;

    if (unk1 != unk2)
    {
        VirtualFree(&g_largeLocalBuf[unk1], unk2 - unk1, MEM_DECOMMIT);
    }

    g_maxLargeLocalPos = g_largeLocalPos;
    unk2 = g_minLargeLocalRightPos & 0xFFFFF000;

    if (unk2 != (g_largeLocalRightPos & 0xFFFFF000))
    {
        VirtualFree(&g_largeLocalBuf[unk2], (g_largeLocalRightPos & 0xFFFFF000) - unk2, MEM_DECOMMIT);
    }

    g_minLargeLocalRightPos = g_largeLocalRightPos;
    memory::call<void()>(0x5091E0)(); // R_ReleaseThreadOwnership
    memory::call<void(int)>(0x50B070)(context); // R_PushRemoteScreenUpdate
}

//DONE : 0x4458F0
void LargeLocalResetToMark(int markPos)
{
    if (Sys_IsMainThread())
    {
        LargeLocalEnd(markPos);
    }
    else
    {
        LargeLocalEndRight(markPos);
    }
}

//DONE : 0x4127A0
int LargeLocalBegin(int size)
{
    g_largeLocalPos += size;
    auto address = g_largeLocalBuf[g_maxLargeLocalPos + 0xFFF] & 0xFFFFF000;

    if (g_maxLargeLocalPos < g_largeLocalPos)
    {
        g_maxLargeLocalPos = g_largeLocalPos;
    }

    auto dwSize = (g_largeLocalBuf[g_maxLargeLocalPos + 0xFFF]  & 0xFFFFF000) - address;
    if (dwSize != 0)
    {
        auto result = VirtualAlloc((LPVOID)address, dwSize, MEM_COMMIT, PAGE_READWRITE);
        if (result == nullptr)
        {
            Sys_OutOfMemErrorInternal("c:\\trees\\build-iw4-pc\\iw4\\code_source\\src\\universal\\com_memory.cpp", 0x2D9);
        }
    }

    return g_largeLocalPos;
}

//DONE : 0x644140
int LargeLocalBeginRight(int size)
{
    g_largeLocalRightPos -= size;
    auto pos = g_largeLocalBuf[g_minLargeLocalRightPos];

    if (g_largeLocalRightPos < g_minLargeLocalRightPos)
    {
        g_minLargeLocalRightPos = g_largeLocalRightPos;
    }

    auto unk1 = g_largeLocalBuf[g_minLargeLocalRightPos] & 0xFFFFF000;
    auto dwSize = (pos & 0xFFFFF000) - unk1;

    if (dwSize != 0)
    {
        auto result = VirtualAlloc((LPVOID)unk1, dwSize, MEM_COMMIT, PAGE_READWRITE);
        if (result == nullptr)
        {
            Sys_OutOfMemErrorInternal("c:\\trees\\build-iw4-pc\\iw4\\code_source\\src\\universal\\com_memory.cpp", 0x2D9);
        }
    }

    return g_largeLocalRightPos;
}

//DONE : 0x40A430
int LargeLocalGetMark()
{
    if (Sys_IsMainThread())
    {
        return g_largeLocalPos;
    }

    return g_largeLocalRightPos;
}

//DONE : 0x4C12E0
void LargeLocalClear()
{
    g_largeLocalPos = 0;
    g_largeLocalRightPos = 0x180000;
    LargeLocalReset();
}

//DONE : 0x4D1110
std::uint8_t* LargeLocal::LargeLocalGetBuf(int startPos, int size)
{
    if (!Sys_IsMainThread())
    {
        startPos -= size;
    }

    return &g_largeLocalBuf[startPos];
}

//DONE : Inlined
void LargeLocal::PopBuf()
{
    if (Sys_IsMainThread())
    {
        LargeLocalEnd(this->startPos);
    }
    else
    {
        LargeLocalEndRight(this->startPos);
    }

    this->size = 0;
}

//DONE : 0x457E50
LargeLocal::LargeLocal(int sizeParam)
{
    const auto size_ = (sizeParam + 127) & 0xFFFFFF80;

    if (Sys_IsMainThread())
    {
        this->startPos = LargeLocalBegin(static_cast<int>(size_));
    }
    else
    {
        this->startPos = LargeLocalBeginRight(static_cast<int>(size_));
    }

    this->size = static_cast<int>(size_);
}

//DONE : 0x44BD60
LargeLocal::~LargeLocal()
{
    if (this->size != 0)
    {
        this->PopBuf();
    }
}

// Z Section

//DONE : Inlined
void Z_VirtualFree(void* ptr, int type)
{
    VirtualFree(ptr, type, MEM_RESET);
}

//DONE : Inlined
void* Z_VirtualReserve(int size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE);
}

//DONE : 0x47AF50
void* Z_TryVirtualAllocInternal(int size)
{
    void* reserved = Z_VirtualReserve(size);
    void* result = VirtualAlloc(reserved, size, MEM_COMMIT, PAGE_READWRITE);

    if (result == NULL)
    {
        Z_VirtualFree(reserved, 0);
        return NULL;
    }

    return reserved;
}

//DONE : 0x48B560
void Z_VirtualDecommitInternal(void* ptr, int size)
{
    VirtualFree(ptr, size, MEM_DECOMMIT);
}

//DONE : 0x496D60
void Z_VirtualFreeInternal(void* ptr)
{
    VirtualFree(ptr, 0, MEM_RELEASE);
}

//DONE : 0x4F3680
void* Z_MallocInternal(int size)
{
    void* buf = malloc(size);

    if (buf == NULL)
    {
        Com_PrintError(0x10, "Failed to Z_Malloc %i bytes\n", size);
        Sys_OutOfMemErrorInternal("c:\\trees\\build-iw4-pc\\iw4\\code_source\\src\\universal\\com_memory.cpp", 0x3B5);
        return NULL;
    }

    Com_Memset(buf, 0, size);
    return buf;
}

// Hunk section

static hunkUsed_t* hunk_low = reinterpret_cast<hunkUsed_t*>(0x63D97AC);
static hunkUsed_t* hunk_high = reinterpret_cast<hunkUsed_t*>(0x63D97A4);
static uint8_t** s_hunkData = reinterpret_cast<uint8_t**>(0x63E2638);
static int* s_hunkTotal = reinterpret_cast<int*>(0x63D978C);
static HunkUser** g_debugUser = reinterpret_cast<HunkUser**>(0x201A45C);

//DONE : 0x6440D0
void Hunk_ClearData()
{
    memory::call<void()>(0x6440D0);
}

//DONE : 0x492F20
void* Hunk_AllocAlign(int size, int alignment)
{
    return memory::call<void*(int, int)>(0x492F20)(size, alignment);
}

//DONE : 0x4B16E0
void* Hunk_Alloc(int size)
{
    return Hunk_AllocAlign(size, 32);
}

//DONE : 0x4FF4D0
void* Hunk_AllocateTempMemory(int size)
{
    return memory::call<void*(int)>(0x4FF4D0)(size);
}

//DONE : 0x475B30
void* Hunk_AllocateTempMemoryHigh(int size)
{
    return memory::call<void* (int)>(0x475B30)(size);
}

//DONE : 0x43B100
void* Hunk_AllocLowAlign(int size, int alignment)
{
    return memory::call<void* (int, int)>(0x43B100)(size, alignment);
}

//DONE : 0x4617E0
void Hunk_Clear()
{
    hunk_low->permanent = 0;
    hunk_low->temp = 0;
    hunk_high->permanent = 0;
    hunk_high->temp = 0;

    Hunk_ClearData();
    VirtualFree(*s_hunkData, *s_hunkTotal, MEM_DECOMMIT);
}

//DONE : 0x4A0460
void Hunk_ClearTempMemory()
{
    memory::call<void()>(0x4A0460);
}

//DONE : 0x4AB3A0
void Hunk_ClearTempMemoryHigh()
{
    memory::call<void()>(0x4AB3A0);
}

//DONE : 0x432AA0
void Hunk_ClearToMarkLow(int mark)
{
    memory::call<void(int)>(0x432AA0)(mark);
}

//DONE : 0x45D1C0
void* Hunk_UserAlloc(HunkUser* user, int size, int alignment)
{
    return memory::call<void*(HunkUser*, int, int)>(0x45D1C0)(user, size, alignment);
}

//DONE : 0x4F0A60
char* Hunk_CopyString(HunkUser* user, const char* in)
{
    int len = I_strlen(in);
    char* result = (char*)Hunk_UserAlloc(user, len + 1, 1);
    memcpy(result, in, len + 1);

    return result;
}

//DONE : 0x49AAC0
void Hunk_FreeTempMemory(void* buf)
{
    memory::call<void(void*)>(0x49AAC0)(buf);
}

//DONE : 0x435910
void Hunk_UserDestroy(HunkUser* user)
{
    HunkUser* current = user->next;

    while (current != NULL)
    {
        HunkUser* next = current->next;
        Z_VirtualFree(current, current->type);
        current = next;
    }

    Z_VirtualFree(user, user->type);
}

//DONE : 0x434E30
void Hunk_ShutdownDebugMemory()
{
    Hunk_UserDestroy(*g_debugUser);
    *g_debugUser = NULL;
}

//DONE : 0x4D55D0
void Hunk_UserReset(HunkUser* user)
{
    memory::call<void(HunkUser*)>(0x4D55D0)(user);
}

const char* CopyString(const char* in)
{
    uint32_t stringValue = SL_GetString_(in, 0, 0x16);
    return SL_ConvertToString(stringValue);
}
