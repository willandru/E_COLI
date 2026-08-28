#include "DNARenderer.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <vector>


// ============================================================
// CONSTRUCTOR
// ============================================================

DNARenderer::DNARenderer(
    const DNA& dna
)
    : m_VAO(0),
      m_VBO(0),
      m_instanceVBO(0),
      m_instanceCount(0),
      m_shader(
          "shaders/dna.vert",
          "shaders/dna.frag"
      )
{
    // ========================================================
    // Crear geometría
    // ========================================================

    createGeometry();


    // ========================================================
    // Crear instancias
    // ========================================================

    createInstances(
        dna
    );
}


// ============================================================
// DESTRUCTOR
// ============================================================

DNARenderer::~DNARenderer()
{
    glDeleteVertexArrays(
        1,
        &m_VAO
    );


    glDeleteBuffers(
        1,
        &m_VBO
    );


    glDeleteBuffers(
        1,
        &m_instanceVBO
    );
}


// ============================================================
// CREATE GEOMETRY
// ============================================================

void DNARenderer::createGeometry()
{
    // ========================================================
    // Quad
    //
    // Cada instancia utiliza el mismo quad.
    // El fragment shader convierte el quad
    // en una esfera visual.
    // ========================================================

    float vertices[] =
    {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,

        -1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f
    };


    // ========================================================
    // VAO
    // ========================================================

    glGenVertexArrays(
        1,
        &m_VAO
    );


    glBindVertexArray(
        m_VAO
    );


    // ========================================================
    // VBO
    // ========================================================

    glGenBuffers(
        1,
        &m_VBO
    );


    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_VBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );


    // ========================================================
    // Position
    // ========================================================

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        nullptr
    );


    glEnableVertexAttribArray(
        0
    );


    // ========================================================
    // Unbind VBO
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );


    // ========================================================
    // Unbind VAO
    // ========================================================

    glBindVertexArray(
        0
    );
}


// ============================================================
// CREATE INSTANCES
// ============================================================

void DNARenderer::createInstances(
    const DNA& dna
)
{
    // ========================================================
    // Obtener secuencia
    // ========================================================

    const std::string& sequence =
        dna.getSequence();


    // ========================================================
    // Número de bases
    // ========================================================

    m_instanceCount =
        static_cast<unsigned int>(
            sequence.size()
        );


    // ========================================================
    // VAO
    // ========================================================

    glBindVertexArray(
        m_VAO
    );


    // ========================================================
    // Instance VBO
    // ========================================================

    glGenBuffers(
        1,
        &m_instanceVBO
    );


    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_instanceVBO
    );


    // ========================================================
    // Copiar secuencia a GPU
    // ========================================================

    glBufferData(
        GL_ARRAY_BUFFER,
        sequence.size() * sizeof(char),
        sequence.data(),
        GL_STATIC_DRAW
    );


    // ========================================================
    // Base nitrogenada
    //
    // IMPORTANTE:
    //
    // El shader utiliza:
    //
    //     uint base
    //
    // Por eso necesitamos una función
    // de atributo ENTERO.
    // ========================================================

    glVertexAttribIPointer(
        1,
        1,
        GL_UNSIGNED_BYTE,
        sizeof(char),
        nullptr
    );


    glEnableVertexAttribArray(
        1
    );


    // ========================================================
    // Una base por instancia
    // ========================================================

    glVertexAttribDivisor(
        1,
        1
    );


    // ========================================================
    // Unbind VBO
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );


    // ========================================================
    // Unbind VAO
    // ========================================================

    glBindVertexArray(
        0
    );
}


// ============================================================
// RENDER
// ============================================================

void DNARenderer::render(
    const Camera& camera,
    const Window& window
)
{
    // ========================================================
    // Aspect ratio
    // ========================================================

    float aspectRatio =
        window.getAspectRatio();


    // ========================================================
    // Matrices
    // ========================================================

    glm::mat4 model =
        glm::mat4(1.0f);


    glm::mat4 view =
        camera.getViewMatrix();


    glm::mat4 projection =
        camera.getProjectionMatrix(
            aspectRatio
        );


    // ========================================================
    // Shader
    // ========================================================

    m_shader.bind();


    // ========================================================
    // Matrices
    // ========================================================

    m_shader.setMat4(
        "model",
        model
    );


    m_shader.setMat4(
        "view",
        view
    );


    m_shader.setMat4(
        "projection",
        projection
    );


    // ========================================================
    // Parámetros de las bolitas
    // ========================================================

    m_shader.setFloat(
        "particleRadius",
        0.025f
    );


    m_shader.setFloat(
        "particleSpacing",
        0.05f
    );


    // ========================================================
    // Render instanciado
    // ========================================================

    glBindVertexArray(
        m_VAO
    );


    glDrawArraysInstanced(
        GL_TRIANGLES,
        0,
        6,
        m_instanceCount
    );


    // ========================================================
    // Unbind
    // ========================================================

    glBindVertexArray(
        0
    );
}