#pragma once

namespace Quantum
{

    // ============================================================
    // POTENCIAL NUCLEAR
    // ============================================================
    //
    // Describe el potencial electrostático producido por un núcleo
    // de carga +Ze sobre un electrón de carga -e.
    //
    // Para un núcleo puntual:
    //
    //              -Z e²
    // V(r) = -------------------
    //          4π ε₀ r
    //
    // El potencial es negativo porque la interacción electrón-
    // núcleo es atractiva.
    //
    // UNIDADES:
    //
    //     distancia : nanómetros
    //     energía   : electronvoltios (eV)
    //
    // Esto permite mantener una representación física cómoda para
    // el proyecto y posteriormente convertir la escala a OpenGL.
    //
    // ============================================================

    class NuclearPotential
    {
    public:

        // --------------------------------------------------------
        // CONSTRUCTOR
        // --------------------------------------------------------

        explicit NuclearPotential(
            int atomicNumber = 1
        );


        // --------------------------------------------------------
        // NÚMERO ATÓMICO
        // --------------------------------------------------------

        void setAtomicNumber(
            int atomicNumber
        );

        int getAtomicNumber() const;


        // --------------------------------------------------------
        // POTENCIAL
        // --------------------------------------------------------
        //
        // r se expresa en nanómetros.
        //
        // Devuelve V(r) en eV.
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


        // --------------------------------------------------------
        // VALIDACIÓN
        // --------------------------------------------------------

        static bool isValidAtomicNumber(
            int atomicNumber
        );


    private:

        int atomicNumber = 1;
    };

}