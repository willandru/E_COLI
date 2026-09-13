#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace Chemistry
{

class OrbitalGeometry
{
public:
    static std::vector<glm::vec3> generate(
        int l,
        float scale = 1.0f,
        int resolution = 32
    );

private:
    static std::vector<glm::vec3> generateS(
        float scale,
        int resolution
    );

    static std::vector<glm::vec3> generateP(
        float scale,
        int resolution
    );

    static std::vector<glm::vec3> generateD(
        float scale,
        int resolution
    );

    static std::vector<glm::vec3> generateF(
        float scale,
        int resolution
    );
};

}