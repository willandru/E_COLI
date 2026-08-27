#include "Timer1.h"

#include <GLFW/glfw3.h>


// ============================================================
// CONSTRUCTOR
// ============================================================

Timer1::Timer1()
    : m_deltaTime(0.0f),
      m_lastFrame(0.0f)
{
}


// ============================================================
// UPDATE
// ============================================================

void Timer1::update()
{
    float currentFrame =
        static_cast<float>(
            glfwGetTime()
        );


    m_deltaTime =
        currentFrame -
        m_lastFrame;


    m_lastFrame =
        currentFrame;
}


// ============================================================
// GET DELTA TIME
// ============================================================

float Timer1::getDeltaTime() const
{
    return m_deltaTime;
}