#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

// Process 
DWORD GetProcId(const wchar_t* procName);

// Module
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* procName);

// Derefrence Pointer Chain
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, const std::vector<unsigned int>& offsets);


