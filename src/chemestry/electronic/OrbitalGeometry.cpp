#include "OrbitalGeometry.h"

#include "WaveFunctionS.h"
#include "RadialProbabilityS.h"
#include "DensityS.h"

#include <glm/glm.hpp>

#include <cmath>
#include <vector>
#include <algorithm>


namespace Chemistry
{

namespace
{

constexpr float PI =
    3.14159265358979323846f;


// ============================================================
// SAFE RESOLUTION
// ============================================================

int sanitizeResolution(
    int resolution
)
{
    if (resolution < 4)
        return 4;

    return resolution;
}


// ============================================================
// GENERATE SPHERE
// ============================================================

void appendSphere(
    std::vector<glm::vec3>& points,
    float radius,
    int resolution
)
{
    const int rings =
        resolution;

    const int segments =
        resolution * 2;


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


// ============================================================
// GENERATE P ALONG Y
// ============================================================

std::vector<glm::vec3>
generatePAlongY(
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
// Esta función NO resuelve directamente la ecuación de
// Schrödinger.
//
// Delegamos:
//
//     WaveFunctionS
//         -> R_n0(r)
//
//     RadialProbabilityS
//         -> P(r)
//
//     DensityS
//         -> |ψ(r)|²
//
// De esta manera OrbitalGeometry solamente transforma los
// resultados físicos en geometría.
//
// ============================================================

std::vector<glm::vec3>
OrbitalGeometry::generateS(
    int n,
    float scale,
    int resolution
)
{
    std::vector<glm::vec3> points;


    // ========================================================
    // PARAMETROS FISICOS
    // ========================================================
    //
    // r está expresado en radios de Bohr:
    //
    //     a0 = 0.529177 Å
    //
    // Posteriormente convertimos:
    //
    //     r[a0] -> unidades OpenGL
    //
    // mediante "scale".
    //

    constexpr double Z =
        1.0;


    // ========================================================
    // MAXIMOS DE PROBABILIDAD RADIAL
    // ========================================================
    //
    // Estos radios salen de:
    //
    //     P(r) = 4πr²|ψ(r)|²
    //
    // y NO de posiciones arbitrarias.
    //

    const std::vector<double> maxima =
        RadialProbabilityS::findMaxima(
            n,
            Z,
            12000
        );


    // ========================================================
    // NODOS
    // ========================================================
    //
    // Los calculamos aquí aunque por ahora no generemos una
    // geometría específica para el nodo.
    //
    // El nodo representa:
    //
    //     ψ(r) = 0
    //
    // y por tanto:
    //
    //     |ψ(r)|² = 0
    //
    const std::vector<double> nodes =
        WaveFunctionS::findNodes(
            n,
            Z,
            12000
        );


    // ========================================================
    // GENERAR LAS REGIONES DE PROBABILIDAD
    // ========================================================
    //
    // Cada máximo radial se representa visualmente como una
    // esfera.
    //
    // Esto nos permite observar:
    //
    // 1s -> 1 región
    // 2s -> 2 regiones
    // 3s -> 3 regiones
    // ...
    //
    // separadas por los nodos radiales.
    //

    for (double radiusBohr : maxima)
    {
        const float radius =
            static_cast<float>(
                radiusBohr
            )
            *
            scale;


        appendSphere(
            points,
            radius,
            resolution
        );
    }


    // ========================================================
    // NODOS
    // ========================================================
    //
    // Por ahora los nodos no se dibujan como superficies.
    //
    // Esto es intencional.
    //
    // Un nodo no es una "cáscara" con materia.
    //
    // Es una región donde:
    //
    //     ψ = 0
    //
    // y:
    //
    //     |ψ|² = 0
    //
    // Posteriormente podemos usar esta información para:
    //
    // - generar superficies nodales
    // - separar fases
    // - colorear regiones positiva/negativa
    // - visualizar la función de onda
    //
    (void)nodes;


    return points;
}


// ============================================================
// GENERATE P
// ============================================================

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
            transformed =
                p;
        }


        // ----------------------------------------------------
        // px
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
                (
                    0.25f +
                    0.75f *
                    angular
                )
                *
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