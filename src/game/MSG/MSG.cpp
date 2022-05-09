#include "MSG.hpp"
#include "../Com/Com.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x48C520
void MSG_WriteByte(msg_t* a1, std::int8_t a2)
{
	if (a1->curSize >= a1->maxSize)
	{
		a1->overflowed = true;
	}
	else
	{
		a1->data[a1->curSize] = a2;
		a1->curSize++;
	}
}

//DONE : 0x44E1F0
void MSG_WriteBit0(msg_t* a1)
{
	if ((a1->bit & 7) == 0)
	{
		if (a1->curSize >= a1->maxSize)
		{
			a1->overflowed = true;
			return;
		}

		a1->bit = 8 * a1->curSize;
		a1->data[a1->curSize] = 0;
		a1->curSize++;
	}

	a1->bit++;
}

//DONE : 0x45A600
void MSG_WriteBit1(msg_t* a1)
{
	if (a1->bit & 7)
	{
		if (a1->curSize >= a1->maxSize)
		{
			a1->overflowed = true;
			return;
		}

		a1->bit = 8 * a1->curSize;
		a1->data[a1->curSize] = 0;
		a1->curSize++;
	}

	char* v3 = &a1->data[a1->bit >> 3];
	*v3 |= 1 << a1->bit & 7;
	a1->bit++;
}

//TODO : 0x441230
void MSG_WriteInt64(msg_t* a1, std::int32_t a2, std::int32_t a3)
{
	if (a1->curSize + 8 > a1->maxSize)
	{
		a1->overflowed = true;
	}
	else
	{
		*(std::uint64_t*)&a1->data[a1->curSize] = memory::call<std::int32_t(std::int32_t, std::int32_t)>(0x4A94F0)(a2, a3); //this might be LittleLong64
		a1->curSize += 8;
	}
}

//DONE : 0x503B90
void MSG_WriteShort(msg_t* a1, std::uint16_t a2)
{
	if (a1->curSize + 2 > a1->maxSize)
	{
		a1->overflowed = true;
	}
	else
	{
		*(std::uint16_t*)&a1->data[a1->curSize] = a2;
		a1->curSize += 2;
	}
}

//DONE : 0x41CA20
void MSG_WriteLong(msg_t* a1, std::int32_t a2)
{
	if (a1->curSize + 4 > a1->maxSize)
	{
		a1->overflowed = true;
	}
	else
	{
		*(unsigned long*)&a1->data[a1->curSize] = a2;
		a1->curSize += 4;
	}
}

//DONE : 0x4F4120
void MSG_WriteData(msg_t* a1, std::int32_t* a2, std::int32_t a3)
{
	std::int32_t v1 = a1->curSize + a3;

	if (v1 > a1->maxSize)
	{
		a1->overflowed = true;
	}
	else
	{
		Com_Memcpy((void*)&a1->data[a1->curSize], a2, a3);
		a1->curSize = v1;
	}
}