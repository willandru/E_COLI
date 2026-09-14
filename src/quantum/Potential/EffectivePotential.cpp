#include "EffectivePotential.h"

#include <stdexcept>


namespace Quantum
{

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    EffectivePotential::EffectivePotential(
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

    void EffectivePotential::setAtomicNumber(
        int atomicNumber
    )
    {
        if (atomicNumber < 1 ||
            atomicNumber > 118)
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

    int EffectivePotential::getAtomicNumber() const
    {
        return atomicNumber;
    }


    // ============================================================
    // SET SCREENING CONSTANT
    // ============================================================
    //
    // σ representa, en esta aproximación, cuánto se reduce la
    // carga nuclear debido al apantallamiento.
    //
    //     Z_eff = Z - σ
    //
    // ============================================================

    void EffectivePotential::setScreeningConstant(
        double screeningConstant
    )
    {
        if (screeningConstant < 0.0)
        {
            throw std::invalid_argument(
                "Screening constant cannot be negative."
            );
        }


        if (screeningConstant >=
            static_cast<double>(atomicNumber))
        {
            throw std::invalid_argument(
                "Screening constant must be smaller than atomic number."
            );
        }


        this->screeningConstant =
            screeningConstant;
    }


    // ============================================================
    // GET SCREENING CONSTANT
    // ============================================================

    double EffectivePotential::getScreeningConstant() const
    {
        return screeningConstant;
    }


    // ============================================================
    // GET EFFECTIVE NUCLEAR CHARGE
    // ============================================================

    double EffectivePotential::getEffectiveNuclearCharge() const
    {
        return
            static_cast<double>(atomicNumber)
            - screeningConstant;
    }


    // ============================================================
    // EVALUATE
    // ============================================================
    //
    // Calcula:
    //
    //             -Z_eff e²
    // V(r) = -------------------
    //          4π ε₀ r
    //
    // usando:
    //
    //     r -> nm
    //     V -> eV
    //
    // ============================================================

    double EffectivePotential::evaluate(
        double radiusNm
    ) const
    {
        if (radiusNm <= 0.0)
        {
            throw std::invalid_argument(
                "Radius must be greater than zero."
            );
        }


        constexpr double NANOMETER_TO_METER =
            1.0e-9;


        const double radiusMeters =
            radiusNm *
            NANOMETER_TO_METER;


        const double effectiveNuclearCharge =
            getEffectiveNuclearCharge();


        // --------------------------------------------------------
        // Carga del núcleo efectiva:
        //
        //     +Z_eff e
        //
        // Carga del electrón:
        //
        //     -e
        //
        // --------------------------------------------------------

        const double effectiveNuclearChargeCoulombs =
            effectiveNuclearCharge *
            ELEMENTARY_CHARGE;


        const double electronCharge =
            -ELEMENTARY_CHARGE;


        // --------------------------------------------------------
        // Potencial electrostático en Joules.
        // --------------------------------------------------------

        const double potentialJoules =
            COULOMB_CONSTANT *
            effectiveNuclearChargeCoulombs *
            electronCharge /
            radiusMeters;


        // --------------------------------------------------------
        // Joules -> eV
        // --------------------------------------------------------

        return
            potentialJoules /
            ELEMENTARY_CHARGE;
    }

}