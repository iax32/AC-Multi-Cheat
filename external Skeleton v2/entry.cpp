#include <iostream>
#include "proc.h"
#include "mem.h"
#include "datatypes.h"
#include "helper.h"
#include <math.h>

HANDLE hProcess = nullptr;
#define M_PI 3.14159265358979323846


int main()
{
    // Variables:
    bool bOptionOne = false;
    bool bOptionTwo = false;
    DWORD procId = 0;
    uintptr_t moduleBase = 0;
    uintptr_t dynamicPtrBaseAddr = 0;

    // Attempt to get process ID and module base address
    procId = GetProcId(L"ac_client.exe");
    if (procId == 0)
    {
        std::cerr << "Failed to retrieve process ID." << std::endl;
        return 1;
    }

    moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");
    if (moduleBase == 0)
    {
        std::cerr << "Failed to retrieve module base address." << std::endl;
        return 1;
    }

    // Open the process with appropriate access rights
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
    if (hProcess == nullptr)
    {
        std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
        return 1;
    }


    
    entity player{};
    player.baseadress = mem::RPM<uintptr_t>(hProcess, moduleBase + 0x10f4f4);
    std::vector<entity> enemy(32);

    // Main loop to monitor process and handle options
    DWORD dwExit = 0;
    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
    {
        getEntity(&player);
        float closest_player = -1.0f;
        float closest_yaw = 0.0f;
        float closest_pitch = 0.0f;

        for (int i = 1; i <= 32; ++i)
        {
            uintptr_t ListEntry = mem::RPM<uintptr_t>(hProcess, moduleBase + offsets::entitylist);
            enemy[i].baseadress = mem::RPM<uintptr_t>(hProcess, ListEntry + (i * 0x4));

            // >> DEBUG LOCATION ENEMY
            //std::cout << "X: " << enemy[i].origin.x << " Y: " << enemy[i].origin.y << " Z: " << enemy[i].origin.x << std::endl;

            // >> DEBUG ANGLES ENEMY
            //std::cout << "X: " << enemy[i].angles.x << " Y: " << enemy[i].angles.y<< std::endl;

            // >> DEBUG ANGLES PLAYER
            //std::cout << "X: " << player.angles.x << " Y: " << player.angles.y<< std::endl;


            
            getEntity(&enemy[i]);

            if (enemy[i].health < 0 || enemy[i].health > 100)
                continue;

            
            float abspos_x = enemy[i].origin.x - player.origin.x;
            float abspos_y = enemy[i].origin.y - player.origin.y;
            float abspos_z = enemy[i].origin.z - player.origin.z;

            
            float temp_distance = euclidean_distance(abspos_x, abspos_y);
            if (closest_player == -1.0f || temp_distance < closest_player)
            {
                closest_player = temp_distance;

                float azimuth_xy = atan2f(abspos_y, abspos_x);
                float yaw = (float)(azimuth_xy * (180.0 / M_PI));

                closest_yaw = yaw + 90;

                if (abspos_y < 0) {
                    abspos_y *= -1;
                }
                if (abspos_y < 5) {
                    if (abspos_x < 0) {
                        abspos_x *= -1;
                    }
                    abspos_y = abspos_x;
                }

                float azimuth_z = atan2f(abspos_z, abspos_y);
                closest_pitch = (float)(azimuth_z * (180.0 / M_PI));
            }

        }

        if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
        {
            mem::WPM<Vec3>(hProcess, player.baseadress + offsets::yaw, { closest_yaw, closest_pitch, 0 });
        }
        
            

        
        

        // Toggle option one with VK_NUMPAD1
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bOptionOne = !bOptionOne;
            // Execute option one logic here if needed
        }

        // Toggle option two with VK_NUMPAD2
        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bOptionTwo = !bOptionTwo;
            // Execute option two logic here if needed
        }

        // Execute option two logic every tick if bOptionTwo is true
        if (bOptionTwo)
        {
            // Execute logic for option two every tick
        }

        Sleep(10); // Adjust the sleep duration as needed
    }

    // Close the process handle before exiting
    CloseHandle(hProcess);
    return 0;
}
