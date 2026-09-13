#include "OrbitalGeometry.h"

#include <cmath>

namespace Chemistry
{

namespace
{

constexpr float PI =
    3.14159265358979323846f;


// ============================================================
// SAFE RESOLUTION
// ============================================================

int sanitizeResolution(int resolution)
{
    if (resolution < 4)
        return 4;

    return resolution;
}


// ============================================================
// GENERATE P ALONG Y
// ============================================================

std::vector<glm::vec3> generatePAlongY(
    float scale,
    int resolution
)
{
    std::vector<glm::vec3> points;

    const int rings =
        resolution;

    const int segments =
        resolution * 2;


    for (int lobe = -1;
         lobe <= 1;
         lobe += 2)
    {
        for (int i = 0;
             i <= rings;
             ++i)
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
                0.55f *
                std::sin(
                    t * PI
                );


            for (int j = 0;
                 j < segments;
                 ++j)
            {
                const float angle =
                    2.0f *
                    PI *
                    static_cast<float>(j) /
                    static_cast<float>(segments);


                points.emplace_back(
                    radius *
                        std::cos(angle),

                    y,

                    radius *
                        std::sin(angle)
                );
            }
        }
    }


    return points;
}

}


// ============================================================
// GENERATE
// ============================================================

std::vector<glm::vec3>
OrbitalGeometry::generate(
    int n,
    int l,
    float scale,
    int resolution
)
{
    resolution =
        sanitizeResolution(
            resolution
        );


    if (n < 1)
        n = 1;


    if (scale <= 0.0f)
        scale = 1.0f;


    switch (l)
    {
        case 0:
            return generateS(
                n,
                scale,
                resolution
            );


        case 1:
        {
            std::vector<glm::vec3> points;


            // ------------------------------------------------
            // px
            // ------------------------------------------------

            std::vector<glm::vec3> px =
                generateP(
                    n,
                    0,
                    scale,
                    resolution
                );


            // ------------------------------------------------
            // py
            // ------------------------------------------------

            std::vector<glm::vec3> py =
                generateP(
                    n,
                    1,
                    scale,
                    resolution
                );


            // ------------------------------------------------
            // pz
            // ------------------------------------------------

            std::vector<glm::vec3> pz =
                generateP(
                    n,
                    2,
                    scale,
                    resolution
                );


            points.insert(
                points.end(),
                px.begin(),
                px.end()
            );


            points.insert(
                points.end(),
                py.begin(),
                py.end()
            );


            points.insert(
                points.end(),
                pz.begin(),
                pz.end()
            );


            return points;
        }


        case 2:
            return generateD(
                n,
                scale,
                resolution
            );


        case 3:
            return generateF(
                n,
                scale,
                resolution
            );


        default:
            return {};
    }
}


// ============================================================
// GENERATE S
// ============================================================
//
// Aproximación visual:
//
// 1s -> una región radial
//
// 2s -> región interna + región externa
//       separadas por un nodo radial
//
// 3s -> tres regiones radiales
//
// El tamaño global aumenta aproximadamente con n².
//

std::vector<glm::vec3>
OrbitalGeometry::generateS(
    int n,
    float scale,
    int resolution
)
{
    std::vector<glm::vec3> points;


    const int rings =
        resolution;

    const int segments =
        resolution * 2;


    // ========================================================
    // ESCALA RADIAL
    // ========================================================
    //
    // No es una solución exacta para átomos multielectrónicos.
    //
    // Se utiliza para producir una representación visual
    // donde los orbitales de mayor n ocupan una región
    // espacial mayor.
    //

    const float shellScale =
        scale *
        static_cast<float>(n * n);


    // ========================================================
    // CADA REGIÓN RADIAL
    // ========================================================

    for (int shell = 0;
         shell < n;
         ++shell)
    {
        const float normalized =
            static_cast<float>(shell + 1) /
            static_cast<float>(n);


        float radius =
            shellScale *
            normalized;


        // ----------------------------------------------------
        // Ajuste para que las regiones no crezcan
        // excesivamente.
        // ----------------------------------------------------

        radius *=
            0.65f +
            0.35f *
            normalized;


        // ----------------------------------------------------
        // Esfera
        // ----------------------------------------------------

        for (int i = 0;
             i <= rings;
             ++i)
        {
            const float phi =
                PI *
                static_cast<float>(i) /
                static_cast<float>(rings);


            const float sinPhi =
                std::sin(phi);

            const float cosPhi =
                std::cos(phi);


            for (int j = 0;
                 j < segments;
                 ++j)
            {
                const float theta =
                    2.0f *
                    PI *
                    static_cast<float>(j) /
                    static_cast<float>(segments);


                points.emplace_back(
                    radius *
                        sinPhi *
                        std::cos(theta),

                    radius *
                        cosPhi,

                    radius *
                        sinPhi *
                        std::sin(theta)
                );
            }
        }
    }


    return points;
}


// ============================================================
// GENERATE P
// ============================================================
//
// axis:
//
// 0 -> px
// 1 -> py
// 2 -> pz
//
// Los tres tienen la misma estructura radial,
// pero distinta orientación espacial.
//

std::vector<glm::vec3>
OrbitalGeometry::generateP(
    int n,
    int axis,
    float scale,
    int resolution
)
{
    std::vector<glm::vec3> base =
        generatePAlongY(
            scale *
                static_cast<float>(n * n),
            resolution
        );


    std::vector<glm::vec3> points;

    points.reserve(
        base.size()
    );


    for (const glm::vec3& p : base)
    {
        glm::vec3 transformed;


        // ----------------------------------------------------
        // py
        // ----------------------------------------------------

        if (axis == 1)
        {
            transformed = p;
        }


        // ----------------------------------------------------
        // px
        //
        // Y -> X
        // ----------------------------------------------------

        else if (axis == 0)
        {
            transformed =
                glm::vec3(
                    p.y,
                    p.x,
                    p.z
                );
        }


        // ----------------------------------------------------
        // pz
        //
        // Y -> Z
        // ----------------------------------------------------

        else
        {
            transformed =
                glm::vec3(
                    p.x,
                    p.z,
                    p.y
                );
        }


        points.push_back(
            transformed
        );
    }


    return points;
}


// ============================================================
// GENERATE D
// ============================================================
//
// Representación visual aproximada.
//
// Se mantiene una geometría de cuatro lóbulos,
// pero ahora el tamaño depende de n.
//

std::vector<glm::vec3>
OrbitalGeometry::generateD(
    int n,
    float scale,
    int resolution
)
{
    std::vector<glm::vec3> points;


    const int rings =
        resolution;

    const int segments =
        resolution * 2;


    const float orbitalScale =
        scale *
        static_cast<float>(n * n);


    for (int lobe = 0;
         lobe < 4;
         ++lobe)
    {
        const float angleOffset =
            static_cast<float>(lobe) *
            (PI / 2.0f);


        const float directionX =
            std::cos(angleOffset);

        const float directionY =
            std::sin(angleOffset);


        for (int i = 0;
             i <= rings;
             ++i)
        {
            const float t =
                static_cast<float>(i) /
                static_cast<float>(rings);


            const float distance =
                t *
                orbitalScale;


            const float width =
                orbitalScale *
                0.45f *
                std::sin(
                    t * PI
                );


            for (int j = 0;
                 j < segments;
                 ++j)
            {
                const float angle =
                    2.0f *
                    PI *
                    static_cast<float>(j) /
                    static_cast<float>(segments);


                const float localX =
                    distance +
                    width *
                        std::cos(angle);


                const float localZ =
                    width *
                    std::sin(angle);


                points.emplace_back(
                    localX *
                        directionX -
                    localZ *
                        directionY,

                    localX *
                        directionY +
                    localZ *
                        directionX,

                    0.0f
                );
            }
        }
    }


    return points;
}


// ============================================================
// GENERATE F
// ============================================================
//
// Aproximación visual de mayor complejidad.
//

std::vector<glm::vec3>
OrbitalGeometry::generateF(
    int n,
    float scale,
    int resolution
)
{
    std::vector<glm::vec3> points;


    const int rings =
        resolution;

    const int segments =
        resolution * 2;


    const float orbitalScale =
        scale *
        static_cast<float>(n * n);


    for (int i = 0;
         i <= rings;
         ++i)
    {
        const float phi =
            PI *
            static_cast<float>(i) /
            static_cast<float>(rings);


        const float sinPhi =
            std::sin(phi);

        const float cosPhi =
            std::cos(phi);


        for (int j = 0;
             j < segments;
             ++j)
        {
            const float theta =
                2.0f *
                PI *
                static_cast<float>(j) /
                static_cast<float>(segments);


            const float angular =
                std::abs(
                    std::sin(
                        3.0f * theta
                    )
                );


            const float radius =
                orbitalScale *
                (0.25f +
                 0.75f * angular) *
                sinPhi;


            points.emplace_back(
                radius *
                    std::cos(theta),

                orbitalScale *
                    cosPhi *
                    0.8f,

                radius *
                    std::sin(theta)
            );
        }
    }


    return points;
}

}