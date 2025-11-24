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
    std::cout << "  ESC        - Exit\n";
    std::cout << "Collision detection: ENABLED\n";
    std::cout << "=======================\n\n";

    // -----------------------------
    // Create Shaders
    // -----------------------------
    std::cout << "Loading shaders...\n";
    Shader cubeShader("src/cube.vert", "src/cube.frag");
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
    
    // Cube collision box (positioned at 0, 1, 0 with size 1x1x1)
    collisionMgr.addBox(AABB(
        glm::vec3(-0.5f, 0.5f, -0.5f),  // min
        glm::vec3(0.5f, 1.5f, 0.5f)     // max
    ));

    // Platform collision - floor (prevent going below y=0.3 which is camera eye height)
    collisionMgr.addBox(AABB(
        glm::vec3(-5.0f, -1.0f, -5.0f),
        glm::vec3(5.0f, 0.3f, 5.0f)
    ));

    // Platform collision - walls around the edges (invisible boundaries)
    // North wall
    collisionMgr.addBox(AABB(
        glm::vec3(-5.0f, 0.0f, -5.5f),
        glm::vec3(5.0f, 5.0f, -5.0f)
    ));
    // South wall
    collisionMgr.addBox(AABB(
        glm::vec3(-5.0f, 0.0f, 5.0f),
        glm::vec3(5.0f, 5.0f, 5.5f)
    ));
    // West wall
    collisionMgr.addBox(AABB(
        glm::vec3(-5.5f, 0.0f, -5.0f),
        glm::vec3(-5.0f, 5.0f, 5.0f)
    ));
    // East wall
    collisionMgr.addBox(AABB(
        glm::vec3(5.0f, 0.0f, -5.0f),
        glm::vec3(5.5f, 5.0f, 5.0f)
    ));

    std::cout << "Collision system initialized with " << collisionMgr.boxes.size() << " collision boxes!\n";

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &platformVAO);
    glDeleteBuffers(1, &platformVBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
