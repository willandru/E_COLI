#include "Orbital.h"

#include <stdexcept>


namespace Quantum
{

    // ============================================================
    // CONSTRUCTOR POR DEFECTO
    // ============================================================

    Orbital::Orbital()
        = default;


    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    Orbital::Orbital(
        int principalQuantumNumber,
        int angularQuantumNumber,
        int magneticQuantumNumber
    )
    {
        setQuantumNumbers(
            principalQuantumNumber,
            angularQuantumNumber,
            magneticQuantumNumber
        );
    }


    // ============================================================
    // SET QUANTUM NUMBERS
    // ============================================================
    //
    // Las reglas son:
    //
    //     n >= 1
    //
    //     0 <= l <= n - 1
    //
    //     -l <= m <= +l
    //
    // Ejemplos válidos:
    //
    //     1s:
    //         n = 1
    //         l = 0
    //         m = 0
    //
    //     2p:
    //         n = 2
    //         l = 1
    //         m = -1, 0, +1
    //
    //     3d:
    //         n = 3
    //         l = 2
    //         m = -2,-1,0,+1,+2
    //
    // ============================================================

    void Orbital::setQuantumNumbers(
        int principalQuantumNumber,
        int angularQuantumNumber,
        int magneticQuantumNumber
    )
    {
        if (!isValidQuantumNumbers(
                principalQuantumNumber,
                angularQuantumNumber,
                magneticQuantumNumber))
        {
            throw std::invalid_argument(
                "Invalid quantum numbers for orbital."
            );
        }


        this->principalQuantumNumber =
            principalQuantumNumber;

        this->angularQuantumNumber =
            angularQuantumNumber;

        this->magneticQuantumNumber =
            magneticQuantumNumber;
    }


    // ============================================================
    // GET PRINCIPAL QUANTUM NUMBER
    // ============================================================

    int Orbital::getPrincipalQuantumNumber() const
    {
        return principalQuantumNumber;
    }


    // ============================================================
    // GET ANGULAR QUANTUM NUMBER
    // ============================================================

    int Orbital::getAngularQuantumNumber() const
    {
        return angularQuantumNumber;
    }


    // ============================================================
    // GET MAGNETIC QUANTUM NUMBER
    // ============================================================

    int Orbital::getMagneticQuantumNumber() const
    {
        return magneticQuantumNumber;
    }


    // ============================================================
    // RADIAL NODE COUNT
    // ============================================================
    //
    // Para un orbital hidrogenoide:
    //
    //     nodos radiales = n - l - 1
    //
    // Ejemplos:
    //
    //     1s -> 1 - 0 - 1 = 0
    //     2s -> 2 - 0 - 1 = 1
    //     2p -> 2 - 1 - 1 = 0
    //     3s -> 3 - 0 - 1 = 2
    //     3p -> 3 - 1 - 1 = 1
    //     3d -> 3 - 2 - 1 = 0
    //
    // ============================================================

    int Orbital::getRadialNodeCount() const
    {
        return principalQuantumNumber
             - angularQuantumNumber
             - 1;
    }


    // ============================================================
    // ANGULAR NODE COUNT
    // ============================================================
    //
    // El número de nodos angulares es:
    //
    //     l
    //
    // ============================================================

    int Orbital::getAngularNodeCount() const
    {
        return angularQuantumNumber;
    }


    // ============================================================
    // TOTAL NODE COUNT
    // ============================================================
    //
    // Para orbitales hidrogenoides:
    //
    //     radial + angular
    //
    //     (n-l-1) + l = n-1
    //
    // ============================================================

    int Orbital::getTotalNodeCount() const
    {
        return getRadialNodeCount()
             + getAngularNodeCount();
    }


    // ============================================================
    // ENERGY
    // ============================================================

    double Orbital::getEnergy() const
    {
        return energy;
    }


    // ============================================================
    // SET ENERGY
    // ============================================================

    void Orbital::setEnergy(double energy)
    {
        this->energy = energy;
    }


    // ============================================================
    // ELECTRON COUNT
    // ============================================================

    int Orbital::getElectronCount() const
    {
        return electronCount;
    }


    // ============================================================
    // SET ELECTRON COUNT
    // ============================================================
    //
    // Un orbital puede contener:
    //
    //     0 electrones
    //     1 electrón
    //     2 electrones
    //
    // ============================================================

    void Orbital::setElectronCount(int electronCount)
    {
        if (electronCount < 0 ||
            electronCount > 2)
        {
            throw std::invalid_argument(
                "An orbital can contain between 0 and 2 electrons."
            );
        }


        this->electronCount =
            electronCount;
    }


    // ============================================================
    // GET TYPE
    // ============================================================

    char Orbital::getType() const
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
                return '?';
        }
    }


    // ============================================================
    // ORBITAL COUNT
    // ============================================================
    //
    // Para un subnivel con número cuántico l:
    //
    //     número de orbitales = 2l + 1
    //
    // ============================================================

    int Orbital::getOrbitalCount() const
    {
        return 2 * angularQuantumNumber + 1;
    }


    // ============================================================
    // MAXIMUM ELECTRON COUNT
    // ============================================================

    int Orbital::getMaximumElectronCount() const
    {
        return 2;
    }


    // ============================================================
    // VALIDATE QUANTUM NUMBERS
    // ============================================================

    bool Orbital::isValidQuantumNumbers(
        int n,
        int l,
        int m
    )
    {
        // --------------------------------------------------------
        // n debe comenzar en 1.
        // --------------------------------------------------------

        if (n < 1)
        {
            return false;
        }


        // --------------------------------------------------------
        // l debe cumplir:
        //
        //     0 <= l <= n - 1
        // --------------------------------------------------------

        if (l < 0 ||
            l > n - 1)
        {
            return false;
        }


        // --------------------------------------------------------
        // m debe cumplir:
        //
        //     -l <= m <= +l
        // --------------------------------------------------------

        if (m < -l ||
            m > l)
        {
            return false;
        }


        return true;
    }

}