#include "geometry.h"

float hypothenuse(float a, float b)
{
	return sqrtf(pow(a, 2) + pow(b, 2));
}

void CalculateNewAngles(Entity& Player, Entity& entity)
{
	float vector_x = entity.Origin.X - Player.Origin.X;
	float vector_y = entity.Origin.Y - Player.Origin.Y;
	float vector_z = entity.Origin.Z - Player.Origin.Z;

	float distance = hypothenuse(vector_x, vector_y);

	if (nearest_enemy == -1.0f || distance < nearest_enemy)
	{
		nearest_enemy = distance;

		float angle_xy = atan2f(vector_y, vector_x);
		float yaw = (float)(angle_xy * (180.0 / M_PI));
		new_yaw = yaw + 90;

		float angle_z = atanf(vector_z / hypothenuse(vector_x, vector_y));
		new_pitch = (float)(angle_z * (180.0 / M_PI));
	}

}

bool WorldToScreen(const Vec3& VecOrigin, Vec2& NDC, float* MVPMatrix)
{

	Vec4 homogeneCoords;
	homogeneCoords.X = 
		VecOrigin.X * MVPMatrix[0] +
		VecOrigin.Y * MVPMatrix[4] +
		VecOrigin.Z * MVPMatrix[8] +
		MVPMatrix[12];

	homogeneCoords.Y =
		VecOrigin.X * MVPMatrix[1] +
		VecOrigin.Y * MVPMatrix[5] +
		VecOrigin.Z * MVPMatrix[9] +
		MVPMatrix[13];

	homogeneCoords.W = 
		VecOrigin.X * MVPMatrix[3] +
		VecOrigin.Y * MVPMatrix[7] +
		VecOrigin.Z * MVPMatrix[11] +
		MVPMatrix[15];

	if (homogeneCoords.W < 0.1f)
	{
		return false; // Objekt ist hinter der Kamera
	}

	NDC.X = homogeneCoords.X / homogeneCoords.W;
	NDC.Y = homogeneCoords.Y / homogeneCoords.W;

	if (NDC.X < -1.0f || NDC.X > 1.0f ||
		NDC.Y < -1.0f || NDC.Y > 1.0f)
	{
		return false; // Objekt ist auﬂerhalb des Sichtfelds
	}

	return true;

}

