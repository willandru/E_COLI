#include <glad/glad.h>

#include "AtomRenderer.h"

#include <cmath>
#include <vector>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ============================================================
// ESCALA FÍSICA
// ============================================================
//
// AtomData utiliza Ångström.
//
// 1 Å = 0.01 unidades de simulación
//
// Por lo tanto:
//
// H  = 1.20 Å -> 0.012
// C  = 1.70 Å -> 0.017
// N  = 1.55 Å -> 0.0155
// O  = 1.52 Å -> 0.0152
// P  = 1.80 Å -> 0.018
// S  = 1.80 Å -> 0.018
//
// ============================================================

namespace
{
constexpr float ANGSTROM_SCALE = 0.01f;
}

// ============================================================
// Constructor
// ============================================================

AtomRenderer::AtomRenderer()
:
m_shader(
"shaders/atom.vert",
"shaders/atom.frag"
),
m_VAO(0),
m_VBO(0),
m_EBO(0),
m_indexCount(0)
{
createSphere();
}

// ============================================================
// Destructor
// ============================================================

AtomRenderer::~AtomRenderer()
{
destroySphere();
}

// ============================================================
// Create sphere
// ============================================================

void AtomRenderer::createSphere()
{
constexpr unsigned int segments = 24;
constexpr unsigned int rings = 16;

std::vector<float> vertices;
std::vector<unsigned int> indices;


// ========================================================
// Vertices
// ========================================================

for (
    unsigned int ring = 0;
    ring <= rings;
    ++ring
)
{
    const float v =
        static_cast<float>(ring) /
        static_cast<float>(rings);

    const float phi =
        v * glm::pi<float>();

    const float sinPhi =
        std::sin(phi);

    const float cosPhi =
        std::cos(phi);


    for (
        unsigned int segment = 0;
        segment <= segments;
        ++segment
    )
    {
        const float u =
            static_cast<float>(segment) /
            static_cast<float>(segments);

        const float theta =
            u * glm::two_pi<float>();

        const float sinTheta =
            std::sin(theta);

        const float cosTheta =
            std::cos(theta);


        glm::vec3 position(
            sinPhi * cosTheta,
            cosPhi,
            sinPhi * sinTheta
        );


        glm::vec3 normal =
            glm::normalize(position);


        vertices.push_back(position.x);
        vertices.push_back(position.y);
        vertices.push_back(position.z);

        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);
    }
}


// ========================================================
// Indices
// ========================================================

for (
    unsigned int ring = 0;
    ring < rings;
    ++ring
)
{
    for (
        unsigned int segment = 0;
        segment < segments;
        ++segment
    )
    {
        const unsigned int current =
            ring * (segments + 1) +
            segment;

        const unsigned int next =
            current +
            segments +
            1;


        indices.push_back(current);
        indices.push_back(next);
        indices.push_back(current + 1);

        indices.push_back(current + 1);
        indices.push_back(next);
        indices.push_back(next + 1);
    }
}


m_indexCount =
    static_cast<int>(
        indices.size()
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


// ========================================================
// EBO
// ========================================================

glGenBuffers(
    1,
    &m_EBO
);


glBindVertexArray(
    m_VAO
);


// ========================================================
// Vertex buffer
// ========================================================

glBindBuffer(
    GL_ARRAY_BUFFER,
    m_VBO
);


glBufferData(
    GL_ARRAY_BUFFER,
    static_cast<GLsizeiptr>(
        vertices.size() *
        sizeof(float)
    ),
    vertices.data(),
    GL_STATIC_DRAW
);


// ========================================================
// Index buffer
// ========================================================

glBindBuffer(
    GL_ELEMENT_ARRAY_BUFFER,
    m_EBO
);


glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    static_cast<GLsizeiptr>(
        indices.size() *
        sizeof(unsigned int)
    ),
    indices.data(),
    GL_STATIC_DRAW
);


// ========================================================
// Position attribute
// ========================================================

glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    6 * sizeof(float),
    nullptr
);

glEnableVertexAttribArray(0);


// ========================================================
// Normal attribute
// ========================================================

glVertexAttribPointer(
    1,
    3,
    GL_FLOAT,
    GL_FALSE,
    6 * sizeof(float),
    reinterpret_cast<void*>(
        3 * sizeof(float)
    )
);

glEnableVertexAttribArray(1);


// ========================================================
// Unbind
// ========================================================

glBindVertexArray(0);


}

// ============================================================
// Destroy sphere
// ============================================================

void AtomRenderer::destroySphere()
{
if (m_EBO != 0)
{
glDeleteBuffers(
1,
&m_EBO
);


    m_EBO = 0;
}


if (m_VBO != 0)
{
    glDeleteBuffers(
        1,
        &m_VBO
    );

    m_VBO = 0;
}


if (m_VAO != 0)
{
    glDeleteVertexArrays(
        1,
        &m_VAO
    );

    m_VAO = 0;
}

}

// ============================================================
// Atom color
// ============================================================

glm::vec3 AtomRenderer::getAtomColor(
const Chemistry::AtomData& data
) const
{
switch (data.atomicNumber)
{
case 1:
return glm::vec3(
1.0f,
1.0f,
1.0f
);

    case 6:
        return glm::vec3(
            0.20f,
            0.20f,
            0.20f
        );

    case 7:
        return glm::vec3(
            0.15f,
            0.30f,
            1.0f
        );

    case 8:
        return glm::vec3(
            1.0f,
            0.10f,
            0.10f
        );

    case 15:
        return glm::vec3(
            1.0f,
            0.55f,
            0.10f
        );

    case 16:
        return glm::vec3(
            1.0f,
            0.90f,
            0.20f
        );

    default:
        return glm::vec3(
            0.7f,
            0.7f,
            0.7f
        );
}


}

// ============================================================
// Render
// ============================================================

void AtomRenderer::render(
const Chemistry::Atom& atom,
const Camera& camera,
const Window& window
)
{
const Chemistry::AtomData& data =
atom.getData();

const glm::vec3& position =
    atom.getPosition();


// ========================================================
// Model
// ========================================================

glm::mat4 model(1.0f);


model =
    glm::translate(
        model,
        position
    );


// ========================================================
// Radio atómico
// ========================================================
//
// AtomData:
//
// vanDerWaalsRadius -> Å
//
// Conversión:
//
// Å -> unidades de simulación
//
// ========================================================

const float radius =
    data.vanDerWaalsRadius *
    ANGSTROM_SCALE;


model =
    glm::scale(
        model,
        glm::vec3(radius)
    );


// ========================================================
// Shader
// ========================================================

m_shader.bind();


m_shader.setMat4(
    "model",
    model
);


m_shader.setMat4(
    "view",
    camera.getViewMatrix()
);


m_shader.setMat4(
    "projection",
    camera.getProjectionMatrix(
        window.getAspectRatio()
    )
);


m_shader.setVec3(
    "atomColor",
    getAtomColor(data)
);


// ========================================================
// Draw
// ========================================================

glBindVertexArray(
    m_VAO
);


glDrawElements(
    GL_TRIANGLES,
    m_indexCount,
    GL_UNSIGNED_INT,
    nullptr
);


glBindVertexArray(0);


}
