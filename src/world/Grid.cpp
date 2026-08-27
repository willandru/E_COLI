#include "Grid.h"

#include <glad/glad.h>


// ============================================================
// CONSTRUCTOR
// ============================================================

Grid::Grid(
    float size,
    int divisions
)
    : m_VAO(0),
      m_VBO(0),
      m_vertexCount(0)
{
    std::vector<float> vertices =
        createVertices(
            size,
            divisions
        );


    m_vertexCount =
        static_cast<unsigned int>(
            vertices.size() / 3
        );


    // ========================================================
    // VAO
    // ========================================================

    glGenVertexArrays(
        1,
        &m_VAO
    );


    // ========================================================
    // VBO
    // ========================================================

    glGenBuffers(
        1,
        &m_VBO
    );


    glBindVertexArray(
        m_VAO
    );


    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_VBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );


    // ========================================================
    // Position
    // ========================================================

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );


    glEnableVertexAttribArray(
        0
    );


    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );


    glBindVertexArray(
        0
    );
}


// ============================================================
// DESTRUCTOR
// ============================================================

Grid::~Grid()
{
    glDeleteVertexArrays(
        1,
        &m_VAO
    );

    glDeleteBuffers(
        1,
        &m_VBO
    );
}


// ============================================================
// CREATE VERTICES
// ============================================================

std::vector<float> Grid::createVertices(
    float size,
    int divisions
)
{
    std::vector<float> vertices;


    float half =
        size / 2.0f;


    float step =
        size /
        static_cast<float>(divisions);


    // ========================================================
    // Líneas paralelas al eje X
    // ========================================================

    for (int i = 0; i <= divisions; ++i)
    {
        float z =
            -half + i * step;


        vertices.push_back(-half);
        vertices.push_back(0.0f);
        vertices.push_back(z);


        vertices.push_back(half);
        vertices.push_back(0.0f);
        vertices.push_back(z);
    }


    // ========================================================
    // Líneas paralelas al eje Z
    // ========================================================

    for (int i = 0; i <= divisions; ++i)
    {
        float x =
            -half + i * step;


        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(-half);


        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(half);
    }


    return vertices;
}


// ============================================================
// RENDER
// ============================================================

void Grid::render(
    Shader& shader
) const
{
    shader.bind();


    glBindVertexArray(
        m_VAO
    );


    glDrawArrays(
        GL_LINES,
        0,
        m_vertexCount
    );


    glBindVertexArray(
        0
    );
}