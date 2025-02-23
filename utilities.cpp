#include "utilities.h"

namespace offsets
{
	extern uintptr_t localplayer = 0x10f4f4;
	extern uintptr_t entitylist = 0x10f4f8;
	extern uintptr_t origin = 0x4;
	extern uintptr_t newpos = 0x34;
	extern uintptr_t yaw = 0x40;
	extern uintptr_t health = 0xF8;
	extern uintptr_t armor = 0xFC;
	extern uintptr_t lifesequence = 0x1F8;
	extern uintptr_t name = 0x225;
	extern uintptr_t team = 0x32C;
	extern uintptr_t mvp_matrix = 0x101AE8;
	extern uintptr_t ammo_assaultRifle = 0x128;
	extern uintptr_t ammo_Pistol = 0x114;
	extern uintptr_t ammo_grenade = 0x158;
	extern uintptr_t mag_assaultRifle = 0x150;
	extern uintptr_t mag_cPistol = 0x13C;
}


DWORD GetProcId(const wchar_t* processName)
{
	DWORD processId = 0;
	HANDLE handleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (handleSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 processEntry;
		processEntry.dwSize = sizeof(processEntry);

		if (Process32First(handleSnap, &processEntry))
		{
			do
			{
				if (!_wcsicmp(processName, processEntry.szExeFile))
				{
					processId = processEntry.th32ProcessID;
					break;
				}

			} while (Process32Next(handleSnap, &processEntry));
		}

	}

	CloseHandle(handleSnap);
	return processId;
}

uintptr_t GetModuleBaseAddr(DWORD processId, const wchar_t* moduleName)
{
	uintptr_t moduleBase = 0;
	HANDLE handleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, processId);

	if (handleSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);

		if (Module32First(handleSnap, &moduleEntry))
		{
			do
			{
				if (!_wcsicmp(moduleName, moduleEntry.szModule))
				{
					moduleBase = (uintptr_t)moduleEntry.modBaseAddr;
					break;
				}

			} while (Module32Next(handleSnap, &moduleEntry));

		}
	}

	CloseHandle(handleSnap);
	return moduleBase;

}




