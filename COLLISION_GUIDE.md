# Collision Detection System

## 🎯 Overview

The collision detection system prevents the camera from passing through 3D objects in the scene. It uses **sphere-AABB (Axis-Aligned Bounding Box)** collision detection for efficient and realistic collision handling.

---

## 🔧 How It Works

### Camera as a Sphere
- The camera is represented as a **sphere** with a radius of **0.3 units**
- This creates a "personal space" around the camera that can't overlap with objects

### Objects as AABBs
- Each object (cube, platform walls, floor) has an **Axis-Aligned Bounding Box**
- AABBs are defined by two points: `min` (corner with smallest coordinates) and `max` (corner with largest coordinates)

### Collision Detection Algorithm
1. **Before Movement**: Store current camera position
2. **Calculate New Position**: Apply movement input (WASD, Q/E)
3. **Check Collision**: See if the camera sphere at new position intersects any AABB
4. **Resolve Collision**: 
   - If no collision → move to new position
   - If collision → try moving on individual axes (X, Y, Z separately)
   - This allows "sliding" along walls instead of stopping completely
5. **Update Position**: Apply the validated position

---

## 📦 Current Collision Boxes

### 1. Cube
- **Position**: Center at (0, 1, 0)
- **Size**: 1×1×1 units
- **AABB**: 
  - Min: (-0.5, 0.5, -0.5)
  - Max: (0.5, 1.5, 0.5)

### 2. Floor
- **Purpose**: Prevents camera from going underground
- **Height**: Camera can't go below y = 0.3 (eye height)
- **AABB**:
  - Min: (-5.0, -1.0, -5.0)
  - Max: (5.0, 0.3, 5.0)

### 3. Boundary Walls (Invisible)
Four walls surround the platform to keep the camera within the scene:

**North Wall** (negative Z):
- Min: (-5.0, 0.0, -5.5)
- Max: (5.0, 5.0, -5.0)

**South Wall** (positive Z):
- Min: (-5.0, 0.0, 5.0)
- Max: (5.0, 5.0, 5.5)

**West Wall** (negative X):
- Min: (-5.5, 0.0, -5.0)
- Max: (-5.0, 5.0, 5.0)

**East Wall** (positive X):
- Min: (5.0, 0.0, -5.0)
- Max: (5.5, 5.0, 5.0)

**Total**: 6 collision boxes in the scene

---

## 🎮 User Experience

### What You'll Notice
✅ **Can't walk through the cube** - You'll bump into it
✅ **Can't fall through the platform** - There's a "floor" at y = 0.3
✅ **Can't leave the platform area** - Invisible walls keep you in bounds
✅ **Smooth sliding** - When you hit a wall at an angle, you slide along it
✅ **Sprint is faster** - Shift now gives 4× speed (was 2×)

---

## 🔨 Technical Implementation

### Files
- **`src/Collision.hpp`** - Core collision detection system
- **`src/Camera.hpp`** - Camera with collision awareness
- **`src/mainWindow.cpp`** - Scene setup and collision box registration

### Key Classes

#### `AABB` - Axis-Aligned Bounding Box
```cpp
struct AABB {
    glm::vec3 min;  // Corner with smallest x,y,z
    glm::vec3 max;  // Corner with largest x,y,z
};
```

#### `Sphere` - Camera Collider
```cpp
struct Sphere {
    glm::vec3 center;  // Camera position
    float radius;       // Collision radius (0.3)
};
```

#### `CollisionManager` - Manages All Collisions
```cpp
class CollisionManager {
    std::vector<AABB> boxes;
    
    void addBox(const AABB& box);
    bool checkCollision(const Sphere& sphere);
    glm::vec3 resolveCollision(oldPos, newPos, radius);
};
```

---

## 📝 Adding New Collision Objects

To add a new collidable object to your scene:

```cpp
// In mainWindow.cpp, after creating the collision boxes:
collisionMgr.addBox(AABB(
    glm::vec3(minX, minY, minZ),  // Minimum corner
    glm::vec3(maxX, maxY, maxZ)   // Maximum corner
));
```

**Example**: Add another cube at position (3, 1, 0):
```cpp
collisionMgr.addBox(AABB(
    glm::vec3(2.5f, 0.5f, -0.5f),   // min
    glm::vec3(3.5f, 1.5f, 0.5f)     // max
));
```

---

## ⚡ Performance Notes

- **Sphere-AABB collision** is very fast (O(1) per check)
- With 6 collision boxes, performance impact is negligible
- Can easily handle 100+ boxes without issues
- Collision resolution uses axis-separated testing for smooth sliding

---

## 🚀 Future Enhancements

Possible improvements for the collision system:

1. **Dynamic Objects** - Moving platforms, rotating cubes
2. **Gravity** - Pull camera down to floor automatically
3. **Jumping** - Allow vertical boost with collision checking
4. **Complex Shapes** - Support for spheres, cylinders, meshes
5. **Spatial Partitioning** - For scenes with many objects
6. **Collision Events** - Trigger actions when touching objects

---

## 🎯 Summary

✅ **Camera collision radius**: 0.3 units  
✅ **Sprint multiplier**: 4× (was 2×)  
✅ **Total collision boxes**: 6 (1 cube, 1 floor, 4 walls)  
✅ **Collision algorithm**: Sphere-AABB with axis-separated resolution  
✅ **Result**: Smooth, realistic movement that respects object boundaries  

**Try it out!** Walk up to the cube and feel how you can't pass through it. Try to walk off the edge of the platform and notice the invisible walls keeping you safe!
