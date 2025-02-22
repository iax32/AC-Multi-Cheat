#pragma once
#include "datatypes.h"
#include <math.h>

#define M_PI 3.14159265358979323846

extern float nearest_enemy;
extern float new_yaw;
extern float new_pitch;

void CalculateNewAngles(Entity& Player, Entity& entity);
bool WorldToScreen(const Vec3& VecOrigin, Vec2& NDC, float* MVPMatrix);

bool WorldToScreenPlayer(const Vec3& VecOrigin, Vec2& NDC, float* MVPMatrix);

