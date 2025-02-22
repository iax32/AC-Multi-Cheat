#include "render.h"

// Helper function to convert const char* to LPCWSTR
LPCWSTR charToLPCWSTR(const char* charArray) {
	size_t len = strlen(charArray) + 1;
	wchar_t* wString = new wchar_t[len];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wString, len, charArray, _TRUNCATE);
	return wString;
}

// Function to create bitmap display lists for a font
GLuint createBitmapFont(HDC hdc, const char* fontName, int fontHeight)
{
	LPCWSTR wFontName = charToLPCWSTR(fontName); // Convert char* to LPCWSTR

	HFONT hFont = CreateFont(
		fontHeight, 0, 0, 0,
		FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		wFontName);

	SelectObject(hdc, hFont);
	GLuint listBase = glGenLists(96);
	wglUseFontBitmaps(hdc, 32, 96, listBase);
	DeleteObject(hFont);
	delete[] wFontName; // Clean up the dynamically allocated memory
	return listBase;
}

// Function to render text using the bitmap font
void RenderText(const char* text, GLuint listBase, float x, float y) 
{
	glRasterPos2f(x, y);
	glPushAttrib(GL_LIST_BIT);
	glListBase(listBase - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}



void DrawBox(Vec2 FeetCoords, Vec2 HeadCoords, float color[4])
{
	glColor4f(color[0], color[1], color[2], color[3]);
	float height = HeadCoords.Y - FeetCoords.Y;
	float width = height / 3.8f;

	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glVertex2f(FeetCoords.X - (width / 2), FeetCoords.Y);
	glVertex2f(FeetCoords.X - (width / 2), HeadCoords.Y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(FeetCoords.X - (width / 2), HeadCoords.Y);
	glVertex2f(FeetCoords.X + (width / 2), HeadCoords.Y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(FeetCoords.X + (width / 2), HeadCoords.Y);
	glVertex2f(FeetCoords.X + (width / 2), FeetCoords.Y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(FeetCoords.X + (width / 2), FeetCoords.Y);
	glVertex2f(FeetCoords.X - (width / 2), FeetCoords.Y);
	glEnd();

}

void DrawHealthBar(Vec2 FeetCoords, Vec2 HeadCoords, int health)
{
	float height = HeadCoords.Y - FeetCoords.Y;
	float width = height / 3.8f;
	float healthBarHeight = height * (health / 100.0f);

	glLineWidth(7.0f);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(FeetCoords.X - (width / 1.5), FeetCoords.Y);
	glVertex2f(FeetCoords.X - (width / 1.5), FeetCoords.Y + healthBarHeight);
	glEnd();

	if (health >= 60) {
		glColor3f(0, 1, 0); // Grün
	}
	else if (health >= 40) { // Gelb
		glColor3f(1, 1, 0);
	}
	else {
		glColor3f(1, 0, 0); // Rot
	}

	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glVertex2f(FeetCoords.X - (width / 1.5), FeetCoords.Y);
	glVertex2f(FeetCoords.X - (width / 1.5), FeetCoords.Y + healthBarHeight);
	glEnd();

}

void DrawCornerOutline(Vec2 FeetCoords, Vec2 HeadCoords, float color[4])
{
	glColor4f(color[0], color[1], color[2], color[3]);
	glLineWidth(2.0f);
	float height = HeadCoords.Y - FeetCoords.Y;
	float width = height / 3.8f;
	float lineLength = width / 4;

	glBegin(GL_LINES);
	// Oben Links
	glVertex2f(FeetCoords.X - (width / 2), HeadCoords.Y);
	glVertex2f(FeetCoords.X - (width / 2) + lineLength, HeadCoords.Y);
	glVertex2f(FeetCoords.X - (width / 2), HeadCoords.Y);
	glVertex2f(FeetCoords.X - (width / 2), HeadCoords.Y - lineLength);

	// Oben Rechts
	glVertex2f(FeetCoords.X + (width / 2), HeadCoords.Y);
	glVertex2f(FeetCoords.X + (width / 2) - lineLength, HeadCoords.Y);
	glVertex2f(FeetCoords.X + (width / 2), HeadCoords.Y);
	glVertex2f(FeetCoords.X + (width / 2), HeadCoords.Y - lineLength);

	// Unten Links
	glVertex2f(FeetCoords.X - (width / 2), FeetCoords.Y);
	glVertex2f(FeetCoords.X - (width / 2) + lineLength, FeetCoords.Y);
	glVertex2f(FeetCoords.X - (width / 2), FeetCoords.Y);
	glVertex2f(FeetCoords.X - (width / 2), FeetCoords.Y + lineLength);

	// Unten Rechts
	glVertex2f(FeetCoords.X + (width / 2), FeetCoords.Y);
	glVertex2f(FeetCoords.X + (width / 2) - lineLength, FeetCoords.Y);
	glVertex2f(FeetCoords.X + (width / 2), FeetCoords.Y);
	glVertex2f(FeetCoords.X + (width / 2), FeetCoords.Y + lineLength);
	glEnd();
}

void DrawArmorBar(Vec2 FeetCoords, Vec2 HeadCoords, int armor)
{
	float height = HeadCoords.Y - FeetCoords.Y;
	float width = height / 3.8f;

	float armorBarWidth = width * (armor / 100.0f);
	float armorBarYOffset = height * 0.05f;
	float topOfBoxY = HeadCoords.Y + armorBarYOffset;

	glLineWidth(5.0f);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(FeetCoords.X - (width / 2), topOfBoxY);
	glVertex2f(FeetCoords.X - (width / 2) + armorBarWidth, topOfBoxY);
	glEnd();

	glColor3f(0, 0.5f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex2f(FeetCoords.X - (width / 2), topOfBoxY);
	glVertex2f(FeetCoords.X - (width / 2) + armorBarWidth, topOfBoxY);
	glEnd();

}

void DrawSnapLines(Vec2 FeetCoords, float color[4])
{
	glColor4f(color[0], color[1], color[2], color[3]);
	glBegin(GL_LINES);
	glVertex2f(0.0f, -1.0f);
	glVertex2f(FeetCoords.X, FeetCoords.Y);
	glEnd();
}

void DrawLine(Vec2 Input, Vec2 Destination ,  float color[4])
{
	glColor4f(color[0], color[1], color[2], color[3]);
	glBegin(GL_LINES);
	glVertex2f(Input.X, Input.Y);
	glVertex2f(Destination.X, Destination.Y);
	glEnd();
}

void DrawBox3D(Vec2 screenCorners[8], float color[4]) {
	
	// Bottom Square
	DrawLine(screenCorners[0], screenCorners[1], color);
	DrawLine(screenCorners[1], screenCorners[2], color);
	DrawLine(screenCorners[2], screenCorners[3], color);
	DrawLine(screenCorners[3], screenCorners[0], color);

	// Top Square
	DrawLine(screenCorners[4], screenCorners[5], color);
	DrawLine(screenCorners[5], screenCorners[6], color);
	DrawLine(screenCorners[6], screenCorners[7], color);
	DrawLine(screenCorners[7], screenCorners[4], color);

	// Connecting Bottom to Top
	DrawLine(screenCorners[0], screenCorners[4], color);
	DrawLine(screenCorners[1], screenCorners[5], color);
	DrawLine(screenCorners[2], screenCorners[6], color);
	DrawLine(screenCorners[3], screenCorners[7], color);
	
}


void DrawMulitLine(Vec2* screenCorners, int length,float color[4])
{
	DrawLine(screenCorners[0], screenCorners[1], color);
	DrawLine(screenCorners[1], screenCorners[2], color);
	DrawLine(screenCorners[2], screenCorners[3], color);

}









