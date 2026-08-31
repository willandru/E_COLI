#pragma once

#include "Nucleus.h"

#include "Camera.h"
#include "Window.h"
#include "Shader.h"

#include <glad/glad.h>

#include <glm/glm.hpp>


class NucleusRenderer
{
public:

    NucleusRenderer();

    ~NucleusRenderer();


    void render(
        const Chemistry::Nucleus& nucleus,
        const Camera& camera,
        const Window& window
    );


private:

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int indexCount;

    Shader* shader;


    void createSphere();

    void destroySphere();
};
