#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace Chemistry
{

class OrbitalGeometry
{
public:

    // ========================================================
    // GENERATE SUBLEVEL GEOMETRY
    // ========================================================
    //
    // n -> principal quantum number
    // l -> angular momentum quantum number
    //
    // l = 0 -> s
    // l = 1 -> p
    // l = 2 -> d
    // l = 3 -> f
    //

    static std::vector<glm::vec3> generate(
        int n,
        int l,
        float scale = 1.0f,
        int resolution = 32
    );


    // ========================================================
    // GENERATE INDIVIDUAL P ORBITAL
    // ========================================================
    //
    // axis:
    //
    // 0 -> px
    // 1 -> py
    // 2 -> pz
    //

    static std::vector<glm::vec3> generateP(
        int n,
        int axis,
        float scale,
        int resolution
    );


private:

    static std::vector<glm::vec3> generateS(
        int n,
        float scale,
        int resolution
    );

    static std::vector<glm::vec3> generateD(
        int n,
        float scale,
        int resolution
    );

    static std::vector<glm::vec3> generateF(
        int n,
        float scale,
        int resolution
    );
};

}