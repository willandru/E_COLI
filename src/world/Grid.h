#pragma once


#include <vector>

#include "Shader.h"


class Grid
{
public:

    // ========================================================
    // Constructor
    // ========================================================

    Grid(
        float size,
        int divisions
    );


    // ========================================================
    // Destructor
    // ========================================================

    ~Grid();


    // ========================================================
    // Render
    // ========================================================

    void render(
        Shader& shader
    ) const;


private:

    // ========================================================
    // Crear geometría
    // ========================================================

    std::vector<float> createVertices(
        float size,
        int divisions
    ) const;


private:

    unsigned int m_VAO;

    unsigned int m_VBO;

    unsigned int m_vertexCount;
};