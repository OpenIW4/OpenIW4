#include "Cbuf.hpp"

#include <utils/memory/memory.hpp>

//THUNK : 0x00404B20
void Cbuf_AddText(int a1, const char* a2)
{
	memory::call<void(int, const char*)>(0x00404B20)(a1, a2);
}

//THUNK : 0x4AFB80
void Con_GetTextCopy(char* text, int maxSize)
{
    memory::call<void(char*, int)>(0x4AFB80)(text, maxSize);
}

//DONE : 0x64DD30
int Conbuf_CleanText(const char* source, char* target, int sizeofTarget)
{
	char* start;
	const char* last;

	start = target;
	last = target + sizeofTarget - 3;
	while (*source && target <= last)
	{
		if (source[0] == '\n' && source[1] == '\r')
		{
			target[0] = '\r';
			target[1] = '\n';
			target += 2;
			source += 2;
		}
		else
		{
			if (source[0] == '\r' || source[0] == '\n')
			{
				target[0] = '\r';
				target[1] = '\n';
				target += 2;
				++source;
			}
			else if (source && source[0] == '^' && source[1] && source[1] != '^' && source[1] >= 48 && source[1] <= 64)
			{
				source += 2;
			}
			else
			{
				*target++ = *source++;
			}
		}
	}

	*target = 0;
	return target - start;
}

//THUNK : 0x004F5770
void Conbuf_AppendText(const char* a1)
{
	char* v1;
	std::int32_t v2;
	char lParam[32768];

	if (strlen(a1) <= 16383)
	{
		v1 = (char*)a1;
	}
	else
	{
		v1 = (char*)&a1[strlen(a1) - 16383];
	}

	v2 = (std::int32_t)memory::call<unsigned char*(char*, int, char*)>(0x64DD30)(lParam, 0x8000, v1);
	*(std::int32_t*)0x64A38B8 += v2;
	
	if (*(std::uint32_t*)0x64A38B8 <= 0x4000)
	{
		SendMessageA(*(HWND*)0x64A328C, 0xB1, 0xFFFF, 0xFFFF);
	}
	else
	{
		SendMessageA(*(HWND*)0x64A328C, 0xB1, 0, -1);
		*(std::int32_t*)0x64A38B8 = v2;
	}

	SendMessageA(*(HWND*)0x64A328C, 0xB6, 0, 0xFFFF);
	SendMessageA(*(HWND*)0x64A328C, 0xB7, 0, 0);
	SendMessageA(*(HWND*)0x64A328C, 0xC2, 0, (long)lParam);
}