#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>


// ============================================================
// CONSTRUCTOR
// ============================================================

Shader::Shader(
    const char* vertexPath,
    const char* fragmentPath
)
    : m_program(0)
{
    // ========================================================
    // Cargar archivos
    // ========================================================

    std::string vertexSource =
        loadFile(vertexPath);


    std::string fragmentSource =
        loadFile(fragmentPath);


    // ========================================================
    // Verificar archivos
    // ========================================================

    if (vertexSource.empty())
    {
        std::cerr
            << "Error: no se pudo cargar vertex shader:\n"
            << vertexPath
            << '\n';

        return;
    }


    if (fragmentSource.empty())
    {
        std::cerr
            << "Error: no se pudo cargar fragment shader:\n"
            << fragmentPath
            << '\n';

        return;
    }


    // ========================================================
    // Compilar vertex shader
    // ========================================================

    unsigned int vertexShader =
        compileShader(
            GL_VERTEX_SHADER,
            vertexSource.c_str()
        );


    if (vertexShader == 0)
    {
        return;
    }


    // ========================================================
    // Compilar fragment shader
    // ========================================================

    unsigned int fragmentShader =
        compileShader(
            GL_FRAGMENT_SHADER,
            fragmentSource.c_str()
        );


    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);

        return;
    }


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


        glDeleteProgram(
            m_program
        );


        m_program = 0;
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
// LOAD FILE
// ============================================================

std::string Shader::loadFile(
    const char* path
)
{
    std::ifstream file(
        path,
        std::ios::in
    );


    if (!file.is_open())
    {
        std::cerr
            << "Error: no se pudo abrir shader:\n"
            << path
            << '\n';

        return "";
    }


    std::stringstream buffer;


    buffer << file.rdbuf();


    file.close();


    return buffer.str();
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
        glCreateShader(
            type
        );


    // ========================================================
    // Fuente
    // ========================================================

    glShaderSource(
        shader,
        1,
        &source,
        nullptr
    );


    // ========================================================
    // Compilación
    // ========================================================

    glCompileShader(
        shader
    );


    // ========================================================
    // Comprobar compilación
    // ========================================================

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


        glDeleteShader(
            shader
        );


        return 0;
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
// SET FLOAT
// ============================================================

void Shader::setFloat(
    const char* name,
    float value
) const
{
    glUniform1f(
        getUniformLocation(name),
        value
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