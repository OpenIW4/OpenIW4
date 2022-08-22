#pragma once

#include "../stdafx.hpp"

class LargeLocal
{
public:
	LargeLocal(int sizeParam);
	~LargeLocal();

	void* GetBuf();

	void PopBuf();

private:
	int startPos;
	int size;
};

void LargeLocalInit();
void LargeLocalEnd(int startPos);
void LargeLocalEndRight(int startPos);
void LargeLocalReset();
void LargeLocalResetToMark(int markPos);
int LargeLocalBegin(int size);
int LargeLocalBeginRight(int size);
int LargeLocalGetMark();
void LargeLocalClear();
void* LargeLocalGetBuf(int startPos, int size);

// Z (Name is from Quake III)
void Z_VirtualFree(void* ptr, int type);
void* Z_VirtualReserve(int size);
void* Z_TryVirtualAllocInternal(int size);
void Z_VirtualDecommitInternal(void* ptr, int size);
void Z_VirtualFreeInternal(void* ptr);
void* Z_MallocInternal(int size);

// Hunk (Name is from Quake III)
void Hunk_ClearData();
void* Hunk_AllocAlign(int size, int alignment);
void* Hunk_Alloc(int size);
void* Hunk_AllocateTempMemory(int size);
void* Hunk_AllocateTempMemoryHigh(int size);
void* Hunk_AllocLowAlign(int size, int alignment);
void Hunk_Clear();
void Hunk_ClearTempMemory();
void Hunk_ClearTempMemoryHigh();
void Hunk_ClearToMarkLow(int mark);
void* Hunk_UserAlloc(HunkUser* user, int size, int alignment);
char* Hunk_CopyString(HunkUser* user, const char* in);
void Hunk_FreeTempMemory(void* buf);
void Hunk_UserDestroy(HunkUser* user);
void Hunk_ShutdownDebugMemory();
void Hunk_UserReset(HunkUser* user);
