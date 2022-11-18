#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

#pragma region LSP Variables
static netadr_t g_iwnetMatchmakingServerAddr = *reinterpret_cast<netadr_t*>(0x66BB650);
static netadr_t g_iwnetStorageServerAddr = *reinterpret_cast<netadr_t*>(0x66BB090);
static netadr_t stru_66BB078 = *reinterpret_cast<netadr_t*>(0x66BB078);
static netadr_t g_iwnetLoggingServerAddr = *reinterpret_cast<netadr_t*>(0x66C714C);

static bool s_logStrings = *reinterpret_cast<bool*>(0x66C6C1E);
static bool s_sendStats = *reinterpret_cast<bool*>(0x66C6C1C);
static bool lsp_connected = *reinterpret_cast<bool*>(0x66C7638);
static bool logMsgInittialized = *reinterpret_cast<bool*>(0x66C639A);

static const dvar_t* lsp_debug = *reinterpret_cast<const dvar_t**>(0x66C639C);
static char byte_66C7638 = *reinterpret_cast<char*>(0x66C7638);
static msg_t stru_66C7160 = *reinterpret_cast<msg_t*>(0x66C7160);
#pragma endregion LSP Variables

void LSP_Init();
bool LSP_Connected();
bool LSP_FindTitleServers();
bool LSP_FindTitleServers_f();
void /*__usercall*/ LSP_CheckForLogSend(std::int32_t a1, std::int32_t a2);
void InitLog(std::int32_t a1);
void LSP_LogStringEvenIfControllerIsInactive(const char* string);
void LSP_ForceSendPacket();
std::int32_t Xenon_SendLSPPacket(const char* buf, std::int32_t, netadr_t* net);
void LSP_WritePacketHeader(std::int32_t localControllerIndex, msg_t* msg, std::int32_t a3, std::int32_t a4, char* source, const SessionData* session);
void LSP_ForceSendPacket();

#pragma region Unreferenced
char sub_4DC200();
#pragma endregion Unreferenced
