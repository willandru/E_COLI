#pragma once

#include <glm/glm.hpp>


class InputMouse;
class InputKeyboard;


class Camera
{
public:

    Camera(
        const glm::vec3& position = glm::vec3(0.0f, 3.0f, 8.0f)
    );


    // ========================================================
    // Actualización
    // ========================================================

    void update(
        const InputKeyboard& keyboard,
        const InputMouse& mouse,
        float deltaTime
    );


    // ========================================================
    // Movimiento
    // ========================================================

    void moveForward(float amount);

    void moveBackward(float amount);

    void moveLeft(float amount);

    void moveRight(float amount);

    void moveUp(float amount);

    void moveDown(float amount);


    // ========================================================
    // Rotación
    // ========================================================

    void rotate(
        float xOffset,
        float yOffset
    );


    // ========================================================
    // Zoom
    // ========================================================

    void zoom(float amount);


    // ========================================================
    // Matrices
    // ========================================================

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix(
        float aspectRatio
    ) const;


    // ========================================================
    // Información
    // ========================================================

    const glm::vec3& getPosition() const;

    const glm::vec3& getFront() const;

    float getFov() const;


private:

    void updateDirection();


private:

    glm::vec3 m_position;

    glm::vec3 m_front;

    glm::vec3 m_up;

    glm::vec3 m_worldUp;

    glm::vec3 m_right;


    float m_yaw;

    float m_pitch;

    float m_fov;

    float m_mouseSensitivity;
};