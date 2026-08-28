#pragma once

#include "Shader.h"
#include "Camera.h"
#include "Window.h"
#include "DNA.h"


class DNARenderer
{
public:

    // ========================================================
    // Constructor
    // ========================================================

    DNARenderer(
        const DNA& dna
    );


    // ========================================================
    // Destructor
    // ========================================================

    ~DNARenderer();


    // ========================================================
    // Render
    // ========================================================

    void render(
        const Camera& camera,
        const Window& window
    );


private:

    // ========================================================
    // Crear geometría
    // ========================================================

    void createGeometry();


    // ========================================================
    // Crear datos de instancias
    // ========================================================

    void createInstances(
        const DNA& dna
    );


private:

    unsigned int m_VAO;

    unsigned int m_VBO;

    unsigned int m_instanceVBO;

    unsigned int m_instanceCount;

    Shader m_shader;
};