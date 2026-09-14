#pragma once

#include "NuclearPotential.h"

namespace Quantum
{

    // ============================================================
    // POTENCIAL EFECTIVO
    // ============================================================
    //
    // El electrón no siente únicamente la atracción del núcleo.
    // También existe la repulsión producida por los demás
    // electrones.
    //
    // Por tanto:
    //
    //     V_eff(r) =
    //         V_nuclear(r)
    //         +
    //         V_electron-electron(r)
    //
    // En una primera aproximación utilizaremos un Z efectivo:
    //
    //     V_eff(r) =
    //
    //         - Z_eff e²
    //         ----------------
    //          4π ε₀ r
    //
    // donde Z_eff representa la carga nuclear efectiva que siente
    // el electrón.
    //
    // IMPORTANTE:
    //
    // Esta clase NO resuelve todavía un problema autoconsistente
    // de Hartree-Fock o DFT. Esa complejidad podrá incorporarse
    // posteriormente.
    //
    // ============================================================

    class EffectivePotential
    {
    public:

        // --------------------------------------------------------
        // CONSTRUCTOR
        // --------------------------------------------------------

        explicit EffectivePotential(
            int atomicNumber = 1
        );


        // --------------------------------------------------------
        // CONFIGURACIÓN
        // --------------------------------------------------------

        void setAtomicNumber(
            int atomicNumber
        );

        int getAtomicNumber() const;


        // --------------------------------------------------------
        // APANTALLAMIENTO
        // --------------------------------------------------------

        void setScreeningConstant(
            double screeningConstant
        );

        double getScreeningConstant() const;


        // --------------------------------------------------------
        // CARGA NUCLEAR EFECTIVA
        // --------------------------------------------------------

        double getEffectiveNuclearCharge() const;


        // --------------------------------------------------------
        // POTENCIAL
        // --------------------------------------------------------
        //
        // radiusNm:
        //     distancia al núcleo en nanómetros.
        //
        // devuelve:
        //     energía potencial en eV.
        //
        // --------------------------------------------------------

        double evaluate(
            double radiusNm
        ) const;


        // --------------------------------------------------------
        // CONSTANTE DE COULOMB
        // --------------------------------------------------------

        static constexpr double COULOMB_CONSTANT =
            8.9875517923e9;


        // --------------------------------------------------------
        // CARGA ELEMENTAL
        // --------------------------------------------------------

        static constexpr double ELEMENTARY_CHARGE =
            1.602176634e-19;


    private:

        int atomicNumber = 1;

        double screeningConstant = 0.0;
    };

}