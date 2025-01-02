// Credits:
// https://guidedhacking.com/

#include "proc.h"


DWORD GetProcId(const wchar_t* procName)
{
    DWORD procId = 0;

    // Take a snapshot of all processes in the system.
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to create snapshot of processes. Error: " << GetLastError() << std::endl;
        return 0;
    }

    // Initialize the process entry structure.
    PROCESSENTRY32 procEntry{};
    procEntry.dwSize = sizeof(procEntry);

    // Retrieve information about the first process encountered in the snapshot.
    if (Process32First(hSnap, &procEntry))
    {
        // Iterate through the list of processes in the snapshot.
        do
        {
            if (!_wcsicmp(procEntry.szExeFile, procName))
            {
                procId = procEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnap, &procEntry));
    }
    else
    {
        std::cerr << "Failed to retrieve the first process. Error: " << GetLastError() << std::endl;
    }


    // Handle case when the process is not found.
    if (procId == 0)
        std::cerr << "Process not found: " << procName << std::endl;
    
    // Close the handle to the snapshot.
    CloseHandle(hSnap);
    return procId;
}

//Module
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
    uintptr_t modBaseAddr = 0;

    // Take a snapshot of all modules in the specified process.
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to create snapshot of modules. Error: " << GetLastError() << std::endl;
        return 0;
    }

    // Initialize the module entry structure.
    MODULEENTRY32 modEntry{};
    modEntry.dwSize = sizeof(modEntry);

    // Retrieve information about the first module in the snapshot.
    if (Module32First(hSnap, &modEntry))
    {
        // Iterate through the list of modules in the snapshot.
        do
        {
            if (!_wcsicmp(modEntry.szModule, modName))
            {
                modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnap, &modEntry));
    }
    else
    {
        std::cerr << "Failed to retrieve the first module. Error: " << GetLastError() << std::endl;
    }

    // Handle case when the module is not found.
    if (modBaseAddr == 0)
        std::cerr << "Module not found: " << modName << std::endl;
    

    // Close the handle to the snapshot.
    CloseHandle(hSnap);
    return modBaseAddr;
}


// Derefrence Pointer chain
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, const std::vector<unsigned int>& offsets)
{
    // Check if the handle to the process is valid.
    if (hProc == nullptr)
    {
        std::cerr << "Invalid process handle." << std::endl;
        return 0;
    }

    // Check if the offsets vector is empty.
    if (offsets.empty())
    {
        std::cerr << "Offset vector is empty." << std::endl;
        return ptr;
    }

    // Initialize the address with the initial pointer value.
    uintptr_t addr = ptr;

    // Iterate through the list of offsets to traverse the pointer chain.
    for (size_t i = 0; i < offsets.size(); ++i)
    {
        // Read the memory at the current address.
        if (!ReadProcessMemory(hProc, (LPCVOID)addr, &addr, sizeof(addr), nullptr))
        {
            std::cerr << "Failed to read memory at address: 0x" << std::hex << addr << ". Error: " << GetLastError() << std::endl;
            return 0;
        }

        // Check if the read address is valid before adding the offset.
        if (addr == 0)
        {
            std::cerr << "Read memory returned a null address at offset index: " << i << std::endl;
            return 0;
        }

        // Add the current offset to the address.
        addr += offsets[i];
    }

    // Return the final address after applying all offsets.
    return addr;
}
