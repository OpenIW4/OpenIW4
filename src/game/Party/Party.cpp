#include "Party.hpp"

//DONE : 0x4F10C0
bool Party_InParty(std::int32_t a1)
{
	return *(unsigned long*)(a1 + 6144) && *(unsigned long*)(a1 + 6140);
}