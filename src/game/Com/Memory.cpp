#include "Memory.hpp"
#include "game/Sys/Sys.hpp"

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
