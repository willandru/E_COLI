#include "ElectronicRenderer.h"

#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>


// ============================================================
// CONSTRUCTOR
// ============================================================

ElectronicRenderer::ElectronicRenderer()
{
    modelMatrix =
        glm::mat4(1.0f);

    viewMatrix =
        glm::mat4(1.0f);

    projectionMatrix =
        glm::mat4(1.0f);
}


// ============================================================
// DESTRUCTOR
// ============================================================

ElectronicRenderer::~ElectronicRenderer()
{
    destroyBuffers();

    delete shader;

    shader = nullptr;
}


// ============================================================
// INITIALIZATION
// ============================================================

void ElectronicRenderer::initialize()
{
    if (initialized)
    {
        return;
    }


    /*
     * The shader is intentionally kept inside the renderer.
     *
     * Rendering logic belongs here.
     * Quantum mechanics does not.
     */

    shader =
        new Shader(
            "../src/shaders/electronic.vert",
            "../src/shaders/electronic.frag"
        );


    createBuffers();


    initialized = true;
}


// ============================================================
// GEOMETRY
// ============================================================

void ElectronicRenderer::setPositions(
    const std::vector<glm::vec3>& newPositions)
{
    positions = newPositions;


    if (initialized)
    {
        uploadGeometry();
    }
}


void ElectronicRenderer::setValues(
    const std::vector<float>& newValues)
{
    values = newValues;


    if (initialized)
    {
        uploadGeometry();
    }
}


void ElectronicRenderer::setWaveFunctionValues(
    const std::vector<float>& newWaveFunctionValues)
{
    waveFunctionValues =
        newWaveFunctionValues;


    if (initialized)
    {
        uploadGeometry();
    }
}


// ============================================================
// TRANSFORMATION
// ============================================================

void ElectronicRenderer::setModelMatrix(
    const glm::mat4& model)
{
    modelMatrix = model;
}


void ElectronicRenderer::setViewMatrix(
    const glm::mat4& view)
{
    viewMatrix = view;
}


void ElectronicRenderer::setProjectionMatrix(
    const glm::mat4& projection)
{
    projectionMatrix = projection;
}


// ============================================================
// APPEARANCE
// ============================================================

void ElectronicRenderer::setPointSize(
    float newPointSize)
{
    if (newPointSize <= 0.0f)
    {
        throw std::invalid_argument(
            "ElectronicRenderer: point size must be greater than zero."
        );
    }


    pointSize = newPointSize;
}


void ElectronicRenderer::setPositiveColor(
    const glm::vec3& color)
{
    positiveColor = color;
}


void ElectronicRenderer::setNegativeColor(
    const glm::vec3& color)
{
    negativeColor = color;
}


// ============================================================
// RENDER
// ============================================================

void ElectronicRenderer::render()
{
    if (!initialized)
    {
        return;
    }


    if (positions.empty())
    {
        return;
    }


    /*
     * All three data arrays must contain one value
     * per rendered point.
     */

    if (values.size() != positions.size())
    {
        throw std::runtime_error(
            "ElectronicRenderer: density values do not match "
            "the number of positions."
        );
    }


    if (waveFunctionValues.size() != positions.size())
    {
        throw std::runtime_error(
            "ElectronicRenderer: wave function values do not "
            "match the number of positions."
        );
    }


    shader->bind();


    shader->setMat4(
        "model",
        modelMatrix
    );


    shader->setMat4(
        "view",
        viewMatrix
    );


    shader->setMat4(
        "projection",
        projectionMatrix
    );


    shader->setFloat(
        "pointSize",
        pointSize
    );


    shader->setVec3(
        "positiveColor",
        positiveColor
    );


    shader->setVec3(
        "negativeColor",
        negativeColor
    );


    /*
     * Point sprites require the vertex shader to control
     * gl_PointSize.
     */

    glEnable(
        GL_PROGRAM_POINT_SIZE
    );


    glBindVertexArray(VAO);


    glDrawArrays(
        GL_POINTS,
        0,
        static_cast<GLsizei>(
            positions.size()
        )
    );


    glBindVertexArray(0);


    glDisable(
        GL_PROGRAM_POINT_SIZE
    );
}


// ============================================================
// INFORMATION
// ============================================================

std::size_t ElectronicRenderer::getPointCount() const
{
    return positions.size();
}


bool ElectronicRenderer::isInitialized() const
{
    return initialized;
}


// ============================================================
// OPENGL BUFFERS
// ============================================================

void ElectronicRenderer::createBuffers()
{
    glGenVertexArrays(
        1,
        &VAO
    );


    glGenBuffers(
        1,
        &VBO
    );


    glGenBuffers(
        1,
        &densityVBO
    );


    glGenBuffers(
        1,
        &waveFunctionVBO
    );


    glBindVertexArray(VAO);


    // ========================================================
    // POSITION BUFFER
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        0,
        nullptr,
        GL_DYNAMIC_DRAW
    );


    glEnableVertexAttribArray(0);


    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec3),
        nullptr
    );


    // ========================================================
    // DENSITY BUFFER
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        densityVBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        0,
        nullptr,
        GL_DYNAMIC_DRAW
    );


    glEnableVertexAttribArray(1);


    glVertexAttribPointer(
        1,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(float),
        nullptr
    );


    // ========================================================
    // WAVE FUNCTION BUFFER
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        waveFunctionVBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        0,
        nullptr,
        GL_DYNAMIC_DRAW
    );


    glEnableVertexAttribArray(2);


    glVertexAttribPointer(
        2,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(float),
        nullptr
    );


    // ========================================================
    // CLEANUP
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );


    glBindVertexArray(0);
}


// ============================================================
// UPLOAD GEOMETRY
// ============================================================

void ElectronicRenderer::uploadGeometry()
{
    if (!initialized)
    {
        return;
    }


    // ========================================================
    // POSITIONS
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        positions.size()
            *
            sizeof(glm::vec3),
        positions.data(),
        GL_DYNAMIC_DRAW
    );


    // ========================================================
    // DENSITY
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        densityVBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        values.size()
            *
            sizeof(float),
        values.data(),
        GL_DYNAMIC_DRAW
    );


    // ========================================================
    // WAVE FUNCTION
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        waveFunctionVBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        waveFunctionValues.size()
            *
            sizeof(float),
        waveFunctionValues.data(),
        GL_DYNAMIC_DRAW
    );


    // ========================================================
    // CLEANUP
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );
}


// ============================================================
// DESTROY BUFFERS
// ============================================================

void ElectronicRenderer::destroyBuffers()
{
    if (waveFunctionVBO != 0)
    {
        glDeleteBuffers(
            1,
            &waveFunctionVBO
        );

        waveFunctionVBO = 0;
    }


    if (densityVBO != 0)
    {
        glDeleteBuffers(
            1,
            &densityVBO
        );

        densityVBO = 0;
    }


    if (VBO != 0)
    {
        glDeleteBuffers(
            1,
            &VBO
        );

        VBO = 0;
    }


    if (VAO != 0)
    {
        glDeleteVertexArrays(
            1,
            &VAO
        );

        VAO = 0;
    }
}