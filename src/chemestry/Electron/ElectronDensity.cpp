#include "ElectronDensity.h"

#include <algorithm>
#include <cmath>


namespace Chemistry
{

namespace
{
    constexpr int MINIMUM_RESOLUTION = 2;

    constexpr float EPSILON = 1.0e-6f;
}


// ============================================================
// CONSTRUCTOR
// ============================================================

ElectronDensity::ElectronDensity(
    const glm::vec3& origin,
    const glm::vec3& size,
    const glm::ivec3& resolution
)
    : origin(origin),
      size(size),
      resolution(resolution),
      spacing(0.0f),
      maximumDensity(0.0f)
{
    // ========================================================
    // Validar resolución
    // ========================================================

    this->resolution.x =
        std::max(
            this->resolution.x,
            MINIMUM_RESOLUTION
        );


    this->resolution.y =
        std::max(
            this->resolution.y,
            MINIMUM_RESOLUTION
        );


    this->resolution.z =
        std::max(
            this->resolution.z,
            MINIMUM_RESOLUTION
        );


    // ========================================================
    // Calcular spacing
    // ========================================================

    spacing =
        glm::vec3(

            size.x /
            static_cast<float>(
                this->resolution.x - 1
            ),

            size.y /
            static_cast<float>(
                this->resolution.y - 1
            ),

            size.z /
            static_cast<float>(
                this->resolution.z - 1
            )
        );


    // ========================================================
    // Reservar memoria
    // ========================================================

    const std::size_t totalSize =
        static_cast<std::size_t>(
            this->resolution.x
        )
        *
        static_cast<std::size_t>(
            this->resolution.y
        )
        *
        static_cast<std::size_t>(
            this->resolution.z
        );


    values.resize(
        totalSize,
        0.0f
    );
}


// ============================================================
// GET ORIGIN
// ============================================================

const glm::vec3&
ElectronDensity::getOrigin() const
{
    return origin;
}


// ============================================================
// GET SIZE
// ============================================================

const glm::vec3&
ElectronDensity::getSize() const
{
    return size;
}


// ============================================================
// GET RESOLUTION
// ============================================================

const glm::ivec3&
ElectronDensity::getResolution() const
{
    return resolution;
}


// ============================================================
// GET SPACING
// ============================================================

const glm::vec3&
ElectronDensity::getSpacing() const
{
    return spacing;
}


// ============================================================
// CHECK COORDINATES
// ============================================================

bool ElectronDensity::isInside(
    int x,
    int y,
    int z
) const
{
    return
        x >= 0 &&
        x < resolution.x &&

        y >= 0 &&
        y < resolution.y &&

        z >= 0 &&
        z < resolution.z;
}


// ============================================================
// INDEX
// ============================================================

int ElectronDensity::getIndex(
    int x,
    int y,
    int z
) const
{
    return
        x
        +
        resolution.x *
        (
            y
            +
            resolution.y * z
        );
}


// ============================================================
// GET DENSITY
// ============================================================

float ElectronDensity::getDensity(
    int x,
    int y,
    int z
) const
{
    if (!isInside(x, y, z))
    {
        return 0.0f;
    }


    return values[
        getIndex(
            x,
            y,
            z
        )
    ];
}


// ============================================================
// SET DENSITY
// ============================================================

void ElectronDensity::setDensity(
    int x,
    int y,
    int z,
    float value
)
{
    if (!isInside(x, y, z))
    {
        return;
    }


    const int index =
        getIndex(
            x,
            y,
            z
        );


    values[index] = value;


    updateMaximum(
        value
    );
}


// ============================================================
// GRID POSITION
// ============================================================

glm::vec3
ElectronDensity::getPosition(
    int x,
    int y,
    int z
) const
{
    return
        origin
        +
        glm::vec3(

            static_cast<float>(x)
            *
            spacing.x,

            static_cast<float>(y)
            *
            spacing.y,

            static_cast<float>(z)
            *
            spacing.z
        );
}


// ============================================================
// GENERATE
// ============================================================
//
//     ρ(r) = |ψ(r)|²
//
// ============================================================

void ElectronDensity::generate(
    const AtomicOrbital& orbital
)
{
    clear();


    for (int z = 0; z < resolution.z; ++z)
    {
        for (int y = 0; y < resolution.y; ++y)
        {
            for (int x = 0; x < resolution.x; ++x)
            {
                const glm::vec3 position =
                    getPosition(
                        x,
                        y,
                        z
                    );


                const float density =
                    orbital.evaluateDensity(
                        position
                    );


                setDensity(
                    x,
                    y,
                    z,
                    density
                );
            }
        }
    }
}


// ============================================================
// ADD ORBITAL
// ============================================================

void ElectronDensity::addOrbital(
    const AtomicOrbital& orbital,
    float occupation
)
{
    if (occupation <= 0.0f)
    {
        return;
    }


    for (int z = 0; z < resolution.z; ++z)
    {
        for (int y = 0; y < resolution.y; ++y)
        {
            for (int x = 0; x < resolution.x; ++x)
            {
                const glm::vec3 position =
                    getPosition(
                        x,
                        y,
                        z
                    );


                const float density =
                    orbital.evaluateDensity(
                        position
                    );


                const int index =
                    getIndex(
                        x,
                        y,
                        z
                    );


                values[index] +=
                    occupation *
                    density;


                updateMaximum(
                    values[index]
                );
            }
        }
    }
}


// ============================================================
// CLEAR
// ============================================================

void ElectronDensity::clear()
{
    std::fill(
        values.begin(),
        values.end(),
        0.0f
    );


    maximumDensity = 0.0f;
}


// ============================================================
// MAXIMUM DENSITY
// ============================================================

float
ElectronDensity::getMaximumDensity() const
{
    return maximumDensity;
}


// ============================================================
// UPDATE MAXIMUM
// ============================================================

void ElectronDensity::updateMaximum(
    float value
)
{
    if (value > maximumDensity)
    {
        maximumDensity = value;
    }
}


// ============================================================
// TRILINEAR SAMPLE
// ============================================================

float ElectronDensity::sample(
    const glm::vec3& position
) const
{
    const glm::vec3 relative =
        position - origin;


    // ========================================================
    // Coordenada continua
    // ========================================================

    const glm::vec3 gridPosition(

        spacing.x > EPSILON
            ? relative.x / spacing.x
            : 0.0f,

        spacing.y > EPSILON
            ? relative.y / spacing.y
            : 0.0f,

        spacing.z > EPSILON
            ? relative.z / spacing.z
            : 0.0f
    );


    // ========================================================
    // Fuera de la región
    // ========================================================

    if (
        gridPosition.x < 0.0f ||
        gridPosition.y < 0.0f ||
        gridPosition.z < 0.0f ||

        gridPosition.x >
            static_cast<float>(
                resolution.x - 1
            ) ||

        gridPosition.y >
            static_cast<float>(
                resolution.y - 1
            ) ||

        gridPosition.z >
            static_cast<float>(
                resolution.z - 1
            )
    )
    {
        return 0.0f;
    }


    // ========================================================
    // Índices inferiores
    // ========================================================

    const int x0 =
        static_cast<int>(
            std::floor(
                gridPosition.x
            )
        );


    const int y0 =
        static_cast<int>(
            std::floor(
                gridPosition.y
            )
        );


    const int z0 =
        static_cast<int>(
            std::floor(
                gridPosition.z
            )
        );


    // ========================================================
    // Índices superiores
    // ========================================================

    const int x1 =
        std::min(
            x0 + 1,
            resolution.x - 1
        );


    const int y1 =
        std::min(
            y0 + 1,
            resolution.y - 1
        );


    const int z1 =
        std::min(
            z0 + 1,
            resolution.z - 1
        );


    // ========================================================
    // Factores
    // ========================================================

    const float tx =
        gridPosition.x -
        static_cast<float>(x0);


    const float ty =
        gridPosition.y -
        static_cast<float>(y0);


    const float tz =
        gridPosition.z -
        static_cast<float>(z0);


    // ========================================================
    // Ocho muestras
    // ========================================================

    const float c000 =
        getDensity(x0, y0, z0);

    const float c100 =
        getDensity(x1, y0, z0);

    const float c010 =
        getDensity(x0, y1, z0);

    const float c110 =
        getDensity(x1, y1, z0);

    const float c001 =
        getDensity(x0, y0, z1);

    const float c101 =
        getDensity(x1, y0, z1);

    const float c011 =
        getDensity(x0, y1, z1);

    const float c111 =
        getDensity(x1, y1, z1);


    // ========================================================
    // Interpolación X
    // ========================================================

    const float c00 =
        c000 +
        (c100 - c000) * tx;


    const float c10 =
        c010 +
        (c110 - c010) * tx;


    const float c01 =
        c001 +
        (c101 - c001) * tx;


    const float c11 =
        c011 +
        (c111 - c011) * tx;


    // ========================================================
    // Interpolación Y
    // ========================================================

    const float c0 =
        c00 +
        (c10 - c00) * ty;


    const float c1 =
        c01 +
        (c11 - c01) * ty;


    // ========================================================
    // Interpolación Z
    // ========================================================

    return
        c0 +
        (c1 - c0) * tz;
}

}