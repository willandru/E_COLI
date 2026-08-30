#pragma once

#include <vector>

namespace Chemistry
{

// ============================================================
// Tipo de orbital atómico
// ============================================================
//
// S -> l = 0
// P -> l = 1
// D -> l = 2
// F -> l = 3
//
// ============================================================

enum class OrbitalType
{
    S,
    P,
    D,
    F
};


// ============================================================
// Estado de un orbital individual
// ============================================================
//
// Ejemplos:
//
//     1s
//     2s
//     2p
//     3p
//
// magneticQuantumNumber:
//
//     S ->  0
//     P -> -1, 0, +1
//     D -> -2, -1, 0, +1, +2
//     F -> -3, -2, -1, 0, +1, +2, +3
//
// electronCount:
//
//     0 -> vacío
//     1 -> un electrón
//     2 -> dos electrones
//
// ============================================================

struct OrbitalState
{
    int principalQuantumNumber;

    OrbitalType type;

    int magneticQuantumNumber;

    int electronCount;
};


// ============================================================
// Configuración electrónica
// ============================================================
//
// Construye la configuración electrónica de un átomo a partir
// de su número atómico.
//
// Ejemplo:
//
//     Carbono
//
//     Z = 6
//
//     1s² 2s² 2p²
//
// ============================================================

class ElectronConfiguration
{
public:

    explicit ElectronConfiguration(
        int atomicNumber
    );


    // ========================================================
    // Información general
    // ========================================================

    int getElectronCount() const;

    int getValenceElectronCount() const;


    // ========================================================
    // Orbitales
    // ========================================================

    const std::vector<OrbitalState>&
    getOrbitals() const;


private:

    // ========================================================
    // Estado
    // ========================================================

    int electronCount;

    int valenceElectronCount;

    std::vector<OrbitalState> orbitals;


    // ========================================================
    // Construcción
    // ========================================================

    void build(
        int atomicNumber
    );


    // ========================================================
    // Electrones de valencia
    // ========================================================

    void calculateValenceElectrons();
};

}