#pragma once

#include "Performance.h"
#include "Window.h"
#include "DNA.h"

#include <glm/glm.hpp>

#include <map>
#include <string>


// ============================================================
// CHARACTER
// ============================================================

struct Character
{
    unsigned int textureID;

    glm::ivec2 size;

    glm::ivec2 bearing;

    unsigned int advance;
};


// ============================================================
// PERFORMANCE RENDERER
// ============================================================

class PerformanceRenderer
{
public:

    // ========================================================
    // Constructor
    // ========================================================

    PerformanceRenderer(
        const char* fontPath,
        const DNA& dna
    );


    // ========================================================
    // Destructor
    // ========================================================

    ~PerformanceRenderer();


    // ========================================================
    // Render
    // ========================================================

    void render(
        const Performance& performance,
        const Window& window
    );


    // ========================================================
    // Visibility
    // ========================================================

    void setVisible(
        bool visible
    );


    bool isVisible() const;


private:

    // ========================================================
    // Shader
    // ========================================================

    void initializeShader();


    // ========================================================
    // Font
    // ========================================================

    void initializeFont(
        const char* fontPath
    );


    // ========================================================
    // Text
    // ========================================================

    void renderText(
        const std::string& text,
        float x,
        float y,
        float scale,
        const glm::vec3& color
    );


private:

    // ========================================================
    // OpenGL
    // ========================================================

    unsigned int m_VAO;

    unsigned int m_VBO;

    unsigned int m_shaderProgram;


    // ========================================================
    // Uniforms
    // ========================================================

    int m_projectionLocation;

    int m_textColorLocation;

    int m_textSamplerLocation;


    // ========================================================
    // Font
    // ========================================================

    std::map<char, Character> m_characters;


    // ========================================================
    // Estado
    // ========================================================

    bool m_visible;


    // ========================================================
    // DNA
    // ========================================================

    const DNA& m_dna;
};