#pragma once

#include <vector>

namespace Chemistry
{

class WaveFunctionS
{
public:

    // ========================================================
    // FUNCION RADIAL DEL ORBITAL ns
    // ========================================================
    //
    // Calcula R_n0(r) para un átomo hidrogenoide.
    //
    // n:
    //     número cuántico principal
    //
    // r:
    //     distancia radial expresada en radios de Bohr (a0)
    //
    // Z:
    //     carga nuclear utilizada en el modelo.
    //
    // Para esta primera implementación:
    //
    //     Z = 1
    //
    // corresponde al modelo hidrogenoide.
    //
    static double radial(
        int n,
        double r,
        double Z = 1.0
    );


    // ========================================================
    // NUMERO DE NODOS RADIALES
    // ========================================================
    //
    // Para orbitales s:
    //
    //     N = n - 1
    //
    static int nodeCount(
        int n
    );


    // ========================================================
    // LOCALIZAR NODOS RADIALES
    // ========================================================
    //
    // Busca numéricamente las posiciones donde:
    //
    //     R_n0(r) = 0
    //
    // Los resultados están expresados en radios de Bohr.
    //
    static std::vector<double> findNodes(
        int n,
        double Z = 1.0,
        int samples = 10000
    );
};

}