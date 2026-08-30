#include "ElectronDensityRenderer.h"

#include <vector>

#include <glm/gtc/type_ptr.hpp>


namespace Chemistry
{

namespace
{

// ============================================================
// CONFIGURACIÓN DE RENDER
// ============================================================

// Densidades inferiores a este valor normalizado no se
// renderizan.

constexpr float DENSITY_THRESHOLD = 0.05f;


// Tamaño visual de cada punto.

constexpr float POINT_SIZE = 3.0f;

}


// ============================================================
// Constructor
// ============================================================

ElectronDensityRenderer::ElectronDensityRenderer()
    : VAO(0),
      VBO(0),
      vertexCount(0),
      shader(
          "shaders/electronDensity.vert",
          "shaders/electronDensity.frag"
      )
{
    initialize();
}


// ============================================================
// Destructor
// ============================================================

ElectronDensityRenderer::~ElectronDensityRenderer()
{
    if (VBO != 0)
    {
        glDeleteBuffers(
            1,
            &VBO
        );
    }


    if (VAO != 0)
    {
        glDeleteVertexArrays(
            1,
            &VAO
        );
    }
}


// ============================================================
// Initialize
// ============================================================

void ElectronDensityRenderer::initialize()
{
    glGenVertexArrays(
        1,
        &VAO
    );


    glGenBuffers(
        1,
        &VBO
    );
}


// ============================================================
// Build points
// ============================================================

void ElectronDensityRenderer::buildPoints(
    const ElectronDensity& density
)
{
    struct Vertex
    {
        glm::vec3 position;

        float density;
    };


    std::vector<Vertex> vertices;


    const glm::ivec3 resolution =
        density.getResolution();


    const float maximumDensity =
        density.getMaximumDensity();


    vertexCount = 0;


    if (maximumDensity <= 0.0f)
    {
        return;
    }


    // ========================================================
    // Recorrer la rejilla
    // ========================================================

    for (int z = 0; z < resolution.z; ++z)
    {
        for (int y = 0; y < resolution.y; ++y)
        {
            for (int x = 0; x < resolution.x; ++x)
            {
                const float value =
                    density.getDensity(
                        x,
                        y,
                        z
                    );


                // ------------------------------------------------
                // Normalización
                // ------------------------------------------------

                const float normalizedDensity =
                    value / maximumDensity;


                // ------------------------------------------------
                // Umbral
                // ------------------------------------------------

                if (
                    normalizedDensity <
                    DENSITY_THRESHOLD
                )
                {
                    continue;
                }


                // ------------------------------------------------
                // Posición física
                // ------------------------------------------------

                const glm::vec3 positionAngstrom =
                    density.getPosition(
                        x,
                        y,
                        z
                    );


                // ------------------------------------------------
                // Å -> OpenGL
                // ------------------------------------------------

                const glm::vec3 positionOpenGL =
                    positionAngstrom *
                    ANGSTROM_TO_OPENGL;


                vertices.push_back(
                    {
                        positionOpenGL,
                        normalizedDensity
                    }
                );
            }
        }
    }


    // ========================================================
    // Número de vértices
    // ========================================================

    vertexCount =
        static_cast<GLsizei>(
            vertices.size()
        );


    // ========================================================
    // Upload GPU
    // ========================================================

    glBindVertexArray(
        VAO
    );


    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            vertices.size() *
            sizeof(Vertex)
        ),
        vertices.empty()
            ? nullptr
            : vertices.data(),
        GL_DYNAMIC_DRAW
    );


    // ========================================================
    // Position
    // ========================================================

    glEnableVertexAttribArray(
        0
    );


    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(0)
    );


    // ========================================================
    // Density
    // ========================================================

    glEnableVertexAttribArray(
        1
    );


    glVertexAttribPointer(
        1,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(
            sizeof(glm::vec3)
        )
    );


    // ========================================================
    // Unbind
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );


    glBindVertexArray(
        0
    );
}


// ============================================================
// Render
// ============================================================

void ElectronDensityRenderer::render(
    const ElectronDensity& density,
    const Camera& camera,
    const Window& window
)
{
    // ========================================================
    // Construir puntos
    // ========================================================

    buildPoints(
        density
    );


    if (vertexCount <= 0)
    {
        return;
    }


    // ========================================================
    // Shader
    // ========================================================

    shader.bind();


    // ========================================================
    // MATRICES
    // ========================================================

    const glm::mat4 view =
        camera.getViewMatrix();


    const glm::mat4 projection =
        camera.getProjectionMatrix(
            window.getAspectRatio()
        );


    shader.setMat4(
        "view",
        view
    );


    shader.setMat4(
        "projection",
        projection
    );


    // ========================================================
    // POINT SIZE
    // ========================================================

    shader.setFloat(
        "pointSize",
        POINT_SIZE
    );


    // ========================================================
    // DRAW
    // ========================================================

    glBindVertexArray(
        VAO
    );


    glDrawArrays(
        GL_POINTS,
        0,
        vertexCount
    );


    glBindVertexArray(
        0
    );
}

}