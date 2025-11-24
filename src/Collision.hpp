// Collision.hpp
// ---------------------------------------------------------
// Collision detection utilities for 3D objects
// ---------------------------------------------------------

#pragma once

#include <glm/glm.hpp>
#include <vector>

// Axis-Aligned Bounding Box
struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}
};

// Sphere collider (for camera)
struct Sphere {
    glm::vec3 center;
    float radius;

    Sphere(const glm::vec3& center, float radius) : center(center), radius(radius) {}
};

// Check if a sphere intersects with an AABB
inline bool sphereAABBCollision(const Sphere& sphere, const AABB& box) {
    // Find the closest point on the AABB to the sphere center
    glm::vec3 closestPoint;
    closestPoint.x = glm::clamp(sphere.center.x, box.min.x, box.max.x);
    closestPoint.y = glm::clamp(sphere.center.y, box.min.y, box.max.y);
    closestPoint.z = glm::clamp(sphere.center.z, box.min.z, box.max.z);

    // Calculate distance from sphere center to this closest point
    float distanceSquared = glm::dot(sphere.center - closestPoint, sphere.center - closestPoint);

    // Collision if distance is less than sphere radius
    return distanceSquared < (sphere.radius * sphere.radius);
}

// Collision manager to store all collidable objects
class CollisionManager {
public:
    std::vector<AABB> boxes;

    void addBox(const AABB& box) {
        boxes.push_back(box);
    }

    void clear() {
        boxes.clear();
    }

    // Check if a sphere (camera) collides with any object
    bool checkCollision(const Sphere& sphere) const {
        for (const auto& box : boxes) {
            if (sphereAABBCollision(sphere, box)) {
                return true;
            }
        }
        return false;
    }

    // Get the valid position after collision resolution
    glm::vec3 resolveCollision(const glm::vec3& oldPos, const glm::vec3& newPos, float radius) const {
        Sphere sphere(newPos, radius);
        
        // If no collision, allow movement
        if (!checkCollision(sphere)) {
            return newPos;
        }

        // Try moving only on X axis
        glm::vec3 testPos = glm::vec3(newPos.x, oldPos.y, oldPos.z);
        sphere.center = testPos;
        if (!checkCollision(sphere)) {
            // Then try adding Y
            testPos.y = newPos.y;
            sphere.center = testPos;
            if (!checkCollision(sphere)) {
                // Then try adding Z
                testPos.z = newPos.z;
                sphere.center = testPos;
                if (!checkCollision(sphere)) {
                    return testPos;
                }
                return glm::vec3(testPos.x, testPos.y, oldPos.z);
            }
            return glm::vec3(testPos.x, oldPos.y, oldPos.z);
        }

        // Try moving only on Y axis
        testPos = glm::vec3(oldPos.x, newPos.y, oldPos.z);
        sphere.center = testPos;
        if (!checkCollision(sphere)) {
            // Then try adding Z
            testPos.z = newPos.z;
            sphere.center = testPos;
            if (!checkCollision(sphere)) {
                return testPos;
            }
            return glm::vec3(oldPos.x, testPos.y, oldPos.z);
        }

        // Try moving only on Z axis
        testPos = glm::vec3(oldPos.x, oldPos.y, newPos.z);
        sphere.center = testPos;
        if (!checkCollision(sphere)) {
            return testPos;
        }

        // Can't move, return old position
        return oldPos;
    }
};
