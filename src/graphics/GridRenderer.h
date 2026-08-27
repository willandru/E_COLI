#pragma once


#include "Grid.h"
#include "Shader.h"
#include "Camera.h"
#include "Window.h"


class GridRenderer
{
public:

    // ========================================================
    // Constructor
    // ========================================================

    GridRenderer();


    // ========================================================
    // Render
    // ========================================================

    void render(
        Grid& grid,
        const Camera& camera,
        const Window& window
    );


private:

    Shader m_shader;
};