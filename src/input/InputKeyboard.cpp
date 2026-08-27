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
    return isPressed(
        GLFW_KEY_ESCAPE
    );
}