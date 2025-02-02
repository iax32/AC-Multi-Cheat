#include "render.h"

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









