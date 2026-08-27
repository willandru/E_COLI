#pragma once

#include <string>

#include <glm/glm.hpp>


class Shader
{
public:

    Shader(
        const char* vertexSource,
        const char* fragmentSource
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


    unsigned int compileShader(
        unsigned int type,
        const char* source
    );


    int getUniformLocation(
        const char* name
    ) const;
};
