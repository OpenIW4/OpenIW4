#include "LSP.hpp"
#include "../Dvar/Dvar.hpp"
#include "../IWNet/IWNet.hpp"
#include "../Live/Live.hpp"
#include "../Sys/Sys.hpp"
#include "../Com/Com.hpp"
#include "../MSG/MSG.hpp"
#include "../CL/CL.hpp"

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

//TODO : 0xB62C0
void LSP_LogStringEvenIfControllerIsInactive(const char* string)
{
    //memory::call<void(const char*)>(0x4B62C0)(string);

    char* i;
    std::int64_t v10 = 0;

    if (*(bool*)0x66C6C1E /*s_logString*/)
    {
        for (i = (char*)string; *i == 10; ++i)
        {
            if (!i[1])
            {
                break;
            }
        }

        if (strlen(i) + (*(msg_t*)0x66C7160).curSize + 6 > (*(msg_t*)0x66C7160).maxSize)
        {
            LSP_ForceSendPacket();
        }

        if (!*(bool*)0x66C639A /*logMsgInitialized*/)
        {
            Sys_EnterCriticalSection(CRITSECT_LIVE);
            if (!*(bool*)0x66C639A)
            {
                *(bool*)0x66C639A = true;
                MSG_Init(*(msg_t**)0x66C7160, *(const char**)0x66C7188, 1200);

                if (CL_AllLocalClientsInactive() || (CL_GetFirstActiveControllerIndex(), !Live_IsSignedIn()))
                {
                    //LSP_WritePacketHeader goes here, todo
                    //labled and done in IDA, just need to add to code base
                }
                else
                {
                    Live_GetLSPXuid(CL_GetFirstActiveControllerIndex(), &v10);
                    //LSP_WritePacketHeader stuff here, todo
                }
            }
            Sys_LeaveCriticalSection(CRITSECT_LIVE);
        }

        if (!*(std::int32_t*)0x66C7120/*s_firstLogWriteTime*/)
        {
            *(std::int32_t*)0x66C7120 = Sys_Milliseconds();
        }

        Sys_EnterCriticalSection(CRITSECT_LIVE);
        MSG_WriteByte(*(msg_t**)0x66C7160, 2);
        MSG_WriteLong(*(msg_t**)0x66C7160, Sys_Milliseconds());
        //msg_writestring todo
        Sys_LeaveCriticalSection(CRITSECT_LIVE);
    }
}

//DONE : 0x4B41E0
//this function may need to be moved
std::int32_t Xenon_SendLSPPacket(const std::uint8_t* buf, std::int32_t a2, netadr_t* net)
{
    sockaddr to;
    NetadrToSockadr(net, &to);
    std::int32_t v3 = sendto(*(std::uint32_t*)0x64A1E04, (const char*)buf, a2, 0, &to, 16);
    if (*(std::uint8_t*)0x66C639C + 16)
    {
        std::uint16_t v4 = ntohs(net->port);
        Com_Printf(14,
            "Sending %i byte LSP packet to %u.%u.%u.%u:%i",
            a2,
            net->ip[0],
            net->ip[1],
            net->ip[2],
            net->ip[3],
            v4);
    }

    if (v3 != a2)
    {
        std::uint16_t v5 = ntohs(net->port);
        Com_Printf(14,
            "Sending %i (actually send %i) byte LSP packet to %u.%u.%u.%u:%i\n",
            a2,
            v3,
            net->ip[0],
            net->ip[1],
            net->ip[2],
            net->ip[3],
            v5);
    }

    return v3;
}


//DONE : 0x682520
void LSP_ForceSendPacket()
{
    if (*(bool*)0x66C7638 /*lsp_connected*/)
    {
        Sys_EnterCriticalSection(CRITSECT_LIVE);

        if (*(bool*)0x66C639A /*logMsgInitialized*/)
        {
            (*(netadr_t*)0x66C714C).port = htons(3005);

            if (Xenon_SendLSPPacket((const std::uint8_t*)(*(msg_t*)0x66C7160).data, (*(msg_t*)0x66C7160).curSize, *(netadr_t**)0x66C714C) < 0)
            {
                *(bool*)0x66C7638 = 0;
            }
        }
        *(bool*)0x66C639A = 0;
        Sys_LeaveCriticalSection(CRITSECT_LIVE);
    }
}
