#include "MSG.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x0048C520
DWORD* MSG_WriteByte(DWORD* a1, std::int8_t a2)
{
	DWORD* result = a1;
	std::int32_t v3 = a1[5];

	if (v3 >= a1[4])
	{
		*a1 = 1;
	}
	else
	{
		*(std::uint8_t*)(v3 + a1[2]) = a2;
		a1[5]++;
	}

	return result;
}

//DONE : 0x0045A600
DWORD* MSG_WriteBit1(DWORD* a1)
{
	DWORD* result = a1;
	std::int32_t v2 = a1[8] & 7;

	std::int32_t v3;
	std::int8_t* v4;

	if (!v2)
	{
		v3 = a1[5];
		if (v3 >= a1[4])
		{
			*a1 = 1;
			return result;
		}

		a1[8] = 8 * v3;
		*(std::int8_t*)(v3 + a1[2]) = 0;
		a1[5]++;
	}

	v4 = (std::int8_t*)(((std::int32_t)a1[8] >> 3) + a1[2]);
	*v4 |= 1 << v2;
	a1[8]++;
	return result;
}

//THUNK : 0x00441230
void MSG_WriteInt64(DWORD* a1, std::int32_t a2, std::int32_t a3)
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