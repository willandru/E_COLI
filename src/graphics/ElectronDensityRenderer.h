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
// Renderiza un campo de densidad electrónica.
//
// La densidad se calcula en Ångstroms.
//
// Conversión:
//
//     1 Å = 0.2 unidades OpenGL
//
// La geometría se construye una sola vez mediante:
//
//     build()
//
// Posteriormente:
//
//     render()
//
// solamente envía los vértices existentes a la GPU.
//
// ============================================================

class ElectronDensityRenderer
{
public:

    // ========================================================
    // ESCALA FÍSICA
    // ========================================================

    static constexpr float ANGSTROM_TO_OPENGL = 0.2f;


    // ========================================================
    // CONSTRUCTOR
    // ========================================================

    ElectronDensityRenderer();


    // ========================================================
    // DESTRUCTOR
    // ========================================================

    ~ElectronDensityRenderer();


    // ========================================================
    // BUILD
    // ========================================================
    //
    // Construye la nube de puntos de una densidad.
    //
    // IMPORTANTE:
    //
    // Esta función debe llamarse una sola vez por densidad,
    // antes del loop principal.
    //
    // ========================================================

    void build(
        const ElectronDensity& density
    );


    // ========================================================
    // RENDER
    // ========================================================
    //
    // Renderiza la densidad previamente construida.
    //
    // ========================================================

    void render(
        const ElectronDensity& density,
        const Camera& camera,
        const Window& window
    );


private:

    // ========================================================
    // DATOS DE OPENGL
    // ========================================================

    GLuint VAO;

    GLuint VBO;

    GLsizei vertexCount;


    // ========================================================
    // DENSIDAD ASOCIADA
    // ========================================================
    //
    // El renderer necesita saber qué densidad fue construida
    // para poder dibujarla correctamente.
    //
    // ========================================================

    const ElectronDensity* builtDensity;


    // ========================================================
    // SHADER
    // ========================================================

    Shader shader;


    // ========================================================
    // INICIALIZACIÓN
    // ========================================================

    void initialize();


    // ========================================================
    // CONSTRUCCIÓN INTERNA
    // ========================================================

    void buildPoints(
        const ElectronDensity& density
    );
};

}