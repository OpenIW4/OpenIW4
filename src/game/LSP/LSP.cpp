#include "LSP.hpp"
#include "../Dvar/Dvar.hpp"
#include "../IWNet/IWNet.hpp"
#include "../Live/Live.hpp"
#include "../Sys/Sys.hpp"

#include <memory/memory.hpp>

//DONE : 0x48A9D0
void LSP_Init()
{
	*(bool*)0x66C6C1E = 1; //s_logStrings
	*(bool*)0x66C6C1C = 1; //s_sendStats

	*(std::uint32_t*)0x66C639C = Dvar_RegisterBool("lsp_debug", 0, 0, "Whether to print LSP debug info"); //lsp_debug
	*(std::uint32_t*)0x66BB650 = 4; //g_iwnetMatchmakingServerAddr
	*(std::uint32_t*)0x66BB090 = 4; //g_iwnetStorageServerAddr
	*(std::uint32_t*)0x66BB078 = 4; //dword_66BB078
	*(std::uint32_t*)0x66C714c = 4; //g_iwnetLoggingServerAddr

}

//DONE : 0x4EC640
bool LSP_Connected()
{
	return *(bool*)0x66C7638; //lsp_connected
}

//DONE : 0x4664A0
bool LSP_FindTitleServers()
{
	bool result = IWNet_DNSResolved();
	*(bool*)0x4664A5 = result; //lsp_connected
	return result;
}

//DONE : 0x5A95F0
bool LSP_FindTitleServers_f()
{
	return LSP_FindTitleServers();
}

//TODO: 0x682360
void /*__usercall*/ InitLog(std::int32_t a1)
{
	std::int64_t v3;
	memory::call<unsigned long* (unsigned long*, std::int32_t, std::int32_t)>(0x45FCA0/*MSG_Init*/)(*(unsigned long**)0x66C7160, *(std::int32_t*)0x66C7188, 1200);
	Live_GetLSPXuid(a1, &v3);

	std::int32_t* currentSession = Live_GetCurrentSession();
	const char* localClientName = Live_GetLocalClientName();
	//fucking LSP_WritePacketHeader is TODO
}

//TODO : 0x682400
void /*__usercall*/ LSP_CheckForLogSend(std::int32_t a1, std::int32_t a2)
{
	std::int32_t* currentSession = Live_GetCurrentSession();

	if (!*(bool*)0x66C639A /*logMsgInittialized*/)
	{
		Sys_EnterCriticalSection(CRITSECT_LIVE);

		if (!*(bool*)0x66C639A)
		{
			if (!Live_IsSignedIn())
			{
				Sys_LeaveCriticalSection(CRITSECT_LIVE);
				return;
			}
			InitLog(a1);
			*(bool*)0x66C639A = true;
		}
		Sys_LeaveCriticalSection(CRITSECT_LIVE);
	}

	if(a2 + *(std::int32_t*)0x66C7174 >= *(std::int32_t*)0x66C7170
		|| *(std::int32_t*)0x66C6C14 /*s_currentControllerIndex*/ != a1
		|| *((unsigned char**)currentSession + 8) && memcmp(currentSession + 118, *(std::int32_t**)0x66C7118, 8))
	{
		//fucking LSP_SendLogRequest is TODO
	}
}

void LSP_LogStringEvenIfControllerIsInactive(const char* string)
{
    memory::call<void(const char*)>(0x4B62C0)(string);
}

void LSP_ForceSendPacket()
{
    memory::call<void()>(0x682520)();
}
