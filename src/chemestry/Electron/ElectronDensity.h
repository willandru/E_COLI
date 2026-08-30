#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "AtomicOrbital.h"

namespace Chemistry
{

// ============================================================
// ElectronDensity
// ============================================================
//
// Campo tridimensional de densidad electrónica:
//
//     ρ(x,y,z)
//
// ============================================================

class ElectronDensity
{
public:

    // ========================================================
    // Constructor
    // ========================================================

    ElectronDensity(
        const glm::vec3& origin,
        const glm::vec3& size,
        const glm::ivec3& resolution
    );


    // ========================================================
    // Información espacial
    // ========================================================

    const glm::vec3& getOrigin() const;

    const glm::vec3& getSize() const;

    const glm::ivec3& getResolution() const;

    const glm::vec3& getSpacing() const;


    // ========================================================
    // Posición espacial
    // ========================================================

    glm::vec3 getPosition(
        int x,
        int y,
        int z
    ) const;


    // ========================================================
    // Acceso a densidad
    // ========================================================

    float getDensity(
        int x,
        int y,
        int z
    ) const;


    void setDensity(
        int x,
        int y,
        int z,
        float value
    );


    // ========================================================
    // Muestreo espacial
    // ========================================================

    float sample(
        const glm::vec3& position
    ) const;


    // ========================================================
    // Generar densidad
    // ========================================================

    void generate(
        const AtomicOrbital& orbital
    );


    // ========================================================
    // Añadir orbital
    // ========================================================

    void addOrbital(
        const AtomicOrbital& orbital,
        float occupation = 1.0f
    );


    // ========================================================
    // Limpiar
    // ========================================================

    void clear();


    // ========================================================
    // Estadísticas
    // ========================================================

    float getMaximumDensity() const;


private:

    // ========================================================
    // Geometría
    // ========================================================

    glm::vec3 origin;

    glm::vec3 size;

    glm::ivec3 resolution;

    glm::vec3 spacing;


    // ========================================================
    // Valores
    // ========================================================

    std::vector<float> values;


    // ========================================================
    // Máximo
    // ========================================================

    float maximumDensity;


    // ========================================================
    // Utilidades internas
    // ========================================================

    bool isInside(
        int x,
        int y,
        int z
    ) const;


    int getIndex(
        int x,
        int y,
        int z
    ) const;


    void updateMaximum(
        float value
    );
};

}