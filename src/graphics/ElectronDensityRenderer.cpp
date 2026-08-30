#include "ElectronDensityRenderer.h"

#include <vector>

#include <glm/gtc/type_ptr.hpp>


namespace Chemistry
{

namespace
{

// ============================================================
// CONFIGURACIÓN
// ============================================================

// Umbral de densidad.
//
// Todo punto con una densidad relativa menor a este valor
// será descartado.
//
// ============================================================

constexpr float DENSITY_THRESHOLD = 0.05f;


// ============================================================
// TAMAÑO DE LOS PUNTOS
// ============================================================

constexpr float POINT_SIZE = 3.0f;

}


// ============================================================
// CONSTRUCTOR
// ============================================================

ElectronDensityRenderer::ElectronDensityRenderer()
    : VAO(0),
      VBO(0),
      vertexCount(0),
      builtDensity(nullptr),
      shader(
          "shaders/electronDensity.vert",
          "shaders/electronDensity.frag"
      )
{
    initialize();
}


// ============================================================
// DESTRUCTOR
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
// INITIALIZE
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
// BUILD
// ============================================================
//
// Esta función se ejecuta solamente cuando queremos preparar
// una nueva densidad para renderización.
//
// ============================================================

void ElectronDensityRenderer::build(
    const ElectronDensity& density
)
{
    buildPoints(
        density
    );


    builtDensity =
        &density;
}


// ============================================================
// BUILD POINTS
// ============================================================

void ElectronDensityRenderer::buildPoints(
    const ElectronDensity& density
)
{
    // ========================================================
    // ESTRUCTURA DEL VÉRTICE
    // ========================================================

    struct Vertex
    {
        glm::vec3 position;

        float density;
    };


    std::vector<Vertex> vertices;


    // ========================================================
    // INFORMACIÓN DE LA REJILLA
    // ========================================================

    const glm::ivec3 resolution =
        density.getResolution();


    const float maximumDensity =
        density.getMaximumDensity();


    vertexCount = 0;


    // ========================================================
    // VALIDACIÓN
    // ========================================================

    if (maximumDensity <= 0.0f)
    {
        return;
    }


    // ========================================================
    // RESERVA APROXIMADA
    // ========================================================
    //
    // Evita múltiples realocaciones del vector.
    //
    // ========================================================

    const std::size_t estimatedSize =
        static_cast<std::size_t>(
            resolution.x
        )
        *
        static_cast<std::size_t>(
            resolution.y
        )
        *
        static_cast<std::size_t>(
            resolution.z
        );


    vertices.reserve(
        estimatedSize
    );


    // ========================================================
    // RECORRER LA REJILLA
    // ========================================================

    for (int z = 0; z < resolution.z; ++z)
    {
        for (int y = 0; y < resolution.y; ++y)
        {
            for (int x = 0; x < resolution.x; ++x)
            {
                // =================================================
                // DENSIDAD
                // =================================================

                const float value =
                    density.getDensity(
                        x,
                        y,
                        z
                    );


                // =================================================
                // NORMALIZACIÓN
                // =================================================

                const float normalizedDensity =
                    value /
                    maximumDensity;


                // =================================================
                // UMBRAL
                // =================================================

                if (
                    normalizedDensity <
                    DENSITY_THRESHOLD
                )
                {
                    continue;
                }


                // =================================================
                // POSICIÓN EN ÅNGSTROMS
                // =================================================

                const glm::vec3 positionAngstrom =
                    density.getPosition(
                        x,
                        y,
                        z
                    );


                // =================================================
                // CONVERSIÓN A OPENGL
                // =================================================

                const glm::vec3 positionOpenGL =
                    positionAngstrom *
                    ANGSTROM_TO_OPENGL;


                // =================================================
                // AGREGAR VÉRTICE
                // =================================================

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
    // CONTADOR
    // ========================================================

    vertexCount =
        static_cast<GLsizei>(
            vertices.size()
        );


    // ========================================================
    // UPLOAD GPU
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
        GL_STATIC_DRAW
    );


    // ========================================================
    // POSITION
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
        reinterpret_cast<void*>(
            0
        )
    );


    // ========================================================
    // DENSITY
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
    // UNBIND
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
// RENDER
// ============================================================
//
// IMPORTANTE:
//
// Ya NO se llama buildPoints() aquí.
//
// Por tanto, el campo no se recalcula durante cada frame.
//
// ============================================================

void ElectronDensityRenderer::render(
    const ElectronDensity& density,
    const Camera& camera,
    const Window& window
)
{
    // ========================================================
    // VERIFICAR QUE ESTA DENSIDAD ESTÁ CONSTRUIDA
    // ========================================================

    if (
        builtDensity !=
        &density
    )
    {
        return;
    }


    // ========================================================
    // VERIFICAR VÉRTICES
    // ========================================================

    if (vertexCount <= 0)
    {
        return;
    }


    // ========================================================
    // SHADER
    // ========================================================

    shader.bind();


    // ========================================================
    // VIEW
    // ========================================================

    const glm::mat4 view =
        camera.getViewMatrix();


    shader.setMat4(
        "view",
        view
    );


    // ========================================================
    // PROJECTION
    // ========================================================

    const glm::mat4 projection =
        camera.getProjectionMatrix(
            window.getAspectRatio()
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


    // ========================================================
    // UNBIND
    // ========================================================

    glBindVertexArray(
        0
    );
}

}