#include "ElectronicStructure.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>


namespace Chemistry
{

    // ============================================================
    // ORDEN DE LLENADO
    // ============================================================
    //
    // Orden de Aufbau utilizado para construir la configuración
    // electrónica.
    //
    // ============================================================

    const std::vector<ElectronicStructure::FillingOrder>
        ElectronicStructure::fillingOrder =
    {
        {1, 0},   // 1s
        {2, 0},   // 2s
        {2, 1},   // 2p
        {3, 0},   // 3s
        {3, 1},   // 3p
        {4, 0},   // 4s
        {3, 2},   // 3d
        {4, 1},   // 4p
        {5, 0},   // 5s
        {4, 2},   // 4d
        {5, 1},   // 5p
        {6, 0},   // 6s
        {4, 3},   // 4f
        {5, 2},   // 5d
        {6, 1},   // 6p
        {7, 0},   // 7s
        {5, 3},   // 5f
        {6, 2},   // 6d
        {7, 1}    // 7p
    };


    // ============================================================
    // CONSTRUCTOR VACÍO
    // ============================================================

    ElectronicStructure::ElectronicStructure()
        = default;


    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    ElectronicStructure::ElectronicStructure(
        int atomicNumber,
        int ionCharge
    )
    {
        setAtom(
            atomicNumber,
            ionCharge
        );
    }


    // ============================================================
    // SET ATOM
    // ============================================================

    void ElectronicStructure::setAtom(
        int atomicNumber,
        int ionCharge
    )
    {
        if (!isValidAtomicNumber(atomicNumber))
        {
            throw std::invalid_argument(
                "Atomic number must be between 1 and 118."
            );
        }

        if (!isValidIonCharge(
                atomicNumber,
                ionCharge))
        {
            throw std::invalid_argument(
                "Invalid ion charge."
            );
        }

        this->atomicNumber = atomicNumber;

        this->ionCharge = ionCharge;

        this->electronCount =
            atomicNumber - ionCharge;

        calculate();
    }


    // ============================================================
    // CALCULATE
    // ============================================================

    void ElectronicStructure::calculate()
    {
        clear();

        if (electronCount <= 0)
        {
            return;
        }

        buildStructure();

        buildLevels();
    }


    // ============================================================
    // CLEAR
    // ============================================================

    void ElectronicStructure::clear()
    {
        levels.clear();

        sublevels.clear();
    }


    // ============================================================
    // BUILD STRUCTURE
    // ============================================================

    void ElectronicStructure::buildStructure()
    {
        int remainingElectrons = electronCount;


        for (const FillingOrder& order : fillingOrder)
        {
            if (remainingElectrons <= 0)
            {
                break;
            }


            ElectronicSublevel sublevel =
                createSublevel(
                    order.n,
                    order.l
                );


            const int capacity =
                sublevelCapacity(order.l);


            const int electronsToPlace =
                std::min(
                    remainingElectrons,
                    capacity
                );


            distributeElectrons(
                sublevel,
                electronsToPlace
            );


            sublevels.push_back(
                sublevel
            );


            remainingElectrons -=
                electronsToPlace;
        }
    }


    // ============================================================
    // CREATE SUBLEVEL
    // ============================================================

    ElectronicSublevel
    ElectronicStructure::createSublevel(
        int n,
        int l
    ) const
    {
        ElectronicSublevel sublevel;

        sublevel.principalQuantumNumber = n;

        sublevel.angularQuantumNumber = l;

        sublevel.type = sublevelType(l);

        sublevel.electronCount = 0;


        const int numberOfOrbitals =
            orbitalCount(l);


        sublevel.orbitals.reserve(
            numberOfOrbitals
        );


        // --------------------------------------------------------
        // Los valores permitidos de m son:
        //
        //     m = -l, ..., 0, ..., +l
        //
        // --------------------------------------------------------

        for (int m = -l; m <= l; ++m)
        {
            ElectronicOrbital orbital;

            orbital.magneticQuantumNumber = m;

            orbital.electronCount = 0;

            sublevel.orbitals.push_back(
                orbital
            );
        }


        return sublevel;
    }


    // ============================================================
    // DISTRIBUTE ELECTRONS
    // ============================================================
    //
    // Aplicamos la regla de Hund:
    //
    // Primera etapa:
    //
    //     ↑   ↑   ↑
    //
    // Segunda etapa:
    //
    //     ↑↓  ↑↓  ↑
    //
    // Esto permite representar correctamente la ocupación de los
    // orbitales dentro de un subnivel.
    //
    // ============================================================

    void ElectronicStructure::distributeElectrons(
        ElectronicSublevel& sublevel,
        int electronCount
    )
    {
        const int capacity =
            sublevelCapacity(
                sublevel.angularQuantumNumber
            );


        if (electronCount < 0 ||
            electronCount > capacity)
        {
            throw std::invalid_argument(
                "Invalid electron count for sublevel."
            );
        }


        int remaining =
            electronCount;


        const int numberOfOrbitals =
            static_cast<int>(
                sublevel.orbitals.size()
            );


        // --------------------------------------------------------
        // Primera ocupación:
        //
        // un electrón por orbital.
        // --------------------------------------------------------

        for (int i = 0;
             i < numberOfOrbitals &&
             remaining > 0;
             ++i)
        {
            sublevel.orbitals[i].electronCount = 1;

            --remaining;
        }


        // --------------------------------------------------------
        // Segunda ocupación:
        //
        // comienza el apareamiento.
        // --------------------------------------------------------

        for (int i = 0;
             i < numberOfOrbitals &&
             remaining > 0;
             ++i)
        {
            sublevel.orbitals[i].electronCount = 2;

            --remaining;
        }


        sublevel.electronCount =
            electronCount;
    }


    // ============================================================
    // BUILD LEVELS
    // ============================================================

    void ElectronicStructure::buildLevels()
    {
        levels.clear();


        for (const ElectronicSublevel& sublevel : sublevels)
        {
            auto levelIterator =
                std::find_if(
                    levels.begin(),
                    levels.end(),
                    [&](const ElectronicLevel& level)
                    {
                        return
                            level.principalQuantumNumber ==
                            sublevel.principalQuantumNumber;
                    }
                );


            if (levelIterator == levels.end())
            {
                ElectronicLevel level;

                level.principalQuantumNumber =
                    sublevel.principalQuantumNumber;

                level.sublevels.push_back(
                    sublevel
                );

                levels.push_back(
                    level
                );
            }
            else
            {
                levelIterator->sublevels.push_back(
                    sublevel
                );
            }
        }
    }


    // ============================================================
    // GET ATOMIC NUMBER
    // ============================================================

    int ElectronicStructure::getAtomicNumber() const
    {
        return atomicNumber;
    }


    // ============================================================
    // GET ION CHARGE
    // ============================================================

    int ElectronicStructure::getIonCharge() const
    {
        return ionCharge;
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

    const std::vector<ElectronicLevel>&
    ElectronicStructure::getLevels() const
    {
        return levels;
    }


    // ============================================================
    // GET SUBLEVELS
    // ============================================================

    const std::vector<ElectronicSublevel>&
    ElectronicStructure::getSublevels() const
    {
        return sublevels;
    }


    // ============================================================
    // GET CONFIGURATION
    // ============================================================

    std::string
    ElectronicStructure::getConfiguration() const
    {
        std::ostringstream stream;

        bool first = true;


        for (const ElectronicSublevel& sublevel : sublevels)
        {
            if (sublevel.electronCount == 0)
            {
                continue;
            }


            if (!first)
            {
                stream << ' ';
            }


            stream
                << sublevel.principalQuantumNumber
                << sublevel.type
                << sublevel.electronCount;


            first = false;
        }


        return stream.str();
    }


    // ============================================================
    // ORBITAL COUNT
    // ============================================================
    //
    // Número de orbitales de un subnivel:
    //
    //     2l + 1
    //
    // s -> 1
    // p -> 3
    // d -> 5
    // f -> 7
    //
    // ============================================================

    int ElectronicStructure::orbitalCount(
        int angularQuantumNumber
    )
    {
        if (angularQuantumNumber < 0)
        {
            throw std::invalid_argument(
                "Angular quantum number cannot be negative."
            );
        }


        return 2 * angularQuantumNumber + 1;
    }


    // ============================================================
    // SUBLEVEL CAPACITY
    // ============================================================
    //
    // Cada orbital puede contener dos electrones.
    //
    // Por tanto:
    //
    //     capacity = 2(2l + 1)
    //
    // s -> 2
    // p -> 6
    // d -> 10
    // f -> 14
    //
    // ============================================================

    int ElectronicStructure::sublevelCapacity(
        int angularQuantumNumber
    )
    {
        return 2 * orbitalCount(
            angularQuantumNumber
        );
    }


    // ============================================================
    // SUBLEVEL TYPE
    // ============================================================

    char ElectronicStructure::sublevelType(
        int angularQuantumNumber
    )
    {
        switch (angularQuantumNumber)
        {
            case 0:
                return 's';

            case 1:
                return 'p';

            case 2:
                return 'd';

            case 3:
                return 'f';

            default:
                throw std::invalid_argument(
                    "Only s, p, d and f sublevels are supported."
                );
        }
    }


    // ============================================================
    // VALID ATOMIC NUMBER
    // ============================================================

    bool ElectronicStructure::isValidAtomicNumber(
        int atomicNumber
    )
    {
        return
            atomicNumber >= 1 &&
            atomicNumber <= 118;
    }


    // ============================================================
    // VALID ION CHARGE
    // ============================================================

    bool ElectronicStructure::isValidIonCharge(
        int atomicNumber,
        int ionCharge
    )
    {
        if (!isValidAtomicNumber(atomicNumber))
        {
            return false;
        }


        const int electrons =
            atomicNumber - ionCharge;


        return electrons >= 0;
    }

}