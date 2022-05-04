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