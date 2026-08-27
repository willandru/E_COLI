#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>


class Window
{
public:

    Window(
        int width,
        int height,
        const char* title
    );

    ~Window();


    // ========================================================
    // Window
    // ========================================================

    bool shouldClose() const;

    void clear();

    void processEvents();

    void present();


    // ========================================================
    // Native window
    // ========================================================

    GLFWwindow* getNativeWindow() const;


    // ========================================================
    // Dimensions
    // ========================================================

    int getWidth() const;

    int getHeight() const;


private:

    GLFWwindow* m_window;

    int m_width;

    int m_height;
};