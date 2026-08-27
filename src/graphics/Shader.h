#pragma once


#include <string>

#include <glm/glm.hpp>


class Shader
{
public:

    // ========================================================
    // Constructor
    // ========================================================

    Shader(
        const char* vertexPath,
        const char* fragmentPath
    );


    ~Shader();


    // ========================================================
    // Activar shader
    // ========================================================

    void bind() const;


    // ========================================================
    // Uniforms
    // ========================================================

    void setMat4(
        const char* name,
        const glm::mat4& value
    ) const;


    void setVec3(
        const char* name,
        const glm::vec3& value
    ) const;


private:

    unsigned int m_program;


    // ========================================================
    // Leer archivo
    // ========================================================

    std::string loadFile(
        const char* path
    );


    // ========================================================
    // Compilación
    // ========================================================

    unsigned int compileShader(
        unsigned int type,
        const char* source
    );


    // ========================================================
    // Uniforms
    // ========================================================

    int getUniformLocation(
        const char* name
    ) const;
};