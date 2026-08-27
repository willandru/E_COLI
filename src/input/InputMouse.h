#pragma once

#include <GLFW/glfw3.h>


class InputMouse
{
public:

    explicit InputMouse(
        GLFWwindow* window
    );


    // ========================================================
    // Actualizar estado
    // ========================================================

    void update();


    // ========================================================
    // Estado de rotación
    // ========================================================

    bool isRotating() const;


    // ========================================================
    // Movimiento
    // ========================================================

    float getDeltaX() const;

    float getDeltaY() const;


    // ========================================================
    // Scroll
    // ========================================================

    float getScrollDelta() const;


private:

    static void mousePositionCallback(
        GLFWwindow* window,
        double xpos,
        double ypos
    );


    static void mouseButtonCallback(
        GLFWwindow* window,
        int button,
        int action,
        int mods
    );


    static void scrollCallback(
        GLFWwindow* window,
        double xOffset,
        double yOffset
    );


private:

    GLFWwindow* m_window;

    float m_deltaX;
    float m_deltaY;

    float m_scrollDelta;

    float m_lastX;
    float m_lastY;

    bool m_firstMouse;

    bool m_rotating;


    static InputMouse* s_instance;
};