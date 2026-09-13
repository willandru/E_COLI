#include "ElectronicRenderer.h"

#include "OrbitalGeometry.h"

#include <glm/gtc/matrix_transform.hpp>

#include <vector>


// ============================================================
// CONSTRUCTOR
// ============================================================

ElectronicRenderer::ElectronicRenderer()
    : VAO(0),
      VBO(0),
      pointCount(0),
      shader(nullptr)
{
    shader = new Shader(
        "../src/shaders/electronic.vert",
        "../src/shaders/electronic.frag"
    );
}


// ============================================================
// DESTRUCTOR
// ============================================================

ElectronicRenderer::~ElectronicRenderer()
{
    destroyGeometry();

    delete shader;
}


// ============================================================
// CREATE GEOMETRY
// ============================================================

void ElectronicRenderer::createGeometry(
    const Chemistry::ElectronicStructure& electronicStructure
)
{
    std::vector<glm::vec3> points;


    // ========================================================
    // RECORRER NIVELES
    // ========================================================

    for (const Chemistry::EnergyLevel& level :
         electronicStructure.getLevels())
    {
        // ====================================================
        // RECORRER SUBNIVELES
        // ====================================================

        for (const Chemistry::Sublevel& sublevel :
             level.sublevels)
        {
            const int l = sublevel.l;


            // =================================================
            // GENERAR FORMA DEL SUBNIVEL
            // =================================================

            std::vector<glm::vec3> geometry =
                Chemistry::OrbitalGeometry::generate(
                    l,
                    1.0f,
                    24
                );


            points.insert(
                points.end(),
                geometry.begin(),
                geometry.end()
            );
        }
    }


    if (points.empty())
        return;


    pointCount =
        static_cast<unsigned int>(
            points.size()
        );


    // ========================================================
    // VAO
    // ========================================================

    glGenVertexArrays(
        1,
        &VAO
    );


    // ========================================================
    // VBO
    // ========================================================

    glGenBuffers(
        1,
        &VBO
    );


    glBindVertexArray(VAO);


    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        points.size() * sizeof(glm::vec3),
        points.data(),
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
        sizeof(glm::vec3),
        reinterpret_cast<void*>(0)
    );


    glBindVertexArray(0);
}


// ============================================================
// DESTROY GEOMETRY
// ============================================================

void ElectronicRenderer::destroyGeometry()
{
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


    pointCount = 0;
}


// ============================================================
// RENDER
// ============================================================

void ElectronicRenderer::render(
    const Chemistry::ElectronicStructure& electronicStructure,
    const glm::vec3& position,
    const Camera& camera,
    const Window& window
)
{
    if (shader == nullptr)
        return;


    // ========================================================
    // CREAR GEOMETRÍA
    // ========================================================

    if (VAO == 0)
    {
        createGeometry(
            electronicStructure
        );
    }


    if (VAO == 0 || pointCount == 0)
        return;


    // ========================================================
    // SHADER
    // ========================================================

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
    // MODEL
    // ========================================================

    glm::mat4 model(1.0f);

    model =
        glm::translate(
            model,
            position
        );


    shader->setMat4(
        "model",
        model
    );


    // ========================================================
    // COLOR
    // ========================================================

    shader->setVec3(
        "particleColor",
        glm::vec3(
            0.30f,
            0.65f,
            1.00f
        )
    );


    // ========================================================
    // POINT SIZE
    // ========================================================

    shader->setFloat(
        "pointSize",
        3.0f
    );


    // ========================================================
    // DRAW
    // ========================================================

    glBindVertexArray(VAO);


    glDrawArrays(
        GL_POINTS,
        0,
        pointCount
    );


    glBindVertexArray(0);
}