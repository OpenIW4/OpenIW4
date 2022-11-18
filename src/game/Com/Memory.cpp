#include "game/defs.hpp"

#include "Memory.hpp"
#include "Com.hpp"

#include "../Sys/Sys.hpp"
#include "../Script/StringList.hpp"
#include "../Render/Render.hpp"

#include <utils/memory/memory.hpp>

static std::uint8_t* g_largeLocalBuf;
static int g_largeLocalPos;
static int g_largeLocalRightPos;

static int g_maxLargeLocalPos;
static int g_minLargeLocalRightPos;

#define LARGE_LOCAL_BUF_SIZE 0x180000
#define PAGE_SIZE 0x1000

//DONE : 0x4A62A0
void LargeLocalInit()
{
    g_largeLocalBuf = static_cast<std::uint8_t*>(VirtualAlloc(nullptr, LARGE_LOCAL_BUF_SIZE, MEM_RESERVE, PAGE_READWRITE));

    g_maxLargeLocalPos = 0;
    g_minLargeLocalRightPos = LARGE_LOCAL_BUF_SIZE;

    LargeLocalClear();
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
    auto context = R_PopRemoteScreenUpdate();
    R_SyncRenderThread();

    auto pos = (g_largeLocalPos + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
    auto rightPos = (g_maxLargeLocalPos + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);

    if (pos != rightPos)
    {
        VirtualFree(&g_largeLocalBuf[pos], rightPos - pos, MEM_DECOMMIT);
    }

    g_maxLargeLocalPos = g_largeLocalPos;
    rightPos = g_minLargeLocalRightPos & ~(PAGE_SIZE - 1);

    if (rightPos != (g_largeLocalRightPos & ~(PAGE_SIZE - 1)))
    {
        VirtualFree(&g_largeLocalBuf[rightPos], (g_largeLocalRightPos & ~(PAGE_SIZE - 1)) - rightPos, MEM_DECOMMIT);
    }

    g_minLargeLocalRightPos = g_largeLocalRightPos;
    R_ReleaseThreadOwnership();
    R_PushRemoteScreenUpdate(context);
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
    auto address = g_largeLocalBuf[g_maxLargeLocalPos + (PAGE_SIZE - 1)] & ~(PAGE_SIZE - 1);

    if (g_maxLargeLocalPos < g_largeLocalPos)
    {
        g_maxLargeLocalPos = g_largeLocalPos;
    }

    auto dwSize = (g_largeLocalBuf[g_maxLargeLocalPos + (PAGE_SIZE - 1)]  & ~(PAGE_SIZE - 1)) - address;
    if (dwSize != 0)
    {
        auto result = VirtualAlloc(LPVOID(address), dwSize, MEM_COMMIT, PAGE_READWRITE);
        if (result == nullptr)
        {
            Sys_OutOfMemError();
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

    auto unk1 = g_largeLocalBuf[g_minLargeLocalRightPos] & ~(PAGE_SIZE - 1);
    auto dwSize = (pos & ~(PAGE_SIZE - 1)) - unk1;

    if (dwSize != 0)
    {
        auto result = VirtualAlloc(LPVOID(unk1), dwSize, MEM_COMMIT, PAGE_READWRITE);
        if (result == nullptr)
        {
            Sys_OutOfMemError();
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
    g_largeLocalRightPos = LARGE_LOCAL_BUF_SIZE;
    LargeLocalReset();
}

//DONE: Inlined
void* LargeLocalGetBuf(int startPos, int size)
{
    if (Sys_IsMainThread())
    {
        return &g_largeLocalBuf[startPos];
    }

    const auto startIndex = startPos - size;
    return &g_largeLocalBuf[startIndex];
}

//DONE : 0x4D1110
void* LargeLocal::GetBuf()
{
    return LargeLocalGetBuf(this->startPos, this->size);
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
    sizeParam = ((sizeParam + (128 - 1)) & ~(128 - 1));

    if (Sys_IsMainThread())
    {
        this->startPos = LargeLocalBegin(sizeParam);
    }
    else
    {
        this->startPos = LargeLocalBeginRight(sizeParam);
    }

    this->size = sizeParam;
}

//DONE : 0x44BD60
LargeLocal::~LargeLocal()
{
    if (this->size)
    {
        this->PopBuf();
    }
}

// Z Section

//DONE: Inlined
int Z_TryVirtualCommitInternal(void* ptr, int size)
{
    if (VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE))
    {
        return 1;
    }

    return 0;
}

//DONE: Inlined
void Z_VirtualCommit(void* ptr, int size)
{
    if (!Z_TryVirtualCommitInternal(ptr, size))
    {
        Sys_OutOfMemError();
    }
}

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
        Sys_OutOfMemError();
        return NULL;
    }

    Com_Memset(buf, 0, size);
    return buf;
}

// Hunk section

//THUNK : 0x6440D0
void Hunk_ClearData()
{
    memory::call<void()>(0x6440D0);
}

//THUNK : 0x492F20
void* Hunk_AllocAlign(int size, int alignment)
{
    return memory::call<void*(int, int)>(0x492F20)(size, alignment);
}

//DONE : 0x4B16E0
void* Hunk_Alloc(int size)
{
    return Hunk_AllocAlign(size, 32);
}

//THUNK : 0x4FF4D0
void* Hunk_AllocateTempMemory(int size)
{
    return memory::call<void*(int)>(0x4FF4D0)(size);
}

//THUNK : 0x475B30
void* Hunk_AllocateTempMemoryHigh(int size)
{
    return memory::call<void* (int)>(0x475B30)(size);
}

//THUNK : 0x43B100
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
    char* result = static_cast<char*>(Hunk_UserAlloc(user, len + 1, 1));
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

//DONE : 0x430E90
HunkUser* Hunk_UserCreate(int maxSize, const char* name, bool fixed, int type)
{
    HunkUser* buffer = static_cast<HunkUser*>(Z_VirtualReserve(maxSize));
    Z_VirtualCommit(buffer, 0x20);

    buffer->end = (buffer->buf[0] + maxSize + -0x20);
    buffer->pos = buffer->buf[0];
    buffer->maxSize = maxSize;
    buffer->name = name;
    buffer->current = buffer;
    buffer->fixed = fixed;
    buffer->type = type;
    return buffer;
}
