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

    PerformanceRenderer(
        const char* fontPath,
        const DNA& dna
    );


    ~PerformanceRenderer();


    void render(
        const Performance& performance,
        const Window& window
    );


    void setVisible(
        bool visible
    );


    bool isVisible() const;


private:

    void initializeShader();


    void initializeFont(
        const char* fontPath
    );


    void renderText(
        const std::string& text,
        float x,
        float y,
        float scale,
        const glm::vec3& color
    );


private:

    unsigned int m_VAO;

    unsigned int m_VBO;

    unsigned int m_shaderProgram;


    int m_projectionLocation;

    int m_textColorLocation;

    int m_textSamplerLocation;


    std::map<char, Character> m_characters;


    bool m_visible;


    const DNA& m_dna;
};