#pragma once

#include "ElectronicStructure.h"

#include "Camera.h"
#include "Window.h"
#include "Shader.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <vector>


class ElectronicRenderer
{
public:

    ElectronicRenderer();

    ~ElectronicRenderer();


    void render(
        const Chemistry::ElectronicStructure& electronicStructure,
        const glm::vec3& position,
        const Camera& camera,
        const Window& window
    );


private:

    unsigned int VAO;
    unsigned int VBO;

    unsigned int pointCount;

    Shader* shader;


    void createGeometry(
        const Chemistry::ElectronicStructure& electronicStructure
    );

    void destroyGeometry();
};