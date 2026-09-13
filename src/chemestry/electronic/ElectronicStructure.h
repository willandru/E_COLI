#pragma once

#include "Nucleus.h"

#include <string>
#include <vector>

namespace Chemistry
{

// ============================================================
// ORBITAL
// ============================================================
//
// Un subnivel contiene varios orbitales:
//
// s -> 1 orbital
// p -> 3 orbitales
// d -> 5 orbitales
// f -> 7 orbitales
//
// m = número cuántico magnético.
// Para una representación espacial:
//   p: -1, 0, +1
//   d: -2 ... +2
//   f: -3 ... +3
//
// electronCount indica cuántos electrones ocupan
// ese orbital.
//

struct Orbital
{
    int m;
    int electronCount;
};


// ============================================================
// SUBLEVEL
// ============================================================

struct Sublevel
{
    int n;
    int l;
    int electronCount;

    std::vector<Orbital> orbitals;
};


// ============================================================
// ENERGY LEVEL
// ============================================================

struct EnergyLevel
{
    int n;

    std::vector<Sublevel> sublevels;
};


// ============================================================
// ELECTRONIC STRUCTURE
// ============================================================

class ElectronicStructure
{
private:

    int electronCount;

    std::vector<EnergyLevel> levels;

    void build();

    void buildOrbitals(
        Sublevel& sublevel
    );

public:

    ElectronicStructure();

    explicit ElectronicStructure(
        const Nucleus& nucleus
    );

    int getElectronCount() const;

    const std::vector<EnergyLevel>&
    getLevels() const;

    std::string getConfiguration() const;
};

}