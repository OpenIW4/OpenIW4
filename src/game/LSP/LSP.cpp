#include "LSP.hpp"
#include "../Dvar/Dvar.hpp"
#include "../IWNet/IWNet.hpp"
#include "../Live/Live.hpp"
#include "../Sys/Sys.hpp"
#include "../Com/Com.hpp"
#include "../MSG/MSG.hpp"
#include "../CL/CL.hpp"
#include "../Unsorted/Unsorted.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x48A9D0
void LSP_Init()
{
    s_logStrings = 1;
    s_sendStats = 1;

	lsp_debug = Dvar_RegisterBool("lsp_debug", 0, 0, "Whether to print LSP debug info"); //lsp_debug
    g_iwnetMatchmakingServerAddr.type = NA_IP;
    g_iwnetStorageServerAddr.type = NA_IP;
    stru_66BB078.type = NA_IP;
    g_iwnetLoggingServerAddr.type = NA_IP;
}

//DONE : 0x4EC640
bool LSP_Connected()
{
    return lsp_connected;
}

//DONE : 0x4664A0
bool LSP_FindTitleServers()
{
	bool result = IWNet_DNSResolved();
	lsp_connected = result;
	return result;
}

//DONE : 0x5A95F0
bool LSP_FindTitleServers_f()
{
	return LSP_FindTitleServers();
}

//DONE : 0x682360
void InitLog(std::int32_t a1)
{
	std::int64_t v3[2];
    MSG_Init(&unk_66C7160, *(char**)0x66C7188, 1200);
    Live_GetLSPXuid(a1, v3);

	SessionData* currentSession = Live_GetCurrentSession();
	const char* localClientName = Live_GetLocalClientName();
    LSP_WritePacketHeader(a1, &unk_66C7160, v3[0], v3[1], (char*)localClientName, currentSession);
}

//TODO : 0x682400
void /*__usercall*/ LSP_CheckForLogSend(std::int32_t a1, std::int32_t a2)
{
	SessionData* currentSession = Live_GetCurrentSession();

	if (!logMsgInittialized)
	{
		Sys_EnterCriticalSection(CRITSECT_LIVE);

		if (!logMsgInittialized)
		{
			if (!Live_IsSignedIn())
			{
				Sys_LeaveCriticalSection(CRITSECT_LIVE);
				return;
			}
			InitLog(a1);
            logMsgInittialized = true;
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

//DONE : 0x4B62C0
void LSP_LogStringEvenIfControllerIsInactive(const char* string)
{
    char* i;
    std::int64_t v10 = 0;

    if (s_logStrings)
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

        if (!logMsgInittialized)
        {
            Sys_EnterCriticalSection(CRITSECT_LIVE);
            if (!logMsgInittialized)
            {
                logMsgInittialized = true;
                MSG_Init((msg_t*)0x66C7160, *(char**)0x66C7188, 1200);

                if (CL_AllLocalClientsInactive() || (CL_GetFirstActiveControllerIndex(), !Live_IsSignedIn()))
                {
                    LSP_WritePacketHeader(*(std::int32_t*)0x66C6C14,
                        (msg_t*)0x66C7160,
                        v10,
                        v10, "Not signed in",
                        Live_GetCurrentSession());
                }
                else
                {
                    const char* v7 = memory::call<const char* ()>(0x441FC0)(); //some steam call
                    Live_GetLSPXuid(CL_GetFirstActiveControllerIndex(), &v10);
                    LSP_WritePacketHeader(
                        CL_GetFirstActiveControllerIndex(),
                        (msg_t*)0x66C7160,
                        v10,
                        v10,
                        (char*)v7,
                        Live_GetCurrentSession()
                    );
                }
            }
            Sys_LeaveCriticalSection(CRITSECT_LIVE);
        }

        if (!*(std::int32_t*)0x66C7120/*s_firstLogWriteTime*/)
        {
            *(std::int32_t*)0x66C7120 = Sys_Milliseconds();
        }

        Sys_EnterCriticalSection(CRITSECT_LIVE);
        MSG_WriteByte((msg_t*)0x66C7160, 2);
        MSG_WriteLong((msg_t*)0x66C7160, Sys_Milliseconds());
        MSG_WriteString((msg_t*)0x66C7160, i);
        Sys_LeaveCriticalSection(CRITSECT_LIVE);
    }
}

//DONE : 0x4B41E0
//this function may need to be moved
std::int32_t Xenon_SendLSPPacket(const char* buf, std::int32_t a2, netadr_t* net)
{
    sockaddr to;
    NetadrToSockadr(net, &to);
    std::int32_t v3 = sendto(*(std::uint32_t*)0x64A1E04, (const char*)buf, a2, 0, &to, 16);
    if (*(std::uint8_t*)0x66C639C + 16) //some sort of struct most likely
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
    if (lsp_connected)
    {
        Sys_EnterCriticalSection(CRITSECT_LIVE);

        if (logMsgInittialized)
        {
            g_iwnetLoggingServerAddr.port = htons(3005);

            if (Xenon_SendLSPPacket((const char*)(*(msg_t*)0x66C7160).data, (*(msg_t*)0x66C7160).curSize, *(netadr_t**)0x66C714C) < 0)
            {
                *(bool*)0x66C7638 = 0;
            }
        }
        *(bool*)0x66C639A = 0;
        Sys_LeaveCriticalSection(CRITSECT_LIVE);
    }
}

//DONE : 0x4DA7F0
void LSP_WritePacketHeader(std::int32_t localControllerIndex, msg_t* msg, std::int32_t a3, std::int32_t a4, char* source, const SessionData* session)
{
    const char* map;
    MSG_WriteByte(msg, 14);
    MSG_WriteBit1(msg);
    MSG_WriteInt64(msg, a3, a4);
    MSG_WriteString(msg, source);
    MSG_WriteString(msg, va("%s %s build %s %s", "OpenIW4 MP", "inf-dev", getBuildNumber(), "win-x86"));

    if (sv_map)
    {
        map = (char*)sv_map->current.integer;
    }
    else
    {
        map = "no map yet";
    }
    MSG_WriteString(msg, map);
    MSG_WriteBit1(msg);
    std::int32_t v9 = *(unsigned long*)session->dyn.sessionInfo.sessionID.ab;
    *(unsigned long*)0x66C7118 = v9;
    std::int32_t v10 = *(unsigned long*)&session->dyn.sessionInfo.sessionID.ab[4];
    *(unsigned long*)0x66C711C = v10;

    if (*(unsigned long*)0x66C7110)
    {
        MSG_WriteInt64(msg, *(unsigned long*)0x66C7110, (*(unsigned long*)0x66C7110 >> 31 | v9));
    }
    else
    {
        MSG_WriteInt64(msg, v9, v10);
    }

    *(unsigned long*)0x66C6C14 = localControllerIndex;
    *(std::int32_t*)0x66C7120 = 0; //s_firstLogWriteTime
}

//TODO : 0x4DC200
//needs to be named but its not referenced
char sub_4DC200()
{
    if (lsp_debug->current.enabled)
    {
        Com_Printf(25, "IWNet transaction complete\n");
    }
    return 1;
}

//DONE : 0x682520
void LSP_ForceSendPacket()
{
    if (byte_66C7638)
    {
        Sys_EnterCriticalSection(CRITSECT_LSP);
        if (logMsgInittialized)
        {
            g_iwnetLoggingServerAddr.port = htons(3005);
            if (Xenon_SendLSPPacket(stru_66C7160.data, stru_66C7160.curSize, &g_iwnetLoggingServerAddr) < 0)
            {
                byte_66C7638 = 0;
            }
        }
    }
    logMsgInittialized = false;
    Sys_LeaveCriticalSection(CRITSECT_LSP);
}
