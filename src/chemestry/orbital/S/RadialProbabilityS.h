#pragma once

#include <vector>

namespace Chemistry
{

class RadialProbabilityS
{
public:

    // ========================================================
    // PROBABILIDAD RADIAL
    // ========================================================
    //
    // P(r) = 4π r² |R_n0(r)|²
    //
    // En orbitales s:
    //
    // |ψ|² = |R|² / 4π
    //
    // por lo que:
    //
    // P(r) = r² |R|²
    //
    // salvo una constante de normalización.
    //
    static double probability(
        int n,
        double r,
        double Z = 1.0
    );


    // ========================================================
    // RADIOS DE MAXIMA PROBABILIDAD
    // ========================================================
    //
    // Encuentra los máximos locales de P(r).
    //
    static std::vector<double> findMaxima(
        int n,
        double Z = 1.0,
        int samples = 10000
    );
};

}