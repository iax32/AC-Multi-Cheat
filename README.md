# ac_multi_cheat

External Multi Cheat in Assault Cube

# WorldToScreen Function Documentation

## Overview
The `WorldToScreen` function converts 3D world coordinates into 2D screen coordinates. It takes three parameters:

1. **VecOrigin**: A `Vec3` vector representing the world position of the object or player to be converted. This is passed by reference to avoid unnecessary copies.
2. **NDC**: A `Vec2` vector passed by reference, which stores the computed Normalized Device Coordinates (NDC), representing the object's position in the 2D screen space.
3. **MVPMatrix**: A pointer to a `4x4` matrix containing the Model-View-Projection (MVP) matrix. This matrix transforms the world position into homogeneous coordinates and then into screen coordinates.

The function returns a boolean value:
- `true` if the transformation was successful and the object is visible.
- `false` if the object is outside the visible area or behind the camera.

### Function Signature
```cpp
bool WorldToScreen(const Vec3& VecOrigin, Vec2& NDC, float* MVPMatrix);
```

## Transformation Process

### Step 1: Convert World Coordinates to Homogeneous Coordinates
The world position (`VecOrigin`) is multiplied by the MVP matrix. The MVP matrix is stored in OpenGL in a **column-major format**:

```
MVPMatrix = |  0   4   8  12 |
            |  1   5   9  13 |
            |  2   6  10  14 |
            |  3   7  11  15 |
```

- The **first column** `(0,1,2,3)` affects the X transformation.
- The **second column** `(4,5,6,7)` affects the Y transformation.
- The **third column** `(8,9,10,11)` affects the Z transformation.
- The **fourth column** `(12,13,14,15)` applies translation.

The homogeneous coordinates are calculated as follows:

```cpp
Vec4 homogeneCoords;
homogeneCoords.X = VecOrigin.X * MVPMatrix[0] + VecOrigin.Y * MVPMatrix[4] + VecOrigin.Z * MVPMatrix[8] + MVPMatrix[12];
homogeneCoords.Y = VecOrigin.X * MVPMatrix[1] + VecOrigin.Y * MVPMatrix[5] + VecOrigin.Z * MVPMatrix[9] + MVPMatrix[13];
homogeneCoords.W = VecOrigin.X * MVPMatrix[3] + VecOrigin.Y * MVPMatrix[7] + VecOrigin.Z * MVPMatrix[11] + MVPMatrix[15];
```

The **Z-coordinate** is not calculated, as it is not needed for screen coordinate conversion. Instead, it is used for depth information storage.

### Step 2: Visibility Check
Before converting the homogeneous coordinates into screen coordinates, the function checks if the object is visible.

```cpp
if (homogeneCoords.W < 0.1f) {
    return false; // Object is behind the camera
}
```

### Step 3: Convert to Normalized Device Coordinates (NDC)
If the object is visible, its **X and Y** values are divided by the **W** component to obtain **Normalized Device Coordinates (NDC)**.

```cpp
NDC.X = homogeneCoords.X / homogeneCoords.W;
NDC.Y = homogeneCoords.Y / homogeneCoords.W;
```

NDC values range from **-1 to 1**, representing the object's position in the field of view.

### Step 4: Screen Bounds Check
If the NDC values are outside the range `[-1, 1]`, the object is outside the screen view.

```cpp
if (NDC.X < -1.0f || NDC.X > 1.0f || NDC.Y < -1.0f || NDC.Y > 1.0f) {
    return false; // Object is outside the field of view
}
```

### Step 5: Return Success
If all checks pass, the function returns `true`, allowing the NDC values to be used for rendering elements like ESP boxes.

```cpp
return true;
```

## Forward Declaration
To use the function in other files, add the following forward declaration in `geometry.h`:

```cpp
bool WorldToScreen(const Vec3& VecOrigin, Vec2& NDC, float* MVPMatrix);
```

## Summary
The `WorldToScreen` function is crucial for converting 3D game objects into 2D screen space. It ensures objects are visible before processing their coordinates and can be used for rendering overlays or UI elements.



![Image](https://github.com/user-attachments/assets/e222ca09-cf74-4f78-b264-2ebffe465415)

![Image](https://github.com/user-attachments/assets/d873aa18-f8a3-4b58-bdfb-1ae7ce703dd6)
