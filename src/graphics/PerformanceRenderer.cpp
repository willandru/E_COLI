#include "PerformanceRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>


// ============================================================
// VERTEX SHADER
// ============================================================

static const char* TEXT_VERTEX_SHADER = R"(
#version 330 core

layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position =
        projection *
        vec4(
            vertex.xy,
            0.0,
            1.0
        );

    TexCoords =
        vertex.zw;
}
)";


// ============================================================
// FRAGMENT SHADER
// ============================================================

static const char* TEXT_FRAGMENT_SHADER = R"(
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D text;

uniform vec3 textColor;

void main()
{
    float alpha =
        texture(
            text,
            TexCoords
        ).r;

    FragColor =
        vec4(
            textColor,
            alpha
        );
}
)";


// ============================================================
// CONSTRUCTOR
// ============================================================

PerformanceRenderer::PerformanceRenderer(
    const char* fontPath
)
    : m_VAO(0),
      m_VBO(0),
      m_shaderProgram(0),
      m_projectionLocation(-1),
      m_textColorLocation(-1),
      m_visible(true)
{
    initializeShader();

    initializeFont(
        fontPath
    );


    // ========================================================
    // VAO / VBO
    // ========================================================

    glGenVertexArrays(
        1,
        &m_VAO
    );

    glGenBuffers(
        1,
        &m_VBO
    );


    glBindVertexArray(
        m_VAO
    );

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_VBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * 6 * 4,
        nullptr,
        GL_DYNAMIC_DRAW
    );


    glEnableVertexAttribArray(
        0
    );


    glVertexAttribPointer(
        0,
        4,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        nullptr
    );


    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );

    glBindVertexArray(
        0
    );


    // ========================================================
    // UNIFORM LOCATIONS
    // ========================================================

    m_projectionLocation =
        glGetUniformLocation(
            m_shaderProgram,
            "projection"
        );

    m_textColorLocation =
        glGetUniformLocation(
            m_shaderProgram,
            "textColor"
        );
}


// ============================================================
// DESTRUCTOR
// ============================================================

PerformanceRenderer::~PerformanceRenderer()
{
    for (const auto& pair : m_characters)
    {
        glDeleteTextures(
            1,
            &pair.second.textureID
        );
    }


    glDeleteBuffers(
        1,
        &m_VBO
    );


    glDeleteVertexArrays(
        1,
        &m_VAO
    );


    glDeleteProgram(
        m_shaderProgram
    );
}


// ============================================================
// INITIALIZE SHADER
// ============================================================

void PerformanceRenderer::initializeShader()
{
    // ========================================================
    // VERTEX SHADER
    // ========================================================

    unsigned int vertexShader =
        glCreateShader(
            GL_VERTEX_SHADER
        );


    glShaderSource(
        vertexShader,
        1,
        &TEXT_VERTEX_SHADER,
        nullptr
    );


    glCompileShader(
        vertexShader
    );


    // ========================================================
    // FRAGMENT SHADER
    // ========================================================

    unsigned int fragmentShader =
        glCreateShader(
            GL_FRAGMENT_SHADER
        );


    glShaderSource(
        fragmentShader,
        1,
        &TEXT_FRAGMENT_SHADER,
        nullptr
    );


    glCompileShader(
        fragmentShader
    );


    // ========================================================
    // PROGRAM
    // ========================================================

    m_shaderProgram =
        glCreateProgram();


    glAttachShader(
        m_shaderProgram,
        vertexShader
    );


    glAttachShader(
        m_shaderProgram,
        fragmentShader
    );


    glLinkProgram(
        m_shaderProgram
    );


    // ========================================================
    // CLEANUP
    // ========================================================

    glDeleteShader(
        vertexShader
    );

    glDeleteShader(
        fragmentShader
    );
}


// ============================================================
// INITIALIZE FONT
// ============================================================

void PerformanceRenderer::initializeFont(
    const char* fontPath
)
{
    FT_Library freeType;


    if (FT_Init_FreeType(
        &freeType
    ))
    {
        std::cerr
            << "Error: no se pudo inicializar FreeType.\n";

        return;
    }


    FT_Face face;


    if (FT_New_Face(
        freeType,
        fontPath,
        0,
        &face
    ))
    {
        std::cerr
            << "Error: no se pudo cargar la fuente: "
            << fontPath
            << "\n";


        FT_Done_FreeType(
            freeType
        );

        return;
    }


    // ========================================================
    // FONT SIZE
    // ========================================================

    FT_Set_Pixel_Sizes(
        face,
        0,
        18
    );


    // ========================================================
    // ALPHA TEXTURE
    // ========================================================

    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        1
    );


    // ========================================================
    // ASCII CHARACTERS
    // ========================================================

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(
            face,
            c,
            FT_LOAD_RENDER
        ))
        {
            std::cerr
                << "Advertencia: no se pudo cargar "
                << "el caracter "
                << static_cast<char>(c)
                << "\n";

            continue;
        }


        unsigned int texture;


        glGenTextures(
            1,
            &texture
        );


        glBindTexture(
            GL_TEXTURE_2D,
            texture
        );


        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );


        // ====================================================
        // TEXTURE PARAMETERS
        // ====================================================

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
        );


        // ====================================================
        // CHARACTER DATA
        // ====================================================

        Character character;


        character.textureID =
            texture;


        character.size =
            glm::ivec2(
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows
            );


        character.bearing =
            glm::ivec2(
                face->glyph->bitmap_left,
                face->glyph->bitmap_top
            );


        character.advance =
            static_cast<unsigned int>(
                face->glyph->advance.x
            );


        m_characters.insert(
            std::pair<char, Character>(
                c,
                character
            )
        );
    }


    glBindTexture(
        GL_TEXTURE_2D,
        0
    );


    FT_Done_Face(
        face
    );


    FT_Done_FreeType(
        freeType
    );
}


// ============================================================
// RENDER
// ============================================================

void PerformanceRenderer::render(
    const Performance& performance,
    const Window& window
)
{
    if (!m_visible)
        return;


    // ========================================================
    // SAVE DEPTH STATE
    // ========================================================

    GLboolean depthTestEnabled;

    glGetBooleanv(
        GL_DEPTH_TEST,
        &depthTestEnabled
    );


    glDisable(
        GL_DEPTH_TEST
    );

    // ========================================================
    // BLENDING
    // ========================================================

    glEnable(
        GL_BLEND
    );

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );


    // ========================================================
    // 2D PROJECTION
    // ========================================================

    glm::mat4 projection =
        glm::ortho(
            0.0f,
            static_cast<float>(
                window.getWidth()
            ),
            0.0f,
            static_cast<float>(
                window.getHeight()
            )
        );


    // ========================================================
    // SHADER
    // ========================================================

    glUseProgram(
        m_shaderProgram
    );


    glUniformMatrix4fv(
        m_projectionLocation,
        1,
        GL_FALSE,
        &projection[0][0]
    );


    glUniform3f(
        m_textColorLocation,
        0.85f,
        0.90f,
        0.95f
    );


    glActiveTexture(
        GL_TEXTURE0
    );


    glUniform1i(
        glGetUniformLocation(
            m_shaderProgram,
            "text"
        ),
        0
    );


    glBindVertexArray(
        m_VAO
    );


    // ========================================================
    // FPS
    // ========================================================

    std::ostringstream fpsText;

    fpsText
        << "FPS: "
        << std::fixed
        << std::setprecision(1)
        << performance.getFPS();


    // ========================================================
    // FRAME TIME
    // ========================================================

    std::ostringstream frameText;

    frameText
        << "Frame: "
        << std::fixed
        << std::setprecision(2)
        << performance.getFrameTime()
        << " ms";


    // ========================================================
    // UPDATE TIME
    // ========================================================

    std::ostringstream updateText;

    updateText
        << "Update: "
        << std::fixed
        << std::setprecision(2)
        << performance.getUpdateTime()
        << " ms";


    // ========================================================
    // RENDER TIME
    // ========================================================

    std::ostringstream renderTimeText;

    renderTimeText
        << "Render: "
        << std::fixed
        << std::setprecision(2)
        << performance.getRenderTime()
        << " ms";


    // ========================================================
    // MEMORY
    // ========================================================

    std::ostringstream memoryText;

    memoryText
        << "RAM: "
        << std::fixed
        << std::setprecision(1)
        << performance.getMemoryMB()
        << " MB";


    // ========================================================
    // POSITION
    // ========================================================

    float x = 20.0f;

    float y =
        static_cast<float>(
            window.getHeight()
        ) - 30.0f;


    float scale = 1.0f;


    // ========================================================
    // DRAW
    // ========================================================

    renderText(
        fpsText.str(),
        x,
        y,
        scale,
        glm::vec3(
            0.85f,
            0.90f,
            0.95f
        )
    );


    y -= 22.0f;


    renderText(
        frameText.str(),
        x,
        y,
        scale,
        glm::vec3(
            0.85f,
            0.90f,
            0.95f
        )
    );


    y -= 22.0f;


    renderText(
        updateText.str(),
        x,
        y,
        scale,
        glm::vec3(
            0.85f,
            0.90f,
            0.95f
        )
    );


    y -= 22.0f;


    renderText(
        renderTimeText.str(),
        x,
        y,
        scale,
        glm::vec3(
            0.85f,
            0.90f,
            0.95f
        )
    );


    y -= 22.0f;


    renderText(
        memoryText.str(),
        x,
        y,
        scale,
        glm::vec3(
            0.85f,
            0.90f,
            0.95f
        )
    );


    // ========================================================
    // CLEANUP STATE
    // ========================================================

    glBindVertexArray(
        0
    );


    glBindTexture(
        GL_TEXTURE_2D,
        0
    );


    // ========================================================
    // RESTORE DEPTH TEST
    // ========================================================

    if (depthTestEnabled)
    {
        glEnable(
            GL_DEPTH_TEST
        );
    }
}


// ============================================================
// RENDER TEXT
// ============================================================

void PerformanceRenderer::renderText(
    const std::string& text,
    float x,
    float y,
    float scale,
    const glm::vec3& color
)
{
    glUniform3f(
        m_textColorLocation,
        color.x,
        color.y,
        color.z
    );


    for (const char c : text)
    {
        const Character& character =
            m_characters[c];


        float xpos =
            x +
            character.bearing.x *
            scale;


        float ypos =
            y -
            (character.size.y -
             character.bearing.y) *
            scale;


        float width =
            character.size.x *
            scale;


        float height =
            character.size.y *
            scale;


        float vertices[6][4] =
        {
            {
                xpos,
                ypos + height,
                0.0f,
                0.0f
            },

            {
                xpos,
                ypos,
                0.0f,
                1.0f
            },

            {
                xpos + width,
                ypos,
                1.0f,
                1.0f
            },

            {
                xpos,
                ypos + height,
                0.0f,
                0.0f
            },

            {
                xpos + width,
                ypos,
                1.0f,
                1.0f
            },

            {
                xpos + width,
                ypos + height,
                1.0f,
                0.0f
            }
        };


        glBindTexture(
            GL_TEXTURE_2D,
            character.textureID
        );


        glBindBuffer(
            GL_ARRAY_BUFFER,
            m_VBO
        );


        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(vertices),
            vertices
        );


        glBindBuffer(
            GL_ARRAY_BUFFER,
            0
        );


        glDrawArrays(
            GL_TRIANGLES,
            0,
            6
        );


        x +=
            static_cast<float>(
                character.advance >> 6
            ) *
            scale;
    }
}


// ============================================================
// SET VISIBLE
// ============================================================

void PerformanceRenderer::setVisible(
    bool visible
)
{
    m_visible =
        visible;
}


// ============================================================
// IS VISIBLE
// ============================================================

bool PerformanceRenderer::isVisible() const
{
    return m_visible;
}