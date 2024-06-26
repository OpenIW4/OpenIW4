#pragma once

#include "../stdafx.hpp"
#include "../defs.hpp"

#define Sys_OutOfMemError() Sys_OutOfMemErrorInternal(__FILE__, __LINE__);

#pragma region Sys variables

static volatile std::int32_t* sv_thread_owns_game = reinterpret_cast<volatile std::int32_t*>(0x2089DB8);
static SysInfo* sys_info = reinterpret_cast<SysInfo*>(0x064A17A0);
static std::uint32_t s_cpuCount = *reinterpret_cast<std::uint32_t*>(0x1CDE7F0);

static HANDLE hEvent = *reinterpret_cast<HANDLE*>(0x1CDE7EC);
static HANDLE noThreadOwnershipEvent = *reinterpret_cast<HANDLE*>(0x1CDE704);
static void* backendEvent = *reinterpret_cast<void**>(0x1CDE730); //backendEvent[2] (was just a HANDLE in IDA)
static HANDLE dword_1CDE850 = *reinterpret_cast<HANDLE*>(0x1CDE850);

static HANDLE* threadHandle = *reinterpret_cast<HANDLE**>(0x1CDE828); //9 elements
static std::int32_t* threadId = *reinterpret_cast<std::int32_t**>(0x1CDE7FC); //9 elements

static WinConData* s_wcd = reinterpret_cast<WinConData*>(0x064A3288);
static HANDLE* databaseCompletedEvent = reinterpret_cast<HANDLE*>(0x1CDE7F8);
static HANDLE* databaseCompletedEvent2 = reinterpret_cast<HANDLE*>(0x01CDE858);

static char* sys_processSemaphoreFile = *reinterpret_cast<char**>(0x649FF74);
#pragma endregion
void Sys_ShowConsole();
void Sys_CreateConsole(HINSTANCE hInstance);
void Sys_InitializeCriticalSections();
void Sys_InitMainThread();
void Sys_Sleep(DWORD dwMilliseconds);
void Sys_CheckQuitRequest();
void Sys_RecordAccessibilityShortcutSettings();
void Sys_AllowAccessibilityShortcutKeys(bool a1);
void Sys_CreateSplashWindow();
void Sys_ShowSplashWindow();
void Sys_RegisterClass();
int Sys_Milliseconds();
int* Sys_GetValue(int a1);
void Sys_SetValue(int valueIndex, void* data);
void Sys_getcwd();
bool Sys_IsMainThread();
void Sys_EnterCriticalSection(CriticalSection critSect);
void Sys_LeaveCriticalSection(CriticalSection critSect);
void Sys_EnumerateHw();
bool Sys_IsDatabaseReady();
bool Sys_IsDatabaseReady2();
bool Sys_ReleaseThreadOwnership();
void Sys_DatabaseCompleted();
long __stdcall ConsoleWndProc(HWND hWnd, std::uint32_t msg, std::uint32_t wParam, long lParam);
long __stdcall InputLineWndProc(HWND hWnd, std::uint32_t msg, std::uint32_t wParam, long lParam);
void Sys_Error(const char* error, ...);
void Sys_SuspendOtherThreads();
void Sys_SetErrorText(const char* text);
void Sys_OutOfMemErrorInternal(const char* filename, std::int32_t line);
long Sys_InterlockedDecrement(volatile long* target);
long Sys_InterlockedIncrement(volatile long* target);
long Sys_InterlockedCompareExchange(volatile long* target, long exChange, long comperand);
long Sys_InterlockedExchange(volatile long* target, long value);
void Sys_TempPriorityAtLeastNormalBegin(TempPriority* tempPriority);
void Sys_TempPriorityEnd(TempPriority* tempPriority);
void Sys_LockWrite(FastCriticalSection* section);
void Sys_UnlockWrite(FastCriticalSection* section);
void Sys_LockRead(FastCriticalSection* section);
void Sys_UnlockRead(FastCriticalSection* section);
int __stdcall HideWindowCallback(HWND hWnd, long lParam);
void FixWindowsDesktop();
bool Sys_IsRenderThread();
bool Sys_IsServerThread();
bool Sys_IsDatabaseThread();
char Sys_SendPacket(std::int32_t len, const void* data, netadr_t to);
void NetadrToSockadr(netadr_t* a1, sockaddr* a2);
std::int32_t Sys_GetCPUCount();
std::int32_t Sys_SystemMemoryMB();
void Sys_NormalExit();
std::int32_t SV_GetServerThreadOwnsGame();
void Sys_DetectVideoCard(std::int32_t descLimit, char* description);
void Sys_UnlockWrite(FastCriticalSection* critSect);
void Sys_FrontEndSleep();
void Sys_NotifyRenderer();
int Sys_GetSemaphoreFileName();
int Sys_IsGameProcess(int id);
int Sys_CheckCrashOrRerun();
