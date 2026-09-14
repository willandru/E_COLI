#pragma once

#include <vector>

#include "ElectronicStructure.h"
#include "Orbital.h"


namespace Quantum
{
    class OrbitalBuilder
    {
    public:

        static std::vector<Orbital> build(
            const Chemistry::ElectronicStructure& electronicStructure
        );


    private:

        static int angularQuantumNumber(
            char type
        );


        static void addSublevelOrbitals(
            std::vector<Orbital>& orbitals,
            int n,
            char type,
            int electronCount
        );
    };
}