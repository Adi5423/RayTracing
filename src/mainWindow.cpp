#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

// Include GLM for camera math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include Camera controller
#include "Camera.hpp"

// Include Shader utility
#include "Shader.hpp"

// Include Collision detection
#include "Collision.hpp"

// Include RelNo_D1
#include "Noise.hpp"

using namespace Noise;

// -----------------------------
// Global state
// -----------------------------
Camera camera(glm::vec3(0.0f, 2.0f, 8.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool rightMousePressed = false;
CollisionManager collisionMgr;
bool showCollisionBoxes = false;
bool gKeyPressed = false;

// -----------------------------
// Callbacks
// -----------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.processMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos), rightMousePressed);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(static_cast<float>(yoffset));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMousePressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (action == GLFW_RELEASE) {
            rightMousePressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Toggle collision box visualization with G key
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (!gKeyPressed) {
            showCollisionBoxes = !showCollisionBoxes;
            gKeyPressed = true;
            std::cout << "Collision boxes: " << (showCollisionBoxes ? "VISIBLE" : "HIDDEN") << "\n";
        }
    } else {
        gKeyPressed = false;
    }

    // Camera movement with collision detection
    camera.processKeyboard(window, deltaTime, &collisionMgr);
}

// -----------------------------
// Main
// -----------------------------
int main() 
{
    // -----------------------------
    // Init GLFW
    // -----------------------------
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        1280, 720, 
        "Ray Tracer - Cube on Platform", 
        nullptr, nullptr
    );

    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // -----------------------------
    // Init GLAD
    // -----------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return -1;
    }

    // Enable depth testing for 3D
    glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "\n=== Camera Controls ===\n";
    std::cout << "Hold RIGHT MOUSE BUTTON to activate camera\n";
    std::cout << "  WASD       - Move forward/left/back/right\n";
    std::cout << "  Q/E        - Move up/down\n";
    std::cout << "  SHIFT      - Sprint (4x speed)\n";
    std::cout << "  Mouse Move - Look around\n";
    std::cout << "  G          - Toggle collision box visualization\n";
    std::cout << "  ESC        - Exit\n";
    std::cout << "Collision detection: ENABLED\n";
    std::cout << "=======================\n\n";

    // -----------------------------
    // Create Shaders
    // -----------------------------
    std::cout << "Loading shaders...\n";
    Shader cubeShader("src/cube.vert", "src/cube.frag");
    Shader lineShader("src/line.vert", "src/line.frag");
    std::cout << "Shaders loaded successfully!\n";

    // -----------------------------
    // Cube vertex data (position + normals)
    // -----------------------------
    float cubeVertices[] = {
        // positions          // normals
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        // Right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // Platform vertices (a flat plane)
    float platformVertices[] = {
        // positions          // normals
        -5.0f, 0.0f, -5.0f,  0.0f,  1.0f,  0.0f,
         5.0f, 0.0f, -5.0f,  0.0f,  1.0f,  0.0f,
         5.0f, 0.0f,  5.0f,  0.0f,  1.0f,  0.0f,
         5.0f, 0.0f,  5.0f,  0.0f,  1.0f,  0.0f,
        -5.0f, 0.0f,  5.0f,  0.0f,  1.0f,  0.0f,
        -5.0f, 0.0f, -5.0f,  0.0f,  1.0f,  0.0f
    };

    // -----------------------------
    // Setup Cube VAO and VBO
    // -----------------------------
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // -----------------------------
    // Setup Platform VAO and VBO
    // -----------------------------
    unsigned int platformVAO, platformVBO;
    glGenVertexArrays(1, &platformVAO);
    glGenBuffers(1, &platformVBO);

    glBindVertexArray(platformVAO);
    glBindBuffer(GL_ARRAY_BUFFER, platformVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(platformVertices), platformVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::cout << "Cube and platform geometry created!\n";

    // -----------------------------
    // Setup Collision Boxes
    // -----------------------------
    collisionMgr.clear();
    
    // ==============================
    // COLLISION CONFIGURATION
    // Adjust these parameters to fine-tune collision boxes
    // ==============================
    
    // --- CUBE COLLISION PARAMETERS ---
    struct {
        glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f);  // Center position
        glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);      // Base size (matches mesh)
        glm::vec3 padding = glm::vec3(0.01f, 0.01f, 0.01f); // Extra padding per axis
    } cubeCollision;
    
    // --- PLATFORM FLOOR COLLISION PARAMETERS ---
    struct {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);  // Center of platform
        glm::vec3 size = glm::vec3(10.0f, 0.04f, 10.0f);   // Size: 10×10 in XZ, very thin in Y
        glm::vec3 padding = glm::vec3(0.0f, 0.0f, 0.0f);   // No extra padding needed
    } floorCollision;
    
    // --- PLATFORM WALLS COLLISION PARAMETERS ---
    struct {
        float thickness = 0.05f;      // How thick the walls are (Z or X direction)
        float height = 0.1f;          // How tall the walls are (Y direction)
        float platformRadius = 5.0f;  // Distance from center to edge
    } wallCollision;
    
    // ==============================
    // BUILD COLLISION BOXES FROM PARAMETERS
    // ==============================
    
    // CUBE collision box
    {
        glm::vec3 halfSize = (cubeCollision.size + cubeCollision.padding) * 0.5f;
        collisionMgr.addBox(AABB(
            cubeCollision.position - halfSize,  // min
            cubeCollision.position + halfSize   // max
        ));
    }
    
    // FLOOR collision box
    {
        glm::vec3 halfSize = (floorCollision.size + floorCollision.padding) * 0.5f;
        collisionMgr.addBox(AABB(
            floorCollision.position - halfSize,  // min
            floorCollision.position + halfSize   // max
        ));
    }
    
    // WALL collision boxes (4 walls around platform)
    {
        float r = wallCollision.platformRadius;
        float t = wallCollision.thickness;
        float h = wallCollision.height;
        
        // North wall (at -Z edge)
        collisionMgr.addBox(AABB(
            glm::vec3(-r, 0.0f, -r - t),
            glm::vec3(r, h, -r)
        ));
        
        // South wall (at +Z edge)
        collisionMgr.addBox(AABB(
            glm::vec3(-r, 0.0f, r),
            glm::vec3(r, h, r + t)
        ));
        
        // West wall (at -X edge)
        collisionMgr.addBox(AABB(
            glm::vec3(-r - t, 0.0f, -r),
            glm::vec3(-r, h, r)
        ));
        
        // East wall (at +X edge)
        collisionMgr.addBox(AABB(
            glm::vec3(r, 0.0f, -r),
            glm::vec3(r + t, h, r)
        ));
    }

    std::cout << "Collision system initialized with " << collisionMgr.boxes.size() << " collision boxes!\n";

    // -----------------------------
    // Setup Collision Box Visualization VAO/VBO
    // -----------------------------
    unsigned int collisionVAO, collisionVBO;
    glGenVertexArrays(1, &collisionVAO);
    glGenBuffers(1, &collisionVBO);

    glBindVertexArray(collisionVAO);
    glBindBuffer(GL_ARRAY_BUFFER, collisionVBO);
    
    // We'll update this buffer dynamically when rendering
    // Allocate enough space for wireframe lines
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 1000, nullptr, GL_DYNAMIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // -----------------------------
    // Generate a Perlin Noise map
    // -----------------------------
    std::cout << "Generating Perlin noise using RelNo_D1...\n";

    auto noiseMap = create_perlinnoise(
        256, 256,
        40.0f,     // scale
        4,         // octaves
        1.0f,      // frequency
        0.5f,      // persistence
        2.0f,      // lacunarity
        0.0f,      // base offset
        21,        // seed
        OutputMode::None,
        ""
    );

    std::cout << "Noise generated! Sample value = "
              << noiseMap[100][100] << "\n";

    // Light properties
    glm::vec3 lightPos(3.0f, 5.0f, 3.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    // -----------------------------
    // Main Loop
    // -----------------------------
    std::cout << "\nRendering cube on platform! Use camera controls to explore.\n";

    while (!glfwWindowShouldClose(window)) 
    {
        // Calculate delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        processInput(window);

        // Render
        glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        cubeShader.use();

        // Set view and projection matrices
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix(1280.0f / 720.0f);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);

        // Set lighting uniforms
        cubeShader.setVec3("lightPos", lightPos);
        cubeShader.setVec3("viewPos", camera.position);
        cubeShader.setVec3("lightColor", lightColor);

        // -----------------------------
        // Render the Cube
        // -----------------------------
        glBindVertexArray(cubeVAO);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // Position cube above platform
        cubeShader.setMat4("model", model);
        cubeShader.setVec3("objectColor", glm::vec3(0.3f, 0.7f, 0.9f)); // Cyan-ish color
        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // -----------------------------
        // Render the Platform
        // -----------------------------
        glBindVertexArray(platformVAO);
        
        model = glm::mat4(1.0f);
        // Platform is already at y=0, no translation needed
        cubeShader.setMat4("model", model);
        cubeShader.setVec3("objectColor", glm::vec3(0.5f, 0.5f, 0.5f)); // Gray platform
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // -----------------------------
        // Render Collision Boxes (if enabled)
        // -----------------------------
        if (showCollisionBoxes) {
            // Get wireframe vertices from collision manager
            auto wireframeVerts = collisionMgr.getWireframeVertices();
            
            if (!wireframeVerts.empty()) {
                // Use line shader
                lineShader.use();
                lineShader.setMat4("view", view);
                lineShader.setMat4("projection", projection);
                lineShader.setVec3("lineColor", glm::vec3(0.0f, 1.0f, 0.0f)); // Bright green
                
                // Update VBO with wireframe data
                glBindBuffer(GL_ARRAY_BUFFER, collisionVBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, wireframeVerts.size() * sizeof(glm::vec3), wireframeVerts.data());
                
                // Draw lines
                glBindVertexArray(collisionVAO);
                glDrawArrays(GL_LINES, 0, wireframeVerts.size());
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &platformVAO);
    glDeleteBuffers(1, &platformVBO);
    glDeleteVertexArrays(1, &collisionVAO);
    glDeleteBuffers(1, &collisionVBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
