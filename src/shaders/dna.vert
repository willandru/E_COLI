#version 330 core

// ============================================================
// INPUT
// ============================================================

layout(location = 0) in vec2 vertexPosition;

layout(location = 1) in uint base;


// ============================================================
// MATRICES
// ============================================================

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


// ============================================================
// PARÁMETROS
// ============================================================

uniform float particleRadius;
uniform float particleSpacing;


// ============================================================
// OUTPUT
// ============================================================

flat out uint baseType;

out vec2 localPosition;

out vec3 particleViewPosition;


// ============================================================
// MAIN
// ============================================================

void main()
{
    // ========================================================
    // Índice de instancia
    // ========================================================

    uint instanceID =
        uint(gl_InstanceID);


    // ========================================================
    // Posición de la base
    // ========================================================

    float x =
        float(instanceID) *
        particleSpacing;


    vec3 particlePosition =
        vec3(
            x,
            0.0,
            0.0
        );


    // ========================================================
    // Centro de la partícula en VIEW SPACE
    // ========================================================

    vec4 viewCenter =
        view *
        model *
        vec4(
            particlePosition,
            1.0
        );


    particleViewPosition =
        viewCenter.xyz;


    // ========================================================
    // BILLBOARD
    //
    // El quad se construye directamente en
    // coordenadas de cámara.
    // ========================================================

    vec4 vertexViewPosition =
        viewCenter;


    vertexViewPosition.xy +=
        vertexPosition *
        particleRadius;


    // ========================================================
    // PROYECCIÓN
    // ========================================================

    gl_Position =
        projection *
        vertexViewPosition;


    // ========================================================
    // Coordenadas locales
    // ========================================================

    localPosition =
        vertexPosition;


    // ========================================================
    // Base
    // ========================================================

    baseType =
        base;
}