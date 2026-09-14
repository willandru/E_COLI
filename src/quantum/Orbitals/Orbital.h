#pragma once

namespace Quantum
{

    // ============================================================
    // ORBITAL CUÁNTICO
    // ============================================================
    //
    // Representa un orbital definido por sus números cuánticos:
    //
    //     n -> número cuántico principal
    //     l -> número cuántico azimutal
    //     m -> número cuántico magnético
    //
    // También almacena información física que será obtenida o
    // calculada posteriormente por los módulos de quantum.
    //
    // IMPORTANTE:
    //
    // Esta clase NO calcula:
    //
    //     - funciones de onda
    //     - potenciales
    //     - energía mediante Schrödinger
    //     - densidad electrónica
    //     - geometría
    //
    // Es simplemente el objeto que identifica y almacena el estado
    // orbital que los demás módulos utilizarán.
    //
    // ============================================================

    class Orbital
    {
    public:

        // --------------------------------------------------------
        // CONSTRUCTORES
        // --------------------------------------------------------

        Orbital();

        Orbital(
            int principalQuantumNumber,
            int angularQuantumNumber,
            int magneticQuantumNumber
        );


        // --------------------------------------------------------
        // NÚMEROS CUÁNTICOS
        // --------------------------------------------------------

        int getPrincipalQuantumNumber() const;

        int getAngularQuantumNumber() const;

        int getMagneticQuantumNumber() const;


        void setQuantumNumbers(
            int principalQuantumNumber,
            int angularQuantumNumber,
            int magneticQuantumNumber
        );


        // --------------------------------------------------------
        // PROPIEDADES DEL ORBITAL
        // --------------------------------------------------------

        int getRadialNodeCount() const;

        int getAngularNodeCount() const;

        int getTotalNodeCount() const;


        // --------------------------------------------------------
        // ENERGÍA
        // --------------------------------------------------------
        //
        // La energía será calculada posteriormente por los modelos
        // cuánticos. Aquí solamente almacenamos el resultado.
        //
        // --------------------------------------------------------

        double getEnergy() const;

        void setEnergy(double energy);


        // --------------------------------------------------------
        // OCUPACIÓN ELECTRÓNICA
        // --------------------------------------------------------

        int getElectronCount() const;

        void setElectronCount(int electronCount);


        // --------------------------------------------------------
        // PROPIEDADES DEL SUBNIVEL
        // --------------------------------------------------------

        char getType() const;

        int getOrbitalCount() const;

        int getMaximumElectronCount() const;


        // --------------------------------------------------------
        // VALIDACIÓN
        // --------------------------------------------------------

        static bool isValidQuantumNumbers(
            int n,
            int l,
            int m
        );


    private:

        // --------------------------------------------------------
        // NÚMEROS CUÁNTICOS
        // --------------------------------------------------------

        int principalQuantumNumber = 1;

        int angularQuantumNumber = 0;

        int magneticQuantumNumber = 0;


        // --------------------------------------------------------
        // PROPIEDADES FÍSICAS
        // --------------------------------------------------------

        double energy = 0.0;


        // --------------------------------------------------------
        // OCUPACIÓN
        // --------------------------------------------------------

        int electronCount = 0;
    };

}