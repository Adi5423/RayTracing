// Gizmo.hpp
// ---------------------------------------------------------
// 3D Gizmo system for object manipulation
// Includes coordinate axes and translation gizmo
// ---------------------------------------------------------

#pragma once

#include <glm/glm.hpp>
#include <vector>

// Generate coordinate axes geometry (lines)
inline std::vector<glm::vec3> generateCoordinateAxes(float length = 2.0f) {
    std::vector<glm::vec3> vertices;
    
    // X axis - Red (will be colored in shader)
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3(length, 0.0f, 0.0f));
    
    // Y axis - Green
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3(0.0f, length, 0.0f));
    
    // Z axis - Blue
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3(0.0f, 0.0f, length));
    
    return vertices;
}

// Generate translation gizmo arrows
struct GizmoArrow {
    std::vector<glm::vec3> vertices;
    glm::vec3 color;
    glm::vec3 axis;
};

inline std::vector<GizmoArrow> generateTranslationGizmo(float length = 1.0f, float thickness = 0.02f) {
    std::vector<GizmoArrow> arrows;
    
    // X axis arrow (Red)
    {
        GizmoArrow arrow;
        arrow.color = glm::vec3(1.0f, 0.0f, 0.0f);
        arrow.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        
        // Shaft (line)
        arrow.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        arrow.vertices.push_back(glm::vec3(length, 0.0f, 0.0f));
        
        arrows.push_back(arrow);
    }
    
    // Y axis arrow (Green)
    {
        GizmoArrow arrow;
        arrow.color = glm::vec3(0.0f, 1.0f, 0.0f);
        arrow.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        
        // Shaft (line)
        arrow.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        arrow.vertices.push_back(glm::vec3(0.0f, length, 0.0f));
        
        arrows.push_back(arrow);
    }
    
    // Z axis arrow (Blue)
    {
        GizmoArrow arrow;
        arrow.color = glm::vec3(0.0f, 0.0f, 1.0f);
        arrow.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        
        // Shaft (line)
        arrow.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        arrow.vertices.push_back(glm::vec3(0.0f, 0.0f, length));
        
        arrows.push_back(arrow);
    }
    
    return arrows;
}

// Simple gizmo interaction state
struct GizmoState {
    bool active = false;
    int selectedAxis = -1; // -1 = none, 0 = X, 1 = Y, 2 = Z
    glm::vec3 dragStartPos = glm::vec3(0.0f);
    glm::vec3 objectStartPos = glm::vec3(0.0f);
    
    void startDrag(int axis, const glm::vec3& startPos, const glm::vec3& objPos) {
        active = true;
        selectedAxis = axis;
        dragStartPos = startPos;
        objectStartPos = objPos;
    }
    
    void endDrag() {
        active = false;
        selectedAxis = -1;
    }
    
    glm::vec3 getAxisVector() const {
        if (selectedAxis == 0) return glm::vec3(1.0f, 0.0f, 0.0f);
        if (selectedAxis == 1) return glm::vec3(0.0f, 1.0f, 0.0f);
        if (selectedAxis == 2) return glm::vec3(0.0f, 0.0f, 1.0f);
        return glm::vec3(0.0f);
    }
};
