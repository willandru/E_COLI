#include "OrbitalBuilder.h"

#include <stdexcept>
#include <vector>


namespace Quantum
{
    // ============================================================
    // BUILD
    // ============================================================

    std::vector<Orbital> OrbitalBuilder::build(
        const Chemistry::ElectronicStructure& electronicStructure
    )
    {
        std::vector<Orbital> orbitals;


        const auto& levels =
            electronicStructure.getLevels();


        for (const auto& level : levels)
        {
            for (const auto& sublevel : level.sublevels)
            {
                if (sublevel.electronCount <= 0)
                {
                    continue;
                }


                addSublevelOrbitals(
                    orbitals,
                    level.principalQuantumNumber,
                    sublevel.type,
                    sublevel.electronCount
                );
            }
        }


        return orbitals;
    }


    // ============================================================
    // ANGULAR QUANTUM NUMBER
    // ============================================================

    int OrbitalBuilder::angularQuantumNumber(
        char type
    )
    {
        switch (type)
        {
        case 's':
            return 0;

        case 'p':
            return 1;

        case 'd':
            return 2;

        case 'f':
            return 3;

        default:
            throw std::invalid_argument(
                "Unknown electronic sublevel type."
            );
        }
    }


    // ============================================================
    // ADD SUBLEVEL ORBITALS
    // ============================================================

    void OrbitalBuilder::addSublevelOrbitals(
        std::vector<Orbital>& orbitals,
        int n,
        char type,
        int electronCount
    )
    {
        const int l =
            angularQuantumNumber(type);


        const int orbitalCount =
            2 * l + 1;


        // --------------------------------------------------------
        // VALIDATE ELECTRON COUNT
        // --------------------------------------------------------

        if (
            electronCount < 1 ||
            electronCount > 2 * orbitalCount
        )
        {
            throw std::invalid_argument(
                "Invalid electron count for electronic sublevel."
            );
        }


        // --------------------------------------------------------
        // ELECTRONS PER ORBITAL
        // --------------------------------------------------------

        std::vector<int> electronsPerOrbital(
            orbitalCount,
            0
        );


        int remainingElectrons =
            electronCount;


        // --------------------------------------------------------
        // HUND - FIRST ELECTRON
        // --------------------------------------------------------

        /*
         * First we place one electron in each
         * available orbital.
         *
         * Example:
         *
         *     p²
         *
         * becomes:
         *
         *     m=-1 -> 1
         *     m= 0 -> 1
         *     m=+1 -> 0
         */

        for (
            int i = 0;
            i < orbitalCount &&
            remainingElectrons > 0;
            ++i
        )
        {
            electronsPerOrbital[i] = 1;

            --remainingElectrons;
        }


        // --------------------------------------------------------
        // HUND - SECOND ELECTRON
        // --------------------------------------------------------

        /*
         * Once every orbital has one electron,
         * the remaining electrons form pairs.
         */

        for (
            int i = 0;
            i < orbitalCount &&
            remainingElectrons > 0;
            ++i
        )
        {
            electronsPerOrbital[i] = 2;

            --remainingElectrons;
        }


        // --------------------------------------------------------
        // CREATE ORBITAL OBJECTS
        // --------------------------------------------------------

        for (int i = 0; i < orbitalCount; ++i)
        {
            if (electronsPerOrbital[i] == 0)
            {
                continue;
            }


            /*
             * Magnetic quantum number:
             *
             *     m = -l ... +l
             */

            const int m =
                -l + i;


            // ----------------------------------------------------
            // CREATE USING n, l, m
            // ----------------------------------------------------

            Orbital orbital(
                n,
                l,
                m
            );


            // ----------------------------------------------------
            // SET ELECTRON OCCUPATION
            // ----------------------------------------------------

            orbital.setElectronCount(
                electronsPerOrbital[i]
            );


            // ----------------------------------------------------
            // ADD TO RESULT
            // ----------------------------------------------------

            orbitals.push_back(
                orbital
            );
        }
    }
}