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

    // ========================================================
    // CONSTRUCTOR
    // ========================================================

    ElectronicRenderer();


    // ========================================================
    // DESTRUCTOR
    // ========================================================

    ~ElectronicRenderer();


    // ========================================================
    // RENDER
    // ========================================================

    void render(
        const Chemistry::ElectronicStructure& electronicStructure,
        const glm::vec3& position,
        const Camera& camera,
        const Window& window
    );


private:

    // ========================================================
    // DRAW BATCH
    // ========================================================
    //
    // Cada batch representa un orbital/subnivel independiente.
    //
    // Ejemplos:
    //
    // 1s
    // 2s
    // 2px
    // 2py
    // 2pz
    //
    // first  -> posición inicial dentro del VBO
    // count  -> cantidad de puntos
    // color  -> color de ese nivel
    //

    struct DrawBatch
    {
        unsigned int first;
        unsigned int count;

        glm::vec3 color;
    };


    // ========================================================
    // OPENGL
    // ========================================================

    unsigned int VAO;

    unsigned int VBO;


    // ========================================================
    // TOTAL POINTS
    // ========================================================

    unsigned int pointCount;


    // ========================================================
    // DRAW BATCHES
    // ========================================================

    std::vector<DrawBatch> batches;


    // ========================================================
    // SHADER
    // ========================================================

    Shader* shader;


    // ========================================================
    // GEOMETRY
    // ========================================================

    void createGeometry(
        const Chemistry::ElectronicStructure&
            electronicStructure
    );


    void destroyGeometry();


    // ========================================================
    // COLOR
    // ========================================================

    glm::vec3 getLevelColor(
        int n
    ) const;
};