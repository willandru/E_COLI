#pragma once

#include <vector>

namespace Chemistry
{

class DensityS
{
public:

    // ========================================================
    // DENSIDAD ORBITAL
    // ========================================================
    //
    // |ψ(r)|²
    //
    // Para orbitales s:
    //
    // ψ(r) = R_n0(r) / sqrt(4π)
    //
    static double density(
        int n,
        double r,
        double Z = 1.0
    );


    // ========================================================
    // RADIOS DE MAXIMA DENSIDAD
    // ========================================================
    //
    // Devuelve los radios donde la densidad espacial
    // alcanza máximos locales.
    //
    static std::vector<double> findMaxima(
        int n,
        double Z = 1.0,
        int samples = 10000
    );
};

}