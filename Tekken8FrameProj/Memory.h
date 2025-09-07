#pragma once

#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>

#define TEKKEN_EXE_NAME "Polaris-Win64-Shipping.exe"

#define TEKKEN_MODULE_NAME L"Polaris-Win64-Shipping.exe"


#define INJECTION_DLL_ABSOLUTE_PATH "Replace with the absolute location of your dll"

#define INJECTION_DLL_NAME_WCHAR L"d3d12imgui.dll"
#define INJECTION_DLL_NAME "d3d12imgui.dll"

extern HANDLE tekkenHandle;
extern int tekkenPid;
extern DWORD_PTR tekkenModulePointer;

HANDLE getProcessHandle(DWORD pid);

DWORD getProcessId(const std::wstring& programNameExe);

DWORD_PTR getModuleBaseAddressDWORD(DWORD pid, const wchar_t* moduleName);

void initTekkenHandle();

void initModuleAddresses();

DWORD64 readDwordFromMemory(HANDLE processHandle, DWORD_PTR address);

DWORD readDwordWithOffset(HANDLE processHandle, DWORD_PTR baseAddress, DWORD offset);

DWORD readDwordWithOffsets(HANDLE processHandle, DWORD_PTR baseAddress, int numOffsets, DWORD* offsets);

//commented out injections to avoid detection
//void injectDLL();

//void uninjectDLL();
