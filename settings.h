#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> 


namespace offsets
{
	extern uintptr_t localplayer;
	extern uintptr_t entitylist;
	extern uintptr_t origin; // Position vom Spieler / Origin
	extern uintptr_t newpos; // Kopf Position vom Spieler
	extern uintptr_t yaw; // Yaw vom Spieler
	extern uintptr_t health; // Gesundheitswert vom Spieler
	extern uintptr_t lifesequence; // Lebensstatus vom Spieler
	extern uintptr_t name; // Name vom Spieler
	extern uintptr_t team; // Team Nummer vom Spieler
	extern uintptr_t mvp_matrix;
	extern uintptr_t ammo_assaultRifle;
	extern uintptr_t ammo_Pistol;
	extern uintptr_t ammo_grenade;
	extern uintptr_t mag_assaultRifle;
	extern uintptr_t mag_cPistol;
}

void SetupImGuiStyle();
void ShowMenu(GLFWwindow* Window);
void HideMenu(GLFWwindow* Window);

