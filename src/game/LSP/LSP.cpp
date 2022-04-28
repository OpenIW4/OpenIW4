#include "LSP.hpp"
#include "../Dvar/Dvar.hpp"
#include "../IWNet/IWNet.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x0048A9D0
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

//DONE : 0x004EC640
bool LSP_Connected()
{
	return *(bool*)0x66C7638; //lsp_connected
}

//DONE : 0x004664A0
bool LSP_FindTitleServers()
{
	bool result = IWNet_DNSResolved();
	*(bool*)0x4664A5 = result; //lsp_connected
	return result;
}

//DONE : 0x005A95F0
bool LSP_FindTitleServers_f()
{
	return LSP_FindTitleServers();
}