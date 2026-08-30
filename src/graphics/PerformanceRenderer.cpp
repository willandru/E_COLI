#include "PerformanceRenderer.h"

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>


// ============================================================
// TEXT VERTEX SHADER
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
// TEXT FRAGMENT SHADER
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
    const char* fontPath,
    const DNA& dna
)
    :
      m_VAO(0),
      m_VBO(0),
      m_shaderProgram(0),

      m_projectionLocation(-1),
      m_textColorLocation(-1),
      m_textSamplerLocation(-1),

      m_visible(true),

      m_dna(dna)
{
    initializeShader();

    initializeFont(
        fontPath
    );


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


    if (m_shaderProgram != 0)
    {
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


        m_textSamplerLocation =
            glGetUniformLocation(
                m_shaderProgram,
                "text"
            );
    }
}


// ============================================================
// DESTRUCTOR
// ============================================================

PerformanceRenderer::~PerformanceRenderer()
{
    for (const auto& pair : m_characters)
    {
        if (pair.second.textureID != 0)
        {
            glDeleteTextures(
                1,
                &pair.second.textureID
            );
        }
    }


    if (m_VBO != 0)
    {
        glDeleteBuffers(
            1,
            &m_VBO
        );
    }


    if (m_VAO != 0)
    {
        glDeleteVertexArrays(
            1,
            &m_VAO
        );
    }


    if (m_shaderProgram != 0)
    {
        glDeleteProgram(
            m_shaderProgram
        );
    }
}


// ============================================================
// INITIALIZE SHADER
// ============================================================

void PerformanceRenderer::initializeShader()
{
    const unsigned int vertexShader =
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


    int success = 0;

    char infoLog[1024];


    glGetShaderiv(
        vertexShader,
        GL_COMPILE_STATUS,
        &success
    );


    if (!success)
    {
        glGetShaderInfoLog(
            vertexShader,
            1024,
            nullptr,
            infoLog
        );


        std::cerr
            << "Error compilando Performance vertex shader:\n"
            << infoLog
            << '\n';


        glDeleteShader(
            vertexShader
        );

        return;
    }


    const unsigned int fragmentShader =
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


    glGetShaderiv(
        fragmentShader,
        GL_COMPILE_STATUS,
        &success
    );


    if (!success)
    {
        glGetShaderInfoLog(
            fragmentShader,
            1024,
            nullptr,
            infoLog
        );


        std::cerr
            << "Error compilando Performance fragment shader:\n"
            << infoLog
            << '\n';


        glDeleteShader(
            vertexShader
        );


        glDeleteShader(
            fragmentShader
        );

        return;
    }


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


    glGetProgramiv(
        m_shaderProgram,
        GL_LINK_STATUS,
        &success
    );


    if (!success)
    {
        glGetProgramInfoLog(
            m_shaderProgram,
            1024,
            nullptr,
            infoLog
        );


        std::cerr
            << "Error enlazando Performance shader:\n"
            << infoLog
            << '\n';


        glDeleteProgram(
            m_shaderProgram
        );


        m_shaderProgram = 0;
    }


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
            << "Error: no se pudo cargar la fuente:\n"
            << fontPath
            << '\n';


        FT_Done_FreeType(
            freeType
        );

        return;
    }


    FT_Set_Pixel_Sizes(
        face,
        0,
        18
    );


    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        1
    );


    for (unsigned char c = 0; c < 128; ++c)
    {
        if (FT_Load_Char(
            face,
            c,
            FT_LOAD_RENDER
        ))
        {
            continue;
        }


        unsigned int texture = 0;


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


        m_characters.emplace(
            static_cast<char>(c),
            character
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
    {
        return;
    }


    if (m_shaderProgram == 0)
    {
        return;
    }


    GLboolean depthTestEnabled =
        GL_FALSE;

    GLboolean blendEnabled =
        GL_FALSE;


    glGetBooleanv(
        GL_DEPTH_TEST,
        &depthTestEnabled
    );


    glGetBooleanv(
        GL_BLEND,
        &blendEnabled
    );


    glDisable(
        GL_DEPTH_TEST
    );


    glEnable(
        GL_BLEND
    );


    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );


    const glm::mat4 projection =
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


    glUseProgram(
        m_shaderProgram
    );


    glUniformMatrix4fv(
        m_projectionLocation,
        1,
        GL_FALSE,
        &projection[0][0]
    );


    glUniform1i(
        m_textSamplerLocation,
        0
    );


    glActiveTexture(
        GL_TEXTURE0
    );


    glBindVertexArray(
        m_VAO
    );


    const glm::vec3 color(
        0.85f,
        0.90f,
        0.95f
    );


    // ========================================================
    // DNA INFORMATION
    // ========================================================

    const std::size_t dnaBases =
        m_dna.getLength();


    const double dnaMbp =
        static_cast<double>(
            dnaBases
        ) / 1'000'000.0;


    const double dnaGpuMB =
        static_cast<double>(
            dnaBases
        ) / (1024.0 * 1024.0);


    // ========================================================
    // FPS
    // ========================================================

    std::ostringstream fpsText;

    fpsText
        << "FPS        "
        << std::fixed
        << std::setprecision(1)
        << performance.getFPS();


    // ========================================================
    // FRAME TIME
    // ========================================================

    std::ostringstream frameText;

    frameText
        << "Frame      "
        << std::fixed
        << std::setprecision(2)
        << performance.getFrameTime()
        << " ms";


    // ========================================================
    // UPDATE TIME
    // ========================================================

    std::ostringstream updateText;

    updateText
        << "Update     "
        << std::fixed
        << std::setprecision(2)
        << performance.getUpdateTime()
        << " ms";


    // ========================================================
    // RENDER TIME
    // ========================================================

    std::ostringstream renderCpuText;

    renderCpuText
        << "Render CPU "
        << std::fixed
        << std::setprecision(2)
        << performance.getRenderTime()
        << " ms";


    // ========================================================
    // RAM
    // ========================================================

    std::ostringstream ramText;

    ramText
        << "RAM        "
        << std::fixed
        << std::setprecision(1)
        << performance.getMemoryMB()
        << " MB";


    // ========================================================
    // DNA BASES
    // ========================================================

    std::ostringstream basesText;

    basesText
        << "DNA        "
        << dnaBases
        << " bp";


    // ========================================================
    // DNA MBP
    // ========================================================

    std::ostringstream mbpText;

    mbpText
        << "DNA        "
        << std::fixed
        << std::setprecision(3)
        << dnaMbp
        << " Mbp";


    // ========================================================
    // DNA GPU MEMORY
    // ========================================================

    std::ostringstream gpuText;

    gpuText
        << "DNA GPU    "
        << std::fixed
        << std::setprecision(2)
        << dnaGpuMB
        << " MB";


    // ========================================================
    // POSITION
    // ========================================================

    const float x =
        20.0f;


    float y =
        static_cast<float>(
            window.getHeight()
        ) - 28.0f;


    const float lineHeight =
        21.0f;


    // ========================================================
    // DRAW
    // ========================================================

    renderText(
        fpsText.str(),
        x,
        y,
        1.0f,
        color
    );


    y -= lineHeight;


    renderText(
        frameText.str(),
        x,
        y,
        1.0f,
        color
    );


    y -= lineHeight;


    renderText(
        updateText.str(),
        x,
        y,
        1.0f,
        color
    );


    y -= lineHeight;


    renderText(
        renderCpuText.str(),
        x,
        y,
        1.0f,
        color
    );


    y -= lineHeight;


    renderText(
        ramText.str(),
        x,
        y,
        1.0f,
        color
    );


    y -= lineHeight;


    renderText(
        basesText.str(),
        x,
        y,
        1.0f,
        color
    );


    y -= lineHeight;


    renderText(
        mbpText.str(),
        x,
        y,
        1.0f,
        color
    );


    y -= lineHeight;


    renderText(
        gpuText.str(),
        x,
        y,
        1.0f,
        color
    );


    glBindVertexArray(
        0
    );


    glBindTexture(
        GL_TEXTURE_2D,
        0
    );


    glUseProgram(
        0
    );


    if (depthTestEnabled)
    {
        glEnable(
            GL_DEPTH_TEST
        );
    }
    else
    {
        glDisable(
            GL_DEPTH_TEST
        );
    }


    if (blendEnabled)
    {
        glEnable(
            GL_BLEND
        );
    }
    else
    {
        glDisable(
            GL_BLEND
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
    if (m_shaderProgram == 0)
    {
        return;
    }


    glUniform3f(
        m_textColorLocation,
        color.x,
        color.y,
        color.z
    );


    for (const char c : text)
    {
        const auto iterator =
            m_characters.find(c);


        if (iterator ==
            m_characters.end())
        {
            continue;
        }


        const Character& character =
            iterator->second;


        const float xpos =
            x +
            static_cast<float>(
                character.bearing.x
            ) *
            scale;


        const float ypos =
            y -
            static_cast<float>(
                character.size.y -
                character.bearing.y
            ) *
            scale;


        const float width =
            static_cast<float>(
                character.size.x
            ) *
            scale;


        const float height =
            static_cast<float>(
                character.size.y
            ) *
            scale;


        const float vertices[6][4] =
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


    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );
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