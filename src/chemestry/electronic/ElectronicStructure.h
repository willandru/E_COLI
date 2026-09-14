#pragma once

#include <string>
#include <vector>

namespace Chemistry
{
    // ============================================================
    // ORBITAL ELECTRÓNICO
    // ============================================================
    //
    // Representa un orbital individual dentro de un subnivel.
    //
    // Ejemplo:
    //
    //     p -> l = 1
    //
    //     m = -1
    //     m =  0
    //     m = +1
    //
    // Cada orbital puede contener como máximo 2 electrones.
    //
    // ============================================================

    struct ElectronicOrbital
    {
        int magneticQuantumNumber = 0;
        int electronCount = 0;
    };


    // ============================================================
    // SUBNIVEL ELECTRÓNICO
    // ============================================================
    //
    // Representa un subnivel s, p, d o f.
    //
    // l = 0 -> s
    // l = 1 -> p
    // l = 2 -> d
    // l = 3 -> f
    //
    // ============================================================

    struct ElectronicSublevel
    {
        int principalQuantumNumber = 0;
        int angularQuantumNumber = 0;

        char type = 's';

        int electronCount = 0;

        std::vector<ElectronicOrbital> orbitals;
    };


    // ============================================================
    // NIVEL ELECTRÓNICO
    // ============================================================
    //
    // Agrupa los subniveles que pertenecen al mismo número
    // cuántico principal n.
    //
    // Ejemplo:
    //
    // n = 2
    //
    //     2s
    //     2p
    //
    // ============================================================

    struct ElectronicLevel
    {
        int principalQuantumNumber = 0;

        std::vector<ElectronicSublevel> sublevels;
    };


    // ============================================================
    // ESTRUCTURA ELECTRÓNICA
    // ============================================================
    //
    // Esta clase describe la distribución electrónica de un átomo
    // o ion.
    //
    // RESPONSABILIDAD:
    //
    //     Número atómico
    //          ↓
    //     carga del ion
    //          ↓
    //     número de electrones
    //          ↓
    //     distribución en subniveles y orbitales
    //
    // NO realiza:
    //
    //     - solución de Schrödinger
    //     - funciones de onda
    //     - densidad electrónica
    //     - probabilidad radial
    //     - potenciales
    //     - geometría 3D
    //     - renderizado
    //
    // ============================================================

    class ElectronicStructure
    {
    public:

        // --------------------------------------------------------
        // CONSTRUCTORES
        // --------------------------------------------------------

        ElectronicStructure();

        explicit ElectronicStructure(
            int atomicNumber,
            int ionCharge = 0
        );


        // --------------------------------------------------------
        // CONFIGURACIÓN
        // --------------------------------------------------------

        void setAtom(
            int atomicNumber,
            int ionCharge = 0
        );

        void calculate();


        // --------------------------------------------------------
        // INFORMACIÓN DEL ÁTOMO
        // --------------------------------------------------------

        int getAtomicNumber() const;

        int getIonCharge() const;

        int getElectronCount() const;


        // --------------------------------------------------------
        // ESTRUCTURA ELECTRÓNICA
        // --------------------------------------------------------

        const std::vector<ElectronicLevel>&
        getLevels() const;

        const std::vector<ElectronicSublevel>&
        getSublevels() const;


        // --------------------------------------------------------
        // CONFIGURACIÓN ELECTRÓNICA
        // --------------------------------------------------------
        //
        // Ejemplo:
        //
        //     1s2 2s2 2p4
        //
        // --------------------------------------------------------

        std::string getConfiguration() const;


        // --------------------------------------------------------
        // UTILIDADES
        // --------------------------------------------------------

        static int orbitalCount(int angularQuantumNumber);

        static int sublevelCapacity(int angularQuantumNumber);

        static char sublevelType(int angularQuantumNumber);


    private:

        // --------------------------------------------------------
        // PROPIEDADES DEL ÁTOMO
        // --------------------------------------------------------

        int atomicNumber = 0;

        int ionCharge = 0;

        int electronCount = 0;


        // --------------------------------------------------------
        // RESULTADO DE LA ESTRUCTURA ELECTRÓNICA
        // --------------------------------------------------------

        std::vector<ElectronicLevel> levels;

        std::vector<ElectronicSublevel> sublevels;


        // --------------------------------------------------------
        // ORDEN DE LLENADO
        // --------------------------------------------------------

        struct FillingOrder
        {
            int n;
            int l;
        };

        static const std::vector<FillingOrder> fillingOrder;


        // --------------------------------------------------------
        // CONSTRUCCIÓN INTERNA
        // --------------------------------------------------------

        void clear();

        void buildStructure();

        void buildLevels();

        ElectronicSublevel createSublevel(
            int n,
            int l
        ) const;

        void distributeElectrons(
            ElectronicSublevel& sublevel,
            int electronCount
        );


        // --------------------------------------------------------
        // VALIDACIÓN
        // --------------------------------------------------------

        static bool isValidAtomicNumber(
            int atomicNumber
        );

        static bool isValidIonCharge(
            int atomicNumber,
            int ionCharge
        );
    };

}