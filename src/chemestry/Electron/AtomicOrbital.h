#pragma once

#include <glm/glm.hpp>

#include "ElectronConfiguration.h"

namespace Chemistry
{

// ============================================================
// Orientación espacial del orbital
// ============================================================
//
// Para orbitales s:
//
//     None
//
// Para orbitales p:
//
//     X -> px
//     Y -> py
//     Z -> pz
//
// ============================================================

enum class OrbitalOrientation
{
    None,

    X,
    Y,
    Z
};


// ============================================================
// AtomicOrbital
// ============================================================
//
// Representa espacialmente un orbital atómico:
//
//     ψ(x,y,z)
//
// A partir de ψ podemos obtener:
//
//     ρ(x,y,z) = |ψ(x,y,z)|²
//
// No representa electrones individuales.
//
// No renderiza.
//
// ============================================================

class AtomicOrbital
{
public:

    AtomicOrbital(
        int principalQuantumNumber,
        OrbitalType type,
        OrbitalOrientation orientation,
        const glm::vec3& center
    );


    // ========================================================
    // Información cuántica
    // ========================================================

    int getPrincipalQuantumNumber() const;

    OrbitalType getType() const;

    OrbitalOrientation getOrientation() const;


    // ========================================================
    // Centro
    // ========================================================

    const glm::vec3& getCenter() const;


    // ========================================================
    // Función de onda
    // ========================================================

    float evaluate(
        const glm::vec3& position
    ) const;


    // ========================================================
    // Densidad de probabilidad
    // ========================================================

    float evaluateDensity(
        const glm::vec3& position
    ) const;


private:

    int principalQuantumNumber;

    OrbitalType type;

    OrbitalOrientation orientation;

    glm::vec3 center;


    // ========================================================
    // Evaluadores
    // ========================================================

    float evaluateS(
        const glm::vec3& relativePosition
    ) const;


    float evaluateP(
        const glm::vec3& relativePosition
    ) const;
};

}