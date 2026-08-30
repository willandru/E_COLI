#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "ElectronDensity.h"
#include "Camera.h"
#include "Window.h"
#include "Shader.h"


namespace Chemistry
{

// ============================================================
// ElectronDensityRenderer
// ============================================================
//
// Renderiza el campo de densidad electrónica.
//
// La densidad se calcula en Ångstroms.
//
// Conversión visual:
//
//     1 Å = 0.2 unidades OpenGL
//
// ============================================================

class ElectronDensityRenderer
{
public:

    // ========================================================
    // ESCALA FÍSICA
    // ========================================================
    //
    // Esta es la única constante que necesitas modificar
    // para cambiar la escala visual del campo.
    //
    // ========================================================

    static constexpr float ANGSTROM_TO_OPENGL = 0.2f;


    // ========================================================
    // Constructor
    // ========================================================

    ElectronDensityRenderer();


    // ========================================================
    // Destructor
    // ========================================================

    ~ElectronDensityRenderer();


    // ========================================================
    // Render
    // ========================================================

    void render(
        const ElectronDensity& density,
        const Camera& camera,
        const Window& window
    );


private:

    // ========================================================
    // OpenGL
    // ========================================================

    GLuint VAO;

    GLuint VBO;

    GLsizei vertexCount;


    // ========================================================
    // Shader
    // ========================================================

    Shader shader;


    // ========================================================
    // Inicialización
    // ========================================================

    void initialize();


    // ========================================================
    // Construcción de puntos
    // ========================================================

    void buildPoints(
        const ElectronDensity& density
    );
};

}