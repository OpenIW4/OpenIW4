#include "MSG.hpp"
#include "../Com/Com.hpp"
#include "../Sys/Sys.hpp"
#include "../Huffman/Huffman.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x48B180
static huffman_t* huff = reinterpret_cast<huffman_t*>(0x1CB9EC0);
static std::int32_t* msg = reinterpret_cast<std::int32_t*>(0x1CB9EB8);
void MSG_InitHuffman()
{
    *(std::int32_t*)0x1CB9EB8 = 1; //msgInit
    Huff_Init(huff);
    std::int32_t time = Sys_Milliseconds();
    Huff_BuildFromData(&huff->compressDecompress, msg);
    std::int32_t time2 = Sys_Milliseconds();
    Com_Printf(25, "Huffman took %d milliseconds\n", time2 - time);
}

//DONE : 0x45FCA0
void MSG_Init(msg_t* buffer, char* data, std::size_t size)
{
	if (!*(bool*)0x1CB9EB8 /*msgInit*/)
	{
		MSG_InitHuffman();
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

//DONE : 0x4F56D0
void MSG_Discard(msg_t* msg)
{
    msg->overflowed = true;
    msg->curSize = msg->readCount;
    msg->splitSize = 0;
}

//DONE : 0x4458E0
void MSG_ClearLastReferencedEntity(msg_t* msg)
{
    msg->lastEntityRef = -1;
}

//DONE : Possibly inlined
std::int32_t MSG_GetByte(msg_t* msg, std::int32_t where)
{
    if (where < msg->curSize)
    {
        return msg->data[where];
    }

    return msg->splitData[where - msg->curSize];
}

//DONE : Possibly inlined
std::int32_t MSG_ReadByte(msg_t* msg)
{
    if (msg->readCount >= msg->splitSize + msg->curSize)
    {
        msg->overflowed = true;
        return -1;
    }
    else
    {
        std::int32_t c = MSG_GetByte(msg, msg->readCount);
        ++msg->readCount;
        return c;
    }
}

//DONE : 0x47A530
char* MSG_ReadString(msg_t* msg, char* string, std::uint32_t maxChars)
{
    for (std::uint32_t i = 0; ; ++i)
    {
        std::int32_t c = MSG_ReadByte(msg);
        if (c == -1)
        {
            c = 0;
        }

        if (i < maxChars)
        {
            string[i] = I_CleanChar(c);
        }

        if (!c)
        {
            break;
        }
    }

    string[maxChars - 1] = 0;
    return string;
}

//DONE : 0x463820
void MSG_WriteString(msg_t* msg, const char* source)
{
    std::size_t v2 = strlen(source);

    std::int32_t v3 = v2;
    std::int32_t v7;
    char destination[1024];

    if (v2 < 1024)
    {
        v7 = 0;

        if (v2 > 0)
        {
            do
            {
                destination[v7] = I_CleanChar(destination[source - destination + v7]);
                ++v7;
            }
            while (v7 < v3);
        }

        destination[v7] = 0;

        if (msg->curSize + v3 + 1 <= msg->maxSize)
        {
            msg->curSize += v3 + 1;
            return;
        }
    }
    else
    {
        I_strncpyz(destination, source, 896);

        if (msg->curSize + 1 <= msg->maxSize)
        {
            Com_Memcpy(&msg->data[msg->curSize], *(char**)0x6FAC0D, 1);
            msg->curSize += 1;
            return;
        }
    }
    msg->overflowed = true;
}
