#include "BondRenderer.h"
#include "Atom.h"

#include <vector>
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chemistry
{

namespace
{

constexpr float BOND_RADIUS = 0.12f;

constexpr int CYLINDER_SEGMENTS = 16;

constexpr float PI =
    3.14159265358979323846f;

}


// ============================================================
// CONSTRUCTOR
// ============================================================

BondRenderer::BondRenderer()

    :
    VAO(0),
    VBO(0),
    EBO(0),
    indexCount(0),
    shader(
        "shaders/bond.vert",
        "shaders/bond.frag"
    )

{
    initialize();
}


// ============================================================
// DESTRUCTOR
// ============================================================

BondRenderer::~BondRenderer()
{
    if (EBO != 0)
    {
        glDeleteBuffers(
            1,
            &EBO
        );
    }

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

void BondRenderer::initialize()
{
    buildCylinder(
        CYLINDER_SEGMENTS
    );
}


// ============================================================
// BUILD CYLINDER
// ============================================================

void BondRenderer::buildCylinder(
    int segments
)
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
    };


    std::vector<Vertex> vertices;

    std::vector<unsigned int> indices;


    for (int i = 0; i < segments; ++i)
    {
        float angle =
            2.0f *
            PI *
            static_cast<float>(i) /
            static_cast<float>(segments);

        float x =
            std::cos(angle);

        float y =
            std::sin(angle);


        vertices.push_back(
            {
                glm::vec3(
                    x,
                    y,
                    0.0f
                ),

                glm::vec3(
                    x,
                    y,
                    0.0f
                )
            }
        );


        vertices.push_back(
            {
                glm::vec3(
                    x,
                    y,
                    1.0f
                ),

                glm::vec3(
                    x,
                    y,
                    0.0f
                )
            }
        );
    }


    for (int i = 0; i < segments; ++i)
    {
        int next =
            (i + 1) % segments;


        unsigned int bottomCurrent =
            static_cast<unsigned int>(
                i * 2
            );

        unsigned int topCurrent =
            static_cast<unsigned int>(
                i * 2 + 1
            );

        unsigned int bottomNext =
            static_cast<unsigned int>(
                next * 2
            );

        unsigned int topNext =
            static_cast<unsigned int>(
                next * 2 + 1
            );


        indices.push_back(
            bottomCurrent
        );

        indices.push_back(
            bottomNext
        );

        indices.push_back(
            topCurrent
        );


        indices.push_back(
            topCurrent
        );

        indices.push_back(
            bottomNext
        );

        indices.push_back(
            topNext
        );
    }


    indexCount =
        static_cast<int>(
            indices.size()
        );


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
        &EBO
    );


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
        vertices.data(),
        GL_STATIC_DRAW
    );


    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        EBO
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


    glEnableVertexAttribArray(
        1
    );

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(
            sizeof(glm::vec3)
        )
    );


    glBindVertexArray(0);
}


// ============================================================
// TRANSFORM
// ============================================================

glm::mat4 BondRenderer::calculateTransform(
    const glm::vec3& start,
    const glm::vec3& end,
    float radius
) const
{
    glm::vec3 direction =
        end - start;

    float length =
        glm::length(direction);


    if (length <= 0.000001f)
    {
        return glm::mat4(1.0f);
    }


    glm::vec3 normalizedDirection =
        direction / length;


    glm::vec3 referenceAxis(
        0.0f,
        0.0f,
        1.0f
    );


    glm::quat rotation =
        glm::rotation(
            referenceAxis,
            normalizedDirection
        );


    glm::mat4 transform(
        1.0f
    );


    transform =
        glm::translate(
            transform,
            start
        );


    transform *=
        glm::mat4_cast(
            rotation
        );


    transform =
        glm::scale(
            transform,
            glm::vec3(
                radius,
                radius,
                length
            )
        );


    return transform;
}


// ============================================================
// RENDER CYLINDER
// ============================================================

void BondRenderer::renderCylinder(
    const glm::vec3& start,
    const glm::vec3& end,
    float radius,
    const Camera& camera,
    const Window& window
)
{
    glm::mat4 model =
        calculateTransform(
            start,
            end,
            radius
        );


    shader.setMat4(
        "model",
        model
    );

    shader.setMat4(
        "view",
        camera.getViewMatrix()
    );

    shader.setMat4(
        "projection",
        camera.getProjectionMatrix(
            window.getAspectRatio()
        )
    );


    shader.setVec3(
        "bondColor",
        glm::vec3(
            0.65f,
            0.65f,
            0.65f
        )
    );


    glBindVertexArray(
        VAO
    );


    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        nullptr
    );


    glBindVertexArray(0);
}


// ============================================================
// RENDER
// ============================================================

void BondRenderer::render(
    const Bond& bond,
    const Camera& camera,
    const Window& window
)
{
    const Atom& atomA =
        bond.getAtomA();

    const Atom& atomB =
        bond.getAtomB();


    const glm::vec3& start =
        atomA.getPosition();

    const glm::vec3& end =
        atomB.getPosition();


    glm::vec3 direction =
        end - start;


    if (glm::length(direction) <= 0.000001f)
    {
        return;
    }


    shader.bind();


    // ========================================================
    // SINGLE
    // ========================================================

    if (
        bond.getType() ==
        BondType::Single
    )
    {
        renderCylinder(
            start,
            end,
            BOND_RADIUS,
            camera,
            window
        );
    }


    // ========================================================
    // DOUBLE
    // ========================================================

    else if (
        bond.getType() ==
        BondType::Double
    )
    {
        direction =
            glm::normalize(direction);


        glm::vec3 offset =
            glm::cross(
                direction,
                glm::vec3(
                    0.0f,
                    1.0f,
                    0.0f
                )
            );


        if (glm::length(offset) < 0.000001f)
        {
            offset =
                glm::cross(
                    direction,
                    glm::vec3(
                        1.0f,
                        0.0f,
                        0.0f
                    )
                );
        }


        offset =
            glm::normalize(offset) *
            BOND_RADIUS *
            1.8f;


        renderCylinder(
            start + offset,
            end + offset,
            BOND_RADIUS,
            camera,
            window
        );


        renderCylinder(
            start - offset,
            end - offset,
            BOND_RADIUS,
            camera,
            window
        );
    }


    // ========================================================
    // TRIPLE
    // ========================================================

    else if (
        bond.getType() ==
        BondType::Triple
    )
    {
        direction =
            glm::normalize(direction);


        glm::vec3 offset =
            glm::cross(
                direction,
                glm::vec3(
                    0.0f,
                    1.0f,
                    0.0f
                )
            );


        if (glm::length(offset) < 0.000001f)
        {
            offset =
                glm::cross(
                    direction,
                    glm::vec3(
                        1.0f,
                        0.0f,
                        0.0f
                    )
                );
        }


        offset =
            glm::normalize(offset) *
            BOND_RADIUS *
            2.0f;


        renderCylinder(
            start,
            end,
            BOND_RADIUS,
            camera,
            window
        );


        renderCylinder(
            start + offset,
            end + offset,
            BOND_RADIUS,
            camera,
            window
        );


        renderCylinder(
            start - offset,
            end - offset,
            BOND_RADIUS,
            camera,
            window
        );
    }
}

}