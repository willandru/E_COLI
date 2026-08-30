#pragma once

#include <chrono>
#include <cstddef>


// ============================================================
// PERFORMANCE
// ============================================================
//
// Sistema central de medición de rendimiento.
//
// Mide:
//
//     FRAME
//     UPDATE
//     RENDER
//     FPS
//     RAM
//
// También permite medir:
//
//     ElectronDensity generation
//     ElectronDensity rendering
//     estadísticas de la densidad
//
// No depende de OpenGL.
//
// ============================================================

class Performance
{
public:

    // ========================================================
    // CONSTRUCTOR
    // ========================================================

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
    // ELECTRON DENSITY
    // ========================================================

    void beginDensityGeneration();

    void endDensityGeneration();

    void beginDensityRender();

    void endDensityRender();


    // ========================================================
    // DENSITY STATISTICS
    // ========================================================

    void setDensityGridSamples(
        std::size_t samples
    );

    void setDensityActivePoints(
        std::size_t points
    );

    void setDensityRenderedPoints(
        std::size_t points
    );

    void setDensityResolution(
        int x,
        int y,
        int z
    );


    // ========================================================
    // GETTERS
    // ========================================================

    float getFPS() const;

    float getFrameTime() const;

    float getUpdateTime() const;

    float getRenderTime() const;

    float getMemoryMB() const;


    // ========================================================
    // DENSITY GETTERS
    // ========================================================

    float getDensityGenerationTime() const;

    float getDensityRenderTime() const;

    std::size_t getDensityGridSamples() const;

    std::size_t getDensityActivePoints() const;

    std::size_t getDensityRenderedPoints() const;

    int getDensityResolutionX() const;

    int getDensityResolutionY() const;

    int getDensityResolutionZ() const;


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


    // ========================================================
    // STATE
    // ========================================================

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


    // ========================================================
    // ELECTRON DENSITY
    // ========================================================

    TimePoint m_densityGenerationStart;

    float m_densityGenerationTime;

    TimePoint m_densityRenderStart;

    float m_densityRenderTime;


    // ========================================================
    // DENSITY STATISTICS
    // ========================================================

    std::size_t m_densityGridSamples;

    std::size_t m_densityActivePoints;

    std::size_t m_densityRenderedPoints;


    // ========================================================
    // DENSITY RESOLUTION
    // ========================================================

    int m_densityResolutionX;

    int m_densityResolutionY;

    int m_densityResolutionZ;
};