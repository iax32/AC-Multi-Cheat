#pragma once
#include "datatypes.h"
#include <GLFW/glfw3.h>

void DrawBox(Vec2 FeetCoords, Vec2 HeadCoords, float color[4]);
void DrawSnapLines(Vec2 FeetCoords, float color[4]);
void DrawCornerOutline(Vec2 FeetCoords, Vec2 HeadCoords, float color[4]);
void DrawHealthBar(Vec2 FeetCoords, Vec2 HeadCoords, int health);
void DrawArmorBar(Vec2 FeetCoords, Vec2 HeadCoords, int armor);

