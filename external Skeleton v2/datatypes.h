#pragma once
#include "Windows.h"

extern double PI;

extern HANDLE hProcess;

struct Vec2 
{
	float x, y;
};

struct Vec3 
{
	float x, y, z;
};

struct Matrix
{

};

enum ID
{
	PLAYER,
	ENEMY
};

struct entity
{
	uintptr_t baseadress;

	Vec3 origin;
	Vec3 newpos;
	Vec3 angles;
	// >> Add Viewmatrix here

	int health, armor;
	int lifesequenze;
	int team;

	ID entityId;

	bool valid = false;
};

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
}