# Collision Configuration Guide

## 🎯 How to Adjust Collision Boxes

All collision boxes are now **fully parametric**! You can easily adjust them by changing values in `src/mainWindow.cpp` around **line 260**.

---

## 📦 Cube Collision Parameters

```cpp
struct {
    glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f);  // Center position
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);      // Base size
    glm::vec3 padding = glm::vec3(0.01f, 0.01f, 0.01f); // Extra padding
} cubeCollision;
```

### What Each Parameter Does:

**`position`** - Where the collision box center is located
- **X**: Left/Right position (0 = center)
- **Y**: Up/Down position (1 = 1 unit above ground)
- **Z**: Forward/Back position (0 = center)

**`size`** - Base dimensions of the collision box (should match mesh)
- **X**: Width (1.0 = 1 unit wide)
- **Y**: Height (1.0 = 1 unit tall)
- **Z**: Depth (1.0 = 1 unit deep)

**`padding`** - Extra space added on all sides
- **X**: Extra width on left and right
- **Y**: Extra height on top and bottom
- **Z**: Extra depth on front and back

### Examples:

**Make cube wider:**
```cpp
glm::vec3 size = glm::vec3(1.5f, 1.0f, 1.0f);  // 1.5 units wide
```

**Add more padding only on top/bottom:**
```cpp
glm::vec3 padding = glm::vec3(0.01f, 0.1f, 0.01f);  // More Y padding
```

**Move cube position:**
```cpp
glm::vec3 position = glm::vec3(2.0f, 1.0f, 0.0f);  // Moved 2 units to the right
```

---

## 🟫 Floor Collision Parameters

```cpp
struct {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);  // Center
    glm::vec3 size = glm::vec3(10.0f, 0.04f, 10.0f);   // Size
    glm::vec3 padding = glm::vec3(0.0f, 0.0f, 0.0f);   // Padding
} floorCollision;
```

### What Each Parameter Does:

**`position`** - Center of the floor plane
- Usually at (0, 0, 0) so it's at ground level

**`size`** - Dimensions of the floor
- **X**: Width of floor (10 = 10 units wide)
- **Y**: Thickness (0.04 = very thin, just a plane)
- **Z**: Depth of floor (10 = 10 units deep)

**`padding`** - Extra size in each direction
- Usually zero for floors

### Examples:

**Make floor bigger:**
```cpp
glm::vec3 size = glm::vec3(20.0f, 0.04f, 20.0f);  // 20×20 floor
```

**Make floor thinner:**
```cpp
glm::vec3 size = glm::vec3(10.0f, 0.02f, 10.0f);  // Even thinner
```

**Raise floor slightly:**
```cpp
glm::vec3 position = glm::vec3(0.0f, 0.1f, 0.0f);  // 0.1 units above ground
```

---

## 🧱 Wall Collision Parameters

```cpp
struct {
    float thickness = 0.05f;      // Thickness (X or Z)
    float height = 0.5f;          // Height (Y)
    float platformRadius = 5.0f;  // Distance from center
} wallCollision;
```

### What Each Parameter Does:

**`thickness`** - How thick each wall is
- Controls depth of wall (0.05 = thin invisible barrier)
- Increase for thicker walls: `0.1f`, `0.2f`, etc.

**`height`** - How tall the walls are
- Controls Y dimension (0.5 = knee-high)
- Increase for taller walls: `1.0f`, `2.0f`, etc.

**`platformRadius`** - Where walls are positioned
- Distance from center (0,0) to wall edge
- Should match floor size / 2
- If floor is 10×10, radius should be 5.0

### Examples:

**Taller walls:**
```cpp
float height = 2.0f;  // 2 units tall
```

**Thicker walls:**
```cpp
float thickness = 0.2f;  // Visible thick walls
```

**Walls further out (for bigger platform):**
```cpp
float platformRadius = 10.0f;  // Walls at 10 units from center
```

**Very short walls (ankle-high):**
```cpp
float height = 0.2f;  // Just a small barrier
```

---

## 🎨 Visual Testing

After changing any parameters:

1. **Rebuild**: `cmake --build build`
2. **Run**: `.\build\GameWindow.exe`
3. **Press G** to see collision boxes in green
4. **Verify** the sizes look correct

---

## 💡 Common Adjustments

### Make collision tighter (closer to mesh):
```cpp
glm::vec3 padding = glm::vec3(0.0f, 0.0f, 0.0f);  // No padding
```

### Make collision looser (more forgiving):
```cpp
glm::vec3 padding = glm::vec3(0.1f, 0.1f, 0.1f);  // Bigger padding
```

### Only pad horizontally (not vertically):
```cpp
glm::vec3 padding = glm::vec3(0.1f, 0.0f, 0.1f);  // X and Z only
```

### Remove walls completely:
```cpp
// Comment out the entire wall collision block
// Or set height to 0:
float height = 0.0f;
```

### Make invisible barriers (no floor collision):
```cpp
// Comment out the floor collision box creation
// Just keep walls
```

---

## 🚀 Adding New Collision Boxes

To add a new collision box (e.g., for a second cube):

```cpp
// Add new parameters
struct {
    glm::vec3 position = glm::vec3(3.0f, 1.0f, 0.0f);
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 padding = glm::vec3(0.01f, 0.01f, 0.01f);
} cube2Collision;

// Create the collision box
{
    glm::vec3 halfSize = (cube2Collision.size + cube2Collision.padding) * 0.5f;
    collisionMgr.addBox(AABB(
        cube2Collision.position - halfSize,
        cube2Collision.position + halfSize
    ));
}
```

---

## 📐 Quick Reference Table

| Parameter | Purpose | Typical Range | Default |
|-----------|---------|---------------|---------|
| **Cube Position** | Location in world | Any | (0, 1, 0) |
| **Cube Size** | Base dimensions | 0.1 - 10.0 | (1, 1, 1) |
| **Cube Padding** | Extra buffer | 0.0 - 0.2 | (0.01, 0.01, 0.01) |
| **Floor Size X** | Platform width | 5.0 - 50.0 | 10.0 |
| **Floor Size Y** | Platform thickness | 0.01 - 0.1 | 0.04 |
| **Floor Size Z** | Platform depth | 5.0 - 50.0 | 10.0 |
| **Wall Thickness** | How deep walls are | 0.01 - 0.5 | 0.05 |
| **Wall Height** | How tall walls are | 0.0 - 5.0 | 0.5 |
| **Platform Radius** | Wall position | Half of floor size | 5.0 |

---

## ✅ Benefits of Parametric System

✅ **Easy to adjust** - Change one number, see results  
✅ **Self-documenting** - Clear what each value does  
✅ **Consistent** - All boxes use same system  
✅ **Flexible** - Can scale in any direction (X, Y, Z)  
✅ **Maintainable** - Easy to add new collision boxes  

**Happy adjusting!** 🎛️
