#include "helper.h"

float euclidean_distance(float x, float y)
{
	return sqrtf((x * x) + (y * y));
}

namespace offsets
{
	extern uintptr_t localplayer = 0x10f4f4;
	extern uintptr_t entitylist = 0x10f4f8;
	extern uintptr_t origin = 0x4; // Position vom Spieler / Origin
	extern uintptr_t newpos = 0x34; // Kopf Position vom Spieler
	extern uintptr_t yaw = 0x40; // Yaw vom Spieler
	extern uintptr_t health = 0xF8; // Gesundheitswert vom Spieler
	extern uintptr_t armor = 0x00FC; // Armor vom Spieler
	extern uintptr_t lifesequence = 0x1F8; // Lebensstatus vom Spieler
	extern uintptr_t name = 0x225; // Name vom Spieler
	extern uintptr_t team = 0x22D; // Team Nummer vom Spieler
}


void getEntity(entity* entity)
{
	entity->origin = mem::RPM<Vec3>(hProcess,entity->baseadress + offsets::origin);
	entity->angles = mem::RPM<Vec3>(hProcess, entity->baseadress + offsets::yaw);
	entity->newpos = mem::RPM<Vec3>(hProcess, entity->baseadress + offsets::newpos);
	entity->health = mem::RPM<int>(hProcess, entity->baseadress + offsets::health);
	entity->armor = mem::RPM<int>(hProcess, entity->baseadress + offsets::armor);
	entity->team = mem::RPM<int>(hProcess, entity->baseadress + offsets::team);
}