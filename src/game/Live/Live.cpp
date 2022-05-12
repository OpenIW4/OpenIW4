#include "Live.hpp"
#include "../Party/Party.hpp"

#include <memory/memory.hpp>

//DONE : 0x5027D0
std::int32_t* __stdcall Live_GetCurrentSession()
{
	bool v0 = !Party_InParty((std::int32_t)0x107E500 /*g_partyData*/);
	std::int32_t* result = (std::int32_t*)0x107E500;

	if (v0)
	{
		return (std::int32_t*)0x66B7008; //g_serverSession
	}

	return result;
}

//DONE : 0x43D7a0
bool Live_IsSignedIn()
{
	return true;
}

//TODO : 0x43ABC0
std::int32_t Live_GetLSPXuid(std::int32_t a1, std::int64_t* a2)
{
	std::int64_t v2 = LOWORD(memory::call<std::int64_t()>(0x449A70)()); //Steam_GetClientIDAsXUID
	*a2 = v2;
	return v2;
}

//THUNK : 0x4B3AC0
const char* Live_GetLocalClientName()
{
	return memory::call<const char* ()>(0x441FC0)(); //Steam_GetClientPersonaName
}