#pragma once

#include <vector>

#include "Shader.h"


class Grid
{
public:

    Grid(
        float size,
        int divisions
    );

    ~Grid();


    // ========================================================
    // Render
    // ========================================================

    void render(
        Shader& shader
    ) const;


private:

    std::vector<float> createVertices(
        float size,
        int divisions
    );


private:

    unsigned int m_VAO;
    unsigned int m_VBO;

    unsigned int m_vertexCount;
};
