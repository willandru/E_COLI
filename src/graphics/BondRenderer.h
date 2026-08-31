#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "Bond.h"
#include "Camera.h"
#include "Window.h"
#include "Shader.h"

namespace Chemistry
{

class BondRenderer
{
public:

    BondRenderer();

    ~BondRenderer();

    void render(
        const Bond& bond,
        const Camera& camera,
        const Window& window
    );


private:

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    int indexCount;

    Shader shader;


    void initialize();

    void buildCylinder(
        int segments = 16
    );

    void renderCylinder(
        const glm::vec3& start,
        const glm::vec3& end,
        float radius,
        const Camera& camera,
        const Window& window
    );

    glm::mat4 calculateTransform(
        const glm::vec3& start,
        const glm::vec3& end,
        float radius
    ) const;
};

}