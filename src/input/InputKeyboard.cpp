#include "InputKeyboard.h"


// ============================================================
// CONSTRUCTOR
// ============================================================

InputKeyboard::InputKeyboard(
    GLFWwindow* window
)
    : m_window(window)
{
}


// ============================================================
// UPDATE
// ============================================================

void InputKeyboard::update()
{
    // Actualmente no necesitamos
    // procesar eventos aquí.

    // GLFW procesa los eventos mediante
    // glfwPollEvents().
}


// ============================================================
// IS PRESSED
// ============================================================

bool InputKeyboard::isPressed(
    int key
) const
{
    return glfwGetKey(
        m_window,
        key
    ) == GLFW_PRESS;
}


// ============================================================
// SHOULD CLOSE
// ============================================================

bool InputKeyboard::shouldClose() const
{
    return glfwWindowShouldClose(
        m_window
    );
}