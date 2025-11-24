# Rendering Guide - Cube on Platform

## 🎉 What You Now Have

You now have a **fully functional 3D rendering application** showing:
- **A cyan cube** floating above a platform
- **A gray platform** (ground plane)
- **Phong lighting** with ambient, diffuse, and specular components
- **Interactive camera controls** for exploring the scene

---

## 🎨 Scene Details

### Objects Rendered

1. **Cube**
   - Position: `(0, 1, 0)` - floating 1 unit above the platform
   - Color: Cyan/Blue `(0.3, 0.7, 0.9)`
   - Size: 1x1x1 units

2. **Platform**
   - Position: `(0, 0, 0)` - ground level
   - Color: Gray `(0.5, 0.5, 0.5)`
   - Size: 10x10 units (extends from -5 to +5 in X and Z)

3. **Light Source**
   - Position: `(3, 5, 3)`
   - Color: White `(1, 1, 1)`
   - Type: Point light with Phong shading model

---

## 🎮 Camera Controls

The camera starts at position `(0, 2, 8)` looking at the scene.

**Hold RIGHT MOUSE BUTTON** to activate camera movement:
- **W/A/S/D** - Move forward/left/back/right
- **Q/E** - Move up/down
- **SHIFT** - Sprint mode (2x speed)
- **Mouse** - Look around
- **Scroll Wheel** - Zoom in/out (Field of View)
- **ESC** - Exit application

---

## 🔧 Technical Implementation

### Shaders
- **Vertex Shader**: `src/cube.vert`
  - Transforms vertices from model space to clip space
  - Passes normals and fragment positions to fragment shader
  
- **Fragment Shader**: `src/cube.frag`
  - Implements Phong lighting model
  - Calculates ambient, diffuse, and specular lighting
  - Outputs final colored pixel

### Classes
- **Camera.hpp**: First-person camera controller
- **Shader.hpp**: Shader loading and uniform management utility

### Main Components
- **VAO/VBO** for cube geometry (36 vertices, 6 faces)
- **VAO/VBO** for platform geometry (6 vertices, 2 triangles)
- **Vertex attributes**: Position (3 floats) + Normal (3 floats)

---

## 🚀 Building and Running

```bash
# Build the project
cmake --build build

# Run the application
.\build\GameWindow.exe
```

---

## 🎯 Next Steps

Now that you have basic rendering working, you can:
1. **Add more cubes** - Create multiple cubes at different positions
2. **Add colors** - Experiment with different object colors
3. **Add textures** - Apply textures to the cube faces
4. **Implement ray tracing** - Replace rasterization with ray tracing
5. **Add shadows** - Implement shadow mapping
6. **Add reflections** - Make the platform reflective
7. **Animate the cube** - Make it rotate or move

---

## 📝 Key Files

- `src/mainWindow.cpp` - Main application and rendering loop
- `src/Camera.hpp` - Camera controller
- `src/Shader.hpp` - Shader utility class
- `src/cube.vert` - Vertex shader (GLSL)
- `src/cube.frag` - Fragment shader (GLSL)

---

**Enjoy exploring your 3D scene!** 🎮✨
