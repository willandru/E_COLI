#include "Window.h"

#include <glad/glad.h>

#include <iostream>


// ============================================================
// CONSTRUCTOR
// ============================================================

Window::Window(
    int width,
    int height,
    const char* title
)
    : m_window(nullptr),
      m_width(width),
      m_height(height)
{
    // ========================================================
    // Inicializar GLFW
    // ========================================================

    if (!glfwInit())
    {
        std::cerr
            << "Error: no se pudo inicializar GLFW.\n";

        return;
    }


    // ========================================================
    // Configuración de OpenGL
    // ========================================================

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        3
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3
    );

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );


    // ========================================================
    // Obtener monitor principal
    // ========================================================

    GLFWmonitor* monitor =
        glfwGetPrimaryMonitor();

    const GLFWvidmode* videoMode =
        glfwGetVideoMode(monitor);


    if (!videoMode)
    {
        std::cerr
            << "Error: no se pudo obtener el monitor.\n";

        glfwTerminate();

        return;
    }


    // ========================================================
    // Crear ventana Fullscreen
    // ========================================================

    m_width =
        videoMode->width;

    m_height =
        videoMode->height;


    m_window =
        glfwCreateWindow(
            m_width,
            m_height,
            title,
            monitor,
            nullptr
        );


    if (!m_window)
    {
        std::cerr
            << "Error: no se pudo crear la ventana.\n";

        glfwTerminate();

        return;
    }


    // ========================================================
    // Crear contexto OpenGL
    // ========================================================

    glfwMakeContextCurrent(
        m_window
    );


    // ========================================================
    // Inicializar GLAD
    // ========================================================

    if (!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(
            glfwGetProcAddress
        )))
    {
        std::cerr
            << "Error: no se pudo inicializar GLAD.\n";

        glfwDestroyWindow(
            m_window
        );

        glfwTerminate();

        m_window = nullptr;

        return;
    }


    // ========================================================
    // Viewport
    // ========================================================

    glViewport(
        0,
        0,
        m_width,
        m_height
    );


    // ========================================================
    // Depth testing
    // ========================================================

    glEnable(
        GL_DEPTH_TEST
    );


    // ========================================================
    // V-Sync
    // ========================================================

    glfwSwapInterval(0);
}


// ============================================================
// DESTRUCTOR
// ============================================================

Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(
            m_window
        );
    }

    glfwTerminate();
}


// ============================================================
// SHOULD CLOSE
// ============================================================

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(
        m_window
    );
}


// ============================================================
// CLEAR
// ============================================================

void Window::clear()
{
    glClearColor(
        0.02f,
        0.025f,
        0.035f,
        1.0f
    );


    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );
}


// ============================================================
// PROCESS EVENTS
// ============================================================

void Window::processEvents()
{
    glfwPollEvents();
}


// ============================================================
// PRESENT
// ============================================================

void Window::present()
{
    glfwSwapBuffers(
        m_window
    );
}


// ============================================================
// NATIVE WINDOW
// ============================================================

GLFWwindow* Window::getNativeWindow() const
{
    return m_window;
}


// ============================================================
// WIDTH
// ============================================================

int Window::getWidth() const
{
    return m_width;
}


// ============================================================
// HEIGHT
// ============================================================

int Window::getHeight() const
{
    return m_height;
}


// ============================================================
// ASPECT RATIO
// ============================================================

float Window::getAspectRatio() const
{
    return static_cast<float>(m_width) /
           static_cast<float>(m_height);
}