#pragma once


class Timer1
{
public:

    Timer1();


    // ========================================================
    // Actualizar tiempo
    // ========================================================

    void update();


    // ========================================================
    // Delta time
    // ========================================================

    float getDeltaTime() const;


private:

    float m_deltaTime;

    float m_lastFrame;
};