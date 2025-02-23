#pragma once
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcId(const wchar_t* processName);
uintptr_t GetModuleBaseAddr(DWORD processId, const wchar_t* moduleName);

extern HANDLE hProcess;

namespace Memory
{
    template <typename T>
    const T RPM(const uintptr_t& address)
    {
        T value = {};
        ReadProcessMemory(hProcess, reinterpret_cast<const void*>(address), &value, sizeof(T), 0);
        return value;
    }

    template <typename U>
    const void WPM(const uintptr_t& address, const U& value)
    {
        WriteProcessMemory(hProcess, reinterpret_cast<void*>(address), &value, sizeof(U), 0);
    }

}


