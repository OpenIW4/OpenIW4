#include "MSG.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x0048C520
void MSG_WriteByte(unsigned long* a1, std::int8_t a2)
{
	std::int32_t v1 = a1[5];

	if (v1 >= a1[4])
	{
		*a1 = 1;
	}
	else
	{
		*(std::uint8_t*)(v1 + a1[2]) = a2;
		a1[5]++;
	}

}

//DONE : 0x44E1F0
void MSG_WriteBit0(unsigned long* a1)
{
	std::int32_t v1;

	if ((a1[8] & 7) == 0)
	{
		v1 = a1[5];
		if (v1 >= a1[4])
		{
			*a1 = 1;
		}
		a1[8] = 8 * v1;
		*(unsigned char*)(v1 + a1[2]) = 0;
		a1[5]++;
	}
	a1[8]++;
}

//DONE : 0x0045A600
void MSG_WriteBit1(unsigned long* a1)
{
	std::int32_t v2 = a1[8] & 7;

	std::int32_t v3;
	std::int8_t* v4;

	if (!v2)
	{
		v3 = a1[5];
		if (v3 >= a1[4])
		{
			*a1 = 1;
			return;
		}

		a1[8] = 8 * v3;
		*(std::int8_t*)(v3 + a1[2]) = 0;
		a1[5]++;
	}

	v4 = (std::int8_t*)(((std::int32_t)a1[8] >> 3) + a1[2]);
	*v4 |= 1 << v2;
	a1[8]++;
}

//TODO : 0x00441230
void MSG_WriteInt64(unsigned long* a1, std::int32_t a2, std::int32_t a3)
{
	std::int32_t v3 = a1[5] + 8;

	std::int32_t v4;
	std::int32_t v5;
	std::int32_t v6;
	std::int32_t v7 = 0;

	if (v3 > a1[4])
	{
		*a1 = 1;
	}
	else
	{
		v4 = memory::call<std::int32_t(std::int32_t, std::int32_t)>(0x004A94F0)(a2, a3);
		v5 = a1[5];
		v6 = a1[2];
		*(DWORD*)(v6 + v5) = v4;
		*(DWORD*)(v6 + v5 + 4) = v7; //most likely unused
		a1[5] = v3;
	}
}

//DONE : 0x503B90
void MSG_WriteShort(unsigned long* a1, std::int16_t a2)
{
	std::int32_t v1 = a1[5];

	if (v1 + 2 > a1[4])
	{
		*a1 = 1;
	}
	else
	{
		*(std::uint16_t*)(v1 + a1[2]) = a2;
		a1[5] = v1 + 2;
	}
}