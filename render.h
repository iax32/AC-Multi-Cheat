#pragma once
#include "datatypes.h"
#include <GLFW/glfw3.h>

void DrawBox(Vec2 FeetCoords, Vec2 HeadCoords, float color[4]);
void DrawSnapLines(Vec2 FeetCoords, float color[4]);
void DrawCornerOutline(Vec2 FeetCoords, Vec2 HeadCoords, float color[4]);
void DrawHealthBar(Vec2 FeetCoords, Vec2 HeadCoords, int health);
void DrawArmorBar(Vec2 FeetCoords, Vec2 HeadCoords, int armor);
void DrawLine(Vec2 Input, Vec2 Destination, float color[4]);
void DrawBox3D(Vec2 screenCorners[8], float color[4]);
void DrawMulitLine(Vec2* screenCorners, int length, float color[4]);
void RenderText(const char* text, GLuint listBase, float x, float y);
GLuint createBitmapFont(HDC hdc, const char* fontName, int fontHeight);

