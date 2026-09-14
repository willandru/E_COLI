#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <vector>


class Shader;


class ElectronicRenderer
{
public:

    ElectronicRenderer();

    ~ElectronicRenderer();


    // ============================================================
    // INITIALIZATION
    // ============================================================

    void initialize();


    // ============================================================
    // GEOMETRY
    // ============================================================

    void setPositions(
        const std::vector<glm::vec3>& positions);

    void setValues(
        const std::vector<float>& values);

    void setWaveFunctionValues(
        const std::vector<float>& waveFunctionValues);


    // ============================================================
    // TRANSFORMATION
    // ============================================================

    void setModelMatrix(
        const glm::mat4& model);

    void setViewMatrix(
        const glm::mat4& view);

    void setProjectionMatrix(
        const glm::mat4& projection);


    // ============================================================
    // APPEARANCE
    // ============================================================

    void setPointSize(float pointSize);

    void setPositiveColor(
        const glm::vec3& color);

    void setNegativeColor(
        const glm::vec3& color);


    // ============================================================
    // RENDER
    // ============================================================

    void render();


    // ============================================================
    // INFORMATION
    // ============================================================

    std::size_t getPointCount() const;

    bool isInitialized() const;


private:

    unsigned int VAO = 0;

    unsigned int VBO = 0;

    unsigned int densityVBO = 0;

    unsigned int waveFunctionVBO = 0;


    Shader* shader = nullptr;


    /*
     * Position of each visible orbital point.
     */

    std::vector<glm::vec3> positions;


    /*
     * Probability density:
     *
     *      |psi|^2
     */

    std::vector<float> values;


    /*
     * Original wave function:
     *
     *      psi
     *
     * The sign is preserved.
     */

    std::vector<float> waveFunctionValues;


    glm::mat4 modelMatrix;

    glm::mat4 viewMatrix;

    glm::mat4 projectionMatrix;


    float pointSize = 3.0f;


    /*
     * Color used for positive wave-function phase.
     */

    glm::vec3 positiveColor =
        glm::vec3(0.2f, 0.6f, 1.0f);


    /*
     * Color used for negative wave-function phase.
     */

    glm::vec3 negativeColor =
        glm::vec3(1.0f, 0.25f, 0.25f);


    bool initialized = false;


    void createBuffers();

    void uploadGeometry();

    void destroyBuffers();
};