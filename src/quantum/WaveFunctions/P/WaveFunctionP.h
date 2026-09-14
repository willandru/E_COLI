#pragma once

namespace Quantum
{
    class WaveFunctionP
    {
    public:

        // ============================================================
        // CONSTRUCTOR
        // ============================================================

        explicit WaveFunctionP(
            int principalQuantumNumber = 2,
            int magneticQuantumNumber = 0,
            double nuclearCharge = 1.0
        );


        // ============================================================
        // QUANTUM NUMBERS
        // ============================================================

        void setPrincipalQuantumNumber(int n);

        int getPrincipalQuantumNumber() const;

        int getAngularQuantumNumber() const;


        void setMagneticQuantumNumber(int m);

        int getMagneticQuantumNumber() const;


        char getType() const;


        // ============================================================
        // EFFECTIVE NUCLEAR CHARGE
        // ============================================================

        void setNuclearCharge(double nuclearCharge);

        double getNuclearCharge() const;


        // ============================================================
        // WAVE FUNCTION
        // ============================================================

        double evaluateRadial(
            double radiusNm
        ) const;


        double evaluateAngular(
            double theta,
            double phi
        ) const;


        double evaluate(
            double radiusNm,
            double theta,
            double phi
        ) const;


        // ============================================================
        // NODES
        // ============================================================

        int getRadialNodeCount() const;

        int getAngularNodeCount() const;

        int getTotalNodeCount() const;


        // ============================================================
        // CONSTANTS
        // ============================================================

        static constexpr double BOHR_RADIUS_NM =
            0.0529177210903;


    private:

        int principalQuantumNumber = 2;

        int magneticQuantumNumber = 0;

        double nuclearCharge = 1.0;


        // ============================================================
        // ASSOCIATED LAGUERRE POLYNOMIAL
        // ============================================================

        static double associatedLaguerre(
            int polynomialOrder,
            int alpha,
            double x
        );


        // ============================================================
        // VALIDATION
        // ============================================================

        static void validatePrincipalQuantumNumber(
            int n
        );

        static void validateMagneticQuantumNumber(
            int m
        );

        static void validateNuclearCharge(
            double nuclearCharge
        );

        static void validateRadius(
            double radiusNm
        );
    };
}