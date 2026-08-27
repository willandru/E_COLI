#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>


// ============================================================
// CONSTRUCTOR
// ============================================================

Shader::Shader(
    const char* vertexSource,
    const char* fragmentSource
)
    : m_program(0)
{
    // ========================================================
    // Compilar shaders
    // ========================================================

    unsigned int vertexShader =
        compileShader(
            GL_VERTEX_SHADER,
            vertexSource
        );


    unsigned int fragmentShader =
        compileShader(
            GL_FRAGMENT_SHADER,
            fragmentSource
        );


    // ========================================================
    // Crear programa
    // ========================================================

    m_program =
        glCreateProgram();


    glAttachShader(
        m_program,
        vertexShader
    );


    glAttachShader(
        m_program,
        fragmentShader
    );


    glLinkProgram(
        m_program
    );


    // ========================================================
    // Comprobar linking
    // ========================================================

    int success;

    char infoLog[512];


    glGetProgramiv(
        m_program,
        GL_LINK_STATUS,
        &success
    );


    if (!success)
    {
        glGetProgramInfoLog(
            m_program,
            512,
            nullptr,
            infoLog
        );

        std::cerr
            << "Error enlazando Shader Program:\n"
            << infoLog
            << '\n';
    }


    // ========================================================
    // Los shaders ya no son necesarios
    // ========================================================

    glDeleteShader(
        vertexShader
    );

    glDeleteShader(
        fragmentShader
    );
}


// ============================================================
// DESTRUCTOR
// ============================================================

Shader::~Shader()
{
    if (m_program != 0)
    {
        glDeleteProgram(
            m_program
        );
    }
}


// ============================================================
// BIND
// ============================================================

void Shader::bind() const
{
    glUseProgram(
        m_program
    );
}


// ============================================================
// COMPILE SHADER
// ============================================================

unsigned int Shader::compileShader(
    unsigned int type,
    const char* source
)
{
    unsigned int shader =
        glCreateShader(type);


    glShaderSource(
        shader,
        1,
        &source,
        nullptr
    );


    glCompileShader(
        shader
    );


    int success;

    char infoLog[512];


    glGetShaderiv(
        shader,
        GL_COMPILE_STATUS,
        &success
    );


    if (!success)
    {
        glGetShaderInfoLog(
            shader,
            512,
            nullptr,
            infoLog
        );

        std::cerr
            << "Error compilando shader:\n"
            << infoLog
            << '\n';
    }


    return shader;
}


// ============================================================
// SET MAT4
// ============================================================

void Shader::setMat4(
    const char* name,
    const glm::mat4& value
) const
{
    glUniformMatrix4fv(
        getUniformLocation(name),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}


// ============================================================
// SET VEC3
// ============================================================

void Shader::setVec3(
    const char* name,
    const glm::vec3& value
) const
{
    glUniform3fv(
        getUniformLocation(name),
        1,
        glm::value_ptr(value)
    );
}


// ============================================================
// GET UNIFORM LOCATION
// ============================================================

int Shader::getUniformLocation(
    const char* name
) const
{
    return glGetUniformLocation(
        m_program,
        name
    );
}