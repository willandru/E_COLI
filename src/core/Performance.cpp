#include "Performance.h"


#ifdef _WIN32

#include <windows.h>
#include <psapi.h>

#endif


// ============================================================
// CONSTRUCTOR
// ============================================================

Performance::Performance()
    :
      m_enabled(true),

      m_frameTime(0.0f),
      m_fps(0.0f),

      m_updateTime(0.0f),
      m_renderTime(0.0f),

      m_fpsAccumulator(0.0),
      m_frameCount(0),

      m_densityGenerationTime(0.0f),
      m_densityRenderTime(0.0f),

      m_densityGridSamples(0),
      m_densityActivePoints(0),
      m_densityRenderedPoints(0),

      m_densityResolutionX(0),
      m_densityResolutionY(0),
      m_densityResolutionZ(0)
{
}


// ============================================================
// BEGIN FRAME
// ============================================================

void Performance::beginFrame()
{
    if (!m_enabled)
        return;

    m_frameStart =
        Clock::now();
}


// ============================================================
// END FRAME
// ============================================================

void Performance::endFrame()
{
    if (!m_enabled)
        return;

    const TimePoint current =
        Clock::now();

    const std::chrono::duration<float, std::milli> duration =
        current - m_frameStart;

    m_frameTime =
        duration.count();


    // ========================================================
    // FPS
    // ========================================================

    m_fpsAccumulator +=
        static_cast<double>(
            m_frameTime
        );

    ++m_frameCount;


    if (m_fpsAccumulator >= 1000.0)
    {
        m_fps =
            static_cast<float>(
                static_cast<double>(m_frameCount) /
                (m_fpsAccumulator / 1000.0)
            );

        m_fpsAccumulator = 0.0;

        m_frameCount = 0;
    }
}


// ============================================================
// BEGIN UPDATE
// ============================================================

void Performance::beginUpdate()
{
    if (!m_enabled)
        return;

    m_updateStart =
        Clock::now();
}


// ============================================================
// END UPDATE
// ============================================================

void Performance::endUpdate()
{
    if (!m_enabled)
        return;

    const TimePoint current =
        Clock::now();

    const std::chrono::duration<float, std::milli> duration =
        current - m_updateStart;

    m_updateTime =
        duration.count();
}


// ============================================================
// BEGIN RENDER
// ============================================================

void Performance::beginRender()
{
    if (!m_enabled)
        return;

    m_renderStart =
        Clock::now();
}


// ============================================================
// END RENDER
// ============================================================

void Performance::endRender()
{
    if (!m_enabled)
        return;

    const TimePoint current =
        Clock::now();

    const std::chrono::duration<float, std::milli> duration =
        current - m_renderStart;

    m_renderTime =
        duration.count();
}


// ============================================================
// BEGIN DENSITY GENERATION
// ============================================================

void Performance::beginDensityGeneration()
{
    if (!m_enabled)
        return;

    m_densityGenerationStart =
        Clock::now();
}


// ============================================================
// END DENSITY GENERATION
// ============================================================

void Performance::endDensityGeneration()
{
    if (!m_enabled)
        return;

    const TimePoint current =
        Clock::now();

    const std::chrono::duration<float, std::milli> duration =
        current - m_densityGenerationStart;

    m_densityGenerationTime =
        duration.count();
}


// ============================================================
// BEGIN DENSITY RENDER
// ============================================================

void Performance::beginDensityRender()
{
    if (!m_enabled)
        return;

    m_densityRenderStart =
        Clock::now();
}


// ============================================================
// END DENSITY RENDER
// ============================================================

void Performance::endDensityRender()
{
    if (!m_enabled)
        return;

    const TimePoint current =
        Clock::now();

    const std::chrono::duration<float, std::milli> duration =
        current - m_densityRenderStart;

    m_densityRenderTime =
        duration.count();
}


// ============================================================
// DENSITY GRID SAMPLES
// ============================================================

void Performance::setDensityGridSamples(
    std::size_t samples
)
{
    m_densityGridSamples =
        samples;
}


// ============================================================
// DENSITY ACTIVE POINTS
// ============================================================

void Performance::setDensityActivePoints(
    std::size_t points
)
{
    m_densityActivePoints =
        points;
}


// ============================================================
// DENSITY RENDERED POINTS
// ============================================================

void Performance::setDensityRenderedPoints(
    std::size_t points
)
{
    m_densityRenderedPoints =
        points;
}


// ============================================================
// DENSITY RESOLUTION
// ============================================================

void Performance::setDensityResolution(
    int x,
    int y,
    int z
)
{
    m_densityResolutionX = x;

    m_densityResolutionY = y;

    m_densityResolutionZ = z;
}


// ============================================================
// GET FPS
// ============================================================

float Performance::getFPS() const
{
    return m_fps;
}


// ============================================================
// GET FRAME TIME
// ============================================================

float Performance::getFrameTime() const
{
    return m_frameTime;
}


// ============================================================
// GET UPDATE TIME
// ============================================================

float Performance::getUpdateTime() const
{
    return m_updateTime;
}


// ============================================================
// GET RENDER TIME
// ============================================================

float Performance::getRenderTime() const
{
    return m_renderTime;
}


// ============================================================
// GET MEMORY
// ============================================================

float Performance::getMemoryMB() const
{
#ifdef _WIN32

    PROCESS_MEMORY_COUNTERS memoryInfo{};

    if (
        GetProcessMemoryInfo(
            GetCurrentProcess(),
            &memoryInfo,
            sizeof(memoryInfo)
        )
    )
    {
        const double bytes =
            static_cast<double>(
                memoryInfo.WorkingSetSize
            );

        return static_cast<float>(
            bytes /
            (1024.0 * 1024.0)
        );
    }

#endif

    return 0.0f;
}


// ============================================================
// GET DENSITY GENERATION TIME
// ============================================================

float Performance::getDensityGenerationTime() const
{
    return m_densityGenerationTime;
}


// ============================================================
// GET DENSITY RENDER TIME
// ============================================================

float Performance::getDensityRenderTime() const
{
    return m_densityRenderTime;
}


// ============================================================
// GET DENSITY GRID SAMPLES
// ============================================================

std::size_t Performance::getDensityGridSamples() const
{
    return m_densityGridSamples;
}


// ============================================================
// GET DENSITY ACTIVE POINTS
// ============================================================

std::size_t Performance::getDensityActivePoints() const
{
    return m_densityActivePoints;
}


// ============================================================
// GET DENSITY RENDERED POINTS
// ============================================================

std::size_t Performance::getDensityRenderedPoints() const
{
    return m_densityRenderedPoints;
}


// ============================================================
// GET DENSITY RESOLUTION X
// ============================================================

int Performance::getDensityResolutionX() const
{
    return m_densityResolutionX;
}


// ============================================================
// GET DENSITY RESOLUTION Y
// ============================================================

int Performance::getDensityResolutionY() const
{
    return m_densityResolutionY;
}


// ============================================================
// GET DENSITY RESOLUTION Z
// ============================================================

int Performance::getDensityResolutionZ() const
{
    return m_densityResolutionZ;
}


// ============================================================
// ENABLE
// ============================================================

void Performance::setEnabled(
    bool enabled
)
{
    m_enabled =
        enabled;
}


// ============================================================
// IS ENABLED
// ============================================================

bool Performance::isEnabled() const
{
    return m_enabled;
}