#pragma once
#include "Windows.h"


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

	int health, armor;
	int team;

};
