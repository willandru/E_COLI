#pragma once

#include "Performance.h"
#include "Window.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <string>


class PerformanceRenderer
{
public:

    PerformanceRenderer(
        const char* fontPath
    );

    ~PerformanceRenderer();


    // ========================================================
    // RENDER
    // ========================================================

    void render(
        const Performance& performance,
        const Window& window
    );


    // ========================================================
    // ENABLE / DISABLE
    // ========================================================

    void setVisible(
        bool visible
    );

    bool isVisible() const;


private:

    // ========================================================
    // CHARACTER
    // ========================================================

    struct Character
    {
        unsigned int textureID;

        glm::ivec2 size;

        glm::ivec2 bearing;

        unsigned int advance;
    };


    // ========================================================
    // INITIALIZATION
    // ========================================================

    void initializeFont(
        const char* fontPath
    );

    void initializeShader();


    // ========================================================
    // TEXT
    // ========================================================

    void renderText(
        const std::string& text,
        float x,
        float y,
        float scale,
        const glm::vec3& color
    );


    // ========================================================
    // OPENGL
    // ========================================================

    unsigned int m_VAO;

    unsigned int m_VBO;

    unsigned int m_shaderProgram;


    // ========================================================
    // UNIFORMS
    // ========================================================

    int m_projectionLocation;

    int m_textColorLocation;


    // ========================================================
    // FONT
    // ========================================================

    std::map<char, Character> m_characters;


    // ========================================================
    // STATE
    // ========================================================

    bool m_visible;
};