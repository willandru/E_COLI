#include "NuclearPotential.h"

#include <cmath>
#include <stdexcept>


namespace Quantum
{

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    NuclearPotential::NuclearPotential(
        int atomicNumber
    )
    {
        setAtomicNumber(
            atomicNumber
        );
    }


    // ============================================================
    // SET ATOMIC NUMBER
    // ============================================================

    void NuclearPotential::setAtomicNumber(
        int atomicNumber
    )
    {
        if (!isValidAtomicNumber(atomicNumber))
        {
            throw std::invalid_argument(
                "Atomic number must be between 1 and 118."
            );
        }

        this->atomicNumber =
            atomicNumber;
    }


    // ============================================================
    // GET ATOMIC NUMBER
    // ============================================================

    int NuclearPotential::getAtomicNumber() const
    {
        return atomicNumber;
    }


    // ============================================================
    // EVALUATE
    // ============================================================
    //
    // Calcula:
    //
    //             -Z e²
    // V(r) = -------------------
    //          4π ε₀ r
    //
    // utilizando:
    //
    //     r -> nm
    //
    // y devuelve:
    //
    //     V -> eV
    //
    // ============================================================

    double NuclearPotential::evaluate(
        double radiusNm
    ) const
    {
        if (radiusNm <= 0.0)
        {
            throw std::invalid_argument(
                "Radius must be greater than zero."
            );
        }


        // --------------------------------------------------------
        // Conversión:
        //
        // nm -> m
        // --------------------------------------------------------

        constexpr double NANOMETER_TO_METER =
            1.0e-9;


        const double radiusMeters =
            radiusNm *
            NANOMETER_TO_METER;


        // --------------------------------------------------------
        // Carga del electrón.
        //
        // El electrón tiene carga -e.
        //
        // El núcleo tiene carga +Ze.
        //
        // El producto de las cargas es:
        //
        //     (+Ze)(-e) = -Ze²
        //
        // --------------------------------------------------------

        const double electronCharge =
            -ELEMENTARY_CHARGE;


        const double nuclearCharge =
            static_cast<double>(atomicNumber) *
            ELEMENTARY_CHARGE;


        // --------------------------------------------------------
        // Potencial electrostático en Joules.
        // --------------------------------------------------------

        const double potentialJoules =
            COULOMB_CONSTANT *
            nuclearCharge *
            electronCharge /
            radiusMeters;


        // --------------------------------------------------------
        // Joules -> eV
        // --------------------------------------------------------

        const double potentialElectronVolts =
            potentialJoules /
            ELEMENTARY_CHARGE;


        return potentialElectronVolts;
    }


    // ============================================================
    // VALIDATE ATOMIC NUMBER
    // ============================================================

    bool NuclearPotential::isValidAtomicNumber(
        int atomicNumber
    )
    {
        return
            atomicNumber >= 1 &&
            atomicNumber <= 118;
    }

}