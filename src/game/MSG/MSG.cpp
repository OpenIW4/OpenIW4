#include "MSG.hpp"
#include "../Com/Com.hpp"
#include "../Sys/Sys.hpp"

#include <memory/memory.hpp>

//TODO : 0x48B180
void MSG_InitHuffman()
{
    //new project for when im awake
    *(std::int32_t*)0x1CB9EB8 = 1; //msgInit
    //Huff_Init(blah); //todo
    std::int32_t time = Sys_Milliseconds();
    //Huff_BuildFromData(cat, dog); //todo part 2
    std::int32_t time2 = Sys_Milliseconds();
    Com_Printf(25, "Huffman took %d milliseconds\n", time2 - time);
}

//TODO : 0x45FCA0
void MSG_Init(msg_t* buffer, char* data, size_t size)
{
	if (!*(bool*)0x1CB9EB8 /*msgInit*/)
	{
		memory::call<void()>(0x48B180)(); //MSG_InitHuffman
	}

	buffer->overflowed = false;
	buffer->curSize = 0;
	buffer->readCount = 0;
	buffer->bit = 0;
	buffer->lastEntityRef = 0;
	buffer->data = data;
	buffer->maxSize = size;
	buffer->readOnly = false;
	buffer->splitData = 0;
	buffer->splitSize = 0;
}

//DONE : 0x48C520
void MSG_WriteByte(msg_t* msg, std::int8_t c)
{
	if (msg->curSize >= msg->maxSize)
	{
		msg->overflowed = true;
	}
	else
	{
		msg->data[msg->curSize] = c;
		++msg->curSize;
	}
}

//DONE : 0x44E1F0
void MSG_WriteBit0(msg_t* msg)
{
	if ((msg->bit & 7) == 0)
	{
		if (msg->curSize >= msg->maxSize)
		{
			msg->overflowed = true;
			return;
		}

		msg->bit = 8 * msg->curSize;
		msg->data[msg->curSize] = 0;
		++msg->curSize;
	}

	++msg->bit;
}

//DONE : 0x45A600
void MSG_WriteBit1(msg_t* msg)
{
	if (msg->bit & 7)
	{
		if (msg->curSize >= msg->maxSize)
		{
			msg->overflowed = true;
			return;
		}

		msg->bit = 8 * msg->curSize;
		msg->data[msg->curSize] = 0;
		++msg->curSize;
	}

	char* v3 = &msg->data[msg->bit >> 3];
	*v3 |= 1 << msg->bit & 7;
	++msg->bit;
}

//TODO : 0x441230
void MSG_WriteInt64(msg_t* msg, std::int32_t a2, std::int32_t a3)
{
	if (msg->curSize + 8 > msg->maxSize)
	{
		msg->overflowed = true;
	}
	else
	{
		*(std::uint64_t*)&msg->data[msg->curSize] = memory::call<std::int32_t(std::int32_t, std::int32_t)>(0x4A94F0)(a2, a3); //this might be LittleLong64
		msg->curSize += 8;
	}
}

//DONE : 0x503B90
void MSG_WriteShort(msg_t* msg, std::int32_t c)
{
	if (msg->curSize + 2 > msg->maxSize)
	{
		msg->overflowed = true;
	}
	else
	{
		*(std::uint16_t*)&msg->data[msg->curSize] = c;
		msg->curSize += 2;
	}
}

//DONE : 0x41CA20
void MSG_WriteLong(msg_t* msg, std::int32_t c)
{
	if (msg->curSize + 4 > msg->maxSize)
	{
		msg->overflowed = true;
	}
	else
	{
		*(unsigned long*)&msg->data[msg->curSize] = c;
		msg->curSize += 4;
	}
}

//DONE : 0x4F4120
void MSG_WriteData(msg_t* msg, std::int32_t* a2, std::int32_t a3)
{
	std::int32_t v1 = msg->curSize + a3;

	if (v1 > msg->maxSize)
	{
		msg->overflowed = true;
	}
	else
	{
		Com_Memcpy((void*)&msg->data[msg->curSize], a2, a3);
		msg->curSize = v1;
	}
}
