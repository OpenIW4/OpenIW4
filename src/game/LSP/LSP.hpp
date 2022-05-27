#pragma once

#include "../stdafx.hpp"

void LSP_Init();
bool LSP_Connected();
bool LSP_FindTitleServers();
bool LSP_FindTitleServers_f();
void /*__usercall*/ LSP_CheckForLogSend(std::int32_t a1, std::int32_t a2);
void /*__usercall*/ InitLog(std::int32_t a1);
void LSP_LogStringEvenIfControllerIsInactive(const char* string);
void LSP_ForceSendPacket();
std::int32_t Xenon_SendLSPPacket(const std::uint8_t* buf, std::int32_t, netadr_t* net);
