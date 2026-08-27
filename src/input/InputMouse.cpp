#include "InputMouse.h"

#include <iostream>


InputMouse*
InputMouse::s_instance = nullptr;


// ============================================================
// CONSTRUCTOR
// ============================================================

InputMouse::InputMouse(
    GLFWwindow* window
)
    : m_window(window),
      m_deltaX(0.0f),
      m_deltaY(0.0f),
      m_scrollDelta(0.0f),
      m_lastX(0.0f),
      m_lastY(0.0f),
      m_firstMouse(true),
      m_rotating(false)
{
    s_instance = this;


    // ========================================================
    // Registrar callbacks
    // ========================================================

    glfwSetCursorPosCallback(
        m_window,
        mousePositionCallback
    );


    glfwSetMouseButtonCallback(
        m_window,
        mouseButtonCallback
    );


    glfwSetScrollCallback(
        m_window,
        scrollCallback
    );


    // ========================================================
    // Cursor visible inicialmente
    // ========================================================

    glfwSetInputMode(
        m_window,
        GLFW_CURSOR,
        GLFW_CURSOR_NORMAL
    );
}


// ============================================================
// UPDATE
// ============================================================

void InputMouse::update()
{
    m_deltaX = 0.0f;
    m_deltaY = 0.0f;

    m_scrollDelta = 0.0f;
}


// ============================================================
// MOUSE POSITION
// ============================================================

void InputMouse::mousePositionCallback(
    GLFWwindow* window,
    double xpos,
    double ypos
)
{
    if (!s_instance)
    {
        return;
    }


    // Si no estamos rotando,
    // ignoramos el movimiento.

    if (!s_instance->m_rotating)
    {
        s_instance->m_firstMouse = true;

        return;
    }


    // Primera posición al comenzar a rotar

    if (s_instance->m_firstMouse)
    {
        s_instance->m_lastX =
            static_cast<float>(xpos);

        s_instance->m_lastY =
            static_cast<float>(ypos);

        s_instance->m_firstMouse = false;

        return;
    }


    // Calcular desplazamiento

    s_instance->m_deltaX =
        static_cast<float>(xpos) -
        s_instance->m_lastX;


    s_instance->m_deltaY =
        s_instance->m_lastY -
        static_cast<float>(ypos);


    s_instance->m_lastX =
        static_cast<float>(xpos);

    s_instance->m_lastY =
        static_cast<float>(ypos);
}


// ============================================================
// MOUSE BUTTON
// ============================================================

void InputMouse::mouseButtonCallback(
    GLFWwindow* window,
    int button,
    int action,
    int mods
)
{
    if (!s_instance)
    {
        return;
    }


    // ========================================================
    // BOTÓN CENTRAL
    // ========================================================

    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        // ----------------------------------------------------
        // Presionar
        // ----------------------------------------------------

        if (action == GLFW_PRESS)
        {
            s_instance->m_rotating = true;

            s_instance->m_firstMouse = true;


            // Ocultar y capturar cursor

            glfwSetInputMode(
                window,
                GLFW_CURSOR,
                GLFW_CURSOR_DISABLED
            );
        }


        // ----------------------------------------------------
        // Soltar
        // ----------------------------------------------------

        else if (action == GLFW_RELEASE)
        {
            s_instance->m_rotating = false;

            s_instance->m_firstMouse = true;


            // Mostrar cursor

            glfwSetInputMode(
                window,
                GLFW_CURSOR,
                GLFW_CURSOR_NORMAL
            );
        }
    }
}


// ============================================================
// SCROLL
// ============================================================

void InputMouse::scrollCallback(
    GLFWwindow* window,
    double xOffset,
    double yOffset
)
{
    if (!s_instance)
    {
        return;
    }


    s_instance->m_scrollDelta =
        static_cast<float>(yOffset);
}


// ============================================================
// GETTERS
// ============================================================

bool InputMouse::isRotating() const
{
    return m_rotating;
}


float InputMouse::getDeltaX() const
{
    return m_deltaX;
}


float InputMouse::getDeltaY() const
{
    return m_deltaY;
}


float InputMouse::getScrollDelta() const
{
    return m_scrollDelta;
}