#pragma once
#include "Windows.h"
#include "utilities.h"
#include "settings.h"



struct Vec2
{
	float X, Y;
};

struct Vec3
{
	float X, Y, Z;
};

struct Vec4
{
	float X, Y, Z, W;
};


struct MVPMatrix
{
	float Matrix[16];
};

struct Entity
{
	uintptr_t baseaddress;

	MVPMatrix Matrix;


	Vec3 HeadOrigin;
	Vec3 Origin;
	Vec3 Angles;


	int sizeBoxes = 2;

	Vec3 boundingBoxes[8]
	{
		{Origin.X + sizeBoxes, Origin.Y + sizeBoxes, Origin.Z},
		{Origin.X + sizeBoxes, Origin.Y - sizeBoxes, Origin.Z},

		{Origin.X - sizeBoxes, Origin.Y - sizeBoxes, Origin.Z},
		{Origin.X - sizeBoxes, Origin.Y + sizeBoxes, Origin.Z},

		{HeadOrigin.X + sizeBoxes, HeadOrigin.Y + sizeBoxes, HeadOrigin.Z},
		{HeadOrigin.X + sizeBoxes, HeadOrigin.Y - sizeBoxes, HeadOrigin.Z},

		{HeadOrigin.X - sizeBoxes, HeadOrigin.Y - sizeBoxes, HeadOrigin.Z},
		{HeadOrigin.X - sizeBoxes, HeadOrigin.Y + sizeBoxes, HeadOrigin.Z}
	};

	Vec2 boundingBoxesNdc[8];

	int health, armor;
	int team;

	void getEntity()
	{
		Origin = Memory::RPM<Vec3>(baseaddress + offsets::newpos);
		
		HeadOrigin = Memory::RPM<Vec3>(baseaddress + offsets::origin);

		Angles = Memory::RPM<Vec3>(baseaddress + offsets::yaw);

		health = Memory::RPM<int>(baseaddress + offsets::health);

		armor = Memory::RPM<int>(baseaddress + offsets::armor);

		team = Memory::RPM<int>(baseaddress + offsets::team);




	}

};
