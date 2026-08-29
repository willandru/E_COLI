#pragma once

#include <chrono>


class Performance
{
public:

    Performance();


    // ========================================================
    // FRAME
    // ========================================================

    void beginFrame();

    void endFrame();


    // ========================================================
    // UPDATE
    // ========================================================

    void beginUpdate();

    void endUpdate();


    // ========================================================
    // RENDER
    // ========================================================

    void beginRender();

    void endRender();


    // ========================================================
    // GETTERS
    // ========================================================

    float getFPS() const;

    float getFrameTime() const;

    float getUpdateTime() const;

    float getRenderTime() const;

    float getMemoryMB() const;


    // ========================================================
    // CONTROL
    // ========================================================

    void setEnabled(
        bool enabled
    );

    bool isEnabled() const;


private:

    using Clock =
        std::chrono::high_resolution_clock;

    using TimePoint =
        std::chrono::time_point<Clock>;


    bool m_enabled;


    // ========================================================
    // FRAME
    // ========================================================

    TimePoint m_frameStart;

    float m_frameTime;

    float m_fps;


    // ========================================================
    // UPDATE
    // ========================================================

    TimePoint m_updateStart;

    float m_updateTime;


    // ========================================================
    // RENDER
    // ========================================================

    TimePoint m_renderStart;

    float m_renderTime;


    // ========================================================
    // FPS
    // ========================================================

    double m_fpsAccumulator;

    unsigned int m_frameCount;
};