#include "ElectronConfiguration.h"

namespace Chemistry
{

namespace
{

// ============================================================
// Capacidad electrónica
// ============================================================

int getOrbitalCapacity(
    OrbitalType type
)
{
    switch (type)
    {
        case OrbitalType::S:
            return 2;

        case OrbitalType::P:
            return 6;

        case OrbitalType::D:
            return 10;

        case OrbitalType::F:
            return 14;
    }

    return 0;
}


// ============================================================
// Número de orbitales dentro del subnivel
// ============================================================

int getOrbitalCount(
    OrbitalType type
)
{
    switch (type)
    {
        case OrbitalType::S:
            return 1;

        case OrbitalType::P:
            return 3;

        case OrbitalType::D:
            return 5;

        case OrbitalType::F:
            return 7;
    }

    return 0;
}

}


// ============================================================
// Constructor
// ============================================================

ElectronConfiguration::ElectronConfiguration(
    int atomicNumber
)
    : electronCount(atomicNumber),
      valenceElectronCount(0)
{
    build(atomicNumber);

    calculateValenceElectrons();
}


// ============================================================
// Build
// ============================================================

void ElectronConfiguration::build(
    int atomicNumber
)
{
    orbitals.clear();


    if (atomicNumber <= 0)
    {
        electronCount = 0;

        return;
    }


    // ========================================================
    // Orden de llenado
    // ========================================================
    //
    // Hasta 4p.
    //
    // Esto cubre los elementos que necesitamos inicialmente
    // para la química orgánica de interés.
    //
    // ========================================================

    struct OrbitalLevel
    {
        int n;

        OrbitalType type;
    };


    constexpr OrbitalLevel fillingOrder[] =
    {
        {1, OrbitalType::S},

        {2, OrbitalType::S},
        {2, OrbitalType::P},

        {3, OrbitalType::S},
        {3, OrbitalType::P},

        {4, OrbitalType::S},
        {3, OrbitalType::D},
        {4, OrbitalType::P}
    };


    int remainingElectrons =
        atomicNumber;


    // ========================================================
    // Llenado
    // ========================================================

    for (
        const OrbitalLevel& level :
        fillingOrder
    )
    {
        if (remainingElectrons <= 0)
        {
            break;
        }


        const int orbitalCount =
            getOrbitalCount(
                level.type
            );


        const int capacity =
            getOrbitalCapacity(
                level.type
            );


        const int electronsToFill =
            remainingElectrons < capacity
                ? remainingElectrons
                : capacity;


        // ====================================================
        // Números cuánticos magnéticos
        // ====================================================

        const int firstMagneticNumber =
            -(orbitalCount - 1) / 2;


        int electronsRemaining =
            electronsToFill;


        // ====================================================
        // Primera ocupación
        // ====================================================
        //
        // Un electrón por orbital antes de comenzar a
        // emparejar.
        //
        // Hund.
        //
        // ====================================================

        for (
            int i = 0;
            i < orbitalCount &&
            electronsRemaining > 0;
            ++i
        )
        {
            OrbitalState state
            {
                level.n,

                level.type,

                firstMagneticNumber + i,

                1
            };


            orbitals.push_back(
                state
            );


            --electronsRemaining;
        }


        // ====================================================
        // Segunda ocupación
        // ====================================================
        //
        // Emparejar orbitales.
        //
        // Pauli.
        //
        // ====================================================

        for (
            OrbitalState& orbital :
            orbitals
        )
        {
            if (
                orbital.principalQuantumNumber ==
                    level.n

                &&

                orbital.type ==
                    level.type

                &&

                orbital.electronCount == 1

                &&

                electronsRemaining > 0
            )
            {
                orbital.electronCount = 2;

                --electronsRemaining;
            }


            if (electronsRemaining <= 0)
            {
                break;
            }
        }


        remainingElectrons -=
            electronsToFill;
    }
}


// ============================================================
// Calculate valence electrons
// ============================================================

void ElectronConfiguration::calculateValenceElectrons()
{
    valenceElectronCount = 0;


    if (orbitals.empty())
    {
        return;
    }


    // ========================================================
    // Nivel principal más externo
    // ========================================================

    int highestPrincipalQuantumNumber = 0;


    for (
        const OrbitalState& orbital :
        orbitals
    )
    {
        if (
            orbital.principalQuantumNumber >
            highestPrincipalQuantumNumber
        )
        {
            highestPrincipalQuantumNumber =
                orbital.principalQuantumNumber;
        }
    }


    // ========================================================
    // Contar electrones de valencia
    // ========================================================

    for (
        const OrbitalState& orbital :
        orbitals
    )
    {
        if (
            orbital.principalQuantumNumber ==
            highestPrincipalQuantumNumber
        )
        {
            valenceElectronCount +=
                orbital.electronCount;
        }
    }
}


// ============================================================
// Electron count
// ============================================================

int ElectronConfiguration::getElectronCount() const
{
    return electronCount;
}


// ============================================================
// Valence electrons
// ============================================================

int ElectronConfiguration::getValenceElectronCount() const
{
    return valenceElectronCount;
}


// ============================================================
// Orbitals
// ============================================================

const std::vector<OrbitalState>&
ElectronConfiguration::getOrbitals() const
{
    return orbitals;
}

}