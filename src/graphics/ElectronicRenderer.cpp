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
      batches(),
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
// LEVEL COLOR
// ============================================================
//
// El color depende del nivel principal n.
//
// Importante:
//
// Esto es solamente una codificación visual.
// No significa que los orbitales tengan realmente
// esos colores físicamente.
//

glm::vec3 ElectronicRenderer::getLevelColor(
    int n
) const
{
    switch (n)
    {
        case 1:
            return glm::vec3(
                0.20f,
                0.80f,
                1.00f
            );

        case 2:
            return glm::vec3(
                0.30f,
                1.00f,
                0.45f
            );

        case 3:
            return glm::vec3(
                1.00f,
                0.75f,
                0.20f
            );

        case 4:
            return glm::vec3(
                1.00f,
                0.35f,
                0.25f
            );

        case 5:
            return glm::vec3(
                0.85f,
                0.35f,
                1.00f
            );

        case 6:
            return glm::vec3(
                1.00f,
                0.35f,
                0.70f
            );

        case 7:
            return glm::vec3(
                0.55f,
                0.55f,
                1.00f
            );

        default:
            return glm::vec3(
                1.00f,
                1.00f,
                1.00f
            );
    }
}


// ============================================================
// CREATE GEOMETRY
// ============================================================

void ElectronicRenderer::createGeometry(
    const Chemistry::ElectronicStructure&
        electronicStructure
)
{
    std::vector<glm::vec3> points;

    batches.clear();


    // ========================================================
    // RECORRER NIVELES
    // ========================================================

    for (
        const Chemistry::EnergyLevel& level :
        electronicStructure.getLevels()
    )
    {
        const int n =
            level.n;


        const glm::vec3 color =
            getLevelColor(n);


        // ====================================================
        // RECORRER SUBNIVELES
        // ====================================================

        for (
            const Chemistry::Sublevel& sublevel :
            level.sublevels
        )
        {
            const int l =
                sublevel.l;


            // =================================================
            // SUBNIVEL S
            // =================================================
            //
            // s tiene solamente un orbital.
            //
            // 1s
            // 2s
            // 3s
            // ...

            if (l == 0)
            {
                std::vector<glm::vec3> geometry =
                    Chemistry::OrbitalGeometry::generate(
                        n,
                        l,
                        1.0f,
                        24
                    );


                if (!geometry.empty())
                {
                    const unsigned int first =
                        static_cast<unsigned int>(
                            points.size()
                        );


                    points.insert(
                        points.end(),
                        geometry.begin(),
                        geometry.end()
                    );


                    const unsigned int count =
                        static_cast<unsigned int>(
                            geometry.size()
                        );


                    batches.push_back(
                        {
                            first,
                            count,
                            color
                        }
                    );
                }
            }


            // =================================================
            // SUBNIVEL P
            // =================================================
            //
            // p contiene tres orbitales:
            //
            // px
            // py
            // pz
            //
            // El nuevo ElectronicStructure ya contiene
            // esos tres orbitales mediante m:
            //
            // m = -1
            // m =  0
            // m = +1
            //

            else if (l == 1)
            {
                for (
                    const Chemistry::Orbital& orbital :
                    sublevel.orbitals
                )
                {
                    if (orbital.electronCount <= 0)
                        continue;


                    int axis = 1;


                    // ----------------------------------------
                    // m = -1 -> px
                    // m =  0 -> py
                    // m = +1 -> pz
                    // ----------------------------------------

                    if (orbital.m == -1)
                    {
                        axis = 0;
                    }
                    else if (orbital.m == 0)
                    {
                        axis = 1;
                    }
                    else
                    {
                        axis = 2;
                    }


                    std::vector<glm::vec3> geometry =
                        Chemistry::OrbitalGeometry::generateP(
                            n,
                            axis,
                            1.0f,
                            24
                        );


                    if (geometry.empty())
                        continue;


                    const unsigned int first =
                        static_cast<unsigned int>(
                            points.size()
                        );


                    points.insert(
                        points.end(),
                        geometry.begin(),
                        geometry.end()
                    );


                    const unsigned int count =
                        static_cast<unsigned int>(
                            geometry.size()
                        );


                    batches.push_back(
                        {
                            first,
                            count,
                            color
                        }
                    );
                }
            }


            // =================================================
            // SUBNIVEL D
            // =================================================
            //
            // Por ahora utilizamos la geometría d general.
            //
            // Posteriormente podremos separar los cinco
            // orbitales d individuales.
            //

            else if (l == 2)
            {
                std::vector<glm::vec3> geometry =
                    Chemistry::OrbitalGeometry::generate(
                        n,
                        l,
                        1.0f,
                        24
                    );


                if (!geometry.empty())
                {
                    const unsigned int first =
                        static_cast<unsigned int>(
                            points.size()
                        );


                    points.insert(
                        points.end(),
                        geometry.begin(),
                        geometry.end()
                    );


                    const unsigned int count =
                        static_cast<unsigned int>(
                            geometry.size()
                        );


                    batches.push_back(
                        {
                            first,
                            count,
                            color
                        }
                    );
                }
            }


            // =================================================
            // SUBNIVEL F
            // =================================================

            else if (l == 3)
            {
                std::vector<glm::vec3> geometry =
                    Chemistry::OrbitalGeometry::generate(
                        n,
                        l,
                        1.0f,
                        24
                    );


                if (!geometry.empty())
                {
                    const unsigned int first =
                        static_cast<unsigned int>(
                            points.size()
                        );


                    points.insert(
                        points.end(),
                        geometry.begin(),
                        geometry.end()
                    );


                    const unsigned int count =
                        static_cast<unsigned int>(
                            geometry.size()
                        );


                    batches.push_back(
                        {
                            first,
                            count,
                            color
                        }
                    );
                }
            }
        }
    }


    // ========================================================
    // NO GEOMETRY
    // ========================================================

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


    glBindVertexArray(
        VAO
    );


    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        points.size() *
            sizeof(glm::vec3),
        points.data(),
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
        sizeof(glm::vec3),
        reinterpret_cast<void*>(0)
    );


    // ========================================================
    // UNBIND
    // ========================================================

    glBindVertexArray(
        0
    );
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

    batches.clear();
}


// ============================================================
// RENDER
// ============================================================

void ElectronicRenderer::render(
    const Chemistry::ElectronicStructure&
        electronicStructure,

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


    if (
        VAO == 0 ||
        pointCount == 0 ||
        batches.empty()
    )
    {
        return;
    }


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

    glm::mat4 model(
        1.0f
    );


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
    // POINT SIZE
    // ========================================================

    shader->setFloat(
        "pointSize",
        3.0f
    );


    // ========================================================
    // DRAW
    // ========================================================

    glBindVertexArray(
        VAO
    );


    for (
        const DrawBatch& batch :
        batches
    )
    {
        // ----------------------------------------------------
        // COLOR DEL NIVEL
        // ----------------------------------------------------

        shader->setVec3(
            "particleColor",
            batch.color
        );


        // ----------------------------------------------------
        // DRAW ORBITAL
        // ----------------------------------------------------

        glDrawArrays(
            GL_POINTS,
            static_cast<GLint>(
                batch.first
            ),
            static_cast<GLsizei>(
                batch.count
            )
        );
    }


    // ========================================================
    // UNBIND
    // ========================================================

    glBindVertexArray(
        0
    );
}