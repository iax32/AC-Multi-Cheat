# WorldToScreen, Aimbot, and Wallhack Documentation

## 11.2.13 Implementation of the Aimbot

### Define Constants and Global Variables
In `geometry.h`, the macro `M_PI` is defined to store the mathematical constant Pi (3.14159265358979323846). This macro is used later for converting radians to degrees.
```cpp
#define M_PI 3.14159265358979323846
```
Additionally, three `float` variables are declared as `extern` in `geometry.h`. These variables are defined globally so their values can be accessed and modified across different files:
```cpp
extern float nearest_enemy;
extern float new_yaw;
extern float new_pitch;
```
- `nearest_enemy`: Stores the distance to the nearest enemy, used to target the closest opponent.
- `new_yaw`: Holds the calculated yaw angle, which determines the horizontal aiming direction (rotation around the vertical axis).
- `new_pitch`: Holds the calculated pitch angle, which determines the vertical aiming direction (rotation around the horizontal axis).

### Function to Calculate Aiming Angles
A function named `CalculateNewAngles` is created in `geometry.cpp`. It takes references to a player structure and an enemy structure as parameters.
```cpp
void CalculateNewAngles(Entity& Player, Entity& entity);
```

### Calculate Direction Vector
The vector from the player to the enemy is computed by subtracting their coordinates:
```cpp
float vector_x = entity.Origin.X - Player.Origin.X;
float vector_y = entity.Origin.Y - Player.Origin.Y;
float vector_z = entity.Origin.Z - Player.Origin.Z;
```

### Compute Distance Using Hypotenuse Function
A function named `hypothenuse` is defined to compute the hypotenuse using the Pythagorean theorem.
```cpp
float hypothenuse(float a, float b) {
    return sqrtf(pow(a,2) + pow(b,2));
}
```
The distance to the enemy is then calculated:
```cpp
float distance = hypothenuse(vector_x, vector_y);
```

### Identify the Nearest Enemy
Check if `nearest_enemy` has its default value (`-1.0f`) or if the newly calculated `distance` is smaller than the current `nearest_enemy` value:
```cpp
if (nearest_enemy == -1.0f || distance < nearest_enemy) {
    nearest_enemy = distance;
}
```

### Calculate Yaw Angle
The yaw angle is calculated using `atan2` and converted from radians to degrees:
```cpp
float angle_xy = atan2f(vector_y, vector_x);
float yaw = (float)(angle_xy * (180.0 / M_PI));
new_yaw = yaw + 90;
```

### Calculate Pitch Angle
The pitch angle is calculated using `atan`:
```cpp
float angle_z = atanf(vector_z / hypothenuse(vector_x, vector_y));
new_pitch = (float)(angle_z * (180.0 / M_PI));
```

### Forward Declaration
To allow function usage across multiple files, a forward declaration is added in `geometry.h`:
```cpp
void CalculateNewAngles(Entity& Player, Entity& entity);
```

## 11.3 Development of the Wallhack
### WorldToScreen Function
The `WorldToScreen` function converts 3D world coordinates into 2D screen coordinates. It takes three parameters:
```cpp
bool WorldToScreen(const Vec3& VecOrigin, Vec2& NDC, float* MVPMatrix);
```

### Transformation Process
#### Convert World Coordinates to Homogeneous Coordinates
The MVP matrix is stored in a column-major format in OpenGL:
```
MVPMatrix = |  0   4   8  12 |
            |  1   5   9  13 |
            |  2   6  10  14 |
            |  3   7  11  15 |
```

#### Check Object Visibility
```cpp
if (homogeneCoords.W < 0.1f) {
    return false; // Object is behind the camera
}
```

#### Convert to Normalized Device Coordinates (NDC)
```cpp
NDC.X = homogeneCoords.X / homogeneCoords.W;
NDC.Y = homogeneCoords.Y / homogeneCoords.W;
```

#### Check Screen Bounds
```cpp
if (NDC.X < -1.0f || NDC.X > 1.0f || NDC.Y < -1.0f || NDC.Y > 1.0f) {
    return false; // Object is outside the field of view
}
```

#### Return Success
```cpp
return true;
```

### Forward Declaration
```cpp
bool WorldToScreen(const Vec3& VecOrigin, Vec2& NDC, float* MVPMatrix);
```

## Summary
This document covers the implementation of the `WorldToScreen` function, the Aimbot, and the Wallhack in a structured and formatted manner suitable for GitHub documentation. The Aimbot ensures that the nearest enemy is targeted by calculating yaw and pitch angles, while the Wallhack transforms 3D coordinates into a 2D space for visualization.



![Image](https://github.com/user-attachments/assets/e222ca09-cf74-4f78-b264-2ebffe465415)

![Image](https://github.com/user-attachments/assets/d873aa18-f8a3-4b58-bdfb-1ae7ce703dd6)
