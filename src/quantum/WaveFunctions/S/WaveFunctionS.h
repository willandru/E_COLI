#pragma once

namespace Quantum
{
    class WaveFunctionS
    {
    public:

        // ============================================================
        // CONSTRUCTOR
        // ============================================================

        explicit WaveFunctionS(
            int principalQuantumNumber = 1,
            double nuclearCharge = 1.0
        );


        // ============================================================
        // QUANTUM NUMBERS
        // ============================================================

        void setPrincipalQuantumNumber(int n);

        int getPrincipalQuantumNumber() const;

        int getAngularQuantumNumber() const;

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

        int principalQuantumNumber = 1;

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

        static void validateNuclearCharge(
            double nuclearCharge
        );

        static void validateRadius(
            double radiusNm
        );
    };
}