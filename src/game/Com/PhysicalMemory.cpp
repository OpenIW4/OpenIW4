#include "game/defs.hpp"
#include "PhysicalMemory.hpp"
#include "Com.hpp"

#include "utils/memory/memory.hpp"

static PhysicalMemory* pmem = reinterpret_cast<PhysicalMemory*>(0x64663F8);

//DONE : 0x64A020
void PMem_Init()
{
    char* result = (char*)VirtualAlloc(nullptr, 0x12C00000, MEM_RESERVE, PAGE_READWRITE);
    memset(pmem, 0, sizeof(PhysicalMemory));
    pmem->buf = result;
    pmem->prim[1].pos = 0x12C00000;
}

//DONE : 0x47A6D0
uint8_t* PMem_AllocFromSource(size_t size, uint32_t alignment, uint32_t type, PMem_Source source)
{
    return memory::call<uint8_t*(size_t, uint32_t, uint32_t, PMem_Source)>(0x47A6D0)(size, alignment, type, source);
}


//DONE : 0x4FEE10
void PMem_DumpMemStat()
{
    unsigned int unk1 = 0;

    if (pmem->prim[1].allocListCount != 0)
    {
        do
        {
            unsigned int pos = pmem->prim[1].pos;
            if (unk1 != pmem->prim[1].allocListCount - 1)
            {
                pos = pmem->prim[1].allocList[unk1 + 1].pos;
            }

            Com_Printf(0x10, "%-18.18s %5.1f\n", pmem->prim[1].allocList[unk1].name, (pmem->prim[1].allocList[unk1].pos - pos) * 0.0000009536743);
            unk1++;
        } while (unk1 < pmem->prim[1].allocListCount);
    }

    int unk2 = pmem->prim[1].pos - pmem->prim[0].pos + 2621440;
    Com_Printf(0x10, "free physical      %5.1f\n", unk2 * 0.0000009536743);

    unk1 = pmem->prim[0].allocListCount;
    while (-1 < (int)(unk1 - 1))
    {
        Com_Printf(0x10, "%-18.18s %5.1f\n", pmem->prim[0].allocList[unk1 - 1].name);
        unk1--;
    }

    Com_Printf(0x10, "------------------------\n");
}
