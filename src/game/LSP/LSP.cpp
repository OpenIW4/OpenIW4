#include "LSP.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x48A9D0
void LSP_Init()
{
	*(char*)0x66C6C1E = 1; //s_logStrings
	*(char*)0x66C6C1C = 1; //s_sendStats

	*(std::uint32_t*)0x66C639C = Dvar_RegisterBool("lsp_debug", 0, 0, "Whether to print LSP debug info"); //lsp_debug
	*(std::uint32_t*)0x66BB650 = 4; //g_iwnetMatchmakingServerAddr
	*(std::uint32_t*)0x66BB090 = 4; //g_iwnetStorageServerAddr
	*(std::uint32_t*)0x66BB078 = 4; //dword_66BB078
	*(std::uint32_t*)0x66C714c = 4; //g_iwnetLoggingServerAddr

}