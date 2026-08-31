#include "NucleusRenderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include <vector>
#include <cmath>


// ============================================================
// SPHERE VERTEX
// ============================================================

struct NucleusVertex
{
    glm::vec3 position;
    glm::vec3 normal;
};


// ============================================================
// CONSTRUCTOR
// ============================================================

NucleusRenderer::NucleusRenderer()
    : VAO(0),
      VBO(0),
      EBO(0),
      indexCount(0),
      shader(nullptr)
{
    shader = new Shader(
        "../src/shaders/nucleus.vert",
        "../src/shaders/nucleus.frag"
    );

    createSphere();
}


// ============================================================
// DESTRUCTOR
// ============================================================

NucleusRenderer::~NucleusRenderer()
{
    destroySphere();

    delete shader;
}


// ============================================================
// CREATE SPHERE
// ============================================================

void NucleusRenderer::createSphere()
{
    constexpr int segments = 12;
    constexpr int rings = 8;

    std::vector<NucleusVertex> vertices;
    std::vector<unsigned int> indices;


    // ========================================================
    // VERTICES
    // ========================================================

    for (int ring = 0; ring <= rings; ++ring)
    {
        const float v =
            static_cast<float>(ring)
            /
            static_cast<float>(rings);

        const float phi =
            v * glm::pi<float>();

        const float sinPhi =
            std::sin(phi);

        const float cosPhi =
            std::cos(phi);


        for (int segment = 0;
             segment <= segments;
             ++segment)
        {
            const float u =
                static_cast<float>(segment)
                /
                static_cast<float>(segments);

            const float theta =
                u * glm::two_pi<float>();

            const float sinTheta =
                std::sin(theta);

            const float cosTheta =
                std::cos(theta);


            glm::vec3 normal(
                sinPhi * cosTheta,
                cosPhi,
                sinPhi * sinTheta
            );


            NucleusVertex vertex;

            vertex.position = normal;
            vertex.normal = normal;


            vertices.push_back(vertex);
        }
    }


    // ========================================================
    // INDICES
    // ========================================================

    for (int ring = 0; ring < rings; ++ring)
    {
        for (int segment = 0;
             segment < segments;
             ++segment)
        {
            const unsigned int current =
                ring * (segments + 1)
                +
                segment;

            const unsigned int next =
                current + segments + 1;


            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);


            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }


    indexCount =
        static_cast<unsigned int>(
            indices.size()
        );


    // ========================================================
    // VAO / VBO / EBO
    // ========================================================

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


    glBindVertexArray(VAO);


    // ========================================================
    // VBO
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(NucleusVertex),
        vertices.data(),
        GL_STATIC_DRAW
    );


    // ========================================================
    // EBO
    // ========================================================

    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        EBO
    );

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW
    );


    // ========================================================
    // POSITION
    // ========================================================

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(NucleusVertex),
        reinterpret_cast<void*>(0)
    );


    // ========================================================
    // NORMAL
    // ========================================================

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(NucleusVertex),
        reinterpret_cast<void*>(
            offsetof(
                NucleusVertex,
                normal
            )
        )
    );


    glBindVertexArray(0);
}


// ============================================================
// DESTROY SPHERE
// ============================================================

void NucleusRenderer::destroySphere()
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
// RENDER
// ============================================================

void NucleusRenderer::render(
    const Chemistry::Nucleus& nucleus,
    const Camera& camera,
    const Window& window
)
{
    if (shader == nullptr)
        return;


    shader->bind();


    // ========================================================
    // CAMERA
    // ========================================================

    const glm::mat4 view =
        camera.getViewMatrix();

    const glm::mat4 projection =
        camera.getProjectionMatrix(
            window.getAspectRatio()
        );


    shader->setMat4(
        "view",
        view
    );

    shader->setMat4(
        "projection",
        projection
    );


    // ========================================================
    // SPHERE
    // ========================================================

    glBindVertexArray(VAO);


    constexpr float radius = 0.18f;


    // ========================================================
    // PROTONS
    // ========================================================

    shader->setVec3(
        "particleColor",
        glm::vec3(
            0.85f,
            0.20f,
            0.20f
        )
    );


    for (const Chemistry::Proton& proton :
         nucleus.getProtons())
    {
        glm::mat4 model(1.0f);


        model =
            glm::translate(
                model,
                proton.getPosition()
            );


        model =
            glm::scale(
                model,
                glm::vec3(radius)
            );


        shader->setMat4(
            "model",
            model
        );


        glDrawElements(
            GL_TRIANGLES,
            indexCount,
            GL_UNSIGNED_INT,
            nullptr
        );
    }


    // ========================================================
    // NEUTRONS
    // ========================================================

    shader->setVec3(
        "particleColor",
        glm::vec3(
            0.70f,
            0.70f,
            0.75f
        )
    );


    for (const Chemistry::Neutron& neutron :
         nucleus.getNeutrons())
    {
        glm::mat4 model(1.0f);


        model =
            glm::translate(
                model,
                neutron.getPosition()
            );


        model =
            glm::scale(
                model,
                glm::vec3(radius)
            );


        shader->setMat4(
            "model",
            model
        );


        glDrawElements(
            GL_TRIANGLES,
            indexCount,
            GL_UNSIGNED_INT,
            nullptr
        );
    }


    glBindVertexArray(0);
}