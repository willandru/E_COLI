#include "OrbitalGeometry.h"

#include <cmath>

namespace Chemistry
{

std::vector<glm::vec3> OrbitalGeometry::generate(
    int l,
    float scale,
    int resolution
)
{
    if (resolution < 4)
        resolution = 4;

    if (scale <= 0.0f)
        scale = 1.0f;

    switch (l)
    {
        case 0:
            return generateS(scale, resolution);

        case 1:
            return generateP(scale, resolution);

        case 2:
            return generateD(scale, resolution);

        case 3:
            return generateF(scale, resolution);

        default:
            return {};
    }
}

std::vector<glm::vec3> OrbitalGeometry::generateS(
    float scale,
    int resolution
)
{
    std::vector<glm::vec3> points;

    const int rings = resolution;
    const int segments = resolution * 2;

    for (int i = 0; i <= rings; ++i)
    {
        const float phi =
            static_cast<float>(i) /
            static_cast<float>(rings);

        const float theta =
            phi * 3.14159265359f;

        const float sinTheta = std::sin(theta);
        const float cosTheta = std::cos(theta);

        for (int j = 0; j < segments; ++j)
        {
            const float angle =
                2.0f * 3.14159265359f *
                static_cast<float>(j) /
                static_cast<float>(segments);

            points.emplace_back(
                scale * sinTheta * std::cos(angle),
                scale * cosTheta,
                scale * sinTheta * std::sin(angle)
            );
        }
    }

    return points;
}

std::vector<glm::vec3> OrbitalGeometry::generateP(
    float scale,
    int resolution
)
{
    std::vector<glm::vec3> points;

    const int rings = resolution;
    const int segments = resolution * 2;

    /*
        p orbital:

            (+) lóbulo
               |
               |
               ●
               |
               |
            (-) lóbulo

        La forma se orienta inicialmente
        sobre el eje Y.
    */

    for (int lobe = -1; lobe <= 1; lobe += 2)
    {
        for (int i = 0; i <= rings; ++i)
        {
            const float t =
                static_cast<float>(i) /
                static_cast<float>(rings);

            const float y =
                static_cast<float>(lobe) *
                t *
                scale;

            const float radius =
                scale *
                std::sin(t * 3.14159265359f) *
                0.55f;

            for (int j = 0; j < segments; ++j)
            {
                const float angle =
                    2.0f * 3.14159265359f *
                    static_cast<float>(j) /
                    static_cast<float>(segments);

                points.emplace_back(
                    radius * std::cos(angle),
                    y,
                    radius * std::sin(angle)
                );
            }
        }
    }

    return points;
}

std::vector<glm::vec3> OrbitalGeometry::generateD(
    float scale,
    int resolution
)
{
    std::vector<glm::vec3> points;

    const int rings = resolution;
    const int segments = resolution * 2;

    /*
        Representación aproximada del orbital d
        mediante cuatro lóbulos en el plano XY.
    */

    for (int lobe = 0; lobe < 4; ++lobe)
    {
        const float angleOffset =
            static_cast<float>(lobe) *
            (3.14159265359f / 2.0f);

        const float directionX =
            std::cos(angleOffset);

        const float directionY =
            std::sin(angleOffset);

        for (int i = 0; i <= rings; ++i)
        {
            const float t =
                static_cast<float>(i) /
                static_cast<float>(rings);

            const float distance =
                t * scale;

            const float width =
                scale *
                0.45f *
                std::sin(t * 3.14159265359f);

            for (int j = 0; j < segments; ++j)
            {
                const float angle =
                    2.0f * 3.14159265359f *
                    static_cast<float>(j) /
                    static_cast<float>(segments);

                const float localX =
                    distance +
                    width * std::cos(angle);

                const float localZ =
                    width * std::sin(angle);

                points.emplace_back(
                    localX * directionX -
                    localZ * directionY,

                    localX * directionY +
                    localZ * directionX,

                    0.0f
                );
            }
        }
    }

    return points;
}

std::vector<glm::vec3> OrbitalGeometry::generateF(
    float scale,
    int resolution
)
{
    std::vector<glm::vec3> points;

    const int rings = resolution;
    const int segments = resolution * 2;

    /*
        Representación visual aproximada de un
        orbital f mediante una distribución
        angular de mayor complejidad.
    */

    for (int i = 0; i <= rings; ++i)
    {
        const float phi =
            3.14159265359f *
            static_cast<float>(i) /
            static_cast<float>(rings);

        const float sinPhi = std::sin(phi);
        const float cosPhi = std::cos(phi);

        for (int j = 0; j < segments; ++j)
        {
            const float theta =
                2.0f * 3.14159265359f *
                static_cast<float>(j) /
                static_cast<float>(segments);

            /*
                Patrón angular aproximado para
                producir una forma f.
            */

            const float angular =
                std::abs(
                    std::sin(3.0f * theta)
                );

            const float radius =
                scale *
                (0.25f + 0.75f * angular) *
                sinPhi;

            points.emplace_back(
                radius * std::cos(theta),
                scale * cosPhi * 0.8f,
                radius * std::sin(theta)
            );
        }
    }

    return points;
}

}