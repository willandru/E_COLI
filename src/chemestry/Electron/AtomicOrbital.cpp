#include "AtomicOrbital.h"

#include <cmath>

namespace Chemistry
{

namespace
{

constexpr float EPSILON = 1.0e-6f;


// ============================================================
// Función radial exponencial
// ============================================================

float exponentialRadial(
    float radius,
    float scale
)
{
    return std::exp(
        -radius * scale
    );
}

}


// ============================================================
// Constructor
// ============================================================

AtomicOrbital::AtomicOrbital(
    int principalQuantumNumber,
    OrbitalType type,
    OrbitalOrientation orientation,
    const glm::vec3& center
)
    : principalQuantumNumber(principalQuantumNumber),
      type(type),
      orientation(orientation),
      center(center)
{
}


// ============================================================
// Principal quantum number
// ============================================================

int AtomicOrbital::getPrincipalQuantumNumber() const
{
    return principalQuantumNumber;
}


// ============================================================
// Orbital type
// ============================================================

OrbitalType AtomicOrbital::getType() const
{
    return type;
}


// ============================================================
// Orientation
// ============================================================

OrbitalOrientation
AtomicOrbital::getOrientation() const
{
    return orientation;
}


// ============================================================
// Center
// ============================================================

const glm::vec3&
AtomicOrbital::getCenter() const
{
    return center;
}


// ============================================================
// Evaluate
// ============================================================

float AtomicOrbital::evaluate(
    const glm::vec3& position
) const
{
    const glm::vec3 relativePosition =
        position - center;


    switch (type)
    {
        case OrbitalType::S:

            return evaluateS(
                relativePosition
            );


        case OrbitalType::P:

            return evaluateP(
                relativePosition
            );


        case OrbitalType::D:
        case OrbitalType::F:

            return 0.0f;
    }


    return 0.0f;
}


// ============================================================
// Evaluate density
// ============================================================

float AtomicOrbital::evaluateDensity(
    const glm::vec3& position
) const
{
    const float psi =
        evaluate(
            position
        );


    return psi * psi;
}


// ============================================================
// S orbital
// ============================================================

float AtomicOrbital::evaluateS(
    const glm::vec3& relativePosition
) const
{
    const float radius =
        glm::length(
            relativePosition
        );


    if (principalQuantumNumber <= 0)
    {
        return 0.0f;
    }


    const float scale =
        1.0f /
        static_cast<float>(
            principalQuantumNumber
        );


    const float radial =
        exponentialRadial(
            radius,
            scale
        );


    return radial;
}


// ============================================================
// P orbital
// ============================================================

float AtomicOrbital::evaluateP(
    const glm::vec3& relativePosition
) const
{
    const float radius =
        glm::length(
            relativePosition
        );


    if (
        radius < EPSILON ||
        principalQuantumNumber <= 0
    )
    {
        return 0.0f;
    }


    // ========================================================
    // Parte radial
    // ========================================================

    const float scale =
        1.0f /
        static_cast<float>(
            principalQuantumNumber
        );


    const float radial =
        radius *
        exponentialRadial(
            radius,
            scale
        );


    // ========================================================
    // Parte angular
    // ========================================================

    float angular = 0.0f;


    switch (orientation)
    {
        case OrbitalOrientation::X:

            angular =
                relativePosition.x /
                radius;

            break;


        case OrbitalOrientation::Y:

            angular =
                relativePosition.y /
                radius;

            break;


        case OrbitalOrientation::Z:

            angular =
                relativePosition.z /
                radius;

            break;


        case OrbitalOrientation::None:

            angular = 0.0f;

            break;
    }


    return radial * angular;
}

}