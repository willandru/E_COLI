#include "DNARenderer.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>


// ============================================================
// ESTRUCTURA DE INSTANCIA
// ============================================================
//
// Cada instancia contiene:
//
// position.x
// position.y
// position.z
//
// base
//
// position = posición espacial de la representación
// base     = base nitrogenada representativa
//
// ============================================================

struct DNAInstance
{
    glm::vec3 position;

    unsigned char base;
};


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
      m_totalBases(
          static_cast<unsigned int>(
              dna.getSequence().size()
          )
      ),
      m_baseStep(1),
      m_lastCameraPosition(
          1000000.0f,
          1000000.0f,
          1000000.0f
      ),
      m_lastLOD(0),
      m_shader(
          "shaders/dna.vert",
          "shaders/dna.frag"
      ),
      m_dna(dna)
{
    // ========================================================
    // Geometría
    // ========================================================

    createGeometry();


    // ========================================================
    // Instancias
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
    // Position del quad
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


    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );


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
    // Reservar inicialmente un pequeño buffer
    // ========================================================
    //
    // NO mandamos aquí los 4.6 millones.
    //
    // El contenido real será construido según
    // la posición de la cámara.
    //
    // ========================================================

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(DNAInstance),
        nullptr,
        GL_DYNAMIC_DRAW
    );


    // ========================================================
    // Position
    //
    // location = 1
    // ========================================================

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(DNAInstance),
        reinterpret_cast<void*>(
            offsetof(
                DNAInstance,
                position
            )
        )
    );


    glEnableVertexAttribArray(
        1
    );


    glVertexAttribDivisor(
        1,
        1
    );


    // ========================================================
    // Base
    //
    // location = 2
    // ========================================================

    glVertexAttribIPointer(
        2,
        1,
        GL_UNSIGNED_BYTE,
        sizeof(DNAInstance),
        reinterpret_cast<void*>(
            offsetof(
                DNAInstance,
                base
            )
        )
    );


    glEnableVertexAttribArray(
        2
    );


    glVertexAttribDivisor(
        2,
        1
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


    // ========================================================
    // No renderizamos todo el genoma inicialmente.
    // ========================================================

    m_instanceCount = 0;
}


// ============================================================
// CALCULATE LOD
// ============================================================

unsigned int DNARenderer::calculateLOD(
    float distance
) const
{
    // ========================================================
    // LOD 0
    //
    // Muy cerca:
    //
    // 1 base = 1 instancia
    // ========================================================

    if (distance < 5.0f)
    {
        return 1;
    }


    // ========================================================
    // LOD 1
    //
    // 100 bases = 1 instancia
    // ========================================================

    if (distance < 20.0f)
    {
        return 100;
    }


    // ========================================================
    // LOD 2
    //
    // 1.000 bases = 1 instancia
    // ========================================================

    if (distance < 100.0f)
    {
        return 1000;
    }


    // ========================================================
    // LOD 3
    //
    // 10.000 bases = 1 instancia
    // ========================================================

    return 10000;
}


// ============================================================
// UPDATE INSTANCES
// ============================================================

void DNARenderer::updateInstances(
    const Camera& camera
)
{
    // ========================================================
    // Posición de cámara
    // ========================================================

    const glm::vec3 cameraPosition =
        camera.getPosition();


    // ========================================================
    // Distancia al origen del DNA
    // ========================================================

    float distance =
        glm::length(
            cameraPosition
        );


    // ========================================================
    // Determinar LOD
    // ========================================================

    unsigned int newLOD =
        calculateLOD(
            distance
        );


    // ========================================================
    // Determinar si realmente necesitamos actualizar
    // ========================================================

    float cameraMovement =
        glm::length(
            cameraPosition -
            m_lastCameraPosition
        );


    if (
        newLOD == m_lastLOD &&
        cameraMovement < 0.5f
    )
    {
        return;
    }


    m_lastLOD =
        newLOD;


    m_lastCameraPosition =
        cameraPosition;


    m_baseStep =
        newLOD;


    // ========================================================
    // SECUENCIA
    // ========================================================

    const std::string& sequence =
        m_dna.getSequence();


    if (sequence.empty())
    {
        m_instanceCount = 0;

        return;
    }


    // ========================================================
    // RADIO DE VISIBILIDAD
    // ========================================================
    //
    // No necesitamos construir todo el genoma.
    //
    // Solamente una región alrededor de la cámara.
    //
    // ========================================================

    float visibleRadius;


    if (m_baseStep == 1)
    {
        visibleRadius = 10.0f;
    }
    else if (m_baseStep == 100)
    {
        visibleRadius = 30.0f;
    }
    else if (m_baseStep == 1000)
    {
        visibleRadius = 100.0f;
    }
    else
    {
        visibleRadius = 300.0f;
    }


    // ========================================================
    // CONVERSIÓN:
    //
    // posición espacial
    //       ↓
    // índice de base
    //
    // Actualmente:
    //
    // 1 base = 0.05 unidades
    //
    // ========================================================

    constexpr float spacing =
        0.05f;


    // ========================================================
    // Índice aproximado correspondiente
    // a la posición X de la cámara
    // ========================================================

    long long centerIndex =
        static_cast<long long>(
            std::floor(
                cameraPosition.x /
                spacing
            )
        );


    // ========================================================
    // Cantidad de bases correspondientes
    // al radio visible.
    // ========================================================

    long long radiusBases =
        static_cast<long long>(
            std::ceil(
                visibleRadius /
                spacing
            )
        );


    // ========================================================
    // Rango de bases que necesitamos
    // ========================================================

    long long startIndex =
        centerIndex -
        radiusBases;


    long long endIndex =
        centerIndex +
        radiusBases;


    // ========================================================
    // Limitar al genoma
    // ========================================================

    startIndex =
        std::max(
            0LL,
            startIndex
        );


    endIndex =
        std::min(
            static_cast<long long>(
                m_totalBases
            ) - 1,
            endIndex
        );


    // ========================================================
    // Si estamos fuera del genoma
    // ========================================================

    if (
        startIndex > endIndex
    )
    {
        m_instanceCount = 0;

        return;
    }


    // ========================================================
    // Crear instancias visibles
    // ========================================================

    std::vector<DNAInstance> instances;


    long long numberOfBases =
        endIndex -
        startIndex +
        1;


    long long estimatedInstances =
        (
            numberOfBases +
            m_baseStep -
            1
        ) /
        m_baseStep;


    instances.reserve(
        static_cast<size_t>(
            estimatedInstances
        )
    );


    // ========================================================
    // Construir representación
    // ========================================================

    for (
        long long index = startIndex;
        index <= endIndex;
        index += m_baseStep
    )
    {
        // ====================================================
        // Índice representativo del grupo
        // ====================================================

        long long representativeIndex =
            index +
            (
                m_baseStep /
                2
            );


        if (
            representativeIndex >
            endIndex
        )
        {
            representativeIndex =
                endIndex;
        }


        // ====================================================
        // Posición espacial
        // ====================================================

        float x =
            static_cast<float>(
                representativeIndex
            ) *
            spacing;


        // ====================================================
        // Base representativa
        // ====================================================

        unsigned char base =
            static_cast<unsigned char>(
                sequence[
                    representativeIndex
                ]
            );


        // ====================================================
        // Crear instancia
        // ====================================================

        DNAInstance instance;

        instance.position =
            glm::vec3(
                x,
                0.0f,
                0.0f
            );


        instance.base =
            base;


        instances.push_back(
            instance
        );
    }


    // ========================================================
    // Subir únicamente las instancias necesarias
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_instanceVBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            instances.size() *
            sizeof(DNAInstance)
        ),
        instances.data(),
        GL_DYNAMIC_DRAW
    );


    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );


    // ========================================================
    // Número real de instancias
    // ========================================================

    m_instanceCount =
        static_cast<unsigned int>(
            instances.size()
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
    // Actualizar representación
    // ========================================================

    updateInstances(
        camera
    );


    // ========================================================
    // Si no hay nada visible
    // ========================================================

    if (m_instanceCount == 0)
    {
        return;
    }


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
    // Parámetros visuales
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