# Coordinate System & Gizmo Guide

## 🎯 What's New

You now have a complete 3D editing interface with:
1. **Coordinate Axes** - RGB axes showing X, Y, Z directions
2. **Camera Position Display** - Real-time coordinates in window title
3. **Translation Gizmo** - RGB arrows for moving the cube
4. **Object Selection** - Cube is selected by default

---

## 🌐 Coordinate Axes (Always Visible)

### Visual Representation
- **Red Line** = X axis (left/right)
- **Green Line** = Y axis (up/down)  
- **Blue Line** = Z axis (forward/back)

### Purpose
- Shows world orientation
- Helps understand 3D space
- Always rendered from origin (0, 0, 0)
- Extends 100 units in each direction

---

## 📍 Camera Position Display

### Location
The **window title** shows your camera coordinates in real-time:
```
Ray Tracer | Camera: X=0.0 Y=2.0 Z=8.0
```

### Format
- Updates every frame
- Shows X, Y, Z with 1 decimal place
- Helps track your position in the scene

---

## 🎮 Translation Gizmo

### What It Is
When an object is selected (cube by default), you'll see 3 colored arrows:
- **Red Arrow** = Move along X axis
- **Green Arrow** = Move along Y axis
- **Blue Arrow** = Move along Z axis

### Current Status
- **Gizmo is visible** on the cube
- Shows at cube's current position: (0, 1, 0)
- **Note**: Mouse interaction for dragging is not yet implemented
- For now, the gizmo is visual only

###To Move the Cube (Manual Method)
Currently, you can change the cube position by editing code:
```cpp
// In mainWindow.cpp, around line 42
glm::vec3 cubePosition = glm::vec3(2.0f, 1.0f, 0.0f);  // Change these values
```

Then rebuild: `cmake --build build`

---

## 🎨 Visual Guide

### Color Scheme
| Element | Color | Meaning |
|---------|-------|---------|
| X Axis / Arrow | 🔴 Red | Horizontal (left/right) |
| Y Axis / Arrow | 🟢 Green | Vertical (up/down) |
| Z Axis / Arrow | 🔵 Blue | Depth (forward/back) |
| Platform |Gray | Ground plane |
| Cube | Cyan | Selectable object |
| Collision Boxes | 🟢 Bright Green | When G is pressed |

---

##🛠️ Current Features

✅ **Coordinate axes always visible**
✅ **Camera position shown in title**  
✅ **Translation gizmo renders on selected object**
✅ **Cube is selected by default**
✅ **Gizmo follows cube position**
✅ **RGB color coding (industry standard)**

---

## 🚧 Next Steps (To Be Implemented)

The following features would complete the gizmo system:

### 1. Mouse Picking
- Click on gizmo arrows
- Detect which arrow is clicked
- Start drag operation

### 2. Drag Interaction
- Hold mouse button on arrow
- Move mouse to drag object
- Constrain movement to selected axis

### 3. Visual Feedback
- Highlight arrow on hover
- Change color when being dragged
- Show snap grid (optional)

### 4. Keyboard Shortcuts
- **T** - Toggle translation gizmo
- **ESC** - Deselect object
- **Delete** - Remove selected object (if implemented)

---

## 💡 Usage Tips

### Understanding the Axes
1. **Start at center** - Axes meet at (0, 0, 0)
2. **Look for colors** - RGB = XYZ (easy to remember!)
3. **Camera moves** - Axes stay fixed in world

### Reading Coordinates
1. **Window title updates** - Watch it as you move
2. **Positive values** - Right (X+), Up (Y+), Back (Z+)
3. **Negative values** - Left (X-), Down (Y-), Forward (Z-)

### Using the Gizmo (Visual)
1. **Find the arrows** - They're at the cube position
2. **Identify direction** - Red=X, Green=Y, Blue=Z
3. **Plan movement** - Which direction do you want to move?

---

##⚙️ Technical Details

### Rendering Order
1. Scene objects (cube, platform)
2. Coordinate axes (2px wide lines)
3. Translation gizmo (3px wide lines, if object selected)
4. Collision boxes (1px lines, if G pressed)

### Performance
- **Minimal overhead** - Just line rendering
- **60 FPS** - No performance impact
- **Dynamic updates** - Gizmo follows object every frame

### Line Rendering
- Uses same `lineShader` as collision boxes
- GL_LINES primitive type
- Different line widths for visual hierarchy

---

## 🎯 Configuration

### Adjust Axes Length
In `mainWindow.cpp`, around line 373:
```cpp
auto axesVerts = generateCoordinateAxes(100.0f);  // Change length here
```

### Adjust Gizmo Size
Around line 389:
```cpp
auto gizmoArrows = generateTranslationGizmo(1.5f, 0.05f);  // Length and thickness
```

### Default Cube Position
Around line 42:
```cpp
glm::vec3 cubePosition = glm::vec3(0.0f, 1.0f, 0.0f);  // X, Y, Z
```

### Selection State
Around line 43:
```cpp
bool cubeSelected = true;  // Set to false to hide gizmo
```

---

## 📊 Coordinate System Reference

### World Space
- **Origin**: (0, 0, 0) - Center of platform
- **X Axis**: -∞ to +∞ (left to right)
- **Y Axis**: 0 to +∞ (ground to sky)
- **Z Axis**: -∞ to +∞ (forward to back)

### Default Positions
- **Cube**: (0, 1, 0) - Center, 1 unit above ground
- **Camera**: (0, 2, 8) - Above ground, looking at cube
- **Platform**: (0, 0, 0) - At ground level, 10×10 size

---

## 🎮 Controls Summary

**Camera Movement:**
- RIGHT MOUSE + WASD - Move around
- RIGHT MOUSE + Q/E - Up/down
- SHIFT - Sprint (4x speed)

**View Options:**
- **G** - Toggle collision boxes
- **ESC** - Exit

**Information:**
- Window title - Camera coordinates (always visible)

---

## ✨ Benefits

✅ **Better spatial awareness** - See orientation clearly
✅ **Professional appearance** - Industry-standard RGB axes
✅ **Real-time feedback** - Know your position instantly
✅ **Object manipulation ready** - Gizmo system in place
✅ **Visual debugging** - Combine with G key for full view

---

**Your scene now has professional 3D editing visualization!** 🎨

The coordinate system and gizmo provide the foundation for object manipulation.
Mouse interaction can be added next to make it fully interactive!
