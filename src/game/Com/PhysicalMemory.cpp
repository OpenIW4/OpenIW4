#include "game/defs.hpp"

#include "PhysicalMemory.hpp"
#include "Com.hpp"

#include "../Sys/Sys.hpp"

#include "utils/memory/memory.hpp"

static PhysicalMemory* g_mem = reinterpret_cast<PhysicalMemory*>(0x64663F8);

//DONE : Inlined
void PMem_InitPhysicalMemory(PhysicalMemory* pmem, void* memory, unsigned int memorySize)
{
    memset(pmem, 0, sizeof(PhysicalMemory));
    pmem->buf = (char*)memory;
    pmem->prim[1].pos = memorySize;
}

//DONE : 0x64A020
void PMem_Init()
{
    void* result = VirtualAlloc(nullptr, 0x12C00000, MEM_RESERVE, PAGE_READWRITE);
    PMem_InitPhysicalMemory(g_mem, result, 0x12C00000);
}

//DONE : 0x47A6D0
uint8_t* PMem_AllocFromSource(size_t size, uint32_t alignment, uint32_t type, PMem_Source source)
{
    int pool;
    char* buf;

    if (source == PMEM_SOURCE_MOVIE)
    {
        pool = 0;
    }
    else
    {
        bool result = memory::call<bool()>(0x4984E0)();
        pool = result ? memory::call<int()>(0x4FB6C0)() : (type != 4);
    }

    uint32_t pos = g_mem->prim[pool].pos;
    uint32_t unk1 = alignment - 1;

    if (pool == 0)
    {
        pos += unk1 & ~unk1;
        unk1 = pos + size;

        if (0 < (int)(unk1 - g_mem->prim[1].pos))
        {
            Com_PrintError(0x10, "Need %i more bytes of ram for alloc to succeed\n", unk1 - g_mem->prim[1].pos);
            Sys_OutOfMemErrorInternal("c:\\trees\\build-iw4-pc\\iw4\\code_source\\src\\universal\\physicalmemory.cpp", 0x28A);
        }

        buf = g_mem->buf + pos;
        g_mem->prim[0].pos = unk1;
    }
    else
    {
        pos -= size & ~unk1;

        if (0 < (int)(g_mem->prim[0].pos - pos))
        {
            Com_PrintError(0x10, "Need %i more bytes of ram for alloc to succeed\n", g_mem->prim[0].pos - pos);
            Sys_OutOfMemErrorInternal("c:\\trees\\build-iw4-pc\\iw4\\code_source\\src\\universal\\physicalmemory.cpp", 0x28A);
        }

        unk1 = size + pos;
        buf = g_mem->buf + pos;
        g_mem->prim[pool].pos = pos;
    }

    VirtualAlloc(g_mem->buf + (pos & 0xFFFFF000), ((unk1 + 0xFFF) & 0xFFFFF000) - (pos & 0xFFFFF000), MEM_COMMIT, PAGE_READWRITE);
    return (uint8_t*)buf;
}

//DONE : 0x4FEE10
void PMem_DumpMemStat()
{
    unsigned int unk1 = 0;

    if (g_mem->prim[1].allocListCount != 0)
    {
        do
        {
            unsigned int pos = g_mem->prim[1].pos;
            if (unk1 != g_mem->prim[1].allocListCount - 1)
            {
                pos = g_mem->prim[1].allocList[unk1 + 1].pos;
            }

            Com_Printf(0x10, "%-18.18s %5.1f\n", g_mem->prim[1].allocList[unk1].name, (g_mem->prim[1].allocList[unk1].pos - pos) * 0.0000009536743);
            unk1++;
        } while (unk1 < g_mem->prim[1].allocListCount);
    }

    int unk2 = g_mem->prim[1].pos - g_mem->prim[0].pos + 2621440;
    Com_Printf(0x10, "free physical      %5.1f\n", unk2 * 0.0000009536743);

    unk1 = g_mem->prim[0].allocListCount;
    while (-1 < (int)(unk1 - 1))
    {
        Com_Printf(0x10, "%-18.18s %5.1f\n", g_mem->prim[0].allocList[unk1 - 1].name);
        unk1--;
    }

    Com_Printf(0x10, "------------------------\n");
}
