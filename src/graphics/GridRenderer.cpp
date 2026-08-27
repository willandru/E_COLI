#include "GridRenderer.h"

#include <glm/glm.hpp>


// ============================================================
// CONSTRUCTOR
// ============================================================

GridRenderer::GridRenderer()
    : m_shader(
        "shaders/grid.vert",
        "shaders/grid.frag"
    )
{
}


// ============================================================
// RENDER
// ============================================================

void GridRenderer::render(
    Grid& grid,
    const Camera& camera,
    const Window& window
)
{
    // ========================================================
    // Matrices
    // ========================================================

    float aspectRatio =
        window.getAspectRatio();


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


    m_shader.setVec3(
        "color",
        glm::vec3(
            0.25f,
            0.30f,
            0.35f
        )
    );


    // ========================================================
    // Grid
    // ========================================================

    grid.render(
        m_shader
    );
}