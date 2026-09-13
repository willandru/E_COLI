#include "ElectronicStructure.h"

namespace Chemistry
{

// ============================================================
// CONSTRUCTOR
// ============================================================

ElectronicStructure::ElectronicStructure()
    : electronCount(0)
{
}


// ============================================================
// CONSTRUCTOR
// ============================================================

ElectronicStructure::ElectronicStructure(
    const Nucleus& nucleus
)
    : electronCount(
        nucleus.getProtonCount()
    )
{
    build();
}


// ============================================================
// BUILD ORBITALS
// ============================================================
//
// Número de orbitales:
//
// l = 0 -> s -> 1 orbital
// l = 1 -> p -> 3 orbitales
// l = 2 -> d -> 5 orbitales
// l = 3 -> f -> 7 orbitales
//
// Primero se coloca un electrón en cada orbital
// y posteriormente se empiezan a aparear.
//

void ElectronicStructure::buildOrbitals(
    Sublevel& sublevel
)
{
    const int orbitalCount =
        2 * sublevel.l + 1;

    sublevel.orbitals.clear();

    sublevel.orbitals.reserve(
        orbitalCount
    );

    // ========================================================
    // CREAR ORBITALES
    // ========================================================

    for (int m = -sublevel.l;
         m <= sublevel.l;
         ++m)
    {
        sublevel.orbitals.push_back(
            {
                m,
                0
            }
        );
    }


    // ========================================================
    // DISTRIBUIR ELECTRONES
    // ========================================================

    int remainingElectrons =
        sublevel.electronCount;


    // --------------------------------------------------------
    // PRIMERA PASADA
    //
    // Un electrón por orbital.
    //
    // Regla de Hund.
    // --------------------------------------------------------

    for (Orbital& orbital :
         sublevel.orbitals)
    {
        if (remainingElectrons <= 0)
            break;

        orbital.electronCount = 1;

        --remainingElectrons;
    }


    // --------------------------------------------------------
    // SEGUNDA PASADA
    //
    // Emparejar electrones.
    // --------------------------------------------------------

    for (Orbital& orbital :
         sublevel.orbitals)
    {
        if (remainingElectrons <= 0)
            break;

        if (orbital.electronCount < 2)
        {
            orbital.electronCount = 2;

            --remainingElectrons;
        }
    }
}


// ============================================================
// BUILD ELECTRONIC STRUCTURE
// ============================================================

void ElectronicStructure::build()
{
    levels.clear();

    int remainingElectrons =
        electronCount;


    // ========================================================
    // SUBLEVEL DEFINITION
    // ========================================================

    struct SublevelDefinition
    {
        int n;
        int l;
        int capacity;
    };


    // ========================================================
    // AUFBAU FILLING ORDER
    // ========================================================

    const SublevelDefinition fillingOrder[] =
    {
        {1, 0, 2},    // 1s
        {2, 0, 2},    // 2s
        {2, 1, 6},    // 2p
        {3, 0, 2},    // 3s
        {3, 1, 6},    // 3p
        {4, 0, 2},    // 4s
        {3, 2, 10},   // 3d
        {4, 1, 6},    // 4p
        {5, 0, 2},    // 5s
        {4, 2, 10},   // 4d
        {5, 1, 6},    // 5p
        {6, 0, 2},    // 6s
        {4, 3, 14},   // 4f
        {5, 2, 10},   // 5d
        {6, 1, 6},    // 6p
        {7, 0, 2},    // 7s
        {5, 3, 14},   // 5f
        {6, 2, 10},   // 6d
        {7, 1, 6}     // 7p
    };


    // ========================================================
    // BUILD SUBLEVELS
    // ========================================================

    for (
        const SublevelDefinition& definition :
        fillingOrder
    )
    {
        if (remainingElectrons <= 0)
            break;


        const int electrons =
            remainingElectrons <
            definition.capacity
                ? remainingElectrons
                : definition.capacity;


        // ====================================================
        // BUSCAR NIVEL
        // ====================================================

        bool levelExists = false;


        for (EnergyLevel& level : levels)
        {
            if (level.n == definition.n)
            {
                Sublevel sublevel;

                sublevel.n =
                    definition.n;

                sublevel.l =
                    definition.l;

                sublevel.electronCount =
                    electrons;


                buildOrbitals(
                    sublevel
                );


                level.sublevels.push_back(
                    sublevel
                );


                levelExists = true;

                break;
            }
        }


        // ====================================================
        // CREAR NIVEL
        // ====================================================

        if (!levelExists)
        {
            EnergyLevel level;

            level.n =
                definition.n;


            Sublevel sublevel;

            sublevel.n =
                definition.n;

            sublevel.l =
                definition.l;

            sublevel.electronCount =
                electrons;


            buildOrbitals(
                sublevel
            );


            level.sublevels.push_back(
                sublevel
            );


            levels.push_back(
                level
            );
        }


        remainingElectrons -=
            electrons;
    }
}


// ============================================================
// GET ELECTRON COUNT
// ============================================================

int ElectronicStructure::getElectronCount() const
{
    return electronCount;
}


// ============================================================
// GET LEVELS
// ============================================================

const std::vector<EnergyLevel>&
ElectronicStructure::getLevels() const
{
    return levels;
}


// ============================================================
// GET CONFIGURATION
// ============================================================

std::string
ElectronicStructure::getConfiguration() const
{
    std::string configuration;


    for (const EnergyLevel& level :
         levels)
    {
        for (const Sublevel& sublevel :
             level.sublevels)
        {
            char orbital = '?';


            switch (sublevel.l)
            {
                case 0:
                    orbital = 's';
                    break;

                case 1:
                    orbital = 'p';
                    break;

                case 2:
                    orbital = 'd';
                    break;

                case 3:
                    orbital = 'f';
                    break;
            }


            configuration +=
                std::to_string(
                    sublevel.n
                ) +
                orbital +
                std::to_string(
                    sublevel.electronCount
                ) +
                " ";
        }
    }


    if (!configuration.empty())
        configuration.pop_back();


    return configuration;
}

}