#include "mem.h"
#include <iostream>

void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
    DWORD oldprotect;

    // Check if the handle to the process is valid.
    if (hProcess == nullptr)
    {
        std::cerr << "Invalid process handle." << std::endl;
        return;
    }

    // Change the memory protection to allow writing.
    if (!VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect))
    {
        std::cerr << "Failed to change memory protection. Error: " << GetLastError() << std::endl;
        return;
    }

    // Write the new bytes to the destination address.
    if (!WriteProcessMemory(hProcess, dst, src, size, nullptr))
    {
        std::cerr << "Failed to write memory. Error: " << GetLastError() << std::endl;

        // Restore the original memory protection in case of failure.
        if (!VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect))
        {
            std::cerr << "Failed to restore memory protection. Error: " << GetLastError() << std::endl;
        }
        return;
    }

    // Restore the original memory protection.
    if (!VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect))
    {
        std::cerr << "Failed to restore memory protection. Error: " << GetLastError() << std::endl;
    }
}


void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
    // Check if the handle to the process is valid.
    if (hProcess == nullptr)
    {
        std::cerr << "Invalid process handle." << std::endl;
        return;
    }

    // Check if the size is valid.
    if (size == 0)
    {
        std::cerr << "Size must be greater than 0." << std::endl;
        return;
    }

    // Allocate memory for the NOP array.
    BYTE* nopArray = new(std::nothrow) BYTE[size];
    if (!nopArray)
    {
        std::cerr << "Failed to allocate memory for NOP array." << std::endl;
        return;
    }

    // Fill the NOP array with 0x90 (NOP instruction).
    memset(nopArray, 0x90, size);

    // Apply the NOP array to the target address.
    PatchEx(dst, nopArray, size, hProcess);

    // Clean up the allocated memory.
    delete[] nopArray;
}

