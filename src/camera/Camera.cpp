#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>


// ============================================================
// CONSTRUCTOR
// ============================================================

Camera::Camera(
    const glm::vec3& position
)
    : m_position(position),
      m_front(0.0f, 0.0f, -1.0f),
      m_up(0.0f, 1.0f, 0.0f),
      m_worldUp(0.0f, 1.0f, 0.0f),
      m_right(1.0f, 0.0f, 0.0f),
      m_yaw(-90.0f),
      m_pitch(-15.0f),
      m_fov(60.0f),
      m_mouseSensitivity(0.1f)
{
    updateDirection();
}


// ============================================================
// MOVIMIENTO
// ============================================================

void Camera::moveForward(float amount)
{
    m_position +=
        m_front * amount;
}


void Camera::moveBackward(float amount)
{
    m_position -=
        m_front * amount;
}


void Camera::moveLeft(float amount)
{
    m_position -=
        m_right * amount;
}


void Camera::moveRight(float amount)
{
    m_position +=
        m_right * amount;
}


void Camera::moveUp(float amount)
{
    m_position +=
        m_worldUp * amount;
}


void Camera::moveDown(float amount)
{
    m_position -=
        m_worldUp * amount;
}


// ============================================================
// ROTACIÓN
// ============================================================

void Camera::rotate(
    float xOffset,
    float yOffset
)
{
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;


    // Limitar rotación vertical

    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }

    if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }


    updateDirection();
}


// ============================================================
// ZOOM
// ============================================================

void Camera::zoom(float amount)
{
    m_fov -= amount;


    if (m_fov < 20.0f)
    {
        m_fov = 20.0f;
    }


    if (m_fov > 90.0f)
    {
        m_fov = 90.0f;
    }
}


// ============================================================
// ACTUALIZAR DIRECCIÓN
// ============================================================

void Camera::updateDirection()
{
    glm::vec3 direction;

    direction.x =
        std::cos(glm::radians(m_yaw)) *
        std::cos(glm::radians(m_pitch));

    direction.y =
        std::sin(glm::radians(m_pitch));

    direction.z =
        std::sin(glm::radians(m_yaw)) *
        std::cos(glm::radians(m_pitch));


    m_front =
        glm::normalize(direction);


    m_right =
        glm::normalize(
            glm::cross(
                m_front,
                m_worldUp
            )
        );


    m_up =
        glm::normalize(
            glm::cross(
                m_right,
                m_front
            )
        );
}


// ============================================================
// VIEW MATRIX
// ============================================================

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(
        m_position,
        m_position + m_front,
        m_up
    );
}


// ============================================================
// PROJECTION MATRIX
// ============================================================

glm::mat4 Camera::getProjectionMatrix(
    float aspectRatio
) const
{
    return glm::perspective(
        glm::radians(m_fov),
        aspectRatio,
        0.1f,
        500.0f
    );
}


// ============================================================
// GETTERS
// ============================================================

const glm::vec3& Camera::getPosition() const
{
    return m_position;
}


const glm::vec3& Camera::getFront() const
{
    return m_front;
}


float Camera::getFov() const
{
    return m_fov;
}